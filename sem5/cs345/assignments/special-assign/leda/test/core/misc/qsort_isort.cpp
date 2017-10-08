#include <LEDA/system/basic.h>
#include <LEDA/core/array.h>
#include <LEDA/core/basic_alg.h>
#include <LEDA/core/quicksort.h>
#include <assert.h>

using namespace leda;

#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::endl;
using std::flush;
#endif



int main () 
{
  int N = read_int("N = "); 


  array<int> A(N);
  array<int> B(N);

  for (int i = 0; i < N; i++) A[i] = B[i] = rand_int(1,1000000);

  leda_smaller<int> smaller;

  float T = used_time();

  cout << "quicksort       " << flush;
  quicksort(B.first_item(),B.last_item(),smaller);
  cout << string("%5.3f sec",used_time(T)) << endl;

/*
  for (int i = 0; i < N; i++) B[i] = A[i];
  T = used_time();
  cout << "mergesort       " << flush;
  mergesort(B.first_item(),B.last_item(),smaller);
  cout << string("%5.3f sec",used_time(T)) << endl;
  cout << endl;
*/

  for (int i = 0; i < N; i++) B[i] = A[i];
  T = used_time();
  cout << "int_sort        " << flush;
  INTSORT(B.first_item(),N);
  cout << string("%5.3f sec",used_time(T)) << endl;
  cout << endl;

  for(int d=1; d<256;d*=2)
  { 
    for (int i = 0; i < N; i++) B[i] = A[i];

    cout << string("quicksort d = %3d ",d) << flush;

    float t1 = used_time();
    quicksort_d(B.first_item(),B.last_item(),smaller,d);
    float t2 = used_time();
    insertion_sort(B.first_item(),B.last_item(),smaller);
    float t3 = used_time();
    cout << string("%5.3f + %5.3f  =  %5.2f sec",t2-t1,t3-t2,t3-t1) << flush;

    for (int i = 1; i < N; i++) assert(B[i-1] <= B[i]);

    for (int i = 0; i < N; i++) B[i] = A[i];
    t1 = used_time();
    quicksort_i(B.first_item(),B.last_item(),smaller,d);
    cout << string("    %5.2f sec",used_time(t1)) << endl;
    for (int i = 1; i < N; i++) assert(B[i-1] <= B[i]);
  }

 
  return 0;

}


