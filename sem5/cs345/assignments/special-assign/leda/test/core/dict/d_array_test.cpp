/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d_array_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/d_array.h>
#include <LEDA/core/list.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


#include <LEDA/core/impl/ab_tree.h>
#include <LEDA/core/impl/rs_tree.h>
#include <LEDA/core/impl/avl_tree.h>
#include <LEDA/core/impl/rb_tree.h>
#include <LEDA/core/list.h>
#include <LEDA/core/impl/ch_hash.h>
#include <LEDA/core/impl/skiplist.h>




template<class impl>
void d_array_test(d_array<int,int,impl>& count, list<int>& L, const char* name)
{ 
  int min;
  int max;
  int x;

  cout << string("%-10s",name);
  cout.flush();

  float T = used_time();


  forall(x,L) count[x]++;

  min = max = L.head();

  forall(x,L)
  { if (count[x] > count[max]) max = x;
    if (count[x] < count[min]) min = x;
   }

  cout << "min: " << min << " appears " << count[min] << " times, ";
  cout << "max: " << max << " appears " << count[max] << " times. ";
  cout << string(" (%6.2f sec)",used_time(T));
  cout << endl;
}


int main()
{
  d_array<int,int>           D_ARRAY(0);

  d_array<int,int,avl_tree>  AVL_ARRAY(0);
  d_array<int,int,ab_tree>   AB_ARRAY(0);
  d_array<int,int,rb_tree>   RB_ARRAY(0);
  d_array<int,int,rs_tree>   RS_ARRAY(0);
  d_array<int,int,ch_hash>   CH_ARRAY(0);
  d_array<int,int,skiplist>  SK_ARRAY(0);

  int N = read_int("# keys = ");

  list<int> L;

  while(N--) L.append(rand_int(1,100));

  d_array_test(AB_ARRAY, L,"ab_tree");
  d_array_test(SK_ARRAY, L,"skiplist");
  d_array_test(AVL_ARRAY,L,"avl_tree");
  d_array_test(RS_ARRAY, L,"rs_tree");
  d_array_test(RB_ARRAY, L,"rb_tree");
  d_array_test(CH_ARRAY, L,"ch_hash");
  cout << endl;
  d_array_test(D_ARRAY,L,"default");
 
  return 0;
}

