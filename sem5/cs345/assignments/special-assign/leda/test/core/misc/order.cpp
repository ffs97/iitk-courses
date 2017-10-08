/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  order.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



// This programs is using dictionaries with keys of type "string" with
// different linear orders, the lexicographic ordering  (default ordering)
// the reversed lexicographic ordering, and the ordering by increasing length


#include <LEDA/core/dictionary.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


int cmp_rev(const string& x, const string& y) { return compare(y,x); }

int cmp_length(const string& x, const string& y) 
{ return compare(x.length(),y.length()); }


int main()
{
  dictionary<string,int>  D;
  dictionary<string,int>  D_rev(cmp_rev);
  dictionary<string,int>  D_length(cmp_length);

  string x;

  while (cin >> x) 
  { D.insert(x,0);
    D_rev.insert(x,0);
    D_length.insert(x,0);
   }

  dic_item it;

  forall_items(it,D) cout << D.key(it)  << "\n";
  cout << endl;

  forall_items(it,D_rev) cout << D_rev.key(it)  << "\n";
  cout << endl;

  forall_items(it,D_length) cout << D_length.key(it)  << "\n";
  cout << endl;

  return 0;
}

