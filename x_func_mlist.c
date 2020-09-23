
#define   XCRPT_MALLOC   (1)
#define   XCRPT_MMAP     (2)


// Development idea:  remove a (formerly allocated) memory address from the mlist
/** PUBLIC **/
mlist_remove_( void * maddr ) {
  // ...
  // remove/evict maddr (if it is real) from mlist ...
  // ...
 return 11;
 return 0;
}
mlist_rm_    ( void * maddr ) { return mlist_remove_( maddr ); }
mlist_evict_ ( void * maddr ) { return mlist_remove_( maddr ); }
mlist_delete_( void * maddr ) { return mlist_remove_( maddr ); }


/** STATIC **/
mlist_free_( void ) {

 int  localc = 0,
      idxj   = 0;
 struct mbox_ * mbx  = NULL,
              * mbx0 = NULL;

  if(    !$mlistc ) return  0;   // nothing to do
  if( 0 > $mlistc ) return 11;   // error
  if(    !$mlistp ) return 12;   // that is an error
  localc = $mlistc;
  mbx = mbx0 = $mlistp;
  for( ; ; ) {
    if( XCRPT_MALLOC == mbx->type ) free( mbx->addr );
    if( XCRPT_MMAP   == mbx->type ) (void) munmap( mbx->addr, mbx->size );
    mbx->addr = NULL;
    mbx->size = (size_t) 0;
    mbx->type = (char) 0;
    ++mbx;
    ++idxj;
    if( idxj == localc ) break;
    if( 10 == idxj ) {
      mbx = mbx->addr;
      free( mbx0 );
      mbx0 = mbx;
      localc -= 10;
      idxj    =  0;
    }
  }
  free( mbx0 );
  $mlistt = (size_t) 0;
  $mlistc =          0;
  $mlistp = NULL;

 return 0;
}


/** STATIC **/
mlist_add_( char mtype, void * mptrd, size_t msize ) {

 static struct mbox_ * mbx;
 struct mbox_ * mbx0;

  if( !mptrd ) return 11;
  if( !msize ) return 12;
  if( XCRPT_MMAP != mtype && XCRPT_MALLOC != mtype ) return 13;
  if( $mlistc % 10 ) ++mbx;
  else {
    mbx0 = malloc( sizeof(struct mbox_) * 11 );
    if( !mbx0 ) return 14;
    (void) memset( mbx0, 0, ( sizeof(struct mbox_) * 11 ));
    if( !$mlistc ) {
      $mlistp = mbx = mbx0;
    }
    else {
      ++mbx;
      mbx->addr = mbx0;
      mbx = mbx0;
    }
  }
  mbx->type = mtype;
  mbx->addr = mptrd;
  mbx->size = msize;
  ++$mlistc;
  $mlistt += msize;

 return 0;
}


/** PUBLIC **/
mmap_flags_( size_t msize, void **mptr, int mprot, int mflags ) {
  if( !mptr )  return 11;
  *mptr = NULL;
  if( !msize ) return 12;
  *mptr = mmap( NULL, msize, mprot, mflags, -1, (off_t) 0 );
  if( MAP_FAILED == *mptr ) {
    *mptr = NULL;
    return 13;
  }
  if( !( MAP_ANONYMOUS & mflags )) (void) memset( *mptr, 0, msize );
  (void) mlist_add_( XCRPT_MMAP, *mptr, msize );
 return 0;
}


/** PUBLIC **/
mmap_( size_t msize, void **mptr ) {
  return mmap_flags_( msize, mptr, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS );
}


/** PUBLIC **/
malloc_( size_t msize, void **mptr ) {
  if( !mptr )  return 11;
  *mptr = NULL;
  if( !msize ) return 12;
  *mptr = malloc( msize );
  if( !*mptr ) return 13;
  (void) memset( *mptr, 0, msize );
  (void) mlist_add_( XCRPT_MALLOC, *mptr, msize );
 return 0;
}


/** STATIC **/   // ? mostly
malloc_noadd_( size_t msize, void **mptr ) {
  if( !mptr )  return 11;
  *mptr = NULL;
  if( !msize ) return 12;
  *mptr = malloc( msize );
  if( !*mptr ) return 13;
  (void) memset( *mptr, 0, msize );
 return 0;
}

