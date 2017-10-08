#include <LEDA/geo/geo_alg.h>
#include <LEDA/system/multi_thread.h>

using namespace leda;
using std::cout;
using std::endl;

static leda_mutex io_mutex; // used for io
static int seg_num = 0; // number of segments
static int thr_num = 0; // number of threads

static list<rat_segment> seglist;

void* thread_func(void* p)
{ 
  long id = long(p);

  float T = used_time();

  GRAPH<rat_point,rat_segment> G;
  SWEEP_SEGMENTS(seglist,G,true);

  io_mutex.lock();
  cout << string("thread %2d: |V|= %4d  |E| = %d   %.2f sec", id,
                       G.number_of_nodes(),G.number_of_edges(),used_time(T));
  cout <<endl;
  io_mutex.unlock();

  return NULL;
}


int main()
{
  seg_num = read_int("#segments = ");
  thr_num = read_int("#threads = ");

  thread_t* thr = new thread_t[thr_num];

  int count = 0;
for(;;)
{ 
  seglist.clear(); 
  for (int i = 0; i < seg_num; i++)
  { leda::rat_point p, q;
    leda::random_point_in_unit_square(p);
    leda::random_point_in_unit_square(q);
    if (p != q) seglist.push_back(leda::rat_segment(p, q));
  }

  float T = used_time();
  cout << ++count << endl;
  for(int i=0; i<thr_num; i++) thr[i] = thread_create(thread_func,(void*)i);
  for(int i=0; i<thr_num; i++) thread_join(thr[i]);
  cout << string("cpu time: %.2f sec", used_time(T)) << endl;
  cout << endl;
/*
  std_memory.clear();
  std_memory.print_statistics();
*/
}

  return 0;
}
