/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fpu_power.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



/* ---------------------------------------------------------------------
 * POWER/POWER-PC architecture layout 
 * -------------------------------------------------------------------*/

/* Sources: POWER/POWER PC by Weiss and Smith
 * The power architecture is IEEE754 compatible.
 * As a modern processor, power (pc) is pipelined and so there is an
 * advantage of having a fast execution mode where fp interrupts
 * are disabled. (There can be chosen among 4 modes.)
 * Implementations might decide to use one of these faster modes,
 * so take care that the respective NON_IEEE flag is not set.
 * The code in this header is completely untested, so you might
 * have to work on it a little bit.
 */

/* The layout for the combined control/status word: 
 *
 *  31-30  29  28-24  23-21    20   19-15   14-7    6-2     1      0
 *   RN  | NI | TEM | E10-E8 |res.| FPRF | E7-E0 | flags | ESum | TSum
 *
 *  RN     :  rounding control
 *              00 = to nearest
 *              01 = to zero
 *              10 = to +infinity
 *              11 = to -infinity
 *  NI     :  Non-IEEE flag
 *  TEM    :  trap enable mask, order like flags
 *  E10-E0 :  non-IEEE sticky exception bits
 *  FPRF   :  floating-point result flags
 *  flags  :  sticky IEEE exception bits
 *            6    5    4    3    2
 *            XX | ZX | UX | OX | VX
 *              XX = inexact operation
 *              ZX = division by zero
 *              UX = underflow exception
 *              OX = overflow exception
 *              VX = invalid operation
 *  ESum   :  Exception Summary (summarize all exceptions)
 *  TSum   :  Trapped Eception Summary (summarize all trapped exceptions)
 *
 */


/*
#ifndef _POWER
#error this file should only be included on POWER/POWER-PC machines
#endif
*/

#ifndef POWER_FPU_H
#define POWER_FPU_H

LEDA_BEGIN_NAMESPACE

typedef unsigned long fcr_type;
/* 32-bit */

#ifdef __GNUC__

#define IEEE_INLINE_ASSEMBLER

#define IEEE_GET_FCR(CW) asm volatile ("mffs %0" : "=m" (CW) :)
#define IEEE_SET_FCR(CW) asm volatile ("mtfsfi %0" : :"m" (CW) )
#define IEEE_GET_FSR(CW) asm volatile ("mffs %0" : "=m" (CW) :)
#define IEEE_SET_FSR(CW) asm volatile ("mtfsfi %0" : :"m" (CW) )
/* I hope that the syntax is correct, but I'm not sure... */

#endif

struct ieee_exception_set {
    fcr_type invalid;
    fcr_type overflow;
    fcr_type underflow;
    fcr_type divbyzero;
    fcr_type inexact;
 };

static const fcr_type ieee_round_mask = (3UL<<30);

static const fcr_type ieee_nearest   = (0UL<<30);
static const fcr_type ieee_tozero     = (1UL<<30);
static const fcr_type ieee_positive   = (2UL<<30);
static const fcr_type ieee_negative  = (3UL<<30);

static const fcr_type ieee_all_exceptions    = (31UL<<2);
static const fcr_type ieee_exception_mask = (31UL<<2);
static const fcr_type ieee_nontrivial_exceptions = (30UL<<2);

static const struct ieee_exception_set ieee_exception_bit =
{
  (1UL<<2), /* invalid operation*/
  (1UL<<3), /* overflow */
  (1UL<<4), /* underflow */
  (1UL<<5), /* divbyzero */
  (1UL<<6)  /* inexact */
};

static const fcr_type ieee_all_current_exceptions = (31UL<<2);

static const struct ieee_exception_set ieee_current_exception_bit =
{
  (1UL<<2), /* invalid operation*/
  (1UL<<3), /* overflow */
  (1UL<<4), /* underflow */
  (1UL<<5), /* divbyzero */
  (1UL<<6)  /* inexact */
};

static const fcr_type ieee_all_traps    = (31UL<<24);
static const fcr_type ieee_trap_mask = (31UL<<24);

static const struct ieee_exception_set ieee_trap_bit =
{
  (1UL<<24), /* invalid operation*/
  (1UL<<25), /* overflow */
  (1UL<<26), /* underflow */
  (1UL<<27), /* divbyzero */
  (1UL<<28)  /* inexact */
};

static const fcr_type ieee_default_mask = 0UL; 

#define HIGH_WORD_FIRST
/* is this true? */

LEDA_END_NAMESPACE

#endif /* POWER_FPU_H */
