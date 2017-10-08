/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  cygwin32.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SYS_CYGWIN32_H
#define LEDA_SYS_CYGWIN32_H


#if !defined(__unix__)
#define __unix__
#endif

#define __HAS_BUILTIN_BOOL__
#define __HAS_EXPLICIT_KEYWORD__
#define __HAS_TYPENAME_KEYWORD__
#define __HAS_MEMBER_TEMPLATES__


#define __explicit          explicit
#define typename          typename
#define __temp_friend_decl  <>
#define __temp_func_inline


#define LITTLE_ENDIAN_MACHINE
#define ASM_INTEL_386

#undef WINMAIN

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

/*
#define leda_IMP(name)  __imp_##name
#define leda_GLOBAL(name)       (*leda_IMP(name))

#define leda_impure_ptr   leda_GLOBAL(leda_impure_ptr)
#define std_LEDA_EXCEPTION leda_GLOBAL(std_LEDA_EXCEPTION)
#define std_memory_mgr    leda_GLOBAL(std_memory_mgr)
#define rand_int          leda_GLOBAL(rand_int)
#define gw_auto_color     leda_GLOBAL(gw_auto_color)
#define FIVE_COLOR        leda_GLOBAL(FIVE_COLOR)
#define double_min        leda_GLOBAL(double_min)
#define nInf_double       leda_GLOBAL(nInf_double)
#define pInf_double       leda_GLOBAL(pInf_double)
#define nZero_double      leda_GLOBAL(nZero_double)
#define pZero_double      leda_GLOBAL(pZero_double)
*/

#endif

#else

#define __exportC
#define __exportF
#define __exportD


#endif

#define __exportGeoW


#endif
