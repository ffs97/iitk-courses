/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  list4.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/list.h>

#if defined(LEDA_STD_HEADERS)
using namespace std;
#endif

int array1 [] = { 1, 3, 6, 7 };
int array2 [] = { 2, 4 };

int main ()
{
  leda::list<int> l1 (array1, array1 + 4);
  leda::list<int> l2 (array2, array2 + 2);
  l1.merge (l2);
  for (leda::list<int>::iterator i = l1.begin (); i != l1.end (); i++)
    cout << *i;
  cout << endl;
  return 0;
}
