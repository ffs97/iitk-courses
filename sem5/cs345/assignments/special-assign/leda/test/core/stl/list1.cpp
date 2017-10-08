/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  list1.c
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

int array1 [] = { 9, 16, 36 };
int array2 [] = { 1, 4 };

void print(const leda::list<int>& L)
{ leda::list<int>::const_iterator i = L.begin ();
  while (i != L.end ()) cout << *i++ << endl;
  cout << endl;
}

int main ()
{
  leda::list<int> l1 (array1, array1 + 3);
  leda::list<int> l2 (array2, array2 + 2);
  leda::list<int>::iterator i1 = l1.begin ();
  l1.splice (i1, l2);
  leda::list<int>::iterator i2 = l1.begin ();
  while (i2 != l1.end ()) cout << *i2++ << endl;
  cout << endl;

  print(l1);
  return 0;
}

