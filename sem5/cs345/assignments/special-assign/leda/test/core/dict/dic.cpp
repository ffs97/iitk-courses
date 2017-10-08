/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dic.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#define LEDA_STL_ITERATORS

#include <LEDA/core/dictionary.h>
#include <LEDA/core/impl/ch_hash.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


LEDA_BEGIN_NAMESPACE
// inline int Hash(const string& s)  { return (s.length() > 0) ? s[0] : 0; }
LEDA_END_NAMESPACE


int main()
{
  dictionary<string,int> D;

/*
  _dictionary<string,int,ch_hash> D;
*/

  dic_item it;
  string s;

  while (cin >> s)
  { it = D.lookup(s);
    if (it==nil) D.insert(s,1);
    else D.change_inf(it,D.inf(it)+1);
   }

  dictionary<string,int>::iterator dit;
   for (dit = D.begin(); dit != D.end(); dit++)
    //cout << dit->first << " : " << dit->second << endl;
      cout << (*dit).first << " : " << (*dit).second << endl;
  cout << endl;

  forall_items(it,D) 
  {  cout <<  D.key(it) << " : " << D.inf(it) << "\n";
     D.del_item(it);
   }
  cout << endl;

  cout << "size after deletion: " << D.size() << endl;

  return 0;
}

