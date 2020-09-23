
////// STORE run process return value in a var (on wait) => waitid() usage !
////// PLACE this (later) into a separate func file
////// ADD   redirection of stdin, stdout, stderr


#define   XCRPT_INBG   (0)
#define   XCRPT_WAIT   (1)


run_ifwait_ ( char * cmdline, int ifwaitflag ) {

 pid_t  childpid,
        retpid;
 int  idxj      = 0,
      childargc = 1,   // min argc (==1)
      mywstatus;
 char **childargv = NULL;
 char * tmpp,
      * runpath = NULL;

  // CHECK PARAMS:
  if( !cmdline  ) return 11;
  if( !*cmdline ) return 12;
  // BUILD argv LIST FROM cmdline:
  // NOTE: this section is placed here, before the fork() because
  //       if malloc_() (down) fails then its easier to know and treat
  tmpp = cmdline;
  while( 1 ) {
    if( !*tmpp ) return 13;
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
  E( malloc_( (size_t)(( 1 + childargc ) * sizeof(char*)), &childargv ), in run_ malloc_ failed );
  if( $errno ) return 14;
  tmpp = runpath;
  for( ; idxj < childargc; ++idxj ) {
    while( !*tmpp ) ++tmpp;
    childargv[idxj] = tmpp;
    while( *++tmpp ) { }
  }
  childargv[idxj] = NULL;
  // FORK, ERROR-CHECK:
  childpid = fork();
  if( 0 > childpid ) return 15;
  // CHILD: EXECVE:
  if( !childpid ) {   // child process
    E( execve( runpath, childargv, NULL ), in run_ execve failed after fork );
    $pid = getpid();
    // E( kill_( $pid, SIGKILL ), in run_ kill_ failed after fork and failed execve );
    E( kill( $pid, SIGKILL ), in run_ kill failed after fork and failed execve );
    return 16;   // should never actually happen
  }
  // else   // parent process
  // IF NO NEED TO WAIT THEN END IT HERE:
  if( !ifwaitflag ) return 0;    // XCRPT_WAIT (1), XCRPT_INBG (0)
  // PARENT: WAITPID CHILDPID:
  if( -1 == ( retpid = waitpid( childpid, &mywstatus, 0 ))) {
    E( 17, in run_ waitpid failed badly ); return 17;
  }
  if( childpid != retpid )      return 18;   // wtf #1 error
  if( WIFSIGNALED( mywstatus )) return 19;   // child terminated by a signal
  if( WIFEXITED(   mywstatus )) return  0;   // ok; terminated by _exit(2), exit(3), from main()

 return 20;   // impossible, wtf #2 error
}


run_      ( char * cmdline ) { return run_ifwait_( cmdline, XCRPT_WAIT ); }
run_wait_ ( char * cmdline ) { return run_ifwait_( cmdline, XCRPT_WAIT ); }
run_inbg_ ( char * cmdline ) { return run_ifwait_( cmdline, XCRPT_INBG ); }
run_bg_   ( char * cmdline ) { return run_ifwait_( cmdline, XCRPT_INBG ); }

