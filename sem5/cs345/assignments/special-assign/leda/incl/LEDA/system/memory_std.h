/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  memory_std.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_MEMORY_STD_H
#define LEDA_MEMORY_STD_H

LEDA_BEGIN_NAMESPACE


#define STDMEM_TABLESIZE 256


struct  memory_elem_type { 
 memory_elem_type* next; 
};

typedef memory_elem_type* memory_elem_ptr;


class __exportC memory_manager {

 friend class __exportC memory_management;
 friend class __exportC memory_manager_init;

#if defined(LEDA_MULTI_THREAD)
spinlock* lck;
#endif


 int              max_sz;
 char             name[64];
 unsigned         id;

 long int         alloc_count;
 long int*        total_count;
 long int*        block_count;
 memory_elem_ptr* block_list;
 memory_manager*  next;

 unsigned         save_count; // to allow nested save/restore
 long int         save_alloc_count;
 long int*        save_total_count;
 long int*        save_block_count;
 memory_elem_ptr* save_block_list;
 memory_manager*  save_next;
 memory_elem_ptr* save_free_list;

 void init(int, const char* = 0, unsigned=0, bool top_level = true);
 void destroy();

public:

/*
#if defined(LEDA_MULTI_THREAD)
 void   lock(int i) { lck[i].lock();   }
 void unlock(int i) { lck[i].unlock(); }
#else
 void   lock(int) {}
 void unlock(int) {}
#endif
*/

 memory_elem_ptr* free_list; // needed in handle_types.h

 memory_manager(int sz=STDMEM_TABLESIZE);
 memory_manager(int sz, const char* label, unsigned id); 
~memory_manager();

 void  allocate_block(size_t,int);

 void* allocate_bytes(size_t);
 void* allocate_vector(size_t);
 void* allocate_bytes_with_check(size_t);

 
 void deallocate_bytes(void*, size_t);
 void deallocate_vector(void*);
 void deallocate_vector_with_delay(void*);
 void deallocate_bytes_with_check(void*,size_t);

 void* allocate_list(size_t,int);
 void deallocate_list(void*, void*, size_t);

 void* allocate_words(size_t w) 
 { return allocate_bytes(w*sizeof(void*)); }

 void  deallocate_words(void* p,size_t w) 
 { deallocate_bytes(p,w*sizeof(void*)); }

 void  test_limit();
 
 void clear();

 void save();
 void restore();

 void push() { save(); }
 void pop()  { restore(); }

 void kill();
 void statistics_for_blocksize(int blk_sz, long& used_chks, long& free_chks, long& blk_cnt, long& bytes);
 void print_statistics();
 void print_used_memory();
 void check_free_list(int bytes);
 int  max_size() { return max_sz; }
 int  used_memory();
};



// standard memory manager

extern __exportD memory_manager std_memory_mgr;

#ifdef std_memory
#undef std_memory
#endif

#define std_memory  std_memory_mgr



class __exportC memory_manager_init {

public:

static unsigned count;

  memory_manager_init();
 ~memory_manager_init();
};

static memory_manager_init memory_mgr_init;


#if defined(__BORLANDC__) || defined(__DMC__) || defined(__mipspro__)
#define MATCHING_DELETE
#else
#define MATCHING_DELETE \
void  operator delete(void*,void*)\
{ LEDA_SCOPE default_error_handler(1,"delete(void*,void*) called."); }
#endif


LEDA_END_NAMESPACE


#define LEDA_MEMORY_STD(type)\
\
void* operator new(size_t bytes)\
{ if (bytes < STDMEM_TABLESIZE)\
  { /* leda_std_memory_mgr.lock(bytes); */\
    leda_memory_elem_ptr* q = leda_std_memory_mgr.free_list+bytes;\
    if (*q==0) leda_std_memory_mgr.allocate_block(bytes,-1);\
    leda_memory_elem_ptr p = *q;\
    *q = p->next;\
   /* leda_std_memory_mgr.unlock(bytes); */\
    return p; }\
  else return leda_std_memory_mgr.allocate_bytes(bytes);\
 }\
\
void* operator new(size_t, void* p) { return p; }\
\
void  operator delete(void* p, size_t bytes)\
{ if (p != 0)\
  { if (bytes < STDMEM_TABLESIZE)\
    { /* leda_std_memory_mgr.lock(bytes); */\
      leda_memory_elem_ptr* q = leda_std_memory_mgr.free_list+bytes;\
      leda_memory_elem_ptr(p)->next = *q;\
      *q = leda_memory_elem_ptr(p);\
      /* leda_std_memory_mgr.unlock(bytes); */}\
    else leda_std_memory_mgr.deallocate_bytes(p,bytes);\
  }\
 }\
\
MATCHING_DELETE


/*
#define LEDA_MEMORY_WITH_CHECK(type)\
void* operator new(size_t bytes)\
{ return leda_std_memory_mgr.allocate_bytes_with_check(bytes); }\
\
void* operator new(size_t,void* p) { return p; }\
\
void  operator delete(void* p,size_t bytes)\
{ if (p != 0) leda_std_memory_mgr.deallocate_bytes_with_check(p,bytes); }
*/


/*
#if defined(LEDA_INSTALL)
inline void* operator new[](size_t s) { return std_memory.allocate_vector(s); }
inline void  operator delete[](void* p){ if (p != 0) std_memory.deallocate_vector(p); }
#endif
*/


#endif
