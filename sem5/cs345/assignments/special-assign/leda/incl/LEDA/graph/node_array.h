/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_array.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_ARRAY_H
#define LEDA_NODE_ARRAY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600198
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// node arrays
//------------------------------------------------------------------------------

#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE



/*{\Manpage {node_array} {E} {Node Arrays}}*/


template <class E, class graph_t=graph>
class node_array : public graph_map<graph_t> {



/*{\Mdefinition
An instance $A$ of the parameterized data type |\Mname|
is a partial mapping from the node set of a graph $G$ to the set of 
variables of type $E$, called the element type of the array. The domain 
$I$ of $A$ is called the index set of $A$ and $A(v)$ is called the element 
at position $v$. $A$ is said to be valid for all nodes in $I$.
The array access operator |A[v]| checks its precondition 
($A$ must be valid for $v$). The check can be turned off by compiling 
with the flag \verb+-DLEDA_CHECKING_OFF+. }*/

E def;

void clear_entry(GenPtr& x) const { LEDA_CLEAR(E,x); }
void copy_entry(GenPtr& x)  const { LEDA_COPY(E,x);  }
void init_entry(GenPtr& x)  const { x = leda_copy(def); }

#if defined(LEDA_GMAP_IO)
void read_entry(istream& i, GenPtr& x)        { LEDA_READ(E,x,i); }
void write_entry(ostream& o, GenPtr& x) const { LEDA_PRINT(E,x,o);}
#endif
  
public:

typedef typename graph_t::node node;
typedef typename graph_t::node index;
//typedef graph_t graph;
typedef E value_type;


int elem_type_id() const { return LEDA_TYPE_ID(E); }

void connect(graph_t* gptr) { graph_map<graph_t>::init(gptr,0); }


/*{\Mcreation A }*/

node_array() : graph_map<graph_t>(nil,0,0) 
{ leda_init_default(def); this->init_def_entry(); }
/*{\Mcreate creates an instance $A$ of type |\Mname| with empty
            index set.}*/

node_array(const graph_t& G) : graph_map<graph_t>(&G,G.max_node_index()+1,0) 
{ leda_init_default(def); this->init_def_entry(); this->init_table(); }
/*{\Mcreate creates an instance $A$ of type |\Mname| and initializes
            the index set of $A$ to the current node set of graph $G$. }*/


node_array(const graph_t& G, E x) : graph_map<graph_t>(&G,G.max_node_index()+1,0), def(x)
{ this->init_def_entry(); this->init_table(); }
/*{\Mcreate creates an instance $A$ of type |\Mname|, sets the
            index set of $A$ to the current node set of graph $G$ and 
            initializes $A(v)$ with $x$ for all nodes $v$ of $G$. }*/


node_array(const graph_t& G, int n, E x) : graph_map<graph_t>(&G,n,0), def(x) 
{ this->init_def_entry(); this->init_table(); }
/*{\Mcreate creates an instance $A$ of type |\Mname| valid for
            up to $n$ nodes of graph $G$ and initializes $A(v)$ with $x$ 
            for all nodes $v$ of $G$.\\
	    \precond $n \ge \Labs{V}$.\\ $A$ is also
            valid for the next $n - \Labs{V}$ nodes added to $G$.}*/


node_array(const node_array<E>& A) : graph_map<graph_t>(A),def(A.def) 
{ this->init_def_entry(); }

node_array<E>& operator=(const node_array<E>& A) 
{ this->clear_def_entry();
  graph_map<graph_t>::operator=(A);  
  def = A.def;
  this->init_def_entry();
  return *this;
 }

~node_array() { this->clear_table(); this->clear_def_entry(); }



/*{\Moperations 1.3 4.7 }*/

const graph_t&  get_graph() const
{ return *(graph_map<graph_t>::get_owner()); }
/*{\Mop  returns a reference to the graph of $A$. }*/

const E& operator[](node v) const
{ return LEDA_CONST_ACCESS(E,graph_map<graph_t>::array_read(v)); }

E& operator[](node v)
{ return LEDA_ACCESS(E,graph_map<graph_t>::array_access(v)); }

/*{\Marrop    returns the variable $A(v)$.\\
	      \precond $A$ must be valid for $v$.}*/


/*
// direct access to node slots 

int slot() { return graph_map<graph_t>::slot_index(); }

const E& operator()(node v, int i) const
{ return LEDA_CONST_ACCESS(E,v->data(i)); }

E& operator()(node v, int i)
{ return LEDA_ACCESS(E,v->data(i)); }
*/


void init()  
{ graph_map<graph_t>::init(nil,0,0); }

void init(const graph_t& G)      
{ graph_map<graph_t>::init(&G,G.max_node_index()+1,0); 
  this->init_table();
 }
/*{\Mop       sets the index set $I$ of $A$ to the node  
	      set of $G$, i.e., makes $A$ valid for all nodes 
              of $G$.}*/

void init(const graph_t& G, E x) 
{ def = x;
  graph_map<graph_t>::init(&G,G.max_node_index()+1,0); 
  this->init_table();
}
/*{\Mop       makes $A$ valid for all nodes of $G$
	      and sets $A(v) = x$ for all nodes $v$ of $G$.}*/

void init(const graph_t& G, int n, E x) 
{ def = x;
  graph_map<graph_t>::init(&G,n,0); 
  this->init_table();
}
/*{\Mopl      makes $A$ valid for at most $n$ nodes  of $G$ 
              and sets $A(v) = x$ for all nodes $v$  of $G$.\\
              \precond {$n \ge \Labs{V}$}.\\ $A$ is also valid for 
              the next  $n - \Labs{V}$ nodes added to $G$. }*/

bool use_node_data(const graph_t& G) 
{ int i = graph_map<graph_t>::init(&G,G.max_node_index()+1,0,true);
  this->init_table();
  return i >= 0;
}
/*{\Mopl  use free data slots in the nodes of $G$ (if available) for 
          storing the entries of $A$. If no free data slot is available in $G$, 
          an ordinary |\Mname|is created. The number of additional data 
          slots in the nodes and edges of a graph can be specified 
          in the |graph::graph(int n_slots, int e_slots)| constructor. 
          The result is |true| if a free slot is available and |false|
          otherwise. }*/

bool use_node_data(const graph_t& G, E x) 
{ int i = graph_map<graph_t>::init(&G,G.max_node_index()+1,0,true); 
  def = x;
  this->init_table();
  return i >= 0;
}
/*{\Mopl  use free data slots in the nodes of $G$ (if available) for 
          storing the entries of $A$ and initializes $A(v) = x$ for all nodes
          $v$ of $G$. If no free data slot is available in $G$, an ordinary 
          |\Mname| is created. The number of additional data 
          slots in the nodes and edges of a graph can be specified 
          in the |graph::graph(int n_slots, int e_slots)| constructor. 
          The result is |true| if a free slot is available and |false|
          otherwise. }*/


/*{\Mimplementation
Node arrays for a graph $G$ are implemented by \CC vectors and an
internal numbering of the nodes and edges of $G$. The access operation
takes constant time, $init$ takes time $O(n)$, where $n$ is the number of
nodes in $G$. The space requirement is $O(n)$.
    
{\bf Remark}: A node array is only valid for a bounded number of
nodes of $G$. This number is either the number
of nodes of $G$ at the moment of creation of the array or it is explicitely 
set by the user. Dynamic node arrays can be realized by node 
maps (cf. section \ref{Node Maps}). }*/

};


#if LEDA_ROOT_INCL_ID == 600198
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
