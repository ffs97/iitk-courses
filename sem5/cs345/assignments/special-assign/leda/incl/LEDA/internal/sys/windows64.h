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


#ifndef LEDA_SYS_WIN64_H
#define LEDA_SYS_WIN64_H

#if !defined(__win64__)
#define __win64__
#endif


#define LITTLE_ENDIAN_MACHINE

#define LEDA_NO_FPU

#ifndef NOMINMAX
#define NOMINMAX
#endif




/*
#define __temp_friend_decl <>
*/
#define __temp_friend_decl
#define __temp_func_inline







#if defined(_MSC_VER)

#if (_MSC_VER < 1400)
#error Error: This version of LEDA cannot be compiled with Microsoft C++ < 8.0 !
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


// turn of deprecated waring for insecure CRT runtime
// functions (Visual C++ 2005 and above)
#if ( _MSC_VER >= 1400)
#pragma warning(disable:4996)
#endif



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
