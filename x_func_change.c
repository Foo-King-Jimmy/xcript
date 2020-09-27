

// ? chgrp_ missing
// custom globb needed : globb files only, globb dirs only, globb (no)dots too, etc.


str2mode_ ( const char * modestr, mode_t * retmode ) {

 char  charx;
 const char * tmpp;
 size_t  lettersnum;
 mode_t  newmode = (mode_t) 0;

  if( !retmode )  return 11;
  // *retmode = (mode_t) 0;
  if( !modestr )  return 12;
  if( !*modestr ) return 13;
  // check modestr/permission: [[ 0-7 ]][ 0-7 ][ 0-7 ][ 0-7 ]
  lettersnum = strlen( modestr );
  if( 3 > lettersnum || 4 < lettersnum ) {
    Es( 14, in str2mode_ wrong length permission string given, modestr );
    return 14;
  }
  if( lettersnum != strspn( modestr, "01234567" )) {
    Es( 15, in str2mode_ wrong characters in permission string, modestr );
    return 15;
  }
  if( (size_t) 4 == lettersnum ) tmpp = &modestr[1];
  else      /* 3 */              tmpp = &modestr[0];
  if( (size_t) 4 == lettersnum ) {
    charx = tmpp[-1] - '0';
    if( 3 < charx ) { charx -= 4; newmode |= S_ISUID; }
    if( 1 < charx ) { charx -= 2; newmode |= S_ISGID; }
    if(     charx ) {             newmode |= S_ISVTX; }
  }
  charx = tmpp[0] - '0';
  if( 3 < charx ) { charx -= 4; newmode |= S_IRUSR; }
  if( 1 < charx ) { charx -= 2; newmode |= S_IWUSR; }
  if(     charx ) {             newmode |= S_IXUSR; }
  charx = tmpp[1] - '0';
  if( 3 < charx ) { charx -= 4; newmode |= S_IRGRP; }
  if( 1 < charx ) { charx -= 2; newmode |= S_IWGRP; }
  if(     charx ) {             newmode |= S_IXGRP; }
  charx = tmpp[2] - '0';
  if( 3 < charx ) { charx -= 4; newmode |= S_IROTH; }
  if( 1 < charx ) { charx -= 2; newmode |= S_IWOTH; }
  if(     charx ) {             newmode |= S_IXOTH; }
  *retmode = newmode;

 return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
// umask() sets the calling process's file mode creation mask (umask) to mask & 0777
// typical default value for the process umask is S_IWGRP | S_IWOTH (octal 022)
// umask of any process can be viewed via the Umask field of /proc/[pid]/status
// inspecting this field in /proc/self/status allows a process to retrieve its umask w/o changing
//
// ??? why not just use a string for umask "0022" or "022" ???

umask_raw_get_ ( mode_t * umaskp ) {
 mode_t  newmask = (mode_t) 0;
  if( !umaskp ) return 11;
  *umaskp = umask( newmask );
  (void) umask( *umaskp );
 return 0;
}

umask_raw_set_ ( mode_t newumask ) {
  (void) umask( newumask );
 return 0;
}

umask_raw_ret_ ( mode_t newumask, mode_t * oldumaskp ) {
  if( !oldumaskp ) return 11;
  *oldumaskp = umask( newumask );
 return 0;
}
umask_raw_flip_ ( mode_t newmsk, mode_t * oldmskp ) { return umask_raw_ret_( newmsk, oldmskp ); }
umask_raw_      ( mode_t newmsk, mode_t * oldmskp ) { return umask_raw_ret_( newmsk, oldmskp ); }

// LATER: get umask into a string:
// umask_get_ ( char **umaskstrp );

umask_set_ ( const char * newumaskstr ) {
 mode_t  newmode = (mode_t) 0;
 int  retfunc;
  if( !newumaskstr )  return 11;
  if( !*newumaskstr ) return 12;
  retfunc = str2mode_( newumaskstr, &newmode );
  if( retfunc )       return 13;
  (void) umask( newmode );
 return 0;
}

umask_ret_ ( const char * newumaskstr, mode_t * oldumaskp ) {
 mode_t  newmode = (mode_t) 0;
 int  retfunc;
  if( !oldumaskp )    return 11;
  if( !newumaskstr )  return 12;
  if( !*newumaskstr ) return 13;
  retfunc = str2mode_( newumaskstr, &newmode );
  if( retfunc )       return 14;
  *oldumaskp = umask( newmode );
 return 0;
}
umask_flip_ ( const char * newmsks, mode_t * oldmskp ) { return umask_ret_( newmsks, oldmskp ); }
umask_      ( const char * newmsks, mode_t * oldmskp ) { return umask_ret_( newmsks, oldmskp ); }


//////////////////////////////////////////////////////////////////////////////////////////

rmdir_ ( const char * dirpath ) {
  E( rmdir( dirpath ), in rmdir_ wrapper rmdir syscall failed );
 return $errno;
}

// rmdir_globb_:  not-recursive, ignore errors (except: _stop_), applies only to dirs, (?derefs?)

rmdir_globb_ignore_ ( const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  ) return 13;
  if( !*pattern ) return 14;
  // ! this returns only dirs/ and no files, no .periods
  if(( globb_dirs_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    (void) rmdir_( fileslst[idxj++] );
  }
  free( fileslst );
 return 0;
}
rmdir_globb_void_ ( const char * pattern ) { return rmdir_globb_ignore_( pattern ); }
rmdir_globb_      ( const char * pattern ) { return rmdir_globb_ignore_( pattern ); }

rmdir_globb_stop_ ( const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  ) return 13;
  if( !*pattern ) return 14;
  // ! this returns only dirs/ and no files, no .periods
  if(( globb_dirs_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    Es(  rmdir_( fileslst[idxj] ),   in rmdir_globb_stop_ rmdir_ failed on, fileslst[idxj] );
    if( $errno ) {
      free( fileslst );
      return 15;
    }
    ++idxj;
  }
  free( fileslst );
 return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
// ? chmod only files / ? chmod only dirs
// ? chmod mode name  >  globb_ file1 fileN
//
// chmod changes the mode of the path; if a symbolic link it is dereferenced
// 1-  there is no lchmod thus (sym)links (always) dereferenced ( as opposed to lchown() )
// 2-  thus symlinks permissions are irrelevant (no need to set them)
// 3-  chmod xxxx globb-pattern  =>  chmod xxxx file1 file2 ? dir1 ? dir2 ?
// 4-  globbing: if globb() then maybe files and dirs should be treated separatelly
//               files eg.: 644  dirs: eg.: 775  !?

chmod_ ( const char * perm, const char * path ) {
 mode_t  newmode = (mode_t) 0;
 int  retfunc;
  if( !perm )  return 11;
  if( !*perm ) return 12;
  if( !path )  return 13;
  if( !*path ) return 14;
  retfunc = str2mode_( perm, &newmode );
  if( retfunc ) return 15;
  Es( chmod( path, newmode ), in chmod_ chmod syscall failed on pathname, path );
  if( -1 == $errno ) return 16;
 return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////

#define   XCRPT_DERFLNK   (0)
#define   XCRPT_CHWNLNK   (1)

chown_raw_ ( const char * usrname, const char * grpname, const char * path, int nodereflink ) {

 struct passwd * pwdstrctp = NULL;
 struct group  * grpstrctp = NULL;
 uid_t  newowner = -1;   // If the owner or group is specified as -1,
 gid_t  newgroup = -1;   //   then that ID is not changed.

  if( !path )  return 11;
  if( !*path ) return 12;
  if( (usrname) && !*usrname ) return 13;
  if( (grpname) && !*grpname ) return 14;
  if( !usrname  &&  !grpname ) return 15;
  if( (usrname) ) {
    pwdstrctp = getpwnam( usrname );
    if( !pwdstrctp ) {
      Es( 16, in chown_raw_ getpwname failed with unknown username, usrname );
      return 16;
    }
    newowner = pwdstrctp->pw_uid;
  }
  if( (grpname) ) {
    grpstrctp = getgrnam( grpname );
    if( !grpstrctp ) {
      Es( 17, in chown_raw_ getgrname failed with unknown groupname, grpname );
      return 17;
    }
    newgroup = grpstrctp->gr_gid;
  }
  if( nodereflink ) {
    E( lchown( path, newowner, newgroup ), in chown_raw_ lchown syscall failed );
    if( $errno ) return 18;
  }
  else {   // deref symlinks - default action
    E( chown( path, newowner, newgroup ), in chown_raw_ chown syscall failed );
    if( $errno ) return 19;
  }

 return 0;
}

chown_ ( const char * usrname, const char * grpname, const char * path ) {
  return chown_raw_( usrname, grpname, path, XCRPT_DERFLNK );
}

chown_h_ ( const char * usrname, const char * grpname, const char * path ) {
  return chown_raw_( usrname, grpname, path, XCRPT_CHWNLNK );
}

chown_link_ ( const char * usrname, const char * grpname, const char * path ) {
  return chown_raw_( usrname, grpname, path, XCRPT_CHWNLNK );
}

chown_noderef_ ( const char * usrname, const char * grpname, const char * path ) {
  return chown_raw_( usrname, grpname, path, XCRPT_CHWNLNK );
}

chown_nodereflink_ ( const char * usrname, const char * grpname, const char * path ) {
  return chown_raw_( usrname, grpname, path, XCRPT_CHWNLNK );
}


//////////////////////////////////////////////////////////////////////////////////////////
// newly created directory will be owned by the effective user ID of the process
// + how to globb in mkdir_ ?
// mkdir_parent_
// mkdir_p_  //?use-default-mask-mode  -p, --parents   make parent directories as/if needed

mkdir_m_ ( const char * perm, const char * path ) {
 mode_t  themode = (mode_t) 0;
 int  retfunc;
  if( !path )  return 11;
  if( !*path ) return 12;
  if( !perm )
    retfunc = str2mode_( "777", &themode );
  else {
    if( !*perm ) return 13;
    retfunc = str2mode_( perm, &themode );
  }
  if( retfunc ) return 14;
  Es( mkdir( path, themode ), in mkdir_ mkdir syscall failed on pathname, path );
  if( -1 == $errno ) return 15;
 return 0;
}
mkdir_mode_ ( const char * perm, const char * path ) { return mkdir_m_( perm, path ); }
mkdir_      (                    const char * path ) { return mkdir_m_( NULL, path ); }

mkdir_forcemode_ ( const char * perm, const char * path ) {
 mode_t  themode = (mode_t) 0,
         oldmask = (mode_t) 0;
 int  retfunc;
  if( !path )   return 11;
  if( !*path )  return 12;
  if( !perm )   return 13;
  if( !*perm )  return 14;
  retfunc = str2mode_( perm, &themode );
  if( retfunc ) return 15;
  (void) umask_raw_ret_( (mode_t) 0, &oldmask );
  Es( mkdir( path, themode ), in mkdir_forcemode_ mkdir syscall failed on pathname, path );
  (void) umask_raw_set_( oldmask );
  if( -1 == $errno ) return 16;
 return 0;
}
mkdir_mode_force_ ( const char * perm, const char * path ) {
  return mkdir_forcemode_( perm, path );
}


//////////////////////////////////////////////////////////////////////////////////////////
// how to dereference a symlink?: symlink -> realfile ? use man 2 readlink() !
// if path==symlink ? rm symlink ?OR? rm deref(symlink)
// rm_deref_(?)   // use readlink to dereference a symlink
// ? rm_any_ => rm_any_void_ remove_void_

rm_ ( const char * pathname ) {
  // unlink(): If the name referred to a symbolic link, the link is removed.
  // deletes *file*, symlink (itself), can not delete directory
  if( !pathname  ) return 11;
  if( !*pathname ) return 12;
  E( unlink( pathname ), in rm_ wrapper unlink syscall failed );
 return $errno;
}
unlink_  ( const char * pathname ) { return rm_( pathname ); }
rm_file_ ( const char * pathname ) { return rm_( pathname ); }

rm_f_ ( const char * pathname ) {
  if( !pathname  ) return 0;
  if( !*pathname ) return 0;
  (void) unlink( pathname );
 return 0;
}
rm_void_      ( const char * pathname ) { return rm_f_( pathname ); }
rm_ignore_    ( const char * pathname ) { return rm_f_( pathname ); }
unlink_void_  ( const char * pathname ) { return rm_f_( pathname ); }
rm_file_void_ ( const char * pathname ) { return rm_f_( pathname ); }

rm_any_ ( const char * pathname ) {
  // same as remove(3): int remove() == unlink() + rmdir() .
  // deletes *file*, symlink (itself), empty directory
  if( !pathname  ) return 11;
  if( !*pathname ) return 12;
  E( remove( pathname ), in rm_any_ remove failed );
 return $errno;
}
remove_ ( const char * pathname ) { return rm_any_( pathname ); }


// rm_r_  ==  rm_recursive_  :

// This FORWARD needed because of the double-recursive nature:
//
rm_r_raw_ ( const char * , int );
//
//          ... needed because of the double-recursive nature.

#define   XCRPT_RMRNOSTOP   (0)
#define   XCRPT_RMRERSTOP   (1)

/* static */
rm_r_mid_ ( const char * pathname, const char * dname, int stoponerror ) {

 size_t  tmpdname = (size_t) 0,
         tmpsizet = (size_t) 0;
 char * newpath = NULL;

  /* ? exclude these checks under ? */
  if( !pathname  ) return 11;
  if( !*pathname ) return 12;
  if( !dname     ) return 13;
  if( !*dname    ) return 14;
  /* ? exclude these checks above ? */

  tmpdname = strlen( dname    );
  tmpsizet = strlen( pathname );
  if( '/' == pathname[ tmpsizet - 1 ] ) {
    newpath = alloca( tmpsizet + tmpdname + 1 );
    (void) sprintf( newpath, "%s%s", pathname, dname );
  }
  else {
    newpath = alloca( tmpsizet + tmpdname + 2 );
    (void) sprintf( newpath, "%s/%s", pathname, dname );
  }

 return rm_r_raw_( newpath, stoponerror );
}

rm_r_raw_ ( const char * pathname, int stoponerror ) {

 int  retrm,
      localerrno;
 DIR * dirstream           = NULL;
 struct dirent * direntity = NULL;

  if( !pathname  ) return 11;
  if( !*pathname ) return 12;
  // ??? remove:  .  ..  xxx/..
  if( !strcmp( pathname, "/" )) {
    E( 13, in rm_r_ refusing to remove the root folder );
    return 13;
  }

  if( !directory_exists_( pathname )) {
    retrm = rm_( pathname );
    if( stoponerror && retrm ) return 14;
    // if( !stoponerror && retrm ) ?
    return 0;
  }
  else {   // a directory
    retrm = rmdir( pathname );
    if( !retrm ) return 0;
    else {
      localerrno = errno;
      if( ENOTEMPTY != localerrno ) {
        if( stoponerror ) return 15;
        return 0;
      }

      dirstream = opendir( pathname );
      if( !dirstream ) {
        if( stoponerror ) return 16;
        return 0;
      }
      while( 1 ) {
        direntity = readdir( dirstream );
        if( !direntity ) break;   // NULL ~= no more item (on dirstream)
        if( !strcmp( direntity->d_name, "."  )) continue;
        if( !strcmp( direntity->d_name, ".." )) continue;
        retrm = rm_r_mid_( pathname, direntity->d_name, stoponerror );
        if( stoponerror && retrm ) {
          (void) closedir( dirstream );
          return retrm;
        }
      }   // while( 1 ).
      (void) closedir( dirstream );
      retrm = rmdir( pathname );
      if( !retrm ) return 0;
      else {
        if( stoponerror ) return 17;
        return 0;
      }
    }
  }

 return 0;
}
rm_recursive_raw_ ( const char * pathname, int stoponerror ) {
  return rm_r_raw_( pathname, stoponerror );
}

rm_r_                ( const char * pathname ) { return rm_r_raw_( pathname, XCRPT_RMRNOSTOP ); }
rm_recursive_        ( const char * pathname ) { return rm_r_raw_( pathname, XCRPT_RMRNOSTOP ); }
rm_r_void_           ( const char * pathname ) { return rm_r_raw_( pathname, XCRPT_RMRNOSTOP ); }
rm_recursive_void_   ( const char * pathname ) { return rm_r_raw_( pathname, XCRPT_RMRNOSTOP ); }
rm_r_ignore_         ( const char * pathname ) { return rm_r_raw_( pathname, XCRPT_RMRNOSTOP ); }
rm_recursive_ignore_ ( const char * pathname ) { return rm_r_raw_( pathname, XCRPT_RMRNOSTOP ); }

rm_r_stop_           ( const char * pathname ) { return rm_r_raw_( pathname, XCRPT_RMRERSTOP ); }
rm_recursive_stop_   ( const char * pathname ) { return rm_r_raw_( pathname, XCRPT_RMRERSTOP ); }
rm_r_error_          ( const char * pathname ) { return rm_r_raw_( pathname, XCRPT_RMRERSTOP ); }
rm_recursive_error_  ( const char * pathname ) { return rm_r_raw_( pathname, XCRPT_RMRERSTOP ); }


///////////////////////////////////////////////////////////////////////////
// chmodown() - double-wrapper:
// as chmod() ALWAYS dereferences a symlink (, a symlink's permission can not be set)
// in this function chown() also ALWAYS dereferences a (sym)link
// therefore this function can NOT be used to work on the symlink (itself)

chmodown_ ( const char * perm, const char * usrname, const char * grpname, const char * path ) {
  if( (usrname) && !*usrname ) return 13;   // keep the same return-values as from original func
  if( (grpname) && !*grpname ) return 14;
  if( !usrname  &&  !grpname ) return 15;
  if(( chmod_( perm, path )))                                return 11;
  if(( chown_raw_( usrname, grpname, path, XCRPT_DERFLNK ))) return 12;
 return 0;
}

// chmodown_globb_: not-recursive, deref symlinks, ignore-errors, ? applies to files/dirs ?

chmodown_globb_all_ ( const char * perm, const char * usrname,
                      const char * grpname, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( !perm     )              return 15;
  if( !*perm    )              return 16;
  if( (usrname) && !*usrname ) return 17;
  if( (grpname) && !*grpname ) return 18;
  if( !usrname  &&  !grpname ) return 19;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    (void) chmodown_( perm, usrname, grpname, fileslst[idxj++] );
  }
  free( fileslst );
 return 0;
}

chmodown_globb_dirs_ ( const char * perm, const char * usrname,
                       const char * grpname, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( !perm     )              return 15;
  if( !*perm    )              return 16;
  if( (usrname) && !*usrname ) return 17;
  if( (grpname) && !*grpname ) return 18;
  if( !usrname  &&  !grpname ) return 19;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    if( directory_exists_( fileslst[idxj] )) {
      (void) chmodown_( perm, usrname, grpname, fileslst[idxj] );
    }
    ++idxj;
  }
  free( fileslst );
 return 0;
}

chmodown_globb_files_ ( const char * perm, const char * usrname,
                       const char * grpname, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( !perm     )              return 15;
  if( !*perm    )              return 16;
  if( (usrname) && !*usrname ) return 17;
  if( (grpname) && !*grpname ) return 18;
  if( !usrname  &&  !grpname ) return 19;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    if( !directory_exists_( fileslst[idxj] )) {   // not a directory == others (any kind of file)
      (void) chmodown_( perm, usrname, grpname, fileslst[idxj] );
    }
    ++idxj;
  }
  free( fileslst );
 return 0;
}
chmodown_globb_ ( const char * perm, const char * usrname,
                  const char * grpname, const char * pattern ) {
  return chmodown_globb_files_( perm, usrname, grpname, pattern );
}


/////////////////////////////////////////////////////////////////////////////////////////////
// chmod_globb_: not-recursive, always derefs symlinks, ignore errors, ? applies to files/dirs ?

chmod_globb_all_ ( const char * perm, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( !perm     )              return 15;
  if( !*perm    )              return 16;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    (void) chmod_( perm, fileslst[idxj++] );
  }
  free( fileslst );
 return 0;
}

chmod_globb_dirs_ ( const char * perm, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( !perm     )              return 15;
  if( !*perm    )              return 16;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    if( directory_exists_( fileslst[idxj] )) {
      (void) chmod_( perm, fileslst[idxj] );
    }
    ++idxj;
  }
  free( fileslst );
 return 0;
}

chmod_globb_files_ ( const char * perm, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( !perm     )              return 15;
  if( !*perm    )              return 16;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    if( !directory_exists_( fileslst[idxj] )) {   // not a directory == others (any kind of file)
      (void) chmod_( perm, fileslst[idxj] );
    }
    ++idxj;
  }
  free( fileslst );
 return 0;
}
chmod_globb_ ( const char * perm, const char * pattern ) {
  return chmod_globb_files_( perm, pattern );
}


/////////////////////////////////////////////////////////////////////////////////////////////
// chown_globb_:   not-recursive, ignore errors, ? applies to files/dirs ?, (derefs)
// chown_h_globb_: applies to (the) symlink: [ chown_link_, chown_noderef_, chown_nodereflink_ ]

chown_globb_all_ ( const char * usrname, const char * grpname, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( (usrname) && !*usrname ) return 17;
  if( (grpname) && !*grpname ) return 18;
  if( !usrname  &&  !grpname ) return 19;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    (void) chown_( usrname, grpname, fileslst[idxj++] );
  }
  free( fileslst );
 return 0;
}

chown_globb_dirs_ ( const char * usrname, const char * grpname, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( (usrname) && !*usrname ) return 17;
  if( (grpname) && !*grpname ) return 18;
  if( !usrname  &&  !grpname ) return 19;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    if( directory_exists_( fileslst[idxj] )) {
      (void) chown_( usrname, grpname, fileslst[idxj] );
    }
    ++idxj;
  }
  free( fileslst );
 return 0;
}

chown_globb_files_ ( const char * usrname, const char * grpname, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( (usrname) && !*usrname ) return 17;
  if( (grpname) && !*grpname ) return 18;
  if( !usrname  &&  !grpname ) return 19;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    if( !directory_exists_( fileslst[idxj] )) {   // not a directory == others (any kind of file)
      (void) chown_( usrname, grpname, fileslst[idxj] );
    }
    ++idxj;
  }
  free( fileslst );
 return 0;
}
chown_globb_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_globb_files_( usrname, grpname, pattern );
}


chown_h_globb_all_ ( const char * usrname, const char * grpname, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( (usrname) && !*usrname ) return 17;
  if( (grpname) && !*grpname ) return 18;
  if( !usrname  &&  !grpname ) return 19;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    (void) chown_h_( usrname, grpname, fileslst[idxj++] );
  }
  free( fileslst );
 return 0;
}

chown_h_globb_dirs_ ( const char * usrname, const char * grpname, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( (usrname) && !*usrname ) return 17;
  if( (grpname) && !*grpname ) return 18;
  if( !usrname  &&  !grpname ) return 19;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    if( directory_exists_( fileslst[idxj] )) {
      (void) chown_h_( usrname, grpname, fileslst[idxj] );
    }
    ++idxj;
  }
  free( fileslst );
 return 0;
}

chown_h_globb_files_ ( const char * usrname, const char * grpname, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  )              return 13;
  if( !*pattern )              return 14;
  if( (usrname) && !*usrname ) return 17;
  if( (grpname) && !*grpname ) return 18;
  if( !usrname  &&  !grpname ) return 19;
  // ! this returns files, dirs/ and no .periods
  if(( globb_noadd_( pattern, &fileslst ))) {
    // on NO-MATCH: here
    if( fileslst ) free( fileslst );
    return 11;
  }
  while( fileslst[idxj] ) {
    if( !directory_exists_( fileslst[idxj] )) {   // not a directory == others (any kind of file)
      (void) chown_h_( usrname, grpname, fileslst[idxj] );
    }
    ++idxj;
  }
  free( fileslst );
 return 0;
}
chown_h_globb_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_files_( usrname, grpname, pattern );
}


chown_nodereflink_globb_all_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_all_( usrname, grpname, pattern );
}
chown_noderef_globb_all_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_all_( usrname, grpname, pattern );
}
chown_link_globb_all_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_all_( usrname, grpname, pattern );
}
chown_nodereflink_globb_dirs_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_dirs_( usrname, grpname, pattern );
}
chown_noderef_globb_dirs_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_dirs_( usrname, grpname, pattern );
}
chown_link_globb_dirs_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_dirs_( usrname, grpname, pattern );
}
chown_nodereflink_globb_files_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_files_( usrname, grpname, pattern );
}
chown_noderef_globb_files_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_files_( usrname, grpname, pattern );
}
chown_link_globb_files_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_files_( usrname, grpname, pattern );
}
chown_nodereflink_globb_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_files_( usrname, grpname, pattern );
}
chown_noderef_globb_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_files_( usrname, grpname, pattern );
}
chown_link_globb_ ( const char * usrname, const char * grpname, const char * pattern ) {
  return chown_h_globb_files_( usrname, grpname, pattern );
}

