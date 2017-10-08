#include <LEDA/core/array.h>
#include <LEDA/core/queue.h>
#include <LEDA/core/stack.h>
#include <LEDA/core/list.h>
#include <LEDA/system/thread.h>
#include <LEDA/core/quicksort.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

using namespace LEDA;
using std::cout;
using std::cin;
using std::endl;
using std::flush;

// README  <<<<<<<<<--------------------
// line 24 - 402: Serial Solver, Parallel Solver, Base class for parallel jobs (get/set threads)
// line 404 - 648: Parallel Partition Object for Random Access Iterators
// line 650 - 755: Quicksort Job for iterators with parallel partition
// line 757 - 775: Specialized Serial Solver for Quicksort jobs with iterators (for efficiency reasons)
// line 803 - : main
#ifndef DC_FRAME
#define DC_FRAME

// SOLVER TYPE
template <class Job>
class SOLVER {
public:
virtual ~SOLVER(){};
virtual void solve(Job&) {};
};

// HELP FUNCTIONS FOR PARALLEL JOBS
class MC_JOB
{
private:
int num_threads;

public:
  MC_JOB(){num_threads = 1;}; 
  virtual ~MC_JOB(){};

  void set_threads(int i) {num_threads = i;} ;
  int get_threads() {return num_threads;};

  virtual int 	    size() = 0;
  virtual   bool      is_leaf() = 0;
  virtual   void      handle_leaf() = 0;

  virtual   void divide(MC_JOB& , MC_JOB& )
  { }
  virtual   void      merge(MC_JOB& , MC_JOB& )
  { }
};

// SERIAL RECURSIV PAIRWISE

template <class Job>
class dc_serial_solver_2 : public SOLVER<Job> {

public:
void solve(Job& j) { recurse(j); }

void recurse(Job& j)
{
  if (j.is_leaf()) 
  { j.handle_leaf();
    return;
   }

  Job A,B;
  j.divide(A,B);

  recurse(A);
  recurse(B);

  j.merge(A,B);
}

};

// HELP CLASS
template <class Job>
struct jobNode
{
  jobNode(Job* j, jobNode* parent_): job(j), parent(parent_){unsolved_childs = 0;};
  jobNode(){};
  jobNode(Job& j, jobNode* parent_): job(&j), parent(parent_){unsolved_childs = 0;};
  ~jobNode(){};
  Job* job;
  jobNode* parent;
  list<Job> childs;
  int unsolved_childs; 
};

// PRARALLEL SOLVER PAIRWISE
template <class Job> 
class dc_parallel_solver_2 : SOLVER<Job>
{
protected:
typedef pthread_t thread_t;

struct threadArgs
{
  void* pointer;
  int arg;
  threadArgs(void* pointer_, int arg_) : pointer(pointer_), arg(arg_){};
  threadArgs() {};
};

// HELP CLASS
struct jobNode_2
{
  jobNode_2(Job* j, jobNode_2* parent_): job(j), parent(parent_){unsolved_childs = 0;};
  jobNode_2(){};
  jobNode_2(Job& j, jobNode_2* parent_): job(&j), parent(parent_){unsolved_childs = 0;};
  ~jobNode_2(){};
  Job* job;
  jobNode_2* parent;
  Job A;
  Job B;
  int unsolved_childs; 
};

public:
  dc_parallel_solver_2(int thread_num_, int limit_, SOLVER<Job>& serial_): thread_num(thread_num_), limit(limit_), serial(&serial_), unfinished_leaves(0), max_job_num(1) 
    {
      thread_times.resize(thread_num); 
      pthread_mutex_init(&control_mutex, NULL); 
      pthread_cond_init(&pending, NULL); 
      pthread_cond_init(&master_wait, NULL); 
    };
  virtual ~dc_parallel_solver_2()
    {
      pthread_mutex_destroy(&control_mutex); 
      pthread_cond_destroy (&pending); 
      pthread_cond_destroy (&master_wait); 
    };

  virtual void solve(Job& j)
  { list<Job*> L; L.push(&j); solve(L);}
  
  virtual void solve(list<Job*>& L)
  {
     float t1 = used_time();
     double t2 = real_time();

     cout << "thread work: " << endl;
     Job* j;     
     forall(j,L)
     {unsolved_jobs.push(new jobNode_2(j,NULL)); unfinished_leaves++;}

     //start threads 
     int max_start = unsolved_jobs.size() > thread_num ? thread_num : unsolved_jobs.size();

     active_threads = max_start;
     int thread_counter = 0;
     thread_t* thr = new thread_t[max_start];
     for(int i =0; i< max_start; i++) 
        { 
          int start_threads = (thread_num - thread_counter) / ( max_start - i); 
          thread_counter += start_threads;
          thr[i] = thread_create(threadCaller, new threadArgs(this,start_threads));
        }
     cout << "threads created" << endl;
     //for(int i=0; i<max_start; i++) thread_join(thr[i]);
     //sleep(5.0);
  
     pthread_mutex_lock(&control_mutex);
     while (active_threads > 0) pthread_cond_wait(&master_wait, &control_mutex);
     pthread_mutex_unlock(&control_mutex);

     cout << "Solved Jobs " << max_job_num << endl;
     cout << "finished" << endl;
     used_time_total = used_time(t1);
     real_time_total = real_time(t2);
     //cout << string("total: %.2f (%.2f) sec",used_time(t1),real_time(t2)) <<endl;
     cout << endl;
     delete thr;
   return ;
  }

  array<double> thread_times;
  float used_time_total;
  double real_time_total;

protected:
  
  stack<jobNode_2*> unsolved_jobs;
  int thread_num;
  int limit;
  SOLVER<Job>* serial;
  int unfinished_leaves;
  int max_job_num;
  int active_threads;

  pthread_mutex_t control_mutex;
  pthread_cond_t pending;
  pthread_cond_t master_wait;

  bool check_parent(jobNode_2*& jn)
  {
    bool mergeable = true;
    jobNode_2* h = jn;
    pthread_mutex_lock(&control_mutex);
    if (jn->parent != NULL)
      jn->parent->unsolved_childs--;
    unfinished_leaves--;
    if (jn->parent != NULL && jn->parent->unsolved_childs == 0)
      jn = jn->parent;
    else
      {
        jn = NULL;
        mergeable = false;
      }
    pthread_mutex_unlock(&control_mutex);
    delete h;
    return mergeable;
  }

  // time measurement
  double real_time(double& T)
  { double t = real_time() -  T;
    T += t;
    return t;
  }

  double thread_time(double& T)
  { double t = thread_time() -  T;
    T += t;
    return t;
  }

  double real_time()
  { timeval tv;
    gettimeofday(&tv,NULL);
    return double(tv.tv_sec-1165000000) + double(tv.tv_usec)/1000000;
    return 0;
  }

  double thread_time()
  { struct timespec t;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t);
    return t.tv_sec + t.tv_nsec/1E9;
   return 0;
  }

  // thread control
  void  thread_join(const thread_t& thr)
  { pthread_join(thr,NULL); }

  unsigned thread_self() 
  { return pthread_self(); }

  thread_t thread_create(void* (*func)(void*), void* arg)
  { 
    thread_t thr;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&(thr),&attr,(func),(arg));
    pthread_attr_destroy(&attr);
    return thr;
  }

  static void* threadCaller(void* p)
  {
    threadArgs* a = (threadArgs*)p;
    dc_parallel_solver_2<Job>* t = (dc_parallel_solver_2<Job>*) a->pointer;
    t->threadLoop((void*)a->arg);
    delete a;
    return NULL;
  }

  virtual void* threadLoop(void* p)
  {
    //double t = thread_time();
    int threads = long(p);
    jobNode_2* jn = NULL;
    int size = 0;
    int busy = 0;
    //cout << "here " << unfinished_leaves << endl;
    while (true)
    {
      pthread_mutex_lock(&control_mutex);
      //control_mutex.lock();
      //cout << unsolved_jobs.size() << endl;
      if (unfinished_leaves == 0) 
        {
          pthread_mutex_unlock(&control_mutex); 
          /*return NULL;*/ 
          break;
        }
      if (unsolved_jobs.empty())
        {
          active_threads--; 
          //cout << "actice threads " << active_threads << endl;
          //cout << "threads used: " << threads << " time = " << string("%.3f", thread_time(t)) << " sec; busy " << busy << "; average queue length " << (double)size / (double)busy << endl;
          pthread_mutex_unlock(&control_mutex); return NULL;}
          //{ waiting++; pthread_cond_wait(&pending, &control_mutex);}

      size += unsolved_jobs.size();
      busy++;
      jn = unsolved_jobs.top();
      unsolved_jobs.pop(); 

      pthread_mutex_unlock(&control_mutex);
      //control_mutex.unlock();
      
      while(jn != NULL)
        {
          bool mergeable = false;
          int d = !(jn->job->is_leaf());
          if (d == 1) d += (limit >= jn->job->size());
          //cout << "case " << d << endl;
          //cin.get();
          switch (d)
            {
              case 0: 
                {
                // Solve
                threads = 1;
                jn->job->set_threads(1);
                jn->job->handle_leaf();
                mergeable = check_parent(jn);
                break;
                }
              case 1:
                {
                // Divide
                //double t = thread_time();
                jn->job->set_threads(threads);
                jn->job->divide(jn->A,jn->B);
                pthread_mutex_lock(&control_mutex);
                Job* j1, *j2;
                if (jn->A.size() < jn->B.size())
                  {
                    j1 = &jn->A;
                    j2 = &jn->B;
                  }
                else
                  {
                    j1 = &jn->B;
                    j2 = &jn->A;
                  }
                jobNode_2* jnh = new jobNode_2(j1,jn);
                unsolved_jobs.push(new jobNode_2(j2,jn));
                  
                jn->unsolved_childs = 2;
                unfinished_leaves += 2;
                max_job_num += 2;
                busy++;
                //if (threads == 4) cout << "first job " << thread_time(t) << endl;
                //pthread_cond_broadcast(&pending);
                if (active_threads < thread_num)
                  {
                    thread_create(threadCaller,new threadArgs(this,1));
                    active_threads++;
                    threads = thread_num - active_threads + 1;
                  }
                else
                  threads = 1;
                pthread_mutex_unlock(&control_mutex);
                jn = jnh;
                break;
                }
              case 2:
                {
                // Serial Solve
                threads = 1;
                jn->job->set_threads(1);
                serial->solve(*jn->job);
                mergeable = check_parent(jn);
                break;
                }
	    }
          while(mergeable)
            {
              int free = thread_num - active_threads + 1;
              jn->job->set_threads(free);
              jn->job->merge(jn->A, jn->B);
              mergeable = check_parent(jn);
            }
        }
    }
    //pthread_cond_broadcast(&pending);
    pthread_mutex_lock(&control_mutex); 
    active_threads--;   
    //thread_times[id] = thread_time(t);
    //cout << "actice threads " << active_threads << endl;
    //cout << "threads used: " << threads << " time = " << string("%.3f", thread_time(t)) << " sec; busy " << busy << "; average queue length " << (double)size / (double)busy << endl;
    pthread_cond_broadcast(&master_wait);
    pthread_mutex_unlock(&control_mutex);
    return NULL;
  }
};

#endif

#ifndef PARTITION
#define PARTITION

template <class iterator, class relation>
class partition
{
typedef pthread_t thread_t;
typedef typename iterator::value_type t;

struct funcArgs
{
  funcArgs(){};

  ~funcArgs(){};
};

struct threadArgs
{
  void* pointer;
  funcArgs* arg; 
  threadArgs(void* pointer_, funcArgs* arg_) : pointer(pointer_), arg(arg_){};
};

public:
 partition(relation r_, int thread_num_ = 1, int blocksize = 10000) : r(r_), thread_num(thread_num_), BS(blocksize) {};

  iterator operator()(iterator a, iterator b)
  {
    left = a; lb = a;
    right = (b - 1); rb = (b - 1);

    //start threads 
    thread_t thr[thread_num];
    funcArgs* args[thread_num];  

    //create_args(a, b, thread_num, args);
    for(int i =0; i<thread_num; i++) 
      args[i] = new funcArgs();

    for(int i =0; i<thread_num; i++) 
      thr[i] = thread_create(t_func,new threadArgs(this,args[i]));
    //cout << "threads created" << endl;
    for(int i=0; i<thread_num; i++) thread_join(thr[i]);

    part_seq();

    for(int i =0; i<thread_num; i++) delete args[i];
    return lb;
  }

  void set_relation(relation r_)
  { r = r_;}
  
private:

  relation r;
  int thread_num;
  iterator left, right, lb, rb;
  leda_lock mutex;//ml, mr;
  int BS;

class rest
  {
  public:
  rest() {};
  rest(iterator l_, iterator r_) : l(l_), r(r_) {}
    iterator l;
    iterator r;
  };

class cmp_rest //: public leda_cmp_base<rest>
  {
    int operator()(rest& r1, rest& r2)
    { if (r1.l < r2.l) return -1; else return 1;};
  };

  list<rest> l_rest;
  list<rest> r_rest;

  void part_seq() 
  {
    //cmp_rest cmp;
    /*l_rest.sort(cmp);
    r_rest.sort(cmp);*/
    //quicksort(l_rest.begin(),l_rest.end(),cmp);

    while(!r_rest.empty())
      {
        iterator l = r_rest.head().l;
        iterator r = r_rest.head().r;
        r_rest.pop_front();
        if (l == rb + 1) {rb = r; continue;}
        rb++;
        while(l <= r)
          {swap(*rb, *l); rb++; l++;}
        rb--;
      }

    while(!l_rest.empty())
      {
        iterator l = l_rest.tail().l;
        iterator r = l_rest.tail().r;
        l_rest.pop_back();
        if (r == lb - 1) {lb = l; continue;}
        lb--;
        while(l <= r)
          { swap(*lb, *r); lb--; r--;}
        lb++;
      }

    lb--; rb++;
    for(;;)
    { while (r(*(++lb)));
      while (!r(*(--rb)));
      if (lb >= rb) break;
      swap(*lb,*rb);
    }
  }

  void part_parallel() 
  {
    iterator loc_l, loc_lb, loc_r, loc_rb;
    int state = 1;
    bool init = false;
    while(true)
      {
        mutex.lock();
        if (rb - lb + 1 < BS) 
          {mutex.unlock(); break;}

        if (state == 1 && rb - lb + 1 < 2 * BS)
          {mutex.unlock(); break;}
        init = true;

        if(state <= 1)
	  {
	    loc_l = lb;
	    lb = lb + BS;
	    loc_lb = lb - 1;
	  }

        if (state >= 1)
          {
            loc_r = rb;
            rb = rb - BS;
            loc_rb = rb + 1;
          }
        mutex.unlock();

        // sort blocks
        --loc_l; loc_r++;
	for(;;)
	  { while (++loc_l <= loc_lb && r(*(loc_l)));
	    while (--loc_r >= loc_rb && !r(*(loc_r)));

	    if (loc_l > loc_lb) 
              {
                if(loc_r < loc_rb)
		  { state = 1; break; }
                else
                  { state = 0; break; }
              }
	    else if (loc_r < loc_rb)
              { state = 2; break;}           

	    swap(*loc_l,*loc_r);
	  }
      }
    mutex.lock();
    if (init && state == 2)
      //l_rest.push(rest(loc_l,loc_lb));
      {     
        if(l_rest.empty()|| loc_l > l_rest.tail().l) 
          l_rest.push_back(rest(loc_l,loc_lb));
        else
	  {
            list_item it = l_rest.first();
            for(; it != nil; it = l_rest.succ(it))
              if(loc_l < l_rest[it].l) break;
            l_rest.insert(rest(loc_l,loc_lb),it,leda::before);
	  }
      }
    if (init && state == 0)
      //r_rest.push(rest(loc_rb,loc_r));
      {   
        if(r_rest.empty()|| loc_rb > r_rest.tail().l) 
          r_rest.push_back(rest(loc_rb,loc_r));
        else
          {
            list_item it = r_rest.first();
            for(; it != nil; it = r_rest.succ(it))
              if(loc_rb < r_rest[it].l) break;
            r_rest.insert(rest(loc_rb,loc_r),it,leda::before);
	  }
      }
    mutex.unlock();
  }

  static void* t_func(void* p)
  {
    threadArgs* tA = (threadArgs*)p;

    ((partition*)tA->pointer)->part_parallel();

    delete tA;
    return NULL;
  }

  /*void create_args(iterator s, iterator e, int thread_num, funcArgs* args[])
  {
    int size = 0;
    for (iterator i = s; i != e; i ++) size++;
  
    int i_size = size / thread_num;

    iterator it = s;
    for(int i = 0; i<thread_num - 1; i++)
      {
      for (int j = 0; j < i_size; j++) it++;
      list<t>* n = new list<t>;
      L->split(it,*n,*L,leda::before); 
      args[i] = new funcArgs(n, r);    
      }
    list<t>* n = new list<t>; n->conc(*L);
    args[thread_num - 1] = new funcArgs(n,r); 
  }*/

  void  thread_join(const thread_t& thr)
  { pthread_join(thr,NULL); }

  unsigned thread_self() { return pthread_self(); }

  thread_t thread_create(void* (*func)(void*), void* arg)
  { 
    thread_t thr;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&(thr),&attr,(func),(arg));
    pthread_attr_destroy(&attr);
    return thr;
  }
};

#endif

//---------------------------------------------------------------------------

#ifndef DC_QSORT
#define DC_QSORT

template <class iterator>
class qsjob_it : public MC_JOB
{
  typedef qsjob_it<iterator> job;

public:

  qsjob_it(){};
  qsjob_it(iterator l, iterator r) : left(l), right(r) {};

  iterator left;
  iterator right;
  typedef typename iterator::value_type E;

  int size() { return right - left + 1; }

  bool is_leaf() { return size() <= 3; }

  void handle_leaf()
  {
    iterator l = left;
    iterator r = right;

    if (size() <= 1) return;

    if (size() >= 2 && *r < *l) swap(*l,*r); 

    if (size() == 3)
    { if (*r < *(l+1))
        swap(*(l+1),*r); 
      else
      if (*(l+1) < *l)
        swap(*(l+1),*l); 
     }
  }


  class relation
  {
      leda_smaller<E> r;
      E pivot;

    public:
      relation(E p) : pivot(p) {}
      bool operator()(E v) { return r(v,pivot); }
  };


  void divide(job& j1, job& j2)
  {
    iterator l = left;
    iterator r = right;

    if (*(r) < *(l)) swap(*l,*r);
    iterator k = l + (r-l)/2;
    if (*k < *l) 
       swap(*k,*l);
    else
       if (*r < *k) swap(*k,*r);
    iterator i = l+1;
    iterator j = r;
    swap(*k,*i);
    k = i;
    const E& s = *i;

    if (get_threads() > 1 && size() > 10000)
      {
        relation rel(s);
        partition<iterator,relation> part(rel,get_threads());
        j = part(l + 2, r) - 1;
      }
    else
      {
        for(;;)
        { while (*(++i) < s);
          while (s < *(--j));
          if (i >= j) break;
          swap(*i,*j);
        }
      }
    swap(*k,*j);

    j1.left = l;
    j1.right = j; 
    //j1 = job(l,j);

    j2.left = j + 1;
    j2.right = r;
    //j2 = job(j+1,r);
 
    return;
  }

  void  merge(job& , job& ) { }
};

#endif



#ifndef QS_SOLVER
#define QS_SOLVER

template <class iterator>
class qs_solver : public SOLVER<qsjob_it<iterator> > 
{
public:
qs_solver() {};
typedef typename iterator::value_type E;

void solve(qsjob_it<iterator>& j) 
{ leda_smaller<E> s;
  quicksort<E, leda_smaller<E> >(j.left,j.right,s);
};

};
#endif





double real_time()
{ timeval tv;
  gettimeofday(&tv,NULL);
  return double(tv.tv_sec-1165000000) + double(tv.tv_usec)/1000000;
  return 0;
}

double thread_time()
{ struct timespec t;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t);
  return t.tv_sec + t.tv_nsec/1E9;
 return 0;
}

double real_time(double& T)
{ double t = real_time() -  T;
  T += t;
  return t;
}

double thread_time(double& t)
{ double tt = thread_time()-t;
  t += tt;
  return tt;
}

int main(int argc, const char** argv)
{
   int thread_num = (argc > 1) ? atoi(argv[1]) : read_int("threads  = ");
   int n = (argc > 2) ? atoi(argv[2]) : read_int("size  = ");
   int limit = (argc > 3) ? atoi(argv[3]) : read_int("limit  = ");

   double qs_para_real = 0;
   double qs_para_used = 0;

   array<int> V(n);
   int* W = new int[n];

   cout << endl;
   for(int i=0; i<n; i++) V[i] = W[i] = rand_int();

 // PARALLEL SOLVER  PAIRWISE
   {
   //V.sort(); 
   cout << "parallel solver pairwise quick sort: " << endl;
   qsjob_it<array<int>::iterator> qdca(V.begin(), V.end() - 1);
   //dc_serial_solver_2<qsjob_it<array<int>::iterator> > help_solvera;
   qs_solver<array<int>::iterator> help_solvera;
   dc_parallel_solver_2<qsjob_it<array<int>::iterator> > solvera(thread_num, limit, help_solvera);

   solvera.solve(qdca);

 //debugging

   int erra = 0;
   for(int i=1; i<n; i++) 
      if (V[i-1] > V[i]) erra++;

   cout << "err = " << erra << endl << endl; 

   if (erra > 0)  { /*cout << V << endl; */ error_handler(1,"error in V"); }
   

//time measurement
//for(int i=0; i< solvera.thread_times.size(); i++) 
//cout << "thread: id = " << i << " time = " << solvera.thread_times[i] << endl;

   qs_para_real += solvera.real_time_total;
   qs_para_used += solvera.used_time_total;

   cout << endl;
   cout << string("real: %.2f sec   used: %.2f sec",solvera.real_time_total,
                                                    solvera.used_time_total);
   cout <<endl;

   for(int i=0; i<n; i++) V[i] = W[i];
   cout << endl << endl;
   }


   cout << "Input size: " << n << "; Threads: " << thread_num << endl;
   cout << string("parallel solver pairwise quick sort time real,(used) %.2f (%.2f) sec",qs_para_real, qs_para_used) <<endl;

   return 0;
}
