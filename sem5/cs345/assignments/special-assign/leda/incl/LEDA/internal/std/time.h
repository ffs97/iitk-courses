/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  time.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define LEDA_STD_INCLUDE

#if defined(LEDA_STD_HEADERS)

#include <ctime>

#if (__BORLANDC__ > 0x520)
using std::time;
using std::time_t;
using std::clock;
using std::clock_t;
using std::ctime;
using std::tm;
using std::localtime;
#endif

#else

#include <time.h>

#endif

#undef LEDA_STD_INCLUDE
