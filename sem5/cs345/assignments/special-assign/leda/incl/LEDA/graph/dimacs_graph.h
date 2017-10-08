/*******************************************************************************
+
+  leda 5.1  
+
+
+  dimacs_graph.h
+
+
+  copyright (c) 1995-2010
+  by algorithmic solutions software gmbh
+  all rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_DIMACS_GRAPH_H
#define LEDA_DIMACS_GRAPH_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600180
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graph/graph.h> 
#include <LEDA/core/tuple.h>
#include <LEDA/system/assert.h>
#include <LEDA/system/http.h>


LEDA_BEGIN_NAMESPACE

class __exportC dimacs_graph
{  

enum { max_n_slot=1, max_a_slot=3 };

protected:

  struct vertex {
     int first_adj;
     int last_adj;
     int data[1];
     vertex() : first_adj(-1), last_adj(-1) {}
  };
  
  
  struct arc {
     int target;
     int next_adj;
     int data[1];
   arc(int t) : target(t), next_adj(-1) {}
   LEDA_MEMORY(arc)
  };



  char      problem[128];
  int       n;
  int       m;
  int*      V;
  int*      A;
  int       n_slots;
  int       a_slots;
  int       n_sz;
  int       a_sz;
  int       min_i;
  int       max_i;

  list<int> vlist;

  int       min_node_data[max_n_slot];
  int       max_node_data[max_n_slot];

  int       min_edge_data[max_a_slot];
  int       max_edge_data[max_a_slot];

  void parse_line(const char* line, int& cur_arc);

  vertex* V_get(int i) const { return  (vertex*)(V+i*n_sz); }
  arc*    A_get(int i) const { return  (arc*)   (A+i*a_sz); }


public:   

  dimacs_graph(int ns, int as);
 ~dimacs_graph();

  string problem_name() const { return problem; }
  int number_of_nodes() const { return n; }
  int number_of_edges() const { return m; }

  int first_adj_edge(int v)  const { return V_get(v)->first_adj; }
  int last_adj_edge(int v)   const { return V_get(v)->last_adj; }
  int next_adj_edge(int e)   const { return A_get(e)->next_adj; }
  int target(int v)          const { return A_get(v)->target; }
  int node_inf(int v, int i) const { return V_get(v)->data[i]; }
  int edge_inf(int e, int i) const { return A_get(e)->data[i]; }

  const list<int>& special_nodes() const { return vlist; }

  void read(string fn);
  void read(istream&);
  void read(http&);

  void read() { read(""); }

  void print(ostream& out) const;
  void print_statistics(ostream& out) const;

  template<class graph_t, class node_t, class edge_t>
  void build_graph(graph_t& G, node_t* N, edge_t* E) const
  { 
    G.init(n,m);
  
    int i;
    for(i=0; i<n; i++) N[min_i+i] = G.new_node();
  
    for(i=min_i; i<=max_i; i++)
    { node_t v = N[i];
      int j = first_adj_edge(i);
      while (j != -1)
      { node_t w = N[target(j)];
        edge_t e = G.new_edge(v,w);
        E[j] = e;
        j = next_adj_edge(j);
       }
    } 
  
    G.finish_construction();
  }

  template<class graph_t, class node_t, class edge_t>
  void build_st_graph(graph_t& G, node_t* N, edge_t* E) const
  { 
    G.init(n+2,m+2*n);

    node_t s = G.new_node();
  
    int i;
    for(i=0; i<n; i++) N[min_i+i] = G.new_node();

    node_t t = G.new_node();

    for(i=min_i; i<=max_i; i++) G.new_edge(s,N[i]);
  
    for(i=min_i; i<=max_i; i++)
    { node_t v = N[i];
      int j = first_adj_edge(i);
      while (j != -1)
      { node_t w = N[target(j)];
        edge_t e = G.new_edge(v,w);
        E[j] = e;
        j = next_adj_edge(j);
       }
      G.new_edge(v,t);
    } 
    G.finish_construction();

  
assert(t == G.last_node());
  }
  
  
};



class dimacs_sp : public dimacs_graph {

public:

  dimacs_sp()            : dimacs_graph(0,1) {}
  dimacs_sp(string fn)   : dimacs_graph(0,1) { read(fn); }
  dimacs_sp(istream& is) : dimacs_graph(0,1) { read(is); }
  dimacs_sp(http& ht)    : dimacs_graph(0,1) { read(ht); }


  template<class graph_t, class node_t, class cost_array>
  void translate(graph_t& G, node_t& s, cost_array& cost)
  { 
    typedef typename graph_t::node node;
    typedef typename graph_t::edge edge;
  
    string name = problem_name();
  
    if (name!= "sp")
      LEDA_EXCEPTION(1,
       string("read_dimacs_sp: non-matching problem line (\"%s\").",~name));
  
    //int n = number_of_nodes();
    //int m = number_of_edges();
  
    node* N = new node[dimacs_graph::n+1];
    edge* E = new edge[dimacs_graph::m];
  
    build_graph(G,N,E);
    cost.init(G);
  
    for(int x = 0; x < dimacs_graph::m; x++) cost[E[x]] = edge_inf(x,0);
  
    s = N[special_nodes().head()];
  
    delete[] N;
    delete[] E;
  }
};




class dimacs_mf : public dimacs_graph {

public:

  dimacs_mf()            : dimacs_graph(0,1) {}
  dimacs_mf(string fn)   : dimacs_graph(0,1) { read(fn); }
  dimacs_mf(istream& is) : dimacs_graph(0,1) { read(is); }
  dimacs_mf(http& ht)    : dimacs_graph(0,1) { read(ht); }

  template<class graph_t, class node_t, class cap_array>
  void translate(graph_t& G, node_t& s, node_t& t, cap_array& cap)
  { 
    typedef typename graph_t::node node;
    typedef typename graph_t::edge edge;
  
    string name = problem_name();
  
    if (name != "max")
      LEDA_EXCEPTION(1,
       string("read_dimacs_mf: non-matching problem line (\"%s\").",~name));
  
    if (special_nodes().length() != 2)
      LEDA_EXCEPTION(1,"read_dimacs_mf: wrong number of node lines.");
  
    //int n = number_of_nodes();
    //int m = number_of_edges();
  
    node* N = new node[dimacs_graph::n+1];
    edge* E = new edge[dimacs_graph::m];
  
    build_graph(G,N,E);
    cap.init(G);
  
    for(int x = 0; x < dimacs_graph::m; x++) cap[E[x]] = edge_inf(x,0);
  
    s = N[special_nodes().head()];
    t = N[special_nodes().tail()];
  
    delete[] N;
    delete[] E;
  }

};






class dimacs_mcf : public dimacs_graph {

public:

  dimacs_mcf()            : dimacs_graph(1,3) {}
  dimacs_mcf(string fn)   : dimacs_graph(1,3) { read(fn); }
  dimacs_mcf(istream& is) : dimacs_graph(1,3) { read(is); }
  dimacs_mcf(http& ht)    : dimacs_graph(1,3) { read(ht); }


  template<class graph_t, class supply_array, class lcap_array, 
                          class ucap_array, class cost_array>
  void translate(graph_t& G, supply_array& supply, lcap_array& lcap,
                                                   ucap_array& ucap,
                                                   cost_array& cost)
  { 
    typedef typename graph_t::node node;
    typedef typename graph_t::edge edge;
  
    string name = problem_name();
  
    if (name != "min")
      LEDA_EXCEPTION(1,
       string("read_dimacs_mcf: non-matching problem line (\"%s\").",~name));
  
    //int n = number_of_nodes();
    //int m = number_of_edges();
  
    node* N = new node[dimacs_graph::n+1];
    edge* E = new edge[dimacs_graph::m];
  
    build_graph(G,N,E);
  
    supply.init(G,0);
    lcap.init(G);
    ucap.init(G);
    cost.init(G);
  
    int i;
    forall(i,special_nodes()) supply[N[i]] = node_inf(i,0);
  
    for(int j = 0; j <dimacs_graph::m; j++) {
       edge e = E[j];
       lcap[e] = edge_inf(j,0);
       ucap[e] = edge_inf(j,1);
       cost[e] = edge_inf(j,2);
    }
  
    delete[] N;
    delete[] E;
  }

  template<class graph_t, class supply_array, class lcap_array, 
                          class ucap_array, class cost_array>
  void translate_st(graph_t& G, typename graph_t::node& s, 
                                typename graph_t::node& t, 
                                supply_array& supply, 
                                lcap_array& lcap,
                                ucap_array& ucap,
                                cost_array& cost)
  { 
    typedef typename graph_t::node node;
    typedef typename graph_t::edge edge;
  
    string name = problem_name();
  
    if (name != "min")
      LEDA_EXCEPTION(1,
       string("read_dimacs_mcf: non-matching problem line (\"%s\").",~name));
  
    //int n = number_of_nodes();
    //int m = number_of_edges();
  
    node* N = new node[dimacs_graph::n+1];
    edge* E = new edge[dimacs_graph::m];
  
    build_st_graph(G,N,E);
  
    supply.init(G,0);
    lcap.init(G,0);
    ucap.init(G,0);
    cost.init(G,0);
  
    int i;
    forall(i,special_nodes()) supply[N[i]] = node_inf(i,0);
  
    for(int j = 0; j <dimacs_graph::m; j++) {
       edge e = E[j];
       lcap[e] = edge_inf(j,0);
       ucap[e] = edge_inf(j,1);
       cost[e] = edge_inf(j,2);
    }


    s= G.first_node();
    t= G.last_node();

    supply[s] = supply[t] = 0;

/*
    edge e;
    forall_out_edges(e,s)
    { node v = G.opposite(e,s);
      lcap[e] = 0;
      cost[e] = 0;
      int sup = supply[v];
      if (sup > 0)
         { ucap[e] = sup;
           supply[s] += sup;
          }
      else
         ucap[e] = 0;
     }

    forall_in_edges(e,t)
    { node v = G.opposite(e,t);
      lcap[e] = 0;
      cost[e] = 0;
      int sup = supply[v];
      if (sup < 0)
         { ucap[e] = -sup;
           supply[t] += sup;
          }
      else
         ucap[e] = 0;
     }


    forall_nodes(v,G)
      if (v != s && v != t) supply[v] = 0;

*/
    int count = 0;
    edge e;
    forall_in_edges(e,t) count++;

    assert(count == n);

  
    delete[] N;
    delete[] E;
  }


};


class dimacs_mat : public dimacs_graph {

public:

  dimacs_mat()            : dimacs_graph(0,1) {}
  dimacs_mat(string fn)   : dimacs_graph(0,1) { read(fn); }
  dimacs_mat(istream& is) : dimacs_graph(0,1) { read(is); }
  dimacs_mat(http& ht)    : dimacs_graph(0,1) { read(ht); }

  template<class graph_t, class cost_array>
  void translate(graph_t& G, cost_array& cost)
  { 
    typedef typename graph_t::node node;
    typedef typename graph_t::edge edge;
  
    string name = problem_name();
  
    if (name!= "mat")
      LEDA_EXCEPTION(1,
       string("read_dimacs_mat: non-matching problem line (\"%s\").",~name));
  
    //int n = number_of_nodes();
    //int m = number_of_edges();
  
    node* N = new node[dimacs_graph::n+1];
    edge* E = new edge[dimacs_graph::m];
  
    build_graph(G,N,E);
    cost.init(G);
  
    for(int x = 0; x < dimacs_graph::m; x++) cost[E[x]] = edge_inf(x,0);
  
    delete[] N;
    delete[] E;
  }
  
};



template<class graph_t, class node_t, class cost_array>
void read_dimacs_sp(istream& is, graph_t& G, node_t& s, cost_array& cost)
{ dimacs_sp D(is); 
  D.translate(G,s,cost);
}


template<class graph_t, class node_t, class cap_array>
void read_dimacs_mf(istream& is, graph_t& G, node_t& s, node_t& t, cap_array& cap)
{ dimacs_mf D(is);
  D.translate(G,s,t,cap);
}


template<class graph_t, class sup_array, class lcap_array, class ucap_array,
                                                           class cost_array>
void read_dimacs_mcf(istream& is, graph_t& G, sup_array& supply,
                                              lcap_array& lcap,
                                              ucap_array& ucap,
                                              cost_array& cost)
{ dimacs_mcf D(is);
  D.translate(G,supply,lcap,ucap,cost);
}



template<class graph_t, class cost_array>
void read_dimacs_mat(istream& is, graph_t& G, cost_array& cost)
{ dimacs_mat D(is);
  D.translate(G,cost);
}





// writing dimacs formats

#define TARGET(G,e,s)\
((graph_t::category()==opposite_graph_category) ? G.opposite(e,s):G.target(e))


template <class graph_t, class node_t, class cost_array> 
__temp_func_inline
void write_dimacs_sp(ostream& out, const graph_t& G, 
                                   node_t s, 
                                   const cost_array& cost)
{
  typedef typename graph_t::node node;
  typedef typename graph_t::edge edge;

  int n = G.number_of_nodes();
  int m = G.number_of_edges();

  node_array<int,graph_t> n_id(G);
  int count = 0;

  node v;
  forall_nodes(v,G) n_id[v] = ++count;
  
  out << "c LEDA shortest path problem" << endl;
  out << "c" << endl;
  out << string("p sp %d %d",n,m) << endl;
  out << "c" << endl;
  out << string("n %d",n_id[s]) << endl;
  out << "c" << endl;
  
  forall_nodes(v,G)
  { int i = n_id[v];
    edge e;
    forall_out_edges(e,v)
    { int j = n_id[TARGET(G,e,v)];
      out << "a " << i << " " << j << " " <<  cost[e] << endl;
     }
   }
}


template <class graph_t, class node_t, class cap_array> 
__temp_func_inline
void write_dimacs_mf(ostream& out, const graph_t& G, 
                                   node_t s, 
                                   node_t t, 
                                   const cap_array& cap)
{
  typedef typename graph_t::node node;
  typedef typename graph_t::edge edge;

  int n = G.number_of_nodes();
  int m = G.number_of_edges();

  node_array<int,graph_t> n_id(G);
  int count = 0;

  node v;
  forall_nodes(v,G) n_id[v] = ++count;
  
  out << "c maxflow problem" << endl;

  out << string("p max %d %d",n,m) << endl;
  out << string("n %d s",n_id[s]) << endl;
  out << string("n %d t",n_id[t]) << endl;

  forall_nodes(v,G)
  { int i = n_id[v];
    edge e;
    forall_out_edges(e,v)
    { int j = n_id[TARGET(G,e,v)];
      out << "a " << i << " " << j << " " <<  cap[e] << endl;
     }
   }
}


template<class graph_t, class supply_array, class lcap_array,
                                            class ucap_array,
                                            class cost_array>
__temp_func_inline
void write_dimacs_mcf(ostream& out, const graph_t& G, 
                                    const supply_array& supply,
                                    const lcap_array& lcap,
                                    const ucap_array& ucap,
                                    const cost_array& cost)
{
  typedef typename graph_t::node node;
  typedef typename graph_t::edge edge;
 
  int n = G.number_of_nodes();
  int m = G.number_of_edges();

  node_array<int,graph_t> n_id(G);
  int count = 0;
 
  out << "c LEDA MinCostFlow problem" << endl;
  out << "p min " << n << " " << m << endl;

  node v;
  forall_nodes(v,G)
  { n_id[v] = ++count;
    if (supply[v] == 0) continue;
    out << "n " << n_id[v] << " " << supply[v] << endl;
   }

  forall_nodes(v,G)
  { int i = n_id[v];
    edge e;
    forall_out_edges(e,v)
    { int j = n_id[TARGET(G,e,v)];
      out << "a " << i << " " << j << " " ;
      out << lcap[e] << " " << ucap[e] << " " << cost[e] << endl;
     }
   }
}


template <class graph_t, class cost_array> 
__temp_func_inline
void write_dimacs_mat(ostream& out, const graph_t& G, 
		                    const cost_array& cost)
{
  typedef typename graph_t::node node;
  typedef typename graph_t::edge edge;

  int n = G.number_of_nodes();
  int m = G.number_of_edges();

  node_array<int,graph_t> n_id(G);
  int count = 0;

  node v;
  forall_nodes(v,G) n_id[v] = ++count;
  
  out << "c LEDA matching problem (edge adjacencies)" << endl;
  out << "c" << endl;
  out << "p edge " << n << " " << m << endl;
  out << "c" << endl;

  forall_nodes(v,G)
  { int i = n_id[v];
    edge e;
    forall_out_edges(e,v)
    { int j = n_id[TARGET(G,e,v)];
      out << "e " << i << " " << j << " " <<  cost[e] << endl;
     }
   }
}


LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600180
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif


