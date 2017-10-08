/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  face_array.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_FACE_ARRAY_H
#define LEDA_FACE_ARRAY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600193
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// face arrays
//------------------------------------------------------------------------------

#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE



/*{\Manpage {face_array} {E} {Face Arrays}}*/

template <class E, class graph_t=graph>
class face_array : public graph_map<graph_t> {

typedef typename graph_t::face face;


/*{\Mdefinition
An instance $A$ of the parameterized data type |\Mname|
is a partial mapping from the face set of a graph $G$ to the set of 
variables of type $E$, called the element type of the array. The domain 
$I$ of $A$ is called the index set of $A$ and $A(f)$ is called the element 
at position $f$. $A$ is said to be valid for all faces in $I$.
The array access operator |A[f]| checks its precondition 
($A$ must be valid for $f$). The check can be turned off by compiling 
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

const char* elem_type_name() const { return LEDA_TYPE_NAME(E); }


/*{\Mcreation A }*/

face_array() : graph_map<graph_t>(nil,0,2) { leda_init_default(def);  this->init_def_entry(); }
/*{\Mcreate creates an instance $A$ of type |\Mname| with empty
            index set.}*/

face_array(const graph_t& G) : graph_map<graph_t>(&G,G.max_face_index()+1,2)
{ leda_init_default(def);  this->init_def_entry(); this->init_table(); }
/*{\Mcreate creates an instance $A$ of type |\Mname| and initializes
            the index set of $A$ to the current face set of graph $G$. }*/


face_array(const graph_t& G, E x) : graph_map<graph_t>(&G,G.max_face_index()+1,2), def(x)
{ this->init_def_entry();  this->init_table(); }
/*{\Mcreate creates an instance $A$ of type |\Mname|, sets the
            index set of $A$ to the current face set of graph $G$ and 
            initializes $A(f)$ with $x$ for all faces $f$ of $G$. }*/


face_array(const graph_t& G, int n, E x) : graph_map<graph_t>(&G,n,2), def(x) 
{ this->init_def_entry();  this->init_table(); }
/*{\Mcreate creates an instance $A$ of type |\Mname| valid for
            up to $n$ faces of graph $G$ and initializes $A(f)$ with $x$ 
            for all faces $f$ of $G$.\\
	    \precond $n \ge \Labs{V}$.\\ $A$ is also
            valid for the next $n - \Labs{V}$ faces added to $G$.}*/


face_array(const face_array<E>& A) : graph_map<graph_t>(A),def(A.def) 
{ this->init_def_entry(); }

face_array<E>& operator=(const face_array<E>& A) 
{ this->clear_def_entry();
  graph_map<graph_t>::operator=(A);  
  def = A.def;
  this->init_def_entry();
  return *this;
 }

~face_array() { this->clear_table(); this->clear_def_entry(); }



/*{\Moperations 1.3 4.7 }*/

const graph_t&  get_graph() const
{ return *(graph_map<graph_t>::get_owner()); }
/*{\Mop  returns a reference to the graph of $A$. }*/
 
const E& operator[](face f) const
{ return LEDA_CONST_ACCESS(E,graph_map<graph_t>::array_read(f)); }

E& operator[](face f) {  return LEDA_ACCESS(E,graph_map<graph_t>::array_access(f)); }
/*{\Marrop    returns the variable $A(f)$.\\
	      \precond $A$ must be valid for $f$.}*/


void init()  { graph_map<graph_t>::init(nil,1,2); }

void init(const graph_t& G)      
{ graph_map<graph_t>::init(&G,G.max_face_index()+1,2); 
  this->init_table();
 }
/*{\Mop       sets the index set $I$ of $A$ to the face  
	      set of $G$, i.e., makes $A$ valid for all faces 
              of $G$.}*/

void init(const graph_t& G, E x) 
{ graph_map<graph_t>::init(&G,G.max_face_index()+1,2); 
  def = x;
  this->init_table();
}
/*{\Mop       makes $A$ valid for all faces of $G$
	      and sets $A(f) = x$ for all faces $f$ of $G$.}*/

void init(const graph_t& G, int n, E x) 
{ graph_map<graph_t>::init(&G,n,2); 
  def = x;
  this->init_table();
}
/*{\Mopl      makes $A$ valid for at most $n$ faces  of $G$ 
              and sets $A(f) = x$ for all faces $f$  of $G$.\\
              \precond {$n \ge \Labs{V}$}.\\ $A$ is also valid for 
              the next  $n - \Labs{V}$ faces added to $G$. }*/

bool use_face_data(const graph_t& G) 
{ int i = graph_map<graph_t>::init(&G,G.max_face_index()+1,2,true);
  this->init_table();
  return i >= 0;
}

bool use_face_data(const graph_t& G, E x) 
{ int i = graph_map<graph_t>::init(&G,G.max_face_index()+1,2,true); 
  def = x;
  this->init_table();
  return i > 0;
}
/*{\Mopl  use free data slots in the faces of $G$ (if available) for 
          storing the entries of $A$. The number of additional data 
          slots in the nodes and edges of a graph can be specified 
          in the |graph::graph(int n_slots, int e_slots)| constructor.
          The result is |true| if a free slot is available and |false|
          otherwise. }*/


/*{\Mimplementation
Node arrays for a graph $G$ are implemented by \CC vectors and an
internal numbering of the faces and edges of $G$. The access operation
takes constant time, $init$ takes time $O(n)$, where $n$ is the number of
faces in $G$. The space requirement is $O(n)$.
    
{\bf Remark}: A face array is only valid for a bounded number of
faces of $G$. This number is either the number
of faces of $G$ at the moment of creation of the array or it is explicitely 
set by the user. Dynamic face arrays can be realized by face 
maps (cf. section \ref{Node Maps}). }*/

};



#if LEDA_ROOT_INCL_ID == 600193
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
