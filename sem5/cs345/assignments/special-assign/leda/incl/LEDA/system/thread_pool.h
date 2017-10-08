/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  thread_pool.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <LEDA/core/slist.h>
#include <LEDA/system/thread.h>
#include <LEDA/system/multi_thread.h>
#include <LEDA/system/monitor.h>

LEDA_BEGIN_NAMESPACE

class thread_pool {
public:
  /* The base class for all jobs. */
  class job {
  public:
    virtual void operator()() = 0;
  };
  
  /* For example a job-Functor might look like this:
   *
   * class my_job : public thread_pool::job {
   *   my_sink_t *result_sink;
   *   int       job_id;
   *
   * public:
   *   my_result_t perform_the_job();
   *
   *   void operator()() {
   *     result_sink.set_result(job_id, perform_the_job());
   *   }
   * }
   */
  
  /*
   * A specific job class running a static function; for compatibity with the
   * signature of thread_create().
   */
  class job_static : public job {
    void *(*func)(void *);
    void *arg;
  public:
    job_static(void *(*func_)(void *), void *arg_) : func(func_), arg(arg_) {}
    void operator()() { func(arg); }
  };
  
private:
  leda_mutex      mutex;        /* the synchronization mutex of the pool */
  leda_monitor    busycv;       /* signalled when all workers have terminated */
  leda_monitor    workcv;       /* monitor, all workers wait on for new jobs */
  leda_monitor    waitcv;       /* monitor used for wait(), broadcasted when */
                                /*   the last job has been performed */
  slist<thread_t> active;       /* thread-ids of all workers performing jobs */
  slist<job*>     job_queue;    /* fifo (append & pop) for jobs not taken yet */
  slist<thread_t> workers;      /* all workers (idle & working) */
  bool            waiting;      /* set while wait() is running */
  bool            cancelling;   /* set while destructor is running */
  const unsigned  max_workers;  /* max number of workers running concurrently */
  unsigned        cur_workers;  /* total number of (in)active workers running */
  unsigned        idle_workers; /* number of idle workers (waiting on workcv) */
  bool            use_queue;
  
  void create_worker();
  void notify_waiters();
  void enqueue(job *);
  static void * worker_thread(void *);
  void worker_cleanup();
  void job_cleanup();
  
  // no copy
  thread_pool(const thread_pool &);
  
public:
  /*
   * Create a thread pool.
   *  max_threads: the maximum number of threads performing work requests.
   *  use_queue_ : if set to true, the pool uses a queue to collect the work
   *               requests which have not been assigned to a worker yet,
   *               otherwise the pool uses a stack.
   */
  explicit
  thread_pool(unsigned max_threads, bool use_queue_ = true);
  
  /*
   * Destroys this thread pool. 
   * This method will first stop all idle worker threads, next wait for all
   * active workers to finish and third flush all pending jobs in the queue,
   * prior to releasing all memory associated with this thread pool.
   */
 ~thread_pool();
  
  /*
   * Enqueue a work request to the thread pool job queue.
   * If there are idle worker threads, awaken one to perform the job.
   * Else if the maximum number of workers has not been reached, create a new
   * worker thread to perform the job.
   * Else just return after adding the job to the queue; an existing worker
   * thread will perform the job when it finishes the job it is currently
   * performing.
   *
   * The job is performed as if a new thread were created for it.
   *
   * The following restrictions apply to a job:
   *
   *  - func() must not cause the thread to terminate (neither abnormally nor
   *    through invokation of ExitThread() or pthread_exit()). This restriction
   *    is necessary due to cleanup procedures internal to the thread pool.
   *    Failing to ensure this behaviour will induce memory leaks and leave
   *    the thread pool in an inconsistent state, i.e. it's destructor may
   *    never terminate.
   *    Furthermore, the number of running workers will be reduced by one even
   *    if additional jobs are pending in the queue, until the next call to
   *    queue() after the thread has terminated.
   *
   *  - func() must not permanently change the system's internal thread
   *    attributes, as procedures to detect and reverse these changes are
   *    not implemented.
   */
  void enqueue(void *(*func)(void *), void *arg) {
    enqueue(job_static(func, arg));
  }
  
  /*
   * Same semantics as enqueue(void *(*func)(void *), void *).
   *
   * JobFunct must be derived from thread_pool::job. func is copied to new
   * allocated memory and may therefore be a stack reference. The copy is
   * deleted when the job has finished.
   */
  template <class JobFunct>
  void enqueue(const JobFunct &func) {
    enqueue(static_cast<job *>(new JobFunct(func)));
  }
  
  /*
   * Wait for all queued jobs to complete.
   */
  void wait();
  
  int active_jobs() { return active.size(); }
};

LEDA_END_NAMESPACE

#endif
