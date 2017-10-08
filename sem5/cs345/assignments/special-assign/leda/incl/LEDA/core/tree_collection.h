/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  tree_collection.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_TREE_COLLECTION_H
#define LEDA_TREE_COLLECTION_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600039
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

 /*********************************************************************
 *                                                                     *
 *  dyna_trees T; deklariert eine (vorerst leere) Menge von dyn_trees  *
 *                                                                     *
 *  d_vertex T.maketree(); Erzeugt einen Baum, der genau einen Knoten  *
 *     enthaelt (der in keinen anderem Baum enthalten ist.             *
 *                                                                     *
 *  d_vertex T.findroot(d_vertex v); Gibt die Wurzel des v enthaltenden*
 *     Baumes zurueck.                                                 *
 *                                                                     *  
 *  d_vertex T.findcost(d_vertex v, double& d); Gibt den entsprechenden*
 *     Knoten zurueck (s.o.), die Kosten werden im Argument d zurueck- *
 *     gegeben.                                                        *
 *                                                                     *
 *  void   T.addcost(d_vertex v, double x); s.o.                       *
 *                                                                     *
 *  void   T.link(d_vertex v, d_vertex w); s.o.                        *
 *                                                                     *
 *  void   T.cut(d_vertex v); s.o.                                     *
 *                                                                     *
 *                                                                     *
 * Implemented by Jan Timm                                             *
 *                                                                     *
 *********************************************************************/





class __exportC d_node;
typedef d_node *d_vertex;
typedef d_node *d_path;

class __exportC d_node {

friend class __exportC dyna_trees;

     void*    info;        // user-defined information

     d_vertex left,        // linkes Kind
              right,       // rechtes Kind
              parent,      // Elter
              successor,   // Nachfolger (fuer Pfad)
              next;        // fuer Verkettung fuer ~dyna_tree

     double dcost,
            dmin;

     d_node(void* i) { 
                       left=right=parent=successor=next=0;
                       dcost=dmin=0;
                       info = i;
                      };
   LEDA_MEMORY(d_node)

};
  
   
class __exportC dyna_trees {
     d_vertex first,
            last;  // diese beiden Zeiger fuer ~dyna_tree

     void splay(d_vertex);

     d_vertex assemble(d_vertex, d_vertex, d_vertex);
     void   disassemble(d_vertex, d_vertex&, d_vertex&);

     d_vertex makepath(void*);
     d_vertex findpath(d_vertex);
     d_vertex findpathcost(d_path, double&);
     d_vertex findtail(d_path);
     void   addpathcost(d_path, double);
     d_vertex join(d_path, d_path, d_path);
     void   split(d_vertex, d_vertex&, d_vertex&);
     
     d_path   expose(d_vertex);

virtual void copy_inf(GenPtr& x)      { x=x; }
virtual void clear_inf(GenPtr& x)     { x=0; }

public:
     dyna_trees() { first=last=0; };
     virtual ~dyna_trees();

     const GenPtr& inf(d_vertex v) { return v->info; }
       
     d_vertex maketree(void*);
     d_vertex findroot(d_vertex);
     d_vertex findcost(d_vertex, double&);
     d_vertex parent(d_vertex v) { return v->successor; }

     void   addcost(d_vertex, double);
     void   link(d_vertex, d_vertex);
     void   cut(d_vertex);

};


/*{\Manpage {tree_collection} {I} {Dynamic Collections of Trees}}*/

template<class I>

class  tree_collection : private dyna_trees{

/*{\Mdefinition
An instance $D$ of the parameterized data type |\Mname| is a
collection of vertex disjoint rooted trees, each of whose vertices has a
double-valued cost and contains an information of type $I$, called the
information type of $D$.}*/

void copy_inf(GenPtr& x)      { LEDA_COPY(I,x); }
void clear_inf(GenPtr& x)     { LEDA_CLEAR(I,x);  }

public:

/*{\Mcreation D }*/ 

tree_collection() {}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname|, initialized with the
            empty collection.}*/

~tree_collection() {}


/*{\Moperations 2 5}*/

d_vertex maketree(const I& x)  { return dyna_trees::maketree(leda_cast(x)); }
/*{\Mop       adds a new tree to $D$ containing a single
	      vertex $v$ with cost zero and information $x$,
	      and returns $v$.}*/

const I& inf(d_vertex v) { return LEDA_CONST_ACCESS(I,dyna_trees::inf(v)); }
/*{\Mop       returns the information of vertex $v$.}*/

d_vertex findroot(d_vertex v) { return dyna_trees::findroot(v); }
/*{\Mop       returns the root of the tree containing $v$.}*/

d_vertex findcost(d_vertex v, double& x) { return dyna_trees::findcost(v,x); }
/*{\Mopl      sets $x$ to the minimum cost of a vertex on the
	      tree path from $v$ to findroot($v$) and returns
	      the last vertex (closest to the root) on this
	      path of cost $x$.}*/

d_vertex parent(d_vertex v) { return dyna_trees::parent(v); }
/*{\Mop       returns the parent vertex of $v$. }*/

void addcost(d_vertex v, double x) { dyna_trees::addcost(v,x); }
/*{\Mopl     adds double number $x$ to the cost of every vertex
	     on the tree path from $v$ to findroot($v$).}*/

void link(d_vertex v, d_vertex x) { dyna_trees::link(v,x); }
/*{\Mopl     combines the trees containing vertices $v$ and $w$
	     by adding the edge $(v,w)$. (We regard tree
	     edges as directed from child to parent.)\\
	     \precond $v$ and $w$ are in different trees
	     and $v$ is a root.}*/

void cut(d_vertex v) {dyna_trees::cut(v); }
/*{\Mop      divides the tree containing vertex $v$ into 
	     two trees by deleting the edge out of $v$.\\
	     \precond $v$ is not a tree root.}*/


};

/*{\Mimplementation
Dynamic collections of trees are implemented by partitioning the trees
into vertex disjoint paths and representing each path by a self-adjusting
binary tree (see \cite{T83}). All operations take amortized time $O(\log n)$
where $n$ is the number of maketree operations.}*/


#if LEDA_ROOT_INCL_ID == 600039
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

