/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  edge_map.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_EDGE_MAP_H
#define LEDA_EDGE_MAP_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600200
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// edge maps
//
// last modified: december 1998  (derived from edge_array) 
//
//------------------------------------------------------------------------------

#define  edge_map edge_map_base

/*{\Manpage {edge_map} {E} {Edge Maps} }*/

#include <LEDA/graph/graph.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/edge_slot.h>

LEDA_BEGIN_NAMESPACE



template <class E,class graph_t = graph>
class edge_map : public edge_array<E,graph_t> {

typedef typename graph_t::edge edge;
typedef edge_array<E,graph_t> base;



/*{\Mdefinition
An instance of the data type |\Mname| is a map for the edges of a graph
$G$, i.e., equivalent to |map<edge,E>| (cf. \ref{Maps}). It can be
used as a dynamic variant of the data type |edge_array|
(cf. \ref{Edge Arrays}). {\bf New:} Since |\Mname| is derived from 
|edge_array<E>| edge maps can be passed (by reference) to functions
with edge array parameters. In particular, all LEDA graph algorithms
expecting an |edge_array<E>&| argument can be passed an |edge_map<E>&|
instead. }*/

public:


/*{\Mcreation M }*/

edge_map() { base::enable_map_access(); }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it to the map with empty domain. }*/

edge_map(const graph_t& G) : edge_array<E,graph_t>(G) { base::enable_map_access(); }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it with a mapping $m$ from the set of all edges of $G$ into 
             the set of variables of type $E$. The variables in the range 
             of $m$ are initialized by a call of the default constructor 
             of type $E$. }*/

edge_map(const graph_t& G, E x) : edge_array<E,graph_t>(G,x) { base::enable_map_access(); }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it with a mapping $m$ from the set of all edges of $G$ into 
             the set of variables of type $E$. The variables in the range 
             of $m$ are initialized with a copy of $x$. }*/

edge_map(const edge_map<E,graph_t>& A) : edge_array<E,graph_t>(A) { base::enable_map_access(); }

edge_map<E,graph_t>& operator=(const edge_map<E,graph_t>& A) 
{ edge_array<E,graph_t>::operator=(A); return *this; }

~edge_map() {}


/*{\Moperations 1.3 4.3 }*/

const graph_t&  get_graph() const { return edge_array<E,graph_t>::get_graph(); }
/*{\Mop  returns a reference to the graph of |\Mvar|. }*/
 

void init()  { graph_map<graph_t>::init(nil,0,0); }
/*{\Mop      makes |\Mvar| a edge map with empty domain. }*/

void init(const graph_t& G) { edge_array<E,graph_t>::init(G); }     
/*{\Mop       makes |\Mvar| a mapping $m$ from the set of all edges of $G$ 
              into the set of variables of type $E$. The variables in the 
              range of $m$ are initialized by a call of the default 
              constructor of type $E$. }*/

void init(const graph_t& G, E x) { edge_array<E,graph_t>::init(G,x); }     
/*{\Mop       makes |\Mvar| a mapping $m$ from the set of all edges of $G$ 
              into the set of variables of type $E$. The variables in the 
              range of $m$ are initialized with a copy of $x$. }*/

bool use_edge_data(const graph_t& G) { return edge_array<E,graph_t>::use_edge_data(G); } 

bool use_edge_data(const graph_t& G, E x) 
{ return edge_array<E,graph_t>::use_edge_data(G,x); } 
/*{\Mopl  use free data slots in the edges of $G$ (if available) for 
          storing the entries of $A$. The number of additional data 
          slots in the nodes and edges of a graph can be specified 
          in the |graph::graph(int n_slots, int e_slots)| constructor. 
          The result is |true| if a free slot is available and |false|
          otherwise. }*/


const E& operator()(edge e) const 
{  return LEDA_CONST_ACCESS(E,graph_map<graph_t>::map_read(e)); }

const E& operator[](edge e) const 
{  return LEDA_CONST_ACCESS(E,graph_map<graph_t>::map_read(e)); }

E& operator[](edge e)
{  return LEDA_ACCESS(E,graph_map<graph_t>::map_access(e)); }
/*{\Marrop    returns the variable $M(v)$. }*/

};

/*{\Mimplementation
Edge maps are implemented by an efficient hashing method based on the 
internal numbering of the edges. An access operation takes expected 
time $O(1)$. 
}*/



#if !defined(__BORLANDC__) && (!defined(_MSC_VER) || _MSC_VER >= 1300)
#undef edge_map
template <class E, class graph_t=graph, int slot=ext_slot>
class edge_map : public edge_slot<E,graph_t,slot> {
public:
edge_map() {}
edge_map(const graph_t& G) : edge_slot<E,graph_t,slot>(G)  {}
edge_map(const graph_t& G, const E& x) : edge_slot<E,graph_t,slot>(G,x)  {}
};



template <class E, class graph_t>
class edge_map<E,graph_t,ext_slot> : public edge_map_base<E,graph_t> {
public:
edge_map() {}
edge_map(const graph_t& G) : edge_map_base<E,graph_t>(G) {}
edge_map(const graph_t& G, const E& x) : edge_map_base<E,graph_t>(G,x) {}
};


template <class E, class graph_t>
class edge_map<E,graph_t,arr_slot> : public edge_array<E,graph_t> {
public:
edge_map() {}
edge_map(const graph_t& G) : edge_array<E,graph_t>(G) {}
edge_map(const graph_t& G, const E& x) : edge_array<E,graph_t>(G,x) {}
};



template <class E, class graph_t>
class edge_map<E,graph_t,use_slot> : public edge_array<E,graph_t> {

public:
edge_map() {}
edge_map(const graph_t& G) {
if (!use_edge_data(G,E())) LEDA_EXCEPTION(1,"edge_map: no slot available");
}
edge_map(const graph_t& G, const E& x) {
if (!use_edge_data(G,x)) LEDA_EXCEPTION(1,"edge_map: no slot available");
}

};


template <class E, class graph_t>
class edge_map<E,graph_t,try_slot> : public edge_array<E,graph_t> {
public:
edge_map() {}
edge_map(const graph_t& G) { use_edge_data(G,E()); }
edge_map(const graph_t& G, const E& x) { use_edge_data(G,x); }

};

#endif

LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600200
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif
