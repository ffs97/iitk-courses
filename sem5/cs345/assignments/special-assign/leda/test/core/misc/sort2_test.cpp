/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sort2_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>
#include <LEDA/core/array.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif



int cmp_func(const int& x, const int& y) {  return compare(x,y); } 


int main () 
{
  int N = read_int("N = "); 


  list<int> L0;

  int i;
  for (i=0; i < N; i++) L0.append(i);
  L0.permute();

  list<int> L1 = L0;
  list<int> L2 = L0;

  for(i=0; i<2; i++)
  {
    float T = used_time();
  
    cout << "quick_sort      ";
    cout.flush();
    L1.sort();
    cout << string("  %5.3f sec",used_time(T));
    cout << endl;
  
    cout << "merge_sort      ";
    cout.flush();
    T = used_time();
    L2.merge_sort();
    cout << string("  %5.3f sec",used_time(T));
    cout << endl;
  

    list_item it1 = L1.first();
    list_item it2 = L2.first();
    while (it1)
    { int x = L1[it1];
      int y = L2[it2];
      if (x != y) error_handler(1, "error in merge_sort");
      it1 = L1.succ(it1);
      it2 = L2.succ(it2);
     }

  }
  cout << endl;

  L1 = L0;
  L2 = L0;

  float T = used_time();

  cout << "quick_sort    ";
  cout.flush();
  L1.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  //L1.print();
  //cout << endl;

  cout << "merge_sort    ";
  cout.flush();
  T = used_time();
  L2.merge_sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  //L2.print();
  //cout << endl;

  return 0;
}

