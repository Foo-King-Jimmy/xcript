

// ADD double-wrapper:  chmodown_( mode, owner, group, path );
// ? chmod_globb_ ? chown_globb_ ? chgrp_ missing
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
    E0( 14, in str2mode_ wrong length permission string given, modestr );
    return 14;
  }
  if( lettersnum != strspn( modestr, "01234567" )) {
    E0( 15, in str2mode_ wrong characters in permission string, modestr );
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
  E0( chmod( path, newmode ), in chmod_ chmod syscall failed on pathname, path );
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
      E0( 16, in chown_raw_ getpwname failed with unknown username, usrname );
      return 16;
    }
    newowner = pwdstrctp->pw_uid;
  }
  if( (grpname) ) {
    grpstrctp = getgrnam( grpname );
    if( !grpstrctp ) {
      E0( 17, in chown_raw_ getgrname failed with unknown groupname, grpname );
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
//
// newly created directory will be owned by the effective user ID of the process
//
// + how to globb in mkdir_ ?
//
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
  E0( mkdir( path, themode ), in mkdir_ mkdir syscall failed on pathname, path );
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
  E0( mkdir( path, themode ), in mkdir_forcemode_ mkdir syscall failed on pathname, path );
  (void) umask_raw_set_( oldmask );
  if( -1 == $errno ) return 16;
 return 0;
}
mkdir_mode_force_ ( const char * perm, const char * path ) {
  return mkdir_mode_force_( perm, path );
}

