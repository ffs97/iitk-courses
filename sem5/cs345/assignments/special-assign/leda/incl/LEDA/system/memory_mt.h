/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  memory_mt.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_MEMORY_MT_H
#define LEDA_MEMORY_MT_H

LEDA_BEGIN_NAMESPACE


class __exportC memory_management {

  friend class __exportC memory_management_init;

  int num;     // maximal number of managers   (default: see init)
  int tbl_sz;  // table size of managers  (default: see init) 

  memory_manager** mgrs;
  memory_manager** last;

  void init(int n=256, int sz=256);
  void destroy();

public:

  memory_management();
  memory_management(int n, int sz);
 ~memory_management();

  int max_size()    { return tbl_sz; }
  int num_of_mgrs() { return num;    }
  
  void kill();
  void clear();
  void print_statistics();
  int  used_memory();

  memory_manager** add_manager(unsigned id);
  memory_manager& acquire0();

  memory_manager& acquire()
  { unsigned id = get_thread_id();
    memory_manager** p = last;
    while (p >= mgrs && (*p)->id != id) p--;
    if (p < mgrs) p = add_manager(id);
    return **p;
  }


  void release_mgr(memory_manager& mgr) { mgr.id = 0; }

  
  void allocate_block(size_t,int);
  
  void* allocate_words(size_t);
  void* allocate_bytes(size_t);
  void* allocate_bytes_with_check(size_t);
  void* allocate_vector(size_t);
  
  void deallocate_words(void*,size_t);
  void deallocate_bytes(void*, size_t);
  void deallocate_bytes_with_check(void*,size_t);
  void deallocate_vector(void*);
  void deallocate_list(void*, void*, size_t);
};


extern memory_management __exportD mt_memory_mgr;

#ifdef std_memory
#undef std_memory
#endif

#define std_memory  mt_memory_mgr


class __exportC memory_management_init
{
  static unsigned count;

public:
  memory_management_init();
 ~memory_management_init();

};


static memory_management_init memory_initialize;


#if defined(__BORLANDC__) || defined(__DMC__) || defined(__mipspro__)
#define MATCHING_DELETE_MT
#else
#define MATCHING_DELETE_MT \
void  operator delete(void*,void*)\
{ LEDA_SCOPE default_error_handler(1,"delete(void*,void*) called."); }
#endif

LEDA_END_NAMESPACE


#define LEDA_MEMORY_MT(type)\
void* operator new(size_t bytes)\
{ if (bytes < STDMEM_TABLESIZE)\
  { leda_memory_manager& manager = leda_mt_memory_mgr.acquire();\
    leda_memory_elem_ptr* q = manager.free_list+bytes;\
    if (*q==0) manager.allocate_block(bytes,-1);\
    leda_memory_elem_ptr p = *q;\
    *q = p->next;\
    return p; }\
  else return malloc(bytes);\
}\
\
void* operator new(size_t,void* p) { return p; }\
void* operator new(size_t,void* p,int) { return p; }\
\
void  operator delete(void* p, size_t bytes)\
{ if (p != 0)\
  { if (bytes < STDMEM_TABLESIZE)\
    { leda_memory_manager& manager = leda_mt_memory_mgr.acquire();\
      leda_memory_elem_ptr* q = manager.free_list+bytes;\
      leda_memory_elem_ptr(p)->next = *q;\
      *q = leda_memory_elem_ptr(p);\
    }\
    else free((char*)p);\
  }\
}\
\
MATCHING_DELETE_MT


class leda_thread_memory_init {
  leda_memory_manager& mgr;

  // to avoid a stupid warning of msvc 2008
  leda_thread_memory_init(const leda_thread_memory_init&);
  leda_thread_memory_init& operator=(const leda_thread_memory_init&);

public:
  leda_thread_memory_init() : mgr(leda_mt_memory_mgr.acquire0()) {}

 ~leda_thread_memory_init() { 
    leda_mt_memory_mgr.release_mgr(mgr); 
  }
};

#endif
