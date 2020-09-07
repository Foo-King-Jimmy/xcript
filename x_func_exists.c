
// man 2 stat
// man 7 inode

#define   XCRPT_FLAG_SOCK    S_IFSOCK   // socket
#define   XCRPT_FLAG_LINK    S_IFLNK    // symbolic link
#define   XCRPT_FLAG_FILE    S_IFREG    // regular file
#define   XCRPT_FLAG_DIR     S_IFDIR    // directory
#define   XCRPT_FLAG_CHAR    S_IFCHR    // character (special) device
#define   XCRPT_FLAG_FIFO    S_IFIFO    // FIFO, named pipe
#define   XCRPT_FLAG_BLOCK   S_IFBLK    // block (special) device
#define   XCRPT_FLAG_EXIST   S_IFMT     // exists - bit mask for the file type bit field

/****
  if( character_device_exists_( name )) { }   // S_ISCHR(m)
  if(        character_exists_( name )) { }   // ? not a very good name
  if(     block_device_exists_( name )) { }   // S_ISBLK(m)
  if(            block_exists_( name )) { }   // ? not a very good name
  if(        directory_exists_( name )) { }   // S_ISDIR(m)
  if(           folder_exists_( name )) { }
  if(              dir_exists_( name )) { }
  if(     regular_file_exists_( name )) { }   // S_ISREG(m)
  if(             file_exists_( name )) { }   // ? not a very good name
  if(           socket_exists_( name )) { }   // S_ISSOCK(m)
  if(          symlink_exists_( name )) { }   // S_ISLNK(m)
  if(             link_exists_( name )) { }   // ? not a very good name
  if(       named_pipe_exists_( name )) { }   // S_ISFIFO(m)
  if(             pipe_exists_( name )) { }
  if(             fifo_exists_( name )) { }
  if(             FIFO_exists_( name )) { }
  if(             name_exists_( name )) { }
  if(               it_exists_( name )) { }
  if(                  exists_( name )) { }
****/

// NO globbing ( in these ):


exists_flag_ ( const char * name, mode_t flag ) {

 int  retstat;
 struct stat  mystatbuf;

  retstat = stat( name, &mystatbuf );
  if( retstat ) return 0;   // no additional error-checking; suppose no such file
  if( S_IFMT == flag ) return 1;   // (does) exist
  if(( S_IFMT & mystatbuf.st_mode ) == flag ) return 1;

 return 0;
}


// exists:
name_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFMT ); }
  it_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFMT ); }
     exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFMT ); }

// socket:
socket_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFSOCK ); }

// block dev:
block_device_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFBLK ); }
       block_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFBLK ); }

// character dev:
character_device_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFCHR ); }
       character_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFCHR ); }

// symlink:
symbolic_link_device_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFLNK ); }
      symlink_device_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFLNK ); }
                link_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFLNK ); }

// (regular) file:
regular_file_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFREG ); }
    reg_file_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFREG ); }
        file_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFREG ); }

// directory:
directory_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFDIR ); }
   folder_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFDIR ); }
      dir_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFDIR ); }

// FIFO, named pipe:
named_pipe_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFIFO ); }
      pipe_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFIFO ); }
      fifo_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFIFO ); }
      FIFO_exists_ ( const char * name ) { return exists_flag_( name, (mode_t) S_IFIFO ); }

