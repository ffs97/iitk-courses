#include <LEDA/geo/geo_alg.h>
#include <LEDA/system/multi_thread.h>

using namespace leda;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;


//int* A = 0;
atomic_counter* A = 0;
int a_sz = 0;
int steps = 0;

void* thread_func(void*)
{ 
  random_source ran;
  for(int i=0; i< steps; i++) 
  { int k = ran(0,a_sz-1);
    int l = ran(0,a_sz-1);
    //int k = rand() % a_sz;
    //int l = rand() % a_sz;
    A[k]++;
    A[l]--;
   }

  return NULL;
}


int main(int argc, char** argv)
{
  a_sz  = read_int("# of counters = ");
  steps = read_int("# of steps    = ");
  int t = read_int("# of threads  = ");

  A = new atomic_counter[a_sz];
/*
  A = new int[a_sz];
  for(int i=0; i<a_sz; i++) A[i] = 0;
*/

  float T = used_time();

  thread_t* thr = new thread_t[t];

  for(int j=0; j < 2; j++)
  { for(int i=0; i<t; i++) thr[i] = thread_create(thread_func,(void*)i);
    for(int i=0; i<t; i++) thread_join(thr[i]);
   }

  int sum = 0;

  for(int i=0; i<a_sz; i++)
  { int x = A[i];
    cout << string("%3d ",x);
    sum += x;
   }
  cout << endl;

  cout << "sum = " << sum << endl;

  cout << endl;
  cout << string("total cpu time: %.2f sec", used_time(T)) << endl;
  cout << endl;

  return 0;
}
