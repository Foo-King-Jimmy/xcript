

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
    Es( 17, in print_ bad param mode, mode );
    return 17;
  }
  fh = fopen( path, mode );
  if( !fh ) {
    tmpsizet = strlen( path ) + strlen( mode ) + 15;   // "PATH: %s, MODE: %s\0"
    errallc = alloca( tmpsizet );
    (void) sprintf( errallc, "PATH: %s, MODE: %s", path, mode );
    Es( 18, in print_ fopen failed, errallc );
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
    Es( 17, in print_void_ bad param mode, mode );
    return 17;
  }
  fh = fopen( path, mode );
  if( !fh ) {
    tmpsizet = strlen( path ) + strlen( mode ) + 15;   // "PATH: %s, MODE: %s\0"
    errallc = alloca( tmpsizet );
    (void) sprintf( errallc, "PATH: %s, MODE: %s", path, mode );
    Es( 18, in print_void_ fopen failed, errallc );
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
    Es( 17, in print_ bad param mode, mode );
    return 17;
  }
  fh = fopen( path, mode );
  if( !fh ) {
    tmpsizet = strlen( path ) + strlen( mode ) + 15;   // "PATH: %s, MODE: %s\0"
    errallc = alloca( tmpsizet );
    (void) sprintf( errallc, "PATH: %s, MODE: %s", path, mode );
    Es( 18, in print_ fopen failed, errallc );
    return 18;
  }
  va_start( myargs, format );
  retprnt = vfprintf( fh, format, myargs );
  va_end( myargs );
  (void) fclose( fh );
  if( 0 > retprnt ) return 19;
 return 0;
}


///////////////////////////////////////////////////////////////////////////
//
//  ? echo_err_  <~ there is always fprintf_( stderr, ... ) available !
//
//  echo_                echo_void_         echo_ret_   => stdout
//  echo_file_      echo_file_void_    echo_file_ret_   => pathname
//  echo_append_  echo_append_void_  echo_append_ret_   => pathname
//

echo_ret_ ( int * retprnt, const char * format, ... ) {
 va_list  myargs;
 int  retone = -1;
  if( !retprnt ) return 11;
  *retprnt = -1;
  if( !stdout )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  *retprnt = vfprintf( stdout, format, myargs );
  if( 0 <= *retprnt ) retone = fprintf( stdout, "\n" );
  va_end( myargs );
  if( 0 > *retprnt ) return 14;
  if( 1 != retone  ) return 15;
  ++*retprnt;
 return 0;
}

echo_void_ ( const char * format, ... ) {
 va_list  myargs;
 int  retprnt = -1;
  if( !stdout )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  retprnt = vfprintf( stdout, format, myargs );
  if( 0 <= retprnt ) (void) fprintf( stdout, "\n" );
  va_end( myargs );
  // if( 0 > retprnt ) return 14;
 return 0;
}
echo_ ( const char * format, ... ) {
 va_list  myargs;
 int  retprnt = -1;
  if( !stdout )  return 12;
  if( !format )  return 13;
  va_start( myargs, format );
  retprnt = vfprintf( stdout, format, myargs );
  if( 0 <= retprnt ) (void) fprintf( stdout, "\n" );
  va_end( myargs );
  // if( 0 > retprnt ) return 14;
 return 0;
}


echo_file_ret_ ( int * retprnt, const char * path, const char * mode, const char * format, ... ) {

 FILE * fh;
 int  retone = -1;
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
    Es( 17, in echo_file_ret_ bad param mode, mode );
    return 17;
  }
  fh = fopen( path, mode );
  if( !fh ) {
    tmpsizet = strlen( path ) + strlen( mode ) + 15;   // "PATH: %s, MODE: %s\0"
    errallc = alloca( tmpsizet );
    (void) sprintf( errallc, "PATH: %s, MODE: %s", path, mode );
    Es( 18, in echo_file_ret_ fopen failed, errallc );
    return 18;
  }
  va_start( myargs, format );
  *retprnt = vfprintf( fh, format, myargs );
  if( 0 <= *retprnt ) retone = fprintf( fh, "\n" );
  va_end( myargs );
  (void) fclose( fh );
  if( 0 > *retprnt ) return 19;
  if( 1 != retone  ) return 20;
  ++*retprnt;
 return 0;
}

echo_file_void_ ( const char * path, const char * mode, const char * format, ... ) {
 FILE * fh;
 char * errallc = NULL;
 size_t  tmpsizet = (size_t) 0;
 va_list  myargs;
 int  retprnt = -1;
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
    Es( 17, in echo_file_void_ bad param mode, mode );
    return 17;
  }
  fh = fopen( path, mode );
  if( !fh ) {
    tmpsizet = strlen( path ) + strlen( mode ) + 15;   // "PATH: %s, MODE: %s\0"
    errallc = alloca( tmpsizet );
    (void) sprintf( errallc, "PATH: %s, MODE: %s", path, mode );
    Es( 18, in echo_file_void_ fopen failed, errallc );
    return 18;
  }
  va_start( myargs, format );
  retprnt = vfprintf( fh, format, myargs );
  if( 0 <= retprnt ) (void) fprintf( fh, "\n" );
  va_end( myargs );
  (void) fclose( fh );
  // if( 0 > retprnt ) return 19;
 return 0;
}
echo_file_ ( const char * path, const char * mode, const char * format, ... ) {
 FILE * fh;
 char * errallc = NULL;
 size_t  tmpsizet = (size_t) 0;
 va_list  myargs;
 int  retprnt = -1;
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
    Es( 17, in echo_file_ bad param mode, mode );
    return 17;
  }
  fh = fopen( path, mode );
  if( !fh ) {
    tmpsizet = strlen( path ) + strlen( mode ) + 15;   // "PATH: %s, MODE: %s\0"
    errallc = alloca( tmpsizet );
    (void) sprintf( errallc, "PATH: %s, MODE: %s", path, mode );
    Es( 18, in echo_file_ fopen failed, errallc );
    return 18;
  }
  va_start( myargs, format );
  retprnt = vfprintf( fh, format, myargs );
  if( 0 <= retprnt ) (void) fprintf( fh, "\n" );
  va_end( myargs );
  (void) fclose( fh );
  // if( 0 > retprnt ) return 19;
 return 0;
}


echo_append_ret_ ( int * retprnt, const char * path, const char * format, ... ) {
 FILE * fh;
 int  retone = -1;
 va_list  myargs;
  if( !retprnt ) return 11;
  *retprnt = -1;
  if( !path    ) return 12;
  if( !*path   ) return 13;
  if( !format  ) return 16;
  fh = fopen( path, "ab" );
  if( !fh ) {
    Es( 18, in echo_append_ret_ fopen failed, path );
    return 18;
  }
  va_start( myargs, format );
  *retprnt = vfprintf( fh, format, myargs );
  if( 0 <= *retprnt ) retone = fprintf( fh, "\n" );
  va_end( myargs );
  (void) fclose( fh );
  if( 0 > *retprnt ) return 19;
  if( 1 != retone  ) return 20;
  ++*retprnt;
 return 0;
}

echo_append_void_ ( const char * path, const char * format, ... ) {
 FILE * fh;
 va_list  myargs;
 int  retprnt = -1;
  if( !path    ) return 12;
  if( !*path   ) return 13;
  if( !format  ) return 16;
  fh = fopen( path, "ab" );
  if( !fh ) {
    Es( 18, in echo_append_void_ fopen failed, path );
    return 18;
  }
  va_start( myargs, format );
  retprnt = vfprintf( fh, format, myargs );
  if( 0 <= retprnt ) (void) fprintf( fh, "\n" );
  va_end( myargs );
  (void) fclose( fh );
  // if( 0 > retprnt ) return 19;
 return 0;
}
echo_append_ ( const char * path, const char * format, ... ) {
 FILE * fh;
 va_list  myargs;
 int  retprnt = -1;
  if( !path    ) return 12;
  if( !*path   ) return 13;
  if( !format  ) return 16;
  fh = fopen( path, "ab" );
  if( !fh ) {
    Es( 18, in echo_append_ fopen failed, path );
    return 18;
  }
  va_start( myargs, format );
  retprnt = vfprintf( fh, format, myargs );
  if( 0 <= retprnt ) (void) fprintf( fh, "\n" );
  va_end( myargs );
  (void) fclose( fh );
  // if( 0 > retprnt ) return 19;
 return 0;
}

