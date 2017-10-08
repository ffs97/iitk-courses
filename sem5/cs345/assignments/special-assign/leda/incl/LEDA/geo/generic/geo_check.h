/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geo_check.t
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GEO_CHECK_T
#define LEDA_GEO_CHECK_T

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600073
#include <LEDA/internal/PREAMBLE.h>
#endif


LEDA_BEGIN_NAMESPACE


template <class geo_graph>
class cmp_edges_by_angle: public leda_cmp_base<edge> { 
  
  const geo_graph* Gptr;

public:

  cmp_edges_by_angle(const geo_graph& G): Gptr(&G){}

  int operator()(const edge& e, const edge& f) const
  { return compare_by_angle(edge_vector(*Gptr,e),
                            edge_vector(*Gptr,f));
  } 
};


template <class T>
__temp_func_inline 
bool Is_C_Nondecreasing(const list<T>& L, const leda_cmp_base<T>& cmp)
{ list_item it;
  int number_of_less = 0;

  forall_items(it,L) 
    if ( cmp(L[L.cyclic_succ(it)],L[it]) < 0 ) 
      number_of_less++;

  return (number_of_less < 2);
}

template <class T>
__temp_func_inline
bool Is_C_Increasing(const list<T>& L, const leda_cmp_base<T>& cmp)
{ list_item it;
  int number_of_lesseq = 0;

  forall_items(it,L) 
    if ( cmp(L[L.cyclic_succ(it)],L[it]) <= 0 ) 
      number_of_lesseq++;

  return (number_of_lesseq < 2);
}


template <class T>
__temp_func_inline 
bool Is_C_Nonincreasing(const list<T>& L, const leda_cmp_base<T>& cmp)
{ list_item it;
  int number_of_greater = 0;
  forall_items(it,L) 
    if ( cmp(L[L.cyclic_succ(it)],L[it]) > 0 ) number_of_greater++;
  return (number_of_greater < 2);
}


template <class T>
__temp_func_inline 
bool Is_C_Decreasing(const list<T>& L, const leda_cmp_base<T>& cmp)
{ list_item it;
  int number_of_greatereq = 0;
  forall_items(it,L) 
    if ( cmp(L[L.cyclic_succ(it)],L[it]) >= 0 ) number_of_greatereq++;
  return (number_of_greatereq < 2);
}


template <class geo_graph>
__temp_func_inline 
bool Is_CCW_Ordered(const geo_graph& G)
{ node v;
  cmp_edges_by_angle<geo_graph> cmp(G);
  
  forall_nodes(v,G)
    if ( !Is_C_Increasing(G.out_edges(v),cmp) ) return false;

  return true;
}

template <class geo_graph>
__temp_func_inline 
bool Is_CCW_Ordered_Plane_Map(const geo_graph& G)
{ return Is_Plane_Map(G) && Is_CCW_Ordered(G); }

template <class geo_graph>
__temp_func_inline 
bool Is_CCW_Weakly_Ordered(const geo_graph& G)
{ node v;
  cmp_edges_by_angle<geo_graph> cmp(G);
  
  forall_nodes(v,G)
    if ( !Is_C_Nondecreasing(G.out_edges(v),cmp) ) return false;

  return true;
}

template <class geo_graph>
__temp_func_inline 
bool Is_CCW_Weakly_Ordered_Plane_Map(const geo_graph& G)
{ return Is_Plane_Map(G) && Is_CCW_Weakly_Ordered(G); }

template <class geo_graph>
void SORT_EDGES(geo_graph& G)
{ 
  cmp_edges_by_angle<geo_graph> cmp(G);

  list<edge> L = G.all_edges();
  L.sort(cmp);
  G.sort_edges(L);
}


template <class geo_graph>
__temp_func_inline 
bool Is_CCW_Convex_Face_Cycle(const geo_graph& G, const edge& e)
{ 
  list<edge> L;
  edge e1 = e;
  do { L.append(e1); 
       e1 = G.face_cycle_succ(e1);
  } while ( e1 != e );

  cmp_edges_by_angle<geo_graph> cmp(G);
  
  return Is_C_Increasing(L,cmp);
}


template <class geo_graph>
__temp_func_inline 
bool Is_CCW_Weakly_Convex_Face_Cycle(const geo_graph& G, const edge& e)
{ 
  list<edge> L;
  edge e1 = e;
  do { L.append(e1); 
       e1 = G.face_cycle_succ(e1);
  } while ( e1 !=  e);

  cmp_edges_by_angle<geo_graph> cmp(G);
  
  return Is_C_Nondecreasing(L,cmp);
}

template <class geo_graph>
__temp_func_inline 
bool Is_CW_Convex_Face_Cycle(const geo_graph& G, const edge& e)
{ 
  list<edge> L;
  edge e1 = e;
  do { L.append(e1); 
       e1 = G.face_cycle_succ(e1);
  } while ( e1 !=  e);

  cmp_edges_by_angle<geo_graph> cmp(G);
  
  return Is_C_Decreasing(L,cmp);
}


template <class geo_graph>
__temp_func_inline 
bool Is_CW_Weakly_Convex_Face_Cycle(const geo_graph& G, const edge& e)
{ 
  list<edge> L;
  edge e1 = e;
  do { L.append(e1); 
       e1 = G.face_cycle_succ(e1);
  } while ( e1 !=  e);

  cmp_edges_by_angle<geo_graph> cmp(G);
  
  return Is_C_Nonincreasing(L,cmp);
}


#if LEDA_ROOT_INCL_ID == 600073
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif
