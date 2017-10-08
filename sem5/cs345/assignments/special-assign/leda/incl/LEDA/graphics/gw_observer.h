/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gw_observer.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GW_OBSERVER_H
#define LEDA_GW_OBSERVER_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600138
#include <LEDA/internal/PREAMBLE.h>
#endif

//#include <LEDA/graphics/graphwin.h>

LEDA_BEGIN_NAMESPACE

class node_position_base {
public:
  node_position_base() {}
  virtual ~node_position_base() {}
  virtual point operator()(node v) const = 0;
};


template<class point_type, class etype>
class  graph_node_position : public node_position_base 
{
  const GRAPH<point_type,etype>& G;
public:
  graph_node_position(const GRAPH<point_type,etype>& gr) : G(gr) {}
  point operator()(node v) const { return G[v].to_float(); }
};




class gw_observer : public GraphWin {

  int         e_num;
  event_item* e_vec;

  node   del_node;

  void  (*draw_graph)(GraphWin&);

  const node_position_base* node_pos;

  bool online;

  vector get_vector(node v) const 
  { if (node_pos == 0) 
      return get_graph().get_position(v);
    else
      return (*node_pos)(v).to_vector();
   }


  void new_node1(graph&);
  void new_node2(graph&, node);
  void del_node1(graph&, node);
  void del_node2(graph&);
  void new_edge1(graph&, node, node);
  void new_edge2(graph&, edge);
  void del_edge1(graph&, edge);
  void del_edge2(graph&, node, node);
  void move_edge1(graph&, edge, node, node);
  void move_edge2(graph&, edge, node, node);
  void clr_graph1(graph&);
  void clr_graph2(graph&);
  void layout_changed(GraphWin& gw, const list<node>& V, const list<edge>& E);

  void init_events(graph&);

  public :

  void set_online(bool b) { online = b; }

  void update_layout();
  void update_layout(GraphWin& gw);

  gw_observer(graph& G, int w=400, int h=400, const char* label="");

  gw_observer(graph& G, const node_position_base& pos, int w=400, int h=400, 
                                                       const char* label="");

  gw_observer(GraphWin& gw, int w=400, int h=400, const char* label="");

 ~gw_observer() { detach(e_vec,e_num); delete[] e_vec; }

  void set_draw_func(void (*draw)(GraphWin&)) { draw_graph = draw; }

};



#if LEDA_ROOT_INCL_ID == 600138
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
