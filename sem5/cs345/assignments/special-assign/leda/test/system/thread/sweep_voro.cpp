#include <LEDA/geo/geo_alg.h>
#include <LEDA/system/multi_thread.h>


using namespace leda;
using std::cout;
using std::endl;

static leda_mutex io_mutex; // used for io
static int seg_num = 0; // number of segments
static int thr_num = 0; // number of threads

void* thread_func(void* p)
{ 
  long id = long(p);

  for(int i=0; i<4; i++)
  { list<rat_segment> seglist;

    for (int i = 0; i < seg_num; i++)
    { leda::rat_point p, q;
      leda::random_point_in_unit_square(p);
      leda::random_point_in_unit_square(q);
      if (p != q) seglist.push_back(leda::rat_segment(p, q));
     }

    float T = used_time();
    float t = elapsed_time();

    GRAPH<rat_point,rat_segment> G;
    SWEEP_SEGMENTS(seglist,G,true);
/*
    io_mutex.lock();
    cout << string("%2d: |V|= %d  |E| = %d  cpu: %.2f  real: %.2f", id,
                       G.number_of_nodes(), G.number_of_edges(), 
                       used_time(T), elapsed_time(t));
    cout << "  memory: " << std_memory.used_memory() << endl;
    io_mutex.unlock();
*/

    list<rat_point> L;
    node v;
    forall_nodes(v,G) L.append(G[v]);
    GRAPH<rat_circle,rat_point> V;
    VORONOI(L,V);

    io_mutex.lock();
    cout << string("%2d: |V|= %d  |E| = %d  cpu: %.2f  real: %.2f", id,
                       V.number_of_nodes(), V.number_of_edges(), 
                       used_time(T), elapsed_time(t));
    cout << "  memory: " << std_memory.used_memory() << endl;
    //rat_point::print_statistics();
    io_mutex.unlock();

   }

  return NULL;
}



int main()
{
  seg_num = read_int("#segments = ");
  thr_num = read_int("#threads = ");

  thread_t* thr = new thread_t[thr_num];

  for(int i=0; i<thr_num; i++) thr[i] = thread_create(thread_func,(void*)i);
  for(int i=0; i<thr_num; i++) thread_join(thr[i]);

  return 0;
}
