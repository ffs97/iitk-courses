#ifndef STATIC_FGRAPH_H
#define STATIC_FGRAPH_H

#include <LEDA/graph/static_graph.h>
#include <LEDA/graph/node_slot.h>

LEDA_BEGIN_NAMESPACE

template <class cat, class nd = data_slots<0>, class ed = data_slots<0> >
class static_fgraph : public static_graph<cat,nd,ed>
{ 
  public:

  typedef static_graph<cat,nd,ed> st_graph;
  typedef typename st_graph::node node;
  typedef typename st_graph::edge edge;

  typedef graph_map<static_fgraph<cat,nd,ed> >* map_ptr;
  typedef graph_map<st_graph>* st_map_ptr;


  mutable edge* V_first_out;
  mutable edge* V_first_in;

  mutable unsigned long* E_pred_in;
  mutable unsigned long* E_index;

  edge E_end;


  struct incident_edge {

    unsigned long xx;

    typedef typename st_graph::edge edge;

    bool out() const { return xx & 0x00000001; }
    bool in()  const { return !out(); }

    incident_edge(unsigned long x=0) : xx(x) {}

    incident_edge(edge e, bool out)
    { xx = 2*(unsigned long)(e-edge(0)) + (unsigned long)out; }

    incident_edge reverse() const { return incident_edge(xx ^ 0x00000001); }

    edge get_edge() const { return edge(0) + xx/2; }
    operator edge() const { return get_edge(); }
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


  edge first_in_edge(node v) const { return (edge)v->FirstInEdge; }
  edge next_in_edge(edge e) const { return (edge)e->NextInEdge; }

  edge first_out_edge(node v) const
  { edge x = v->first_out_edge(); 
    return (x == v->stop_out_edge()) ? 0 : x;
   }
  edge next_out_edge(edge e, node v) const 
  { edge x = e->next_out_edge(); 
    return (x == v->stop_out_edge()) ? 0 : x;
   }





  edge get_pred_in(int i) const
  { return edge(0) + E_pred_in[i]/2; }

  void set_pred_in(int i, edge x, unsigned long mask) const
  { E_pred_in[i]  = 2*(unsigned long)(x - edge(0)) | mask; }

  void set_pred_in(int i, edge x) const
  { unsigned long p = E_pred_in[i] & 0x00000001;
    E_pred_in[i] = 2*(unsigned long)(x-edge(0)) + p;
   }


  bool hidden_in_edge(edge e) const 
  { return E_pred_in[st_graph::index(e)] & 0x00000001; }

  void hide_all_in_edges() const
  { node v;
    forall_nodes(v,*this) v->FirstInEdge = 0;
    int m = st_graph::number_of_edges();
    for(int i=0; i<m; i++) E_pred_in[i] |= 0x00000001;
   }


  void restore_all_in_edges() const
  { node v;
    forall_nodes(v,*this) v->FirstInEdge = V_first_in[st_graph::index(v)];
    int m = this->first_hidden_edge() - this->first_edge();
    for(int i=0; i<m; i++) E_pred_in[i] &= 0xfffffffe;
   }


  void swap_in_edges(node v) const
  { edge e = (edge)v->FirstInEdge;
    v->FirstInEdge = V_first_in[st_graph::index(v)]; 
    V_first_in[st_graph::index(v)] = e;
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

    int i_v = st_graph::index(v);
    int i_e = st_graph::index(e);

    edge first= V_first_in[i_v];
    edge last = get_pred_in(st_graph::index(first));

    E_pred_in[i_e] |= 0x00000001;

    if (e == first)
    { assert(v->FirstInEdge == first);
      v->FirstInEdge = e->NextInEdge;
      return;
     }

    // unlink e

    edge succ = (edge)e->NextInEdge;
    edge pred = get_pred_in(i_e);

    pred->NextInEdge = succ;

    if (e == last)
       last = pred;
    else
       set_pred_in(st_graph::index(succ),pred);

    if (e == v->FirstInEdge) v->FirstInEdge = succ;

    // move e to to front

    set_pred_in(i_e,last);
    e->NextInEdge = first;
    set_pred_in(st_graph::index(first),e);
    first= V_first_in[i_v] = e;
  }


  void restore_in_edge(edge e, node v) const
  {  
    if (!hidden_in_edge(e)) return;

    int i_v = st_graph::index(v);
    int i_e = st_graph::index(e);

    edge first= V_first_in[i_v];
    edge last = get_pred_in(st_graph::index(first));

    if (e == last)
    { assert(v->FirstInEdge == 0);
      v->FirstInEdge = e;
      E_pred_in[i_e] &= 0xfffffffe;
      return;
     }

    // unlink e

    edge succ = (edge)e->NextInEdge;

    if (e == first)
      { first = succ;
        V_first_in[i_v] = first;
       }
    else
      { edge pred = get_pred_in(i_e);
        pred->NextInEdge = succ;
        set_pred_in(st_graph::index(succ),pred);
       }

    // move e to back: append behind last

    last->NextInEdge = e;
    e->NextInEdge = 0;

    set_pred_in(i_e,last,0);
    set_pred_in(st_graph::index(first),e);
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



  void finish_construction()
  { st_graph::finish_construction();
    int n = st_graph::number_of_nodes();
    int m = st_graph::number_of_edges();

    V_first_out = new edge[n+1];
    V_first_in = new edge[n+1];

    E_end = st_graph::e_end;
    E_pred_in = new unsigned long[m];
    E_index = new unsigned long[m];
    for(int i=0; i<m; i++) E_index[i] = i;

    V_first_out[n] = (edge)st_graph::v_end->FirstOutEdge;
    V_first_in[n] = (edge)st_graph::v_end->FirstInEdge;

    node v;
    forall_nodes(v,*this) 
    { V_first_in[st_graph::index(v)] = (edge)v->FirstInEdge;
      V_first_out[st_graph::index(v)] = (edge)v->FirstOutEdge;
      edge p = 0;
      edge e;
      forall_in_edges(e,v)
      { E_pred_in[st_graph::index(e)] = 2*(unsigned long)(p-edge(0));
        p = e;
       }
      if (v->first_in_edge()) 
         E_pred_in[st_graph::index(v->first_in_edge())] = 
                                        2*(unsigned long)(p-edge(0));
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
        { unsigned long last = E_pred_in[st_graph::index(f)];
          E_pred_in[st_graph::index(f)] = 2*(unsigned long)(e-edge(0));
          E_pred_in[st_graph::index(e)] = last;
         }
        else
          E_pred_in[st_graph::index(e)] = 2*(unsigned long)(e-edge(0));
       }
     }
    forall_nodes(v,(*this)) 
         V_first_in[st_graph::index(v)] = (edge)v->FirstInEdge;
   }


  void swap_edges(edge x, edge y) const 
  { if (x != y)
    { swap(E_index[st_graph::index(x)],E_index[st_graph::index(y)]);
      swap(*x,*y);
     }
  }


// only for hidden edges:

node hidden_source(edge e) const { return node(e->NextInEdge); }

edge first_hidden_edge() const { return st_graph::e_end; }
edge stop_hidden_edge() const { return E_end; }

template<class edge_filter>
int filter_out_edges(edge_filter& F) const
{ node v;
  forall_nodes(v,(*this)) 
  { edge e; 
    forall_out_edges(e,v) e->NextInEdge = v; // source
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
  int num = st_graph::e_end - p;
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

  int n = st_graph::number_of_nodes();
  int m = st_graph::number_of_edges();

  edge f = st_graph::first_edge();
  for(int i=0; i<m; i++)
  { int j = E_index[i];
    while (i != j) 
    { swap_edges(f+i,f+j);
      j = E_index[i];
     }
   }

  node v;
  forall_nodes(v,*this)
     v->FirstOutEdge = V_first_out[st_graph::index(v)];

  st_graph::v_end->FirstOutEdge = V_first_out[n];

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



 static_fgraph() : V_first_out(0),V_first_in(0), E_pred_in(0), E_index(0) {}  

 static_fgraph(char* VV, char* EE) : st_graph(VV,EE), 
                   V_first_out(0),V_first_in(0), E_pred_in(0), E_index(0) {}  

 ~static_fgraph() 
  { if (V_first_out) delete[] V_first_out;
    if (V_first_in)  delete[] V_first_in;
    if (E_pred_in) delete[] E_pred_in;
    if (E_index) delete[] E_index;
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
  if (x == e_stop) return 0;
  return x;
 }


#define forall_incident_edges(e,v) \
for(First_Inc_Edge(e,v); e; Next_Inc_Edge(e,v))

#define forall_cyclic_incident_edges(e,e_start,v) \
for(e = e_start; e; e=Next_Cyclic_Inc_Edge(e,e_start,v))

#define forall_hidden_edges(e,G) \
for(e = G.first_hidden_edge(); e != G.stop_hidden_edge(); e++)


   
LEDA_END_NAMESPACE

#endif
