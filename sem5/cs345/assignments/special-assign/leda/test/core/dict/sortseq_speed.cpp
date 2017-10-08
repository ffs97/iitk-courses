/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sortseq_speed.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>
#include <LEDA/core/tuple.h>
#include <LEDA/core/string.h>
#include <LEDA/core/random.h>
/*
#include <LEDA/core/_sortseq.h>
*/
#include <LEDA/core/sortseq.h>

#include <assert.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif


#include <LEDA/core/impl/skiplist.h>


int convert(int a, int y){ return y;}

double convert(double a, int y){ return ((double) y); }

four_tuple<int,int,int,int> convert(four_tuple<int,int,int,int> a,int y)
       { return four_tuple<int,int,int,int>(0,0,0,y);}


template <class K>
void speed_test(K a, int n, const char* s)
{ cout << "\n\ntest with type " << s <<"\n\n";
  K* A = new K[n];
  for (int i = 0; i < n; i++)
      { int x = rand_int(0,1000000);
        A[i] = convert(a,x);
      }
  float T = used_time();
  { sortseq<K,int> S;
    for (int i = 0; i < n; i++)
      S.insert(A[i],0);
    S.clear();
    cout << "\n\nused time for sortseq = " << used_time(T);
  }
/*
  { _sortseq<K,int,skiplist> S;
    for (int i = 0; i < n; i++)
      S.insert(A[i],0);
    S.clear();
    cout << "\n\nused time for sortseq = " << used_time(T);
  }
*/
  cout << "\n\nand now the sorted insertions";
  { sortseq<K,int> S;
    S.insert(convert(a,0),0);
    for (int i = 1; i < n; i++)
      S.insert_at(S.max(),convert(a,i),0);
    S.clear();
    cout << "\n\nused time for sortseq = " << used_time(T);
  }
/*
  { _sortseq<K,int,skiplist> S;
    S.insert(convert(a,0),0);
    for (int i = 1; i < n; i++)
      S.insert_at(S.max(),convert(a,i),0);
    S.clear();
    cout << "\n\nused time for sortseq = " << used_time(T);
  }
*/
  
  cout << endl;
  delete[] A;
}

int main(){

int n = read_int("n = ");

int a1 = 0; 
double a2 = 0; 
four_tuple<int,int,int,int> a3(0,0,0,0);


speed_test(a1,n,"int");
speed_test(a2,n,"double");
#if !defined(__aCC__)
speed_test(a3,n,"quadruple");
#endif

return 0;

}


