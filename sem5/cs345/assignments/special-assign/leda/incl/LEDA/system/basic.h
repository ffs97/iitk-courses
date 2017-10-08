/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  basic.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_BASIC_H
#define LEDA_BASIC_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600240
#include <LEDA/internal/PREAMBLE.h>
#endif

// define LEDA version macro

#define __LEDA__ 630

#define LEDA_CHECK_VERSION check_version_630


// turn checking off when building the libraries

#if defined(LEDA_INSTALL) && !defined(LEDA_CHECKING_OFF)
#define LEDA_CHECKING_OFF
#endif

// include system config file


#include <LEDA/internal/system.h>


/* moved to PREAMBLE.h
// include std header files
#include <LEDA/internal/std/iostream.h>
#include <LEDA/internal/std/fstream.h>
#include <LEDA/internal/std/sstream.h>
#include <LEDA/internal/std/stddef.h>
#include <LEDA/internal/std/stdlib.h>
#include <LEDA/internal/std/math.h>
*/

// define leda_... names

#include <LEDA/internal/PREFIX.h>


// include basic LEDA headers

#include <LEDA/internal/global.h>
#include <LEDA/system/error.h>
#include <LEDA/system/thread.h>
#include <LEDA/system/memory.h>
#include <LEDA/internal/param_types.h>
#include <LEDA/internal/handle_types.h>
#include <LEDA/core/random_source.h>
#include <LEDA/internal/iteration.h>
#include <LEDA/internal/lin_order.h>
#include <LEDA/core/string.h>
#include <LEDA/system/misc.h>



#if LEDA_ROOT_INCL_ID == 600240
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
