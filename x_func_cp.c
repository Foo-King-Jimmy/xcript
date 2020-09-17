

#define   XCRPT_OVERWRT   (0)
#define   XCRPT_NOVRWRT   (1)
#define   XCRPT_NORCRSV   (0)
#define   XCRPT_RECURSV   (1)


cp_raw_ ( const char * srcname, const char * trgtname ) {

 FILE * src_fh,
      * trg_fh;
 int  charctr;

  if( !srcname )   return 11;
  if( !*srcname )  return 12;
  if( !trgtname )  return 13;
  if( !*trgtname ) return 14;
  src_fh = fopen( srcname, "rb" );
  if( !src_fh ) {
    E0( 15, in cp_raw_ fopen-1-rb failed on filename, srcname );
    return 15;
  }
  trg_fh = fopen( trgtname, "wb" );
  if( !trg_fh ) {
    E0( 16, in cp_raw_ fopen-2-wb failed on filename, trgtname );
    (void) fclose( src_fh );
    return 16;
  }
  while( 1 ) {
    charctr = fgetc( src_fh );
    if( EOF == charctr ) break;
    (void) fputc( charctr, trg_fh );
  }
  (void) fclose( src_fh );
  (void) fclose( trg_fh );

 return 0;
}


cp_raw_recursive_ ( const char * srcname, const char * trgtname ) {

  // NOT IMPLEMENTED YET !
  // NOT IMPLEMENTED YET !
  // NOT IMPLEMENTED YET !

 return 99;   // not-implemented-yet
}


cp_full_ ( const char * srcname, const char * trgtname, int nooverwrite, int recursive ) {

 const char * tmpp1;
 char * tmpp2,
      * trgtname_a   = NULL,
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
      // outp !exists: ok, (that is a) new dir-name  [ mkdir, cp -r ]
      // [ mkdir ?mode? new-dir-name ] [ cp -r ]



      return cp_raw_recursive_( srcname, trgtname );   // UNFINISHED .



    }
    else {
      if( directory_exists_( trgtname )) {
        //  cp  /d1/d2/  /d3/d4/   >> /d3/d4/d2/
        //  cp   d1/d2/   d3/d4/   >>  d3/d4/d2/
        //  cp -r dir1 dir2  >>  ? dir2/dir1 ? exists-whatis
        basename_( srcname, &srcname_base );
        if( !strcmp( "/", srcname_base )) {
          E0( 24, in cp_full_ wont copy root-folder recursively, srcname );
          return 24;
        }
        tmpsizet = strlen( trgtname ) + 1 + strlen( srcname_base ) + 1 + 1;   // '/' + '/' + '\0'
        trgtname_a = alloca( tmpsizet );
        // add slash (to trgtname) if needed
        tmpp1 = trgtname;
        while( *tmpp1 ) ++tmpp1;
        if( '/' == *--tmpp1 ) (void) sprintf( trgtname_a, "%s%s/",  trgtname, srcname_base );
        else                  (void) sprintf( trgtname_a, "%s/%s/", trgtname, srcname_base );
        tmpp2 = strrchr( trgtname_a, '/' );
        // *tmpp2 = '\0';   // =>  dir/dir0



        if( !name_exists_( trgtname_a )) return cp_raw_recursive_( srcname, trgtname_a );



        else {
          // if           a dir  >> error: name occupied  [ ?! COULD ACTUALLY OVERWRITE ]
          // else if a reg-file  >> error: name occupied
          // else   others-file  >> error: name occupied
          E0( 25, in cp_full_ error as target name exists, trgtname_a );
          return 25;
        }
      }
      else return 23;   // no sense to cp a-dir (into) a-file/others
    }
  }
  else return 16;   // exists, but neither a dir, nor a reg file

 return 0;
}


cp_ ( const char * srcname, const char * trgtname ) {
  return cp_full_( srcname, trgtname, XCRPT_OVERWRT, XCRPT_NORCRSV );
}

cp_safe_ ( const char * srcname, const char * trgtname ) {
  return cp_full_( srcname, trgtname, XCRPT_NOVRWRT, XCRPT_NORCRSV );
}

cp_recursive_ ( const char * srcname, const char * trgtname ) {
  return cp_full_( srcname, trgtname, XCRPT_OVERWRT, XCRPT_RECURSV );
}

cp_r_ ( const char * srcname, const char * trgtname ) {
  return cp_full_( srcname, trgtname, XCRPT_OVERWRT, XCRPT_RECURSV );
}

cp_r_safe_ ( const char * srcname, const char * trgtname ) {
  return cp_full_( srcname, trgtname, XCRPT_NOVRWRT, XCRPT_RECURSV );
}

cp_recursive_safe_ ( const char * srcname, const char * trgtname ) {
  return cp_full_( srcname, trgtname, XCRPT_NOVRWRT, XCRPT_RECURSV );
}




#if  0

=======================
 /d1/d2/f/   /d3/d4/f/
 /d1/d2/f    /d3/d4/f
  d1/d2/f/    d3/d4/f/
  d1/d2/f     d3/d4/f
============================================================================================
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
============================================================================================

//  SIMPLE :
//  filename |=> filename  ..simple;   _?_overwrite
//  filename |=> dirname   ..copies.into.dir ..alloca.dirname/filename0;
//  dirname  |=> dirname   ..fails;    _not_recursive
//  dirname  |=> filename  ..fails;    _stupid
//  RECURSIVE :
//  filename |=> filename  ..falls.back.to.simple.mode;
//  filename |=> dirname   ..falls.back.to.simple.mode;
//  dirname  |=> dirname   ..recursive;
//  dirname  |=> filename  ..fails;    _stupid
//  +globbing()

cp_array_( const char * src, const char * new ) {
cp_files_
cp_dir_
cp_r_
cp_globb_


#endif

