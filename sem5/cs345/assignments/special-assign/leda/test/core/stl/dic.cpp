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



#include <LEDA/core/dictionary.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif

int main()
{
  dictionary<string,int> D;
  dic_item it;
  string s;

  while (cin >> s)
  { it = D.lookup(s);
    if (it==nil) D.insert(s,1);
    else D.change_inf(it,D.inf(it)+1);
   }


  // stl iteration
  dictionary<string,int>::iterator dit;
   for (dit = D.begin(); dit != D.end(); dit++)
   cout << dit->first << " : " << dit->second << endl;
     //cout << (*dit).first << " : " << (*dit).second << endl;
  cout << endl;


  // leda iteration
  forall_items(it,D) 
  { cout <<  D.key(it) << " : " << D.inf(it) << "\n";
    D.del_item(it);
   }
  cout << endl;


  cout << "size after deletion: " << D.size() << endl;

  return 0;
}

