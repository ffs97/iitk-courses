/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  edge_slot.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_EDGE_SLOT_H
#define LEDA_EDGE_SLOT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600219
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE


template <class E, class graph_t=graph, int slot=-1>
class edge_slot {

const graph_t* gptr;

public:

typedef typename graph_t::edge edge;
typedef typename graph_t::edge index;
//typedef graph_t graph;
typedef E value_type;


void init(const graph_t& G, const E& x) 
{ gptr = &G;
  edge e; 
  forall_edges(e,G) operator[](e) = x;
     //e->data(graph_t::e_base_sz + slot) = leda_copy(x); 
     *(E*)&(e->data(graph_t::e_base_sz+slot)) = x; 
}

void init(const graph_t& G, int, const E& x) // compatibility with edge_array 
{ init(G,x); }

void init(const graph_t& G)
{ gptr = &G;
  if (LEDA_TYPE_ID(E) != INT_TYPE_ID)
  { edge e; 
    forall_edges(e,G) 
       LEDA_CREATE(E,e->data(graph_t::e_base_sz + slot)); 
   }
}

edge_slot()                              { gptr = 0;  }
edge_slot(const graph_t& G)              { init(G);   }
edge_slot(const graph_t& G, const E& x)  { init(G,x); }

~edge_slot()
{ if (gptr)
  { if (LEDA_TYPE_ID(E) != INT_TYPE_ID)
    { edge e; 
      forall_edges(e,*gptr) 
         LEDA_CLEAR(E,e->data(graph_t::e_base_sz+slot)); 
     }
   }
}

const E& operator[](edge e) const
{ return LEDA_CONST_ACCESS(E,e->data(graph_t::e_base_sz+slot)); 
  //return *(const E*)&(e->data(graph_t::e_base_sz+slot)); 
}

E& operator[](edge e)       
{ return LEDA_ACCESS(E,e->data(graph_t::e_base_sz+slot)); 
  //return *(E*)&(e->data(graph_t::e_base_sz+slot)); 
}



bool use_edge_data(const graph_t& G, const E& x) { init(G,x); return true; }
bool use_edge_data(const graph_t&) { return true; }


};


#if !(defined(_MSC_VER) && _MSC_VER < 1300) && !defined(__BORLANDC__)
template <class E, class graph_t>
class edge_slot<E,graph_t,-1> : public edge_array<E,graph_t> {

public:

edge_slot(const graph_t& G) {
if (!use_edge_data(G,E()))
   LEDA_EXCEPTION(1,"edge_dslot: no slot available");
}

edge_slot(const graph_t& G, const E& x) {
if (!use_edge_data(G,x))
   LEDA_EXCEPTION(1,"edge_dslot: no slot available");
}

};



/*
template <class E, class graph_t, int slot, int offset>
class edge_slot2 {

typedef typename graph_t::edge edge;

const graph_t* gptr;

public:

typedef E value_type;

void init(const graph_t& G, const E&) 
{ gptr = &G; }

void init(const graph_t& G, int, const E& x) // compatibility with edge_array 
{ init(G,x); }

void init(const graph_t& G)
{ gptr = &G; }

edge_slot2()                              { gptr = 0;  }
edge_slot2(const graph_t& G)              { init(G);   }
edge_slot2(const graph_t& G, const E& x)  { init(G,x); }

~edge_slot2() {}

const E& operator[](edge e) const
{ const unsigned short* p = (unsigned short*)&(e->data(graph_t::n_base_sz+slot)); 
  return *(const E*)(p+offset);
}


E& operator[](edge e)       
{ unsigned short* p = (unsigned short*)&(e->data(graph_t::n_base_sz+slot)); 
  return *(E*)(p+offset);
}



bool use_edge_data(const graph_t& G, const E& x) { init(G,x); return true; }
bool use_edge_data(const graph_t&) { return true; }


};
*/


#endif

LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600219
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif
