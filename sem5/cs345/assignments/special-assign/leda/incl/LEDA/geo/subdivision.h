/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  subdivision.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SUBDIVISION_H
#define LEDA_SUBDIVISION_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600124
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/point.h>
#include <LEDA/geo/segment.h>
#include <LEDA/graph/planar_map.h>

LEDA_BEGIN_NAMESPACE

class __exportC SubDivision : public planar_map
{
  edge outer_edge;

  void* strip_ptr;   //pointer to strip_list

void print_node_entry(ostream& o, GenPtr& x)  const
     { o << "("; LEDA_PRINT(point,x,o); o << ")"; }
  
public:
  
  SubDivision(const graph&);

 ~SubDivision();

  const point& position(node v) const { return LEDA_CONST_ACCESS(point,inf(v));}

  segment      edge_seg(edge e) const 
  { return segment(position(source(e)),position(target(e))); }
  
  face   locate_point(point) const;
  void   print_stripes() const;
  
  face outer_face() const { return adj_face(outer_edge); }
};


//------------------------------------------------------------------------------
//
// subdivision: generic subdivisions with face entries of type "I"
//
//------------------------------------------------------------------------------

/*{\Manpage {subdivision} {I} {Planar Subdivisions}}*/

template <class I>

class subdivision : public SubDivision {

/*{\Mdefinition
An instance $S$ of the parameterized data type |\Mname| is a
subdivision of the two-dimensional plane, i.e., an embedded planar graph
with straight line edges (see also sections \ref{Planar Maps} and
\ref{Parameterized Planar Maps}). With each node
$v$ of $S$ is associated a point, called the position of $v$ and with each
face of $S$ is associated an information from data type $I$, called the
information type of $S$.}*/

void copy_face_entry(GenPtr& x)  const { LEDA_COPY(I,x); }
void clear_face_entry(GenPtr& x) const { LEDA_CLEAR(I,x);  }

public:

/*{\Mcreation S }*/
   subdivision(GRAPH<point,I>& G) : SubDivision(G)   
   { face f;
     forall_faces(f,*this) 
        graph::entry(f) = graph::entry(first_face_edge(f));
    }

/*{\Mcreate 
creates an instance |\Mvar| of type |\Mname| and initializes it to
the subdivision represented by the parameterized directed graph $G$.
The node entries of $G$ (of type point)  define the positions of the
corresponding nodes of |\Mvar|. Every face $f$ of |\Mvar| is assigned the
information of one of its bounding edges in $G$.\\
\precond $G$ represents
a planar subdivision, i.e., a straight line embedded planar map.}*/

  ~subdivision()     { clear(); }


/*{\Moperations 2 5.0}*/

point position(node v) const {return SubDivision::position(v);}
/*{\Mop       returns the position of node $v$.}*/

const I& inf(face f) const {return LEDA_CONST_ACCESS(I,SubDivision::inf(f));}
/*{\Mop       returns the information of face $f$.}*/

face locate_point(point p) const { return SubDivision::locate_point(p);}
/*{\Mop       returns the face containing point $p$.}*/

face outer_face() const { return SubDivision::outer_face(); }
/*{\Mop       returns the outer face of |\Mvar|.}*/

const point& operator[](node v) const 
{ return LEDA_CONST_ACCESS(point,SubDivision::inf(v)); }

const I& operator[](face f) const 
{ return LEDA_CONST_ACCESS(I,SubDivision::inf(f)); }

I& operator[](face f) 
{ return LEDA_ACCESS(I,graph::entry(f)); }

};

/*{\Mimplementation
Planar subdivisions are implemented by parameterized planar maps and an
additional data structure for point location based on partially persistent 
search trees\cite{DSST89}. Operations position and inf take constant time, a 
locate\_point operation takes (expected) time $O(\log n)$. Here $n$ is the 
number of nodes. 
The space requirement is $O(n+m)$ and the initialization time is 
$O(n + m \log m)$, where $m$ is the number of edges in the map.}*/


#if LEDA_ROOT_INCL_ID == 600124
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
