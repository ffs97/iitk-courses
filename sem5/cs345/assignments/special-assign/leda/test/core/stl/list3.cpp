/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  list3.c
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

char array [] = { 'x', 'l', 'x', 't', 's', 's' };

int main ()
{
  leda::list<char> str (array, array + 6);
  leda::list<char>::iterator i;

  cout << "original: ";
  for (i = str.begin (); i != str.end (); i++)
    cout << *i;
  cout << endl;
  cout << "reversed: ";
  str.reverse ();
  for (i = str.begin (); i != str.end (); i++)
    cout << *i;
  cout << endl;
  cout << "removed: ";
  str.remove ('x');
  for (i = str.begin (); i != str.end (); i++)
    cout << *i;
  cout << endl;
  cout << "uniqued: ";
  str.unique ();
  for (i = str.begin (); i != str.end (); i++)
    cout << *i;
  cout << endl;
  cout << "sorted: ";
  str.sort ();
  for (i = str.begin (); i != str.end (); i++)
    cout << *i;
  cout << endl;
  return 0;
}
