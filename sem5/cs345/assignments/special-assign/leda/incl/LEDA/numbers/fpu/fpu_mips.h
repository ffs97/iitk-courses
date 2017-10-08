/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fpu_mips.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




/* ---------------------------------------------------
 * MIPS architecture layout (SGI/irix)
 ---------------------------------------------------*/


/* 
 *  Source: The MIPS RISC Architecture by Gerry Kane, Joe Heinrich
 *  MIPS R4000 Microprocessor User's Manual (second edition)
 *
 *  The MIPS architecture is almost IEEE conform, without particular 
 *  extensions. There are 'cause bits' for the last exception, but
 *  the trap handler only sees the accrued exception bits.
 */

/* The layout for the combined control/status word: 
 * 
 *   31-25   24   23  22-18  17-12    11-7      6-2    1-0
 *     0   | FS | C  |  0  | Cause | Enables | Flags | RM 
 *
 *   FS      :  Non-IEEE flag, used only for R4000 (flush to zero 
 *              for denormalised results)
 *   C       :  Condition bit; set when the preceding fp-comparison is true
 *   Cause   :  Current exception bits set by the preceding fp-operation;
 *              not visible for trap handlers.
 *              bit 17 unimplemented, rest like flags.
 *   Enables :  Enable exception traps, like flags
 *                                     4   3   2   1   0
 *   Flags   :  Sticky exception bits  V | Z | O | U | I 
 *               V = invalid operation
 *               Z = division by zero
 *               O = overflow exception
 *               U = underflow exception
 *               I = inexact operation
 */

#ifndef MPIS_FPU_H
#define MPIS_FPU_H

#ifndef mips
#error this file should only be included on MIPS machines
#endif

LEDA_BEGIN_NAMESPACE

typedef unsigned int fcr_type;
/* 32 bit */

#ifdef __GNUC__

#define IEEE_INLINE_ASSEMBLER
#define IEEE_GET_FCR(CW) \
asm volatile ("cfc1 $8 $31; sw $8,%0": "=m"(CW) :)
#define IEEE_SET_FCR(CW) \
asm volatile ("lw $8,%0; ctc1 $8 $31": :"m"(CW))
#define IEEE_GET_FSR(CW) \
asm volatile ("cfc1 $8 $31; sw $8,%0": "=m"(CW) :)
#define IEEE_SET_FSR(CW) \
asm volatile ("cfc1 $8 $31; lw $8,%0; ctc1 $8 $31": :"m"(CW))
/*
 * note that (following Kane/Heinrich) the pipeline must first 
 * be emptied from fp-operations before the status register 
 * can be set; this is done by first reading the status register
 */

#endif /* __GNUC__ */

struct ieee_exception_set {
    fcr_type invalid;
    fcr_type overflow;
    fcr_type underflow;
    fcr_type divbyzero;
    fcr_type inexact;
 };

static const fcr_type ieee_round_mask = (3UL<<0);

static const fcr_type ieee_nearest  = (0UL);
static const fcr_type ieee_tozero   = (1UL);
static const fcr_type ieee_positive = (2UL);
static const fcr_type ieee_negative = (3UL);

static const fcr_type ieee_all_exceptions = (31UL<<2);
static const fcr_type ieee_exception_mask = (31UL<<2);
static const fcr_type ieee_nontrivial_exceptions = (15UL<<3);

static const struct ieee_exception_set ieee_exception_bit =
{
  (1UL<<6), /* invalid operation*/
  (1UL<<4), /* overflow */
  (1UL<<3), /* underflow */
  (1UL<<5), /* divbyzero */
  (1UL<<2)  /* inexact */
};

static const fcr_type ieee_all_traps = (31UL<<7);
static const fcr_type ieee_trap_mask = (31UL<<7);

static const struct ieee_exception_set ieee_trap_bit =
{
  (1UL<<11), /* invalid operation*/
  (1UL<< 9), /* overflow */
  (1UL<< 8), /* underflow */
  (1UL<<10), /* divbyzero */
  (1UL<< 7)  /* inexact */
};

static const fcr_type ieee_default_mask = 0UL; 

#define HIGH_WORD_FIRST

LEDA_END_NAMESPACE

#endif 
