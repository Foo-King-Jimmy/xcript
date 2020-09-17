

// WILL APPEND the output w a terminating slash ALWAYS !!!

dirname_ ( const char * pathstr, char **retname ) {

 const char * tmppf = NULL,
            * tmppl = NULL,
            * tmppx = NULL;
 char * tmppy;
 int  foldedslashcount = 0,
      noslashcharcount = 0,
      lastcharwasslash = 0,
      it_is_root       = 0;
 size_t  length  = (size_t) 0,
         malsize;

  if( !retname )  return 11;
  *retname = NULL;
  if( !pathstr )  return 12;
  if( !*pathstr ) return 13;

  length = strlen(  pathstr );        // min 1
  tmppf  = strchr(  pathstr, '/' );
  tmppl  = strrchr( pathstr, '/' );
  tmppx  = pathstr;
  while( *tmppx ) {
    if( '/' == *tmppx ) {
      if( !lastcharwasslash ) {
        ++foldedslashcount;
        lastcharwasslash = 1;
      }
    }
    else {
      ++noslashcharcount;
      lastcharwasslash = 0;
    }
    ++tmppx;
  }

  if(      ( !noslashcharcount )  \
       ||  ( 1 == foldedslashcount  &&  tmppf == pathstr )  \
       ||  ( 2 == foldedslashcount  &&  tmppf == pathstr  &&  tmppl == &pathstr[ length - 1 ] )
    ) it_is_root = 1;
  if( !it_is_root ) {
    if(      ( !foldedslashcount )  \
         ||  ( 1 == foldedslashcount  &&  tmppl == &pathstr[ length - 1 ] )
      ) {
      E( malloc_( (size_t) 3, retname ), in dirname_ malloc_ 2 failed );
      if( $errno ) return 15;
      (void) memcpy( *retname, "./", (size_t) 2 );
      return 0;
    }
    // else
    // IF (folded) slash is     last char: [ONLY THIS POSSIBLE BY NOW:]  aa//bb//   ...
    // IF (folded) slash is NOT last char: [ONLY THIS POSSIBLE BY NOW:]  aa//bb     ...
    //   aa//bb//  =>  aa/   >> -1_fs -?_b
    //   aa//bb    =>  aa/   >>       -?_b
    malsize = (size_t)( 1 + foldedslashcount + noslashcharcount );
    tmppx = &pathstr[ length - 1 ];
    if( tmppl == tmppx ) {
      --malsize;
      while( '/' == *--tmppx ) { }
    }
    //   aa//bb//    aa//bb
    //        *           *   <== tmppx
    // let's count ending no-slash-chars  ( let's count bs ):
    lastcharwasslash = 1;   // REUSING this var
    while( '/' != *--tmppx ) ++lastcharwasslash;
    //   aa//bb//    aa//bb
    //      *           *     <== tmppx
    malsize -= lastcharwasslash;
    E( malloc_( malsize, retname ), in dirname_ malloc_ 3 failed );
    if( $errno ) return 16;
    // let's write output string REUSING ALL VARS:
    // tmppx OK it is the doorstop
    tmppf = pathstr;
    lastcharwasslash = 0;
    tmppy = *retname;
    while( tmppf <= tmppx ) {
      if( '/' == *tmppf ) {
        if( !lastcharwasslash ) {
          *tmppy = *tmppf;
          ++tmppy;
          lastcharwasslash = 1;
        }
      }
      else {
        *tmppy = *tmppf;
        ++tmppy;
        lastcharwasslash = 0;
      }
      ++tmppf;
    }
  }
  if( it_is_root ) {
    E( malloc_( (size_t) 2, retname ), in dirname_ malloc_ 1 failed );
    if( $errno ) return 14;
    **retname = '/';
  }

 return 0;
}


chdir_ ( const char * path ) {

 char * tmpp;

  if( !path )  return 11;
  if( !*path ) return 12;
  if( -1 == chdir( path )) return 13;
  if( !$pwd ) {
    E( malloc_( (size_t)( 1 + ( PATH_MAX )), &$pwd ), in chdir_ malloc_ failed );
    if( $errno ) return 14;
  }
  $pwd = getcwd( $pwd, (size_t)( PATH_MAX ));
  if( !$pwd )  return 15;   // $pwd (buffer) is corrupted/has undefined value
  if( '(' == *$pwd ) {   // "(unreachable)"
    *$pwd = '\0';   // not passing that back
    return 16;
  }
  // append w terminating slash if needed
  tmpp = $pwd;
  if( !*tmpp ) return 17;
  else {
    while( *tmpp ) ++tmpp;
    if( '/' != tmpp[-1] ) {
      *tmpp   = '/';
      *++tmpp = '\0';
    }
  }

 return 0;
}
cd_ ( const char * path ) { return chdir_( path ); }


getpid_ ( void ) {
  $pid  = getpid();
  $ppid = getppid();
 return 0;
}
getppid_ ( void ) { return getpid_(); }


pwd_ ( char **pwdstrp ) {

 char * tmpp;

  if( !pwdstrp ) return 11;
  *pwdstrp = NULL;
  E( malloc_( (size_t)( 1 + ( PATH_MAX )), pwdstrp ), in pwd_ malloc_ failed );
  if( $errno ) return 12;
  *pwdstrp = getcwd( *pwdstrp, (size_t)( PATH_MAX ));
  if( !*pwdstrp ) return 13;   // buffer is corrupted/has undefined value
  if( '(' == **pwdstrp ) {   // "(unreachable)"
    **pwdstrp = '\0';   // ? not passing back "(unreachable)" string
    return 14;
  }
  // append the string w terminating slash if needed (its a directory after all)
  tmpp = *pwdstrp;
  if( !*tmpp ) return 15;
  else {
    while( *tmpp ) ++tmpp;
    if( '/' != tmpp[-1] ) {
      *tmpp   = '/';
      *++tmpp = '\0';
    }
  }

 return 0;
}
cwd_ ( char **cwdstrp ) {
  return pwd_( cwdstrp );
}


sleep_force_ ( int secs ) {
  if(    !secs ) return  0;
  if( 0 > secs ) return 11;
  while( secs ) secs = (int) sleep( (unsigned int) secs );
 return 0;
}

sleep_ret_ ( int secs, int * retsecs ) {
  if(    !secs ) return  0;
  if( 0 > secs ) return 11;
  *retsecs = (int) sleep( (unsigned int) secs );
  // if( *retsecs ) return 12;   // interrupted by a signal
 return 0;
}

sleep_void_ ( int secs ) {
  if(    !secs ) return  0;
  if( 0 > secs ) return 11;
  (void) sleep( (unsigned int) secs );
 return 0;
}
sleep_ ( int secs ) { return sleep_void_( secs ); }


////// touch_globb_ ( const char * fnamepattern ) {   // ? cannot be a directory, only files

touch_ ( const char * fname ) {   // simple touch
 FILE * fp = NULL;
  if(  !fname ) return 11;
  if( !*fname ) return 12;
  fp = fopen( fname, "ab" );
  if( !fp ) return 13;
  (void) fclose( fp );
 return 0;
}


symlink_ ( const char * target, const char * linkpath ) { return symlink( target, linkpath ); }
ln_s_    ( const char * target, const char * linkpath ) { return symlink( target, linkpath ); }

symlink_re_ ( const char * target, const char * linkpath ) {
  // -f, --force remove existing destination files
  // -n, --no-dereference treat LINK_NAME as a normal file if it is a symbolic link to a directory
  (void) unlink( linkpath );
  return symlink( target, linkpath );
}
ln_sfn_ ( const char * target, const char * linkpath ) { return ln_sfn_( target, linkpath ); }


basename_( const char * name, char **basenamep ) {
  //       /f   == f
  //        f   == f
  //  d1/d2/f   == f
  //       /d   == d
  //       /d/  == d
  //        d   == d
  //        d/  == d
  //  d1/d2/d   == d
  //  d1/d2/d/  == d
  //         /  == /
  //       ///  == /
  //      /d//  == d

 size_t  tmpsizet;
 int  that_is_root = 0;
 const char * lastp,
            * frstp;

  if( !basenamep ) return 11;
  *basenamep = NULL;
  if(  !name )     return 12;
  if( !*name )     return 13;
  tmpsizet = strlen( name );
  lastp = &name[ tmpsizet - 1 ];
  if( '/' != *lastp ) {
    frstp = strrchr( name, '/' );
    if( !frstp ) frstp = name;
    else ++frstp;
    tmpsizet = (size_t)( lastp + 2 - frstp );
    E( malloc_( tmpsizet, basenamep ), in basename_ malloc_ 1 failed );
    if( $errno ) return 14;
    (void) strcpy( *basenamep, frstp );
  }
  else {   // last char is a slash
    frstp = strchr( name, '/' );
    if( frstp == lastp ) {   // there is only 1 slash inside (the last char)
      if( (size_t) 1 == tmpsizet ) ++that_is_root;
      else {
        frstp = name;
        E( malloc_( tmpsizet, basenamep ), in basename_ malloc_ 2 failed );
        if( $errno ) return 15;
        --tmpsizet;
        (void) memcpy( *basenamep, frstp, tmpsizet );
      }
    }
    else {   // two or more slashes inside
      if( tmpsizet == strspn( name, "/" )) ++that_is_root;   // only slashes inside
      else {   // we have a not-slash character
        while( '/' == *lastp ) --lastp;   // move lastp
        ++lastp;
        if( frstp == lastp ) {
          frstp = name;
          tmpsizet = (size_t)( lastp + 1 - frstp );
          E( malloc_( tmpsizet, basenamep ), in basename_ malloc_ 4 failed );
          if( $errno ) return 17;
          --tmpsizet;
          (void) memcpy( *basenamep, frstp, tmpsizet );
        }
        else {
          frstp = lastp;
          while( '/' != *--frstp ) { }
          ++frstp;
          tmpsizet = (size_t)( lastp + 1 - frstp );
          E( malloc_( tmpsizet, basenamep ), in basename_ malloc_ 5 failed );
          if( $errno ) return 18;
          --tmpsizet;
          (void) memcpy( *basenamep, frstp, tmpsizet );
        }
      }
    }
    if( that_is_root ) {
      E( malloc_( (size_t) 2, basenamep ), in basename_ malloc_ 3 failed );
      if( $errno ) return 16;
      (void) strcpy( *basenamep, "/" );
    }
  }

 return 0;
}


// rmdir file  =>  error (a file)
// rmdir  dir  =>  ok, let's try to rmdir it
// rmdir globb pattern  =>  ... same ...  files:error dirs:try  ? force/ignore error on ...

rmdir_ ( const char * dirpath ) {
  E( rmdir( dirpath ), in rmdir_ wrapper rmdir syscall failed );
 return $errno;
}

// rmdir_globb_      (   ) { }
// rmdir_globb_cont_ (   ) { }   // cont(inue) == --ignore-fail-on-non-empty ( man 1 rmdir )


/* If PID  >  0, then signal is sent to the process with the ID specified by PID.
   If PID ==  0, then signal is sent to every process in the process group of the calling process.
   If PID  < -1, then signal is sent to every process in the process group whose ID is -PID.
   If PID == -1, then signal is sent to every process for which the calling process has permission
                 to send signals, except for process 1 (init). */

kill_ ( pid_t procid, int signalnum ) {
  E( kill( procid, signalnum ), in kill_ wrapper kill syscall failed );
 return $errno;
}

