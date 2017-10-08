/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  prio_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/p_queue.h>

#include <LEDA/core/impl/k_heap.h>
#include <LEDA/core/impl/bin_heap.h>
#include <LEDA/core/impl/p_heap.h>
#include <LEDA/core/impl/r_heap.h>
/*
#include <LEDA/core/impl/m_heap.h>
#include <LEDA/core/impl/list_pq.h>
*/

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


typedef float FLOAT;


template<class impl, class E>
void prio_test(p_queue<E,E,impl>& D, int N, E* A, const char* name)
{ 
  typedef typename p_queue<int,int,impl>::item pq_item;

  pq_item* I = new pq_item[N];

  cout << string("%-12s",name);
  cout.flush();

  float T;
  float T0 = T = used_time();

  int i;
  for(i=0; i<N; i++)  I[i] = D.insert(A[i],i);
  D.insert(0,0);
  cout << string("%10.2f",used_time(T));
  cout.flush();

  for(i=0; i<N; i++)  D.decrease_p(I[i],A[i]/2);
  cout << string("%14.2f",used_time(T));
  cout.flush();


  E old_min = 0;

  for(i=0; i<N+1; i++)  
  { pq_item it = D.find_min();
    if (it == nil || D.prio(it) < old_min) error_handler(1,"error in del_min");
    old_min = D.prio(it);
    D.del_min();
   }

  cout << string("%10.2f",used_time(T));

  cout << string("%10.2f",used_time(T0));

  if (!D.empty()) cout << " NOT EMPTY !!\n";	

  cout << endl;

  delete[] I;
}



int main()
{ p_queue<int,int>            Q;
  p_queue<FLOAT,FLOAT>        Qf;

  int    N     = read_int("# keys = ");
  int*   Int   = new int[N];
  FLOAT* Float = new FLOAT[N];

  random_source ran(0,999);

  for(int i=0; i<N; i++) Float[i] = Int[i] = ran();

  p_queue<int,int,k_heap>    k_q(N+1);
  p_queue<int,int,bin_heap>  b_q(N+1);
  p_queue<int,int,p_heap>    p_q;
//p_queue<int,int,r_heap>    r_q(1000);
//p_queue<int,int,m_heap>    m_q(1000);
//p_queue<int,int,list_pq>   l_q;

  p_queue<FLOAT,FLOAT,k_heap>    k_qf(N+1);
  p_queue<FLOAT,FLOAT,bin_heap>  b_qf(N+1);
  p_queue<FLOAT,FLOAT,p_heap>    p_qf;
  p_queue<FLOAT,FLOAT,r_heap>    r_qf(1000);
//p_queue<FLOAT,FLOAT,m_heap>    m_qf(1000);
//p_queue<FLOAT,FLOAT,list_pq>   l_qf;



  cout << endl;
  cout << "                insert   decrease_p    del_min    total\n";
  cout << endl;

  prio_test(Q,N,Int,"prio");
  prio_test(k_q,N,Int,"k_heap");
  prio_test(b_q,N,Int,"b_heap");
  prio_test(p_q,N,Int,"p_heap");
//prio_test(r_q,N,Int,"r_heap");
  cout << endl;
//prio_test(m_q,N,Int,"m_heap");
//prio_test(l_q,N,Int,"list_pq");

  prio_test(Qf,N,Float,"prio");
  prio_test(k_qf,N,Float,"k_heap");
  prio_test(b_qf,N,Float,"b_heap");
  prio_test(p_qf,N,Float,"p_heap");
//prio_test(r_qf,N,Float,"r_heap");
  cout << endl;
//prio_test(m_qf,N,Float,"m_heap");
//prio_test(l_qf,N,Float,"list_pq");


  delete[]  Int;
  delete[]  Float;
 
  return 0;
}
