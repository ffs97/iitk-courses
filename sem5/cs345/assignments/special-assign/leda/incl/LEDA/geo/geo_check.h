/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geo_check.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_GEO_CHECK_H
#define LEDA_GEO_CHECK_H
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_alg.h>
#include <LEDA/core/d_array.h>

LEDA_BEGIN_NAMESPACE
 

/*{\Mtext
\bigskip
$\bullet$ {\bf Properties of Geometric Graphs}
\medskip


We give procedures to check properties of geometric graph.
We give procedures to verify properties of \emph{geometric graph}.
A geometric graph is a straight-line embedded map. Every node 
is mapped to a point in the plane and every edge is mapped to the line segment
connecting its endpoints. 

We use |geo_graph| as a template parameter for geometric graph. Any
instantiation of |geo_graph| must provide a function

|VECTOR edge_vector(const geo_graph& G, const edge& e)|

that returns a vector  
from the source to the target of $e$. In order to use any of these
template functions the file /LEDA/geo/generic/geo\_check.h must be included.

\settowidth{\typewidth}{|string|}
\addtolength{\typewidth}{2\colsep}
\setlength{\callwidth}{2cm}
\computewidths
}*/

template <class geo_graph>
bool Is_CCW_Ordered(const geo_graph& G);
/*{\Mfunc returns true if for all nodes $v$ the neighbors of $v$ are in
increasing counter-clockwise order around $v$.}*/

template <class geo_graph>
bool Is_CCW_Ordered_Plane_Map(const geo_graph& G);
/*{\Mfunc Equivalent to |Is_Plane_Map(G)| and |Is_CCW_Ordered(G)|.}*/

template <class geo_graph>
void SORT_EDGES(geo_graph& G);
/*{\Mfunc Reorders the edges of $G$ such that for every node $v$ the 
edges in $A(v)$ are in non-decreasing order by angle.}*/



template <class geo_graph>
bool Is_CCW_Convex_Face_Cycle(const geo_graph& G, const edge& e);
/*{\Mfunc returns true if the face cycle of $G$ containing $e$
defines a counter-clockwise convex polygon, i.e, if the face cycle forms a 
cyclically increasing sequence of edges according to the compare-by-angles
ordering. }*/

template <class geo_graph>
bool Is_CCW_Weakly_Convex_Face_Cycle(const geo_graph& G, const edge& e);
/*{\Mfunc returns true if the face cycle of $G$ containing $e$
defines a counter-clockwise weakly
convex polygon, i.e, if the face cycle forms a 
cyclically non-decreasing sequence of edges according to the compare-by-angles
ordering. }*/

template <class geo_graph>
bool Is_CW_Convex_Face_Cycle(const geo_graph& G, const edge& e);
/*{\Mfunc returns true if the face cycle of $G$ containing $e$
defines a clockwise convex polygon, i.e, if the face cycle forms a 
cyclically decreasing sequence of edges according to the compare-by-angles
ordering. }*/

template <class geo_graph>
bool Is_CW_Weakly_Convex_Face_Cycle(const geo_graph& G, const edge& e);
/*{\Mfunc returns true if the face cycle of $G$ containing $e$
defines a clockwise weakly
convex polygon, i.e, if the face cycle forms a 
cyclically non-increasing sequence of edges according to the compare-by-angles
ordering. }*/


bool Is_Convex_Subdivision(const GRAPH<rat_point,int>& G);
/*{\Mfunc returns true if $G$ is a convex planar subdivision.}*/

bool Is_Convex_Subdivision(const GRAPH<point,int>& G);


bool Is_Triangulation(const GRAPH<rat_point,int>& G);
/*{\Mfunc returns true if $G$ is convex planar subdivision in which every
bounded face is a triangle or if all nodes of $G$ lie on a common line.}*/

bool Is_Triangulation(const GRAPH<point,int>& G);

/*{\Mtext The next tests check Delaunay triangulations and 
Voronoi diagrams of point sites. These diagrams come in two 
kinds: \emph{nearest} or \emph{furthest}. An appropriate enumeration type
|delaunay_voronoi_kind| with members |NEAREST| and |FURTHEST| is 
defined. }*/

bool Is_Delaunay_Triangulation(const GRAPH<rat_point,int>& G, 
                               delaunay_voronoi_kind kind);
/*{\Mfunc checks whether $G$ is a nearest (|kind = NEAREST|)
 or furthest (|kind = FURTHEST|)  site Delaunay triangulation of
its vertex set. $G$ is a Delaunay triangulation iff it is a 
triangulation and 
all triangles have the Delaunay property. A triangle has the 
Delaunay property if
no vertex of an adjacent triangle is contained in the 
interior (|kind = NEAREST|)
or exterior (|kind = FURTHEST|) of the triangle. }*/

bool Is_Delaunay_Triangulation(const GRAPH<point,int>& G, 
                               delaunay_voronoi_kind kind);


bool Is_Delaunay_Diagram(const GRAPH<rat_point,int>& G, 
                         delaunay_voronoi_kind kind);
/*{\Mfunc checks whether $G$ is a nearest (|kind = NEAREST|)
 or furthest (|kind = FURTHEST|)  site Delaunay diagram of
its vertex set. $G$ is a Delaunay diagram if it is a convex subdivision, 
if the vertices
of any bounded face are 
co-circular, and if every triangulation of $G$ is a Delaunay
triangulation.}*/

bool Is_Delaunay_Diagram(const GRAPH<point,int>& G, 
                         delaunay_voronoi_kind kind);


bool Is_Voronoi_Diagram(const GRAPH<rat_circle,rat_point>& G, 
                        delaunay_voronoi_kind kind);
/*{\Mfunc checks whether $G$ represents a nearest (|kind = NEAREST|)
 or furthest (|kind = FURTHEST|) site Voronoi diagram. 

Voronoi diagrams of point sites are represented as 
planar maps as follows: There is a vertex for each vertex of the 
Voronoi diagram
and, in addition, a vertex ``at infinity'' for each ray of the Voronoi diagram.
Vertices at infinity have degree one. The edges of the graph correspond to the
edges of the Voronoi diagram. Figure
\ref{Voronoi diagram} shows some examples and the chapter on Voronoi 
diagrams of the 
LEDA-book \cite{LEDAbook} contains more details.
Each edge is labeled with the site (class |POINT|)
owning the region to its left and each vertex is labeled with a
triple of points (= the three defining points of a |CIRCLE|). For 
a ``finite'' vertex
the three points are any three sites associated with regions incident 
to the vertex
(and hence the center of the circle is the position of the vertex in the plane)
and for a vertex at infinity the three points are collinear and the 
first point 
and the third point of the triple are the sites whose regions are 
incident to the
vertex at infinity. Let $a$ and $c$ be the first and third point of the 
triple respectively; $a$ and
$c$ encode the geometric position of the vertex at infinity as follows:
the vertex lies
on the perpendicular bisector of $a$ and $c$ and to the left of the 
segment $ac$.
\vspace{1ex}

}*/

bool Is_Voronoi_Diagram(const GRAPH<circle,point>& G, 
                        delaunay_voronoi_kind kind);



LEDA_END_NAMESPACE

#endif

