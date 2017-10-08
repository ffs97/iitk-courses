/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  fd_graph.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef FD_GRAPH_H
#define FD_GRAPH_H

#include <LEDA/graph/graph_map.h>
#include <LEDA/system/assert.h>

LEDA_BEGIN_NAMESPACE

// ------------------------------------------------------------------------- 
// macros
// ------------------------------------------------------------------------- 

#undef  forall_out_edges
#define forall_out_edges(e,v)\
LEDA_FORALL_PREAMBLE \
for(edge loop_stop = ((e = (edge) v->first_out_edge()), (edge) v->stop_out_edge());\
e != loop_stop; e++)

#undef  forall_in_edges
#define forall_in_edges(e,v)\
for(e = (edge) v->first_in_edge(); e; e = (edge) e->in_succ())

#undef  forall_adj_edges
#define forall_adj_edges(e,v) forall_out_edges(e,v)


#ifdef  SLOT_ENUM
#undef  SLOT_ENUM
#endif

#define SLOT_ENUM \
enum  \
{ \
  out_degree   = node_type::out_degree, \
  n_data_slots = node_type::data_slots, \
  n_first_slot = node_type::first_slot, \
  e_data_slots = edge_type::data_slots, \
  e_first_slot = edge_type::first_slot \
};

  
// ------------------------------------------------------------------------- 
// open namespace fixed_degree
// ------------------------------------------------------------------------- 

namespace fixed_degree 
{

// ------------------------------------------------------------------------- 
// base_node<ns>, base_edge<es>
// ------------------------------------------------------------------------- 

template <unsigned slots = 0>
class base_node
{
  GenPtr Data[ slots ];
  
  public:
              
  GenPtr& data(int i) { return Data[i]; }
};


template <unsigned slots = 0>
class base_edge
{
  GenPtr Data[ slots ];
  
  public:

  GenPtr& data(int i) { return Data[i]; }
};


// ------------------------------------------------------------------------- 
// basic_graph< target_node<deg,ET,ns>, target_edge<es> >
// basic_graph_traits<deg,ns,es>
// ------------------------------------------------------------------------- 

template <unsigned slots = 0>
struct target_edge : public base_edge<1 + slots>
{
  enum { data_slots = slots, first_slot = 1 };
  
  template <class GT, class NT>
  target_edge(const GT& G, NT v, NT w) { data(0) = w; }
  
  GenPtr target() { return data(0); }      
  
  LEDA_MEMORY(target_edge<slots>);
};


template <unsigned deg, class ET, unsigned slots = 0>
struct target_node : public base_node<(deg * sizeof(ET)/sizeof(GenPtr)) + slots>
{
  enum { out_degree = deg, data_slots = slots, first_slot = 0 };
  
  template <class GT> target_node(const GT& G) 
  {
    unsigned i = (deg * sizeof(ET)/sizeof(GenPtr)) + slots;
    while (--i >= deg) data(i) = G.stop_node();
  }
  
  //GenPtr first_out_edge() { return &data(slots); }  
  GenPtr first_out_edge() { return GenPtr((GenPtr*)this +slots); }  
  GenPtr stop_out_edge()  { return (this+1); }
  
  LEDA_MEMORY(target_node<slots>);
};


template <class NT, class ET>
struct basic_graph
{
  typedef NT  node_type;
  typedef ET  edge_type;
  
  typedef NT* node;
  typedef ET* edge;
    
  edge first_out_edge(node v) const { return (edge) v->first_out_edge(); }
  edge next_out_edge(edge e)  const { return e+1; }
  edge stop_out_edge(node v)  const { return (edge) v->stop_out_edge();  }
  
  node target(edge e, node=0) const { return (node) e->target(); }  
  int  outdeg(node v)         const { return out_degree; }

  SLOT_ENUM
};

template <unsigned deg, unsigned ns, unsigned es>
struct basic_graph_traits : 
  public basic_graph< target_node<deg, target_edge<es>, ns>, target_edge<es> >
{  
  typedef basic_graph< target_node<deg,target_edge<es>, ns>, target_edge<es> > graph_type;
  typedef typename graph_type::node_type node_type;
  typedef typename graph_type::edge_type edge_type;
  typedef typename graph_type::node node;
  typedef typename graph_type::edge edge;
  
  SLOT_ENUM
};


// ------------------------------------------------------------------------- 
// in_edge_graph< in_node<deg,ET,ns>, in_edge<es> >
// in_edge_graph_trait<deg,ns,es>
// ------------------------------------------------------------------------- 

template <unsigned deg, class ET, unsigned slots = 0>
struct in_node : public base_node<(deg * sizeof(ET)/sizeof(GenPtr)) + slots + 1>
{  
  enum { out_degree = deg, data_slots = slots, first_slot = 1 };
  
  template <class GT> in_node(const GT& G) 
  { 
    data(0) = 0;
    unsigned i = (deg * sizeof(ET)/sizeof(GenPtr)) + slots + 1;
    while (--i >= deg) data(i) = G.stop_node();
  }
 
  GenPtr add_in(GenPtr e) 
  {
    GenPtr x = data(0);
    data(0) = e;
    return x;
  }             
  
  GenPtr first_out_edge() { return &data(first_slot + slots); }  
  GenPtr stop_out_edge()  { return this + 1; }

  GenPtr first_in_edge()  { return data(0);  }
  GenPtr stop_in_edge()   { return 0; }

  LEDA_MEMORY(in_node<slots>);
};


template <unsigned slots = 0>
struct in_edge : public base_edge<2 + slots>
{
  enum { data_slots = slots, first_slot = 2 };
  
  template <class GT, class NT>
  in_edge(const GT& G, NT v, NT w) 
  {
    data(0) = w;
    data(1) = w->add_in(this);
  }
  
  GenPtr target()  { return data(0); }      
  GenPtr in_succ() { return data(1); }
  
  LEDA_MEMORY(in_edge<slots>);
};

template <class NT, class ET>
struct in_edge_graph : public basic_graph<NT,ET>
{
  typedef NT node_type;
  typedef ET edge_type;
  
  typedef NT* node;
  typedef ET* edge;
  
  edge first_in_edge(node v) const { return (edge) v->first_in_edge(); }
  edge next_in_edge(edge e)  const { return (edge) e->in_succ(); }
  edge stop_in_edge(node v)  const { return (edge) v->stop_in_edge(); }

  int indeg(node v) const
  {
    int deg = 0;
    edge e = first_in_edge(v), e_stop = stop_in_edge(v);
    for (; e != e_stop; e = next_in_edge(e)) deg++;
    return deg;
  }
  
  int degree(node v) const { return indeg(v) + outdeg(v); }
  
  SLOT_ENUM
};

template <unsigned deg, unsigned ns, unsigned es>
struct in_edge_graph_traits : 
  public in_edge_graph< in_node<deg, in_edge<es>, ns>, in_edge<es> >
{
  typedef in_edge_graph< in_node<deg, in_edge<es>, ns>, in_edge<es> > graph_type;
  typedef typename graph_type::node_type node_type;
  typedef typename graph_type::edge_type edge_type;
  typedef typename graph_type::node node;
  typedef typename graph_type::edge edge;  
  
  SLOT_ENUM
};


// ------------------------------------------------------------------------- 
// graph_register
// ------------------------------------------------------------------------- 

template <class GT>
class graph_register
{
  public:

  typedef graph_map<GT>  graph_map_type;
  typedef graph_map<GT>* gmap;

  protected:

  list<gmap> map_list[3];  
  list<int>  free_data[3]; 

  public:

  graph_register()
  { 
    int i;
    for (i = 0; i < GT::n_data_sl; i++) 
      free_data[0].append(GT::n_base_sz + i);
    
    for (i = 0; i < GT::e_data_sl; i++) 
      free_data[1].append(GT::e_base_sz + i);
  }
  
  void clear(GT& G) 
  {
    typename GT::node v = G.first_node(), v_stop = G.stop_node();
    for (; v != v_stop; v = G.next_node(v))
    { 
      gmap M;
      forall(M, get_map_list(0))
      { 
        int i = M->slot_index();
        if (i > 0) 
          M->clear_entry(v->data(i));
      }
    }

    typename GT::edge e = G.first_edge(), e_stop = G.stop_edge();
    for (; e != e_stop; e = G.next_edge(e))
    { 
      gmap M;
      forall(M, get_map_list(1)) 
      { 
        int i = M->slot_index();
        if (i > 0) 
          M->clear_entry(e->data(i));
      }
    }
  } 

  const list<gmap>& get_map_list(int i) { return map_list[i]; }

  list_item register_map(gmap m, int& slot_index, bool use_slot = false)
  { 
    int k = m->get_kind();
    slot_index = (!use_slot || free_data[k].empty()) ? -1 : free_data[k].pop();
    return map_list[k].push(m);
  }

  void unregister_map(gmap m)
  { 
    int k = m->get_kind();
    map_list[k].del_item(m->get_loc());
    
    if (m->slot_index() > 0)
      free_data[k].push(m->slot_index());
  }
};


// ------------------------------------------------------------------------- 
// close namespace fixed_degree
// ------------------------------------------------------------------------- 

}

// ------------------------------------------------------------------------- 
// fd_graph <graph_impl>
// ------------------------------------------------------------------------- 

template <class graph_impl>
class fd_graph : public graph_impl,
                 public fixed_degree::graph_register< fd_graph<graph_impl> >
{
  public:
  
  typedef typename graph_impl::node      node;
  typedef typename graph_impl::node_type node_type;
  
  typedef typename graph_impl::edge      edge;
  typedef typename graph_impl::edge_type edge_type;

  enum
  {
    n_base_sz  = node_type::first_slot,
    n_data_sl  = node_type::data_slots,
    e_base_sz  = edge_type::first_slot,
    e_data_sl  = edge_type::data_slots,    
    out_degree = graph_impl::out_degree
  };
    
  protected:

  node v_first;
  node v_last;
  node v_end;
  
  edge e_first;
  edge e_last;
  edge e_end;
    
  public:  
  
  fd_graph()
  { 
    v_first = v_last = v_end = 0;
    e_first = e_last = e_end = 0;
  }

  fd_graph(unsigned n) : v_first(0), v_end(0) { init(n); }

 ~fd_graph() { clear(); }
  
  void init(unsigned n, unsigned m) { init(n); }
  
  void init(unsigned n)
  { 
    clear();
    
    if (n == 0) return;

    v_first = (node) std_memory.allocate_vector(sizeof(node_type) * (n + 1));
    v_last  = v_first;
    v_end   = v_first + n;

    e_first = graph_impl::first_out_edge(v_first);
    e_last  = graph_impl::stop_out_edge(v_last);
   // e_last  = e_first;
    e_end   = graph_impl::first_out_edge(v_end);

    new (v_end) node_type(*this);
  }
  
 
  void clear()
  {     
    if (!v_first) return;
    
    fixed_degree::graph_register< fd_graph<graph_impl> >::clear(*this);
      
    std_memory.deallocate_vector(v_first);     
  
    v_first = v_last = v_end = 0;    
    e_first = e_last = e_end = 0;
  }  

 
  node new_node() 
  {
    new (v_last) node_type(*this);            
    return v_last + 1 != v_end ? v_last++ : v_last;    
  }

  /*
  edge new_edge(node v, node w)
  {
    new (e_last) edge_type(*this,v,w);
    return e_last + 1 == graph_impl::stop_out_edge(v) ? e_last = graph_impl::first_out_edge(v+1) : e_last++;  
  }
  */
  
  edge new_edge(node v, node w)
  {
    edge e = graph_impl::first_out_edge(v);
    edge e_stop = graph_impl::stop_out_edge(v);
    for (; e->target() != v_end && e != e_stop; e++);
   
    if (e == e_stop)
    {
      cout << "error: too new_edge() many new edge operations" << endl;
      exit(1);
    }
    
    new (e) edge_type(*this,v,w);
    return e;
  }
  
  
  void finish_construction() {}
  
  node opposite(node v, edge e) const { return opposite(e,v); }
  node opposite(edge e, node v) const 
  { return v == graph_impl::target(e) ? source(e) : graph_impl::target(e); }
  
  node source(edge e, node=0) const 
  { return v_first + ((char*) e - (char*) v_first) / sizeof(node_type); }

  int index(node v) const { return v - v_first; }
  int index(edge e) const 
  { 
    int idx_v = ((char*) e - (char*) v_first) / sizeof(node_type); // index v
    return (out_degree * idx_v) + (e - graph_impl::first_out_edge(v_first + idx_v)); 
  }

  int  number_of_nodes() const { return v_end - v_first; }
  int  number_of_edges() const { return number_of_nodes() * out_degree; }
 
  int  max_node_index()  const { return number_of_nodes() - 1; }
  int  max_edge_index()  const { return number_of_edges() - 1; }
 
  bool member(node v)    const { return v_first <= v && v < v_end; }
  bool member(edge e)    const { return e_first <= e && e < e_end; }
  
  bool empty() const { return v_first == v_end; }
      
  node first_node() const { return v_first; }
  edge first_edge() const { return e_first; }
  
  node last_node()  const { return v_last; }
  edge last_edge()  const { return e_last; }
  
  node stop_node()  const { return v_end; }
  edge stop_edge()  const { return e_end; }
  
  node next_node(node v) const { return v + 1; }
 
  edge next_edge(edge e) const 
  { 
    node v = source(e);
    return e + 1 == graph_impl::stop_out_edge(v) ? graph_impl::first_out_edge(v+1) : e + 1; 
  }
  
  node succ_node(node v) const { return (v == v_end) ? v_end : v + 1; }
  edge succ_edge(edge e) const { return (e == e_end) ? e_end : next_edge(e); }
      
  list<node> all_nodes() const
  { 
    list<node> L;
    for (node v = v_first; v != v_end; v++) L.append(v);
    return L;
  }

  list<edge> all_edges() const
  { 
    list<edge> L;
    for (edge e = e_first; e != e_end; e = next_edge(e)) L.append(e);
    return L;
  }
  
  
  struct face_dummy 
  { 
    GenPtr Data; 
    GenPtr& data(int) { return Data; } 
  };

  typedef face_dummy  face_type;
  typedef face_dummy* face;
  
  face first_face()    const { return 0; }
  face next_face(face) const { return 0; }
  face stop_face()     const { return 0; }
};


LEDA_END_NAMESPACE

#endif
