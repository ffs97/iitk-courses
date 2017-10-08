/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  win32.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SYS_WIN32_H
#define LEDA_SYS_WIN32_H

#if !defined(__win32__)
#define __win32__
#endif

#if !defined(i386)
#define i386
#endif

#define LITTLE_ENDIAN_MACHINE
//#define ASM_INTEL_386

#ifndef NOMINMAX
#define NOMINMAX
#endif




/*
#define __temp_friend_decl <>
*/
#define __temp_friend_decl
#define __temp_func_inline



#if defined(__BORLANDC__)

#if (__BORLANDC__ < 0x550)
#error Error: This version of LEDA cannot be compiled with Borland C++ < 5.5 !
#endif

#if (__BORLANDC__ >= 0x560)
#define _USE_OLD_RW_STL
#endif

// turn off some warnings
#pragma option -w-inl -w-ccc -w-aus -w-eff -w-par -w-rch -w-pia

#include <float.h>

static int leda_init_fpu()
{ _control87(PC_53,MCW_PC);
  _control87(63U,MCW_EM);
  return 0;
}

static int setdouble_ieee_for_bcc = leda_init_fpu();

#endif



#if defined(_MSC_VER)

#if (_MSC_VER < 1200)
#error Error: This version of LEDA cannot be compiled with Microsoft C++ < 6.0 !
#endif

// turn off some warnings

// exception handling turned off when using std headers
#pragma warning(disable:4530)

// no matching delete operator (in presence of -GX)
#pragma warning(disable:4291)

// this used in base class initialization
#pragma warning(disable:4355)

// missing dll-interface
#pragma warning(disable:4251)
#pragma warning(disable:4275)


// resolved overload found by argument dependent lookup
#pragma warning(disable:4675)


// turn of deprecated warning for insecure CRT runtime
// functions (Visual C++ 2005 and above)
#if ( _MSC_VER >= 1400)
#pragma warning(disable:4996)
#endif


// turn of reference exceeds storage space warning
#if (_MSC_VER >= 1400)
#pragma warning(disable:4739)
#endif



#include <float.h>

inline int leda_init_fpu() 
{ _control87(_PC_53,_MCW_PC);
  _control87(_RC_NEAR,_MCW_RC);
  return 0; 
}

static int setdouble_ieee_for_msc = leda_init_fpu();

#endif



#if defined(__MWERKS__)

#if (__MWERKS__ < 0x2400)
#error Error: This version of LEDA cannot be compiled with Metrowerks C++ < 2.4 !
#endif

#endif


#if (_MSC_VER < 1400)
#undef LEDA_NO_EXPORTS
#endif

//------------------------------------------------------------------------------
//  DLL definitions
//------------------------------------------------------------------------------


#if defined(LEDA_DLL)

#if defined(LEDA_INSTALL) && !defined(LEDA_IMPORT) 
#define __exportC __declspec(dllexport)
#define __exportF __declspec(dllexport)
#define __exportD __declspec(dllexport)
#else
#define __exportC __declspec(dllimport)
#define __exportF __declspec(dllimport)
#define __exportD __declspec(dllimport)
#endif

#else
#define __exportC
#define __exportF
#define __exportD

#endif


#define __exportGeoW


#endif
