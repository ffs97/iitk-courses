/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  seq_test.c
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

#include <LEDA/core/impl/skiplist.h>
#include <LEDA/core/impl/ab_tree.h>
#include <LEDA/core/impl/bin_tree.h>

#include <assert.h>



template<class impl, class E>
void seq_test(sortseq<E,E,impl>& D, int N, E* A, const char* name)
{ 
  typedef typename sortseq<E,E,impl>::item seq_item;

  int i;
  float T0 = used_time();
  float T  = T0;

  cout << string("%-10s",name);
  cout.flush();

  for(i=0; i<N; i++)  D.insert(A[i],0);
  cout << string("%10.2f",used_time(T));
  cout.flush();

  int sum = 0;
  seq_item it;
  forall_items(it,D) sum += D.key(it);
  forall_rev_items(it,D) sum -= D.key(it);

  assert(sum == 0);


  for(i=0; i<N; i++)  D.lookup(A[i]);
  cout << string("%10.2f",used_time(T));
  cout.flush();

  for(i=0; i<N; i++)  D.del(A[i]);
  cout << string("%10.2f",used_time(T));

  assert(D.size() == 0);

  cout << string("%10.2f",used_time(T0));
  cout << endl;
}


int main()
{

 sortseq<int,int>          DEF_SEQ;

 sortseq<int,int,skiplist> SKIP_SEQ;
 sortseq<int,int,ab_tree>  AB_SEQ;
 sortseq<int,int,bin_tree> BIN_SEQ;


  int     N = read_int("# keys = ");
  int* RAND = new int[N];

  for(int i=0; i<N; i++) RAND[i] = rand_int(0,1000000);

  cout << endl;
  cout << "               insert    lookup    delete     total";
  cout << endl;
  cout << endl;

  seq_test(DEF_SEQ,N,RAND,"default");
  seq_test(SKIP_SEQ,N,RAND,"skiplist");
  seq_test(AB_SEQ,N,RAND,"ab_tree");
  seq_test(BIN_SEQ,N,RAND,"bin_tree");

  delete[] RAND;

  return 0;
}
