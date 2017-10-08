/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fpu_sparc.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




/* ---------------------------------------------------------------------
 * SPARC architecture layout (SUN OS/SOLARIS)
 * -------------------------------------------------------------------*/

/*  Sources: The SPARC Architecture manual Version 8 / Version 9
 *  Arithmetic on SPARC processors is 100% compatible to IEEE754.
 *  There are 'cause bits' that show the exception status of the
 *  last fp operation, and in contrast to all other tested platforms
 *  they are visible for the trap handler.
 *  Maybe this is why the trap handling works properly for SPARC.
 *  More hints how to write trap handlers that exploit the 
 *  saved FPU configurations can be found in <ieeefp.h> and <ucontext.h>.
 */

/* The layout for the combined control/status word 
 * (in SPARC-V9 the 
 *
 * 31-30 29-28 27-23  22 21-20 19-17 16-14  13   12   11-10  9-5    4-0
 * | RC  |res.| TEM | NS |res.| VER | FTT | QNE |res.| FCC | AEXC | CEXC
 *
 * RC   : Rounding control
 * res. : reserved, currently unused
 * TEM  : Trap enable mask; order like CEXC
 * NS   : Non-IEEE (non-standard) mode; e.g. flush denormal to zero 
 * VER  : fpu version number
 * FTT  : floating-point trap type; e.g. 
 *          *001 = IEEE754 exception
 *          *101 = hardware error
 * QNE  : queue not empty
 * FCC  : Floating-point condition codes (latest result of fp-compare)
 * AEXC : Accrued (sticky) exception flags; order like CEXC
 *                                  4   3    2    1    0
 * CEXC : current exception flags  NV | OF | UF | DZ | NX
 *        NV    = invalid operation,
 *        OF/UF = overflow, underflow,
 *        DZ    = division by zero
 *        NX     = inexact operation
 */

#ifndef SPARC_FPU_H
#define SPARC_FPU_H

#ifndef sparc
#error this file should only be included on SPARC machines
#endif

LEDA_BEGIN_NAMESPACE

typedef unsigned long fcr_type;
/* until SPARC-V8 the FSR was 32-bit;
 * since SPARC-V9 the FSR is  64-bit 
 */

#ifdef __GNUC__

#define IEEE_INLINE_ASSEMBLER
/*
#define IEEE_GENUINE_ASSEMBLER
*/
#define IEEE_GET_FCR(CW) asm volatile ("st %%fsr,%0" : "=m" (CW) :)
#define IEEE_SET_FCR(CW) asm volatile ("ld %0,%%fsr" : :"m" (CW) )
#define IEEE_GET_FSR(CW) asm volatile ("st %%fsr,%0" : "=m" (CW) :)
#define IEEE_SET_FSR(CW) asm volatile ("ld %0,%%fsr" : :"m" (CW) )

#endif /* __GNUC__ */
/* Sunpro/CC accepts assembler code but I don't know how to use it
 * inline; hence we use GNU assembler code instead
 */

#define IEEE_CAUSING_BITS

struct ieee_exception_set {
    fcr_type invalid;
    fcr_type overflow;
    fcr_type underflow;
    fcr_type divbyzero;
    fcr_type inexact;
 };

static const fcr_type ieee_round_mask = (3UL<<30);

static const fcr_type ieee_nearest  = (0UL<<30);
static const fcr_type ieee_tozero   = (1UL<<30);
static const fcr_type ieee_positive = (2UL<<30);
static const fcr_type ieee_negative = (3UL<<30);


static const fcr_type ieee_all_exceptions = (31UL<<5);
static const fcr_type ieee_exception_mask = (31UL<<5);
static const fcr_type ieee_nontrivial_exceptions = (15UL<<6);

static const struct ieee_exception_set ieee_exception_bit =
{
  (1UL<<9), /* invalid operation*/
  (1UL<<8), /* overflow */
  (1UL<<7), /* underflow */
  (1UL<<6), /* divbyzero */
  (1UL<<5)  /* inexact */
};

static const fcr_type ieee_all_current_exceptions = (31UL);

static const struct ieee_exception_set ieee_current_exception_bit =
{
  (1UL<<4), /* invalid operation*/
  (1UL<<3), /* overflow */
  (1UL<<2), /* underflow */
  (1UL<<1), /* divbyzero */
  (1UL<<0)  /* inexact */
};

static const fcr_type ieee_all_traps = (31UL<<23);
static const fcr_type ieee_trap_mask = (31UL<<23);

static const struct ieee_exception_set ieee_trap_bit =
{
  (1UL<<27), /* invalid operation*/
  (1UL<<26), /* overflow */
  (1UL<<25), /* underflow */
  (1UL<<24), /* divbyzero */
  (1UL<<23)  /* inexact */
};

static const fcr_type ieee_default_mask = 0UL; /* prev. (8UL<<16) */

#define HIGH_WORD_FIRST

LEDA_END_NAMESPACE

#endif /* SPARC_FPU_H */
