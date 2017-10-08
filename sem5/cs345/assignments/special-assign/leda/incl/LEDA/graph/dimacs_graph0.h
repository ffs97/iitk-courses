/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dimacs_graph0.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_DIMACS_GRAPH0_H
#define LEDA_DIMACS_GRAPH0_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600158
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graph/graph.h>
#include <LEDA/core/tuple.h>
#include <LEDA/internal/std/stdio.h>

LEDA_BEGIN_NAMESPACE


class dimacs_vertex {

   int   first_adj;
   void* data;

 LEDA_MEMORY(dimacs_vertex)
 friend class __exportC dimacs_base_graph;
};



class dimacs_arc {

   int target;
   int next_adj;
   void* data;

 dimacs_arc() {}
 dimacs_arc(int t, int succ) : target(t), next_adj(succ) {}


 LEDA_MEMORY(dimacs_arc)
 friend class __exportC dimacs_base_graph;
};



class __exportC dimacs_base_graph
{  
  typedef dimacs_arc arc;
  typedef dimacs_vertex vertex;

  char    problem[128];
  int     n;
  int     m;
  vertex* V_vec;
  vertex* V;
  arc*    A;
  arc*    A_stop;

  list<int> vlist;

  virtual void read_node_data(istream&, void*& data) { data = 0; }
  virtual void read_arc_data(istream&, void*& data)  { data = 0; }

  virtual void clear_node_data(void*& data){ LEDA_EXCEPTION(1,"base node arc"); }
  virtual void clear_arc_data(void*& data) { LEDA_EXCEPTION(1,"base clear arc");}

  virtual void print_node_data(ostream& os, void* x) const { os << x; }
  virtual void print_arc_data(ostream& os, void* x)  const { os << x; }
  
public:   

  dimacs_base_graph() {}

  virtual ~dimacs_base_graph();

  void clear_all_node_entries();
  void clear_all_arc_entries();

  int number_of_nodes() const { return n; }
  int number_of_edges() const { return m; }

  int first_adj_edge(int i) const { return V[i].first_adj; }
  int next_adj_edge(int j) const { return A[j].next_adj; }

  int target(int j) const { return A[j].target; }

  void* node_inf(int i) const { return V[i].data; }
  void* edge_inf(int j) const { return A[j].data; }

  string problem_name() { return problem; }


  const list<int>& special_nodes() const { return vlist; }

  int  special_node(int i) 
  { return  (i < vlist.length()) ? vlist[vlist.get_item(i)] : -1; }

  void read(istream& in);
  void print(ostream& out) const;
};





template<class NT, class AT>
class dimacs_graph0 : public dimacs_base_graph {
public:

 void clear_node_data(void*& data) { LEDA_CLEAR(NT,data); } 
 void clear_arc_data(void*& data) { LEDA_CLEAR(AT,data); } 

 void read_node_data(istream& is, void*& data) 
 { NT x; is >> x; data = leda_copy(x); }

 void read_arc_data(istream& is, void*& data) 
 { AT x; is >> x; data = leda_copy(x); }

 NT node_inf(int v) const 
 { return LEDA_CONST_ACCESS(NT,dimacs_base_graph::node_inf(v)); }

 AT edge_inf(int e) const 
 { return LEDA_CONST_ACCESS(AT,dimacs_base_graph::edge_inf(e)); }

 ~dimacs_graph0() 
 { clear_all_node_entries();
   clear_all_arc_entries(); 
  }

};


template <class T, class graph_t>
class dummy_array {

  typedef typename graph_t::node node;
  typedef typename graph_t::edge edge;

  T x;

public:

  typedef T value_type;

void init(const graph_t&,int, const T&) {}
T& operator[](node) { return x; }
T& operator[](edge) { return x; }
};


template<class graph_t, class node_t, class N_array, class E_array,
         class N_array_value_type> // hack for msc6
__temp_func_inline
string read_dimacs_graph0(istream& is, 
                         graph_t& G, 
                         list<node_t>& v_list,
                         N_array& n_label,
                         E_array& e_label,
                         //const typename N_array::value_type& def_val)
                         const N_array_value_type& def_val)
{ 
  typedef typename graph_t::node node;
  typedef typename graph_t::edge edge;

  typedef typename N_array::value_type NT; // node type
  typedef typename E_array::value_type AT; // arc  type


  dimacs_graph0<NT,AT> D;
  D.read(is);

  int n = D.number_of_nodes();
  int m = D.number_of_edges();
  int i;

  v_list.clear();

  n_label.init(G,n,NT());
  e_label.init(G,m,AT());

  G.init(n,m);

  node* V = new node[n];
  node* p = V;
  for(i=0; i<n; i++) *p++ = G.new_node();


  for(i=0; i<n; i++)
  { node v = V[i];
    n_label[v] = def_val;
    int j = D.first_adj_edge(i);
    while (j != -1)
    { node w = V[D.target(j)];
      edge e = G.new_edge(v,w);
      e_label[e] = D.edge_inf(j);
      j = D.next_adj_edge(j);
     }
  } 

  forall(i,D.special_nodes()) 
  { node v = V[i];
    v_list.append(v);
    n_label[v] = D.node_inf(i);
   }
  
  G.finish_construction();

  delete[] V;

  return D.problem_name();
}


#if LEDA_ROOT_INCL_ID == 600158
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

