

/*** in text_globalvars & text_main :
unsigned long  $remount_ro = (unsigned long)( MS_RELATIME | MS_REMOUNT | MS_RDONLY ),
               $remount_rw = (unsigned long)( MS_RELATIME | MS_REMOUNT             ),
                 $mount_ro = (unsigned long)( MS_RELATIME |              MS_RDONLY ),
                 $mount_rw = (unsigned long)( MS_RELATIME                          );
--- ( mine : ) ----------------------------------------------------------------------
unsigned long  $remountro = ( MS_REMOUNT | MS_NODIRATIME | MS_NOATIME | MS_RDONLY ),
               $remountrw = ( MS_REMOUNT | MS_NODIRATIME | MS_NOATIME             ),
                 $mountro = (              MS_NODIRATIME | MS_NOATIME | MS_RDONLY ),
                 $mountrw = (              MS_NODIRATIME | MS_NOATIME             );
***/ //----------------------------------------------------------------------------//

/*** List Of Possible Mount Flags :
MS_BIND  MS_DIRSYNC  MS_LAZYTIME  MS_MANDLOCK  MS_MOVE  MS_NOATIME  MS_NODEV  MS_NODIRATIME
MS_NOEXEC  MS_NOSUID  MS_PRIVATE  MS_RDONLY  MS_REC  MS_RELATIME  MS_REMOUNT  MS_SHARED
MS_SILENT  MS_SLAVE  MS_STRICTATIME  MS_UNBINDABLE  ***/


// mount_ thin wrapper:

mount_full_ ( const char * srcdev, const char * trgtdir,
              const char * fstype, unsigned long flags, const void * data ) {
 int  retmnt;
 size_t  tmpsizet;
 char * myerrstr = NULL;
  if( !srcdev   ) return 11;
  if( !*srcdev  ) return 12;
  if( !trgtdir  ) return 13;
  if( !*trgtdir ) return 14;
  if( !fstype   ) return 15;
  if( !*fstype  ) return 16;
  retmnt = mount( srcdev, trgtdir, fstype, flags, data );
  if( retmnt ) {   // == -1;  ERROR
    tmpsizet = (size_t) 25 + strlen( srcdev ) + strlen( trgtdir ) +  \
                             strlen( fstype ) + strlen( (const char *) data );
    myerrstr = alloca( tmpsizet );
    (void) sprintf( myerrstr, "SRC: %s  TO: %s  FS: %s  DAT: %s", srcdev, trgtdir, fstype, data );
    Es(  17   , in mount_full_ mount syscall failed on, myerrstr );
    return 17;
  }
 return 0;
}

umount_ ( const char * trgtdir ) {
  if( !trgtdir  ) return 11;
  if( !*trgtdir ) return 12;
  Es(  umount2( trgtdir, (int) MNT_DETACH ),   in umount_ umount2 syscall failed on, trgtdir );
 return $errno ? 13 : 0;
}


mount_type_ro_ ( const char * srcdev, const char * trgtdir, const char * fstype ) {
  return mount_full_( srcdev, trgtdir, fstype, $mount_ro, "user_xattr" );
}

mount_type_rw_ ( const char * srcdev, const char * trgtdir, const char * fstype ) {
  return mount_full_( srcdev, trgtdir, fstype, $mount_rw, "user_xattr" );
}


remount_type_ro_ ( const char * srcdev, const char * trgtdir, const char * fstype ) {
  return mount_full_( srcdev, trgtdir, fstype, $remount_ro, "user_xattr" );
}

remount_type_rw_ ( const char * srcdev, const char * trgtdir, const char * fstype ) {
  return mount_full_( srcdev, trgtdir, fstype, $remount_rw, "user_xattr" );
}


init$fstypes_ ( void ) {

 int  ptrsnum   = 1,   // 1: NULL
      charnum   = 0,
      loopbreak = 0,
      charx;
 char * strings = NULL;
 char **tmpvect = NULL;
 FILE * fhandle = NULL;
 size_t  tmpsizet = (size_t) 0;
 const char  procfilesystems[ ] = "/proc/filesystems";

  fhandle = fopen( procfilesystems, "rb" );
  if( !fhandle ) return 11;
  if( $fstypes ) {   // we are asked to redo this method
    free( $fstypes );
    $fstypes = NULL;
  }

  // /proc/filesystems: [nodev]\tfsname\r
  // let's count ptrsnum & charnum:
  while( EOF != ( charx = fgetc( fhandle ))) {
    if( '\t' == charx ) {   // \t sgsgsg \n
      charx = fgetc( fhandle );
      if( '\n' == charx ) continue;
      if( EOF  == charx ) break;
      ++ptrsnum;
      ++charnum;
      while( '\n' != ( charx = fgetc( fhandle ))) {
        if( EOF  == charx ) { loopbreak = 1; break; }
        ++charnum;
      }
    }
    else {   // nodev \t sgsgsg \n
      if( '\n' == charx ) continue;
      else {
        while( '\n' != ( charx = fgetc( fhandle ))) {
          if( EOF == charx ) { loopbreak = 1; break; }
        }
      }
    }
    if( loopbreak ) break;
  }

  if( !charnum || 1 == ptrsnum ) {
    (void) fclose( fhandle );
    return 12;
  }

  tmpsizet = (size_t)( charnum + ptrsnum - 1 + ptrsnum * sizeof(char *) );
  E(  malloc_( tmpsizet, &$fstypes ),   in init$fstypes_ malloc_ failed );
  if( $errno ) {
    (void) fclose( fhandle );
    return 13;
  }
  rewind( fhandle );
  loopbreak = 0,

  tmpvect = $fstypes;
  strings = (char *)( tmpvect + ptrsnum );   // strings = (char *)( &tmpvect[ptrsnum] );

  // let's fill $fstypes w pointers, and pointers w strings:
  while( EOF != ( charx = fgetc( fhandle ))) {
    if( '\t' == charx ) {   // \t sgsgsg \n
      charx = fgetc( fhandle );
      if( '\n' == charx ) continue;
      if( EOF  == charx ) break;
      *tmpvect++ = strings;
      *strings++ = (char)( 0xFF & charx );
      while( '\n' != ( charx = fgetc( fhandle ))) {
        if( EOF == charx ) { loopbreak = 1; break; }
        *strings++ = (char)( 0xFF & charx );
      }
      *strings++ = '\0';
    }
    else {   // nodev \t sgsgsg \n
      if( '\n' == charx ) continue;
      else {
        while( '\n' != ( charx = fgetc( fhandle ))) {
          if( EOF == charx ) { loopbreak = 1; break; }
        }
      }
    }
    if( loopbreak ) break;
  }
  (void) fclose( fhandle );
  *tmpvect = NULL;

 return 0;
}


mount_auto_full_ ( const char * srcdev, const char * trgtdir,
                   unsigned long flags, const void * data ) {

 char **tmpvect = NULL;
 int  retmnt;

  if( !srcdev   ) return 11;
  if( !*srcdev  ) return 12;
  if( !trgtdir  ) return 13;
  if( !*trgtdir ) return 14;
  if( !$fstypes ) {
    E(  init$fstypes_(),   in mount_auto_full_ init$fstypes_ failed );
    if( $errno ) return 15;
  }
  tmpvect = $fstypes;
  while( *tmpvect ) {
    retmnt = mount( srcdev, trgtdir, *tmpvect, flags, data );
    if( !retmnt ) return 0;
    ++tmpvect;
  }
  // could not mount it auto .

 return 16;
}


mount_ro_ ( const char * srcdev, const char * trgtdir ) {
  return mount_auto_full_( srcdev, trgtdir, $mount_ro, "user_xattr" );
}

mount_rw_ ( const char * srcdev, const char * trgtdir ) {
  return mount_auto_full_( srcdev, trgtdir, $mount_rw, "user_xattr" );
}


remount_ro_ ( const char * srcdev, const char * trgtdir ) {
  return mount_auto_full_( srcdev, trgtdir, $remount_ro, "user_xattr" );
}

remount_rw_ ( const char * srcdev, const char * trgtdir ) {
  return mount_auto_full_( srcdev, trgtdir, $remount_rw, "user_xattr" );
}

