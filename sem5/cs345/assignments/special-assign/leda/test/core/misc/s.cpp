/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  s.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include<LEDA/core/list.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::ostream;
using std::istream;
using std::cout;
using std::endl;
#endif



struct abc{ 
int a; 
int b; 
int c; 

abc(int x, int y, int z) : a(x), b(y), c(z) {}
abc() : a(0), b(0), c(0) {}

};

ostream& operator<<(ostream& out, const abc&) { return out; }
istream& operator>>(istream& in, abc&) { return in; }

main()
{

  list<abc> L;

  L.push_back(abc(1,2,3));

  list<abc>::iterator it;
  for(it = L.begin(); it; it++)
  { cout << it->a << endl;
    cout << it->b << endl;
    cout << it->c << endl;
   }

  return 0;
}
  
