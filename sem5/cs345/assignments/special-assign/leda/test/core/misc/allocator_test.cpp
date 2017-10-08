/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  allocator_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/system/allocator.h>
#include <vector>

using namespace leda;

using std::vector;

static int m=10;

template <class T, class A1, class A2>
void test_alloc(vector< vector<T,A1>, A2>& V);

int main(int argc, char* argv[])
{
  if (argc < 2) {
    cout << "usage:\n";
    cout << "       " << argv[0] << " number_of_iterations\n";
    return 0;
  }

  print_statistics();
  int n = atoi(argv[1]);
  float t0 = used_time();
  for (int i = 0; i < n; ++i) {
    vector< vector<int> > V_std;
    test_alloc(V_std);
  }
  float t1 = used_time(t0);
  for (int j = 0; j < n; ++j) {
    vector < vector<int, leda_allocator<int> >, 
	     leda_allocator< vector<int, leda_allocator<int> > > > V_leda;
    test_alloc(V_leda);
  }
  float t2 = used_time(t1);
  cout << "used times (std,leda) " << t1 << " " << t2 << endl;
  print_statistics();
  return 0;
}

template <class T, class A1, class A2>
void test_alloc(vector< vector<T,A1>, A2>& V)
{
  V = vector< vector<T,A1>, A2> (m);
  for (int i = 0; i < m; ++i) {
    V[i] = vector<T,A1>(m);
    for (int j = 0; j < m; ++j) V[i][j]=j;
  }
}

