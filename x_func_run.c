

// ADD redirection of stdin, stdout, stderr


#define   XCRPT_WAIT   (0)
#define   XCRPT_INBG   (1)


/* static */   // seriously static because of the 'va_list myap'
run_raw_ ( int nowait, int * retexitcode, va_list myap, const char * cmdfmt ) {

 siginfo_t  mysiginfot;
 int  retprn,
      idxj      = 0,
      childargc = 1;   // min argc (==1)
 pid_t  childpid;
 char **childargv = NULL;
 char * tmpp,
      * runpath = NULL,
      * command = NULL,
        testbuf[ 2 ];

  // CHECK PARAMS:
  if( retexitcode ) *retexitcode = 0;
  if( !cmdfmt  ) return 11;
  if( !*cmdfmt ) return 12;

  retprn = vsnprintf( testbuf, (size_t) 2, cmdfmt, myap );
  if( 1 > retprn ) return 13;
  ++retprn;
  // OLD: Es( malloc_noadd_((size_t) retprn, &command), in run_raw_ malloc_noadd_ failed on,cmdfmt);
  // OLD: if( $errno ) return 14;
  command = alloca( (size_t) retprn );
  retprn = vsprintf( command, cmdfmt, myap );
  if( 1 > retprn ) {
    // OLD: free( command );
    return 15;
  }

  (void) memset( &mysiginfot, 0, sizeof(siginfo_t) );

  // BUILD argv LIST FROM command:
  // NOTE: this section is placed here, before the fork() because
  //       if malloc_() (down) fails then its easier to know and treat
  tmpp = command;
  while( 1 ) {
    if( !*tmpp ) {
      // OLD: free( command );
      return 16;
    }
    if( ' ' == *tmpp || '\t' == *tmpp ) ++tmpp;
    else break;
  }
  runpath = tmpp;   // argv[0] 1==argc
  while( 1 ) {
    if( !*++tmpp ) break;
    if( ' ' == *tmpp || '\t' == *tmpp ) break;
  }
  --tmpp;
  while( *++tmpp ) {
    if( ' ' == *tmpp || '\t' == *tmpp ) *tmpp = '\0';
    else {
      ++childargc;
      while( 1 ) {
        if( !*++tmpp ) break;
        if( ' ' == *tmpp || '\t' == *tmpp ) break;
      }
      --tmpp;
    }
  }
  // OLD: E(malloc_((size_t)((1+childargc) * sizeof(char*)),&childargv),in run_raw_ malloc_ failed);
  // OLD: if( $errno ) { free( command ); return 17; }
  childargv = alloca( (size_t)(( 1 + childargc ) * sizeof(char *) ));
  tmpp = runpath;
  for( ; idxj < childargc; ++idxj ) {
    while( !*tmpp ) ++tmpp;
    childargv[idxj] = tmpp;
    while( *++tmpp ) { }
  }
  childargv[idxj] = NULL;

  // FORK, ERROR-CHECK:
  childpid = fork();
  if( 0 > childpid ) {
    // OLD: free( command );
    return 18;
  }

  // CHILD: EXECVE:
  if( !childpid ) {   // child process
    E( execve( runpath, childargv, NULL ), in run_raw_ execve failed after fork );
    $pid = getpid();
    // E( kill_( $pid, SIGKILL ), in run_raw_ kill_ failed after fork and failed execve );
    E( kill( $pid, SIGKILL ), in run_raw_ kill failed after fork and failed execve );
    // OLD: free( command );
    return 19;   // should never actually happen
  }

  // else   // parent process
  // IF NO NEED TO WAIT THEN END IT HERE:
  // ? ignore SIGCHILD in order not to let proc-table to fill up ?
  if( nowait ) {   // XCRPT_WAIT (0), XCRPT_INBG (1)
    // OLD: free( command );
    return 0;
  }

  // PARENT: WAITPID CHILDPID:
  E(  waitid( (idtype_t) P_PID, (id_t) childpid, &mysiginfot, WEXITED ), in run_raw_ waitid failed);
  if( $errno ) {
    // OLD: free( command );
    return 20;
  }
  if( childpid != mysiginfot.si_pid ) {   // wtf #1 error
    // OLD: free( command );
    return 21;
  }
  // si_uid == The real user ID of the child.
  if( CLD_KILLED == mysiginfot.si_code ) {   // child killed by a signal
    // OLD: free( command );
    return 22;
  }
  if( CLD_DUMPED == mysiginfot.si_code ) {   // child killed by a signal (and dumped core)
    // OLD: free( command );
    return 23;
  }
  if( CLD_EXITED == mysiginfot.si_code ) {   // ok; terminated by _exit(2), exit(3), from main()
    if( retexitcode ) *retexitcode = mysiginfot.si_status;
    // OLD: free( command );
    return 0;
  }
  // OLD: free( command );

 return 24;   // impossible, wtf #2 error
}



run_bg_ ( const char * cmdline, ... ) {
 int  retrun;
 va_list  ap;
  va_start( ap, cmdline );
  retrun = run_raw_( XCRPT_INBG, NULL, ap, cmdline );
  va_end( ap );
 return retrun;
}
run_inbg_ ( const char * cmdline, ... ) {
 int  retrun;
 va_list  ap;
  va_start( ap, cmdline );
  retrun = run_raw_( XCRPT_INBG, NULL, ap, cmdline );
  va_end( ap );
 return retrun;
}
run_nowait_ ( const char * cmdline, ... ) {
 int  retrun;
 va_list  ap;
  va_start( ap, cmdline );
  retrun = run_raw_( XCRPT_INBG, NULL, ap, cmdline );
  va_end( ap );
 return retrun;
}


run_ ( const char * cmdline, ... ) {
 int  retrun;
 va_list  ap;
  va_start( ap, cmdline );
  retrun = run_raw_( XCRPT_WAIT, NULL, ap, cmdline );
  va_end( ap );
 return retrun;
}
run_wait_ ( const char * cmdline, ... ) {
 int  retrun;
 va_list  ap;
  va_start( ap, cmdline );
  retrun = run_raw_( XCRPT_WAIT, NULL, ap, cmdline );
  va_end( ap );
 return retrun;
}


run_ret_ ( int * retcode, const char * cmdline, ... ) {
 int  retrun;
 va_list  ap;
  va_start( ap, cmdline );
  retrun = run_raw_( XCRPT_WAIT, retcode, ap, cmdline );
  va_end( ap );
 return retrun;
}
run_wait_ret_ ( int * retcode, const char * cmdline, ... ) {
 int  retrun;
 va_list  ap;
  va_start( ap, cmdline );
  retrun = run_raw_( XCRPT_WAIT, retcode, ap, cmdline );
  va_end( ap );
 return retrun;
}

