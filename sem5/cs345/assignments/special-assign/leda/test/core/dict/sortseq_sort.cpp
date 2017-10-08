/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sortseq_sort.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>
#include <LEDA/core/tuple.h>
#include <LEDA/core/list.h>
#include <LEDA/core/string.h>
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
void sort_demo(K a, int n, const char* s)
{ cout << "\n\ntest with type " << s <<"\n\n";
  int f = 0; int powf = 1;
  while (powf < n) {f++; powf *=2;}
  f = 200;
{ cout << "\n\nf = " << f;
  list<K > L;

{
  for(int i = 0; i < n; i++)
    { list_item it = L.last();
      int j = f-1;
      while (it && j > 0)
        { it = L.pred(it); j--; }
      if (it) L.insert(convert(a,i),it,LEDA::after);
      else L.push(convert(a,i));
    }
}
 
  sortseq<K,int> S;
  K i; 
  float T = used_time();
  forall(i,L)
     if (S.empty()) S.insert(i, 0); 
    else S.insert_at(S.max_item(),i,0,LEDA::after);
  cout << "\ncopying L to S " << used_time(T);
  S.clear();
  T = used_time();
  forall(i,L)
     S.insert(i, 0); 
  cout << "\nrepeated insertion " << used_time(T);
  S.clear();
  T = used_time();
  forall(i, L)
  { if (S.empty()) S.insert(i, 0); 
    else 
    { seq_item it = S.finger_locate(S.max_item(), i); 
      if (it) S.insert_at(it,i,0,LEDA::before); 
      else S.insert_at(S.max_item(),i,0,LEDA::after);
    }
  }
  cout << "\nfinger sort1: " << used_time(T);
  S.clear();
  T = used_time();
  S.insert(L.head(),0);
  list_item it = L.succ(L.first());
  i = L.back();
  while (it)
  { seq_item s_it = S.finger_locate(i); 
      if (s_it) S.insert_at(s_it,i,0,LEDA::before); 
      else S.insert_at(S.max_item(),i,0,LEDA::after);
    it = L.succ(it);
  }
  cout << "\nfinger sort2: " << used_time(T);
  S.clear();

  list<K > L1(L);
  L1.sort();
  cout << "\nlist sort: " << used_time(T);
}
cout << endl;cout << endl;
}


int main(){ 
int n = read_int("n = ");

int a1 = 0; 
double a2 = 0; 
four_tuple<int,int,int,int> a3(0,0,0,0);

sort_demo(a1,n,"int");
sort_demo(a2,n,"double");
#if !defined(__aCC__)
sort_demo(a3,n,"four_tuple");
#endif

return 0;

}

