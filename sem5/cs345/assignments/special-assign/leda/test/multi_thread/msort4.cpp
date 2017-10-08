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


void print_time(string txt, float cpu_t, float real_t)
{
  float speed_up = (real_t > 0.001) ? cpu_t/real_t : 0;

  cout << string("%6s: %5.2f (%5.2f) sec  speed_up: %3.2f ", ~txt,
                 cpu_t, real_t, speed_up) << endl;
}


template<class E>
struct qsort_arg {
  E*  left;
  E*  right;
  qsort_arg(E* l, E* r) : left(l), right(r) {}
};


template<class E>
struct merge_arg {
  E*  a;
  E*  b;
  E*  out;
  int sz;
  merge_arg(E* q, E* r, E* p, int n) : a(q), b(r), out(p), sz(n) {}
};



template<class E>
void merge_up(E* l, E* r, E* p, int k)
{ E* stop = p+k;
  while (p < stop) *p++ = (*l < *r) ? *l++ : *r++; 
}


template<class E>
void merge_down(E* r, E* l, E* p, int k)
{ E* stop = p-k;
  while (p > stop) *p-- = (*l < *r) ? *r-- : *l--; 
}


template<class E>
void* merge_up_thread(void* p)
{ merge_arg<E>* arg = (merge_arg<E>*)p;
  merge_up(arg->a,arg->b,arg->out,arg->sz);
  return NULL;
}

template<class E>
void* merge_down_thread(void* p)
{ merge_arg<E>* arg = (merge_arg<E>*)p;
  merge_down(arg->a,arg->b,arg->out,arg->sz);
  return NULL;
}



template<class E>
void* qsort_thread(void* p)
{ qsort_arg<E>* arg = (qsort_arg<E>*)p;
  QUICKSORT_C(arg->left,arg->right);
  return NULL;
}


template<class E>
void merge2(E* A, E* B, int n) 
{ int k = n/2;
  merge_arg<int> arg3(A,A+k,B,k);
  merge_arg<int> arg4(A+n-1,A+k-1,B+n-1,n-k);
  thread_t thr[2];
  thr[0] = thread_create(merge_up_thread<int>,&arg3);
  thr[1] = thread_create(merge_down_thread<int>,&arg4);
  thread_join(thr[0]);
  thread_join(thr[1]);
}



template<class E>
void msort2(E* A, E* B, int n)
{ 
  float T = used_time();
  float R = real_time();

  int k = n/2;

  qsort_arg<int> arg1(A,A+k-1);
  qsort_arg<int> arg2(A+k,A+n-1);

  thread_t thr[2];
  thr[0] = thread_create(qsort_thread<E>,&arg1);
  thr[1] = thread_create(qsort_thread<E>,&arg2);
  thread_join(thr[0]);
  thread_join(thr[1]);

  print_time("qsort",used_time(T),real_time(R));

  if (A[0] > A[n-1]) swap(A[0],A[n-1]);

  merge2(A,B,n);

  print_time("merge",used_time(T),real_time(R));
}


template<class E>
struct msort2_arg {
  E*  A;
  E*  B;
  int sz;
  msort2_arg(E* p, E* q, int n) : A(p), B(q), sz(n) {}
};


template<class E>
void* msort2_thread(void* p)
{ msort2_arg<E>* arg = (msort2_arg<E>*)p;
  msort2(arg->A,arg->B,arg->sz);
  return NULL;
}


template<class E>
void msort4(E* A, E* B, int n)
{ 
  float T = used_time();
  float R = real_time();

  int k = n/2;

  msort2_arg<E> arg1(A,B,k);
  msort2_arg<E> arg2(A+k,B+k,n-k);

  thread_t thr[2];
  thr[0] = thread_create(msort2_thread<E>,&arg1);
  thr[1] = thread_create(msort2_thread<E>,&arg2);
  thread_join(thr[0]);
  thread_join(thr[1]);

  print_time("msort2",used_time(T),real_time(R));

  merge2(B,A,n);

  print_time("merge2",used_time(T),real_time(R));
}
  



int main()
{
   int n = read_int("size  = ");

   //thread_num = read_int("threads = ");
   //limit = read_int("limit = ");

   int* A = new int[n];
   int* B = new int[n];

   for(int i=0; i<n; i++) B[i] = A[i] = rand_int(30);

   float t1 = used_time();
   float t2 = real_time();

   cout << "merge sort:  " << endl;
   cout << endl;
   msort4(A,B,n);
   cout << endl;
   print_time("total",used_time(t1),real_time(t2));
   cout << endl;

   int err = 0;
   for(int i=0; i<n; i++) { 
      if (i < n-1 && A[i] > A[i+1]) err++;
    }

   cout << "err = " << err << endl;
   cout << endl;

   if (err > 0)  error_handler(1,"error in sort");


   delete[] A;
   delete[] B;

   return 0;
}
