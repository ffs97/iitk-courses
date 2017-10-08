#include <LEDA/geo/plane_alg.h>
#include <LEDA/geo/rat_point.h>
#include <LEDA/core/list.h>
#include <LEDA/core/array.h>
#include <LEDA/core/queue.h>
#include <LEDA/system/multi_thread.h>
#include <assert.h>


const int MAX_THREADS = 256;

using namespace leda;

using std::cout;
using std::cin;
using std::endl;
using std::flush;




struct qhull_arg {
  rat_point* left;
  rat_point* right;
  rat_point  right_p;
  qhull_arg(rat_point* l, rat_point* r,rat_point p) : 
                               left(l), right(r),right_p(p) {}
};


class rp_smaller {
public:
bool operator()(const rat_point& p, const rat_point& q) const
{ return rat_point::cmp_xy(p,q) < 0; }
};

class rp_smaller_F {
public:
 bool operator()(const rat_point& a, const rat_point& b) const
 { double d = a.XD()*b.WD() - b.XD()*a.WD();
   if (d == 0) d = a.YD()*b.WD() - b.YD()*a.WD();
   return d < 0;
 }
};

class rp_greater: public rp_smaller {
public:
bool operator()(const rat_point& p, const rat_point& q) const
{ return rp_smaller::operator()(q,p); }
};

class rp_greater_F: public rp_smaller_F {
public:
bool operator()(const rat_point& p, const rat_point& q) const
{ return rp_smaller_F::operator()(q,p); }
};



static leda_lock mutex1;
static leda_lock mutex2;



static queue<qhull_arg*> jobs;
static int limit = 0;
static int thread_num = 0;
static int status[MAX_THREADS];




void new_job(rat_point* l, rat_point* r, rat_point rp, bool lock)
{ qhull_arg* p = new qhull_arg(l,r,rp);
  if (lock) mutex1.lock();
  //jobs.append(p);
  jobs.push(p);
//cout << "jobs = " << jobs.size() << endl;
  if (lock) mutex1.unlock();
 }
  

/*
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

*/


rat_point NullPoint;


void quick_hull(rat_point* l, rat_point* r, const rat_point& b, bool lock)
{

//cout << "size = " << r-l+1 <<endl;

  if (r <= l+2) return;

  rat_point a = *l;

  if (r <= l+256)  // graham scan
  {
    if (rat_point::cmp_xy(a,b) < 0)
       quicksort(l+1,r-1,rp_smaller(),rp_smaller_F());
    else
       quicksort(l+1,r-1,rp_greater(),rp_greater_F());

    rat_point tmp = *r;
    *r = b;

    rat_point* j = l+1;
    for(rat_point* p = l+2; p<=r; p++)
    { rat_point pp = *p;
      while(j > l && orientation(*(j-1),*j,pp) >= 0) j--;
      *++j = pp;
     }

    while (j < r) *j++ = NullPoint;
    *r = tmp;
    return;
   }


//float T = used_time();

  rat_vector vec = b-a;
  rat_point* k = l+1;
  rat_point c = *k;
  rat_point d = c+vec;

  for(rat_point* p=l+2; p<r; p++)
  {  int orient = orientation(c,d,*p);
     if (orient > 0 || (orient == 0 && compare(c,*p) > 0)) 
     { k = p; 
       c = *k; 
       d = c+vec; 
      } 
   }

//if (orientation(a,b,c) == 0) return; 


  *k = *(l+1);

  rat_point* i = l+1;
  rat_point* j = r;
  rat_point* p = i+1;

  while (p < j)
  { if (orientation(a,c,*p) > 0)
      swap(*++i,*p++);
    else
      if (orientation(c,b,*p) > 0)
         swap(*--j,*p);
      else
         *p++ = NullPoint;
   }

  *(l+1) = *i;
  *i = NullPoint;
  *--j = c;

//cout << string("partition:  %.2f sec", used_time(T)) << endl;

/*
  quick_hull(l,i,c,delta);
  quick_hull(j,r,b,delta);
*/

  if (i < l+limit) 
     quick_hull(l,i,c,lock);
  else
     new_job(l,i,c,lock);

  if (r < j+limit) 
     quick_hull(j,r,b,lock);
  else
     new_job(j,r,b,lock);
}


void* qhull_thread(void* p)
{ 
  float t = elapsed_time();

  int id = long(p);

  while (true)
  { qhull_arg* arg = 0; 

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

    quick_hull(arg->left,arg->right,arg->right_p,more_jobs);
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




list<rat_point> quick_hull(const list<rat_point>& L)
{ int n = L.length();

float T = used_time();

  rat_point* A = new rat_point[n+1];

  rat_point a = L.front();
  rat_point b = L.front();

  rat_point p;
  forall(p,L)
  { if (rat_point::cmp_xy(p,a) < 0) a = p;
    if (rat_point::cmp_xy(p,b) > 0) b = p;
   }

  cout << string("min/max search:  %.2f sec", used_time(T)) <<endl;

  int i = 0;
  int j = n;

  A[0] = a;
  A[n] = NullPoint;

  forall(p,L) 
  { if (orientation(a,b,p) > 0) 
      A[++i] = p;
    else
      if (orientation(a,b,p) < 0) 
        A[--j] = p;
   }

  A[++i] = NullPoint;
  A[--j] = b;
  for(int k = i+1; k < j; k++) A[k] = NullPoint;

  cout << endl;
  cout << string("first partition: %.2f sec", used_time(T)) <<endl;

  //quick_hull(A,   A+i, b);
  //quick_hull(A+j, A+n, a);
  
  jobs.append(new qhull_arg(A,A+i,b));
  jobs.append(new qhull_arg(A+j,A+n,a));

  thread_t* thr = new thread_t[thread_num];

  for(int i=0; i<thread_num; i++)  status[i] = 0;

  for(int i=0; i<thread_num; i++) 
    thr[i] = thread_create(qhull_thread,(void*)i);

  for(int i=0; i<thread_num; i++) thread_join(thr[i]);


  list<rat_point> result;

  for(int k=0; k<n; k++) 
  { rat_point p = A[k];
    if (identical(p,NullPoint)) continue;
    result.append(p);
   }

  delete[] A;
  return result;
}


//------------------------------------------------------------------------------

void quick_hull8(rat_point* l, rat_point* r, const rat_point& b, int delta)
{
  if (r <= l+2) return;

  rat_point a = *l;

  if (r <= l+delta)  // graham scan
  {
    if (rat_point::cmp_xy(a,b) < 0)
       quicksort(l+1,r-1,rp_smaller(),rp_smaller_F());
    else
       quicksort(l+1,r-1,rp_greater(),rp_greater_F());

    rat_point tmp = *r;
    *r = b;

    rat_point* j = l+1;
    for(rat_point* p = l+2; p<=r; p++)
    { rat_point pp = *p;
      while(j > l && orientation(*(j-1),*j,pp) >= 0) j--;
      *++j = pp;
     }

    while (j < r) *j++ = NullPoint;
    *r = tmp;
    return;
   }



  rat_vector vec = b-a;
  rat_point* k = l+1;
  rat_point c = *k;
  rat_point d = c+vec;

  for(rat_point* p=l+2; p<r; p++)
  {  int orient = orientation(c,d,*p);
     if (orient > 0 || (orient == 0 && compare(c,*p) > 0))
     { k = p; 
       c = *k; 
       d = c+vec; 
      } 
   }

//if (orientation(a,b,c) == 0) return; 


  *k = *(l+1);

  rat_point* i = l+1;
  rat_point* j = r;
  rat_point* p = i+1;

  while (p < j)
  { if (orientation(a,c,*p) > 0)
      swap(*++i,*p++);
    else
      if (orientation(c,b,*p) > 0)
         swap(*--j,*p);
      else
         *p++ = NullPoint;
   }

  *(l+1) = *i;
  *i = NullPoint;
  *--j = c;

  quick_hull8(l,i,c,delta);
  quick_hull8(j,r,b,delta);
}



list<rat_point> quick_hull8(const list<rat_point>& L, int d = 256)
{ int n = L.length();

  rat_point* A = new rat_point[n+1];

  rat_point a = L.front();
  rat_point b = L.front();

  rat_point p;
  forall(p,L)
  { if (rat_point::cmp_xy(p,a) < 0) a = p;
    if (rat_point::cmp_xy(p,b) > 0) b = p;
   }

  int i = 0;
  int j = n;

  A[0] = a;
  A[n] = NullPoint;

  forall(p,L) 
  { if (orientation(a,b,p) > 0) 
      A[++i] = p;
    else
      if (orientation(a,b,p) < 0) 
        A[--j] = p;
   }

  A[++i] = NullPoint;
  A[--j] = b;
  for(int k = i+1; k < j; k++) A[k] = NullPoint;

  quick_hull8(A,   A+i, b, d);
  quick_hull8(A+j, A+n, a, d);

  list<rat_point> result;

  for(int k=0; k<n; k++) 
  { rat_point p = A[k];
    if (identical(p,NullPoint)) continue;
    result.append(p);
   }

  delete[] A;
  return result;
}


//------------------------------------------------------------------------------





int main()
{
  int n = read_int("size  = ");
  thread_num = read_int("threads = ");
  limit = read_int("limit = ");

  assert(limit > 3);
  assert(thread_num <= MAX_THREADS);

  list<rat_point>  L;
  string gen;

  while (gen == "")
  { gen  = read_string("generator d(isk)/s(quare)/c(ircle) : ");
    if (gen[0] == 'd')
       random_points_in_disc(n,10000,L);
    else if (gen[0] == 's')
       random_points_in_square(n,10000,L);
    else if (gen[0] == 'c')
       random_points_on_circle(n,10000,L);
    else gen = "";
   }


  float t1 = used_time();
  float t2 = real_time();
  cout << "thread quickhull: " << endl;

  list<rat_point> H = quick_hull(L);

  cout << string("|H| = %4d  time: %.2f (%.2f) sec",
                             H.size(), used_time(t1),real_time(t2)) <<endl;
  cout << endl;


  thread_num = 1;
  cout << "serial quickhull: " << endl;
  t1 = used_time();
  t2 = real_time();
  H =  quick_hull8(L);
  cout << string("h = %d  %.2f (%.2f) sec",H.size(),used_time(t1),real_time(t2)) <<endl;
  cout << endl;

  return 0;
}
