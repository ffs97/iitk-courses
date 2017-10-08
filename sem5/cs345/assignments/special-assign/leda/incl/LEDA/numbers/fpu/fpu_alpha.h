/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fpu_alpha.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




/* ----------------------------------------------------------------------
 * DEC alpha architecture layout 
 * -------------------------------------------------------------------*/

/* 
 *  Sources: 
 *  1) Alpha Architectire Handbook Version 4
 *  2) Alpha Implementations and architecture; 
 *     Complete Reference and Guide (author: Bhandarkar)
 *
 *  The ALPHA architecture does not provide full IEEE754 functionality
 *  _in hardware_ (See page 293, 10.6.6 above in Bhandarkar's book).
 *  This is because e.g. all overflows and invalid operations are trapped
 *  and underflow is handled by flushing to zero, by default.
 *  Proper results for these exceptions can only be obtained by
 *  kernel software (in case of underflow a certain qualifier has to 
 *  be set). In particular, this means that fp operations can take
 *  a few hundred times longer than normal if an exception occurs.
 *  Similarly, rounding to nearest is hard-coded into the
 *  operations by default and hence dynamic rounding according to the 
 *  rounding bits in the control word can only be achieved by
 *  setting the qulifier /D.
 *  E.g. using g++/gcc 2.8 one needs -mfp-rounding-mode=d and 
 *  for CC/cc -fprm (the latter information is from Sylvain Pion)
 *  Furthermore g++ -mieee allows to cimoute infinities, NaN's and
 *  denormalized numbers. 
 */

/* The layout for the combined control/status word: 
 * Here a part of the 64-bit FPCR (fp control register) named Fbox 
 * is relevant.
 *
 *    63       62      61     60   59-58   57    56    55    54    53    52    
 *  Summary | INED |  UNFD | UNDZ | DYN  | IOV | INE | UNF | OVF | DZE | INV |
 *
 *   51     50      49    48    47    46-0
 *  OVFD | DZED | INVD | DNZ | DNOD | res.
 *
 *  Summary: exception summary
 *
 *  DYN    : dynamic rounding mode selection
 *           Only used when machine operation has /D specifier
 *           in the instruction field (caution!)
 *
 *   00 = to zero (chop)
 *   01 = to -infinity
 *   10 = to nearest
 *   11 = to +infinity
 *
 *  The following are the sticky exception flags.
 *  (There are no trap enabling bits, but only 
 *   *optional trap disable bits*, see below )
 *
 *  IOV    : integer overflow
 *  INE    : inexact operation
 *  UNF    : underflow
 *  OVF    : overflow
 *  DZE    : division by zero
 *  INV    : invalid operation
 *
 *  There are trap disable bits whose implementation is *optional* and 
 *  which are only valid together with the /S qualifier. These are  
 *  OVFD, DZED, INVD. There are also INED and UNFD disbale bits, but 
 *  they need additional /I and /U qualifiers, respectively. 
 *
 *  To complete the mess, there are more FCR bits:
 *
 *  UNDZ Flush underflow to zero
 *  DNZ  Treat all denormal IEEE operands as zero 
 *  DNOD Has effect only if DNZ is not set. Disables the 
 *  INVD exception that would otherwise be triggered for
 *       denormal operands. 
 */

#if !defined(alpha) 
#error this file should only be included on ALPHA machines
#endif

#ifndef ALPHA_FPU_H
#define ALPHA_FPU_H

LEDA_BEGIN_NAMESPACE

typedef unsigned long fcr_type;
/* 64-bit */

#define IEEE_GNU_ASSEMBLER

#ifdef __GNUC__

#define IEEE_INLINE_ASSEMBLER

/* excb is needed to empty the fp pipeline */
#define IEEE_GET_FCR(CW) asm volatile ("excb; mf_fpcr $f0; stt $f0,%0; excb" :"=m"(CW) :)
#define IEEE_SET_FCR(CW) asm volatile ("excb; ldt $f0,%0; mt_fpcr $f0; excb" : : "m"(CW))
#define IEEE_GET_FSR(CW) asm volatile ("excb; mf_fpcr $f0; stt $f0,%0; excb" :"=m"(CW) :)
#define IEEE_SET_FSR(CW) asm volatile ("excb; ldt $f0,%0; mt_fpcr $f0; excb" : : "m"(CW))
#endif


#define IEEE_INVERSE_TRAPPING

struct ieee_exception_set {
    fcr_type invalid;
    fcr_type overflow;
    fcr_type underflow;
    fcr_type divbyzero;
    fcr_type inexact;
 };

static const fcr_type ieee_round_mask = (3UL<<58);

#define IEEE_NEAREST (2UL<<58)
static const fcr_type ieee_nearest  = (2UL<<58);
static const fcr_type ieee_tozero   = (0UL<<58);
static const fcr_type ieee_positive = (3UL<<58);
static const fcr_type ieee_negative = (1UL<<58);

/* be *very* careful because to use this, every fp operation needs
 * the -D 'operation qualifier' in order to activate what they call
 * 'dynamic rounding' (which is elsewhere the standard!)
 * Further note that the modes IEEE_POSITIVE and IEEE_NEGATIVE 
 * need not be in hardware. On an old DEC alpha I got
 * errors for these two modes, and I couldn't even identify
 * it as a hardware error or handler error. In effect, it's 
 * all the same: You cannot use safely rounding to infinity 
 * on this old machine. On a newer alpha chip roundig worked,
 * but not for the math library's sqrt function, which was not
 * not compiled using the /D qualifier.
 */

static const fcr_type ieee_all_exceptions        = (31UL<<52);
static const fcr_type ieee_exception_mask        = (63UL<<52);
static const fcr_type ieee_nontrivial_exceptions = (15UL<<52);

static const struct ieee_exception_set ieee_exception_bit =
{
  (1UL<<52), /* invalid operation*/
  (1UL<<54), /* overflow */
  (1UL<<55), /* underflow */
  (1UL<<53), /* divbyzero */
  (1UL<<56)  /* inexact */
};

#define IEEE_TRAP_MASK      ((3UL<<61) + (7UL<<49))
static const fcr_type ieee_all_traps = IEEE_TRAP_MASK;
static const fcr_type ieee_trap_mask = IEEE_TRAP_MASK;

static const struct ieee_exception_set ieee_trap_bit =
{
  (1UL<<49), /* invalid operation*/
  (1UL<<51), /* overflow */
  (1UL<<61), /* underflow */
  (1UL<<50), /* divbyzero */
  (1UL<<62)  /* inexact */
};


  /* As said before, the usual trap enable bits are not provided.
   * More precisely, overflow, divbyzero, invalid exceptions 
   * are always trapped by a PAL code, OS or user trap handler. 
   * In the default setting the computation is stopped with 
   * a floating exception by the OS handler. 
   * Using the /S qualifier the OS trap handler computes IEEE
   * conformant results (caution: slow) but then we cannot
   * activate any trap handler anymore and, worse, the sticky
   * exception bits are garbage.
   * Further note that to support inexact operation and 
   * floating point underflow, the additional qualifiers
   * /I and /U are needed. There is another one for 
   * integer overflow, /V.
   *
   * With alpha's design, providing IEEE conformant reults
   * is rather difficult and moreover depends on the 
   * correct implementation of the OS and PAL code trap
   * handlers. 
   *
   */

static const fcr_type ieee_default_mask (IEEE_TRAP_MASK + IEEE_NEAREST);

LEDA_END_NAMESPACE

#endif 
