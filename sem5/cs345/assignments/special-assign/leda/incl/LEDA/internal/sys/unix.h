/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  unix.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SYS_UNIX_H
#define LEDA_SYS_UNIX_H


#if (__GNUC__ == 2)  && (__GNUC_MINOR__ < 95)
#error Error: This version of LEDA cannot be used with g++ < 2.95 !
#endif

#if defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x520)
#error Error: This version of LEDA cannot be used with Sunpro C++ < 5.2 !
#endif

#if defined(mips) && !defined(__GNUC__) && (_COMPILER_VERSION < 730)
#error Error: This version of LEDA cannot be used with MIPS C++ < 7.3 !
#endif

#if defined(__DECCXX) && (__DECCXX_VER <= 60000000)
#error Error: This version of LEDA cannot be used with DEC C++ <= 6.0 !
#endif

#if defined(__hpux) && defined(hpux) && !defined(__GNUC__)
#error Error: This version of LEDA cannot be used with HPUXC++, please use aCC !
#endif


#if defined(sparc) && !defined(__sparcv9)
#include<ieeefp.h>
#endif


//----------------------------------------------------------------------------
// system/compiler dependent definitions 
//----------------------------------------------------------------------------


#if !defined(__unix__) 
#define __unix__
#endif


#if defined(__SVR4) || defined(_SYSTYPE_SVR4) || defined(__SYSTYPE_SVR4)
#if !defined(__svr4__)
#define __svr4__
#endif
#endif


#if defined(__hpux) && !defined(__GNUC__)
#if defined(hpux)
#define __hpuxcc__
#else
#define __aCC__
#endif
#endif

#if defined(mips) && !defined(__GNUC__) && defined(_COMPILER_VERSION)
#define __mipspro__
#endif



#if defined(__DECCXX) || defined(__INTEL_COMPILER)
#define __temp_friend_decl
#else
#define __temp_friend_decl <>
#endif


#if defined(__DECCXX) || defined(__KCC) || defined(__SUNPRO_CC) 
#define __temp_func_inline inline
#else
#define __temp_func_inline
#endif



#if defined(i386) || defined(__i386) || defined(__DECCXX) || defined(__ia64__)
#define LITTLE_ENDIAN_MACHINE
#endif



#if defined(linux) && (defined(i386) || defined(__i386))

inline int leda_init_fpu()
{ // set internal rounding of FPU to double (not extended) format
  // and leave rounding to nearest as well as exceptions unchanged
  int cw = 4735;
  __asm__("fldcw %0" : : "m" (cw));
  return 0; 
}

static int set_double_ieee_for_linux = leda_init_fpu();

#endif

/*
#if defined(__amd64)
int leda_set_fpu_defaults();
static int init_amd64_fpu = leda_set_fpu_defaults();
#endif

#if defined(mips)
int leda_set_fpu_defaults();
static int set_double_ieee_for_mips = leda_set_fpu_defaults();
#endif


// assembler flags 

#if defined(sparc) && !defined(__sparcv9)
#define ASM_SPARC
static int set_default_rounding_mode_for_sparc = fpsetround(FP_RN);
#endif

#if (defined(i386) || defined(__i386)) && defined(linux)
#define ASM_INTEL_386
#endif

#if (defined(__amd64) && defined(linux))
#define ASM_AMD64
#define LITTLE_ENDIAN_MACHINE
#define SQRT_ROUNDING_OKAY
#endif


#if defined(mips) && _MIPS_SZLONG == 64
#define ASM_MIPS_64
#define SQRT_ROUNDING_OKAY
#endif


#if defined(__alpha)
#define ASM_ALPHA
#endif
*/
#define LEDA_NO_FPU
#define LITTLE_ENDIAN_MACHINE

#if (!defined(i386) || defined(ia64)) && !defined(mips) && !defined(__alpha) && !defined(sparc) && !defined(__amd64)
#define LEDA_NO_FPU
#endif

#define __exportC
#define __exportF
#define __exportD

#define __exportGeoW
#endif
