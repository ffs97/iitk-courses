#include <LEDA/core/array.h>
#include <LEDA/core/stack.h>
#include <LEDA/core/queue.h>
#include <LEDA/system/multi_thread.h>
#include <assert.h>

const int MAX_THREADS = 256;

using namespace leda;

using std::cout;
using std::cin;
using std::endl;
using std::flush;



template<class E>
struct qsort_arg {
  E* left;
  E* right;
  qsort_arg(E* l, E* r) : left(l), right(r) {}
};




static leda_lock mutex1;
static leda_lock mutex2;



static queue<qsort_arg<int>*> jobs;
static int limit = 0;
static int thread_num = 0;
static int status[MAX_THREADS];




template<class E> 
void new_job(E* l, E* r, bool lock)
{ qsort_arg<E>* p = new qsort_arg<E>(l,r);
  if (lock) mutex1.lock();
  //jobs.append(p);
  jobs.push(p);
  if (lock) mutex1.unlock();
 }
  

template<class E>
void quicksort(E* l, E* r, bool lock)
{ 
  if (r <= l) return;

  if (*r < *l) swap(*l,*r);

  if (r == l+1) return;

  E* k = l + (r-l)/2;
  if (*k < *l) 
     swap(*l,*k);
  else
     if (*r < *k) swap(*k,*r);

  if (r == l+2) return;

  E* i = l+1;
  E* j = r;
  swap(*i,*k);
  k = i;
  const E& s = *i;

  for(;;)        
  { while (*++i < s);
    while (s < *--j);
    if (i<j) swap(*i,*j);
    else break;
   }             
  swap(*k,*j);

  if (j < l+limit) 
     quicksort(l,j-1,lock);
  else
     new_job(l,j-1,lock);

  if (r < j+limit) 
     quicksort(j+1,r,lock);
  else
     new_job(j+1,r,lock);
}



void* qsort_thread(void* p)
{ 
  float t = elapsed_time();

  int id = long(p);

  while (true)
  { qsort_arg<int>* arg = 0; 

    status[id] = 0; // waiting
    mutex1.lock();
    status[id] = 1; // busy

    if (!jobs.empty()) arg = jobs.pop();
    bool more_jobs =  !jobs.empty(); 
    if (more_jobs) mutex1.unlock();

    if (arg == 0)
    { int s = 0;
      for(int i=0; i<thread_num;  i++) 
      { if (i == id) continue;
        s += status[i];
       }
      mutex1.unlock();
      if (s == 0) break;
      continue;
     }

    quicksort(arg->left,arg->right, more_jobs);
    delete arg;

    if (!more_jobs) mutex1.unlock();
   }

  mutex2.lock();
  cout << string("thread: id = %2d  time = %.2f sec", 
                                           id, elapsed_time(t)) << endl;
  mutex2.unlock();

  status[id] = 0;

  //thread_exit(NULL);
  return NULL;
}



template<class E>
void qsort(E* A, int n)
{ 
  qsort_arg<E>* p = new qsort_arg<E>(A,A+n-1);
  //jobs.append(p);
  jobs.push(p);

  thread_t* thr = new thread_t[thread_num];

  for(int i=0; i<thread_num; i++)  status[i] = 0;

  for(int i=0; i<thread_num; i++) 
      thr[i] = thread_create(qsort_thread,(void*)i);

  for(int i=0; i<thread_num; i++) thread_join(thr[i]);

}




int main()
{
   int n = read_int("size  = ");
   thread_num = read_int("threads = ");
   limit = read_int("limit = ");

   assert(limit > 3);
   assert(thread_num <= MAX_THREADS);

   int* A = new int[n];
   array<int> B(n);

   for(int i=0; i<n; i++) A[i] = B[i] = rand_int(0,100000000);

   float t1 = used_time();
   float t2 = real_time();
   cout << "thread sort: " << endl;
   qsort(A,n);
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
