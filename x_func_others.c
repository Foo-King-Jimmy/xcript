

cd_ ( const char * path ) {
  return chdir( path );
}


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
  if( !*pwdstrp ) {
    **pwdstrp = '\0';   // otherwise buffer is corrupted/has undefined value
    return 13;
  }
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

