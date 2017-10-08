/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_pq22.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_PQ22_H
#define LEDA_NODE_PQ22_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600209
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <limits>


LEDA_BEGIN_NAMESPACE

template <class prio_t, class graph_t = graph>
class node_pq22  {

typedef typename graph_t::node node;


struct pq_elem {
   node  v;
   prio_t p;
};

pq_elem* HEAP;
pq_elem* root;
pq_elem* last;
node* stop;
node* top;
int sz;


node_pq22& operator=(const node_pq22&);
node_pq22(const node_pq22&);


void sink(pq_elem* it)
{ 
  prio_t p = it->p;

  pq_elem* h = HEAP;
  pq_elem* x = h+1; // root
  pq_elem* y = h+2; // left child

  while (y < it)
  { pq_elem* z = y+1;
    prio_t py = y->p;
    prio_t pz = z->p;
    if (pz < py)
      { if (p <= pz) goto stop;
        *x = *z;
         x = z;
       }
    else
      { if (p <= py) goto stop;
        *x = *y;
        x = y;
       }
    y = h+2*(x-h);
   }

stop:
  *x = *it;
}



template <class dist_array>
void rebuild(const dist_array& dist)

{ 
  pq_elem* h = HEAP;
  pq_elem* l = HEAP;
  for (pq_elem* z = root; z < last; z++)
  { node v = z->v;
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


void construct(int n)
{ sz = n;
  HEAP = new pq_elem[n+2];
  root = HEAP+1;
  last = HEAP; 

  //sentinel
  if (std::numeric_limits<prio_t>::is_integer)
    HEAP->p = std::numeric_limits<prio_t>::min();
  else
    HEAP->p = -std::numeric_limits<prio_t>::max();

  // stack
  stop = (node*)(HEAP+n+1);
  top = stop;
  *top = 0;
}


public:

node_pq22(int n) { construct(n); }

node_pq22(const graph_t& G)
{ construct(G.number_of_nodes() + G.number_of_edges()); }


void push(node v) {  *--top = v; }
node pop() {  return *top++; }

node* Top() {  return top; }
node* Stop() { return stop; }


~node_pq22()  { delete[] HEAP; }

int  size()  const { return (last - HEAP);    }
bool empty() const { return last == HEAP; }

void clear() { last = HEAP; top = stop; }

node operator[](int i) { return HEAP[i+1].v; }

prio_t prio(int i) { return HEAP[i+1].p; }


void append(node v, const prio_t& p)
{ pq_elem* x = ++last;
  x->v = v; 
  x->p = p; 
}


void insert(node v, const prio_t& p)
{ pq_elem* h = HEAP;
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


template <class dist_array>
void insert(const node& v, const prio_t& p, const dist_array& dist)
{ if (last == HEAP+sz) rebuild(dist);
  insert(v,p);
 }



node del_min()
{ node v = root->v;
  sink(last--);
  return v;
}

node del_min(prio_t& p)
{ node v = root->v;
  p = root->p;
  sink(last--);
  return v;
}


template<class dist_array>
node del_min(const dist_array& dist)
{ node v = root->v;
  root->p = dist[v];
  pq_elem* x = last;
  while (x->p != dist[x->v]) x--;
  last = x-1;
  sink(x);
  return v;
}


template<class dist_array>
node del_min(prio_t& p, const dist_array& dist)
{ node v = root->v;
  p = root->p;
  root->p = dist[v];
  pq_elem* x = last;
  while (x->p != dist[x->v]) x--;
  last = x-1;
  sink(x);
  return v;
}


};


LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600209
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
