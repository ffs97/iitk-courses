/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  set.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/set.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


int main()
{
  set<string> D;

  string s;
  while (cin >> s) D.insert(s);
  cout << endl;


  forall(s,D) 
  { cout << s << endl;
    D.del(s);
   }
  cout << endl;

  cout << "size after deletion: " << D.size() << endl;

  return 0;
}

