

// Basic:  globb_       :  + files  + dirs/  - .periods
// Extra:  globb_dirs_  :  - files  + dirs/  - .periods
// Extra:  globb_dots2_ :  + files  + dirs/  + .periods
// +       globb_noadd_  globb_dirs_noadd_  globb_dots2_noadd_


#define   XCRPT_AD2MLST   (0)
#define   XCRPT_NOADD2M   (1)


globberr_ ( const char * epath, int eerrno ) {
  Es( eerrno, globberr_ path failed, epath );   // ?! use this macro here at all !?
 return 0;  // if returns non-zero, glob() will terminate
}


globb_full_ ( const char * pattern, char ***resultp, int addflags, int noadd2m ) {

 int  idxj       = 0,
      retcode    = 0,
      retval     = 0,
      globbflags = 0;
      // OLD:  globbflags = GLOB_MARK | GLOB_BRACE | GLOB_TILDE | GLOB_TILDE_CHECK;
 char * tmpp,
      * tmpmemb  = NULL;
 char **tmpptrs  = NULL;
 glob_t  globbbuf;
 size_t  mallcsz = (size_t) 0;

  if( !resultp ) return 12;
  *resultp = NULL;
  if( !pattern ) return 11;

  globbflags |= addflags;
  // OLD:  globbflags &= ~offflags;
  //
  // Unset flags:  +(  GLOB_MARK  GLOB_BRACE  GLOB_TILDE  GLOB_TILDE_CHECK  )
  //               GLOB_ERR  GLOB_DOOFFS  GLOB_APPEND  GLOB_NOESCAPE  GLOB_PERIOD  GLOB_NOCHECK
  //               GLOB_ONLYDIR  GLOB_ALTDIRFUNC  GLOB_TILDE_CHECK  GLOB_NOSORT  GLOB_NOMAGIC
  // ?   GLOB_MARK         = mark/append folders with terminating slash
  // ?   GLOB_ERR          = on error terminates early ( eg: can not enter a folder )
  // ?   GLOB_PERIOD       = also outputs dotfiles
  // ?   GLOB_ONLYDIR      ? outputs only folders ?
  // ?   GLOB_TILDE        = home_dir applied
  // ?   GLOB_BRACE        = do BSD-style folder expansion
  // ?   GLOB_TILDE_CHECK  ? user/home_dir cheking enabled
  // ?   GLOB_NOESCAPE     ?
  // ??? GLOB_NOCHECK      ? input still returned even if no match
  // ??? GLOB_NOMAGIC      ? input still returned even if no match if pattern has no meta-chars
  // ??? GLOB_NOSORT       = no sorting ( default is to sort )
  // !   GLOB_DOOFFS       = (no, hard-coded) use .gl_offs when setting char** globbbuf
  // !   GLOB_APPEND       = (no, hard-coded) extends char** globbbuf after a new call
  //
  globbbuf.gl_offs = 0;

  retval = glob( pattern, globbflags, globberr_, &globbbuf );
  if( retval ) {
    if(      GLOB_NOSPACE == retval ) retcode = 13;  // running out of memory
    else if( GLOB_ABORTED == retval ) retcode = 14;  // read error
    else if( GLOB_NOMATCH == retval ) retcode = 15;  // found no matches
    else                              retcode = 16;  // wtf
  }
  if( !retcode && globbbuf.gl_pathc ) {

    mallcsz = ( 1 + globbbuf.gl_pathc ) * sizeof(char*) + globbbuf.gl_pathc;
    for( ; idxj < globbbuf.gl_pathc; ++idxj ) {
      mallcsz += strlen( globbbuf.gl_pathv[idxj] );
    }
    if( noadd2m ) retval = malloc_noadd_( mallcsz, resultp );
    else          retval = malloc_      ( mallcsz, resultp );
    if( retval ) retcode = 17;
    else {
      tmpptrs = *resultp;
      tmpmemb = (char *) tmpptrs;
      tmpmemb += ( 1 + globbbuf.gl_pathc ) * sizeof(char*);
      for( idxj = 0; idxj < globbbuf.gl_pathc; ++idxj ) {
        *tmpptrs = tmpmemb;
        tmpp = globbbuf.gl_pathv[idxj];
        while( *tmpp ) *tmpmemb++ = *tmpp++;
        *tmpmemb++ = '\0';
        ++tmpptrs;
      }
      *tmpptrs = NULL;
    }
  }
  globfree( &globbbuf );

 return retcode;
}


globb_ ( const char * pattern, char ***resultp ) {
  return globb_full_( pattern, resultp,
                      GLOB_MARK | GLOB_BRACE | GLOB_TILDE | GLOB_TILDE_CHECK,
                      XCRPT_AD2MLST );
}

globb_flags_ ( const char * pattern, char ***resultp, int addflags ) {
  return globb_full_( pattern, resultp, addflags, XCRPT_AD2MLST );
}

globb_noadd_ ( const char * pattern, char ***resultp ) {
  return globb_full_( pattern, resultp,
                      GLOB_MARK | GLOB_BRACE | GLOB_TILDE | GLOB_TILDE_CHECK,
                      XCRPT_NOADD2M );
}

globb_flags_noadd_ ( const char * pattern, char ***resultp, int flags ) {
  return globb_full_( pattern, resultp, flags, XCRPT_NOADD2M );
}


globb_dots2_ ( const char * pattern, char ***resultp ) {
  return globb_full_( pattern, resultp,
                      GLOB_PERIOD | GLOB_MARK | GLOB_BRACE | GLOB_TILDE | GLOB_TILDE_CHECK,
                      XCRPT_AD2MLST );
}

globb_dots2_noadd_ ( const char * pattern, char ***resultp ) {
  return globb_full_( pattern, resultp,
                      GLOB_PERIOD | GLOB_MARK | GLOB_BRACE | GLOB_TILDE | GLOB_TILDE_CHECK,
                      XCRPT_NOADD2M );
}


globb_dirs_ ( const char * pattern, char ***resultp ) {
  return globb_full_( pattern, resultp,
                      GLOB_ONLYDIR | GLOB_MARK | GLOB_BRACE | GLOB_TILDE | GLOB_TILDE_CHECK,
                      XCRPT_AD2MLST );
}

globb_dirs_noadd_ ( const char * pattern, char ***resultp ) {
  return globb_full_( pattern, resultp,
                      GLOB_ONLYDIR | GLOB_MARK | GLOB_BRACE | GLOB_TILDE | GLOB_TILDE_CHECK,
                      XCRPT_NOADD2M );
}

