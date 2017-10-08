#include <LEDA/core/array.h>
#include <LEDA/core/stack.h>
#include <LEDA/core/queue.h>
#include <LEDA/system/multi_thread.h>
#include <assert.h>

const int MAX_THREADS = 64;

using namespace leda;

using std::cout;
using std::cin;
using std::endl;
using std::flush;



template<class E>
struct qsort_arg {
  E* left;
  E* right;
  E  pivot;
};


static int limit = 0;
static int thread_num = 0;

static qsort_arg<int> Arg[MAX_THREADS];
static thread_t thr[MAX_THREADS];


void* partition_thread(void* p)
{ 

  qsort_arg<int>* arg = (qsort_arg<int>*)p;

  int* l = arg->left;
  int* r = arg->right;
  int s = arg->pivot;

//cout << string("thread started: sz = %d",r-l+1) << endl;

  int* i = l-1;
  int* j = r+1;

  for(;;)        
  { while (i < r && *++i < s);
    while (j >= l && *--j > s);
    if (i<j) swap(*i,*j);
    else break;
   }             

  arg->left = j;

//cout << string("thread finished: j = %d",j-l) << endl;

  //thread_exit(NULL);
  return NULL;
}



template<class E>
void quicksort(E* l, E* r)
{ 
  if (r <= l) return;

//cout << "sz = " << r-l+1 << endl;

  E s = *l;

  E* i = l;
  E* j = r+1;

/*
  float t1 = used_time();
  float t2 = real_time();
*/

  if (thread_num == 1 || r-l+1 < limit)
  { for(;;)        
    { while (i < r && *++i < s);
      while (j > l && *--j > s);
      if (i<j) swap(*i,*j);
      else break;
     }             
   }
  else
  {
    //qsort_arg<E>* Arg = new qsort_arg<E>[thread_num];
    //thread_t* thr = new thread_t[thread_num];

    int d = (r-l)/thread_num;
    E* k = l+1; 

    int i=0; 
    while(i<thread_num-1) 
    { Arg[i].left = k;
      Arg[i].right = k+d-1;
      Arg[i].pivot = s;
      k += d;
      i++;
    }

    Arg[i].left = k;
    Arg[i].right = r;
    Arg[i].pivot = s;

    for(int i=0; i<thread_num; i++) 
         thr[i] = thread_create(partition_thread,Arg+i);

    for(int i=0; i<thread_num; i++) thread_join(thr[i]);

    int l = Arg[1].left - Arg[0].right;
    j = Arg[0].left + l;

    int* p = Arg[0].left+1;
    int* q = Arg[1].left;
    while (k-- > 0) swap(*p++,*q--);


    //delete[] Arg;
    //delete[] thr;
  }

  assert(*j <= s);

  for(i=l; i<=j; i++) assert(*i <= *l);
  for(i=r; i>j; i--) assert(*i >= *l);

  swap(*l,*j);

/*
  cout << string("partition: %.2f (%.2f) sec",
                     used_time(t1), real_time(t2)) <<endl;
*/

  quicksort(l,j-1);
  quicksort(j+1,r);
}





int main()
{
   int n = read_int("size  = ");
   thread_num = read_int("threads = ");
   limit = read_int("limit = ");

   assert(thread_num <= MAX_THREADS);

   int* A = new int[n];
   array<int> B(n);

   for(int i=0; i<n; i++) A[i] = B[i] = rand_int(0,100000000);
   //for(int i=0; i<n; i++) A[i] = B[i] = i;

   float t1 = used_time();
   float t2 = real_time();
   cout << "thread sort: " << endl;
   quicksort(A,A+n-1);
   cout << string("total: %.2f (%.2f) sec",used_time(t1),real_time(t2)) <<endl;
   cout << endl;

   int err = 0;
   for(int i=1; i<n; i++) 
      if (A[i-1] > A[i]) err++;
   cout << "err = " << err << endl;
   if (err > 0)  error_handler(1,"error in A.sort()");



   t1 = used_time();
   t2 = real_time();
   cout << "serial sort: " << flush;
   B.sort(0,n-1);
   cout << string("%.2f (%.2f) sec",used_time(t1),real_time(t2)) <<endl;

   delete[] A;

   //thread_exit(NULL);
   return 0;
}
