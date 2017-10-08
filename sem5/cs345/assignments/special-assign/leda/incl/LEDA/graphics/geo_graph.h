/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geo_graph.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GEO_GRAPH_H
#define LEDA_GEO_GRAPH_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600137
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/graph/graph.h>
#include <LEDA/geo/float_kernel.h>
#include <LEDA/geo/rat_kernel.h>
#include <LEDA/graphics/window.h>

LEDA_BEGIN_NAMESPACE


class __exportC geo_graph_rep : public handle_rep
{
  friend class __exportC geo_graph;

  GRAPH<point,int> gr;
  node_map<int>   kind;

  public:
 
  enum { finite, infinite };

  void copy_graph(graph&, const graph&, node_map<node>&, edge_map<edge>&);

  // constructors
  
  geo_graph_rep();
  // geo_graph_rep(const geo_graph_rep&);

  geo_graph_rep(const graph& G, const node_array<point>& pos, 
                            const edge_array<int>& lab);

  geo_graph_rep(const graph& G, const node_array<rat_point>& pos, 
                            const edge_array<int>& lab);

  // triangulations
  geo_graph_rep(const GRAPH<point,int>& G);
  geo_graph_rep(const GRAPH<rat_point,int>& G);

  // arrangements of segments (result of SWEEP)
  geo_graph_rep(const GRAPH<point,segment>& G);
  geo_graph_rep(const GRAPH<rat_point,rat_segment>& G);

  // voronoi diagrams
  geo_graph_rep(const GRAPH<circle,point>& G);
  geo_graph_rep(const GRAPH<rat_circle,rat_point>& G);

  
  const GRAPH<point,int>& get_graph() const { return gr; }
  // get a reference to the encapsulated graph

  void set_graph(const GRAPH<point,int>& G) { gr = G; }

  void set_position(node v, const point& p, int k = geo_graph_rep::finite)
  // set the position of node v to p
  // k=infinite : infinite node ...
  { gr[v] = p;
    kind[v] = k;
   }

  point get_position(node v) const { return gr[v]; }
  // return the position of node v
  
  bool  is_finite(node v) const    { return kind[v] == finite; }
  // v finite node: true is returned, otherwise false

  bool get_point(node v, point& p) const;

  bool get_segment(edge e, segment& s) const;

  bool get_ray(edge e, ray& r) const;

  bool get_line(edge e, line& l) const;

  void get_objects(list<segment>& slist, list<ray>& rlist, 
                                         list<line>& llist) const;
  // gets the segments, lines and rays of the geo_graph_rep					 
					 
  void translate(double dx, double dy);
  // translates the geo_graph_rep
  
  void rotate(const point& p, double phi);
  // rotates the geo_graph_rep


  friend __exportF window& operator<<(window&, const geo_graph_rep&);
  friend __exportF window& operator>>(window&, geo_graph_rep&);
  
  friend __exportF ostream& operator<<(ostream&, const geo_graph_rep&);
  friend __exportF istream& operator>>(istream&, geo_graph_rep&);

};


class __exportC geo_graph : public HANDLE_BASE(geo_graph_rep)
{

geo_graph_rep* ptr() const { return (geo_graph_rep*)PTR; }

public:

  geo_graph(const geo_graph& gg) : HANDLE_BASE(geo_graph_rep)(gg) {}
 ~geo_graph() {}

  geo_graph& operator=(const geo_graph& gg)
  { HANDLE_BASE(geo_graph_rep)::operator=(gg); return *this; }

  
  geo_graph()
  { PTR = new geo_graph_rep; }

  geo_graph(const graph& G, const node_array<point>& pos, 
                            const edge_array<int>& lab)
  { PTR = new geo_graph_rep(G,pos,lab); }

  geo_graph(const graph& G, const node_array<rat_point>& pos, 
                            const edge_array<int>& lab)
  { PTR = new geo_graph_rep(G,pos,lab); }

  // triangulations
  geo_graph(const GRAPH<point,int>& G)
  { PTR = new geo_graph_rep(G); }

  geo_graph(const GRAPH<rat_point,int>& G)
  { PTR = new geo_graph_rep(G); }

  geo_graph(const GRAPH<point,segment>& G)
  { PTR = new geo_graph_rep(G); }

  geo_graph(const GRAPH<rat_point,rat_segment>& G)
  { PTR = new geo_graph_rep(G); }

  // voronoi diagrams
  geo_graph(const GRAPH<circle,point>& G)
  { PTR = new geo_graph_rep(G); }

  geo_graph(const GRAPH<rat_circle,rat_point>& G)
  { PTR = new geo_graph_rep(G); }

  

  const GRAPH<point,int>& get_graph()
  { return ptr()->get_graph(); }

  point get_position(node v) const
  { return ptr()->get_position(v); }
  
  bool  is_finite(node v) const
  { return ptr()->is_finite(v); }

  bool get_point(node v, point& p) const
  { return ptr()->get_point(v,p); }

  bool get_segment(edge e, segment& s) const
  { return ptr()->get_segment(e,s); }

  bool get_ray(edge e, ray& r) const
  { return ptr()->get_ray(e,r); }

  bool get_line(edge e, line& l) const
  { return ptr()->get_line(e,l); }


  void get_objects(list<segment>& slist, list<ray>& rlist, 
                                         list<line>& llist) const
  { ptr()->get_objects(slist,rlist,llist); }
					 
  geo_graph translate(double, double) const
  { geo_graph gg = *this;
    return gg;
   }
    
  // translates the geo_graph
  
  geo_graph rotate(const point&, double) const
  { geo_graph gg = *this;
    return gg;
   }
  // rotates the geo_graph


  friend __exportF window& operator<<(window& W, const geo_graph& gg)
  { return W << *(gg.ptr()); }

  friend __exportF window& operator>>(window& W, geo_graph& gg)
  { return W >> *(gg.ptr()); }

  friend __exportF ostream& operator<<(ostream& O, const geo_graph& gg)
  { return O << *(gg.ptr()); }

  friend __exportF istream& operator>>(istream& I, geo_graph& gg)
  { return I >> *(gg.ptr()); }

};

LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600137
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
