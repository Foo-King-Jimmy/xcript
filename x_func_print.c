

fprintf_ret_ ( int * retprnt, FILE * stream, const char * format, ... ) {

 va_list  myargs;

  if( !retprnt ) return 11;
  *retprnt = -1;
  if( !stream )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  *retprnt = vfprintf( stream, format, myargs );
  va_end( myargs );
  if( 0 > *retprnt ) return 14;   // In case of an output error a negative value returned

 return 0;
}

fprintf_void_ ( FILE * stream, const char * format, ... ) {
 va_list  myargs;
  if( !stream )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  (void) vfprintf( stream, format, myargs );
  va_end( myargs );
 return 0;
}
fprintf_ ( FILE * stream, const char * format, ... ) {
 va_list  myargs;
  if( !stream )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  (void) vfprintf( stream, format, myargs );
  va_end( myargs );
 return 0;
}


printf_ret_ ( int * retprnt, const char * format, ... ) {
 va_list  myargs;
  if( !retprnt ) return 11;
  *retprnt = -1;
  if( !stdout )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  *retprnt = vfprintf( stdout, format, myargs );
  va_end( myargs );
  if( 0 > *retprnt ) return 14;
 return 0;
}

printf_void_ ( const char * format, ... ) {
 va_list  myargs;
  if( !stdout )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  (void) vfprintf( stdout, format, myargs );
  va_end( myargs );
 return 0;
}
printf_ ( const char * format, ... ) {
 va_list  myargs;
  if( !stdout )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  (void) vfprintf( stdout, format, myargs );
  va_end( myargs );
 return 0;
}


snprintf_ret_ ( int * retprnt, char * strbuf, size_t bufsize, const char * format, ... ) {

 va_list  myargs;

  if( !retprnt ) return 11;
  *retprnt = -1;
  if( !strbuf )  return 12;
  if( !format )  return 13;
  if( !bufsize ) return 15;
  va_start( myargs, format );
  *retprnt = vsnprintf( strbuf, bufsize, format, myargs );
  va_end( myargs );
  if( 0 > *retprnt ) return 14;   // In case of an output error a negative value returned
  if( bufsize <= (size_t) *retprnt ) return 16;   // Truncated output

 return 0;
}

snprintf_void_ ( char * strbuf, size_t bufsize, const char * format, ... ) {
 va_list  myargs;
 int  retprnt = -1;
  if( !strbuf )  return 12;
  if( !format )  return 13;
  if( !bufsize ) return 15;
  va_start( myargs, format );
  retprnt = vsnprintf( strbuf, bufsize, format, myargs );
  va_end( myargs );
  if( 0 > retprnt ) return 14;
  if( bufsize <= (size_t) retprnt ) return 16;
 return 0;
}
snprintf_ ( char * strbuf, size_t bufsize, const char * format, ... ) {
 va_list  myargs;
 int  retprnt = -1;
  if( !strbuf )  return 12;
  if( !format )  return 13;
  if( !bufsize ) return 15;
  va_start( myargs, format );
  retprnt = vsnprintf( strbuf, bufsize, format, myargs );
  va_end( myargs );
  if( 0 > retprnt ) return 14;
  if( bufsize <= (size_t) retprnt ) return 16;
 return 0;
}


sprintf_ret_ ( int * retprnt, char * strbuf, const char * format, ... ) {
 va_list  myargs;
  if( !retprnt ) return 11;
  *retprnt = -1;
  if( !strbuf )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  *retprnt = vsprintf( strbuf, format, myargs );
  va_end( myargs );
  if( 0 > *retprnt ) return 14;   // In case of an output error a negative value returned
 return 0;
}

sprintf_void_ ( char * strbuf, const char * format, ... ) {
 va_list  myargs;
  if( !strbuf )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  (void) vsprintf( strbuf, format, myargs );
  va_end( myargs );
 return 0;
}
sprintf_ ( char * strbuf, const char * format, ... ) {
 va_list  myargs;
  if( !strbuf )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  (void) vsprintf( strbuf, format, myargs );
  va_end( myargs );
 return 0;
}


print_ret_ ( int * retprnt, const char * path, const char * mode, const char * format, ... ) {

 FILE * fh;
 char * errallc = NULL;
 size_t  tmpsizet = (size_t) 0;
 va_list  myargs;

  if( !retprnt ) return 11;
  *retprnt = -1;
  if( !path    ) return 12;
  if( !*path   ) return 13;
  if( !mode    ) return 14;
  if( !*mode   ) return 15;
  if( !format  ) return 16;
  // accepted modes:  a a+ ab ab+ a+b w w+ wb wb+ w+b r+ rb+ r+b
  if(      strcmp( mode, "a"  )  &&  strcmp( mode, "ab"  )  &&  strcmp( mode, "ab+" )
       &&  strcmp( mode, "a+" )  &&  strcmp( mode, "a+b" )
       &&  strcmp( mode, "w"  )  &&  strcmp( mode, "wb"  )  &&  strcmp( mode, "wb+" )
       &&  strcmp( mode, "w+" )  &&  strcmp( mode, "w+b" )
       &&  strcmp( mode, "r+" )  &&  strcmp( mode, "r+b" )  &&  strcmp( mode, "rb+" )
    ) {
    E0( 17, in print_ bad param mode, mode );
    return 17;
  }
  fh = fopen( path, mode );
  if( !fh ) {
    tmpsizet = strlen( path ) + strlen( mode ) + 15;   // "PATH: %s, MODE: %s\0"
    errallc = alloca( tmpsizet );
    (void) sprintf( errallc, "PATH: %s, MODE: %s", path, mode );
    E0( 18, in print_ fopen failed, errallc );
    return 18;
  }
  va_start( myargs, format );
  *retprnt = vfprintf( fh, format, myargs );
  va_end( myargs );
  (void) fclose( fh );   // also does fflush()
  if( 0 > *retprnt ) return 19;   // In case of an output error a negative value returned
 return 0;
}

print_void_ ( const char * path, const char * mode, const char * format, ... ) {
 FILE * fh;
 int  retprnt = -1;
 char * errallc = NULL;
 size_t  tmpsizet = (size_t) 0;
 va_list  myargs;
  if( !path    ) return 12;
  if( !*path   ) return 13;
  if( !mode    ) return 14;
  if( !*mode   ) return 15;
  if( !format  ) return 16;
  if(      strcmp( mode, "a"  )  &&  strcmp( mode, "ab"  )  &&  strcmp( mode, "ab+" )
       &&  strcmp( mode, "a+" )  &&  strcmp( mode, "a+b" )
       &&  strcmp( mode, "w"  )  &&  strcmp( mode, "wb"  )  &&  strcmp( mode, "wb+" )
       &&  strcmp( mode, "w+" )  &&  strcmp( mode, "w+b" )
       &&  strcmp( mode, "r+" )  &&  strcmp( mode, "r+b" )  &&  strcmp( mode, "rb+" )
    ) {
    E0( 17, in print_void_ bad param mode, mode );
    return 17;
  }
  fh = fopen( path, mode );
  if( !fh ) {
    tmpsizet = strlen( path ) + strlen( mode ) + 15;   // "PATH: %s, MODE: %s\0"
    errallc = alloca( tmpsizet );
    (void) sprintf( errallc, "PATH: %s, MODE: %s", path, mode );
    E0( 18, in print_void_ fopen failed, errallc );
    return 18;
  }
  va_start( myargs, format );
  retprnt = vfprintf( fh, format, myargs );
  va_end( myargs );
  (void) fclose( fh );
  if( 0 > retprnt ) return 19;
 return 0;
}
print_ ( const char * path, const char * mode, const char * format, ... ) {
 FILE * fh;
 int  retprnt = -1;
 char * errallc = NULL;
 size_t  tmpsizet = (size_t) 0;
 va_list  myargs;
  if( !path    ) return 12;
  if( !*path   ) return 13;
  if( !mode    ) return 14;
  if( !*mode   ) return 15;
  if( !format  ) return 16;
  if(      strcmp( mode, "a"  )  &&  strcmp( mode, "ab"  )  &&  strcmp( mode, "ab+" )
       &&  strcmp( mode, "a+" )  &&  strcmp( mode, "a+b" )
       &&  strcmp( mode, "w"  )  &&  strcmp( mode, "wb"  )  &&  strcmp( mode, "wb+" )
       &&  strcmp( mode, "w+" )  &&  strcmp( mode, "w+b" )
       &&  strcmp( mode, "r+" )  &&  strcmp( mode, "r+b" )  &&  strcmp( mode, "rb+" )
    ) {
    E0( 17, in print_ bad param mode, mode );
    return 17;
  }
  fh = fopen( path, mode );
  if( !fh ) {
    tmpsizet = strlen( path ) + strlen( mode ) + 15;   // "PATH: %s, MODE: %s\0"
    errallc = alloca( tmpsizet );
    (void) sprintf( errallc, "PATH: %s, MODE: %s", path, mode );
    E0( 18, in print_ fopen failed, errallc );
    return 18;
  }
  va_start( myargs, format );
  retprnt = vfprintf( fh, format, myargs );
  va_end( myargs );
  (void) fclose( fh );
  if( 0 > retprnt ) return 19;
 return 0;
}


#if  0


// echo  'CURL_CA_BUNDLE=/webadmin/curlcacert.pem'  >>  /usr/local/etc/bashrc
//   echo_out_   echo_err_   echo_file_   echo_
// echo 'x'          => put to stdout (if any) 'x' + \n  => Echo      ( "x",  NULL  );
// echo 'x'  > file  => create/truncate file w 'x' + \n  => Echo      ( "x", "file" );
// echo 'x' >> file  => create/append   file w 'x' + \n  => EchoAppend( "x", "file" );

EchoAppend ( const char *text, const char *fileName ) {
  FILE *fp;
    if( !fileName ) {
      if( !stdout ) return -6;
      else {
        if( text ) (void) fputs( text, stdout ); (void) fputc( '\n', stdout );
        (void) fflush( stdout );
        return 0;
      }
    }
    if( !( fp = fopen( fileName, "ab" ))) return -7;
    if( text ) (void) fputs( text, fp ); (void) fputc( '\n', fp );
    (void) fclose( fp );
 return 0;
}

Echo ( const char *text, const char *fileName ) {
  int  retv;
  FILE *fp;
    if( fileName ) {   // create/truncate [wb]
      if( !( fp = fopen( fileName, "wb" ))) return -8; (void) fclose( fp );
    }
    if(( retv = EchoAppend( text, fileName ))) return retv;
 return 0;
}


#endif

