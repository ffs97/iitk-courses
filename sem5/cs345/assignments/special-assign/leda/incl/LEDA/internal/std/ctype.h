/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  ctype.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define LEDA_STD_INCLUDE

#if defined(LEDA_STD_HEADERS)

#include <cctype>

#if __BORLANDC__ >= 0x540
using std::isalpha;
using std::isspace;
using std::isdigit;
using std::isprint;
using std::tolower;
#endif

#else

#include <ctype.h>

#endif

#undef LEDA_STD_INCLUDE
