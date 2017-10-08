/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fpu_constants.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




/* --------------------------------------------------------------
 *
 * this is the file fpu_constants.h
 *
 * purpose: constants for cpu-specific IEEE fpu control. 
 *         
 * author:  Ch. Burnikel,
 *          Max Planck Institut fuer Informatik,
 *          66123 Saarbruecken, GERMANY
 *
 * last modified: 07/99
 *
 * Please let me know about extensions, corrections, bug-fixes, 
 * any problems and comments you have.
 * Send mail to burnikel@mpi-sb.mpg.de
 *
-------------------------------------------------------------- */

#ifndef _FPU_CONSTANTS_H
#define _FPU_CONSTANTS_H


/*--------------------------------------------------------------
 *  
 * Supporting a new platform (consisting of cpu, operating system, 
 * and compiler) is done by defining correct values for all 
 * non-optional types, variables and macros below. 
 * Macro names are upper-case. Please preserve the order in
 * which the names are listed. 
 *
 * typedef unsigned xxx fcr_type;
 * ... fcr_type is an unsigned integral type for the floating-point
 *     control register ("fcr") and the floating-point 
 *     state   register ("fsr"), for example: unsigned int.
 *     Note that there are only few processor types where the fsr
 *     and the fcr are different register (e.g. the i386 family).
 *
 * IEEE_INLINE_ASSEMBLER (optional!)
 * ... set if inline assembler code to control fcr and fsr 
 *     is available for the compiler
 *
 * IEEE_GENUINE_ASSEMBLER (optional!)
 * ... set if (non-inline) GNU assembler (gas) code 
 *     is available
 *
 * IEEE_GET_FCR(CW) 
 * ... reads the current fcr value into CW.
 * IEEE_SET_FCR(CW) 
 * ... sets the current fcr value to CW. 
 * IEEE_GET_FSR(SW)
 * ... reads the current fsr value into SW.
 * IEEE_SET_FSR(SW) 
 * ... sets the current fsr value to SW.
 *
 * IEEE_SEPARATE_SW (optional!)
 * ... indicates that there is floating point status register
 *     (FSR) that is _separate_ from the control register (FCR)
 *     if not set, the status register is the same as the 
 *     control register
 *
 * IEEE_CAUSING_BITS (optional!)
 * ... indicates that there are separate bits in the fsr that
 *     notify all exceptions that occured in the _last_
 *     floating point operation. 
 *
 * IEEE_INVERSE_TRAPPING (optional!)
 * ... is a switch that, if defined, indicates that the trap bits
 *     in ieee_trap_mask are set to turn _off_ the traps 
 *     (e.g. for i386 and DEC alpha).
 *
 * struct ieee_exception_set {
 *    fcr_type invalid;
 *    fcr_type overflow;
 *    fcr_type underflow;
 *    fcr_type divbyzero;
 *    fcr_type inexact;
 * };
 * ... structure for the above five exceptions provided 
 *     by the IEEE standard.
 *
 * static const fcr_type ieee_round_mask;
 * ... mask for the bits in the fcr that control the rounding mode
 *
 * static const fcr_type ieee_nearest;
 * static const fcr_type ieee_tozero;
 * static const fcr_type ieee_positive;
 * static const fcr_type ieee_negative;
 * ... the bit combinations for each of the rounding modes:
 *     to nearest (default), to zero (=chopping),
 *     upwards ("positive"), and downwards ("negative")
 *
 * static const fcr_type ieee_all_exceptions;
 * ... the bits in the fsr that contain the accrued IEEE exceptions
 *
 * static const fcr_type ieee_exception_mask;
 * ... the mask for all (including non-IEEE) accrued exceptions 
 *
 * static const fcr_type ieee_nontrivial_exceptions;
 * ... all IEEE exceptions without 'inexact'
 *
 * static const ieee_exception_set ieee_exception_bit;
 * ... single bits in ieee_all_exceptions for each exception
 * 
 * static const fcr_type ieee_all_current_exceptions;
 * ... mask for the non-sticky exception bits in the fsr that 
 *     are raised for the last floating point operation;
 *     only valid if IEEE_CAUSING_BITS is defined, otherwise
 *     same as ieee_accrued_exception_mask
 *
 * static const ieee_exception_set ieee_current_exception_bit;
 * ... single bits in ieee_current_exception_mask for each exception
 *     (same as ieee_accrued_exception_bit if IEEE_CAUSING_BITS 
 *      is not defined)
 *
 * static const fcr_type ieee_all_traps;
 * ... the IEEE bits in the fcr that control trap enabling/disabling
 *
 * static const fcr_type ieee_all_traps;
 * ... mask for all (including non-IEEE) bits in the fcr that control 
 *     trap enabling/disabling
 * 
 * static const ieee_exception_set ieee_trap_bit;
 * ... single bits in ieee_trap_mask for each exception
 *
 * 
 * static const unsigned int ieee_exception_offset;
 * ... the location of the first sticky IEEE exception bit
 *
 * static const unsigned int ieee_current_exception_offset;
 * ... the location of the first current IEEE exception bit
 *     (has only meaning if IEEE_CAUSING_BITS is defined)
 * 
 * static const unsigned int ieee_trap_offset;
 * ... the location of the first IEEE exception trap enabling/disabling bit
 *
 * static const fcr_type ieee_default_mask 
 * ... the default mask that ensures
 *       -- rounding to nearest
 *            (as far as the fcr is concerned; there are cpus where
 *             the rounding mode can be statically set in the 
 *             instruction field of a hardware operation, independent
 *             of the fcr)
 *       -- all traps are disabled 
 *            (as far as the fcr is concerned; there are cpus where
 *             not all traps can be disabled)
 *       -- all sticky and potential non-sticky exception bits are cleared
 *       -- internal rounding is to double precision
 *
 *

---------------------------------------------------------------*/


/*--------------------------------------------------------------
 *
 * FPU control is supported on the following platforms by now:
 *
 * -- solaris/GNU(g++),
 * -- i386 linux/GNU(g++), 
 * -- MIPS MipsPro(CC),
 * -- DEC alpha (with some restrictions)
 *
 * there are more platforms where code exists but could not
 * be tested / was not tested:
 *
 * -- i386 Windows/Microsoft(MSVC++,MSVC), Borland
 * -- Power/PowerPC
 * -- Motorola 68000 (only those with a built-in FPU)
 * -- Hewlett-Packard PA-RISC
---------------------------------------------------------------*/


#if defined(__i386) || defined(__i386__) || defined(_M_IX86)
#undef i386
#define i386
#endif

#if defined(__mips) || defined(__mips__)
#undef mips
#define mips
#endif

#if defined(__sparc) || defined(__sparc__) 
#undef sparc
#define sparc
#endif

#if defined(__alpha) || defined(__alpha__)
#undef alpha
#define alpha
#endif

#if defined(__powerpc) || defined(__powerpc__) || defined(__APPLE_CC__)
#undef powerpc
#define powerpc
#endif

#if defined(__mc68000__) || defined(__mc68000)
#undef mc68000
#define mc68000
#endif

#if defined(__hppa__) || defined(__hppa)
#undef hppa
#define hppa
#endif


#if defined(__amd64) || defined(__win64__)
#undef amd64
#define amd64
#endif

#if defined(i386)
#include <LEDA/numbers/fpu/fpu_i386.h>
#define IEEE_FPU_SUPPORTED
#endif

#if defined(mips)
#include <LEDA/numbers/fpu/fpu_mips.h>
#define IEEE_FPU_SUPPORTED
#endif

#ifdef sparc
#include <LEDA/numbers/fpu/fpu_sparc.h>
#define IEEE_FPU_SUPPORTED
#endif

#ifdef alpha
#include <LEDA/numbers/fpu/fpu_alpha.h>
#define IEEE_FPU_SUPPORTED
#endif

#ifdef hppa
#include <LEDA/numbers/fpu/fpu_hppa.h>
#define IEEE_FPU_SUPPORTED
#endif

#ifdef powerpc
#include <LEDA/numbers/fpu/fpu_power.h>
#define IEEE_FPU_SUPPORTED
#endif

#ifdef amd64
#include <LEDA/numbers/fpu/fpu_amd64.h>
#define IEEE_FPU_SUPPORTED
#endif

/*
#ifdef mc68000
#include <LEDA/numbers/fpu/fpu_mc68k.h>
#define IEEE_FPU_SUPPORTED
#endif
*/


#ifdef IEEE_FPU_SUPPORTED

LEDA_BEGIN_NAMESPACE

static const fcr_type ieee_inexact=ieee_exception_bit.inexact;
static const fcr_type ieee_underflow=ieee_exception_bit.underflow;
static const fcr_type ieee_overflow=ieee_exception_bit.overflow;
static const fcr_type ieee_divbyzero=ieee_exception_bit.divbyzero;
static const fcr_type ieee_invalid=ieee_exception_bit.invalid;
static const fcr_type ieee_no_exception=0;

LEDA_END_NAMESPACE

#else 

LEDA_BEGIN_NAMESPACE


// values similar to SPARC 
typedef unsigned long fcr_type;
static const fcr_type ieee_invalid   = (1UL<<4);
static const fcr_type ieee_overflow  = (1UL<<3);
static const fcr_type ieee_underflow = (1UL<<2);
static const fcr_type ieee_divbyzero = (1UL<<1);
static const fcr_type ieee_inexact   = (1UL<<0);
static const fcr_type ieee_all_exceptions 
 = ieee_invalid+ieee_overflow+ieee_underflow+ieee_divbyzero+ieee_inexact;

LEDA_END_NAMESPACE

#endif

#endif 


