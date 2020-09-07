
///  TODO :
//:  DONT DO define-wrappers ! write full-wrappers !   // see eg. cd_ in others
//:  check all prev/done files and fill up TODO w tasks from them
//:  write cp_() func
//:  mount_() will use init/parse /proc/filesystems based on static init-var
//:  reintroduce (again) negative error-values
//:  and feed these into my strerror_() func on char * errorc[] to errdisp_()
//:  run_() should receive child's return value (uint8_t) by waitid()
//:  BY DEFAULT: xcript > in main() should chdir() to script's dir (to be in its pwd)
//:  dirname_() needed into x_func_others.c
//:  finish x_func_mount.c
//:  finish x_func_cp.c
//:  finish x_func_print.c + echo() too


#include  <xcript/x_func_errdisp.c>   //  E( func(arg1,arg2), stringified error message );
#include  <xcript/x_func_mlist.c>     //  malloc_(), mmap_(), mmap_flags_()
#include  <xcript/x_func_globb.c>     //  globb_( const char * pattern, char ***resultp );
#include  <xcript/x_func_regex.c>     //  regex_( regex, instring, void **storeptr );
#include  <xcript/x_func_exists.c>    //
#include  <xcript/x_func_run.c>       //
#include  <xcript/x_func_others.c>    //  sleep,pwd,cd,ln,getpid,touch,basename,dirname
// #include  <xcript/x_func_mount.c>     //  ???
// #include  <xcript/x_func_cp.c>        //  cp_full_( ? );
#include  <xcript/x_func_print.c>     //  printf_(), fprintf_ret_(), echo_(), ...


//////////////////////////////////////////////////////////////////////////////////////////
// dirname_()   : if not-dir malloc till last slash, if dir !?malloc [append] w slash
// mkdir_()     : mkdir -m775 /tmp/LOG
// mv_()        : mv /webadmin/favicon.ico /favicon.ico
// rm_()        : rm -f /usr/local/lib/libsqlite3.la
// rmdir_()
// ps_()
// head_()
// tail_()
// chmod_()     : chmod 550 /root/jdaemon
// chown_()     : chown -h root:root /sbin/reboot
#if  0



// kill wrapper
// chmod func
// chown func
// rmdir wrappper
// mkdir func




============================================================================================
   rm_file_   //NO: define ? rm_ unlink   define rm_ignore_ (void) unlink
============================================================================================

//NO: #define    rmdir_   rmdir
rmdir_globb_      (      ) { }
rmdir_globb_cont_ (      ) { }    cont(inue) >> --ignore-fail-on-non-empty
int rmdir(const char *pathname);

//  chmod
chmod mode filename
chmod m0d3 name > globb_ file1 fileN
chmod only files
chmod only dirs
-----------------------------------------------------
+> add my own file mmap()   mmap_file_
+> Ef ~= Error_fatal maco needed that at the end exit( 1 ) ... may mlist_free ???
-----------------------------------------------------
// /home/jimmy/lice/commands/jjc.c
//  kill  =>  kill()    //NO: #define   kill_   kill
//  rm  =>  remove(), unlink()  rm_ is recursive or rm_r_
//  mkdir  =>  mkdir()  mkdir_ : mkdir( x, ?string?mode or default )
//  tee
//  ls
//  ifconfig


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


#endif

