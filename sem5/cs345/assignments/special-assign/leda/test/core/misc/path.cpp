/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  path.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>
#include <LEDA/system/file.h>
#include <LEDA/system/stream.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


#if defined(__unix__)
#define sep ':'
#else
#define sep ';'
#endif

int main(int argc, char** argv)
{
   if (argc > 1)
      cout << first_file_in_path(argv[1],getenv("PATH"),sep);

   return 0;

}

