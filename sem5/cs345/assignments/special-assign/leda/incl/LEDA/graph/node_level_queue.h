/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_level_queue.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_LEVEL_QUEUE_H
#define LEDA_NODE_LEVEL_QUEUE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600197
#include <LEDA/internal/PREAMBLE.h>
#endif


LEDA_BEGIN_NAMESPACE


template <class graph, class succ_array>
class node_level_queue {

typedef typename graph::node node;
typedef typename graph::edge edge;

node*  T;
node*  max;
int    sz;

succ_array& NEXT;

public:

node_level_queue(const graph& G, succ_array& succ) : NEXT(succ)
{ 
  sz = G.number_of_nodes();
  T = new node[sz];
  for(int i=0; i<sz; i++) T[i] = 0;
  max = T;
 }

~node_level_queue() { delete[] T; }

void set_max(int i) { max = T + i; }

void insert(node v, int i)
{ // insert on level i
  node* h = T + i;
  if (h > max) max = h;
  NEXT[v] = *h;
  *h = v;
 }


void insert_non_max(node v, int i)
{ // do not update max pointer
  node* h = T + i;
  NEXT[v] = *h;
  *h = v;
 }


void insert_max(node v, int i)
{ // new max at level i
  max = T + i;
  *max = v;
  NEXT[v] = 0;
 }


node find_max()
{ while (*max == 0) max--;
  return *max;
}

node del_max()
{ node* p = max;
  while (*p == 0) p--;
  node v = *p;
  *p = (node)NEXT[v];
  max = p;
  return v; 
}

void del_max(node v) { *max = (node)NEXT[v]; }

void clear() { 
  for(int i=0; i<sz; i++) T[i] = 0;
  max = T;
}

};


#if LEDA_ROOT_INCL_ID == 600197
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
