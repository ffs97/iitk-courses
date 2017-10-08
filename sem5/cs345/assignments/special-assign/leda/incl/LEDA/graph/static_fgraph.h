#ifndef STATIC_FGRAPH_H
#define STATIC_FGRAPH_H

#include <LEDA/graph/static_graph.h>
#include <LEDA/graph/node_slot.h>
#include <LEDA/graph/edge_slot.h>
#include <LEDA/graph/node_array.h>

#if !defined(MASK_00)
#if defined(WORD_LENGTH_64)
#define MASK_00 0xfffffffffffffffc
#define MASK_01 0xfffffffffffffffd
#else
#define MASK_00 0xfffffffc
#define MASK_01 0xfffffffd
#endif
#endif

LEDA_BEGIN_NAMESPACE

template <class cat, class nd = data_slots<0>, class ed = data_slots<0> >
class static_fgraph : public static_graph<cat,data_slots<nd::slots+2>,
                                              data_slots<ed::slots+0> >
{ 

  typedef size_t address;

  public:

  typedef static_graph<cat,data_slots<nd::slots+2>,
                           data_slots<ed::slots+0> > st_graph;

  typedef typename st_graph::node node;
  typedef typename st_graph::edge edge;

  typedef graph_map<static_fgraph<cat,nd,ed> >* map_ptr;
  typedef graph_map<st_graph>* st_map_ptr;

  enum { n_base_sz = st_graph::n_base_sz + 2 };
//enum { e_base_sz = st_graph::e_base_sz + 1 };
  enum { e_base_sz = st_graph::e_base_sz + 0 };

  mutable node_slot<edge,st_graph,0> V_first_out;
  mutable node_slot<edge,st_graph,1> V_first_in;

//mutable edge_slot<address, st_graph, 0>  E_pred_in;
  mutable edge_array<address, st_graph>  E_pred_in;

  mutable unsigned long* E_index;

  edge E_end;



struct incident_edge {

  address xx;

  typedef typename st_graph::edge edge;

  bool out() const { return xx & 0x1; }
  bool in()  const { return !out(); }

  incident_edge(address x=0) : xx(x) {}

  incident_edge(edge e, bool out)
  { xx = (address)e + (address)out; }

  incident_edge reverse() const { return incident_edge(xx ^ 0x1); }

  edge get_edge() const { return edge(xx & MASK_00); }
  operator edge() const { return get_edge(); }

  void mark()   { xx |= 0x2; }
  void unmark() { xx &= MASK_01; }
  bool is_marked() { return (xx & 0x2) != 0; }

};




  int index(edge e) const { return E_index[st_graph::index(e)]; }
  int index(node v) const { return st_graph::index(v); }


  node opposite(edge e, node v) const
  { return st_graph::opposite(e,v); }

  node opposite(const incident_edge& x, node v) const
  { return st_graph::opposite(x.get_edge(),v); }
  


  list_item register_map(map_ptr M, int& slot_index, bool use_slot=false)
  { return st_graph::register_map(st_map_ptr(M),slot_index,use_slot); 
    }

  void unregister_map(map_ptr M) 
  { st_graph::unregister_map(st_map_ptr(M)); }


  edge first_in_edge(node v) const { return v->first_in_edge(); }
  edge next_in_edge(edge e) const { return e->next_in_edge(); }

  edge first_out_edge(node v) const
  { edge x = v->first_out_edge(); 
    return (x == v->stop_out_edge()) ? 0 : x;
   }

  edge next_out_edge(edge e, node v) const 
  { edge x = e->next_out_edge(); 
    return (x == v->stop_out_edge()) ? 0 : x;
   }





  edge get_pred_in(edge e) const
  { address x= E_pred_in[e] & MASK_00;
    return edge(x);
   }

  void set_pred_in(edge e, edge x, address mask) const
  { E_pred_in[e]  = (address)x | mask; }

  void set_pred_in(edge e, edge x) const
  { address p = E_pred_in[e] & 0x1;
    E_pred_in[e] = ((address)x & MASK_00) | p;
   }


  bool hidden_in_edge(edge e) const 
  { return E_pred_in[e] & 0x1; }

  void hide_all_in_edges() const
  { node v;
    forall_nodes(v,*this) v->FirstInEdge = 0;
    edge e;
    forall_edges(e,*this) E_pred_in[e] |= 0x1;
   }


  void restore_all_in_edges() const
  { node v;
    forall_nodes(v,*this) v->FirstInEdge = V_first_in[v];
    edge e;
    forall_edges(e,*this) E_pred_in[e] &= MASK_00;
   }


  void swap_in_edges(node v) const
  { edge e = (edge)v->FirstInEdge;
    v->FirstInEdge = V_first_in[v]; 
    V_first_in[v] = e;
   }

  void enable_in_edge_hiding(node v) const
  { if (v->FirstInEdge && hidden_in_edge(edge(v->FirstInEdge)))
      swap_in_edges(v);
   }

  void disable_in_edge_hiding(node v) const
  { if (v->FirstInEdge ==0 || !hidden_in_edge(edge(v->FirstInEdge)))
      swap_in_edges(v);
   }


  void hide_in_edge(edge e, node v) const
  {  
    if (hidden_in_edge(e)) return;

    edge first= V_first_in[v];
    edge last = get_pred_in(first);

    E_pred_in[e] |= 0x1;

    if (e == first)
    { assert(v->FirstInEdge == first);
      v->FirstInEdge = e->next_in_edge();
      return;
     }

    // unlink e

    edge succ = e->next_in_edge();
    edge pred = get_pred_in(e);

    pred->NextInEdge = succ;
    //pred->NextInEdge = succ-pred;

    if (e == last)
       last = pred;
    else
       set_pred_in(succ,pred);

    if (e == v->FirstInEdge) v->FirstInEdge = succ;

    // move e to to front

    set_pred_in(e,last);

    e->NextInEdge = first;
    //e->NextInEdge = first-e;

    set_pred_in(first,e);
    first= V_first_in[v] = e;
  }


  void restore_in_edge(edge e, node v) const
  {  
    if (!hidden_in_edge(e)) return;

    edge first= V_first_in[v];
    edge last = get_pred_in(first);

    if (e == last)
    { assert(v->FirstInEdge == 0);
      v->FirstInEdge = e;
      E_pred_in[e] &= MASK_00;
      return;
     }

    // unlink e

    edge succ = e->next_in_edge();

    if (e == first)
      { first = succ;
        V_first_in[v] = first;
       }
    else
      { edge pred = get_pred_in(e);
        pred->NextInEdge = succ;
        //pred->NextInEdge = succ-pred;
        set_pred_in(succ,pred);
       }

    // move e to back: append behind last

    last->NextInEdge = e;
    //last->NextInEdge = e-last;

    e->NextInEdge = 0;

    set_pred_in(e,last,0);
    set_pred_in(first,e);
    if (v->FirstInEdge == 0) v->FirstInEdge = e;
  }



  incident_edge first_incident_edge(node v) const
  { edge e = v->first_out_edge();
    if (e != v->stop_out_edge())
      return incident_edge(e,true);
    else
      return incident_edge(v->first_in_edge(),false);
  }


  incident_edge next_incident_edge(const incident_edge& x, node v) const
  { edge e = x;
    bool out = x.out();
    if (out)
      { e = e->next_out_edge();
        if (e == v->stop_out_edge())
        { out = false;
          e = v->first_in_edge();
         }
       }
    else
       e = e->next_in_edge();
    return incident_edge(e,out);
  }

  incident_edge next_cyclic_incident_edge(const incident_edge& x, node v) const
  { incident_edge y  = next_incident_edge(x,v);
    return y ? y : first_incident_edge(v);
  }


  void start_construction(unsigned n, unsigned m)
  { if (E_index) 
    { delete[] E_index;
      E_index = 0;
     }
    st_graph::start_construction(n,m);
   }


  void finish_construction()
  { st_graph::finish_construction();
    int n = st_graph::number_of_nodes();
    int m = st_graph::number_of_edges();

    E_end = st_graph::e_end;

    V_first_out.init(*this,n+1,0);
    V_first_in.init(*this,n+1,0);
    E_pred_in.init(*this,m+1,0);


    E_index = new unsigned long[m];

    for(int i=0; i<m; i++) E_index[i] = i;

    V_first_out[st_graph::v_end] = (edge)st_graph::v_end->FirstOutEdge;
    V_first_in[st_graph::v_end] = (edge)st_graph::v_end->FirstInEdge;

    node v;
    forall_nodes(v,*this) 
    { V_first_in[v] = (edge)v->FirstInEdge;
      V_first_out[v] = (edge)v->FirstOutEdge;
      edge p = 0;
      edge e;
      forall_in_edges(e,v)
      { E_pred_in[e] = (address)p;
        assert(((address)e & 0x1) == 0);
        p = e;
       }
      if (v->first_in_edge()) 
         E_pred_in[v->first_in_edge()] = (address)p;
     }
   }


// sort edges

  void rebuild_in_edges() const
  { node v;
    forall_nodes(v,(*this)) v->FirstInEdge = 0;
    for(v = st_graph::stop_node()-1; v >= st_graph::first_node(); v--)
    { edge e_stop = v->first_out_edge();
      for(edge e=v->stop_out_edge()-1; e>=e_stop; e--)
      { node u = opposite(e,v);
        edge f = (edge)u->FirstInEdge;
        push_in_edge(u,e);
        if (f) 
        { address last = E_pred_in[f];
          E_pred_in[f] = (address)e;
          E_pred_in[e] = last;
         }
        else
          E_pred_in[e] = (address)e;
       }
     }
    forall_nodes(v,(*this)) 
         V_first_in[v] = (edge)v->FirstInEdge;
   }


  void swap_edges(edge x, edge y) const 
  { if (x != y)
    { swap(E_index[st_graph::index(x)],E_index[st_graph::index(y)]);
      swap(*x,*y);
     }
  }


// only for hidden edges:

node hidden_source(edge e) const 
{ return node(e->NextInEdge); 
  //return first_node() + e->NextInEdge; 
}

edge first_hidden_edge()      const { return st_graph::e_end; }
edge next_hidden_edge(edge e) const { return e+1; }
edge stop_hidden_edge()       const { return E_end; }


template<class edge_filter>
int filter_out_edges(edge_filter& F) const
{ node v;
  forall_nodes(v,(*this)) 
  { edge e; 
    forall_out_edges(e,v) e->NextInEdge = v; // source
    //forall_out_edges(e,v) e->NextInEdge = v-st_graph::first_node();
   }

  edge p = st_graph::first_edge();

  forall_nodes(v,(*this)) 
  { edge stop = v->stop_out_edge();
    edge x = v->first_out_edge();
    v->FirstOutEdge = p;
    for(edge e = x; e < stop; e++)
    { if (F(e,v)) continue;
      swap_edges(e,p++);
     }
   }
  st_graph::v_end->FirstOutEdge = p;
  int num = int(st_graph::e_end - p);
  (edge&)st_graph::e_end = p;
  return  num;
}

template<class edge_filter>
int hide_edges(edge_filter& F) const
{ int count = filter_out_edges(F); 
  rebuild_in_edges();
  return count;
}


void restore_all_edges(bool rebuild_in = true) const 
{ (edge&)st_graph::e_end = E_end;

  //int n = st_graph::number_of_nodes();

  address m = st_graph::number_of_edges();

  edge f = st_graph::first_edge();
  for(unsigned long i=0; i<m; i++)
  { unsigned long j = E_index[i];
    while (i != j) 
    { swap_edges(f+i,f+j);
      j = E_index[i];
     }
   }

  node v;
  forall_nodes(v,*this) v->FirstOutEdge = V_first_out[v];

  st_graph::v_end->FirstOutEdge = V_first_out[st_graph::v_end];

  if (rebuild_in) rebuild_in_edges();

 }


  
  void permute_edges() const
  { node v;
    forall_nodes(v,(*this))
    { edge e = v->first_out_edge();
      edge s = v->stop_out_edge();
      for(int d = s-e-1; d>0; d--)
      { swap_edges(e, e + (rand_int.get() % d));
        e++;
       }
    }
    rebuild_in_edges();
   }



 static_fgraph() : V_first_out(*this),V_first_in(*this), E_pred_in(*this), 
                                                         E_index(0) {}  

 ~static_fgraph() 
  { if (E_index) delete[] E_index;
   }


};


template<class node, class incident_edge>
inline void First_Inc_Edge(incident_edge& x, node v)
{ typename incident_edge::edge e = v->first_out_edge();
  if (e != v->stop_out_edge())
    x = incident_edge(e,true);
  else
    x = incident_edge(v->first_in_edge(),false);
}


template<class node, class incident_edge>
inline  void Next_Inc_Edge(incident_edge& x, node v)
{ typename incident_edge::edge e = x;
  if (x.out())
  { e = e->next_out_edge();
    if (e == v->stop_out_edge())
      x = incident_edge(v->first_in_edge(),false);
    else
      x = incident_edge(e,true);
   }
  else 
    x = incident_edge(e->next_in_edge(),false);
 }

/*
template<class node, class incident_edge>
inline  incident_edge Next_Inc_Edge(incident_edge x, node v)
{ typename incident_edge::edge e = x;
  if (x.out())
  { e = e->next_out_edge();
    if (e == v->stop_out_edge())
      return incident_edge(v->first_in_edge(),false);
    else
      return incident_edge(e,true);
   }
  else 
   return incident_edge(e->next_in_edge(),false);
 }
*/



template<class node, class incident_edge>
inline incident_edge Next_Cyclic_Inc_Edge(incident_edge x, 
                                          incident_edge e_stop, node v)
{ Next_Inc_Edge(x,v);
  if (x == 0) First_Inc_Edge(x,v);
  return (x == e_stop) ? incident_edge(0) : x;
 }

   
LEDA_END_NAMESPACE

#endif
