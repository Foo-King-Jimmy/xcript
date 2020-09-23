
///  TODO :

//:  {{ It's up to the programmer to decide when to quit/fatal exit. }}
//:  DONT DO define-wrappers ! write full-wrappers !   // see eg. kill_, rmdir_ in others
//:  check all prev/done files and fill up TODO w tasks from them
//:  reintroduce (again) negative error-values
//:  and feed these into my strerror_() func on char * errorc[] to errdisp_()
//:  NO: { xcript > in main() should chdir() to script's dir (to be in its pwd) } :NO
//
//:  extend x_func_change.c w rmdir_globb(_cont)_
//:  finish x_func_mount.c
//:  mount_() will use init/parse /proc/filesystems based on static init-var
//:  run_() should receive child's return value (uint8_t) by waitid()
//:  in x_func_cp.c there are questions ! about permissions/owners/etc.
//:  finish in x_func_change.c  chgrp_()  [ mkdir_globb_ ]
//:  regex should be extended by a func that mmaps a file into str and then does regex
//:  may write $umask version that gets/display string (not mode_t): "022"  { get_umask_(&str) }
//:  NEEDs getpid/getppid that gets output/return value into ptr (not just $pid)


#include  <xcript/x_func_errdisp.c>   //  E( func(arg1,arg2), stringified error message );
#include  <xcript/x_func_mlist.c>     //  malloc_(), mmap_(), mmap_flags_()
#include  <xcript/x_func_globb.c>     //  globb_( const char * pattern, char ***resultp );
#include  <xcript/x_func_regex.c>     //  regex_( regex, instring, void **storeptr );
#include  <xcript/x_func_exists.c>    //
#include  <xcript/x_func_run.c>       //
#include  <xcript/x_func_others.c>    //  sleep,pwd,cd,ln,getpid,touch,basename,dirname,rmdir,kill
#include  <xcript/x_func_print.c>     //  print_(), printf_(), fprintf_ret_(), echo_(), ...
#include  <xcript/x_func_change.c>    //  chmod_(), chown_(), chgrp_(), chmodown_(), mkdir_(), rm_()
#include  <xcript/x_func_cp.c>        //  cp_full_(), cp_raw_(), cp_raw_recursive_()
// #include  <xcript/x_func_mount.c>     //  ???
// #include  <xcript/x_func_riport.c>    //  ls_(), ps_(), head_(), tail_()


/////////////////////////////////////////////////////////////////////////////////////////////


// change rewrites / rewrites_... remove globb_ change to chmodown_globb_ etc.


/*
*/
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/*
// chown_globb_:   not-recursive, ignore errors, ? applies to files/dirs ?, (derefs)
// chown_h_globb_: applies to (the) symlink: [ chown_link_, chown_noderef_, chown_nodereflink_ ]
chown_globb_dirs_ ( const char * usrname, const char * grpname, const char * pattern ) {
 int  idxj = 0;
 char **fileslst = NULL;
  if( !pattern  ) return 13;
  if( !*pattern ) return 14;
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
*/
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//  GLOBB:  rm_  rm_f_  rmdir_  mkdir_m_    rm_r_  mv_


// run( str ... )  run_redirection_        -ok-
// mount_auto_  mount umount            umount
// mv_ dir+file
// read U; < /tmp/users    ???         <== getline(3)
// date [ format ]         timefmt
// grep -q ???             quiet
// ? evict from mlist ?



/* MOUNT() -> static init parse ********************************

 const char  fstypesrc[ ] = "/proc/filesystems";
 FILE * fhandle = NULL;
 char * tmpp    = NULL,
        buff[ ??? ];
 int  charx,
      loopbreak = 0;

  fhandle = fopen( fstypesrc, "rb" );
  if( !fhandle ) ERROR;
  while( 1 ) {
    charx = fgetc( fhandle );
    if( EOF  == charx ) break;
    if( '\t' == charx ) {   \t sgsgsg \n


readin till \n/eof
 char * tmpp    = NULL,
        buff[ ??? ];


    }
    else {   // nodev \t sgsgsg \n
      if( '\n' == charx ) continue;
      else {
        while( 1 ) {
          charx = fgetc( fhandle );
          if( EOF  == charx ) { loopbreak = 1; break; }
          if( '\n' == charx ) break;
        }
      }
    }
    if( loopbreak ) break;
  }

====== ***********************************/

//////////////////////////////////////////////////////////////////////////////////////////

#if  0
+>  /home/jimmy/lice/commands/jjc.c


  run_( "strip -s libjitcc_1.so" );
? run_( "strip -s %s", var_libjitcc );   // sizeof(buf)==?
  ---
  prog -opt param1 param2   2> errors.txt  1> errors.txt
  ---
? redirection_push_( 1, "filename1" );
? redirection_push_( 2, "filename1" );
  run_( "gcc -c this" );
? redirection_pop_();


recursive (find) listing function that malloc_ a char **

readlink like function that readlinks/malloc_ return

tee   ( ? duplicate output ? you mean cp this to that1+that2 ? )  :nothing

?curl ? can be called by run_ OR can be inetrnal as well

+>  custom globb needed : globb files only, globb dirs only, globb (no)dots too, etc.



+>  add my own file mmap()   mmap_file_
#include  <sys/mman.h>
C_mmap_file ( const char * filename, FILE * fh, void **textp, size_t * textlengthp ) {

 int  mode  =  1,
      fdesc = -1,
      acharx;
 char * tmpcharp = NULL;
 size_t  tmpsizet = (size_t) 0;
 long  loffset = (long) -1;
 FILE * fh_int = NULL;

  if( NULL == textlengthp )             return -1;
  if( NULL == textp )                   return -2;
  if( NULL == filename && NULL == fh )  return -3;

  if( NULL == filename ) { mode = 0; fh_int = fh; }
  else { if( NULL == ( fh_int = fopen( filename, "rb" )))  return -4; }
  if( -1 == ( fdesc = fileno( fh_int ))) {
    if( mode )  (void) fclose( fh_int );
    return -5;
  }
  if( -1 == fseek( fh_int, (long) 0, SEEK_END )) {
    if( mode )  (void) fclose( fh_int );
    return -6;
  }
  loffset = ftell( fh_int );
  if( (long) 0 > loffset ) {
    if( mode )  (void) fclose( fh_int );
    return -7;
  }
  *textlengthp = (size_t)( (long) 1 + loffset );
  (void) rewind( fh_int );
  *textp = mmap( NULL, *textlengthp, (int)( PROT_READ | PROT_WRITE | PROT_EXEC ),
                 (int)( MAP_PRIVATE | MAP_ANONYMOUS ), -1, (off_t) 0 );
  if( MAP_FAILED == *textp ) {
    if( mode )  (void) fclose( fh_int );
    return -8;
  }
  // *textp area is zeroed
  tmpcharp = (char *)( *textp );
  while( 1 ) {
    acharx = fgetc( fh_int );
    if( EOF == acharx )  break;
    tmpcharp[tmpsizet] = (char)( 0xFF & acharx );
    ++tmpsizet;
  }

  if( mode )  (void) fclose( fh_int );
  else        (void) rewind( fh_int );

 return 0;
}




//  ls   ( can be done w globb, can be used in cp/mv/rm recursive )
ls simple => easy by globb => do not do globb just like this! what if path is funky w * or sg else
ls_( path );
>> path does not exist
>> path dir: fdiropen
>> path file ????????



cp+rm=mv       => easy
//  mv_() :  mv /webadmin/favicon.ico /favicon.ico
Mv ( const char *oldName, const char *newName ) {
  int  retv;
    if(( retv = Cp( oldName, newName ))) return retv;
    if(( retv = Rm( oldName )))          return -5;
 return 0;
}



ps, head, tail => skip for now
DummySort ( const struct dirent **dirListA, const struct dirent **dirListB ) { return 0; }

SelectPIDNumberStrings ( const struct dirent * dirListMember ) {
  if( strlen( dirListMember->d_name ) == strspn( dirListMember->d_name, "1234567890" )) {
    if( (size_t) 21 > strlen( dirListMember->d_name )) return 1;
  }
 return 0;
}

Ps ( const char *outName ) {
------------
// www.stackoverflow.com/q/33266678
// www.stackoverflow.com/q/38366362
// www.stackoverflow.com/q/1420426
// www.unix.stackexchange.com/questions/163145
// www.man7.org/linux/man-pages/man5/proc.5.html
// /proc/[pid]/cmdline
//    This read-only file holds the complete command line for the process, unless \
      the process is a zombie.  In the latter case, there is nothing in this file: \
      that is, a read on this file will return 0 characters. The command-line \
      arguments appear in this file as a set of strings separated by null bytes \
      ('\0'), with a further null byte after the last string. Think of this file as \
      the command line that the process wants you to see.
------------
// PID:
 int  retscan, myerrno, idx; struct dirent **dirlist;
  errno = 0;
  if( 0 > ( retscan = scandir( "/proc", &dirlist, SelectPIDNumberStrings, DummySort ))) {
    myerrno = errno;
    // ?! (void) fprintf( stderr, " ERROR: scandir(): %d, %s .\n", myerrno, strerror( myerrno ));
    return -9;
  }
  for( idx = 0; idx < retscan; ++idx ) (void) printf( "%s\t", dirlist[idx]->d_name );
  // ?! (void) fputc( '\n', stdout );
  (void) fflush( NULL );
------------
// UID:  // USER:
struct stat mystat;
uid_t * uidarray = malloc( retscan * sizeof(uid_t) );
if x  NULL error
18446744073709551616
12345678901234567890 == 20
dirlist[idx]->d_name
string[ 28 ];
if( (size_t) 21 > strlen( dirlist[idx]->d_name ))
(void) sprintf( string, "/proc/%s", dirlist[idx]->d_name );
int retv = stat( string, &mystat );
if retv == -1 ERROR  ==>  uidarray[idx] = (uid_t) 0;
else               ==>  uidarray[idx] = mystat.st_uid;
------------
But normally the UID of the process can be retrieved by an fstat call (or a stat command ) on the /proc/[pid] directory itself.
/proc/<PID>  #directory# == pathname
int stat(const char *pathname, struct stat *statbuf);
struct stat { uid_t     st_uid;         /* User ID of owner */
              gid_t     st_gid;         /* Group ID of owner */
------------
// CMD:  // COMMAND:  cat /proc/804/cmdline
------------
// TIME:
------------
 return 0;
}



Head ( const char *sourceName, int lines, (char separator,) const char *outputName ) {
// Head( "./thisfile", 5 /*lines*/, '\n', "./writeithere" );
// may write a memfd_ file first, mmap() the content
 return 0;
}



///////////////////////////////////////////////////////////////////////////////////////
/*************************************************************************************/

LIST ALL FUNCTION THAT CAN/NEEDS GLOBBING !

// GLOBBING for functions
//
// ? Yes:  rm       ./*
           rm    -r ./*
           rmdir    ./mydirs*
           cp       ./*   to/
           cp    -r ./*   to/
           mv ...
          (mkdir)

///////////////////////////////////////////////////////////////////////////////////////
/*************************************************************************************/
/* formerly:  x_x.c */

// REDIRECTION of stdin, stdout, stderr
// EXAMINE redirection for these funcions:
// points:  ? is it a new process   ? does it use stdout actively

================================================================
- REDIRECTION NEEDED, IMPORTANT: -------------------------------

OK(stdout):  print_  print_void_  print_ret_
OK(stdout):  echo_ echo_append_ echo_err_ echo_file_
run_ifwait_ ( char * cmdline, int ifwaitflag ) {        run_  run_wait_  run_inbg_
? curl ? can be called by run_ OR can be inetrnal as well
ls simple      => easy by globb     ( can be done w globb, can be used in cp/mv/rm recursive )
//  tee   ( ? duplicate output ? you mean cp this to that1+that2 ? )  :nothing
Head ( const char *sourceName, int lines, (char separator,) const char *outputName ) {
tail


================================================================
- REDIRECTION NEEDLESS, IRELEVANT: -----------------------------

fprintf_ret_  ( int * retprnt, FILE * stream, const char * format, ... ) { fprintf_void_  fprintf_
printf_ret_  ( int * retprnt, const char * format, ... ) { printf_void_  printf_
mlist_free_( void )    mlist_add_( char mtype, void * mptrd, size_t msize ) {
mmap_flags_( size_t msize, void **mptr, int mprot, int mflags )  mmap_
+>  add my own file mmap()   mmap_file_
malloc_( size_t msize, void **mptr ) {
str2mode_ ( const char * modestr, mode_t * retmode ) {
//  rm_() :  rm -f /usr/local/lib/libsqlite3.la
//  rm  =>  remove(), unlink()  rm_ is recursive or rm_r_
//          rm_file_   //NO: define ? rm_ unlink   define rm_ignore_ (void) unlink
//  mv_()    Mv ( const char *oldName, const char *newName ) {
snprintf_ret_(int *retprnt,char *buf,size_t bufsz,const char *frmt,...) snprintf_void_  snprintf_
sprintf_ret_ (int *retprnt, char *strbuf, const char *format,...)   sprintf_void_  sprintf_
cp_raw_ ( const char * srcname, const char * trgtname ) {
cp_raw_recursive_ ( const char * srcname, const char * trgtname ) {
cp_full_ ( const char * srcname, const char * trgtname, int nooverwrite, int recursive ) {
   cp_   cp_safe_   cp_recursive_   cp_r_   cp_r_safe_   cp_recursive_safe_
// cp_array_( const char * src, const char * new )   // cp_files_  cp_dir_  cp_r_  cp_globb_
chmod_ ( const char * perm, const char * path ) {
  // ? chmod_globb_ ? chown_globb_ ? chgrp_ missing
chown_raw_ ( const char * usrname, const char * grpname, const char * path, int nodereflink ) {
chown_  chown_h_  chown_link_  chown_noderef_  chown_nodereflink_  chmodown_()
mkdir_m_ ( const char * perm, const char * path ) { mkdir_mode_  mkdir_
mkdir_forcemode_ ( const char * perm, const char * path ) { mkdir_mode_force_
umask_raw_get_ ( mode_t * umaskp ) { umask_raw_set_ ( mode_t newumask ) {
umask_raw_ret_ ( mode_t newumask, mode_t * oldumaskp ) { umask_raw_flip_   umask_raw_
umask_set_ ( const char * newumaskstr ) {
umask_ret_ ( const char * newumaskstr, mode_t * oldumaskp ) { umask_flip_   umask_
sleep_force_(int secs)  sleep_ret_(int secs, int * retsecs )  sleep_void_( int secs)  sleep_
symlink_ ( const char * target, const char * linkpath )    ln_s_    symlink_re_    ln_sfn_
kill_ ( pid_t procid, int signalnum ) {
chdir_ ( const char * path ) {  cd_
// mount static init parse > mount
exists_flag_ ( const char * name, mode_t flag ) name_exists_   it_exists_   exists_ socket_exists_
rmdir_ ( const char * dirpath )   rmdir_globb_  rmdir_globb_cont_ 
touch_ ( const char * fname ) {   // simple touch   // touch_globb_ 
   ps: DummySort ( const struct dirent **dirListA, const struct dirent **dirListB )
   ps: SelectPIDNumberStrings ( const struct dirent * dirListMember ) {

---
getpid_ ( void ) {   getppid_
dirname_ ( const char * pathstr, char **retname ) {
basename_( const char * name, char **basenamep ) {
pwd_ ( char **pwdstrp ) {    cwd_
regex_flags_(const char *rgxpttrn,const char *instr,void **storep,int rgcmpfl,int rgxecfl); regex_
globb_flags_( const char * pattern, char ***resultp, int addflags )    globb_  etc...
globberr_( const char * epath, int eerrno )


================================================================
- REDIRECTION DUBIOUS: -----------------------------------------

Ps ( const char *outName ) {

errdisp_( const char * srcfname, int linenum, const char * msg, const char * xstr ) {

? redirection_push_( 1, "filename1" );
? redirection_push_( 2, "filename1" );
  run_( "gcc -c this" );
? redirection_pop_();
================================================================


/*************************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////

#endif

