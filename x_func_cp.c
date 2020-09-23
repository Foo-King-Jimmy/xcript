

// !?: What about others     if cp_raw_   ??? cp symlink/char/block/socket ?
// !?: What about perm/owner if cp_raw_ or cp_recursive_


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
    Es( 15, in cp_raw_ fopen-1-rb failed on filename, srcname );
    return 15;
  }
  trg_fh = fopen( trgtname, "wb" );
  if( !trg_fh ) {
    Es( 16, in cp_raw_ fopen-2-wb failed on filename, trgtname );
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


// This FORWARD needed because of the double-recursive nature:
//
cp_raw_recursive_ ( const char * , const char * );
//
//          ... needed because of the double-recursive nature.


/* static */
cp_raw_mid_alloca_ ( const char * srcname, const char * trgtname, const char * dname,
                     int recursive ) {

 char * srcpath  = NULL,
      * trgtpath = NULL;
 size_t  tmpdname = (size_t) 0,
         tmpsizet = (size_t) 0;

  /* ? exclude these checks under ? */
  if( !srcname   ) return 11;
  if( !*srcname  ) return 12;
  if( !trgtname  ) return 13;
  if( !*trgtname ) return 14;
  if( !dname     ) return 15;
  if( !*dname    ) return 16;
  /* ? exclude these checks above ? */
  tmpdname = strlen( dname   );
  tmpsizet = strlen( srcname );
  if( '/' == srcname[ tmpsizet - 1 ] ) {
    srcpath = alloca( tmpsizet + tmpdname + 1 );
    (void) sprintf( srcpath, "%s%s", srcname, dname );
  }
  else {
    srcpath = alloca( tmpsizet + tmpdname + 2 );
    (void) sprintf( srcpath, "%s/%s", srcname, dname );
  }
  tmpsizet = strlen( trgtname );
  if( '/' == trgtname[ tmpsizet - 1 ] ) {
    trgtpath = alloca( tmpsizet + tmpdname + 1 );
    (void) sprintf( trgtpath, "%s%s", trgtname, dname );
  }
  else {
    trgtpath = alloca( tmpsizet + tmpdname + 2 );
    (void) sprintf( trgtpath, "%s/%s", trgtname, dname );
  }

  if( recursive ) return cp_raw_recursive_( srcpath, trgtpath );
  // else

 return cp_raw_( srcpath, trgtpath );
}


// struct dirent    https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html
//    _DIRENT_HAVE_D_NAMLEN
//    unsigned char  d_namlen   The length of the file name, not including the terminating null.
//    unsigned char  d_type
//       DT_UNKNOWN
//       DT_REG       A regular file.
//       DT_DIR       A directory.
//       DT_FIFO      A named pipe, or FIFO. See FIFO Special Files.
//       DT_SOCK      A local-domain socket.
//       DT_CHR       A character device.
//       DT_BLK       A block device.
//       DT_LNK       A symbolic link.

cp_raw_recursive_ ( const char * srcname, const char * trgtname ) {

 int  retval;
 DIR * dirstream           = NULL;
 struct dirent * direntity = NULL;

  // Never exit on errors, just return as this is a recursive function !

  if( !srcname   ) return 11;
  if( !*srcname  ) return 12;
  if( !trgtname  ) return 13;
  if( !*trgtname ) return 14;
  if( !directory_exists_( srcname  )) return 15;   // srcname  should be a dir
  if(            exists_( trgtname )) return 16;   // trgtname should not exist

  // default (777-umask) mode for mkdir_():
  retval = mkdir_( trgtname );   // ?! should use  mkdir_m_( mode_of_(srcname), trgtname );
  // I do not use E(x,y) macro above as mkdir_m_() includes one.
  if( retval ) return 17;

  // srcname  ~> dir1 or dir1/
  // trgtname ~> dir2 or dir2/
  // fdiropen D
  // > dir1 .     SKIP
  // > dir1 ..    SKIP
  // > dir1 file  cp_raw_( dir1/file dir2/file )
  // > dir1 dir3  cp_rec_( dir1/dir3 dir2/dir3 )
  // > dir1 EOF   done.

  dirstream = opendir( srcname );
  if( !dirstream ) return 18;

  while( 1 ) {
    direntity = readdir( dirstream );
    if( !direntity ) break;   // NULL ~= no more item (on dirstream)
    if( DT_DIR != direntity->d_type ) {   // not a directory, not DT_DIR
      //>  dir1    file    cp_raw_(    dir1/file       dir2/file   )
      //>  srcname d_name  cp_raw_( srcname/d_name trgtname/d_name )
      retval = cp_raw_mid_alloca_( srcname, trgtname, direntity->d_name, 0 );   // not recursive
      if( retval ) {
        (void) closedir( dirstream );
        return 19;
        // ? continue; ?
      }
    }
    else {   // a directory, DT_DIR
      if( !strcmp( direntity->d_name, "."  )) continue;
      if( !strcmp( direntity->d_name, ".." )) continue;
      //>  dir1    dir3    cp_r_(    dir1/dir3       dir2/dir3   )
      //>  srcname d_name  cp_r_( srcname/d_name trgtname/d_name )
      retval = cp_raw_mid_alloca_( srcname, trgtname, direntity->d_name, 1 );   // recursive
      if( retval ) {
        (void) closedir( dirstream );
        return 20;
        // ? continue; ?
      }
    }
  }   // while( 1 ).
  (void) closedir( dirstream );

 return 0;
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
      return cp_raw_recursive_( srcname, trgtname );
    }
    else {
      if( directory_exists_( trgtname )) {
        //  cp  /d1/d2/  /d3/d4/   >> /d3/d4/d2/
        //  cp   d1/d2/   d3/d4/   >>  d3/d4/d2/
        //  cp -r dir1 dir2  >>  ? dir2/dir1 ? exists-whatis
        basename_( srcname, &srcname_base );
        if( !strcmp( "/", srcname_base )) {
          Es( 24, in cp_full_ wont copy root-folder recursively, srcname );
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
          Es( 25, in cp_full_ error as target name exists, trgtname_a );
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

////////////////////////////////////////////////////////////////////////////////////////////

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

? cp_array_
? cp_files_
? cp_dir_

#endif


////////////////////////////////////////////////////////////////////////////////////////////
// cp_globb_: not-recursive, will-overwrite, ignore-errors, if target dir: OK, if target file: ?ok

cp_globb_ ( const char * pattern, const char * target ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  ) return 13;
  if( !*pattern ) return 14;
  if( !target   ) return 15;
  if( !*target  ) return 16;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    (void) cp_( fileslst[idxj++], target );   // this will ignore dirs/ and also errors
  }
  free( fileslst );
 return 0;
}


cp_r_globb_ ( const char * pattern, const char * target ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  ) return 13;
  if( !*pattern ) return 14;
  if( !target   ) return 15;
  if( !*target  ) return 16;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    (void) cp_r_( fileslst[idxj++], target );
  }
  free( fileslst );
 return 0;
}
cp_recursive_globb_ ( const char * pattern, const char * target ) {
  return cp_r_globb_( pattern, target );
}

