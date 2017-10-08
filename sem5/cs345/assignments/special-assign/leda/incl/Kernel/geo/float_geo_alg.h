#ifndef LEDA_FLOAT_GEO_ALG_H
#define LEDA_FLOAT_GEO_ALG_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600002
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/basic.h>
#include <LEDA/geo/geo_global_enums.h>
#include <LEDA/core/list.h>
#include <LEDA/graph/graph.h>
#include <LEDA/geo/float_kernel.h>

/*
#include <LEDA/graph/graph_alg.h>
#include <LEDA/core/d_array.h>
*/

LEDA_BEGIN_NAMESPACE

extern __exportF void SWEEP_SEGMENTS(const list<segment>& S, 
                                     GRAPH<point,segment>& G, 
                                     bool embed=false,
                                     bool use_optimization = true);

extern __exportF void SWEEP_SEGMENTS(const list<segment>&, list<point>&);


extern __exportF node Nesting_Tree(const gen_polygon& P, GRAPH<polygon, int>& T);

extern __exportF list<point> CONVEX_HULL(const list<point>& L);

extern __exportF edge TRIANGULATE_POINTS(const list<point>& L, GRAPH<point,int>& T);

extern __exportF int DELAUNAY_FLIPPING(GRAPH<point,segment>&);

extern __exportF int DELAUNAY_FLIP(const list<point>&,GRAPH<point,int>&);

extern __exportF int F_DELAUNAY_FLIP(const list<point>&,GRAPH<point,int>&);

extern __exportF bool Is_Triangulation(const GRAPH<point,int>& G);

extern __exportF bool Is_Delaunay_Triangulation(const GRAPH<point,int>& G, 
                               delaunay_voronoi_kind kind);

extern __exportF bool Is_Convex_Subdivision(const GRAPH<point,int>& G);

extern __exportF list<polygon> CONVEX_COMPONENTS(const gen_polygon& GP);

extern __exportF edge TRIANGULATE_PLANE_MAP(GRAPH<point,segment>& G);

LEDA_END_NAMESPACE


#if LEDA_ROOT_INCL_ID == 600002
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif

