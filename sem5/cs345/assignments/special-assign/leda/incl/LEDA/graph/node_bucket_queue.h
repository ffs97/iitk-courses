/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_bucket_queue.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_BUCKET_QUEUE_H
#define LEDA_NODE_BUCKET_QUEUE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600181
#include <LEDA/internal/PREAMBLE.h>
#endif


LEDA_BEGIN_NAMESPACE


template <class graph, class succ_array, class pred_array>
class node_bucket_queue {

typedef typename graph::node node;
typedef typename graph::edge edge;

succ_array& NEXT;
pred_array& PRED;

node   senti;
node*  T;
int    inf;
int    max; 

public:

node_bucket_queue(succ_array& succ, pred_array& pred, int n, node s)
            : NEXT(succ),PRED(pred), senti(s), inf(n)
{ T = new node[n+1];
  for(int i=0; i<=n; i++) T[i] = s;
  max = -1;
  NEXT[s] = s;
  PRED[s] = s;
 }

~node_bucket_queue() { delete[] T; }

void reset() { max = -1; T[inf] = senti; }

//void clear(int i) { T[i] = senti; }

void clear(int d = 0) 
{ for(int i=d; i<=max; i++) T[i] = senti;
  T[inf] = senti;
  max = d-1;
}

int   max_level() const { return max; }
node  sentinel() const { return senti; }

void push(node v, int i)
{ node u = T[i];
  NEXT[v] = u;
  PRED[u] = v;
  T[i] = v;
  if (i > max && i < inf) max = i;
 }

node top(int i)   const { return T[i]; }
node first(int i) const { return T[i]; }
node next(node v) const { return NEXT[v]; }
node pred(node v) const { return PRED[v]; }

void set_pred(node v, node p) { PRED[v] = p; }

void pop(node v, int i) { T[i] = NEXT[v]; }

node pop(int i) 
{ node v = T[i]; 
  if (v != senti) T[i] = NEXT[v];
  return v;
}

void remove(node v, int i)
{ if (v == T[i])
    pop(v,i);
  else
  { node n = NEXT[v];
    node p = PRED[v];
    NEXT[p] = n;
    PRED[n] = p;
   }
}

void move(node v, int i, int j)
{ remove(v,i);
  push(v,j);
 }

/*
void insert(node v, node u)
{ NEXT[v] = NEXT[u];
  NEXT[u] = v;
}
*/
void insert(node v, node u)
{ node n = NEXT[u];
  NEXT[u] = v;
  PRED[n] = v;
  NEXT[v] = n;
  PRED[v] = u;
}

void move(node v, int i, node u)
{ remove(v,i);
  insert(v,u);
 }


};


#define forall_nodes_in_bucket(v,PQ,i) \
for(v = PQ.first(i); v != PQ.sentinel(); v = PQ.next(v))


#if LEDA_ROOT_INCL_ID == 600181
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
