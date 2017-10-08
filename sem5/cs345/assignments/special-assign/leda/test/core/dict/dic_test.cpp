/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dic_test.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/dictionary.h>

#include <LEDA/core/impl/ch_hash.h>
#include <LEDA/core/impl/avl_tree.h>
#include <LEDA/core/impl/bin_tree.h>
#include <LEDA/core/impl/rs_tree.h>
#include <LEDA/core/impl/rb_tree.h>
#include <LEDA/core/impl/skiplist.h>
#include <LEDA/core/impl/ab_tree.h>
#include <LEDA/core/impl/bb_tree.h>
#include <LEDA/core/impl/cuckoo_hash.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


template<class T, class impl>
void dic_test(dictionary<T,T,impl>& D, int N, T* A, const char* name)
{ 
  typedef typename dictionary<T,T,impl>::item dic_item;

  cout << string("%-12s",name);
  cout.flush();

  float t;
  float t0 = t = used_time();

  bool ok = true;
  int i;
  for(i=0; i<N; i++)
  { dic_item it;
    it = D.insert(A[i],0);
    if (it == nil || D.key(it) != A[i])  {
     cout << string("error in insert(%d)",A[i]) << endl;
     ok = false;
    }
   }

  if (!ok) error_handler(0,"errors in inserts");

  cout << string("%10.2f",used_time(t));
  cout.flush();


  for(i=0; i<N; i++)  
  { dic_item it = D.lookup(A[i]);
    if (it == nil || D.key(it) != A[i])  {
     cout << string("error in lookup(%d): %d",A[i], it ? D.key(it) : 0) << endl;
     ok = false;
    }
   }

  if (!ok) error_handler(0,"errors in lookups");

  cout << string("%10.2f",used_time(t));
  cout.flush();

  for(i=0; i<N; i++)  D.del(A[i]);
  cout << string("%10.2f",used_time(t));

  cout << string("%10.2f",used_time(t0));

  if (!D.empty())
  {  cout << " NOT EMPTY ";	
     cout << "size = " << D.size() << endl;
   }
  

  cout << endl;

//memory_clear();

}

int cmp_int(const int& x, const int& y) { return -compare(x,y); }
int cmp_double(const double& x, const double& y) { return -compare(x,y); }


int main()
{
  // determine a new seed and report it!
  int seed = rand_int.reinit_seed();
  cout << "seed = " << seed << endl << endl; cout.flush();

  dictionary<int,int,ch_hash> CHH_DIC;

  dictionary<int,int,cuckoo_hash> CUCKOO_DIC;


  dictionary<int,int,avl_tree>   AVL_DIC;
  dictionary<int,int,avl_tree>   AVL_DIC_cmp(cmp_int);
  dictionary<int,int,bin_tree>   BIN_DIC;
  dictionary<int,int,rb_tree>    RB_DIC;
  dictionary<int,int,rs_tree>    RS_DIC;
  dictionary<int,int,skiplist>   SK_DIC;
  dictionary<int,int,bb_tree>    BB_DIC;
  dictionary<int,int,ab_tree>    AB_DIC;

  dictionary<double,double,avl_tree>   AVL_DIC1;
  dictionary<double,double,bin_tree>   BIN_DIC1;
  dictionary<double,double,rb_tree>    RB_DIC1;
  dictionary<double,double,rs_tree>    RS_DIC1;
  dictionary<double,double,skiplist>   SK_DIC1;
  dictionary<double,double,bb_tree>    BB_DIC1;
  dictionary<double,double,ab_tree>    AB_DIC1;


  int     N      = read_int("# keys = ");
  int*    Int    = new int[N];
  int*    Int1   = new int[N];
  double* Double = new double[N];

  int i;

  for(i=0; i<N; i++) Double[i] = Int[i] = rand_int(0,10000000);

  for(i=0; i<N; i++) Int1[i] = i;


  cout << endl;
  cout << "                insert    lookup    delete     total\n";
  cout << endl;


  cout << "Random Integers" << endl;
  cout << endl;


  dic_test(CHH_DIC,N,Int,"ch_hash");
  cout << endl;

  dic_test(CUCKOO_DIC,N,Int,"cuckoo_hash");
  cout << endl;

  dic_test(AB_DIC,N,Int,"ab_tree");
  dic_test(SK_DIC,N,Int,"skiplist");
  dic_test(RS_DIC,N,Int,"rs_tree");
  dic_test(BIN_DIC,N,Int,"bin_tree");
  dic_test(RB_DIC,N,Int,"rb_tree");
  dic_test(AVL_DIC,N,Int,"avl_tree");
  dic_test(AVL_DIC_cmp,N,Int,"avl_tree(c)");
  dic_test(BB_DIC,N,Int,"bb_tree");
  cout << endl;


  cout << "Sorted Integers" << endl;
  cout << endl;

  dic_test(CHH_DIC,N,Int1,"ch_hash");
  cout << endl;

  dic_test(CUCKOO_DIC,N,Int1,"cuckoo_hash");
  cout << endl;

  dic_test(AB_DIC,N,Int1,"ab_tree");
  dic_test(SK_DIC,N,Int1,"skiplist");
  dic_test(RS_DIC,N,Int1,"rs_tree");
//dic_test(BIN_DIC,N,Int1,"bin_tree");
  dic_test(RB_DIC,N,Int1,"rb_tree");
  dic_test(AVL_DIC,N,Int1,"avl_tree");
  dic_test(BB_DIC,N,Int1,"bb_tree");
  cout << endl;


  cout << "Random  Double's" << endl;
  cout << endl;

  dic_test(AB_DIC1,N,Double,"ab_tree");
  dic_test(SK_DIC1,N,Double,"skiplist");
  dic_test(RS_DIC1,N,Double,"rs_tree");
  dic_test(BIN_DIC1,N,Double,"bin_tree");
  dic_test(RB_DIC1,N,Double,"rb_tree");
  dic_test(AVL_DIC1,N,Double,"avl_tree");
  dic_test(BB_DIC1,N,Double,"bb_tree");
  cout << endl;

  delete[] Int;
  delete[] Int1;
  delete[] Double;
 
  return 0;
}


