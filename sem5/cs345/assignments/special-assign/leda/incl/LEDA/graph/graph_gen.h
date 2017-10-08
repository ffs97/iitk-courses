/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_gen.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GRAPH_GEN_H
#define LEDA_GRAPH_GEN_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600213
#include <LEDA/internal/PREAMBLE.h>
#endif


//-----------------------------------------------------------------------------
// graph generators
//-----------------------------------------------------------------------------

#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_array.h>

LEDA_BEGIN_NAMESPACE


/*{\Manpage {graph_gen} {} {Graph Generators} }*/

extern __exportF void complete_graph(graph& G, int n, bool dir);

inline void complete_graph(graph& G, int n) 
{ complete_graph(G,n,G.is_directed()); }
/*{\Mfuncl   creates a complete graph $G$ with $n$ nodes. }*/

inline void complete_ugraph(graph& G, int n) 
{ complete_graph(G,n,false); }
/*{\Mfuncl   creates a complete undirected graph $G$ with $n$ nodes. }*/

extern __exportF void random_graph_noncompact(graph& G, int n, int m);
/*{\Mfunc generates a random graph with $n$ nodes and $m$ edges. No attempt is
made to store all edges in the same adjacency list consecutively. This
function is only included for pedagogical reasons. }*/


extern __exportF void random_graph_compact(graph& G, int n, int m,
                          bool no_anti_parallel_edges,
                          bool loopfree,
                          bool no_parallel_edges);

inline void random_graph(graph& G, int n, int m,
                          bool no_anti_parallel_edges,
                          bool loopfree,
                          bool no_parallel_edges)
{ random_graph_compact(G,n,m,no_anti_parallel_edges,
                         loopfree,no_parallel_edges); }
/*{\Mfunc generates a random graph with $n$ nodes and $m$ edges. 
All edges in the same adjacency list are stored consecutively.\\
If |no_parallel_edges| is true then no parallel edges are generated, if
|loopfree| is true then no self loops are generated, and if
|no_anti_parallel_edges| is true then no anti parallel edges are generated.
}*/

inline void random_graph(graph& G, int n, int m)
{ random_graph_compact(G,n,m,false,false,false); }
/*{\Mfunc same as |random_graph(G,n,m,false,false,false)|. }*/


inline void random_simple_graph(graph& G,int n, int m)
{ random_graph_compact(G,n,m,false,false,true); }
/*{\Mfunc same as |random_graph(G,n,m,false,false,true)|. }*/

inline void random_simple_loopfree_graph(graph& G,int n, int m)
{ random_graph_compact(G,n,m,false,true,true); }
/*{\Mfunc same as |random_graph(G,n,m,false,true,true)|. }*/

inline void random_simple_undirected_graph(graph& G,int n, int m)
{ random_graph_compact(G,n,m,true,true,true); }
/*{\Mfunc same as |random_graph(G,n,m,true,true,true)|. }*/


extern __exportF void random_graph(graph& G, int n, double p);
/*{\Mfunc generates a random graph with $n$ nodes. Each edge
of the complete graph with $n$ nodes is included with probability $p$ .}*/

extern __exportF void test_ugraph(ugraph& G);

extern __exportF void test_graph(graph& G);
/*{\Mfuncl   creates interactively a user defined graph $G$. }*/


extern __exportF void complete_bigraph(graph& G, int a, int b, list<node>& A, list<node>& B);
/*{\Mfuncl   creates a complete bipartite graph $G$ with $a$ nodes 
            on side $A$ and $b$ nodes on side $B$. All edges are 
            directed from $A$ to $B$. }*/

extern __exportF void random_bigraph(graph& G, int a, int b, int m,
            list<node>& A, list<node>& B, int k = 1);
/*{\Mfunc creates a random bipartite graph $G$ with $a$ nodes on 
   side $A$, $b$ nodes on side $B$, and $m$ edges. All 
   edges are directed from $A$ to $B$.\\
If $k > 1$ then $A$ and $B$ are divided into
$k$ groups of about equal size and the nodes in the $i$-th group of $A$ 
have their edges to nodes in the $i-1$-th and $i+1$-th group in $B$. All 
indices are modulo $k$. }*/


extern __exportF void test_bigraph(graph& G, list<node>& A, list<node>& B);
/*{\Mfuncl   creates interactively a user defined bipartite
            graph $G$ with sides $A$ and $B$. All edges are
            directed from $A$ to $B$. }*/


extern __exportF void grid_graph(graph& G,int n);
/*{\Mfuncl   creates a grid graph $G$ with $n\times n$ nodes. }*/


extern __exportF void grid_graph(graph& G, node_array<double>& xcoord, 
                                           node_array<double>& ycoord, int n);
/*{\Mfuncl   creates a grid graph $G$ of size $n\times n$ embedded into the 
             unit square. The embedding is given by $xcoord[v]$ and $ycoord[v]$ 
             for every node $v$ of $G$. }*/


extern __exportF void d3_grid_graph(graph& G,int n);
/*{\Mfuncl   creates a three-dimensional grid graph $G$ with 
             $n\times n\times n$ nodes. }*/


extern __exportF void d3_grid_graph(graph& G,node_array<double>& xcoord, 
                                             node_array<double>& ycoord,
                                             node_array<double>& zcoord,int n);
/*{\Mfuncl   creates a three-dimensional grid graph $G$ of size 
             $n\times n\times n$ embedded into the unit cube. The embedding 
             is given by $xcoord[v]$, $ycoord[v]$, and $zcoord[v]$
             for every node $v$ of $G$. }*/


extern __exportF void cmdline_graph(graph& G, int argc, char** argv);
/*{\Mfuncl   builds graph $G$ as specified by the command line arguments:
\begin{tabular}{llll}
prog & & $\longrightarrow$ & test\_graph() \\ 
prog & $n$ & $\longrightarrow$ & complete\_graph($n$) \\ 
prog & $n$ $m$ & $\longrightarrow$ & test\_graph($n,m$) \\ 
prog & $file$ & $\longrightarrow$ & $G$.read\_graph($file$) 
\end{tabular}
}*/


/*{\Mtext
\headerline{Planar graph: Combinatorial Constructions}
A maximal planar map with $n$ nodes, $n \ge 3$, has $3n-6$ uedges. It is constructed iteratively. For $n = 1$, the graph consists of a single
 isolated node, for $n = 2$, the graph consists of two nodes and one uedge,
for $n = 3$ the graph consists of three nodes and three uedges. For $n > 3$,
a random maximal planar map with $n - 1$ nodes is constructed first and then 
an additional node is put into a random face. 

The generator with the additional parameter $m$ first generates a 
maximal planar map and then deletes all but $m$ edges. 

The generators with the word map replaced by graph, first generate a map 
and then delete one edge from each uedge.}*/

extern __exportF void maximal_planar_map(graph& G, int n);
/*{\Mfuncl   creates a maximal planar map $G$  with $n$ nodes. }*/

extern __exportF void random_planar_map(graph& G, int n, int m);
/*{\Mfuncl   creates a random planar map $G$ with $n$ nodes and $m$ edges. }*/


extern __exportF void maximal_planar_graph(graph& G, int n);
/*{\Mfuncl   creates a maximal planar graph $G$ with $n$ nodes. }*/

extern __exportF void random_planar_graph(graph& G, int n, int m);
/*{\Mfuncl   creates a random planar graph $G$ with $n$ nodes and 
$m$ edges. }*/

/*{\Mtext
\headerline{Planar graph: Geometric Constructions}

We have two kinds of geometric constructions: triangulations of point sets
and intersection graph of line segments. The functions |triangulation_map|
choose points in the unit square and compute a triangulation of them and
the functions |random_planar_graph| construct the intersection graph of
segments. 



The generators with the word map replaced by graph, first generate a map 
and then delete one edge from each uedge.}*/



extern __exportF void triangulation_map(graph& G, 
                       node_array<double>& xcoord, 
                       node_array<double>& ycoord, 
                       int n);
/*{\Mfuncl  chooses $n$ random points in the unit square and returns their
triangulation as a plane map in $G$. The coordinates of node $v$ are returned
as |xcoord[v]| and |ycoord[v]|. The coordinates are random number of the 
form $x/K$ where $K = 2^{20}$ and $x$ is a random integer between 0
(inclusive) and $K$ (exclusive).}*/

extern __exportF void triangulation_map(graph& G, 
                       list<node>& outer_face,
                       node_array<double>& xcoord, 
                       node_array<double>& ycoord, 
                       int n);
/*{\Mfuncl  as above, in addition the list of nodes of the outer face 
            ( convex hull) is returned in |outer_face| in clockwise order. }*/

extern __exportF void triangulation_map(graph& G, int n);
/*{\Mfuncl  as above, but only the map is returned. }*/

extern __exportF void random_planar_map(graph& G, 
                       node_array<double>& xcoord, 
                       node_array<double>& ycoord, 
                       int n, int m);
/*{\Mfuncl  chooses $n$ random points in the unit square and computes their
triangulation as a plane map in $G$. It then keeps all but 
$m$ uedges.
The coordinates of node $v$ are returned
as |xcoord[v]| and |ycoord[v]|.
}*/

extern __exportF void triangulation_graph(graph& G, 
                             node_array<double>& xcoord, 
                             node_array<double>& ycoord, 
                             int n);
/*{\Mfuncl  calls  |triangulation_map| and keeps only one of the 
edges comprising a uedge.
}*/

extern __exportF void triangulation_graph(graph& G, 
                         list<node>& outer_face,
                         node_array<double>& xcoord, 
                         node_array<double>& ycoord, 
                         int n);
/*{\Mfuncl  calls  |triangulation_map| and keeps only one of the 
edges comprising a uedge.
}*/

extern __exportF void triangulation_graph(graph& G, int n);
/*{\Mfuncl  calls  |triangulation_map| and keeps only one of the 
edges comprising a uedge.
}*/

extern __exportF void random_planar_graph(graph& G, 
                       node_array<double>& xcoord, 
                       node_array<double>& ycoord, 
                       int n, int m);
/*{\Mfuncl  calls  |random_planar_map| and keeps only one of the 
edges comprising a uedge.
}*/



extern __exportF void triangulated_planar_graph(graph& G, int n);
/*{\Mfuncl   old name for |triangulation_graph|. }*/


extern __exportF void triangulated_planar_graph(graph& G, node_array<double>& xcoord, 
                                                   node_array<double>& ycoord, 
                                                   int n);
/*{\Mfuncl  old name for |triangulation_graph|.}*/

extern __exportF void triangulated_planar_graph(graph& G, list<node>& outer_face,
                                                   node_array<double>& xcoord, 
                                                   node_array<double>& ycoord, 
                                                   int n);
/*{\Mfuncl old name for |triangulation_graph|.  }*/






extern __exportF void random_planar_graph(graph& G, 
                           node_array<double>& xcoord,                                                     node_array<double>& ycoord,                                                     int n);
/*{\Mfuncl   creates a random planar graph $G$ with $n$ nodes embedded into
            the unit sqare. The embedding is given by $xcoord[v]$ and
            $ycoord[v]$ for every node $v$ of $G$. The generator chooses
            $n$ segments whose endpoints have random coordinates of the
            form $x/K$, where $K$ is the smallest power of two greater or
equal to $n$, and $x$ is a random integer in $0$ to $K -1$. It then constructs
the arrangement defined by the segments and keeps the $n$ nodes with the
smallest $x$-coordinates. Finally, it adds edges to make the graph connected.  }*/

extern __exportF void random_planar_graph(graph& G, int n);
/*{\Mfuncl   creates a random planar graph $G$ with $n$ nodes. Uses the preceding function. }*/


/*{\Mtext
\headerline{Series-Parallel Graphs}
}*/


extern __exportF void random_sp_graph(graph&G, int n, int m);
/*{\Mfuncl   creates a random series-parallel graph $G$ with $n$ nodes
             and $m$ edges. }*/





#if LEDA_ROOT_INCL_ID == 600213
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif



LEDA_END_NAMESPACE

#endif

