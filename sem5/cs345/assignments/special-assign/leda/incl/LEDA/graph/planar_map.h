/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  planar_map.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_PLANAR_MAP_H
#define LEDA_PLANAR_MAP_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600153
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// planar maps
//------------------------------------------------------------------------------


#include <LEDA/graph/graph.h>
#include <LEDA/graph/face_array.h>
#include <LEDA/graph/face_map.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {planar_map} {} {Planar Maps} }*/

class __exportC planar_map : public graph {

/*{\Mdefinition
    An instance $M$ of the data type $planar\_map$ is the combinatorial
    embedding of a planar graph, i.e., $M$ is bidirected (for every edge
    $(v,w)$ of $M$ the reverse edge $(w,v)$ is also in $M$) and  
    there is a planar embedding of $M$ such that for every node $v$ the 
    ordering of the edges in the adjacency list of $v$ corresponds to the 
    counter-clockwise ordering of these edges around $v$ in the embedding.
}*/


protected:

  void init_entries();
  void make_planar_map(); 

  void init_face_entries() const;
  void copy_face_entries() const;
  void clear_face_entries() const;

public:

/*{\Mcreation M }*/

planar_map() {} 

planar_map(const graph& G);
/*{\Mcreate creates an instance $M$ of type $planar\_map$ and initializes it to
    the planar map represented by the directed graph $G$.\\
    \precond $G$
    represents a bidirected planar map, i.e. for every edge $(v,w)$ in $G$
    the reverse edge $(w,v)$ is also in $G$ and there is a planar embedding
    of $G$ such that for every node $v$ the ordering of the edges in the
    adjacency list of $v$ corresponds to the counter-clockwise ordering of
    these edges around $v$ in the embedding.}*/

planar_map(const planar_map& M) : graph(M) {}

virtual ~planar_map() { clear(); }


planar_map& operator=(const planar_map& M)
{ graph::operator=(M);
  return *this;
 }

/*{\Moperations 2 4.6}*/

edge    new_edge(edge e1, edge e2)
{ return graph::split_face(e1,e2); }
/*{\Mopl    inserts the edge $e=(source(e_1),source(e_2))$
	    and its reversal into $M$ and returns $e$.\\
            \precond $e_1$ and $e_2$ are bounding the same face $F$.\\
	    The operation splits $F$ into two new faces. }*/

face    del_edge(edge e) { return graph::join_faces(e); }
/*{\Mop     deletes the edge $e$ and its reversal from |\Mvar|. The two 
            faces adjacent to $e$ are united to one new face which
            is returned. }*/


edge    split_edge(edge e) { return graph::split_map_edge(e); } 
/*{\Mop     splits edge $e=(v,w)$ and its reversal $r=(w,v)$
	    into edges $(v,u)$, $(u,w)$, $(w,u)$, and $(u,v)$.
	    Returns the edge $(u,w)$. }*/


node    new_node(const list<edge>& el);
/*{\Mopl    splits the face bounded by the edges in $el$ by
	    inserting a new node $u$ and connecting it to all
	    source nodes of edges in $el$.\\
            \precond all edges in $el$ bound the same face.}*/

node    new_node(face f) { return new_node(adj_edges(f)); }
/*{\Mop     splits face $f$ into triangles by inserting a new
	    node $u$ and connecting it to all nodes of $f$.
	    Returns $u$.}*/


list<edge> triangulate() { return triangulate_planar_map(); }
/*{\Mop     triangulates all faces of |\Mvar| by inserting new
	    edges. The list of inserted edges is returned.}*/

};

/*{\Mimplementation
Planar maps are implemented by parameterized directed graph.
All operations take constant time, except for new\_edge and del\_edge
which take time $O(f)$ where $f$ is the number of edges in the created
faces and triangulate and straight\_line\_embedding which take time $O(n)$
where $n$ is the current size (number of edges) of the planar map. }*/


//------------------------------------------------------------------------------
// PLANAR_MAP: generic planar map
//------------------------------------------------------------------------------

/*{\Mtext
\newpage
}*/

/*{\Manpage {PLANAR_MAP} {vtype, etype, ftype} {Parameterized Planar Maps}}*/

template <class vtype, class etype, class ftype>

class PLANAR_MAP : public planar_map {

/*{\Mdefinition
A parameterized planar map $M$ is a planar map whose nodes, edges and 
faces contain additional (user defined) data. Every node  contains
an element of a data type $vtype$, called the node type of $M$,every edge 
contains an element of a data type $etype$, called the edge type of $M$, and 
every face contains an element of a data type $ftype$ called the face type of
$M$. All operations of the data type $planar\_map$ are also defined for
instances of any parameterized planar\_map type. For parameterized
planar maps there are additional operations to access or update
the node and face entries.}*/


void init_node_entry(GenPtr& x)  const { LEDA_CREATE(vtype,x); }
void init_edge_entry(GenPtr& x)  const { LEDA_CREATE(etype,x); }
void init_face_entry(GenPtr& x)  const { LEDA_CREATE(ftype,x); }

void copy_node_entry(GenPtr& x)  const { LEDA_COPY(vtype,x); }
void copy_edge_entry(GenPtr& x)  const { LEDA_COPY(etype,x); }
void copy_face_entry(GenPtr& x)  const { LEDA_COPY(ftype,x); }

void clear_node_entry(GenPtr& x) const { LEDA_CLEAR(vtype,x); }
void clear_edge_entry(GenPtr& x) const { LEDA_CLEAR(etype,x); }
void clear_face_entry(GenPtr& x) const { LEDA_CLEAR(ftype,x); }

void print_node_entry(ostream& o, GenPtr& x)  const
{ o << "(" << LEDA_ACCESS(vtype,x) << ")"; }

void print_edge_entry(ostream& o, GenPtr& x)  const
{ o << "(" << LEDA_ACCESS(etype,x) << ")"; }

void print_face_entry(ostream& o, GenPtr& x)  const
{ o << "(" << LEDA_ACCESS(ftype,x) << ")"; }

public:

/*{\Mcreation M }*/

PLANAR_MAP() {}

PLANAR_MAP(const GRAPH<vtype,etype>& G) : planar_map((graph&)G)   
{ copy_all_entries(); 
  init_face_entries(); 
}

/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it to
the planar map represented by the parameterized directed graph $G$.
The node and edge entries of $G$ are copied into the corresponding 
nodes and edges of $M$. Every face $f$ of $M$ is assigned the default
value of type $ftype$.\\
\precond $G$ represents a planar map. }*/

~PLANAR_MAP() { 
  clear_all_entries(); 
  clear_face_entries();
 }
PLANAR_MAP(const PLANAR_MAP<vtype,etype,ftype>& M) : planar_map(M)   
{ copy_all_entries(); 
  copy_face_entries(); 
}


PLANAR_MAP<vtype,etype,ftype>& operator=(const PLANAR_MAP<vtype,etype,ftype>& M)
{ clear_all_entries();
  clear_face_entries();
  planar_map::operator=(M); 
  copy_all_entries();
  copy_face_entries();
  return *this;
 }


/*{\Moperations 1.5 5.0 }*/

const vtype& inf(node v) const 
{ return LEDA_CONST_ACCESS(vtype,planar_map::inf(v)); }
/*{\Mop      returns the information of node $v$.}*/

const etype& inf(edge e) const 
{ return LEDA_CONST_ACCESS(etype,planar_map::inf(e)); }
/*{\Mop      returns the information of node $v$.}*/

const ftype& inf(face f) const 
{ return LEDA_CONST_ACCESS(ftype,planar_map::inf(f)); }
/*{\Mop      returns the information of face $f$.}*/


vtype& entry(node v) { return LEDA_ACCESS(vtype,planar_map::entry(v)); }
etype& entry(edge e) { return LEDA_ACCESS(etype,planar_map::entry(e)); }
ftype& entry(face f) { return LEDA_ACCESS(ftype,planar_map::entry(f)); }


vtype& operator[] (node v)  { return entry(v); }
/*{\Marrop   returns a reference to the information of node $v$.}*/

etype& operator[] (edge e)  { return entry(e); }
/*{\Marrop   returns a reference to the information of edge $e$.}*/

ftype& operator[] (face f)    { return entry(f); }
/*{\Marrop   returns a reference to the information of face $f$.}*/

const vtype& operator[] (node v) const { return inf(v); }
const etype& operator[] (edge e) const { return inf(e); }
const ftype& operator[] (face f) const { return inf(f); }


void   assign(node v, const vtype& x) { entry(v) = x; }
/*{\Mopl     makes $x$ the information of node $v$.}*/

void   assign(edge e, const etype& x) { entry(e) = x; }
/*{\Mopl     makes $x$ the information of node $v$.}*/

void   assign(face f, const ftype& x) { entry(f) = x; }
/*{\Mopl     makes $x$ the information of face $f$.}*/


/*{\Moptions nextwarning=no}*/
edge   new_edge(edge e1, edge e2)
                          { return planar_map::new_edge(e1,e2); }

edge   new_edge(edge e1, edge e2, const ftype& y)
{ edge e = planar_map::new_edge(e1,e2); 
  assign(face_of(reversal(e)),y);
  return e;
}
/*{\Mopl     inserts the edge $e=(source(e_1),source(e_2))$
	     and its reversal edge $e'$ into $M$.\\
	     \precond
	     $e_1$ and $e_2$ are bounding the same face $F$.\\
	     The operation splits $F$ into two new faces $f$,
	     adjacent to edge $e$, and $f'$, adjacent to edge
	     $e'$, with inf($f$) = inf ($F$) and inf($f'$) = $y$.}*/


edge   split_edge(edge e, const vtype& x)
{ edge e1 = graph::split_map_edge(e); 
  assign(source(e1),x); 
  return e1;
}
/*{\Mopl    splits edge $e=(v,w)$ and its reversal $r=(w,v)$
	    into edges $(v,u)$, $(u,w)$, $(w,u)$, and $(u,v)$.
            Assigns information $x$ to the created node $u$ and
	    returns the edge $(u,w)$. }*/

node   new_node(list<edge>& el, const vtype& x)
{ node v = planar_map::new_node(el);
  assign(v,x);
  return v;
 }
/*{\Mopl    splits the face bounded by the edges in $el$ by
	    inserting a new node $u$ and connecting it to all
	    source nodes of edges in $el$. Assigns information $x$
            to $u$ and returns $u$.\\
	    \precond all edges in $el$ bound 
            the same face.}*/

node   new_node(face f, const vtype& x)
{ node v = planar_map::new_node(f);
  assign(v,x);
  return v;
 }
/*{\Mopl      splits face $f$ into triangles by inserting a new
              node $u$ with information $x$ and connecting it 
	      to all nodes of $f$. Returns $u$.}*/

};

/*{\Mimplementation
Parameterized planar maps are derived from planar maps. All additional 
operations for manipulating the node and edge contents take constant time.}*/




#if LEDA_ROOT_INCL_ID == 600153
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
