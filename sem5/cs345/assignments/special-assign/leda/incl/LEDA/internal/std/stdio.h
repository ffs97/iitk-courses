/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  stdio.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define LEDA_STD_INCLUDE

#if defined(LEDA_STD_HEADERS)

#include <cstdio>

#if (__BORLANDC__ > 0x520)
using std::sscanf;
using std::sprintf;
using std::vsprintf;
using std::fprintf;
using std::printf;
using std::tmpnam;
using std::gets;
using std::FILE;
using std::fopen;
using std::fclose;
using std::ferror;
using std::feof;
using std::fscanf;
using std::fputs;
using std::fgets;
#endif

#else

#include <stdio.h>

#endif

#undef LEDA_STD_INCLUDE
