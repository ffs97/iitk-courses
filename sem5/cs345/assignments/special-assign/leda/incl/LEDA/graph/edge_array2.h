/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  edge_array2.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_EDGE_ARRAY2_H
#define LEDA_EDGE_ARRAY2_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600196
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// edge arrays
//------------------------------------------------------------------------------

#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE



template <class E, int slot, class graph_t=graph>

class edge_array2 {

typedef typename graph_t::edge     edge;

typedef E value_type;

//const graph_t* gptr;

const int sl;

public:

void init(const graph_t& G, E x = E()) 
{ //gPTR = &G;
  edge e; forall_edges(e,G) 
      e->bdata(sl) = leda_copy(x); }

edge_array2() : sl(0)
{ /*gPTR = 0; */ 
(int&)sl = (graph_t::e_base_sz + slot) * sizeof(GenPtr); 
}

edge_array2(const graph_t& G)  
{ (int&)sl = (graph_t::e_base_sz + slot) * sizeof(GenPtr);
  //gPTR = &G;
  if (LEDA_TYPE_ID(E) != INT_TYPE_ID) init(G,E()); 
}

edge_array2(const graph_t& G, const E& x)  { init(G,x); }


~edge_array2()
{ /* edge e; forall_edges(e,*gptr) leda_clear(e->bdata(sl)); */ }

void set(edge e, const E& x)
{ LEDA_ACCESS(E,e->bdata(sl)) = x; }

const E& get(edge e) const
{ return LEDA_CONST_ACCESS(E,e->bdata(sl)); }

const E& operator[](edge e) const  { return get(e); }

E&       operator[](edge e)       
{ return LEDA_ACCESS(E,e->bdata(sl)); }


bool use_node_data(const graph_t& G, const E& x) { init(G,x); return true; }
bool use_edge_data(const graph_t&) { return true; }

};



#if LEDA_ROOT_INCL_ID == 600196
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
