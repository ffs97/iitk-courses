#ifndef LEDA_GRAPH_ALG_H
#define LEDA_GRAPH_ALG_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600004
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graph/graph.h>
#include <LEDA/graph/plane_graph_alg.h>
#include <LEDA/graph/max_flow.h>
#include <LEDA/graph/graph_draw.h>

LEDA_BEGIN_NAMESPACE

extern __exportF int COMPONENTS(const graph& G, node_array<int>& compnum);

extern __exportF int BICONNECTED_COMPONENTS(const graph& G, edge_array<int>& compnum);

extern __exportF int STRONG_COMPONENTS(const graph& G,node_array<int>& compnum);

extern __exportF list<edge> SPANNING_TREE(const graph& G);

extern __exportF GRAPH<node,edge> TRANSITIVE_CLOSURE(const graph& G);
extern __exportF GRAPH<node,edge> TRANSITIVE_REDUCTION(const graph& G);

extern __exportF void MAKE_TRANSITIVELY_CLOSED(graph& G);
extern __exportF void MAKE_TRANSITIVELY_REDUCED(graph& G);

extern __exportF bool MCF_COST_SCALING(const graph& G, 
                                                const edge_array<int>& lcap,
                                                const edge_array<int>& ucap,
                                                const edge_array<int>& cost,
                                                const node_array<int>& supply,
                                                edge_array<int>& flow);

extern __exportF int MCMF_COST_SCALING(const graph& G,node s,node t, 
                                                const edge_array<int>& cap, 
                                                const edge_array<int>& cost, 
                                                edge_array<int>& flow);


inline bool MIN_COST_FLOW(graph& G, const edge_array<int>& lcap,
                                    const edge_array<int>& ucap,
                                    const edge_array<int>& cost,
                                    const node_array<int>& supply,
                                          edge_array<int>& flow)
{ return MCF_COST_SCALING(G,lcap,ucap,cost,supply,flow); }

LEDA_END_NAMESPACE


#if LEDA_ROOT_INCL_ID == 600004
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif



