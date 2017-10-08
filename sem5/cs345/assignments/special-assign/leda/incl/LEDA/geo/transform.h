/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  transform.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_TRANSFORM_H
#define LEDA_TRANSFORM_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600091
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/basic.h>
#include <LEDA/geo/geo_global_enums.h>
#include <LEDA/numbers/matrix.h>
#include <LEDA/geo/float_kernel.h>
#include <LEDA/geo/polygon.h>
#include <LEDA/geo/polygon.h>

/* AND NO COMES THE PART THAT ADOPTS THE KERNEL */

#include <LEDA/geo/float_kernel_names.h>

#include <LEDA/geo/generic/TRANSFORM.h>

LEDA_BEGIN_NAMESPACE

inline const char* leda_tname(const transform*) 
{ return "transform"; }

LEDA_END_NAMESPACE

#include <LEDA/geo/kernel_names_undef.h>

#if LEDA_ROOT_INCL_ID == 600091
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif 


