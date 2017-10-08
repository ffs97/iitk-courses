/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  series_parallel.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SERIES_PARALLEL_H
#define LEDA_SERIES_PARALLEL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600215
#include <LEDA/internal/PREAMBLE.h>
#endif


// ------------------------------------------------------------ //
// recognition and decomposition of series-parallel digraph    //
// using the algorithm by Valdes, Tarjan, and Lawler,           //
// using the canonical decomposition tree                       //
//                                                              //
// David Alberts, 1997                                          //
// ------------------------------------------------------------ //

#include<LEDA/graph/graph.h>
#include<LEDA/core/list.h>
#include<LEDA/core/stack.h>
#include<LEDA/system/stream.h>

LEDA_BEGIN_NAMESPACE

// ------------------------------------------------------------ //
// representation of the parse tree                             //
// ------------------------------------------------------------ //

class   spq_tree;
class   spq_node_struct;
typedef spq_node_struct* spq_node;

class embed_result;
class embed_3dresult;

class spq_node_struct
{
  //private:
  public:

    char type;               // 'S', 'P', 'Q', or 'I' (invalid)
    edge e;                  // in case of a Q node the corresponding edge
    list<spq_node> children; // the children of this node

  public:

    // constructors
    spq_node_struct()       { type = 'I'; }
    spq_node_struct(edge f) { type = 'Q'; e = f; }
    spq_node_struct(bool series, spq_node l, spq_node r)
    {
      type = (series ? 'S' : 'P');
      children.append(l);
      children.append(r);
    }

    // clearing an SPQ-tree on the heap
    void clear();


    // data member accessors
    edge get_edge() { return e; }
    char get_type() { return type; }

    // output
    friend ostream& operator<< (ostream& out, spq_node s);

    friend istream& operator>>(istream& in, spq_node) { return in;  }

    friend class spq_tree;
    friend void g_add_spqnode(graph&, spq_node, node, node);
    friend void spq_embed(spq_node, embed_result&, double);
    friend void spq_3dembed(spq_node, embed_3dresult&, double);


    LEDA_MEMORY(spq_node_struct)
};

class spq_tree
{
  public:

    LEDA_MEMORY(spq_tree)

    spq_tree(const graph& G) { root = 0; init(G); }
    spq_tree()               { root = 0; the_is_series_parallel = false; }
    virtual ~spq_tree()
    { edge e; forall_edges(e,GC) if(GC[e]) GC[e]->clear(); }

    void init(const graph& G);
    bool is_series_parallel() { return the_is_series_parallel; }

    node build_tree(GRAPH<string,int>& G);
    bool remaining_graph(GRAPH<node,spq_node>& G);

    void print(ostream& out = cout) { if(root) out << root << "\n"; }
    string description()
    { string_ostream s; if(root) { s << root; } return s.str(); }

    spq_node get_root_node() { return root; }

  protected:

    spq_node root;
    GRAPH<node,spq_node> GC;
    bool the_is_series_parallel;

    bool get_st(const graph& G, node& s, node& t);
    void parallel_reduction(edge e, edge f);
    edge series_reduction(edge e, edge f, node u, node v, node w);
    void build_tree_rec(spq_node s, GRAPH<string,int>& G, node last);
    void sp_reduce(node s, node t, stack<node>& nodes);

    list<spq_node> get_children(spq_node v) { return v->children; }

   // generator
   friend bool random_sp_graph(graph&, spq_tree&, int, int);


};


// checker

inline bool Is_Series_Parallel(const graph& G, spq_tree& spqt)
{ spq_tree _spqt(G); spqt = _spqt; return _spqt.is_series_parallel(); }




#if LEDA_ROOT_INCL_ID == 600215
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

