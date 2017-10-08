#include <LEDA/geo/geo_alg.h>
#include <LEDA/system/multi_thread.h>

using namespace leda;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;

static leda_lock io_mutex; // used for io
static int seg_num = 0; // number of segments
static int thr_num = 0; // number of threads

static list<rat_segment> seglist;


/*
#if defined(__SUNPRO_CC)
extern "C" 
#endif
*/

void* thread_func(void* p)
{ 
  long id = long(p);

/*
  io_mutex.lock();
  cout << string("thread %2d: start",id) << endl;
  io_mutex.unlock();
*/


  float T = used_time();

  GRAPH<rat_point,rat_segment> G;
  SWEEP_SEGMENTS(seglist,G,true);

  io_mutex.lock();
  cout << string("thread %2d: |V|= %4d  |E| = %d   %.2f sec", id,
                       G.number_of_nodes(),G.number_of_edges(),used_time(T));
  cout <<endl;
  rat_point::print_statistics();
  io_mutex.unlock();

  return NULL;
}



int main(int argc, char** argv)
{
  if (argc < 2)
  { cerr << "usage: sweep #threads [m]  (reads segments from cin)" << endl;
    return 1;
   }

  bool print_mem = (argc >=3 && string(argv[2]) == "m");

  thr_num = atoi(argv[1]);

  seglist.read(cin);
  seg_num = seglist.length();

  //rand_int.set_seed(17*seg_num);

  cout << endl;
  cout << "threads:  " << thr_num << endl;
  cout << "segments: " << seg_num << endl;
  cout << endl;

  float T = used_time();


  thread_t* thr = new thread_t[thr_num];

  for(int j=0; j < 4; j++)
  { for(int i=0; i<thr_num; i++) thr[i] = thread_create(thread_func,(void*)i);
    for(int i=0; i<thr_num; i++) thread_join(thr[i]);
   }

  if (print_mem) std_memory.print_statistics();

  cout << endl;
  cout << string("total cpu time: %.2f sec", used_time(T)) << endl;
  cout << endl;

  return 0;
}
