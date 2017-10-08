#include <LEDA/geo/geo_alg.h>

#if defined(__win32__)

#include <windows.h>

STDAPI VarCmp(tagVARIANT*, tagVARIANT*, unsigned long, unsigned long) {
    return 0;
}


typedef void* thread_t;

thread_t thread_create(void* (*func)(void*), void* arg)
{ DWORD thread_id;
  return CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)func,arg,0,&thread_id);
}

inline void  thread_join(const thread_t& thr) 
{ WaitForSingleObject(thr,INFINITE); }

#else

#include <pthread.h>

typedef pthread_t thread_t;

thread_t thread_create(void* (*func)(void*), void* arg)
{ thread_t thr;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&(thr),&attr,(func),(arg));
  pthread_attr_destroy(&attr);
  return thr;
}

void  thread_join(const thread_t& thr) 
{ pthread_join(thr,NULL); }


#endif


using namespace leda;
using std::cout;
using std::endl;

static leda_mutex io_mutex; // used for io
static int seg_num = 0; // number of segments
static int thr_num = 0; // number of threads


void* thread_func(void* p)
{ 
  long id = long(p);

  for(;;)
  { list<rat_segment> seglist;

    for (int i = 0; i < seg_num; i++)
    { leda::rat_point p, q;
      leda::random_point_in_unit_square(p);
      leda::random_point_in_unit_square(q);
      if (p != q) seglist.push_back(leda::rat_segment(p, q));
     }

    float T = used_time();
    GRAPH<rat_point,rat_segment> G;
    SWEEP_SEGMENTS(seglist,G,true);
    io_mutex.lock();
    cout << string("thread %2d: |V|= %4d  |E| = %d   %.2f sec", id,
                       G.number_of_nodes(),G.number_of_edges(),used_time(T));
    cout << "  memory: " << std_memory.used_memory() << endl;
    io_mutex.unlock();
   }

  return NULL;
}



//int __stdcall WinMain(HINSTANCE,HINSTANCE,LPSTR,int)

int main()
{
  seg_num = read_int("#segments = ");
  thr_num = read_int("#threads = ");

  thread_t* thr = new thread_t[thr_num];

  for(int i=0; i<thr_num; i++) thr[i] = thread_create(thread_func,(void*)i);
  for(int i=0; i<thr_num; i++) thread_join(thr[i]);

  return 0;
}
