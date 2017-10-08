/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dimacs.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_DIMACS_H
#define LEDA_DIMACS_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600201
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/graph/graph.h>
#include <LEDA/graph/dimacs_graph0.h>
#include <LEDA/graph/dimacs_graph.h>

LEDA_BEGIN_NAMESPACE

extern __exportF string Read_Dimacs_Graph(istream& in, graph& G, list<node>& L,
                                             node_array<string>& n_label,
                                             edge_array<string>& e_label);
/*{\Mopl   reads any graph problem description in DIMACS format,
           appends all nodes with a node line in |L|, and stores the
           data associated with nodes or edges as strings in the 
           corresponding arrays |n_label| and |e_label|. The function returns 
           the name of the problem (p-line). */


extern __exportF void Read_Dimacs_SP(istream& in, graph& G, node& s, 
                                                  edge_array<int>& cost);

/*{\Mopl   reads a single-source shortest path problem description in 
           DIMACS format. }*/


extern __exportF void Write_Dimacs_SP(ostream& os, const graph& G, node s, 
                                                   const edge_array<int>& cost);

/*{\Mopl   writes a single-source shortest path problem description 
           in DIMACS format. }*/





extern __exportF void Read_Dimacs_MF(istream& in, graph& G, node& s, node& t,
                                                  edge_array<int>& cap);

/*{\Mopl   reads a maxflow problem description in DIMACS format. }*/



extern __exportF void Write_Dimacs_MF(ostream& os,const graph& G,node s,node t,
                                                  const edge_array<int>& cap);

/*{\Mopl   writes a maxflow problem description in DIMACS format. }*/






extern __exportF void Read_Dimacs_MCF(istream& in, graph& G, 
                                                   node_array<int>& supply,
                                                   edge_array<int>& lcap,
                                                   edge_array<int>& ucap,
                                                   edge_array<int>& cost);

/*{\Mopl   reads a min-cost-flow problem description in DIMACS format. }*/


extern __exportF void Write_Dimacs_MCF(ostream& out, const graph& G, 
                                               const node_array<int>& supply,
                                               const edge_array<int>& lcap,
                                               const edge_array<int>& ucap,
                                               const edge_array<int>& cost);

/*{\Mopl   writes a min-cost-flow problem description in DIMACS format. }*/




extern __exportF void Read_Dimacs_MAT(istream& in, graph& G,
                                                   edge_array<int>& cost);

/*{\Mopl   reads a matching problem description in DIMACS format. }*/



extern __exportF void Write_Dimacs_MAT(ostream& os, const graph& G,
                                                   const edge_array<int>& cost);

/*{\Mopl   writes a matching problem description in DIMACS format. }*/




#if LEDA_ROOT_INCL_ID == 600201
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
