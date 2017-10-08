#include <LEDA/graph/graph_alg.h>
#include <LEDA/system/multi_thread.h>

using namespace leda;
using std::cout;
using std::endl;

static leda_mutex io_mutex; // used for io
static int thr_num = 0;     // number of threads


static graph G;
static edge_array<int> cost;


void* thread_func(void* p)
{ 
  long id = long(p);

  float T = used_time();

  node_array<int> dist(G);

  node s = G.first_node();
  node t = G.last_node();

  DIJKSTRA(G,s,cost,dist);

  io_mutex.lock();
  cout << string("thread %2d: dist = %d   %.2f sec", id, dist[t],used_time(T));
  cout <<endl;
  io_mutex.unlock();

  return NULL;
}


int main()
{
  int n = read_int("n = ");
  int m = read_int("m = ");


  thr_num = read_int("#threads = ");

  thread_t* thr = new thread_t[thr_num];

  int count = 0;

  for(;;)
  { 
    random_graph(G,n,m);
    cost.init(G);
  
    edge e;
    forall_edges(e,G) cost[e] = rand_int(0,1000);
  
    float T = used_time();
    cout << ++count << endl;
    for(int i=0; i<thr_num; i++) thr[i] = thread_create(thread_func,(void*)i);
    for(int i=0; i<thr_num; i++) thread_join(thr[i]);
    cout << string("cpu time: %.2f sec", used_time(T)) << endl;
    cout << endl;
  }
  
  return 0;
}
