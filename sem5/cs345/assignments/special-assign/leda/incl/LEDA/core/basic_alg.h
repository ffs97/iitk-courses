/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  basic_alg.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_BASIC_ALG_H
#define LEDA_BASIC_ALG_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600023
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

/*
extern __exportF void QUICKSORT_C(int*    l, int*    r);
extern __exportF void QUICKSORT_C(float*  l, float*  r);
extern __exportF void QUICKSORT_C(double* l, double* r);
*/

extern __exportF int    SELECT(int* l,int* r,int k); 
extern __exportF double SELECT(double*,double*,int);

extern __exportF void INTSORT(int* A, int n);


#if LEDA_ROOT_INCL_ID == 600023
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

