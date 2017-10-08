/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  list2.c
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

int array1 [] = { 1, 16 };
int array2 [] = { 4, 9 };

int main ()
{
  leda::list<int> l1 (array1, array1 + 2);
  leda::list<int> l2 (array2, array2 + 2);
  leda::list<int>::iterator i = l1.begin ();
  i++;
  l1.splice (i, l2, l2.begin (), l2.end ());
  i = l1.begin ();
  while (i != l1.end ())
    cout << *i++ << endl;
  return 0;
}
