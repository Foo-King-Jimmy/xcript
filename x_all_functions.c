
///  TODO :

//:  {{ It's up to the programmer to decide when to quit/fatal exit. }}
//:  {{ DONT DO define-wrappers ! write full-wrappers !   // see eg. kill_ in others }}
//:  {{ check all prev/done files and fill up TODO w tasks from them }}
//:  {{ add ssh-like modul/interface }}
//:  NO: {{ xcript > in main() should chdir() to script's dir (to be in its pwd) }} :NO
//:  reintroduce (again) negative error-values
//:  and feed these into my strerror_() func on char * errorc[] to errdisp_()
//
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
#include  <xcript/x_func_mount.c>     //
#include  <xcript/x_func_riport.c>    //  ?:>  ls_(), ps_(), head_(), tail_()


/////////////////////////////////////////////////////////////////////////////////////////////
// GLOBB: +  rm_globb_  rm_f_globb_ USED already  rm_r_globb_
//        +  mv_globb_
//        +  mkdir_globb_  mkdir_m_globb_
// -  run_redirection_
// -  mv_ dir+file
// -  read U; < /tmp/users    ???         <== getline(3)
// -  grep -q ???             quiet
// -  evict from mlist - just make it NULL and XCRPT_EVICTD - keep the queue
// -  date [ format ]         timefmt
//////////////////////////////////////////////////////////////////////////////////////////

#if  0

  $redirection_default = NULL;
  $redirection         = NULL;


//------------------------------------------//
#define   STD0      "\00"
#define   STD1      "\01"
#define   STD2      "\02"
#define   STDIN_    "\00"
#define   STDOUT_   "\01"
#define   STDERR_   "\02"
 const char  std0   [ ] = STD0,
             std1   [ ] = STD1,
             std2   [ ] = STD2,
             stdin_ [ ] = STDIN_,
             stdout_[ ] = STDOUT_,
             stderr_[ ] = STDERR_;
 const char * $redirection[ 3 ];
  $redirection = NULL;
  $redirection = { NULL,   NULL,    NULL };
  $redirection = { std0,   std1,    std2 };
  $redirection = { stdin_, stdout_, stderr_ };
//------------------------------------------//


  $redirection = { stdin, fh_out, fh_err };

  $redirection = { "/tmp/LOG", "/tmp/LOG-OUT", "/tmp/LOG-ERR" };
  $redirection = { "\00",      "\01",          "\02"          };
#define   fd_      "\03"
#define   fdesc_   "\03"
#define   fh_      "\04"
#define   fhandle_ "\04"
#define   stream_  "\04"


+>  /home/jimmy/lice/commands/jjc.c


  prog -opt param1 param2   2> errors.txt  1> errors.txt
  ---
? redirection_push_( 1, "filename1" );
? redirection_push_( 2, "filename1" );
  run_( "gcc -c this" );
? redirection_pop_();


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


mv=cp+rm       => easy
//  mv_() :  mv /webadmin/favicon.ico /favicon.ico
Mv ( const char *oldName, const char *newName ) {
  int  retv;
    if(( retv = Cp( oldName, newName ))) return retv;
    if(( retv = Rm( oldName )))          return -5;
 return 0;
}



///////////////////////////////////////////////////////////////////////////////////////
/*************************************************************************************/

LIST ALL FUNCTION THAT CAN/NEEDS GLOBBING !
// GLOBBING for functions

// ? Yes:  rm     ./*
           rm  -r ./*
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
run_ifwait_       run_  run_wait_  run_inbg_
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
mount_
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

