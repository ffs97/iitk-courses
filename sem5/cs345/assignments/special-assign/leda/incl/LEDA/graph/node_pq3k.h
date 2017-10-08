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
#define LEDA_ROOT_INCL_ID 600187
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <limits>

#include <assert.h>


LEDA_BEGIN_NAMESPACE

template <class prio_t, class graph_t = graph>
class node_pq22  {

typedef typename graph_t::node node;


struct pq_elem {
//pq_elem*  first_child;
  pq_elem*  parent;
  node  v;
  prio_t p;

pq_elem(node vv, const prio_t pp) : v(vv), p(pp) {}
pq_elem() {}

};

int k;

pq_elem* HEAP;
pq_elem* root;
pq_elem* last;
int      sz;
int      rebuild_count;


node_pq22& operator=(const node_pq22&);
node_pq22(const node_pq22&);


void construct(int n)
{ 
  assert(k <= 16);

  sz = n;
  HEAP = new pq_elem[n+16+1];
  root = HEAP+1;
  last = HEAP; 

  // put -infinity into HEAP[0] (sentinel)

  if (std::numeric_limits<prio_t>::is_integer)
    HEAP->p = std::numeric_limits<prio_t>::min();
  else
    HEAP->p = -std::numeric_limits<prio_t>::max();

  // fill in parent/child pointers

  for(int i=0; i<=n; i++) 
  { pq_elem* p = HEAP+i+1;
    //p->first_child = HEAP+k*i + 2;
    p->parent = HEAP+1+(i-1)/k;
   }

  HEAP[1].parent = HEAP;

  rebuild_count = 0;
}



template <class dist_array>
void rebuild(const dist_array& dist)
{ 
  rebuild_count++;

  pq_elem* l = HEAP;

  for (pq_elem* z = root; z <= last; z++)
  { node v = z->v;
    prio_t p = z->p;

    if (p != dist[v]) continue; 

    pq_elem* x = ++l;
    pq_elem* y = x->parent;
    while (p < y->p)
    { x->v = y->v;
      x->p = y->p;
      x = y;
      y = x->parent;
     }

    x->v = v; 
    x->p = p; 
  }
  last = l;
}


public:

node_pq22(int n, int _k=2) : k(_k) { construct(n); }

node_pq22(const graph_t& G,int _k=2) : k(_k)
{ construct(G.number_of_nodes() + G.number_of_edges()); }

~node_pq22() { delete[] HEAP; }

int  size()  const { return (last - HEAP);    }
bool empty() const { return last == HEAP; }

int rebuilds() const { return rebuild_count; }

void clear() { last = HEAP; }


void append(node v, const prio_t& p)
{ pq_elem* x = ++last;
  x->v = v; 
  x->p = p; 
}


void insert(node v, const prio_t& p)
{ pq_elem* x = ++last;
  pq_elem* y = x->parent;

  while (p < y->p)
  { x->v = y->v;
    x->p = y->p;
    x = y;
    y = x->parent;
   }

  x->v = v; 
  x->p = p; 
}


template <class dist_array>
void insert(node v, const prio_t& p,const dist_array& dist)
{ if (last >= HEAP+sz) rebuild(dist);
  insert(v,p);
}


node del_min()
{ 
  // replace root by last element and let it sink down 

  node v = root->v;
  
  pq_elem* it = last--;

  prio_t p = it->p;

  switch (k) {
  case 16: it[15].p = p;
  case 15: it[14].p = p;
  case 14: it[13].p = p;
  case 13: it[12].p = p;
  case 12: it[11].p = p;
  case 11: it[10].p = p;
  case 10: it[ 9].p = p;
  case  9: it[ 8].p = p;
  case  8: it[ 7].p = p;
  case  7: it[ 6].p = p;
  case  6: it[ 5].p = p;
  case  5: it[ 4].p = p;
  case  4: it[ 3].p = p;
  case  3: it[ 2].p = p;
  case  2: it[ 1].p = p;
  }

  pq_elem* h = HEAP+1;
  pq_elem* x = h;     // root
  pq_elem* y = h+1;   // first child
 
  while (y < it)
  { 
    pq_elem* z = y;

    switch (k) {
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
    //y = x->first_child;
  }


  x->v = it->v;
  x->p = p;

  return v;
}

node del_min(prio_t& p)
{ p = root->p;
  return del_min();
}



template<class dist_array>
node del_min(const dist_array& dist)
{ root->p = dist[root->v]; //sentinel
  pq_elem* x = last;
  while (x->p != dist[x->v]) x--;
  last = x;
  return del_min();
}

template<class dist_array>
node del_min(prio_t& p, const dist_array& dist)
{ p = root->p;
  return del_min(dist);
}


};


LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600187
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
