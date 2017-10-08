/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  list_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


int ord(const int& x) { return x%1000; }

int cmp_rev_int(const int& x, const int& y)   { return compare(y,x); }

int cmp_rev(const double& x, const double& y) 
{ //LEDA::trace_stack(); 
  return compare(y,x); 
}


int main () 
{
  list<int>     L;
  list<int>     L1;
  list<double>   L2;

  int i;

  int N = read_int("Number of list entries: "); 

  float T = used_time();

  rand_int.set_seed(1234*N);

  cout << "allocating         ";
  cout.flush();
  for (i = 0; i < N; i++) L.append(rand_int(1,10000));
  forall(i,L) L2.append(i);
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;


  cout << "reversing          ";
  cout.flush();
  while (!L.empty()) L1.push(L.pop());
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "assignment         ";
  cout.flush();
  L = L1;
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "iteration          ";
  cout.flush();
  forall(i,L) {}
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;


  cout << "sorting(int,rev)   ";
  cout.flush();
  L.sort(cmp_rev_int);
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting(int)       ";
  cout.flush();
  L.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting(double,rev)";
  cout.flush();
  L2.sort(cmp_rev);
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting(double)    ";
  cout.flush();
  L2.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;


/*
  cout << "sorting again      ";
  cout.flush();
  L2.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;
*/


  cout << "test sorting       ";
  cout.flush();
  forall(i,L)
  { double f = L2.pop();
    if (i != f)  
      cout << string("%d  != %f",i,f) << endl;
   }
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;
  
  cout << "bucket sort        ";
  cout.flush();
  L.bucket_sort(0,1000,ord);
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  int count = 0;

  cout << "forall_items       ";
  cout.flush();
  list_item it;
  forall_items(it,L) count += L[it];
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "forall             ";
  cout.flush();
  int x;
  forall(x,L) count += x;
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;



  cout << "clear              ";
  cout.flush();
  L.clear();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << endl;
  cout << "used memory = " << used_memory()/1024.0 << " kb\n";

  std_memory.print_statistics();

  return 0;
}
