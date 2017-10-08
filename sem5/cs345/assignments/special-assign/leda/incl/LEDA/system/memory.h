/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  memory.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>

#ifndef LEDA_MEMORY_H
#define LEDA_MEMORY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600225
#include <LEDA/internal/PREAMBLE.h>
#endif


#if defined(LEDA_USE_MEMORY_STD)
#include <LEDA/system/memory_std.h>
#define LEDA_MEMORY(type) LEDA_MEMORY_STD(type)
#define LEDA_THREAD_BEGIN
#endif


#if defined(LEDA_USE_MEMORY_SYS)
#include <LEDA/system/memory_sys.h>
#define LEDA_MEMORY(type) LEDA_MEMORY_SYS(type)
#define LEDA_THREAD_BEGIN
#endif


#if defined(LEDA_USE_MEMORY_MT)
#include <LEDA/system/memory_std.h>
#include <LEDA/system/memory_mt.h>
#define LEDA_MEMORY(type) LEDA_MEMORY_MT(type)
#define LEDA_THREAD_BEGIN leda_thread_memory_init leda_thread_memory_init_x;
#endif


LEDA_BEGIN_NAMESPACE


/*Manpage*/

/*{\Mtext

\section{Memory Management} \label{Memory Management}

LEDA offers an efficient memory management system that is used internally 
for all node, edge and item types. This system can easily be customized for 
user defined classes by the ``LEDA\_MEMORY" macro. You simply have
to add the macro call ``LEDA\_MEMORY($T$)" to the declaration of a class
$T$. This redefines new and delete operators for type $T$, such that
they allocate and deallocate memory using LEDA's internal memory manager. 

\begin{verbatim}
struct pair {
  double x;
  double y;

  pair() { x = y = 0; }
  pair(const pair& p) { x = p.x; y = p.y; }

  friend ostream& operator<<(ostream&, const pair&) { ... }
  friend istream& operator>>(istream&, pair&)       { ... }
  friend int compare(const pair& p, const pair& q)  { ... }

  LEDA_MEMORY(pair)

};

dictionary<pair,int> D;
\end{verbatim}
\ \\
The LEDA memory manager only frees memory at its time of destruction (program end or
unload of library) as this allows for much faster memory allocation requests. 
As a result, memory that was deallocated by a call to the redefined delete operator still
resides in the LEDA memory management system and is not returned to the system memory
manager. This might lead to memory shortages. To avoid those shortages, it is possible
to return unused memory of LEDA's memory management system to the system memory manager
by calling
\begin{verbatim}
leda::std_memory_mgr.clear();
\end{verbatim}

}*/



#if defined(LEDA_NAMESPACE)
#define LEDA_NEW_VECTOR(T,n) (T*)leda::std_memory.allocate_vector((n)*sizeof(T))
#define LEDA_DEL_VECTOR(x)    leda::std_memory.deallocate_vector(x)
#else
#define LEDA_NEW_VECTOR(T,n) (T*)std_memory.allocate_vector((n)*sizeof(T))
#define LEDA_DEL_VECTOR(x)   std_memory.deallocate_vector(x)
#endif


inline int  used_memory()      { return std_memory.used_memory(); }


#if LEDA_ROOT_INCL_ID == 600225
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
