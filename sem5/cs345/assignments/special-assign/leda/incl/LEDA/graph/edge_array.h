/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  edge_array.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_EDGE_ARRAY_H
#define LEDA_EDGE_ARRAY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600183
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// edge arrays
//------------------------------------------------------------------------------

#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE


/*{\Manpage {edge_array} {E} {Edge Arrays}}*/


template <class E, class graph_t=graph>
class edge_array : public graph_map<graph_t> {

/*{\Mdefinition
An instance $A$ of the parameterized data type |\Mname|
is a partial mapping from the edge set of a graph $G$ to the set of 
variables of type $E$, called the element type of the array. The domain 
$I$ of $A$ is called the index set of $A$ and $A(e)$ is called the element 
at position $e$. $A$ is said to be valid for all edges in $I$.
The array access operator |A[e]| checks its precondition 
($A$ must be valid for $e$). The check can be turned off by compiling 
with the flag \verb+-DLEDA_CHECKING_OFF+. }*/


E def;

private:

void clear_entry(GenPtr& x) const { LEDA_CLEAR(E,x); }
void copy_entry(GenPtr& x)  const { LEDA_COPY(E,x);  }
void init_entry(GenPtr& x)  const { x = leda_copy(def); }

#if defined(LEDA_GMAP_IO)
void read_entry(istream& i, GenPtr& x)        { LEDA_READ(E,x,i); }
void write_entry(ostream& o, GenPtr& x) const { LEDA_PRINT(E,x,o);}
#endif
  
public:


typedef typename graph_t::edge edge;
typedef typename graph_t::edge index;
//typedef graph_t graph;
typedef E value_type;


int elem_type_id() const { return LEDA_TYPE_ID(E); }

void connect(graph_t* gptr) { graph_map<graph_t>::init(gptr,1); }


/*{\Mcreation A }*/

edge_array() : graph_map<graph_t>(nil,0,1) { leda_init_default(def); this->init_def_entry(); }
/*{\Mcreate creates an instance $A$ of type |\Mname| with empty
            index set.}*/

edge_array(const graph_t& G) : graph_map<graph_t>(&G,G.max_edge_index()+1,1)
{ leda_init_default(def); this->init_def_entry(); this->init_table(); }
/*{\Mcreate creates an instance $A$ of type |\Mname| and initializes
            the index set of $A$ to be the current edge set of graph $G$. }*/


edge_array(const graph_t& G, E x) : graph_map<graph_t>(&G,G.max_edge_index()+1,1), def(x)
{ this->init_def_entry(); this->init_table(); }
/*{\Mcreate creates an instance $A$ of type |\Mname|, sets the
            index set of $A$ to the current edge set of graph $G$ and 
            initializes $A(v)$ with $x$ for all edges $v$ of $G$. }*/


edge_array(const graph_t& G, int n, E x) : graph_map<graph_t>(&G,n,1), def(x) 
{ this->init_def_entry(); this->init_table(); }
/*{\Mcreate creates an instance $A$ of type |\Mname| valid for
            up to $n$ edges of graph $G$ and initializes $A(e)$ with $x$
            for all edges $e$ of $G$.\\
	    \precond $n \ge \Labs{E}$.\\
	    $A$ is also
            valid for the next $n - \Labs{E}$ edges added to $G$. }*/


edge_array(const edge_array<E>& A) : graph_map<graph_t>(A),def(A.def)  
{ this->init_def_entry(); }

edge_array<E>& operator=(const edge_array<E>& A) 
{ this->clear_def_entry();
  graph_map<graph_t>::operator=(A);  
  def = A.def;
  this->init_def_entry();
  return *this;
 }

~edge_array() { this->clear_table(); this->clear_def_entry(); }



/*{\Moperations 1.3 4.7 }*/

const graph_t&  get_graph() const
{ return *(graph_map<graph_t>::get_owner()); }
/*{\Mop  returns a reference to the graph of $A$. }*/


const E& operator[](edge e) const
{ return LEDA_CONST_ACCESS(E,graph_map<graph_t>::array_read(e)); }

E& operator[](edge e)
{ return LEDA_ACCESS(E,graph_map<graph_t>::array_access(e)); }
/*{\Marrop    returns the variable $A(e)$.\\
	      \precond $A$ must be valid for $e$.}*/



/*
// direct access to edge slots 

int slot() { return graph_map<graph_t>::slot_index(); }

const E& operator()(edge e, int i) const
{ return LEDA_CONST_ACCESS(E,e->data(i)); }

E& operator()(edge e, int i)
{ return LEDA_ACCESS(E,e->data(i)); }
*/


void init()  { graph_map<graph_t>::init(nil,0,1); }

void init(const graph_t& G)      
{ graph_map<graph_t>::init(&G,G.max_edge_index()+1,1); 
  this->init_table();
 }
/*{\Mop       sets the index set $I$ of $A$ to the edge  
	      set of $G$, i.e., makes $A$ valid for all edges 
              of $G$.}*/

void init(const graph_t& G, E x) 
{ def = x;
  graph_map<graph_t>::init(&G,G.max_edge_index()+1,1); 
  this->init_table();
}
/*{\Mop       makes $A$ valid for all edges of $G$
	      and sets $A(e) = x$ for all edges $e$ of $G$. }*/

void init(const graph_t& G, int n, E x) 
{ def = x;
  graph_map<graph_t>::init(&G,n,1); 
  this->init_table();
}
/*{\Mopl      makes $A$ valid for at most $n$ edges  
	      of $G$ and sets $A(e) = x$ for all edges $e$  
	      of $G$.\\
	      \precond $n \ge \Labs{E}$.\\
	      $A$ is also
              valid for the next $n - \Labs{E}$ edges added to $G$. }*/


bool use_edge_data(const graph_t& G) 
{ int i = graph_map<graph_t>::init(&G,G.max_edge_index()+1,1,true);
  this->init_table();
  return i >= 0;
}


bool use_edge_data(const graph_t& G, E x) 
{ int i = graph_map<graph_t>::init(&G,G.max_edge_index()+1,1,true); 
  def = x;
  this->init_table();
  return i >= 0;
}
/*{\Mopl  use free data slots in the edges of $G$ (if available) for 
          storing the entries of $A$. The number of additional data 
          slots in the nodes and edges of a graph can be specified 
          in the |graph::graph(int n_slots, int e_slots)| constructor.
          The result is |true| if a free slot is available and |false|
          otherwise. }*/


/*{\Mimplementation
Edge arrays for a graph $G$ are implemented by \CC vectors and an
internal numbering of the nodes and edges of $G$. The access operation
takes constant time, $init$ takes time $O(n)$, where $n$ is the number of
edges in $G$. The space requirement is $O(n)$.
    
{\bf Remark}: An edge array is only valid for a bounded number of
edges of $G$. This number is either the number
of edges of $G$ at the moment of creation of the array or it is explicitely 
set by the user.  Dynamic edge arrays can be realized by edge 
maps (cf. section \ref{Edge Maps}). }*/

};


#if LEDA_ROOT_INCL_ID == 600183
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
