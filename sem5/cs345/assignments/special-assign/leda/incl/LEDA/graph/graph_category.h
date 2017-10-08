/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_category.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef GRAPH_CATEGORY_NEW_H
#define GRAPH_CATEGORY_NEW_H

#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_cat_enum.h>

LEDA_BEGIN_NAMESPACE


//-------------------------------------------------------------------------- 
// graph_register
//-------------------------------------------------------------------------- 


template <class graph_t>
class graph_register
{
  public:

  typedef graph_map<graph_t>  graph_map_type;
  typedef graph_map<graph_t>* gmap;

  protected:

  list<gmap> map_list[3];  
  list<int>  free_data[3]; 

  public:

  graph_register()
  { int i;
    for (i = 0; i < graph_t::n_data_sl; i++) 
      free_data[0].append(graph_t::n_base_sz + i);
    
    for (i = 0; i < graph_t::e_data_sl; i++) 
      free_data[1].append(graph_t::e_base_sz + i);
   }
  
 ~graph_register()
  { gmap M;
    forall(M, get_map_list(0)) M->set_owner(0);
    forall(M, get_map_list(1)) M->set_owner(0);
   }
  
  // abmelden der maps; freigabe der slots!!
  
  void clear(graph_t& G) 
  {
    typename graph_t::node v = G.first_node(), v_stop = G.stop_node();
    for (; v != v_stop; v = G.next_node(v))
    { 
      gmap M;
      forall(M, get_map_list(0))
      { 
        int i = M->slot_index();
        if (i > -1) 
          M->clear_entry(v->data(i));
      }
    }

    typename graph_t::edge e = G.first_edge(), e_stop = G.stop_edge();
    for (; e != e_stop; e = G.next_edge(e))
    { 
      gmap M;
      forall(M, get_map_list(1)) 
      { 
        int i = M->slot_index();
        if (i > -1) 
          M->clear_entry(e->data(i));
      }
    }
  } 

  const list<gmap>& get_map_list(int i) const { return map_list[i]; }

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
    
    if (m->slot_index() > -1)
      free_data[k].push(m->slot_index());
  }
};


//-------------------------------------------------------------------------- 
// data_slot<slots>
//-------------------------------------------------------------------------- 

template <unsigned x>
class data_slots 
{ 
  GenPtr Data[x];

  public:
  enum { slots = x };
  GenPtr& data(int i) { return Data[i]; }
};

template <>
class data_slots<0>
{
  public:
  enum { slots = 0 };
  GenPtr& data(int i) { return *(GenPtr*) new GenPtr; }
};


//-------------------------------------------------------------------------- 
// Category: directed_graph
//-------------------------------------------------------------------------- 

struct directed_graph
{ 
  template <class traits>
  struct edge_type : public traits::edge_struct, public traits::edge_data
  {
    typedef typename traits::graph       graph;    
    typedef typename traits::edge_struct edge_str;
    typedef typename traits::node_struct node_str;

    typedef edge_type<traits>* edge;
 
    template <class graph>
    edge_type(const graph& G, typename graph::node v, typename graph::node w) 
      : edge_str(G,v,w), traits::edge_data() {}    
    
    edge next_out_edge() const  
    { return (edge) edge_str::next_out_edge(this); }    

    edge pred_out_edge() const  
    { return (edge) edge_str::pred_out_edge(this); }    
    
    LEDA_MEMORY(edge_type<traits>)
  };  
  
  template <class traits>
  struct node_type : public traits::node_struct, public traits::node_data                                      
  {
    typedef typename traits::graph       graph;
    typedef typename traits::edge_struct edge_str;
    typedef typename traits::node_struct node_str;

    typedef node_type<traits>* node;
    typedef edge_type<traits>* edge;
        
    public:
    
    template <class graph>
    node_type(const graph& G) : node_str(G), traits::node_data() {}  
    
    edge first_out_edge() const { return (edge) node_str::first_out_edge(this);}
    edge stop_out_edge()  const { return (edge) node_str::stop_out_edge(this); }

    edge first_out_edge_rev() const { return stop_out_edge()-1; }
    edge stop_out_edge_rev()  const { return first_out_edge()-1; }
    
    LEDA_MEMORY(node_type<traits>)
  };
  

  struct graph
  { 
    typedef directed_graph category_tag;
     
    template <class edge_t>
    typename edge_t::graph::node target(edge_t* e) const 
    { typedef typename edge_t::graph::node node;
      return (node) e->target(e); 
    }
  
    template <class node>
    int outdeg(node v) const { return v->outdeg(v); } 
    
    bool is_directed()   const { return true; }
    bool is_undirected() const { return false; }

    static int category() { return directed_graph_category; }
  };
};


//-------------------------------------------------------------------------- 
// Category: bidirectional_graph
//-------------------------------------------------------------------------- 

struct bidirectional_graph
{
  template <class traits>
  struct edge_type : public traits::edge_struct, public traits::edge_data
  {
    typedef typename traits::graph       graph;    
    typedef typename traits::edge_struct edge_str;
    typedef typename traits::node_struct node_str;

    typedef edge_type<traits>* edge;
 
    template <class graph>
    edge_type(const graph& G, typename graph::node v, typename graph::node w) 
      : edge_str(G,v,w), traits::edge_data() {}    
    
    edge next_out_edge() const { return (edge) edge_str::next_out_edge(this); }
    edge pred_out_edge() const { return (edge) edge_str::pred_out_edge(this); }
    edge next_in_edge()  const { return (edge) edge_str::next_in_edge(this);  }
      
    LEDA_MEMORY(edge_type<traits>)
  };
  

  template <class traits>
  struct node_type : public traits::node_struct, public traits::node_data                                      
  {
    typedef typename traits::graph       graph;
    typedef typename traits::edge_struct edge_str;
    typedef typename traits::node_struct node_str;

    typedef node_type<traits>* node;
    typedef edge_type<traits>* edge;
        
    public:
    
    template <class graph>
    node_type(const graph& G) : node_str(G), traits::node_data() {}  
    
    edge first_out_edge() const { return (edge) node_str::first_out_edge(this);}
    edge stop_out_edge()  const { return (edge) node_str::stop_out_edge(this); }
    
    edge first_out_edge_rev() const { return stop_out_edge()-1; }
    edge stop_out_edge_rev()  const { return first_out_edge()-1; }

    edge first_in_edge() const { return (edge) node_str::first_in_edge(this); }
    edge stop_in_edge()  const { return (edge) node_str::stop_in_edge(this);  }
    
    LEDA_MEMORY(node_type<traits>)
  };
  
  
  struct graph
  {    
    typedef bidirectional_graph category_tag;
    
    template <class edge_t>
    typename edge_t::graph::node target(edge_t* e) const 
    { typedef typename edge_t::graph::node node;
      return (node) e->target(e); 
    }
  
    template <class edge_t>
    typename edge_t::graph::node source(edge_t* e) const 
    { typedef typename edge_t::graph::node node;
      return (node) e->source(e); 
    }
  
    template <class edge, class node>
    node opposite(edge e, node v) const 
    { return target(e) == v ? source(e) : target(e); }
    
    template <class node>
    int outdeg(node v) const { return v->outdeg(v); } 
    
    template <class node>
    int indeg(node v) const { return v->indeg(v); } 
    
    template <class node>
    int degree(node v) const { return indeg(v) + outdeg(v); } 
        
    bool is_directed()   const { return true; }
    bool is_undirected() const { return false; }
    
    static int category() { return bidirectional_graph_category; }
    
    // compatibility  
    
    template <class edge, class node>
    node target(edge e, node) const { return target(e); }
    
    template <class edge, class node>
    node source(edge e, node) const { return source(e); }    
  };
};


//-------------------------------------------------------------------------- 
// Category: opposite_graph
//-------------------------------------------------------------------------- 

struct opposite_graph
{
  template <class traits>
  struct edge_type : public traits::edge_struct, public traits::edge_data
  {
    typedef typename traits::graph       graph;    
    typedef typename traits::edge_struct edge_str;
    typedef typename traits::node_struct node_str;

    typedef edge_type<traits>* edge;
 
    //template <class graph>
    edge_type(const graph& G, typename graph::node v, typename graph::node w) 
      : edge_str(G,v,w), traits::edge_data() {}    
    
    edge next_out_edge() const { return (edge) edge_str::next_out_edge(this); }
    edge pred_out_edge() const { return (edge) edge_str::pred_out_edge(this); }
    edge next_in_edge()  const { return (edge) edge_str::next_in_edge(this);  }
      
    LEDA_MEMORY(edge_type<traits>)
  };
  

  template <class traits>
  struct node_type : public traits::node_struct, public traits::node_data                                      
  {
    typedef typename traits::graph       graph;
    typedef typename traits::edge_struct edge_str;
    typedef typename traits::node_struct node_str;

    typedef node_type<traits>* node;
    typedef edge_type<traits>* edge;
        
    public:
    
    //template <class graph>
    node_type(const graph& G) : node_str(G), traits::node_data() {}  
    
    edge first_out_edge() const { return (edge) node_str::first_out_edge(this);}
    edge stop_out_edge()  const { return (edge) node_str::stop_out_edge(this); }
    
    edge first_out_edge_rev() const { return stop_out_edge()-1; }
    edge stop_out_edge_rev()  const { return first_out_edge()-1; }
    
    edge first_in_edge() const { return (edge) node_str::first_in_edge(this); }
    edge stop_in_edge()  const { return (edge) node_str::stop_in_edge(this);  }
    
    LEDA_MEMORY(node_type<traits>)
  };
  
  
  struct graph 
  { 
    typedef opposite_graph category_tag;
       
    template <class edge, class node>
    node target(edge e, node v) const { return e->target(e,v); }
  
    template <class edge, class node>
    node source(edge e, node v) const { return e->source(e,v); }
  
    template <class edge_t, class node_t>
    node_t* opposite(edge_t* e, node_t* v) const 
    {
      typedef typename node_t::graph graph;

      return e->opposite(*(const graph*)this,e,v); 
     }

    
    template <class node>
    int outdeg(node v) const { return v->outdeg(v); } 
    
    template <class node>
    int indeg(node v) const { return v->indeg(v); } 
    
    template <class node>
    int degree(node v) const { return indeg(v) + outdeg(v); } 

    bool is_directed()   const { return true; }
    bool is_undirected() const { return false; }
    
    static int category() { return opposite_graph_category; }
    
    // compatibility  
    
    template <class edge_t>
    typename edge_t::graph::node target(edge_t* e) const 
    { typedef typename edge_t::graph::node node;
      LEDA_EXCEPTION(1,"opposite_graph::target(e) not available.");
      return 0; 
    }
 
    template <class edge_t>
    typename edge_t::graph::node source(edge_t* e) const 
    { typedef typename edge_t::graph::node node;
      LEDA_EXCEPTION(1,"opposite_graph::source(e) not available.");
      return 0; 
    }
  };
};

LEDA_END_NAMESPACE

#endif
