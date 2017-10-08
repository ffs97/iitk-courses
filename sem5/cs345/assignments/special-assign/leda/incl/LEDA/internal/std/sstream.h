/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sstream.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define LEDA_STD_INCLUDE

#if defined(LEDA_STD_IO_HEADERS)

#include <sstream>

#else
#if defined(_MSC_VER) || defined(__BORLANDC__)
#include <strstrea.h>
#else
#include <strstream.h>
#endif
typedef ostrstream ostringstream;
typedef istrstream istringstream;
typedef strstream  stringstream;
#endif

#undef LEDA_STD_INCLUDE
