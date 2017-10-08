/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  prio.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/core/p_queue.h>

using namespace leda;


#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


int main()
{
  p_queue<string,int> D;
  pq_item it;
  string s;
  int i=0;

  while (cin >> s) D.insert(s,i++);

  // stl iteration
  p_queue<string,int>::iterator pit;
  for (pit = D.begin(); pit != D.end(); pit++)
   //cout << pit->first << " : " << pit->second << endl;
     cout << (*pit).first << " : " << (*pit).second << endl;
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

