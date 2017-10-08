/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  leda_vs_gmp.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/numbers/integer.h>
#include <LEDA/numbers/digit_vector.h>
#include <LEDA/system/stream.h>
#include <assert.h>

// #include "/usr/local/include/gmp.h"
#include <gmp.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
using std::flush;
#endif



template <class integer_t> 
string integer_test(const integer_t& A, const integer_t& B, int n)
{
   int f = 500;

   int n1 = f*n;
   int steps = n/10;
   if (steps == 0) steps = n;
   int i;

   integer_t D,P,Q,R,S;

   float T = used_time();

   cout << string("%8d additions        ",n1) << flush;
   T = used_time();
   for(i=0; i < n; i++) 
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     for(int j=0; j < f; j++) S = A+B;
    }
   cout << string(" %4.2f",used_time(T)) << endl;

/*
   cout << string("%8d vec additions    ",n1) << flush;
   T = used_time();
   for(i=0; i < n; i++) 
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     for(int j=0; j < f; j++) 
     { int a_sz = A.used_words();
       int b_sz = B.used_words();
       int s_sz = a_sz + b_sz +1;
       digit* S = new digit[s_sz];
       vecAdd(A.word_vector(),a_sz,B.word_vector(),b_sz,S);
       delete[] S;
     }
    }
   cout << string(" %4.2f",used_time(T)) << endl;
*/

   cout << string("%8d subtractions     ",n1) << flush;
   T = used_time();
   for(i=0; i < n; i++) 
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     for(int j=0; j < f; j++) D = A-B;
    }
   cout << string(" %4.2f",used_time(T)) << endl;

   if (S+D-A != A) 
     cout << "error in addition/subtraction" << endl;

   cout << string("%8d multiplications  ",n) << flush;
   T = used_time();
   for(i=0; i<n; i++) 
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     P = A*B; 
    }
   cout << string(" %4.2f",used_time(T)) << endl;


   cout << string("%8d square           ",n) << flush;
   T = used_time();
   for(i=0; i<n; i++) 
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     P = A.square(); 
    }
   cout << string(" %4.2f",used_time(T)) << endl;

   if (P/A != A) 
     cout << "error in square/division" << endl;


   cout << string("%8d divisions        ",n) << flush;
   for(i=0; i < n; i++)
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     Q = A/B; 
    }
   cout << string(" %4.2f",used_time(T)) << endl;


/*
   cout << string("%8d modulos          ",n) << flush;
   for(i=0; i < n; i++)
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     R = A%B; 
    }
   cout << string(" %4.2f",used_time(T)) << endl;
*/
   R = A%B; 

   if (Q*B + R != A) 
      cout << "error in division/multiplication" << endl;


/*
  cout << "conversion to string " << flush;
  ostrstream out;
  T = used_time();
  out << S << ends;
  cout << string(" %4.2f",used_time(T)) << endl;
  return out.str();
*/

  return "";
}


string gmp_integer_test(const mpz_t& A, const mpz_t& B, int n)
{
   int i;

   int f = 500;

   int n1 = f*n;

   int steps = n/10;

   mpz_t D,P,Q,S;
   //mpz_t R;

   float T;

   cout << string("%8d additions        ",n1) << flush;
   T = used_time();
   for(i=0; i < n; i++) 
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     for(int j=0; j < f; j++) 
     { mpz_init(S); 
       mpz_add(S,A,B);
       mpz_clear(S);
      }
    }
   cout << string(" %4.2f",used_time(T)) << endl;

   cout << string("%8d subtractions     ",n1) << flush;
   T = used_time();
   for(i=0; i < n; i++) 
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     for(int j=0; j < f; j++)
     { mpz_init(D); 
       mpz_sub(D,A,B);
       mpz_clear(D);
      }
    }
   cout << string(" %4.2f",used_time(T)) << endl;


   cout << string("%8d multiplications  ",n) << flush;
   T = used_time();
   for(i=0; i<n; i++) 
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     mpz_init(P); 
     mpz_mul(P,A,B); 
     //mpz_out_str(stdout,16,P);
     mpz_clear(P);
    }
   cout << string(" %4.2f",used_time(T)) << endl;


   cout << string("%8d square           ",n) << flush;
   T = used_time();
   for(i=0; i<n; i++) 
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     mpz_init(P); 
     mpz_mul(P,A,A); 
     //mpz_out_str(stdout,16,P);
     mpz_clear(P);
    }
   cout << string(" %4.2f",used_time(T)) << endl;



   cout << string("%8d divisions        ",n) << flush;
   for(i=0; i < n; i++)
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     mpz_init(Q); 
     mpz_tdiv_q(Q,A,B); 
     mpz_clear(Q);
    }
   cout << string(" %4.2f",used_time(T)) << endl;


/*
   cout << string("%8d modulos          ",n) << flush;
   for(i=0; i < n; i++)
   { if (steps > 0 && i%steps == 0) cout << "." << flush;
     mpz_init(R); 
     mpz_tdiv_r(R,A,B); 
     mpz_clear(R);
    }
   cout << string(" %4.2f",used_time(T)) << endl;
*/

  return "";
}





int main(int argc, char** argv)
{

// use_karatsuba = false;

  int la,lb,n;

  if (argc == 2) 
    { la = atoi(argv[1]);
      lb = la;
      n = 1;
     }
  else
    { la = read_int("#words of A  = ");
      lb = read_int("#words of B  = ");
      n  = read_int("iterations   = ");
     }

  unsigned long* a = new unsigned long[la];
  unsigned long* b = new unsigned long[lb];

  random_source ran;

  int i;
  for(i=0; i<la; i++) a[i] = ran();
  for(i=0; i<lb; i++) b[i] = ran();

  //for(i=0; i<la; i++) a[i] = 0xffffffff;
  //for(i=0; i<lb; i++) b[i] = 0xffffffff;

  //integer A = integer(la,a);
  //integer B = integer(lb,b);

  float T;

  cout << endl;
  cout << "LEDA integers" << endl;

  integer A,B;
  cout << string("constructing A and B     ",n) << flush;
  T = used_time();
/*
  for(i=0; i<la; i++) A = (A << 32) + a[i];
  for(i=0; i<lb; i++) B = (B << 32) + b[i];
*/
  for(i=0; i<la; i++) { A <<= 32;  A |= a[i]; }
  for(i=0; i<lb; i++) { B <<= 32; B |= b[i]; }
  cout << string(" %4.2f",used_time(T)) << endl;

  string s0 = integer_test(A,B,n);

  cout << endl;
  cout << endl;
  cout << "GMP " << gmp_version  << endl;

  mpz_t A1,B1;

  mpz_init(A1);
  mpz_init(B1);

  cout << string("constructing A and B     ",n) << flush;
  T = used_time();
  for(i=0; i<la; i++) 
  { mpz_mul_2exp(A1,A1,32);
    mpz_add_ui(A1,A1,a[i]);
   }
  for(i=0; i<lb; i++) 
  { mpz_mul_2exp(B1,B1,32);
    mpz_add_ui(B1,B1,b[i]);
   }
  cout << string(" %4.2f",used_time(T)) << endl;

  string s1 = gmp_integer_test(A1,B1,n);

  assert(s0 == s1);

  delete[] a;
  delete[] b;

  return 0;
}
