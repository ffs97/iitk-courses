#ifndef LEDA_RAT_GEO_ALG_H
#define LEDA_RAT_GEO_ALG_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600001
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/basic.h>
#include <LEDA/geo/geo_global_enums.h>
#include <LEDA/core/list.h>
#include <LEDA/graph/graph.h>
#include <LEDA/geo/rat_kernel.h>

LEDA_BEGIN_NAMESPACE

extern __exportF void SWEEP_SEGMENTS(const list<rat_segment>& S, 
                                     GRAPH<rat_point,rat_segment>& G, 
                                     bool embed=false,
                                     bool use_optimization = true);

extern __exportF void SWEEP_SEGMENTS(const list<rat_segment>&, list<rat_point>&);


extern __exportF node Nesting_Tree(const rat_gen_polygon& P, GRAPH<rat_polygon, int>& T);

extern __exportF list<rat_point> CONVEX_HULL(const list<rat_point>& L);

extern __exportF edge TRIANGULATE_POINTS(const list<rat_point>& L, GRAPH<rat_point,int>& T);

extern __exportF int DELAUNAY_FLIPPING(GRAPH<rat_point,rat_segment>&);

extern __exportF int DELAUNAY_FLIP(const list<rat_point>&,GRAPH<rat_point,int>&);

extern __exportF int F_DELAUNAY_FLIP(const list<rat_point>&,GRAPH<rat_point,int>&);

extern __exportF bool Is_Triangulation(const GRAPH<rat_point,int>& G);

extern __exportF bool Is_Delaunay_Triangulation(const GRAPH<rat_point,int>& G, 
                               delaunay_voronoi_kind kind);

extern __exportF bool Is_Convex_Subdivision(const GRAPH<rat_point,int>& G);

extern __exportF edge TRIANGULATE_PLANE_MAP(GRAPH<rat_point,rat_segment>& G);

LEDA_END_NAMESPACE




#if LEDA_ROOT_INCL_ID == 600001
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif

