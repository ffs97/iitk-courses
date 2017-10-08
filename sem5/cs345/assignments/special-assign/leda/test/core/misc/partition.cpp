/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  partition.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/partition.h>

using namespace leda;



int main()
{
   Partition<string> P;

   for(int i=0; i<100; i++) P.make_block(string("%d",i));

   return 0;
}
  
