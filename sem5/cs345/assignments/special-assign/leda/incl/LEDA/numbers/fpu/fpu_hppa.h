/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fpu_hppa.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



/* ---------------------------------------------------------------------
 * HP-PA RISC architecture layout 
 * -------------------------------------------------------------------*/

/* Sources: HP's "PA-RISC 1.1 Architecture and Implementation Set 
 * Reference Manual", 3th edition.
 * HP-PA architecture is fully IEEE754 compatible.
 * There is one bit to implement a faster arithemtic mode, where
 * underflowed numbers are flushed to zero.
 * Take care that this flag is not set.
 */

/* The layout for the combined control/status word: 
 *
 *   31-27  26   25-22   21-11   10-9  8-7   6   5    4-0
 *   Flags  | C  |   res.   | CQueue | RM | res. | T | D | Enables
 *
 *  Flags  :  Sticky exception flags
 *             31  30  29  28  27
 *           | V |  Z |  O |  U |  I |
 *         V = invalid operation
 *         Z = division by zero
 *         O = overflow exception
 *         U = underflow exception
 *         I = inexact operation
 *  C      :  Compare bit (most recent fp-comparison)
 *  CQueue :  2th-12th most recent fp-comparison
 *  RM     :  Rounding modes
 *            00 = to nearest 
 *            01 = to zero
 *            10 = to +infinity
 *            11 = to -infinity
 *  T      :  Delayed trap bit (set when FPU takes a trap)
 *  D      :  Denormalized as zero bit
 *  Enables:  Trap enable mask, in same order as Flags
 */

#ifndef hppa
#error this file should only be included on HP-PA machines
#endif

#ifndef HPPA_FPU_H
#define HPPA_FPU_H

LEDA_BEGIN_NAMESPACE

typedef unsigned long fcr_type;
/* 32-bit in single-word mode */

#ifdef __GNUC__

#define IEEE_INLINE_ASSEMBLER

#define IEEE_GET_FCR(CW) asm volatile ("fstws %%fr0L,%0" : "=m" (CW) :)
#define IEEE_SET_FCR(CW) asm volatile ("fldws %0,%%fr0L" : :"m" (CW) )
#define IEEE_GET_FSR(CW) asm volatile ("fstws %%fr0L,%0" : "=m" (CW) :)
#define IEEE_SET_FSR(CW) asm volatile ("fldws %0,%%fr0L" : :"m" (CW) )

#endif

struct ieee_exception_set {
    fcr_type invalid;
    fcr_type overflow;
    fcr_type underflow;
    fcr_type divbyzero;
    fcr_type inexact;
 };

static const fcr_type ieee_round_mask = (3UL<<9 );

static const fcr_type ieee_nearest   = (0UL<<9);
static const fcr_type ieee_tozero     = (1UL<<9);
static const fcr_type ieee_positive   = (2UL<<9);
static const fcr_type ieee_negative  = (3UL<<9);

static const fcr_type ieee_all_exceptions    = (31UL<<27);
static const fcr_type ieee_exception_mask = (31UL<<27);
static const fcr_type ieee_nontrivial_exceptions = (30UL<<27);

static const struct ieee_exception_set ieee_exception_bit =
{
  (1UL<<31), /* invalid operation*/
  (1UL<<29), /* overflow */
  (1UL<<28), /* underflow */
  (1UL<<30), /* divbyzero */
  (1UL<<27)  /* inexact */
};

static const fcr_type ieee_all_current_exceptions = (31UL<<27);

static const struct ieee_exception_set ieee_current_exception_bit =
{
  (1UL<<31), /* invalid operation*/
  (1UL<<29), /* overflow */
  (1UL<<28), /* underflow */
  (1UL<<30), /* divbyzero */
  (1UL<<27)  /* inexact */
};

static const fcr_type ieee_all_traps    = 31UL;
static const fcr_type ieee_trap_mask = 31UL;

static const struct ieee_exception_set ieee_trap_bit =
{
  (1UL<<4), /* invalid operation*/
  (1UL<<2), /* overflow */
  (1UL<<1), /* underflow */
  (1UL<<3), /* divbyzero */
  (1UL<<0)  /* inexact */
};

static const fcr_type ieee_default_mask = 0UL; 

#define HIGH_WORD_FIRST
/* is this true? */

LEDA_END_NAMESPACE

#endif /* HPPA_FPU_H */
