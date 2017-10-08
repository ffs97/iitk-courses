/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_pq_slot.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_SLIST_SLOT_H
#define LEDA_NODE_SLIST_SLOT_H

#include <LEDA/graph/node_pq.h>
#include <LEDA/graph/node_slot.h>

LEDA_BEGIN_NAMESPACE

template <class P,int slot, class graph_t=graph,class prio_impl=bin_heap>
struct node_pq_slot : 
      public node_pq<P,graph_t,prio_impl,node_slot<GenPtr,slot,graph_t> >
{
  typedef node_pq<P,graph_t,prio_impl,node_slot<GenPtr,slot,graph_t> > base_pq;
  node_pq_slot() {}
  node_pq_slot(const graph_t& G) : base_pq(G) {}
 ~node_pq_slot() {}
};

LEDA_END_NAMESPACE

#endif
