

#ifdef    E
#error    "PROBLEM: the macro symbol \'E\' is already defined."
#endif
#ifdef    E0
#error    "PROBLEM: the macro symbol \'E0\' is already defined."
#endif


#define   E(func,msg)         $errno = func; if( $errno ) errdisp_( __FILE__, __LINE__, #msg, NULL )
#define   E0(func,msg,xstr)   $errno = func; if( $errno ) errdisp_( __FILE__, __LINE__, #msg, xstr )


errdisp_( const char * srcfname, int linenum, const char * msg, const char * xstr ) {

 FILE * fh;
 int  prevline = 0,
      myerrno  = errno,
      acharx;

  if( !stderr ) return 11;

  (void) fprintf( stderr, "+--ERROR-REPORT------------------------\n" );
  (void) fprintf( stderr, "|  line:   %d\n", linenum );
  (void) fprintf( stderr, "|  file:   %s\n", srcfname );
  (void) fprintf( stderr, "|  msg:   %s\n", msg );
  (void) fprintf( stderr, "|  $argc:   %d\n", $argc );
  (void) fprintf( stderr, "|  $argv[0]:   %s\n", $argv[0] );
  (void) fprintf( stderr, "|  errno:   %d\n", myerrno );
  (void) fprintf( stderr, "|  strerror(errno):   %s\n", strerror( myerrno ));
  (void) fprintf( stderr, "|  $errno:   %d\n", $errno );           if( xstr ) \
  (void) fprintf( stderr, "|  xstr:   %s\n", xstr );
  (void) fprintf( stderr, "+--------------------------------------\n" );

  if( 1 > linenum ) return 12;
  if( !( fh = fopen( srcfname, "rb" ))) return 13;

  if( 3 < linenum ) prevline = linenum - 3;
  else              prevline = 1;
  for( ; ; ) {
    --linenum;
    if( 1 > --prevline ) break;
    while( EOF != ( acharx = fgetc( fh ))) {
      if( '\n' == acharx ) break;
    }
    if( EOF == acharx ) break;
  }
  if( EOF != acharx ) {
    //  prevline 0  linenum 2  =>  -2  x.......
    //  prevline 0  linenum 1  =>  -1  x.......
    //  prevline 0  linenum 0  =>  >>  x.......
    for( ; prevline <= linenum; --linenum ) {
      if( prevline != linenum ) (void) fprintf( stderr, "-%d  ", linenum );
      else                      (void) fprintf( stderr, ">>  " );
      while( EOF != ( acharx = fgetc( fh ))) {
        (void) fputc( acharx, stderr );
        if( '\n' == acharx ) break;
      }
      if( EOF == acharx ) break;
    }
    if( EOF != acharx ) {
      (void) fprintf( stderr, "+1  " );
      while( EOF != ( acharx = fgetc( fh ))) {
        (void) fputc( acharx, stderr );
        if( '\n' == acharx ) break;
      }
      if( EOF != acharx ) {
        (void) fprintf( stderr, "+2  " );
        while( EOF != ( acharx = fgetc( fh ))) {
          (void) fputc( acharx, stderr );
          if( '\n' == acharx ) break;
        }
      }
    }
  }

  if( EOF == acharx ) (void) fputc( '\n', stderr );
  (void) fclose( fh );
  (void) fprintf( stderr, "+--------------------------------------\n" );

 return 0;  // ? is zero okey OR something non-zero ?
}

