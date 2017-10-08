#ifndef LEDA_PLANE_GRAPH_ALG_H
#define LEDA_PLANE_GRAPH_ALG_H


#include <LEDA/graph/graph.h>
#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE


extern __exportF node ST_NUMBERING(const graph& G, 
                                   node_array<int>& stnum, 
                                   list<node>& stlist, edge e_st = nil); 

extern __exportF bool HT_PLANAR(graph&, bool embed=false);
extern __exportF bool BL_PLANAR(graph&, bool embed=false);
extern __exportF bool PLANAR2(graph&, bool embed=false);

extern __exportF bool PLANAR(graph&, bool embed=false);

extern __exportF bool HT_PLANAR(graph&, list<edge>&, bool embed=false);
extern __exportF bool BL_PLANAR(graph&, list<edge>&, bool embed=false);
extern __exportF bool PLANAR(graph& G, list<edge>& el, bool embed=false);

extern __exportF bool CHECK_KURATOWSKI(const graph& G, const list<edge>& el);

extern __exportF int KURATOWSKI(graph& G, list<node>& V, list<edge>& E, node_array<int>& deg);

extern __exportF list<edge> TRIANGULATE_PLANAR_MAP(graph& G);


extern __exportF void FIVE_COLOR(graph& G, node_array<int>& C);


extern __exportF void INDEPENDENT_SET(const graph& G, list<node>& I);

extern __exportF bool Is_CCW_Ordered(const graph& G, 
                            const node_array<int>& x,
                            const node_array<int>& y);

extern __exportF bool SORT_EDGES(graph& G, 
                                 const node_array<int>& x,
                                 const node_array<int>& y);

extern __exportF bool Is_CCW_Ordered(const graph& G, 
                            const edge_array<int>& dx,
                            const edge_array<int>& dy);


extern __exportF bool SORT_EDGES(graph& G, 
                                 const edge_array<int>& dx,
                                 const edge_array<int>& dy);

//-----------------------------------------------------------------------------
// floating point (double) versions 
//-----------------------------------------------------------------------------


extern __exportF bool Is_CCW_Ordered(const graph& G, 
                            const node_array<double>& x,
                            const node_array<double>& y);



extern __exportF bool SORT_EDGES(graph& G, 
                                 const node_array<double>& x,
                                 const node_array<double>& y);

extern __exportF bool Is_CCW_Ordered(const graph& G, 
                            const edge_array<double>& x,
                            const edge_array<double>& y);



extern __exportF bool SORT_EDGES(graph& G, 
                                 const edge_array<double>& x,
                                 const edge_array<double>& y);


LEDA_END_NAMESPACE

#endif

