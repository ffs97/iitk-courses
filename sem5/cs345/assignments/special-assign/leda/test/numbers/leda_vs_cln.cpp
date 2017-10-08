/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  leda_vs_cln.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/numbers/integer.h>
#include <LEDA/system/stream.h>
#include <assert.h>

#include <cln/integer.h>
#include <cln/integer_io.h>

using namespace leda;
using namespace cln;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
using std::ends;
using std::flush;
#endif



template <class integer_t> 
string integer_test(const integer_t& A, const integer_t& B, int n)
{
   int i;

   int n1 = 1000*n;

   int steps = n/10;
   if (steps == 0) steps = n;

   integer_t D,P,Q,R,S;

   float T = used_time();

   cout << string("%8d additions        ",n1) << flush;
   T = used_time();
   for(i=0; i < n; i++) 
   { if (i%steps == 0) cout << "." << flush;
     for(int j=0; j < 1000; j++) S = A+B;
    }
   cout << string(" %4.2f",used_time(T)) << endl;

   cout << string("%8d subtractions     ",n1) << flush;
   T = used_time();
   for(i=0; i < n; i++) 
   { if (i%steps == 0) cout << "." << flush;
     for(int j=0; j < 1000; j++) D = A-B;
    }
   cout << string(" %4.2f",used_time(T)) << endl;

   if (S+D-A != A) 
     cout << "error in addition/subtraction" << endl;

   cout << string("%8d multiplications  ",n) << flush;
   T = used_time();
   for(i=0; i<n; i++) 
   { if (i%steps == 0) cout << "." << flush;
     P = A*B; 
    }
   cout << string(" %4.2f",used_time(T)) << endl;

/*
   cout << string("%8d divisions        ",n) << flush;
   for(i=0; i < n; i++) { Q = A/B; }
   cout << string(" %4.2f",used_time(T)) << endl;


   cout << string("%8d modulos          ",n) << flush;
   for(i=0; i < n; i++) { R = A%B; }
   cout << string(" %4.2f",used_time(T)) << endl;

   if (Q*B + R != A) 
      cout << "error in division/multiplication" << endl;
*/

  cout << "conversion to string     " << flush;
  string_ostream out;
  T = used_time();
  out << S << ends;
  cout << string(" %4.2f",used_time(T)) << endl;

  return out.str();
}


int main()
{

  int la = read_int("#words of A  = ");
  int lb = read_int("#words of B  = ");
  int n  = read_int("iterations   = ");

  unsigned long* a = new unsigned long[la];
  unsigned long* b = new unsigned long[lb];

  random_source ran;

  int i;
  for(i=0; i<la; i++) a[i] = ran();
  for(i=0; i<lb; i++) b[i] = ran();

  //integer A = integer(la,a);
  //integer B = integer(lb,b);

  float T;

  cout << endl;
  cout << "CLN  integers" << endl;

  cl_I A1,B1;
  cout << string("constructing A and B     ",n) << flush;
  T = used_time();
  for(i=0; i<la; i++) A1 = (A1 << 32) + a[i];
  for(i=0; i<lb; i++) B1 = (B1 << 32) + b[i];
  cout << string(" %4.2f",used_time(T)) << endl;
  string s1 = integer_test(A1,B1,n);


  cout << endl;
  cout << endl;
  cout << "LEDA integers" << endl;

  integer A,B;
  cout << string("constructing A and B     ",n) << flush;
  T = used_time();
  for(i=0; i<la; i++) A = (A << 32) + a[i];
  for(i=0; i<lb; i++) B = (B << 32) + b[i];
  cout << string(" %4.2f",used_time(T)) << endl;
  string s0 = integer_test(A,B,n);

  assert(s0 == s1);

  delete[] a;
  delete[] b;

  return 0;
}
