/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_map.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_MAP_H
#define LEDA_NODE_MAP_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600156
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// node maps
//
// last modified: december 1998  (derived from node_array) 
//
//------------------------------------------------------------------------------

enum { ext_slot =-1, use_slot=-2, try_slot=-3, arr_slot=-4, static_slot=-5 };

#define  node_map node_map_base

/*{\Manpage {node_map} {E} {Node Maps} }*/

#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/graph/node_slot1.h>


LEDA_BEGIN_NAMESPACE


template <class E,class graph_t = graph>
class node_map : public node_array<E,graph_t> {

typedef typename graph_t::node node;
typedef node_array<E,graph_t> base;



/*{\Mdefinition
An instance of the data type |\Mname| is a map for the nodes of a graph
$G$, i.e., equivalent to |map<node,E>| (cf. \ref{Maps}). It can be
used as a dynamic variant of the data type |node_array|
(cf. \ref{Node Arrays}). {\bf New:} Since |\Mname| is derived from 
|node_array<E>| node maps can be passed (by reference) to functions
with node array parameters. In particular, all LEDA graph algorithms 
expecting a |node_array<E>&| argument can be passed a |node_map<E>| 
instead. }*/

public:


/*{\Mcreation M }*/

node_map() { base::enable_map_access(); }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it to the map with empty domain. }*/

node_map(const graph_t& G) : node_array<E,graph_t>(G) { base::enable_map_access(); }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it with a mapping $m$ from the set of all nodes of $G$ into 
             the set of variables of type $E$. The variables in the range 
             of $m$ are initialized by a call of the default constructor 
             of type $E$. }*/

node_map(const graph_t& G, E x) : node_array<E,graph_t>(G,x) { base::enable_map_access(); }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it with a mapping $m$ from the set of all nodes of $G$ into 
             the set of variables of type $E$. The variables in the range 
             of $m$ are initialized with a copy of $x$. }*/

node_map(const node_map<E,graph_t>& A) : node_array<E,graph_t>(A) { base::enable_map_access(); }

node_map<E,graph_t>& operator=(const node_map<E,graph_t>& A) 
{ node_array<E,graph_t>::operator=(A); return *this; }

~node_map() {}


/*{\Moperations 1.3 4.3 }*/

const graph_t&  get_graph() const { return node_array<E,graph_t>::get_graph(); }
/*{\Mop  returns a reference to the graph of |\Mvar|. }*/
 

void init()  { graph_map<graph_t>::init(nil,0,0); }
/*{\Mop      makes |\Mvar| a node map with empty domain. }*/

void init(const graph_t& G) { node_array<E,graph_t>::init(G); }     
/*{\Mop       makes |\Mvar| a mapping $m$ from the set of all nodes of $G$ 
              into the set of variables of type $E$. The variables in the 
              range of $m$ are initialized by a call of the default 
              constructor of type $E$. }*/

void init(const graph_t& G, E x) { node_array<E,graph_t>::init(G,x); }     
/*{\Mop       makes |\Mvar| a mapping $m$ from the set of all nodes of $G$ 
              into the set of variables of type $E$. The variables in the 
              range of $m$ are initialized with a copy of $x$. }*/

bool use_node_data(const graph_t& G) { return node_array<E,graph_t>::use_node_data(G); } 

bool use_node_data(const graph_t& G, E x) 
{ return node_array<E,graph_t>::use_node_data(G,x); } 
/*{\Mopl  use free data slots in the nodes of $G$ (if available) for 
          storing the entries of $A$. The number of additional data 
          slots in the nodes and edges of a graph can be specified 
          in the |graph::graph(int n_slots, int e_slots)| constructor. 
          The result is |true| if a free slot is available and |false|
          otherwise. }*/


const E& operator()(node v) const 
{  return LEDA_CONST_ACCESS(E,graph_map<graph_t>::map_read(v)); }

const E& operator[](node v) const 
{  return LEDA_CONST_ACCESS(E,graph_map<graph_t>::map_read(v)); }

E& operator[](node v)
{  return LEDA_ACCESS(E,graph_map<graph_t>::map_access(v)); }
/*{\Marrop    returns the variable $M(v)$. }*/

};

/*{\Mimplementation
Node maps either use free node\_slots or they are implemented 
by an efficient hashing method based on the 
internal numbering of the nodes or they use. In each case an 
access operation takes expected 
time $O(1)$. 
}*/


#if !defined(__BORLANDC__) && (!defined(_MSC_VER) || _MSC_VER >= 1300)
#undef  node_map 
template <class E, class graph_t=graph, int slot=ext_slot>
class node_map : public node_slot1<E,graph_t,slot> {
public:
node_map() {}
node_map(const graph_t& G) : node_slot1<E,graph_t,slot>(G)  {}
node_map(const graph_t& G, const E& x) : node_slot1<E,graph_t,slot>(G,x)  {}
};



template <class E, class graph_t>
class node_map<E,graph_t,ext_slot> : public node_map_base<E,graph_t> {
public:
node_map() {}
node_map(const graph_t& G) : node_map_base<E,graph_t>(G) {}
node_map(const graph_t& G, const E& x) : node_map_base<E,graph_t>(G,x) {}
};


template <class E, class graph_t>
class node_map<E,graph_t,arr_slot> : public node_array<E,graph_t> {
public:
node_map() {}
node_map(const graph_t& G) : node_array<E,graph_t>(G) {}
node_map(const graph_t& G, const E& x) : node_array<E,graph_t>(G,x) {}
};



template <class E, class graph_t>
class node_map<E,graph_t,use_slot> : public node_array<E,graph_t> {

public:
node_map() {}
node_map(const graph_t& G) {
if (!use_node_data(G,E())) LEDA_EXCEPTION(1,"node_map: no slot available");
}
node_map(const graph_t& G, const E& x) {
if (!use_node_data(G,x)) LEDA_EXCEPTION(1,"node_map: no slot available");
}

};


template <class E, class graph_t>
class node_map<E,graph_t,try_slot> : public node_array<E,graph_t> {
public:
node_map() {}
node_map(const graph_t& G) { use_node_data(G,E()); }
node_map(const graph_t& G, const E& x) { use_node_data(G,x); }

};
#endif

LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600156
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
