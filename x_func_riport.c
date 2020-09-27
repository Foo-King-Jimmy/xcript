

// ls_(), ps_(), head_(), tail_()


/////////////////////////////////////////////////////////////////////////////////////////////

#if  0


recursive (find) listing function that malloc_ a char **


//  ls   ( can be done w globb, can be used in cp/mv/rm recursive )
ls simple => easy by globb => do not do globb just like this! what if path is funky w * or sg else
ls_( path );
>> path does not exist
>> path dir: fdiropen
>> path file ????????


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


#endif

