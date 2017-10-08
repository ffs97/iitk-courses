/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_slist_slot.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_SLIST_SLOT_H
#define LEDA_NODE_SLIST_SLOT_H

#include <LEDA/graph/node_slist.h>
#include <LEDA/graph/node_slot.h>

LEDA_BEGIN_NAMESPACE

template <int slot,class graph_t=graph>
                                      
class node_slist_slot : 
      public base_node_slist<graph_t,node_slot<graph_t::node,slot,graph_t> >
{
  typedef typename base_node_slist<graph_t,node_slot<graph_t::node,slot,graph_t> > base_list;
  public:
  node_slist_slot() {}
  node_slist_slot(const graph_t& G) : base_list(G) {}
 ~node_slist_slot() {}
};

LEDA_END_NAMESPACE

#endif
