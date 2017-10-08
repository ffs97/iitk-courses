/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  scale_weights.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_SCALE_WEIGHTS
#define LEDA_SCALE_WEIGHTS

#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/graph/edge_array.h>

LEDA_BEGIN_NAMESPACE


inline bool check_weights(const graph& G, const edge_array<int>& c,
                          int lb, int ub, string inf)
{ edge e;
  bool all_edges_ok = true;
  forall_edges(e,G) 
    if ( c[e] < lb || c[e] > ub ) all_edges_ok = false;

  if ( !all_edges_ok ) 
    LEDA_EXCEPTION(1,inf + " : danger of overflow.");
  
  return all_edges_ok;
}


inline bool check_weights(const graph& G, 
                               const node_array<int>& c,
                               int lb,
                               int ub,
                               string inf)
{ node v;
  bool all_nodes_ok = true;
  forall_nodes(v,G) 
    if ( c[v] < lb || c[v] > ub ) all_nodes_ok = false;

  if ( !all_nodes_ok )
     LEDA_EXCEPTION(1,inf + " : danger of overflow.");
  
  return all_nodes_ok;
}

template <class NT>
inline NT max_absolute_value(const graph& G, const edge_array<NT>& c)
{ NT A = 0;
  edge e;
  forall_edges(e,G) 
  { if (  c[e] > A ) A =  c[e];
    if ( -c[e] > A ) A = -c[e];
  }
  return A;
}

template <class NT>
inline NT max_absolute_value(const graph& G, const node_array<NT>& c)
{ NT A = 0;
  node v;
  forall_nodes(v,G) 
  { if (  c[v] > A ) A =  c[v];
    if ( -c[v] > A ) A = -c[v];
  }
  return A;
}


#include <LEDA/internal/std/math.h>

inline int compute_s(double f, double C)
{ int exp;
  frexp(f*C,&exp); 
  return 53 - exp;
}

inline double compute_S(double f, double C, double& one_over_S)
{ int exp;
  frexp(f*C,&exp);
  one_over_S = ldexp(1.0,exp - 53); 
  return ldexp(1.0,53 - exp);
}


inline double scale_weight(double w, double S, double one_over_S)
{ if ( w == 0 ) return 0;
  int sign_w = +1;
  if ( w < 0 ) { sign_w = -1; w = -w; }
  return sign_w * ::floor(w * S) * one_over_S;
}
  


inline bool scale_weights(const graph& G, const edge_array<double>& c,
                                          edge_array<double>& c1, double f)
{ edge e;
  
  double C = 0;
  forall_edges(e,G) C = leda_max(C,fabs(c[e]));

  double one_over_S;
  double S = compute_S(f,C,one_over_S);
  
  int count = 0;

  forall_edges(e,G)  // note: c and c1 might be the same array
  { double sw =  scale_weight(c[e],S,one_over_S);
    if (c1[e] == sw) continue;
    c1[e] = sw;
    count++;
  }
 
  return count == 0; 
}


inline bool scale_weights(const graph& G, const edge_array<double>& c,
                                          edge_array<double>& c1, double f, string inf)
{ bool no_scaling = scale_weights(G,c,c1,f);
  if ( no_scaling == false ) 
    LEDA_EXCEPTION(0,inf + " : scaling was required.");
  return no_scaling;
}


inline bool scale_weights(const graph& G, edge_array<double>& c, double f)
{ return scale_weights(G,c,c,f); }


// node arrays


inline bool scale_weights(const graph& G, const node_array<double>& c,
                                          node_array<double>& c1, double f)

{ node e;
  
  double C = 0;
  forall_nodes(e,G) C = leda_max(C,fabs(c[e]));

  double one_over_S;
  double S = compute_S(f,C,one_over_S);
  
  int count = 0;

  forall_nodes(e,G)  // note: c and c1 might be the same array
  { double sw = scale_weight(c[e],S,one_over_S);
    if (c1[e] == sw) continue;
    c1[e] = sw;
    count++;
  }
 
  return count == 0;
}



inline bool scale_weights(const graph& G, const node_array<double>& c,
                                          node_array<double>& c1, double f, string inf)
{ bool no_scaling = scale_weights(G,c,c1,f);
  if ( no_scaling == false )
      LEDA_EXCEPTION(0,inf + " : scaling was required.");
  return no_scaling;
}


inline bool scale_weights(const graph& G, node_array<double>& c, double f)
{ return scale_weights(G,c,c,f); }


LEDA_END_NAMESPACE

#endif

