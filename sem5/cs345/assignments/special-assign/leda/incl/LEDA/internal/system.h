/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  system.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SYSTEM_H
#define LEDA_SYSTEM_H


#if defined(__CYGWIN32__)
#include <LEDA/internal/sys/cygwin.h>

#elif defined(unix) || defined(__unix__) || defined(__unix) || defined(_AIX) || defined(__APPLE_CC__)
#include <LEDA/internal/sys/unix.h>

#elif defined(__WIN64__) || defined(_WIN64)
#include<LEDA/internal/sys/windows64.h>

#elif defined(__WIN32__) || defined(_WIN32) || (defined(__NT__) && !defined(__WIN64__))
#include <LEDA/internal/sys/windows.h>


#elif defined(OS2) || defined(__OS2__)
#include <LEDA/internal/sys/os2.h>

#else
// unknown system
#endif


// enable stl-style iterators
#ifndef LEDA_STL_ITERATORS
#define LEDA_STL_ITERATORS
#endif

// leda namespace
#if !defined(__DMC__)
#define LEDA_NAMESPACE
#endif

#if defined(LEDA_NAMESPACE)
#define LEDA_BEGIN_NAMESPACE namespace leda {
#define LEDA_END_NAMESPACE   } 
#define LEDA_NAMESPACE_NAME  leda
#define LEDA_SCOPE           leda::
#else
#define LEDA_BEGIN_NAMESPACE
#define LEDA_END_NAMESPACE
#define LEDA_SCOPE           ::
#endif

#define LEDA                 leda





//-----------------------------------------------------------------------------
// c++ std headers
//-----------------------------------------------------------------------------

#if !defined(__mipspro__) && !defined(__DMC__) && !defined(__DECCXX)

#ifndef LEDA_STD_HEADERS
#define LEDA_STD_HEADERS
#endif

#endif


#if !defined(__DMC__) && (!defined(_MSC_VER) || _MSC_VER >= 1300)

#ifndef LEDA_STD_IO_HEADERS
#define LEDA_STD_IO_HEADERS
#endif

#endif


//-----------------------------------------------------------------------------
// LEDA Evaluation	
//-----------------------------------------------------------------------------

//#define LEDA_EVAL
//#define LEDA_CHECK_LICENSE_BY_DATE

#if defined(LEDA_EVAL)
#define LEDA_CHECK_LICENSE
#endif

#include <LEDA/internal/config.h>

#endif

