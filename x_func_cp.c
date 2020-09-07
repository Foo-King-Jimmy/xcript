

/***
 cp:  inp => outp      // could be  cp_( p1, p2, ... );
      if inp !exists:  error, nothing-to-do
      if inp  exists:  > ok further check >
      if inp  a file:  ok, lets copy it
                       if outp !exists:  ok that is the new name, can not end in slash
                       if outp  exists:  if outp  a file:  ? overwrite/replace ?
                                         if outp  a  dir:  ok alloca new name dir/file cp
      if inp  a  dir:  ? recursive ? n:  error, can not cp a dir
                                     y:  if outp !exists:  ok new dir name, mkdir cp -r
                                         if outp  exists:  > ok further check >
                                         if outp  a file:  error, name occupied (file)
                                         if outp  a  dir:  ok alloca new name dir/dir  cp -r
                                                           further check on dir/dir
                                                           ? exists,file,dir,other
                                                           ? overwrite/replace ? no
***/


#define   XCRPT_FLAG_OVERWRT   (0)
#define   XCRPT_FLAG_NOVRWRT   (1)
#define   XCRPT_FLAG_NORCRSV   (0)
#define   XCRPT_FLAG_RECURSV   (1)


cp_raw_recursive_ ( const char * srcname, const char * trgtname ) {
  ??  ??  ??
 return 0;
}


cp_raw_ ( const char * srcname, const char * trgtname ) {
  ??
  // !? rm trgtname == fopen wb
  cp_raw_ sometimes needs to overwrite a file { rm x == fopen wb }
  cp_raw_( srcname, trgtname );
  onError:
    on error pls display input names that failed w E0 but alloca needed
    to show thme (cp) "xxxxxx => xxxxxxxx0"
    alloca strlen(srcname) + strlen(trgtname) + 4 + 1
    "srcname => trgtname" extra E0 str failed
  ??
 return 0;
}


cp_full_ ( const char * srcname, const char * trgtname, int nooverwrite, int recursive ) {

 const char * tmpp1;
 char * trgtname_a   = NULL,
      * srcname_base = NULL;
 size_t  tmpsizet  = (size_t) 0;

  if( !srcname   ) return 11;
  if( !*srcname  ) return 12;
  if( !trgtname  ) return 13;
  if( !*trgtname ) return 14;

  if( !name_exists_( srcname )) return 15;
  if( regular_file_exists_( srcname )) {
    if( !name_exists_( trgtname )) {
      tmpp1 = trgtname;
      while( *tmpp1 ) ++tmpp1;
      if( '/' == *--tmpp1 ) return 17;   // new (file)name (trgtname) can not end in a slash
      // NOTE:  absolute (target) path /OR/ relative (target) path  here is meaningless
      return cp_raw_( srcname, trgtname );
    }
    else {
      if( regular_file_exists_( trgtname )) {
        if( nooverwrite ) return 19;
        return cp_raw_( srcname, trgtname );
      }
      else if( directory_exists_( trgtname )) {
        //  cp  /d1/d2/f  /d3/d4/   >> /d3/d4/f
        //  cp   d1/d2/f   d3/d4/   >>  d3/d4/f
        basename_( srcname, &srcname_base );
        tmpsizet = strlen( trgtname ) + 1 + strlen( srcname_base ) + 1;   // '/' + '\0'
        trgtname_a = alloca( tmpsizet );   // alloca() fails to fail - so no wrapper around it
        // add slash (to trgtname) if needed
        tmpp1 = trgtname;
        while( *tmpp1 ) ++tmpp1;
        if( '/' == *--tmpp1 ) (void) sprintf( trgtname_a, "%s%s",  trgtname, srcname_base );
        else                  (void) sprintf( trgtname_a, "%s/%s", trgtname, srcname_base );
        // CHECK (trgtname_a) -> IF !exists -> ok cp_raw_
        //                    -> IF  exists -> further check
        //                                  -> IF  file -> IF OVERWRT -> Y: cp_raw_
        //                                                            -> N: error
        //                                  -> IF   dir -> error cannot be a dir
        //                                  -> IF other -> error wont overwrite others
        if( !name_exists_( trgtname_a )) return cp_raw_( srcname, trgtname_a );
        else {
          if( regular_file_exists_( trgtname_a )) {
            if( nooverwrite ) return 21;
            return cp_raw_( srcname, trgtname_a );
          }
          else return 20;   // can not be a dir/others
        }
      }
      else return 18;   // ERROR: file exist & not-a-dir & not-a-regfile
                        //        will not overwrite (a socket,charspec,blockspec, etc.)
    }
  }
  else if( directory_exists_( srcname )) {
    if( !recursive ) return 22;
    if( !name_exists_( trgtname )) {


/*
outp !exists: ok, new dir-name, mkdir, cp -r
mkdir trgtname
cp -r srcname ...
*/


    }
    else {
      if( directory_exists_( trgtname )) {
        //  cp  /d1/d2/  /d3/d4/   >> /d3/d4/d2/
        //  cp   d1/d2/   d3/d4/   >>  d3/d4/d2/


XXXX

/*      basename_( srcname, &srcname_base );
        if basename (for srcname) is "/" then error !!!   !strcmp( srcname_base, "/" )
        will not copy root recursively
       /d  > d
       /d/ > d
        d  > d
        d/ > d
  d1/d2/d  > d
  d1/d2/d/ > d
        tmpsizet = strlen( trgtname ) + 1 + strlen( srcname_base ) + 1;   // '/' + '\0'
        trgtname_a = alloca( tmpsizet );   // alloca() fails to fail - so no wrapper around it
        // add slash (to trgtname) if needed
        tmpp1 = trgtname;
        while( *tmpp1 ) ++tmpp1;
        if( '/' == *--tmpp1 ) (void) sprintf( trgtname_a, "%s%s",  trgtname, srcname_base );
        else                  (void) sprintf( trgtname_a, "%s/%s", trgtname, srcname_base );
*/


/*
alloca new name: dir/dir/ (watch slashes), ..., cp -r
further check on dir/dir/ !!!
??? !exists,exists>file,dir,other
??? overwrite ???
===================
/d/d/f/     /d/d/f/   
/d/d/f      /d/d/f
 d/d/f/      d/d/f/
 d/d/f       d/d/f
============================================= */
      }
      else return 23;   // no sense to cp a-dir (into) a-file/others
    }
  }
  else return 16;   // exists, but neither a dir, nor a reg file

 return 0;
}




#if  0

============================================================================================
 cp:  inp => outp
      if inp !exists:  error, nothing-to-do
      if inp  exists:  > ok further check >
      if inp  a file:  ok, lets copy it
                       if outp !exists:  ok that is the new name, can not end in slash
                       if outp  exists:  if outp  a file:  ? overwrite/replace ?
                                         if outp  a  dir:  ok alloca new name dir/file cp
      if inp  a  dir:  ? recursive ? n:  error, can not cp a dir
                                     y:  if outp !exists:  ok new dir name, mkdir cp -r
                                         if outp  exists:  > ok further check >
                                         if outp  a file:  error, name occupied (file)
                                         if outp  a  dir:  ok alloca new name dir/dir  cp -r
                                                           ? overwrite/replace ? no
============================================================================================

cp_file_( const char * src, const char * new ) {
 int  charctr;
 FILE * srcfp = NULL;
 FILE * newfp = NULL;
  if( !src ) return -1;
  if( !new ) return -2;
  if( !( srcfp = fopen( src, "rb" ))) return -3;
  if( !( newfp = fopen( new, "wb" ))) {
    (void) fclose( srcfp );
    return -4;
  }
  while( EOF != ( charctr = fgetc( srcfp ))) (void) fputc( charctr, newfp );
  (void) fclose( srcfp );
  (void) fclose( newfp );
 return 0;
}

//  SIMPLE :
//  filename |=> filename  ..simple;
//  filename |=> dirname   ..copies.into.dir ..alloca.dirname/filename0;
//  dirname  |=> dirname   ..fails;
//  dirname  |=> filename  ..fails;
//  RECURSIVE :
//  filename |=> filename  ..falls.back.to.simple.mode;
//  filename |=> dirname   ..falls.back.to.simple.mode;
//  dirname  |=> dirname   ..recursive;
//  dirname  |=> filename  ..stupid.fails;
//  +globbing()

cp_array_( const char * src, const char * new ) {
cp_files_( const char * src, const char * new ) {
cp_dir_  ( const char * src, const char * new ) {
cp_r_    ( const char * src, const char * new ) {
cp_glob_ ( const char * src, const char * new ) {


#endif

