/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  map_bug.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/map.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


main()
{

   map<int,int> M(0);

   for(int i=1;i<768; i++) M[i] = i;


   M[700] = M[768]; // evaluation of right side causes rehash that invalidates 
                    // the left reference which was computed before

   cout << M[768] << endl;
   cout << M[700] << endl;

}
