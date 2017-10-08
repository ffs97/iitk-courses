/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  iterator1.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


// NOTE: For the Borland this include must be the first one 
//       (cf. windows.h: _USE_OLD_RW_STL)
#include <LEDA/core/list.h>

#include <iterator>
#include <algorithm>
#include <list>

#if defined(LEDA_STD_HEADERS)
using namespace std;
#endif

int main()
{
  leda::list<int> L;
  leda::list<int> LL;
  std::copy(std::istream_iterator<int>(std::cin), std::istream_iterator<int>(),
            back_inserter(L));
  std::copy( L.begin(), L.end(), back_inserter(LL));

  cout << "L  = " << L  << endl;
  cout << "LL = " << LL << endl;

  return 0;
}
