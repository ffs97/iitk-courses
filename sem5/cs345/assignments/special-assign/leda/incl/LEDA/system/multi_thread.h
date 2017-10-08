/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  multi-thread.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_MULTI_THREAD_H
#define LEDA_MULTI_THREAD_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600228
#include <LEDA/internal/PREAMBLE.h>
#endif


/* thread_t
   thread_t thread_create(void* (*func)(void*), void* arg);
   void     thread_join(const thread_t& thr); 
   unsigned thread_self();
*/
   

#if defined(__win32__) || defined(__win64__)

#if defined(MAXINT)
#undef MAXINT
#endif

#include <windows.h>

STDAPI VarCmp(tagVARIANT*, tagVARIANT*, unsigned long, unsigned long) {
    return 0;
}

LEDA_BEGIN_NAMESPACE

typedef void* thread_t;

inline thread_t thread_create(void* (*func)(void*), void* arg)
{ DWORD thread_id;
  return CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)func,arg,0,&thread_id);
}

inline void thread_join(const thread_t& thr) 
{ WaitForSingleObject(thr,INFINITE); }

inline void thread_detach(const thread_t& thr) { CloseHandle(thr); }

inline unsigned thread_self() { return GetCurrentThreadId(); }


LEDA_END_NAMESPACE

#else

#include <pthread.h>

LEDA_BEGIN_NAMESPACE

typedef pthread_t thread_t;

#if defined(__SUNPRO_CC)
extern "C" 
#endif
typedef void* (*tfunc)(void*);

inline thread_t thread_create(void* (*func)(void*), void* arg)
{ thread_t thr;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&(thr),&attr,(tfunc)func,arg);
  pthread_attr_destroy(&attr);
  return thr;
}

inline void thread_join(const thread_t& thr) { pthread_join(thr,NULL); }

inline void thread_detach(const thread_t& thr) { pthread_detach(thr); }

inline unsigned thread_self() { return pthread_self(); }

LEDA_END_NAMESPACE

#endif


#if LEDA_ROOT_INCL_ID == 600228
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif

