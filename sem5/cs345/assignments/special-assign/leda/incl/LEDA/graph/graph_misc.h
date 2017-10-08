/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_misc.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GRAPH_MISC_H
#define LEDA_GRAPH_MISC_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600157
#include <LEDA/internal/PREAMBLE.h>
#endif




//-----------------------------------------------------------------------------
// miscellaneous  (should be members or constructors)
//-----------------------------------------------------------------------------

#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/graph/edge_array.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {graph_misc} {} {Miscellaneous Graph Functions} }*/

/*{\Moperations 2 5}*/

extern __exportF void CopyGraph(graph& H, const graph& G);
/*{\Mfunc  constructs a copy $H$ of graph $G$. }*/

extern __exportF void CopyGraph(GRAPH<node,edge>& H, const graph& G);
/*{\Mfunc  constructs a copy $H$ of graph $G$ such that |H[v]|
           is the node of $G$ that corresponds to |v| and |H[e]| is 
           the edge of $G$ that corresponds to |e|. }*/


extern __exportF void CopyGraph(GRAPH<node,edge>& H, const graph& G, 
                                                     const list<node>& V, 
                                                     const list<edge>& E);
/*{\Mfunc  constructs a copy $H$ of the subgraph $(V,E)$ of $G$ such that 
           |H[v]| is the node of $G$ that corresponds to |v| and |H[e]| is 
           the edge of $G$ that corresponds to |e|. \precond  $V$ is a subset
           of the nodes of $G$ and $E$ is a subset of $V\times V$. }*/

extern __exportF void CopyGraph(GRAPH<node,edge>& H, const graph& G, 
                                                     const list<edge>& E);
/*{\Mfunc  constructs a copy $H$ of the subgraph of $G$ induced by the
           edges in $E$. }*/



extern __exportF bool Is_Simple(const graph& G);
/*{\Mfunc  returns true if $G$ is simple, i.e., has no parallel
           edges, false otherwise. }*/

extern __exportF bool Is_Simple(const graph& G, list<edge>& el);
/*{\Mfunc  as above, but returns in addition the list of all edges
           sorted lexicographically by source and target node, i.e,
           all parallel edges appear consecutively in |el|. }*/  


extern __exportF bool Is_Loopfree(const graph& G);
/*{\Mfunc returns true if $G$ is loopfree, i.e., has no edge whose 
source is equal to its target.}*/

extern __exportF bool Is_Simple_Loopfree(const graph& G);
/*{\Mfunc returns true if $G$ is simple and loopfree. }*/

extern __exportF bool Is_Undirected_Simple(const graph& G);
/*{\Mfunc returns true if $G$ viewed as an undirected graph is simple,
          i.e., G is loopfree, simple, and has no anti-parallel edges.}*/

// for backward compatibility (s.n. april 2001)
inline bool Is_Simple_Undirected(const graph& G) 
{ return Is_Undirected_Simple(G); }

inline bool Is_Bidirected(const graph& G) { return G.is_bidirected(); }
/*{\Mfuncl      returns true if every edge has a reversal and 
                false otherwise. }*/

extern __exportF bool Is_Bidirected(const graph& G, edge_array<edge>& rev);
/*{\Mfuncl      computes for every edge $e = (v,w)$ in $G$ its
                reversal $rev[e] = (w,v)$ in $G$ (nil if
                not present). Returns true if every edge has a
                reversal and false otherwise. }*/

extern __exportF bool Is_Map(const graph& G);
/*{\Mfuncl      tests whether $G$ is a map. }*/ 

extern __exportF int Genus(const graph& G);
/*{\Mfunc computes the genus of $G$.\\ \precond $G$ must be a map. }*/

extern __exportF bool Is_Plane_Map(const graph& G);
/*{\Mfuncl      tests whether $G$ is a plane map, i.e, whether
$G$ is a map of genus zero. }*/ 

extern __exportF bool Is_Planar_Map(const graph& G);
/*{\Mfuncl      old name for Is\_Plane\_Map }*/ 


extern __exportF bool Is_Acyclic(const graph& G);
/*{\Mfunc  returns true if the directed $G$ is acyclic
           and false otherwise. }*/

extern __exportF bool Is_Acyclic(const graph& G, list<edge>& L);
/*{\Mfunc  as above; in addition, constructs a list of edges $L$ 
           whose deletion makes $G$ acyclic. }*/


extern __exportF bool Is_Connected(const graph& G);
/*{\Mfunc  returns true if the undirected graph underlying $G$ is connected
           and false otherwise. }*/


extern __exportF bool Is_Biconnected(const graph& G);
/*{\Mfunc  returns true if the undirected graph underlying $G$ is biconnected
           and false otherwise. }*/

extern __exportF bool Is_Biconnected(const graph& G, node& s);
/*{\Mfunc  as above, computes a split vertex $s$ if the result is |false|. }*/


extern __exportF bool Is_Triconnected(const graph& G);
/*{\Mfunc  returns true if the undirected graph underlying $G$ is triconnected
           and false otherwise. The running time is $O(n(n+m))$). }*/

extern __exportF bool Is_Triconnected(const graph& G, node& s1, node& s2);
/*{\Mfunc  as above, computes a split pair $s1,s2$ if the result is |false|. }*/


extern __exportF bool Is_Bipartite(const graph& G);
/*{\Mfuncl returns true if $G$ is bipartite and false otherwise. }*/

extern __exportF bool Is_Bipartite(const graph& G, list<node>& A, list<node>& B);
/*{\Mfuncl returns true if $G$ is bipartite and false otherwise. 
           If $G$ is bipartite the two sides are returned in $A$ and $B$,
           respectively. If $G$ is not bipartite the node sequence of an
           odd-length circle is returned in $A$.. }*/

extern __exportF bool Is_Planar(const graph& G);
/*{\Mfuncl returns true if $G$ is planar and false otherwise. }*/


extern __exportF bool Is_Series_Parallel(const graph& G);
/*{\Mfuncl returns true if $G$ is series-parallel and false otherwise. }*/




extern __exportF void Make_Acyclic(graph& G);
/*{\Mfunc  makes $G$ acyclic by removing all DFS back edges. }*/ 


extern __exportF list<edge> Make_Simple(graph& G);
/*{\Mfunc  makes $G$ simple by removing all but one from each set of
           parallel edges. Returns the list of remaining edges with 
           parallel edges in the original graph. }*/


extern __exportF void Make_Bidirected(graph& G,list<edge>& L);
/*{\Mfuncl      makes $G$ bidirected by inserting missing reversal
                edges. Appends all inserted edges to list $L$. }*/

extern __exportF list<edge> Make_Bidirected(graph& G);
/*{\Mfuncl      makes $G$ bidirected by inserting missing reversal
                edges. Returns the list of inserted edges. }*/


extern __exportF void Make_Connected(graph& G,list<edge>& L);
/*{\Mfuncl  makes $G$ connected; appends all inserted 
            edges to list $L$. }*/

extern __exportF list<edge> Make_Connected(graph& G);
/*{\Mfuncl  makes $G$ connected; returns the list of inserted edges. }*/



extern __exportF void Make_Biconnected(graph& G,list<edge>& L);
/*{\Mfuncl  makes $G$ biconnected; appends all inserted 
            edges to list $L$. }*/

extern __exportF list<edge> Make_Biconnected(graph& G);
/*{\Mfuncl  makes $G$ biconnected; returns the list of inserted edges. }*/


extern __exportF list<node> Delete_Loops(graph& G);
/*{\Mfunc returns the list of nodes with self-loops and deletes 
           all self-loops. }*/





// for historical reasons:

inline bool compute_correspondence(const graph& G, edge_array<edge>& rev)
{ return Is_Bidirected(G,rev); }

inline void eliminate_parallel_edges(graph& G) { Make_Simple(G); }







#if LEDA_ROOT_INCL_ID == 600157
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif







