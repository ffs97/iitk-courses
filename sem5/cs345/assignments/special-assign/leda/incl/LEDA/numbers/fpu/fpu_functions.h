/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fpu_functions.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/




/* --------------------------------------------------------------
 *
 * this is the file fpu_functions.h
 *
 * purpose: declare common interface to access some platform--dependent
 *          features of IEEE754 arithmetic (C++/ANSI C):
 *          rounding modes, sticky exception bits, traps 
 * author:  Christoph Burnikel,
 *          Max Planck Institut fuer Informatik,
 *          66123 Saarbruecken, GERMANY
 * last modified: 07/99
 *
 * Please let me know about extensions, corrections, bug-fixes, 
 * any problems and comments you have.
 * Send mail to burnikel@mpi-sb.mpg.de
 *
-------------------------------------------------------------- */


#ifndef _FPU_FUNCTIONS_H
#define _FPU_FUNCTIONS_H

#include <LEDA/numbers/fpu/fpu_constants.h>

#define BOOLEAN fcr_type

LEDA_BEGIN_NAMESPACE
/*
#if defined(sparc) || (defined(i386) && !defined(sun)) || defined(__amd64) || defined(mips) || defined(alpha) || defined(hppa) || (defined(powerpc) && defined(__GNUC__)) 

extern "C"
{
*/
  /* get and set the floating point control register (fcr)
   * or the floating point state register (fsr)
   */

/*
  void     ieee_set_fcr(fcr_type);
  fcr_type ieee_get_fcr();
  void     ieee_set_fsr(fcr_type);
  fcr_type ieee_get_fsr();
} 

#else
// dummy versions
 */ 
inline  void     ieee_set_fcr(fcr_type) {}
inline  fcr_type ieee_get_fcr() { return 0; }
inline  void     ieee_set_fsr(fcr_type) {}
inline  fcr_type ieee_get_fsr() { return 0; }
 
//#endif

  void ieee_round_nearest(void);
  void ieee_round_up(void);
  void ieee_round_down(void);
  void ieee_round_tozero(void);

  /* caution: The ieee_round functions above do not preserve 
   * any bits in the fcr/fsr; the defaults are taken
   * and then the rounding mode is set appropriately. 
   */

  fcr_type set_rounding(fcr_type);
  /* this changes (only) the bits for the rounding precision 
   * and returns the previously active rounding mode
   */

  fcr_type ieee_clear_all_exceptions(void);
  /* this clears (only) the sticky exception bits 
   * and returns the old values
   */

  fcr_type ieee_clear_current_exceptions(void);
  /* this clears (only) the current exception bits 
   * and returns the old values
   */

  BOOLEAN ieee_any_exceptions_raised(void); 
  /* looks whether any of the sticky exception bits
   * is set (usually at least the inexact flag is raised)
   */

  BOOLEAN ieee_nontrivial_exceptions_raised(void);
  /* as ieee_any_exceptions_raised, but the inexact flag 
   * is ignored
   */
  
  BOOLEAN exceptions_raised(fcr_type);
  /* looks whether a specific exception is raised */

  fcr_type ieee_inactivate_all_traps(void);
  /* disables all five ieee traps and returns 
   * the previously enabled ones 
   */

  void ieee_set_defaults(void);
  /* the default setting is:
   * -- all traps disabled
   * -- all exceptions cleared
   * -- rounding to nearest
   * -- double precision (needed for i386 only)
   */

LEDA_END_NAMESPACE

#endif 
/* _FPU_FUNCTIONS_H */


