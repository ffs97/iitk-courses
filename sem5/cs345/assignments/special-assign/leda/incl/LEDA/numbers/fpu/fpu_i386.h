/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fpu_i386.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




/* 
 *----------------------------------------------------
 * I386 architecture layout 
 * ---------------------------------------------------
 */

/*
 * Source: Intel Architecture Software Developer's Manual
 * Volume I: Basic Architecture.
 * I don't know other books on I386 that are of any value for our 
 * purposes.
 * The i386 platforms are mostly IEEE conform, with the extension
 * that rounding can be set to extended precision (i.e., 64 bit
 * mantissa). Trap handlers cannot see the sticky exception bits
 * and cannot return. Underflow cannot be trapped. 
 * There are separate registers for status and control word
 * (of 16 bit length each!) and the traps bits are disable
 * bits (traps is _enabled_ when the corresponding bit is zero!)
 * The status word can only be read by F(N)STSW but cannot be 
 * directly set; the command F(N)LDSW is not available.
 */

/* The layout for the control word: 
 *
 *     15-13    12  11-10  9-8     7-6     5    4    3    2    1    0
 * | reserved | IC | RC  | PC | reserved | PM | UM | OM | ZM | DM | IM
 * 
 * Caution: Interrupts are _disabled_ when mask bit is 1!
 *
 * IC: Infinity control
 * That is for compatibility with 8087 and 80287. 
 * Ignore for 80386 and higher.
 *
 * PC: Precision control
 * 11 - round to extended precision
 * 10 - round to double precision
 * 00 - round to single precision
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
 *
 * The hardware default is 0x037f. 
 */

/* The layout for the status word: 
 *
 *    15    14   13-11   10-8     7      6    5    4    3    2    1    0
 * | Busy | C3 |  TOP  | C2-C0 |Summary| SF | PF | UF | OF | ZF | DF | IF
 *
 * Busy   : FPU busy flag
 * C3-C0  : Condition codes;
            see Intel Architecture Software Developer's Manual
 * TOP    : pointer to top of FPU register stack 
 * Summary: Exception summary, set if on of the bits 5-0 is set
 * SF     : Stack fault (overflow or underflow)
 * PF-IF  : Sticky exception flags for the exception masks EM-IM
 *
 * Note that the status word can only be directly stored into memory,
 * not loaded.
 *
 */


#ifndef I386_FPU_H
#define I386_FPU_H

#ifndef i386
#error this file should only be included on machines of the i386 family
#endif

LEDA_BEGIN_NAMESPACE

typedef unsigned short fcr_type;
/* 16 bit */



#if defined(__GNUC__) || defined(_MSC_VER) || defined(__BORLANDC__)
#define IEEE_INLINE_ASSEMBLER
#endif

#ifdef __GNUC__

#define IEEE_GET_FCR(CW) asm volatile ("fnstcw %0" : "=m" (CW) :)
/* fnstcw seems to be the better choice than fstcw */
#define IEEE_SET_FCR(CW) asm volatile ("fldcw %0" : : "m" (CW))
#define IEEE_GET_FSR(CW) asm volatile ("fnstsw %0" : "=m" (CW) :)
/* fnstsw seems to be the better choice than fstsw */
#define I386_CLEAR_EXCEPTIONS() asm volatile ("fclex")

#elif defined(_MSC_VER) || defined(__BORLANDC__)

#define IEEE_GET_FCR(CW) _asm { FSTCW CW }
#define IEEE_SET_FCR(CW) _asm { FLDCW CW }
#define IEEE_GET_FSR(CW) _asm { FSTSW CW }
#define I386_CLEAR_EXCEPTIONS() _asm { FCLEX }

#endif /* __GNUC__ */

#define IEEE_SEPARATE_SW
#define IEEE_INVERSE_TRAPPING

struct ieee_exception_set {
    fcr_type invalid;
    fcr_type overflow;
    fcr_type underflow;
    fcr_type divbyzero;
    fcr_type inexact;
 };

static const fcr_type ieee_round_mask = (3UL <<10);

static const fcr_type ieee_nearest  = (0UL<<10);
static const fcr_type ieee_tozero   = (3UL<<10);
static const fcr_type ieee_positive = (2UL<<10);
static const fcr_type ieee_negative = (1UL<<10);

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

#define IEEE_TRAP_MASK (63UL<<0)
static const fcr_type ieee_all_traps = IEEE_TRAP_MASK - 2UL;
static const fcr_type ieee_trap_mask = IEEE_TRAP_MASK;

static const struct ieee_exception_set ieee_trap_bit =
{
  (1UL<<0), /* invalid operation*/
  (1UL<<3), /* overflow */
  (1UL<<4), /* underflow */
  (1UL<<2), /* divbyzero */
  (1UL<<5)  /* inexact */
};

#define IEEE_PRECISION_MASK     (3U<<8)
#define IEEE_DOUBLE_PRECISION   (2U<<8)

static const fcr_type ieee_default_mask 
  = IEEE_DOUBLE_PRECISION + IEEE_TRAP_MASK;

/* trap bits are set to avoid trapping */

#define LOW_WORD_FIRST

LEDA_END_NAMESPACE

#endif
