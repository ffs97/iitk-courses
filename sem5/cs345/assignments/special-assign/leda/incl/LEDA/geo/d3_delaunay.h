/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_delaunay.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/geo/d3_rat_point.h>
#include <LEDA/geo/d3_rat_sphere.h>
#include <LEDA/graph/graph.h>

#if defined(LEDA_NAMESPACE)
LEDA_BEGIN_NAMESPACE
#endif

/*{\Manpage {d3_delaunay} {} {3D Triangulation and Voronoi Diagram Algorithms} }*/

/*{\Mtext
\setopdims{1cm}{3cm}
}*/
extern __exportF 
void D3_TRIANG(const list<d3_rat_point>& L, GRAPH<d3_rat_point,int>& G);
/*{\Mfuncl  computes a triangulation |G| of the points in |L|. }*/

extern __exportF 
void D3_DELAUNAY(const list<d3_rat_point>& L, GRAPH<d3_rat_point,int>& G);
/*{\Mfuncl  computes a delaunay triangulation |G| of the points in |L|.}*/

extern __exportF 
void D3_VORONOI(const list<d3_rat_point>& L0, GRAPH<d3_rat_sphere,int>& G);
/*{\Mfuncl  computes the voronoi diagramm |G| of the points in |L|.}*/

extern __exportF 
void D3_VORONOI(const list<d3_rat_point>& L0, GRAPH<d3_rat_point,int>& G, bool flag);


#if defined(LEDA_NAMESPACE)
LEDA_END_NAMESPACE
#endif
