/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  face_map.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_FACE_MAP_H
#define LEDA_FACE_MAP_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600188
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// face maps
//
// last modified: december 1998  (derived from face_array) 
//
//------------------------------------------------------------------------------


/*{\Manpage {face_map} {E} {Face Maps} }*/

#include <LEDA/graph/graph.h>
#include <LEDA/graph/face_array.h>

LEDA_BEGIN_NAMESPACE



template <class E,class graph_t = graph>
class face_map : public face_array<E,graph_t> {

typedef typename graph_t::face face;
typedef face_array<E,graph_t> base;


/*{\Mdefinition
An instance of the data type |\Mname| is a map for the faces of a graph
$G$, i.e., equivalent to |map<face,E>| (cf. \ref{Maps}). It can be
used as a dynamic variant of the data type |face_array|
(cf. \ref{Face Arrays}). {\bf New:} Since |\Mname| is derived from 
|face_array<E>| face maps can be passed (by reference) to functions
with face array parameters. In particular, all LEDA graph algorithms 
expecting a |face_array<E>&| argument can be passed a |face_map<E>|
instead. }*/

public:


/*{\Mcreation M }*/

face_map() { base::enable_map_access(); }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it to the map with empty domain. }*/

face_map(const graph_t& G) : face_array<E,graph_t>(G) { base::enable_map_access(); }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it with a mapping $m$ from the set of all faces of $G$ into 
             the set of variables of type $E$. The variables in the range 
             of $m$ are initialized by a call of the default constructor 
             of type $E$. }*/

face_map(const graph_t& G, E x) : face_array<E,graph_t>(G,x) { base::enable_map_access(); }
/*{\Mcreate  introduces a variable |\Mvar| of type |\Mname| and initializes
             it with a mapping $m$ from the set of all faces of $G$ into 
             the set of variables of type $E$. The variables in the range 
             of $m$ are initialized with a copy of $x$. }*/

face_map(const face_map<E,graph_t>& A) : face_array<E,graph_t>(A) { base::enable_map_access(); }

face_map<E,graph_t>& operator=(const face_map<E,graph_t>& A) 
{ face_array<E,graph_t>::operator=(A); return *this; }

~face_map() {}


/*{\Moperations 1.3 4.3 }*/

const graph_t&  get_graph() const { return face_array<E,graph_t>::get_graph(); }
/*{\Mop  returns a reference to the graph of |\Mvar|. }*/
 

void init()  { graph_map<graph_t>::init(nil,0,0); }
/*{\Mop      makes |\Mvar| a face map with empty domain. }*/

void init(const graph_t& G) { face_array<E,graph_t>::init(G); }     
/*{\Mop       makes |\Mvar| a mapping $m$ from the set of all faces of $G$ 
              into the set of variables of type $E$. The variables in the 
              range of $m$ are initialized by a call of the default 
              constructor of type $E$. }*/

void init(const graph_t& G, E x) { face_array<E,graph_t>::init(G,x); }     
/*{\Mop       makes |\Mvar| a mapping $m$ from the set of all faces of $G$ 
              into the set of variables of type $E$. The variables in the 
              range of $m$ are initialized with a copy of $x$. }*/


const E& operator()(face f) const 
{  return LEDA_CONST_ACCESS(E,graph_map<graph_t>::map_read(f)); }

const E& operator[](face f) const 
{  return LEDA_CONST_ACCESS(E,graph_map<graph_t>::map_read(f)); }

E& operator[](face f)
{  return LEDA_ACCESS(E,graph_map<graph_t>::map_access(f)); }
/*{\Marrop    returns the variable $M(v)$. }*/

};

/*{\Mimplementation
Face maps are implemented by an efficient hashing method based on the 
internal numbering of the faces. An access operation takes expected 
time $O(1)$. 
}*/



#if LEDA_ROOT_INCL_ID == 600188
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
