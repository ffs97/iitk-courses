/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sortseq_merge.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>
#include <LEDA/core/tuple.h>
#include <LEDA/core/sortseq.h>
#include <assert.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


int convert(int a, int y){ return y;}

double convert(double a, int y){ return ((double) y); }

four_tuple<int,int,int,int> convert(four_tuple<int,int,int,int> a,int y)
       { return four_tuple<int,int,int,int>(0,0,0,y);}


template <class K>
void merge_test(K a,int n, const char* s)
{ cout << "\n\ntest with type " << s <<"\n\n";


for (int m = 1; m <= n; m *= 10)
{ cout << "\n\nm = " << m;
  sortseq<K,int> S1;
  sortseq<K,int> S2;
  sortseq<K,int> S3;
  int k = n/m;
  int i;

  for (i = 0; i < n; i++) 
    { S1.insert(convert(a,2*i) ,0); S3.insert(convert(a,2*i) ,0);}
  for (i = 0; i < m; i++) S2.insert(convert(a,2*k*i+1) ,0); 

  sortseq<K,int> R1(S1);
  sortseq<K,int> R2(S2);
  float T = used_time();
  seq_item it = R2.min_item();
  while (it)
  { R1.insert(R2.key(it),R2.inf(it));
    it = R2.succ(it);
  }

  cout << "\ntime for repeated insert:sortseq = " << used_time(T);
  
  it = R2.min_item();
  while (it)
  { S3.insert(R2.key(it),R2.inf(it));
    it = R2.succ(it);
  }

  cout << "\ntime for repeated insert:sortseq = " << used_time(T);

  R1.clear(); R2.clear(); S3.clear();

  sortseq<K,int> T1(S1);
  sortseq<K,int> T2(S2);

  T = used_time();
  seq_item it1 = T1.min_item();
  seq_item it2 = T2.min_item();

  seq_item succ1 = T1.succ(it1);
  while (it2 && compare(T2.key(it2),T1.key(it1)) < 0)
  { T1.insert_at(it1,T2.key(it2),T2.inf(it2),LEDA::before);
    it2 = T2.succ(it2);
  }
  while (it2)
  { K k2 = T2.key(it2);
    while (succ1 && compare(T1.key(succ1),k2) < 0) 
    { it = succ1;
      succ1 = T1.succ(succ1);
    }
    T1.insert_at(it1,k2,T2.inf(it2),LEDA::after);
    it2 = T2.succ(it2);
  }
  cout << "\ntime for straight merge = " << used_time(T);
  T1.clear();
  T2.clear();
  
  T = used_time();
  S1.merge(S2);

  cout << "\ntime for merge = " << used_time(T);


}
cout << endl;
}



int main(){ 
int n = read_int("n = ");

int a1 = 0; 
double a2 = 0; 

four_tuple<int,int,int,int> a3(0,0,0,0);

merge_test(a1,n,"int");
merge_test(a2,n,"double");
#if !defined(__aCC__)
merge_test(a3,n,"quadruple");
#endif

return 0;

}

