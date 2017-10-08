/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bin_heap22.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



LEDA_BEGIN_NAMESPACE

template <class prio_t, class info_t>
class bin_heap22  {

struct pq_elem {
   info_t v;
   prio_t p;
};

pq_elem* HEAP;
pq_elem* root;
pq_elem* last;
int sz;


bin_heap22& operator=(const bin_heap22&);
bin_heap22(const bin_heap22&);


void sink(pq_elem* it)
{ 
  pq_elem* h = HEAP;
  pq_elem* l = last;

  info_t v = it->v;
  prio_t p = it->p;

  //(l+1)->p = p; //sentinel
  it->p = MAXINT;

  pq_elem* x = h+1; // root
  pq_elem* y = h+2; // left child

  while (y <= l)
  { pq_elem* z = y+1;
    prio_t py = y->p;
    prio_t pz = z->p;
    if (pz < py)
      { if (p <= pz)  break;
        *x = *z;
         x = z;
       }
    else
      { if (p <= py)  break;
        *x = *y;
        x = y;
       }
    y = h+2*(x-h);
   }

  x->v = v;
  x->p = p;
}


template <class dist_array>
void rebuild(const dist_array& dist)
{ pq_elem* h = HEAP;
  pq_elem* l = HEAP;
  for (pq_elem* z = root; z < last; z++)
  { info_t v = z->v;
    prio_t p = z->p;
    if (p != dist[v]) continue; 
    pq_elem* x = ++l;
    pq_elem* y = h+(x-h)/2;
    while (p < y->p)
    { *x = *y;
      x = y;
      y = h+(x-h)/2;
     }
    x->v = v; 
    x->p = p; 
  }
  last = l;
}



public:

bin_heap22(int n) 
{ sz = n;
  HEAP = new pq_elem[sz+2];
  root = HEAP+1;
  last = HEAP; 
  HEAP->p = -MAXINT;  // sentinel for insert (rise)
}


~bin_heap22()  { delete[] HEAP; }

int  size()  const { return (last - HEAP);    }
bool empty() const { return last == HEAP; }

void clear() { last = HEAP; }

info_t operator[](int i) { return HEAP[i+1].v; }


pq_elem* find(const info_t& u, const prio_t& p)
{ pq_elem* h = HEAP;
  pq_elem* x = last;
  while (x > h)
  { if (x->v == u) break;
    if (x->p < p) { x = h; break; }
    x = h+(x-h)/2;
   }
  return x;
}



void insert(const info_t& v, const prio_t& p)
{
  pq_elem* h = HEAP;
  pq_elem* x = ++last;
  pq_elem* y = h+(x-h)/2;

  (x+1)->p = MAXINT;

  while (p < y->p)
  { *x = *y;
    x = y;
    y = h+(x-h)/2;
   }

  x->v = v; 
  x->p = p; 
}


template <class dist_array>
void insert(const info_t& v, const prio_t& p, const dist_array& dist)
{
  pq_elem* h = HEAP;

  if (last == h+sz) rebuild(dist);

  pq_elem* x = ++last;
  pq_elem* y = h+(x-h)/2;

  while (p < y->p)
  { *x = *y;
    x = y;
    y = h+(x-h)/2;
   }

  x->v = v; 
  x->p = p; 
}


info_t del_min()
{ info_t v = root->v;
  sink(last--);
  return v;
}

info_t del_min(prio_t& p)
{ info_t v = root->v;
  p = root->p;
  sink(last--);
  return v;
}

template<class dist_array>
info_t del_min(prio_t& p, const dist_array& dist)
{ info_t v = root->v;
  p = root->p;
  pq_elem* x = last;
  while (x->p != dist[x->v]) x--;
  last = x-1;
  sink(x);
  return v;
}

};


LEDA_END_NAMESPACE
