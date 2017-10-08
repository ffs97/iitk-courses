#include <LEDA/core/array.h>
#include <LEDA/core/stack.h>
#include <LEDA/core/queue.h>
#include <LEDA/core/quicksort.h>

#include <LEDA/system/multi_thread.h>
#include <assert.h>

const int MAX_THREADS = 256;


using namespace leda;

using std::cout;
using std::cin;
using std::endl;
using std::flush;


template<class E>
struct msort_arg {
  E* left;
  E* right;
  msort_arg(E* l, E* r) : left(l), right(r) {}
};


template<class E>
void merge_up(E* l, E* r, E* r_stop, E* p, int k)
{ E* l_stop = r;
  E* stop = p+k;
  while (p < stop)
  { if (r == r_stop || (l < l_stop && *l < *r)) 
      *p++ = *l++; 
    else
      *p++ = *r++; 
   }
}

template<class E>
void merge_down(E* r, E* l, E* l_stop, E* p, int k)
{ E* r_stop = l;
  E* stop = p-k;
  while (p > stop)
  { if (l == l_stop || (r > r_stop && *r > *l)) 
      *p-- = *r--; 
    else
      *p-- = *l--; 
   }
}


template<class E>
void msort(E* A, int n)
{ 
  int k = n/2;

  E* l = A; 
  E* m = A+k;
  E* r = A+n-1;

  E* tmp = new E[n];

float T = used_time();
  QUICKSORT_C(l,m-1);
  QUICKSORT_C(m,r);

cout << string("qsort: %.2f sec",used_time(T)) << endl;

  merge_up(l,m,r+1,tmp,k);
  merge_down(r,m-1,l-1,tmp+n-1,n-k);

cout << string("merge: %.2f sec",used_time(T)) << endl;

  for(int i=0; i<n; i++) A[i] = tmp[i];

cout << string("copy:  %.2f sec",used_time(T)) << endl;

  delete[] tmp;
}



template<class E>
void merge_up1(E* l, E* r, E* p, int k)
{ E* stop = p+k;
  while (p < stop) *p++ = (*l < *r) ? *l++ : *r++; 
}


template<class E>
void merge_down1(E* r, E* l, E* p, int k)
{ E* stop = p-k;
  while (p > stop) *p-- = (*l < *r) ? *r-- : *l--; 
}


template<class E>
void msort1(E* A, int n)
{ 
  int len = n;
  if (len % 2) len++; // make length even

  int k = len/2;

float T = used_time();

  E* tmp = new E[len];
  for(int i=0; i<n; i++) tmp[i] = A[i];

cout << string("copy:  %.2f sec",used_time(T)) << endl;

  E* l = tmp; 
  E* m = tmp+k;
  E* r = tmp+n-1;

  QUICKSORT_C(l,m-1);
  QUICKSORT_C(m,r);

  if (len > n) tmp[n] = tmp[0]; // sentinel if n is odd

cout << string("qsort: %.2f sec",used_time(T)) << endl;


  merge_up1(l,m,A,k);
  merge_down1(r,m-1,A+n-1,k);

cout << string("merge: %.2f sec",used_time(T)) << endl;

  delete[] tmp;
}



int main()
{
   int n = read_int("size  = ");

   //thread_num = read_int("threads = ");
   //limit = read_int("limit = ");

   int* A = new int[n];
   int* B = new int[n];

   for(int i=0; i<n; i++) A[i] = B[i] = rand_int(30);

   float t1 = used_time();
   float t2 = real_time();
   cout << "merge sort:  " << endl;
   msort(A,n);
   cout << string("total: %.2f (%.2f) sec",used_time(t1),real_time(t2)) <<endl;
   cout << endl;

   int err = 0;
   for(int i=0; i<n; i++) { 
      if (i < n-1 && A[i] > A[i+1]) err++;
    }
   cout << "err = " << err << endl;

   if (err > 0)  error_handler(1,"error in A.sort()");



   t1 = used_time();
   t2 = real_time();
   cout << "merge sort1: " << endl;
   msort1(B,n);
   cout << string("%.2f (%.2f) sec",used_time(t1),real_time(t2)) <<endl;

   err = 0;
   for(int i=0; i<n; i++) { 
      if (i < n-1 && A[i] > A[i+1]) err++;
    }
   cout << "err = " << err << endl;

   if (err > 0)  error_handler(1,"error in A.sort()");

   delete[] A;
   delete[] B;

   return 0;
}
