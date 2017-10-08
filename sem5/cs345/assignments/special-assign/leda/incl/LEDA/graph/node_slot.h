/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_slot.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_SLOT_H
#define LEDA_NODE_SLOT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600211
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE


template <class E, class graph_t=graph, int slot=-1>
class node_slot {

const graph_t* gptr;
    
private:
node_slot(const node_slot&);
const node_slot& operator=(const node_slot&);

private:
 void cleanup()
 { if (gptr)
   { if (LEDA_TYPE_ID(E) != INT_TYPE_ID)
     { node v; 
       forall_nodes(v,*gptr) LEDA_CLEAR(E,v->data(graph_t::n_base_sz+slot)); 
      }
     gptr = 0;
    }
  }
    
  
    
public:

typedef typename graph_t::node node;
typedef typename graph_t::node index;
//typedef graph_t graph;
typedef E value_type;

void init(const graph_t& G, const E& x) 
{ 
  cleanup();  
    
  gptr = &G;
  node v; 
  forall_nodes(v,G)  operator[](v) = x;
    // v->data(graph_t::n_base_sz + slot) = leda_copy(x); 
    //*(E*)&(v->data(graph_t::n_base_sz+slot)) = x; 
}

void init(const graph_t& G, int, const E& x) // compatibility with node_array 
{ init(G,x); }

void init(const graph_t& G)
{ 
  cleanup();    
  gptr = &G;
  if (LEDA_TYPE_ID(E) != INT_TYPE_ID)
  { node v; 
    forall_nodes(v,G) 
       LEDA_CREATE(E,v->data(graph_t::n_base_sz + slot));
   }
}

void init()
{ 
  cleanup();
  gptr = 0;
}

node_slot()                              { gptr = 0;  }
node_slot(const graph_t& G)              { gptr = 0; init(G);   }
node_slot(const graph_t& G, const E& x)  { gptr = 0; init(G,x); }

~node_slot()
{ 
	cleanup();
}

const E& operator[](node v) const
{ return LEDA_CONST_ACCESS(E,v->data(graph_t::n_base_sz+slot)); 
  //return *(const E*)&(v->data(graph_t::n_base_sz+slot)); 
}

E& operator[](node v)       
{ return LEDA_ACCESS(E,v->data(graph_t::n_base_sz+slot)); 
  //return *(E*)&(v->data(graph_t::n_base_sz+slot)); 
}


bool use_node_data(const graph_t& G, const E& x) { init(G,x); return true; }
bool use_node_data(const graph_t&) { return true; }


};

#if !(defined(_MSC_VER) && _MSC_VER < 1300)  && !defined(__BORLANDC__)

template <class E, class graph_t>
class node_slot<E,graph_t,-1> : public node_array<E,graph_t> {

public:

node_slot(const graph_t& G) {
if (!use_node_data(G,E()))
   LEDA_EXCEPTION(1,"node_dslot: no slot available");
}

node_slot(const graph_t& G, const E& x) {
if (!use_node_data(G,x))
   LEDA_EXCEPTION(1,"node_dslot: no slot available");
}

};
#endif


LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600211
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif
