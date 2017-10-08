/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gen_polygon.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_GEN_POLYGON_H
#define LEDA_GEN_POLYGON_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600058
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/geo_global_enums.h>
#include <LEDA/geo/polygon.h>

#include <LEDA/geo/float_kernel_names.h>

#include <LEDA/geo/generic/GEN_POLYGON.h>

LEDA_BEGIN_NAMESPACE

inline const char* leda_tname(const gen_polygon*) { return "gen_polygon"; }

#include <LEDA/geo/kernel_names_undef.h>


#if LEDA_ROOT_INCL_ID == 600058
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif 




