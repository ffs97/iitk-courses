/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sort_test.c
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
#include <LEDA/core/array.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
#endif



typedef two_tuple<int,int> Pair;

inline bool operator<(const Pair& p1, const Pair& p2)
{ 
  if (p1.first() < p2.first())
    return true;
  else
    if (p1.first() == p2.first() && p1.second() < p2.second())
      return true;
    else 
      return false;
}

class pair_smaller {
public:
bool operator()(const Pair& p1, const Pair& p2) const
{ 
  if (p1.first() < p2.first())
    return true;
  else
    if (p1.first() == p2.first() && p1.second() < p2.second())
      return true;
    else 
      return false;
}
};


int main () 
{
  int i;
  int N = read_int("N = "); 

  float T = used_time();

  array<int> Ai(N);
  array<int> A1(N);
  for (i = 0; i < N; i++) A1[i] = Ai[i] = rand_int(1,1000000);

  cout << "building arrays       ";
  cout.flush();

  array<long>   Al(N);
  array<float>  Af(N);
  array<double> Ad(N);
  array<Pair>   Ap(N);
  array<Pair>   Aq(N);

  for (i = 0; i < N; i++) 
  { Al[i] = Ai[i];
    Af[i] = Ai[i];
    Ad[i] = Ai[i];
    Ap[i] = Pair(Ai[i],Ai[i]);
    Aq[i] = Pair(Ai[i],Ai[i]);
   }

  cout << string("  %5.3f sec",used_time(T));
  cout << endl;
 

  cout << "building lists        ";
  cout.flush();

  list<int>     Li;
  list<long>    Ll;
  list<float>   Lf;
  list<double>  Ld;
  list<Pair>    Lp;
  list<Pair>    Lq;

  for (i = 0; i < N; i++) 
  { Li.append(Ai[i]);
    Ll.append(Ai[i]);
    Lf.append(Af[i]);
    Ld.append(Ad[i]);
    Lp.append(Pair(Ai[i],Ai[i]));
    Lq.append(Pair(Ai[i],Ai[i]));
  }

  cout << string("  %5.3f sec",used_time(T));
  cout << endl;
  cout << endl;

  cout << "sorting array<int>    ";
  cout.flush();
  Ai.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;


  cout << "sorting again         ";
  cout.flush();
  Ai.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "mergesort array<int>  ";
  cout.flush();
  A1.merge_sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting again         ";
  cout.flush();
  A1.merge_sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting array<long>   ";
  cout.flush();
  Al.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;


  cout << "sorting array<float>  ";
  cout.flush();
  Af.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting array<double> ";
  cout.flush();
  Ad.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting array<Pair>   ";
  cout.flush();
  Ap.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  pair_smaller smaller;

  cout << "sorting array<Pair>   ";
  cout.flush();
  Aq.sort_smaller(smaller);
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << endl;



  cout << "sorting list<int>     ";
  cout.flush();
  Li.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting again         ";
  cout.flush();
  Li.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting list<long>    ";
  cout.flush();
  Ll.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting list<float>   ";
  cout.flush();
  Lf.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting list<double>  ";
  cout.flush();
  Ld.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting list<Pair>    ";
  cout.flush();
  Lp.sort();
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;

  cout << "sorting list<Pair>    ";
  cout.flush();
  Lq.sort_smaller(smaller);
  cout << string("  %5.3f sec",used_time(T));
  cout << endl;


  cout << endl;

  cout << "test sorting          " << endl;

  int err_count = 0;

  for(i=0;i<N;i++)
  { int    x = Li.pop();
    double z = Ld.pop();
    Pair   p = Lp.pop();
    if (x != z || x != p.first() 
     || x != Ai[i] || x != Af[i] || x != Ad[i] 
     || x != Ap[i].first())
    //cout << x << " " << y << " " << z << " " << p << endl;
    err_count++;
   }

  cout << string("%d errors   %5.3f sec",err_count,used_time(T));
  cout << endl;

  return 0;

}


