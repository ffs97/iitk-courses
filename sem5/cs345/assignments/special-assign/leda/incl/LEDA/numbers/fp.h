/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fp.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_FP_H
#define LEDA_FP_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600247
#include <LEDA/internal/PREAMBLE.h>
#endif


#ifndef _FP_BASIC_H
#define _FP_BASIC_H 

#include <LEDA/system/basic.h>
#include <LEDA/internal/std/float.h> 

#if FLT_RADIX == 2 && DBL_MANT_DIG == 53 && DBL_MAX_EXP == 1024
#define IEEE_BASIC_FORMAT
#else
#error double representation not IEEE754
#endif

#if !defined(WORD_LENGTH_64)  && !defined(WORD_LENGTH_32)
#error word length must be either 32 bits or 64 bits
#endif 


#if defined(mips) || defined(alpha) ||defined(i386)
#define VOLATILE_FP_REF volatile
#else
#define VOLATILE_FP_REF
#endif

#if defined(alpha) // caution, not for mipspro!
#define VOLATILE_FP     volatile
#else
#define VOLATILE_FP
#endif

#endif // FP_BASIC_H 


#if defined(WORD_LENGTH_64)
// we need a type that is a large as a double
// on most 64-bit machines, this is unsigned long
    #if !defined(__win64__)
    typedef unsigned long LEDA_UINT;
    #else
    typedef __int64 LEDA_UINT;
    #endif
// this will change for Windows 64!!!
#else
typedef unsigned long LEDA_UINT;
#endif


LEDA_BEGIN_NAMESPACE



class __exportC fp {

public:

  static const double min;   // the smallest normalized double 2^{-1022} 
  static const double NaN;   // IEEE quiet NaN 
  static const double pInf;  // IEEE positive infinity 
  static const double nInf;  // IEEE negative infinity 
  static const double pZero; // IEEE positive zero 
  static const double nZero; // IEEE negative zero 

  static double dbl_add(double x, double y);
  static double dbl_sub(double x, double y);
  static double dbl_mul(double x, double y);
  static double dbl_div(double x, double y);

  static inline void clear_sign_bit(VOLATILE_FP_REF double& x);
  // clears the sign bit of x to zero (making x nonnegative) 

  static inline LEDA_UINT sign_bit_set(VOLATILE_FP double x);
  // tells whether the sign bit of x is set 

  static inline LEDA_UINT is_finite(VOLATILE_FP double x);
  // tells whether the biased exponent of x is smaller than 2047 

  static inline LEDA_UINT is_infinite(VOLATILE_FP double x);
  // tells whether the biased exponent of x is equal to 2047 

  static inline LEDA_UINT is_nan(VOLATILE_FP double x);
  // tells whether the last 27 bit of x's significant are zero 

  static double power_two(long l);
  // returns 2^l as a double; precondition -1022 <= l <= 1023 

  static inline LEDA_UINT is_not_denormal(VOLATILE_FP double x);
  // returns nonzero value if and only if x is not a denormal number

  static inline LEDA_UINT is_single_precision(VOLATILE_FP double x);
  // returns nonzero value if and only if x fits into IEEE754 single-precision

  static inline double abs(VOLATILE_FP double x);
  // returns the absolute value of x 

  static int sign(VOLATILE_FP double x);
  // returns the sign of x 

  static double compose_parts
  (int sign_1,                 // 1 sign bit 
   LEDA_UINT exp_11,       // 11 bits of the exponent
   LEDA_UINT most_sig_20,  // higher 20 bit of the significant
   LEDA_UINT least_sig_32  // lower  32 bit of the significant
   );
  // returns a IEEE754 double composed of the four parts 
  // sign_1, exp_11, most_sig_20, least_sig_32

  static void read_parts
  (VOLATILE_FP double a,                    // the double whose parts we read
   int& sign_1,                 // 1 sign bit 
   LEDA_UINT& exp_11,       // 11 bits of the (biased) exponent
   LEDA_UINT& most_sig_20,  // higher 20 bit of the significant
   LEDA_UINT& least_sig_32  // lower  32 bit of the significant
   );
  // decomposes an IEEE 754 double into the four parts 
  // sign_1, exp_11, most_sig_20, least_sig_32. 
  // read_parts is the inverse of compose_parts

  static LEDA_UINT get_biased_exponent(VOLATILE_FP double x);
  // returns the exponent of x in LEDA_UINT format, biased by +1023 

  static LEDA_UINT get_unbiased_exponent(VOLATILE_FP double x);
  // read the unbiased (the true) exponent of x in LEDA_UINT format 

  static void set_unbiased_exponent(VOLATILE_FP_REF double& x, int exp);
  // read the unbiased (the true) exponent of x in LEDA_UINT format 

  static LEDA_UINT binary_equal(VOLATILE_FP double x, VOLATILE_FP double y);
  // says whether x and y are equal as binaries 

  static LEDA_UINT selftest_okay(void);
  // returns nonzero result if all functions work correctly 
  // (execpt for possible hardware errors and anomalies) 

};


static const LEDA_UINT single_precision_mask = 0x1fffffff;
#ifndef WORD_LENGTH_64
static const LEDA_UINT signbit_mask          = 0x80000000;
static const LEDA_UINT clear_signbit_mask    = 0x7fffffff;
static const LEDA_UINT exponent_mask         = 0x7ff00000;
static const LEDA_UINT high_mantissa_mask    = 0x000fffff;
#else
static const LEDA_UINT signbit_mask          = 0x8000000000000000;
static const LEDA_UINT clear_signbit_mask    = 0x7fffffffffffffff;
static const LEDA_UINT exponent_mask         = 0x7ff0000000000000;
static const LEDA_UINT mantissa_mask         = 0x000fffffffffffff;
#endif

#if defined(LITTLE_ENDIAN_MACHINE) && !defined(WORD_LENGTH_64)
union double_shape
{
  double value;
  struct
  {
    LEDA_UINT lsw;
    LEDA_UINT msw;
  } parts;
};
#endif

#if !defined(LITTLE_ENDIAN_MACHINE) && !defined(WORD_LENGTH_64)
union double_shape
{
  double value;
  struct
  {
    LEDA_UINT msw;
    LEDA_UINT lsw;
  } parts;
};
#endif

#if defined(WORD_LENGTH_64)
union double_shape
{
  double value;
  LEDA_UINT w;
};
#endif

#if defined(WORD_LENGTH_64)
inline LEDA_UINT fp::is_single_precision(VOLATILE_FP double x) {
  double_shape p;
  p.value = x; 
  return !(p.w & single_precision_mask);
}

inline LEDA_UINT fp::sign_bit_set(VOLATILE_FP double x) {
  double_shape p;
  p.value = x;
  return p.w & signbit_mask;
}

inline void fp::clear_sign_bit(VOLATILE_FP_REF double& x) {
  double_shape p;
  p.value = x;
  p.w &= clear_signbit_mask;
  x = p.value;
}

inline double fp::abs(VOLATILE_FP double y) {
  double_shape p;
  p.value = y;
  p.w &= clear_signbit_mask;
  return p.value;
}

inline LEDA_UINT fp::is_not_denormal(VOLATILE_FP double x) {
  double_shape p;
  p.value = x;
  return p.w & exponent_mask;
}

inline LEDA_UINT fp::is_finite(VOLATILE_FP double x) {
  double_shape p;
  p.value = x;
  LEDA_UINT exponent_part = p.w & exponent_mask;
  return (exponent_part < exponent_mask);
}

inline LEDA_UINT fp::is_infinite(VOLATILE_FP double x) {
  double_shape p;
  p.value = x;
  LEDA_UINT exponent_part = p.w & exponent_mask;
  return (exponent_part == exponent_mask);
}

inline LEDA_UINT fp::is_nan(VOLATILE_FP double x) {
  double_shape p;
  p.value = x;
  LEDA_UINT exponent_part = p.w & exponent_mask;
  return
   (exponent_part == exponent_mask && (p.w & mantissa_mask));
}

#else
inline LEDA_UINT fp::is_single_precision(VOLATILE_FP double x) {
  double_shape p;
  p.value = x;
  return !(p.parts.lsw & single_precision_mask);
}

inline LEDA_UINT fp::sign_bit_set(VOLATILE_FP double x) {
  double_shape p;
  p.value = x;
  return p.parts.msw & signbit_mask;
}

inline void fp::clear_sign_bit(VOLATILE_FP_REF double& x) {
  double_shape p;
  p.value = x;
  p.parts.msw &= clear_signbit_mask;
  x = p.value;
}

inline double fp::abs(VOLATILE_FP double y) {
  double_shape p;
  p.value = y;
  p.parts.msw &= clear_signbit_mask;
  return p.value;
}

inline LEDA_UINT fp::is_not_denormal(VOLATILE_FP double x) {
  double_shape p;
  p.value = x;
  return p.parts.msw & exponent_mask;
}

inline LEDA_UINT fp::is_finite(VOLATILE_FP double x) {
  double_shape p;
  p.value = x;
  LEDA_UINT exponent_part = p.parts.msw & exponent_mask;
  return (exponent_part < exponent_mask);
}

inline LEDA_UINT fp::is_infinite(VOLATILE_FP double x) {
  double_shape p;
  p.value = x;
  LEDA_UINT exponent_part = p.parts.msw & exponent_mask;
  return (exponent_part == exponent_mask);
}

inline LEDA_UINT fp::is_nan(VOLATILE_FP double x) {
  double_shape p;
  p.value = x;
  LEDA_UINT exponent_part = p.parts.msw & exponent_mask;
  return 
   (   (exponent_part == exponent_mask)
    && (p.parts.lsw || (p.parts.msw & high_mantissa_mask) ) );
}

#endif



#if LEDA_ROOT_INCL_ID == 600247
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

