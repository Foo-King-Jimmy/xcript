
///  TODO :
//:  DONT DO define-wrappers ! write full-wrappers !   // see eg. kill_, rmdir_ in others
//:  check all prev/done files and fill up TODO w tasks from them
//:  mount_() will use init/parse /proc/filesystems based on static init-var
//:  reintroduce (again) negative error-values
//:  and feed these into my strerror_() func on char * errorc[] to errdisp_()
//:  run_() should receive child's return value (uint8_t) by waitid()
//:  finish x_func_mount.c
//:  finish echo_() in x_func_print.c
//:  finish cp_raw_recursive_ in x_func_cp.c
//:  custom globb_() needed: -files-only, -dirs-only, -(no-)dots, ...
//:  extend x_func_others.c w rmdir_globb(_cont)_
//:  finish in x_func_change.c  chmodown_(), chgrp_()  [ mkdir_globb_ ]
//:  NO: { xcript > in main() should chdir() to script's dir (to be in its pwd) } :NO
//:  may write $umask version that gets/display string (not mode_t): "022"  { get_umask_(&str) }
//:  ? change regex_ inetrnal is w 0 flag only w adding-flag option ( not w taking-off-flags )
//:  NEEDs getpid/getppid that gets output/return value into ptr (not just $pid)


#include  <xcript/x_func_errdisp.c>   //  E( func(arg1,arg2), stringified error message );
#include  <xcript/x_func_mlist.c>     //  malloc_(), mmap_(), mmap_flags_()
#include  <xcript/x_func_globb.c>     //  globb_( const char * pattern, char ***resultp );
#include  <xcript/x_func_regex.c>     //  regex_( regex, instring, void **storeptr );
#include  <xcript/x_func_exists.c>    //
#include  <xcript/x_func_run.c>       //
#include  <xcript/x_func_others.c>    //  sleep,pwd,cd,ln,getpid,touch,basename,dirname,rmdir,kill
#include  <xcript/x_func_cp.c>        //  cp_full_(), cp_raw_(), cp_raw_recursive_()
#include  <xcript/x_func_print.c>     //  print_(), printf_(), fprintf_ret_(), echo_(), ...
#include  <xcript/x_func_change.c>    //  chmod_(), chown_(), chgrp_(), chmodown_(), mkdir_()
// #include  <xcript/x_func_mount.c>     //  ???
// #include  <xcript/x_func_riport.c>    //  ls_(), ps_(), head_(), tail_()


//////////////////////////////////////////////////////////////////////////////////////////


// how to dereference a symlink?: symlink -> realfile ? use man 2 readlink() !
// if path==symlink ? rm symlink ?OR? rm deref(symlink)
// rm_deref_(?)   // use readlink to dereference a symlink
// rm_r_  rm_recursive_  missing
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


  run_( "strip -s libjitcc_1.so" );
? run_( "strip -s %s", var_libjitcc );   // sizeof(buf)==?

  prog -opt param1 param2   2> errors.txt  1> errors.txt

? redirection_push_( 1, "filename1" );
? redirection_push_( 2, "filename1" );
  run_( "gcc -c this" );
? redirection_pop_();

  echo_( " %s: error: can not find gke.o .", $argv[0] )  &&  exit( 1 );

----------------------------------------------------

  ? curl ? can be called by run_ OR can be inetrnal as well
  cp+rm=mv       => easy
  ps, head, tail => skip for now
  ls simple      => easy by globb

============================================================================================
+>  custom globb needed : globb files only, globb dirs only, globb (no)dots too, etc.
+>  /home/jimmy/lice/commands/jjc.c
+>  add my own file mmap()   mmap_file_
+>  Ef ~= Error_fatal maco needed that at the end exit( 1 ) ... may mlist_free ???
+>  Ef can only be called from scripts not inetrnal functions
+>  it is up to the programmer to decide when to quit/fatal exit
-----------------------------------------------------
//  ls   ( can be done w globb, can be used in cp/mv/rm recursive )
//  mv_() :  mv /webadmin/favicon.ico /favicon.ico
//  tee   ( ? duplicate output ? you mean cp this to that1+that2 ? )  :nothing


Mv ( const char *oldName, const char *newName ) {
  int  retv;
    if(( retv = Cp( oldName, newName ))) return retv;
    if(( retv = Rm( oldName )))          return -5;
 return 0;
}


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


/* formerly:  x_x.c */

// REDIRECTION of stdin, stdout, stderr
// EXAMINE redirection for these funcions:
// points:  ? is it a new process   ? does it use stdout actively

================================================================
- REDIRECTION NEEDED, IMPORTANT: -------------------------------

OK(stdout):  print_  print_void_  print_ret_
run_ifwait_ ( char * cmdline, int ifwaitflag ) {        run_  run_wait_  run_inbg_
echo_("x",NULL) echo_("x","file") echo_append_("x","file") echo_out_ echo_err_ echo_file_   echo_
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
   chown_  chown_h_  chown_link_  chown_noderef_  chown_nodereflink_
// chmodown_( mode, owner, group, path );
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
globb_flags_( const char * pattern, char ***resultp, int addflags, int offflags )    globb_
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

