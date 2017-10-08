/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_map2.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_MAP2_H
#define LEDA_NODE_MAP2_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600199
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// two-dimensional node maps
//
// last modfified  march 98
//------------------------------------------------------------------------------


/*{\Manpage {node_map2} {E} {Two-Dimensional Node Maps} }*/

#include <LEDA/graph/graph.h>
#include <LEDA/core/map.h>

LEDA_BEGIN_NAMESPACE

template <class E, class graph_t = graph>
class node_map2 {

/*{\Mdefinition
An instance of the data type |\Mname| is a map2 for the pairs of nodes of a 
graph $G$, i.e., equivalent to $map2\<node,node,E\>$ (cf. \ref{Two-Dimensional 
Maps}). It can be used as a dynamic variant of the data type $node\_matrix$
(cf. \ref{Two Dimensional Node Arrays}). }*/

typedef typename graph_t::node node;
typedef typename graph_t::edge edge;

  map<int,E>  m;
  const graph_t* g;

  
public:

/*{\Mcreation M }*/

node_map2() { g  = nil; }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it to the map2 with empty domain. }*/

node_map2(const graph_t& G) { g = &G; }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it with a mapping $m$ from the set of all nodes of $G$ into 
             the set of variables of type $E$. The variables in the range 
             of $m$ are initialized by a call of the default constructor 
             of type $E$. }*/

node_map2(const graph_t& G, E x) : m(x) { g = &G; }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it with a mapping $m$ from the set of all nodes of $G$ into 
             the set of variables of type $E$. The variables in the range 
             of $m$ are initialized with a copy of $x$. }*/


node_map2(const node_map2<E>& M) : m(M.m), g(M.g) {}

node_map2<E>& operator=(const node_map2<E>& M)
{ m = M.m; g = M.g; return *this; }


~node_map2() {}


/*{\Moperations 1.3 4.3 }*/


void init()  { m.clear(); g = 0; }
/*{\Mop      makes |\Mvar| a node map2 with empty domain. }*/

void init(const graph_t& G)  { m.clear(); g = &G; } 
/*{\Mop       makes |\Mvar| to a mapping $m$ from the set of all nodes of $G$ 
              into the set of variables of type $E$. The variables in the 
              range of $m$ are initialized by a call of the default 
              constructor of type $E$. }*/

void init(const graph_t& G, E x) { m.clear(x); g = &G; } 
/*{\Mop       makes |\Mvar| to a mapping $m$ from the set of all nodes of $G$ 
              into the set of variables of type $E$. The variables in the 
              range of $m$ are initialized with a copy of $x$. }*/

const E& operator()(node v, node w) const 
{  return  m[((unsigned long)g->index(v)<<16) + (unsigned long)g->index(w)]; }

E& operator()(node v,node w)
{  return  m[((unsigned long)g->index(v)<<16) + (unsigned long)g->index(w)]; }
/*{\Mfunop    returns the variable $M(v,w)$. }*/

bool defined(node v, node w) const
{ return m.defined(((unsigned long)g->index(v)<<16) + (unsigned long)g->index(w)); }
/*{\Mop    returns true if $(v,w) \in dom(m)$ and false otherwise. }*/

};

/*{\Mimplementation
Node maps are implemented by an efficient hashing method based on the 
internal numbering of the nodes. An access operation takes expected 
time $O(1)$. 
}*/



#if LEDA_ROOT_INCL_ID == 600199
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
