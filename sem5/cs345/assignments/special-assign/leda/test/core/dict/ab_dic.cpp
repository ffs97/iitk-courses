/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  ab_dic.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/core/dictionary.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif


template<class T>
void dic_test(dictionary<T,T>& D, int N, T* A, string name)
{ 
  cout << string("%-12s",name.cstring());
  cout.flush();

  float t;
  float t0 = t = used_time();

  int i;
  for(i=0; i<N; i++)  D.insert(A[i],0);
  cout << string("%10.2f",used_time(t));
  cout.flush();

  bool ok = true;
  for(i=0; i<N; i++)  
  { dic_item it = D.lookup(A[i]);
    if (it == nil || D.key(it) != A[i])  {
     cout << string("error in lookup(%d)",A[i]) << endl;
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
  int     N      = read_int("# keys = ");
  int*    Int    = new int[N];
  int i;
  for(i=0; i<N; i++) Int[i] = rand_int(0,10000000);

  cout << endl;
  cout << "                insert    lookup    delete     total\n";
  cout << endl;

  for(int b = 4; b <= 64; b+=2)
  { dictionary<int,int> D(2,b);
    dic_test(D,N,Int,string("(2,%2d)-tree",b));
   }

  return 0;
}


