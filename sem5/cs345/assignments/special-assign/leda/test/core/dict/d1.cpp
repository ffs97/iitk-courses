/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d1.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/list.h>
#include <LEDA/dic.h>
#include <LEDA/core/impl/avl_tree.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif



template <class obj_type, class dic_type>
void WORD_COUNT(list<obj_type>& L, dic_type& D)
{ 
  dic_type::item it;

  D.clear();
  
  obj_type s;
  forall(s,L) 
  { it = D.lookup(s);
    if (it == nil)
       D.insert(s,1);
    else
       D.change_inf(it,D.inf(it)+1);
   }

  forall_items(it,D)
     cout << D.key(it) << " appeared " << D.inf(it) << " times." << endl;
}


int main()
{
   list<string> L;
   dictionary<string,int,avl_tree> D;

   L.read("L = ");

   WORD_COUNT(L,D);

}
