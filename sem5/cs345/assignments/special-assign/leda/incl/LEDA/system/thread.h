/******************************************************************************
+
+  LEDA 6.3  
+
+
+  thread.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_THREAD_H
#define LEDA_THREAD_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600241
#include <LEDA/internal/PREAMBLE.h>
#endif

LEDA_BEGIN_NAMESPACE

extern __exportF unsigned get_thread_id();

LEDA_END_NAMESPACE

#if !defined(LEDA_MULTI_THREAD)

LEDA_BEGIN_NAMESPACE

class leda_mutex { // dummy mutex class
public:
  void lock()     {}
  void unlock()   {}
  int  trylock()  { return 0; }
}; 

class spinlock { // dummy spinlock class
public:
  void lock()     {}
  void unlock()   {}
  bool try_lock() { return 0; }
};

typedef spinlock leda_lock;
typedef int atomic_counter;

LEDA_END_NAMESPACE

#else

// multi-thread

LEDA_BEGIN_NAMESPACE

class __exportC leda_mutex {

friend class __exportC leda_monitor;

#if defined(__win32__) || defined(__win64__)
  void* mtx;
#else
  pthread_mutex_t mtx;
#endif

  // no copies
  leda_mutex(const leda_mutex&);
  leda_mutex& operator=(const leda_mutex&);

public:
  leda_mutex();
 ~leda_mutex();

  void lock();
  void unlock();
  int  trylock();
}; 

LEDA_END_NAMESPACE



#if defined(_MSC_VER) && (defined(__win32__) || defined (__win64__))

#if _MSC_VER >= 1400

// VC8 and higher: use functions from intrin.h

#include <intrin.h>

#if defined(__win32__)

#pragma intrinsic(_ReadWriteBarrier)
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)

inline int Test_And_Set(int* addr, int val)
{ return _InterlockedExchange((volatile long*)addr,(long)val); }

inline int Fetch_And_Add(int* addr, int val)
{ return _InterlockedExchangeAdd((volatile long*)addr,val); }

inline int Add_And_Fetch(int* addr, int val)
{ return _InterlockedExchangeAdd((volatile long*)addr,val)+val; }

inline int Increment_And_Fetch(int* addr)
{ return _InterlockedIncrement((volatile long*)addr); }

inline int Fetch_And_Increment(int* addr)
{ return _InterlockedIncrement((volatile long*)addr)-1; }

inline int Decrement_And_Fetch(int* addr)
{ return _InterlockedDecrement((volatile long*)addr); }

inline int Fetch_And_Decrement(int* addr)
{ return _InterlockedDecrement((volatile long*)addr)+1; }

inline void Memory_Barrier() { _ReadWriteBarrier(); }

#else

// win64
#pragma intrinsic(_ReadWriteBarrier)
#pragma intrinsic(_InterlockedExchange64)
#pragma intrinsic(_InterlockedExchangeAdd64)
#pragma intrinsic(_InterlockedIncrement64)
#pragma intrinsic(_InterlockedDecrement64)

inline int Test_And_Set(int* addr, int val)
{ return (int)_InterlockedExchange64((volatile __int64*)addr,val); }

inline int Fetch_And_Add(int* addr, int val)
{ return (int)_InterlockedExchangeAdd64((volatile __int64*)addr,val); }

inline int Add_And_Fetch(int* addr, int val)
{ return (int)_InterlockedExchangeAdd64((volatile __int64*)addr,val)+val; }

inline int Increment_And_Fetch(int* addr)
{ return (int)_InterlockedIncrement64((volatile __int64*)addr); }

inline int Fetch_And_Increment(int* addr)
{ return (int)_InterlockedIncrement64((volatile __int64*)addr)-1; }

inline int Decrement_And_Fetch(int* addr)
{ return (int)_InterlockedDecrement64((volatile __int64*)addr); }

inline int Fetch_And_Decrement(int* addr)
{ return (int)_InterlockedDecrement64((volatile __int64*)addr)+1; }

inline void Memory_Barrier() { _ReadWriteBarrier(); }

#endif


#else

// VC < 8: use  inline assembler

inline int Test_And_Set(int* addr, int val)
{ __asm
  { mov edx, dword ptr [addr]
    mov eax, val
    lock xchg eax, dword ptr [edx]
   }
}

inline int Fetch_And_Add(int* addr, int val)
{ __asm
  { mov edx, dword ptr [addr]
    mov eax, val
    lock xadd dword ptr [edx], eax
   }
 }

inline int Add_And_Fetch(int* addr, int val)
{ __asm
  { mov edx, dword ptr [addr]
    mov eax, val
    lock xadd dword ptr [edx], eax
    add eax, val 
   }
 }

inline int Increment_And_Fetch(int* addr) { return Add_And_Fetch(addr,+1); }
inline int Fetch_And_Increment(int* addr) { return Fetch_And_Add(addr,+1); }
inline int Decrement_And_Fetch(int* addr) { return Add_And_Fetch(addr,-1); }
inline int Fetch_And_Decrement(int* addr) { return Fetch_And_Add(addr,-1); }


inline void Memory_Barrier() { /* to do */ }

#endif


#elif defined(linux) && defined(__GNUC__)

/*
extern int Test_And_Set(int* addr, int val);
extern void Memory_Barrier();
extern int Fetch_And_Add(int* addr, int val);
extern int Add_And_Fetch(int* addr, int val);
*/

inline int Test_And_Set(int* addr, int val)
{ return __sync_lock_test_and_set(addr,val); }

inline void Memory_Barrier() { __sync_synchronize(); }

inline int Fetch_And_Add(int* addr, int val)
{ return __sync_fetch_and_add(addr,val); }

inline int Add_And_Fetch(int* addr, int val)
{ return __sync_add_and_fetch(addr,val); }


inline int Increment_And_Fetch(int* addr) { return Add_And_Fetch(addr,+1); }
inline int Fetch_And_Increment(int* addr) { return Fetch_And_Add(addr,+1); }
inline int Decrement_And_Fetch(int* addr) { return Add_And_Fetch(addr,-1); }
inline int Fetch_And_Decrement(int* addr) { return Fetch_And_Add(addr,-1); }


//#elif defined(__sun) || defined(__SUNPRO_CC)
#elif defined(__sun)

#include <atomic.h>

inline int Test_And_Set(int* addr, int val)
{ return atomic_swap_32((volatile unsigned int*)addr,val); }

inline void Memory_Barrier() { membar_producer(); }


inline int Add_And_Fetch(int* addr, int val)
{ return (int)atomic_add_32_nv((volatile unsigned int*)addr,val); }

inline int Fetch_And_Add(int* addr, int val)
{ //return (int)atomic_add_32((volatile unsigned int*)addr,val); 
  return Add_And_Fetch(addr,val)-val; 
} 

inline int Increment_And_Fetch(int* addr)
{ return (int)atomic_inc_32_nv((volatile unsigned int*)addr); }

inline int Fetch_And_Increment(int* addr)
{ //return (int)atomic_inc_32((volatile unsigned int*)addr); 
  return Increment_And_Fetch(addr)-1; 
} 

inline int Decrement_And_Fetch(int* addr)
{ return (int)atomic_dec_32_nv((volatile unsigned int*)addr); }


inline int Fetch_And_Decrement(int* addr)
{ //return (int)atomic_dec_32((volatile unsigned int*)addr); 
  return Decrement_And_Fetch(addr)+1; 
} 


#else

#error "LEDA_MULTI_THREAD not supported for this compiler/platform combination."

#endif


//-------------------------------------------------------------------------

LEDA_BEGIN_NAMESPACE


class spinlock {

  int mtx;

public:

  spinlock() : mtx(0) {}

  void lock()     { while (Test_And_Set(&mtx,1)); }
  bool try_lock() { return Test_And_Set(&mtx,1) == 0; }
  void unlock()   { Memory_Barrier(); mtx = 0; }
};



class atomic_counter_mtx { //mutex
 
  int count;
  leda_mutex mtx;

  //atomic_counter_mtx(const atomic_counter_mtx&);

public:

  atomic_counter_mtx(int i=0) : count(i) {}

  operator int() const { return count; }

  int operator++(int) { int x; mtx.lock(); x=count++; mtx.unlock(); return x; }
  int operator--(int) { int x; mtx.lock(); x=count--; mtx.unlock(); return x; }
  int operator++()    { int x; mtx.lock(); x=++count; mtx.unlock(); return x; }
  int operator--()    { int x; mtx.lock(); x=--count; mtx.unlock(); return x; }
};



class atomic_counter_spl { // spin lock
 
  int count;
  spinlock mtx;

  //atomic_counter_spl(const atomic_counter_spl&);

public:

  atomic_counter_spl(int i=0) : count(i) {}

  operator int() const { return count; }

  int operator++(int) { int x; mtx.lock(); x=count++; mtx.unlock(); return x; }
  int operator--(int) { int x; mtx.lock(); x=count--; mtx.unlock(); return x; }
  int operator++()    { int x; mtx.lock(); x=++count; mtx.unlock(); return x; }
  int operator--()    { int x; mtx.lock(); x=--count; mtx.unlock(); return x; }
};



class atomic_counter_faa { // fetch and add

  int count;

  //atomic_counter_faa(const atomic_counter_faa&);

public:

  atomic_counter_faa(int i=0) : count(i) {}

  operator int() const { return count; }

  int operator++()    { return Increment_And_Fetch(&count); }
  int operator--()    { return Decrement_And_Fetch(&count); }
  int operator++(int) { return Fetch_And_Increment(&count); }
  int operator--(int) { return Fetch_And_Decrement(&count); }
};


#if defined(LEDA_ATOMIC_MTX)
typedef atomic_counter_mtx atomic_counter;
#elif defined(LEDA_ATOMIC_SPL)
typedef atomic_counter_spl atomic_counter;
#elif defined(LEDA_ATOMIC_FAA)
typedef atomic_counter_faa atomic_counter;
#else
#error choose atomic counter implementation in <LEDA/internal/config.h>
#endif


#if defined(LEDA_ATOMIC_MTX)
typedef leda_mutex leda_lock;
#else
typedef spinlock leda_lock;
#endif


LEDA_END_NAMESPACE

#endif


#if LEDA_ROOT_INCL_ID == 600241
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif

