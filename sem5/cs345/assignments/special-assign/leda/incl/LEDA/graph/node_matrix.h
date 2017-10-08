/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  node_matrix.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_NODE_MATRIX_H
#define LEDA_NODE_MATRIX_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600208
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {node_matrix} {E} {Two Dimensional Node Arrays}}*/

template<class E, class graph_t = graph>
class node_matrix {

typedef typename graph_t::node node;
typedef typename graph_t::edge edge;


/*{\Mdefinition
An instance $M$ of the parameterized data type |\Mname| is
a partial mapping from the set of node pairs $V\times V$
of a graph to the set of variables of data type $E$, called the element type
of $M$. The domain $I$ of $M$ is called the index set of $M$. $M$ is said to
be valid for all node pairs in $I$. A node matrix can also be viewed as a
node array with element type $node\_array\<E\>$ ($node\_array\<node\_array\<E\>\ \>$).
}*/


node_array<node_array<E,graph_t>,graph_t> M;

public:

/*{\Mcreation M }*/

node_matrix() {}
/*{\Mcreate creates an instance $M$ of type |\Mname| and initializes the index 
            set of $M$ to the empty set. }*/

node_matrix(const graph_t& G)  { init(G);   }
/*{\Mcreate creates an instance $M$ of type |\Mname| and initializes the index 
            set to be the set of all node pairs of graph $G$, i.e., $M$ is made 
            valid for all pairs in $V \times V$ where $V$ is the set of nodes 
            currently contained in $G$.
}*/

node_matrix(const graph_t& G, E x) { init(G,x); }
/*{\Mcreate creates an instance $M$ of type |\Mname| and initializes the index 
            set of $M$ to be the set of all node pairs of graph $G$, i.e., $M$ 
            is made valid for all pairs in $V \times V$ where $V$ is the set of 
            nodes currently contained in $G$.  In addition, $M(v,w)$ is 
            initialized with $x$ for all 
nodes $v,w \in V$.}*/

 node_matrix(const node_matrix<E>& mat) : M(mat.M) {}

 node_matrix<E>& operator=(const node_matrix<E>& mat) 
{ M = mat.M; return *this; }

~node_matrix() {}


/*{\Moperations 3.2 3.7}*/

void  init(const graph_t& G)
{ M.init(G); 
  node v;
  forall_nodes(v,G) M[v].init(G);
}
/*{\Mop       sets the index set of $M$ to $V\times V$, where  
	      $V$ is the set of all nodes of $G$. }*/


void  init(const graph_t& G, E x)
{ M.init(G); 
  node v;
  forall_nodes(v,G) M[v].init(G,x);
}
/*{\Mopl      sets the index set of $M$ to $V\times V$, where
	      $V$ is the set of all nodes of $G$ and initializes
	      $M(v,w)$ to $x$ for all $v,w \in V$.}*/


/*{\Moptions nextwarning=no }*/
const node_array<E>& operator[](node v) const { return M[v]; }
      node_array<E>& operator[](node v)       { return M[v]; }
/*{\Marrop    returns the node\_array  $M(v)$.}*/


/*{\Moptions nextwarning=no }*/
const E&  operator()(node v, node w) const { return M[v][w]; }
      E&  operator()(node v, node w)       { return M[v][w]; }
/*{\Mfunop    returns the variable $M(v,w)$.\\
	      \precond $M$ must be valid for $v$ and $w$.}*/

};

/*{\Mimplementation
Node matrices for a graph $G$ are implemented by vectors of node arrays and an 
internal numbering of the nodes of $G$. The access operation 
takes constant time, the init operation takes time $O(n^2)$, where $n$ is the 
number of nodes currently contained in $G$. The space requirement is $O(n^2)$.
Note that a node matrix is only valid for the nodes contained in $G$ at the 
moment of the matrix declaration or initialization ($init$). Access operations 
for later added nodes are not allowed.}*/



#if LEDA_ROOT_INCL_ID == 600208
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
