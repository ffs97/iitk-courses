/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_array2.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_ARRAY2_H
#define LEDA_NODE_ARRAY2_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600175
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// node arrays
//------------------------------------------------------------------------------

#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE



template <class E, int slot, class graph_t=graph>

class node_array2 {

typedef typename graph_t::node     node;

typedef E value_type;

//const graph_t* gptr;
const int sl;

public:

int get_slot() { return sl; }

operator int() { return sl; }

void init(const graph_t& G, E x = E()) 
{ //gPTR = &G;
  node v; forall_nodes(v,G) 
      v->bdata(sl) = leda_copy(x); 
}

node_array2(int slt)
{ //gPTR = 0; 
  (int&)sl = (graph_t::n_base_sz + slt) * sizeof(GenPtr); 
}

node_array2() 
{ //gPTR = 0; 
  (int&)sl = (graph_t::n_base_sz + slot) * sizeof(GenPtr); 
}

node_array2(const node_array2<E,slot,graph_t>& A) : sl(A.sl) {}

node_array2(const graph_t& G)  
{ (int&)sl = (graph_t::n_base_sz + slot) * sizeof(GenPtr); 
  //gPTR = &G;
  if (LEDA_TYPE_ID(E) != INT_TYPE_ID) init(G,E()); 
}

node_array2(const graph_t& G, const E& x)  { init(G,x); }


~node_array2()
{ /* node v; 
     forall_nodes(v,*gptr) leda_clear(v->bdata(sl)); 
   */ 
 }


const E& operator[](node v) const
{ return LEDA_CONST_ACCESS(E,v->bdata(sl)); }

E& operator[](node v)       
{ return LEDA_ACCESS(E,v->bdata(sl)); }


bool use_node_data(const graph_t& G, const E& x) { init(G,x); return true; }
bool use_node_data(const graph_t&) { return true; }


};



#if LEDA_ROOT_INCL_ID == 600175
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
