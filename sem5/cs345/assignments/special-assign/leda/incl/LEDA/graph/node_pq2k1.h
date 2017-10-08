/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_pq3k.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_PQ2k_H
#define LEDA_NODE_PQ2k_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600186
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <limits>

#include <assert.h>


LEDA_BEGIN_NAMESPACE

template <class prio_t, class graph_t = graph, int k=2>
class node_pq22  {

typedef typename graph_t::node node;


struct pq_elem {
//pq_elem* parent;
  node   v;
  prio_t p;

pq_elem(node vv, const prio_t pp) : v(vv), p(pp) {}
pq_elem() {}

};

pq_elem* HEAP;
pq_elem* last;
int      sz;
int      rebuild_count;


node_pq22& operator=(const node_pq22&);
node_pq22(const node_pq22&);


void construct(int n)
{ 
  assert(k <= 32);

  sz = n;
  HEAP = new pq_elem[n+k];
  last = HEAP-1; 

  for(int i=0; i<n+k; i++) 
  { HEAP[i].p = std::numeric_limits<prio_t>::max();
    //HEAP[i].parent = HEAP+(i-1)/k;
   }

  rebuild_count = 0;
}



template <class dist_array>
void rebuild(const dist_array& dist)
{ 
  rebuild_count++;

  pq_elem* h = HEAP;
  pq_elem* l = h-1;

  for (pq_elem* z = h; z <= last; z++)
  { node v = z->v;
    prio_t p = z->p;

    if (p != dist[v]) continue; 

    pq_elem* x = ++l;
  //pq_elem* y = x->parent;
    pq_elem* y = h+(x-h-1)/k;

    while (p < y->p)
    { x->v = y->v;
      x->p = y->p;
      x = y;
    //y = x->parent;
      y = h+(x-h-1)/k;
     }

    x->v = v; 
    x->p = p; 
  }
  last = l;

  pq_elem* stop = HEAP+sz+k;
  while (++l < stop) l->p = std::numeric_limits<prio_t>::max();
}


public:

node_pq22(int n) { construct(n); }

node_pq22(const graph_t& G)
{ construct(G.number_of_nodes() + G.number_of_edges()); }

~node_pq22() { delete[] HEAP; }

int  size()  const { return (last - HEAP + 1);    }
bool empty() const { return last == HEAP - 1; }

int rebuilds() const { return rebuild_count; }

void clear() 
{ for(pq_elem* x = HEAP; x <=last; x++)
     x->p = std::numeric_limits<prio_t>::max();
  last = HEAP-1; 
 }


void append(node v, const prio_t& p)
{ pq_elem* x = ++last;
  x->v = v; 
  x->p = p; 
}


void insert(node v, const prio_t& p)
{ pq_elem* x = ++last;
  pq_elem* h = HEAP;
  pq_elem* y = h+(x-h-1)/k;
//pq_elem* y = x->parent;

  while (y != x && p < y->p)
  { x->v = y->v;
    x->p = y->p;
    x = y;
    y = h+(x-h-1)/k;
  //y = x->parent;
   }

  x->v = v; 
  x->p = p; 
}


template <class dist_array>
void insert(node v, const prio_t& p,const dist_array& dist)
{ if (last >= HEAP+sz-1) rebuild(dist);
  insert(v,p);
}


node del_min()
{ 
  // replace root by last element and let it sink down 

  pq_elem* h = HEAP;
  node v = h->v;
  
  pq_elem* it = last--;

  prio_t p = it->p;

  it->p = std::numeric_limits<prio_t>::max();

  pq_elem* x = h;     // root
  pq_elem* y = x+1;   // first child
 
  while (y < it)
  { 
    pq_elem* z = y;

    switch (k) {
    case 32: if ((++z)->p < y->p) y = z;
    case 31: if ((++z)->p < y->p) y = z;
    case 30: if ((++z)->p < y->p) y = z;
    case 29: if ((++z)->p < y->p) y = z;
    case 28: if ((++z)->p < y->p) y = z;
    case 27: if ((++z)->p < y->p) y = z;
    case 26: if ((++z)->p < y->p) y = z;
    case 25: if ((++z)->p < y->p) y = z;
    case 24: if ((++z)->p < y->p) y = z;
    case 23: if ((++z)->p < y->p) y = z;
    case 22: if ((++z)->p < y->p) y = z;
    case 21: if ((++z)->p < y->p) y = z;
    case 20: if ((++z)->p < y->p) y = z;
    case 19: if ((++z)->p < y->p) y = z;
    case 18: if ((++z)->p < y->p) y = z;
    case 17: if ((++z)->p < y->p) y = z;
    case 16: if ((++z)->p < y->p) y = z;
    case 15: if ((++z)->p < y->p) y = z;
    case 14: if ((++z)->p < y->p) y = z;
    case 13: if ((++z)->p < y->p) y = z;
    case 12: if ((++z)->p < y->p) y = z;
    case 11: if ((++z)->p < y->p) y = z;
    case 10: if ((++z)->p < y->p) y = z;
    case  9: if ((++z)->p < y->p) y = z;
    case  8: if ((++z)->p < y->p) y = z;
    case  7: if ((++z)->p < y->p) y = z;
    case  6: if ((++z)->p < y->p) y = z;
    case  5: if ((++z)->p < y->p) y = z;
    case  4: if ((++z)->p < y->p) y = z;
    case  3: if ((++z)->p < y->p) y = z;
    case  2: if ((++z)->p < y->p) y = z;
    }

    if (p <= y->p) break;

    x->v = y->v;
    x->p = y->p;
    x = y;
    y = h+k*(x-h)+1; // first child of x
  }


  x->v = it->v;
  x->p = p;

  return v;
}

node del_min(prio_t& p)
{ p = HEAP->p;
  return del_min();
}



template<class dist_array>
node del_min(const dist_array& dist)
{ pq_elem* x = last;
  while (x >= HEAP && x->p != dist[x->v]) 
  { x->p = std::numeric_limits<prio_t>::max();
    x--;
   }
  last = x;
  return del_min();
}

template<class dist_array>
node del_min(prio_t& p, const dist_array& dist)
{ p = HEAP->p;
  return del_min(dist);
}


};


LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600186
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
