

////// regex should be extended by a func that mmaps a file into str and then does regex


regex_flags_( const char * regexpattern, const char * instring, void **storeptr,
              int regcomp_flags, int regexec_flags ) {

 int  retvalue;
 regex_t  regexvar;
 regmatch_t  found[ 1 ];

  if( !storeptr )      return 11;
  *storeptr = NULL;
  if( ! regexpattern ) return 12;
  if( !*regexpattern ) return 13;
  if( ! instring )     return 14;
  // if( !*instring )  return 15;

  (void) memset( &regexvar, 0, sizeof(regex_t)    );
  (void) memset( found,     0, sizeof(regmatch_t) );
  retvalue = regcomp( &regexvar, regexpattern, regcomp_flags );
  if( retvalue ) {
    regfree( &regexvar );
    // ? return retvalue;
    return 16;
  }
  retvalue = regexec( &regexvar, instring, (size_t) 1, found, regexec_flags );
  if( retvalue ) {
    regfree( &regexvar );
    if( REG_NOMATCH == retvalue ) return 0;
    // ? return retvalue;
    return 17;
  }
  *storeptr = (void *)( instring + found[0].rm_so );
  regfree( &regexvar );

 return 0;
}


regex_( const char * regexpattern, const char * instring, void **storeptr ) {
  return regex_flags_( regexpattern, instring, storeptr, REG_EXTENDED | REG_ICASE, 0 );
}

