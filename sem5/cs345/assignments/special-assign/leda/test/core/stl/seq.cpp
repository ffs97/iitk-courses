/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  seq.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/core/sortseq.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


int main()
{
  sortseq<string,int> D;

  seq_item it;
  string s;

  while (cin >> s)
  { it = D.lookup(s);
    if (it==nil) D.insert(s,1);
    else D.change_inf(it,D.inf(it)+1);
   }
  cout << endl;

  // stl iteration
  for(sortseq<string,int>::iterator sit = D.begin(); sit != D.end(); sit++)
   //cout << sit->first << " : " << sit->second << endl;
     cout << (*sit).first << " : " << (*sit).second << endl;
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

