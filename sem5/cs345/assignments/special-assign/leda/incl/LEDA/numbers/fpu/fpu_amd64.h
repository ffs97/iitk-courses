/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fpu_amd64.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




/* 
 *----------------------------------------------------
 * AMD64 architecture layout 
 * ---------------------------------------------------
 *  AMD64 should be equivalent to I386 fpu 
 */

/*
 * Source: AMD Architecture Software Developer's Manual
 * Volume I: Basic Architecture.

   g++ on amd64 uses 128 media instructions to speed up 
   floating point operations. There is one control and 
   status register named MXCSR (32 bits wide)
 */

/* The layout for MXCSR: 
 *
 *     15  14 -13    12  11    10   9    8    7    6     5    4    3    2    1    0
 *   | FZ |  RC    | PM | UM | OM | ZM | DM | IM | DAZ | PE| UE | OE | ZE | DE | IE
 * 
 * Caution: Interrupts are _disabled_ when mask bit is 1!
 *
 * FZ: Flush to zero for masked underflow
 *
 * RC: Rounding control
 * 00 - rounding to nearest
 * 01 - rounding down (toward - infinity)
 * 10 - rounding up   (toward + infinity)
 * 11 - rounding toward zero
 *
 * IM: Invalid operation mask
 * DM: Denormalized operand mask
 * ZM: Zero-divide mask
 * OM: Overflow mask
 * UM: Underflow mask
 * PM: Precision (inexact result) mask
 * DAZ: denormals are zeros
 * IE: Invalid operation exception
 * DE: Denormalized operand exception
 * ZE: Zero-divide exception
 * OE: Overflow exception
 * UE: Underflow exception
 * PE: Precision (inexact result) exception

 *
  *
 */


#ifndef AMD64_FPU_H
#define AMD64_FPU_H


LEDA_BEGIN_NAMESPACE

typedef unsigned short fcr_type;
/* 16 bit */



#if !defined(__amd64) && !defined(__win64__)
#error only for amd64 cpus
#endif


#define IEEE_INVERSE_TRAPPING

struct ieee_exception_set {
    fcr_type invalid;
    fcr_type overflow;
    fcr_type underflow;
    fcr_type divbyzero;
    fcr_type inexact;
 };

static const fcr_type ieee_round_mask = (3UL <<13);

static const fcr_type ieee_nearest  = (0UL<<13);
static const fcr_type ieee_tozero   = (3UL<<13);
static const fcr_type ieee_positive = (2UL<<13);
static const fcr_type ieee_negative = (1UL<<13);

static const fcr_type ieee_all_exceptions        = (63UL-2UL );
static const fcr_type ieee_exception_mask        = (63UL     );
static const fcr_type ieee_nontrivial_exceptions = 1UL+(7UL<<2);

static const struct ieee_exception_set ieee_exception_bit =
{
  (1UL<<0), /* invalid operation*/
  (1UL<<3), /* overflow */
  (1UL<<4), /* underflow */
  (1UL<<2), /* divbyzero */
  (1UL<<5)  /* inexact */
};

#define IEEE_TRAP_MASK (63UL<<7)
static const fcr_type ieee_all_traps = (61UL<<7);
static const fcr_type ieee_trap_mask = IEEE_TRAP_MASK;

static const struct ieee_exception_set ieee_trap_bit =
{
  (1UL<<7), /* invalid operation*/
  (1UL<<10), /* overflow */
  (1UL<<11), /* underflow */
  (1UL<<9), /* divbyzero */
  (1UL<<12)  /* inexact */
};


static const fcr_type ieee_default_mask 
  =  IEEE_TRAP_MASK;

/* trap bits are set to avoid trapping */


LEDA_END_NAMESPACE

#endif
