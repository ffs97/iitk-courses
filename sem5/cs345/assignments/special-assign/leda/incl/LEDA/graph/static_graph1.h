/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  static_graph.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef STATIC_GRAPH_H
#define STATIC_GRAPH_H

#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_category.h>
#include <LEDA/system/assert.h>

LEDA_BEGIN_NAMESPACE


/*{\Manpage {static_graph} {} {Static Graphs}}*/

/*{\Mdefinition

\paragraph{1.1 Motivation.}

The data type $static\_graph$ representing static graph is the result of
two observations:

First, most graph algorithms do not change the underlying graph, they work on
a constant or static graph and second, different algorithms are based
on different models (we call them {\em categories})
of graph.

The LEDA data type |graph| represents all types of graph used in the
library,
such as directed, undirected, and bidirected graph, networks, planar
maps,
and geometric graph. It provides the operations for all of
these graph in one
fat interface. For efficiency reasons it makes sense to provide
special graph data types for special purposes.
The template data type |static_graph|, which is parameterized
with the graph category, provides specialized implementations for some
of these graph types.

\paragraph{1.2 Static Graphs.}
A static graph consists of a fixed sequence of nodes and edges. The
parameterized
data type |static_graph<category, node_data, edge_data>| is used to represent
static graph. The first template
parameter |category| defines the graph category and is taken from
\{|directed_graph, bidirectional_graph, opposite_graph|\} (see 1.3 for
the details). The last two
parameters are optional and
can be used to define user-defined data structures to be included into
the node
and edge objects (see 1.4 for the details).
An instance $G$ of the parameterized data type
$static\_graph$
contains a sequence $V$ of nodes and a sequence $E$ of edges. New nodes
or edges can be appended only in a construction phase which has to be
started by calling |G.start_construction()| and terminated by
|G.finish_construction()|.
For every node or edge $x$ we define $index(x)$ to be equal to the rank
of
$x$ in its sequence. During the construction phase, the sequence of the
source
node index of all inserted edges must be non-decreasing. After the
construction phase both sequences $V$ and $E$ are fixed.


\paragraph{1.3 Graph Categories.}

We distinguish between five categories where currently only the first
three are supported by |static_graph|:

\begin{itemize}
\item Directed Graphs (|directed_graph|)
represent the concept of a directed graph by providing the ability to
iterate over
all edges incident to a given node $v$ and to ask for the target node of
a given
edge $e$.

\item Bidirectional Graphs (|bidirectional_graph|)
extend directed graph by supporting in addition iterations over all
incoming edges
at a given node $v$ and to ask for the source node of a given edge $e$.

\item Opposite Graphs (|opposite_graph|)
are a variant of the bidirectional graph category. They do not support
the computation
of the source or target node of a given edge but allow to walk from
one
terminal $v$ of an edge $e$ to the other {\em opposite one}.
\end{itemize}

Not yet implemented are bidirected and undirected graph.



\paragraph{1.4 Node and Edge Data.}

Static graph support several efficient ways - efficient compared to using
|node_arrays|, |edge_arrays|, |node_maps|, and
|edge_maps| - to associate data with
the edges and nodes of the graph.

\paragraph{1.4.1 Dynamic Slot Assignment:}

It is possible to attach two optional template parameters |data_slots<int>|
at compile time:

\begin{Mverb}
static_graph<directed_graph, data_slots<3>, data_slots<1> > G;
\end{Mverb}

specifies a static directed graph $G$ with three additional node slots and one
additional edge slot.
Node and edge arrays can use these data slots,
instead of allocating an external array. This method is also supported for
the standard LEDA data type |graph|. Please see the manual page for
|node_array| resp. |edge_array| (esp. the operations |use_node_data|
resp. |use_edge_data|) for the details.

The method is called {\em dynamic slot assignment} since the concrete
arrays are assigned during runtime to the slots.

\paragraph{1.4.2 Static Slot Assignment:}

This method is even more efficient. A variant of the node and edge arrays,
the so-called |node_slot| and |edge_slot| data types, are assigned to
the slots during compilation time. These types take three parameters: the
element type of the array, an integer slot number, and the type of
the graph:

\begin{Mverb}
node_slot<E, graph_t, slot>;
edge_slot<E, graph_t, slot>;
\end{Mverb}

Here is an example for the use of static slot assignment in a maxflow
graph algorithm. It uses three node slots for storing distance, excess,
and a successor node, and two edge slots for storing the flow
and capacity.

\begin{Mverb}
typedef static_graph<opposite_graph, data_slots<3>, data_slots<2> > maxflow_graph;
node_slot<node, maxflow_graph, 0> succ;
node_slot<int, maxflow_graph, 1> dist;
node_slot<edge, maxflow_graph, 2> excess;
edge_slot<int, maxflow_graph, 0> flow;
edge_slot<int, maxflow_graph, 1> cap;
\end{Mverb}

When using the data types |node_slot| resp. |edge_slot| one has to include
the files |LEDA/graph/edge_slot.h|.

\paragraph{1.4.3 Customizable Node and Edge Types:}

It is also possible to pass any structure derived from |data_slots<int>|
as second or third parameter. Thereby the nodes and edges are extended by
{\em named} data members. These are added in addition to the data slots
specified in the base type. In the example

\begin{Mverb}
struct flow_node:public data_slots<1>
{ int excess;
  int level;
}

struct flow_edge:public data_slots<2>
{ int flow;
  int cap;
}

typedef static_graph<bidirectional_graph, flow_node, flow_edge> flow_graph;
\end{Mverb}

there are three data slots (one of them unnamed) associated with each node and
four data slots (two of them unnamed) associated with each edge of a
|flow_graph|.

The named slots can be used as follows:

\begin{Mverb}
flow_graph::node v;
forall_nodes(v, G) v->excess = 0;
\end{Mverb}

\bigskip
}*/


/*{\Mcreation G}*/


/*{\Mtext
$static\_graph\<category, node\_data = data\_slots<0>, edge_data = data\_slots<0>$ $>$ $G$;}*/

/*{\Mtext  creates an empty static graph $G$. $category$ is either |directed_graph|,
or |bidirectional_graph|, or |opposite_graph|.
The use of the other parameters is explained in the section {\em Node and Edge
Data} given above.
}*/

/*{\Mtypes}*/
/*{\Moptions nextwarning=no }*/
/*
typedef typename cat::node_type<traits>::node node;
*/
/*{\Mtypemember the node type. \textbf{Note:} It is different from |graph::node|.}*/
/*{\Moptions nextwarning=no }*/
/*
typedef typename cat::node_type<traits>::edge edge;
*/
/*{\Mtypemember the edge type. \textbf{Note:} It is different from |graph::edge|.}*/


/*{\Moperations 4.0 4.0 }*/

/*{\Mtext The interface consists of two parts. The first part - the basic
interface - is independent
from the actual graph category, the specified operations are common to all
graph. The second part of the interface is different for every category
and contains macros to iterate over incident edges or adjacent nodes and methods
for traversing a given edge.}*/

/*{\Moptions nextwarning=no }*/
/*
void start_construction(int n, int m);
*/

/*{\Mop starts the construction phase for a graph with up to $n$ nodes and
$m$ edges.}*/

/*{\Moptions nextwarning=no }*/
/*
node new_node();
*/

/*{\Mop creates a new node, appends it to $V$, and returns it. The operation
may only be called during construction phase and at most $n$ times.}*/

/*{\Moptions nextwarning=no }*/
/*
edge new_edge(node v, node w);
*/

/*{\Mop creates the edge $(v,w)$, appends it to $E$, and returns it. The operation
may only be called during construction phase and at most $m$ times.\\
\precond All edges $(u,v)$ of $G$ with $index(u) < index(v)$ have been created
before.}*/

/*{\Moptions nextwarning=no }*/
/*
void finish_construction();
*/

/*{\Mop terminates the construction phase.}*/


/*{\Moptions nextwarning=no }*/
/*
forall_nodes(v,G);
*/
/*{\Mfunc $v$ iterates over the node sequence.}*/

/*{\Moptions nextwarning=no }*/
/*
forall_edges(e,G);
*/
/*{\Mfunc $e$ iterates over the edge sequence.}*/

/*{\Mtext
{\bf Static Directed Graphs (static\_graph\<directed\_graph\>)}}*/

/*{\Mtext
For this category the basic interface of |static_graph| is extended by the operations:}*/

/*{\Moptions nextwarning=no }*/
/*
node target(edge e);
*/
/*{\Mop returns the target node of $e$.}*/

/*{\Moptions nextwarning=no }*/
/*
node outdeg (node v);
*/
/*{\Mop returns the number of outgoing edges of $v$.}*/

/*{\Moptions nextwarning=no }*/
/*
forall_out_edges(e,v);
*/
/*{\Mfunc $e$ iterates over all edges with $source(e) = v$.}*/

/*{\Mtext
{\bf Static Bidirectional Graphs (static\_graph\<bidirectional\_graph\>)}}*/

/*{\Mtext
For this category the basic interface of |static_graph| is extended by the operations:}*/

/*{\Moptions nextwarning=no }*/
/*
node target(edge e);
*/
/*{\Mop returns the target node of $e$.}*/

/*{\Moptions nextwarning=no }*/
/*
node source(edge e);
*/
/*{\Mop returns the source node of $e$.}*/

/*{\Moptions nextwarning=no }*/
/*
node outdeg (node v);
*/
/*{\Mop returns the number of outgoing edges of $v$.}*/

/*{\Moptions nextwarning=no }*/
/*
node indeg (node v);
*/
/*{\Mop returns the number of incoming edges of $v$.}*/

/*{\Moptions nextwarning=no }*/
/*
forall_out_edges(e,v);
*/
/*{\Mfunc $e$ iterates over all edges with $source(e) = v$.}*/

/*{\Moptions nextwarning=no }*/
/*
forall_in_edges(e,v);
*/
/*{\Mfunc $e$ iterates over all edges with $target(e) = v$.}*/

/*{\Mtext
{\bf Static Opposite Graphs (static\_graph\<opposite\_graph\>)}}*/

/*{\Mtext
For this category the basic interface of |static_graph| is extended by the operations:}*/

/*{\Moptions nextwarning=no }*/
/*
node opposite (edge e, node v);
*/
/*{\Mop returns the opposite to $v$ along $e$.}*/


/*{\Moptions nextwarning=no }*/
/*
node outdeg (node v);
*/
/*{\Mop returns the number of outgoing edges of $v$.}*/

/*{\Moptions nextwarning=no }*/
/*
node indeg (node v);
*/
/*{\Mop returns the number of incoming edges of $v$.}*/

/*{\Moptions nextwarning=no }*/
/*
forall_out_edges(e,v);
*/
/*{\Mfunc $e$ iterates over all edges with $source(e) = v$.}*/

/*{\Moptions nextwarning=no }*/
/*
forall_in_edges(e,v);
*/
/*{\Mfunc $e$ iterates over all edges with $target(e) = v$.}*/


/*{\Mexample
The simple example illustrates how to create a small
graph and assign some values.
To see how static graph can be used
in a max flow algorithm - please see the source file |mfs.c|
in the directory test\slash flow.


\begin{Mverb}
#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_slot.h>
#include <LEDA/graph/edge_slot.h>
#include <LEDA/core/array.h>

using namespace leda;

struct node_weight:public data_slots<0>
{ int weight; }

struct edge_cap:public data_slots<0>
{ int cap; }

typedef static_graph<opposite_graph, node_weight, edge_cap> static_graph;
typedef static_graph::node st_node;
typedef static_graph::edge st_edge;


int main ()
{
   static_graph G;
   array<st_node> v(4);
   array<st_edge> e(4);
   G.start_construction(4,4);

   for(int i =0; i < 4; i++) v[i] = G.new_node();

   e[0] = G.new_edge(v[0], v[1]);
   e[1] = G.new_edge(v[0], v[2]);
   e[2] = G.new_edge(v[1], v[2]);
   e[3] = G.new_edge(v[3], v[1]);
   G.finish_construction();
   st_node v;
   st_edge e;
   forall_nodes(v, G) v->weight = 1;
   forall_edges(e, G) e->cap = 10;

   return 0;
}
\end{Mverb}

}*/

template <class category> class  st_node_struct {};
template <class category> class  st_edge_struct {};


// ------------------------------------------------------------------------- 
// Category: directed_graph
// ------------------------------------------------------------------------- 

template <>
class st_edge_struct<directed_graph>
{
  protected:
  
  GenPtr Target;
  
  public:
  
  template <class graph>
  st_edge_struct(const graph& G, typename graph::node v, typename graph::node w)
  { 
    Target = w;
    v->set_out_edge(this);
  }
  
  template <class edge>
  edge next_out_edge(edge e) const { return e + 1; }  

  template <class edge>
  edge pred_out_edge(edge e) const { return e - 1; }  
  
  template <class edge>
  GenPtr target(edge) const { return Target; }

  template <class graph>
  static bool finish_construction(graph& G) { return true; }  
};


template <>
class st_node_struct<directed_graph>
{ 
  public:
  
  GenPtr FirstOutEdge;

  public:
    
  template <class graph>
  st_node_struct(const graph& G) : FirstOutEdge(0) {}
  
  template <class graph>
  static bool finish_construction(const graph& G)
  {
    typedef typename graph::node node;
    typedef typename graph::edge edge;
  
    node v_last = G.last_node();  
    edge e_last = G.last_edge();  
    (v_last+1)->FirstOutEdge = e_last + 1;

    for (node v = v_last; v >= G.first_node(); v--)
      if (v->first_out_edge() == 0)
        v->FirstOutEdge = (v+1)->FirstOutEdge;
     
    return true;
  }

  void set_out_edge(GenPtr e) { 
    if (FirstOutEdge == 0) FirstOutEdge = e; 
  } 

  template <class node>
  GenPtr first_out_edge(node) const { return FirstOutEdge; }

  
  template <class node>
  GenPtr stop_out_edge(node v) const { return (v+1)->FirstOutEdge; }

  template <class node>
  int outdeg(node v) const 
  { return int((v+1)->first_out_edge() - v->first_out_edge()); }
};

// ------------------------------------------------------------------------- 
// Category: bidirectional_graph
// ------------------------------------------------------------------------- 

template <>
class st_edge_struct<bidirectional_graph> 
      : public st_edge_struct<directed_graph> 
{
  //protected:
  public:

  GenPtr Source;

  //GenPtr NextInEdge;
  int NextInEdge; // relative address (offset) 32 bit

  public:
  
  template <class graph>
  st_edge_struct(const graph& G, typename graph::node v, typename graph::node w)
                : st_edge_struct<directed_graph>(G,v,w)
  { 
    typedef typename graph::node_type node_type;
    typedef typename graph::edge_type edge_type;
    typedef node_type*  node;
    typedef edge_type*  edge;

    Source = v;
    //NextInEdge = w->set_in_edge(this);    
    GenPtr p = w->set_in_edge(this);    
    NextInEdge = p ? ((edge)p-(edge)this) : 0;
    if (NextInEdge) assert((edge)this + NextInEdge == (edge)p);
  }
  
  template <class graph>
  static bool finish_construction(graph& G) 
  { 
    typedef typename graph::node node;
    typedef typename graph::edge edge;
  
    for (node v = G.first_node(); v != G.stop_node(); v = G.next_node(v))
    {
      if (v->first_in_edge() == 0) continue;
      
      edge e0 = v->first_in_edge(); 
      edge e1 = e0;
      edge e2 = e0;

      while (e0->next_in_edge() != 0)
      {
        e2 = e0->next_in_edge();

        //e0->NextInEdge = e2->next_in_edge();
        if (e2->next_in_edge())
          e0->NextInEdge = e2->next_in_edge() - e0;
        else
          e0->NextInEdge = 0;

        //e2->NextInEdge = e1;
        if (e1)
          e2->NextInEdge = e1-e2;
        else
          e2->NextInEdge = 0;

        e1 = e2;
      }
      
      v->set_in_edge(e2);
    }
  
    return true; 
  }  
 
  template <class edge>
  edge next_in_edge(edge e) const 
  { //return (edge) NextInEdge;     
    return NextInEdge ? ((edge)this + NextInEdge) : 0; 
   }
  
  template <class edge>
  GenPtr source(edge) const { return Source; }
};


template <>
class st_node_struct<bidirectional_graph> 
      : public st_node_struct<directed_graph>
{ 
  //protected:
  public:

  GenPtr FirstInEdge;

  public:
    
  template <class graph>
  st_node_struct(const graph& G) 
   : st_node_struct<directed_graph>(G), FirstInEdge(0) {}
  
  GenPtr set_in_edge(GenPtr e) 
  {
    GenPtr x = FirstInEdge;
    FirstInEdge = e;
    return x;
  }             
  
  template <class node> 
  GenPtr first_in_edge(node) const { return FirstInEdge; }
  
  template <class node> 
  GenPtr stop_in_edge(node) const { return 0; }
  
  template <class node_type> 
  int indeg(node_type* v) const 
  { 
    typedef typename node_type::edge edge;
    int cnt = 0;
    for (edge e = v->first_in_edge(); 
         e != v->stop_in_edge(); e = e->next_in_edge()) cnt++;
    return cnt;
  }

  template <class node> 
  int degree(node v) const { return outdeg(v) + indeg(v); }  
};


// ------------------------------------------------------------------------- 
// Category: opposite_graph
// ------------------------------------------------------------------------- 

template <>
class st_node_struct<opposite_graph> 
      : public st_node_struct<bidirectional_graph> 
{
  public:

  template <class graph>
  st_node_struct(const graph& G) : st_node_struct<bidirectional_graph>(G) {}  
};

template <>
class st_edge_struct<opposite_graph> 
{
  //protected:
  public:
  
  //GenPtr NextInEdge;
  int NextInEdge;

#if defined(WORD_LENGTH_64)
  unsigned int  srcXtgt;  // 32 bit
#else
  unsigned long srcXtgt;
#endif

  
  public:
  
  template <class graph>
  st_edge_struct(const graph& G, typename graph::node v, typename graph::node w)
  { 
    typedef typename graph::node_type node_type;
    typedef typename graph::edge_type edge_type;
    typedef node_type*  node;
    typedef edge_type*  edge;

    v->set_out_edge(this);
    
#if defined(WORD_LENGTH_64)
    unsigned long tmp = (unsigned long)v ^ (unsigned long)w; 
    assert((tmp & 3) == 0);
    tmp >>= 2;
    assert(tmp <= 0xffffffff);
    srcXtgt = (unsigned int)tmp;
#else
    srcXtgt = (unsigned long)v ^ (unsigned long)w; 
#endif

  //NextInEdge = w->set_in_edge(this);

    GenPtr p = w->set_in_edge(this);
    NextInEdge = p ? ((edge)p-(edge)this) : 0;
    if (NextInEdge) assert((edge)this + NextInEdge == (edge)p);
  }
  
  template <class edge>
  edge next_out_edge(edge e) const { return e + 1; }  
  
  template <class edge>
  edge next_in_edge(edge e) const 
  { //return (edge) NextInEdge; 
    return NextInEdge ? ((edge)this + NextInEdge) : 0; 
   }  


  template <class graph, class edge, class node>
  node opposite(const graph&, edge e, node v) const {
#if defined(WORD_LENGTH_64)
    return node((unsigned long)v ^ (unsigned long)(srcXtgt<<2));
#else
    return node((unsigned long)v ^ (unsigned long)srcXtgt);
#endif
}

  template <class edge, class node>
  node target(edge e, node v) const { return opposite(e,v); }
  
  template <class edge, class node>
  node source(edge e, node v) const { return opposite(e,v); }

  template <class graph>
  static bool finish_construction(graph& G) 
  { 
    typedef typename graph::node node;
    typedef typename graph::edge edge;
  
    for (node v = G.first_node(); v != G.stop_node(); v = G.next_node(v))
    {
      if (v->first_in_edge() == 0) continue;
      
      edge e0 = v->first_in_edge(); 
      edge e1 = e0;
      edge e2 = e0;

      while (e0->next_in_edge() != 0)
      {
        e2 = (edge) e0->next_in_edge();

        //e0->NextInEdge = e2->next_in_edge();
        if (e2->next_in_edge())
          e0->NextInEdge = e2->next_in_edge() - e0;
        else
          e0->NextInEdge = 0;

        //e2->NextInEdge = e1;
        if (e1)
          e2->NextInEdge = e1-e2;
        else
          e2->NextInEdge = 0;

        e1 = e2;
      }
      
      v->set_in_edge(e2);
    }
  
    return true; 
  }    
};


// ------------------------------------------------------------------------- 
// static_graph
// ------------------------------------------------------------------------- 

template< class cat, class nd = data_slots<0>, class ed = data_slots<0> >
class static_graph : public cat::graph,
                 public graph_register< static_graph<cat,nd,ed> >
{  
  public:

  enum { static_tag = 1, dynamic_tag = 0 };
 
  struct traits
  {
    typedef static_graph<cat,nd,ed> graph;        
    typedef st_node_struct<cat> node_struct;
    typedef st_edge_struct<cat> edge_struct;
    
    typedef nd node_data;
    typedef ed edge_data;
  };

/*
#if ((__GNUC__ == 3 && __GNUC_MINOR__ >=4) || (__GNUC__ >=4) ) || defined(_AIX)
*/

typedef typename cat::template edge_type<traits> edge_type;
typedef typename cat::template node_type<traits> node_type;

typedef typename cat::template node_type<traits>::node node;
typedef typename cat::template node_type<traits>::edge edge;

/*
#else
typedef typename cat::edge_type<traits> edge_type;
typedef typename cat::node_type<traits> node_type;

typedef typename cat::node_type<traits>::node node;
typedef typename cat::node_type<traits>::edge edge;
#endif
*/

  enum
  { n_base_sz = 0, n_data_sl = nd::slots, 
    e_base_sz = 0, e_data_sl = ed::slots  
  };
  
  protected:
  
  char* V;
  node v_first;
  node v_last;
  node v_end;
 
  char* E;
  edge e_first;
  edge e_last;
  edge e_end;

  bool alloc;
  
  public:  
  
  static_graph()
  { V = 0;
    v_first = v_last = v_end = 0;
    E = 0;
    e_first = e_last = e_end = 0;
    alloc = true;
   }

  static_graph(char* VV, char* EE)
  { V = VV;
    v_first = v_last = v_end = 0;
    E = EE;
    e_first = e_last = e_end = 0;
    alloc = false;
   }


 ~static_graph() { clear(); }


  void start_construction(unsigned n, unsigned m)
  { 
    clear();
    
    if (n == 0) return;

    if (V == 0) V = (char*)std_memory.allocate_vector((n+2)*sizeof(node_type));
    v_first = node(V+sizeof(node_type) - (V - (char*)0) % sizeof(node_type));

    v_last  = v_first - 1;
    v_end   = v_first + n;

    if (E == 0) E = (char*)std_memory.allocate_vector((m+2)*sizeof(edge_type));
    e_first = edge(E+sizeof(edge_type) - (E - (char*)0) % sizeof(edge_type));
    e_last  = e_first - 1;
    e_end   = e_first + m;

  }
  
  void init(unsigned n, unsigned m) { start_construction(n,m); }
  
  void clear()
  {     
    if (!v_first) return;
    
    graph_register< static_graph<cat,nd,ed> >::clear(*this);
    
    if (alloc)
    { std_memory.deallocate_vector(V); 
      std_memory.deallocate_vector(E);
      V = 0;
      E = 0;
     }
    
    e_first = e_last = e_end = 0;
    v_first = v_last = v_end = 0;
  }
  
  node new_node() 
  {
    new (++v_last) node_type(*this);            
    return v_last;    
  }
  
  edge new_edge(node v, node w) 
  { 
    new (++e_last) edge_type(*this,v,w);            
    return e_last;     
  }

  bool finish_construction() 
  { 
    v_end = v_last + 1;
    e_end = e_last + 1;
   
    st_node_struct<cat>::finish_construction(*this);
    st_edge_struct<cat>::finish_construction(*this);    
       
    return true;
  }

  int  index(node v) const { return int(v-v_first); }

  int  index(edge e) const { return int(e-e_first); }

  int  max_node_index()  const { return int(v_last - v_first); }
  int  max_edge_index()  const { return int(e_last - e_first); }
 
  int  number_of_nodes() const { return int(v_end - v_first); }
  int  number_of_edges() const { return int(e_end - e_first); }
 
  bool member(node v) const { return v_first <= v && v < v_end; }
  bool member(edge e) const { return e_first <= e && e < e_end; }
  
  bool empty() const { return v_first == v_end; }
      
  node first_node() const { return v_first; }
  edge first_edge() const { return e_first; }
  
  node last_node()  const { return v_last; }
  edge last_edge()  const { return e_last; }
  
  node stop_node()  const { return v_end; }
  edge stop_edge()  const { return e_end; }
  
  node next_node(node v) const { return v + 1; }
  edge next_edge(edge e) const { return e + 1; }
  
  node succ_node(node v) const { return (v == v_end) ? v_end : v + 1; }
  edge succ_edge(edge e) const { return (e == e_end) ? e_end : e + 1; }


// dynamic in-edge management

void clear_in_edges() const
{ node v;
  forall_nodes(v,(*this)) v->FirstInEdge = 0;
  edge e;
  forall_edges(e,(*this)) 
      //e->NextInEdge = e_end;
      e->NextInEdge = e_end-e;
}


void push_in_edge(node v, edge e) const
{ //e->NextInEdge = v->FirstInEdge;
  e->NextInEdge = v->FirstInEdge ? ((edge)v->FirstInEdge - e) : 0;
  v->FirstInEdge = e;
 }


bool test_in_edge(edge e) const 
{ //return e->NextInEdge != e_end; 
  return e->next_in_edge() != e_end; 
}


/*
void rebuild_in_edges() const
{ node v;
  forall_nodes(v,(*this)) v->FirstInEdge = 0;
  forall_nodes(v,(*this)) 
  { edge e;
    forall_out_edges(e,v) 
         push_in_edge(this->opposite(e,v),e);
   }
}
*/


void rebuild_in_edges() const
{ node v;
  forall_nodes(v,(*this)) v->FirstInEdge = 0;
  for(v = stop_node()-1; v >= first_node(); v--)
  { edge e_stop = v->first_out_edge();
    for(edge e=v->stop_out_edge()-1; e>=e_stop; e--)
         push_in_edge(this->opposite(e,v),e);
   }
}


void permute_in_edges() const
{ node v;
  forall_nodes(v,(*this)) 
  { int indeg = 0;
    for(edge e = v->first_in_edge(); e != 0; e = e->next_in_edge()) indeg++;
    if (indeg <= 2) continue;

    edge* A = new edge[indeg];

    int k = 0;
    for(edge e = v->next_in_edge(); e != 0; e = e->next_in_edge()) 
       A[k++] = e;

    int i=0, j=indeg-1;
    int delta = 1;
    while (i != j)
    { A[i]->NextInEdge = A[j];
      i += delta;
      swap(i,j);
      delta = -delta;
     }
    A[i]->NextInEdge = 0;
    v->FirstInEdge = (edge)A[0];
    delete[] A;
   }
}


double bandwidth()
{ double bw = 0;
  node v;
  forall_nodes(v,(*this))
  { edge e_last = first_in_edge(v);
    for(edge e = v->first_in_edge(); e != 0; e = e->next_in_edge()) 
    { int d = index(e) - index(e_last);
      bw += (d > 0) ? d : -d;
      e_last = e;
     }
   }
  return bw;
}





  edge first_out_edge(node v) const { return v->first_out_edge(); }
  edge stop_out_edge(node v)  const { return v->stop_out_edge(); }

  edge first_in_edge(node v)  const { return v->first_in_edge(); }
  edge stop_in_edge(node v)  const { return v->stop_in_edge(); }

  edge next_out_edge(edge e) const { return e->next_out_edge(); }
  edge pred_out_edge(edge e) const { return e->pred_out_edge(); }
  edge next_in_edge(edge e)  const { return e->next_in_edge(); }


  // compatibility with edge_array / node_array

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


// functions used in forall_inout_edges macro

template <class node, class edge>
inline bool First_Inout_Edge(edge& e, node v)
{ e = v->first_out_edge();
  if (e != v->stop_out_edge()) return true;
  e = v->first_in_edge();
  return false;
}

template <class node, class edge>
inline  bool Succ_Inout_Edge(edge& e, bool e_out, node v)
{ if (e_out)
  { e = e->next_out_edge();
    if (e != v->stop_out_edge()) return true;
    e = v->first_in_edge();
    return false;
   }
  e = e->next_in_edge();
  return false;
}



LEDA_END_NAMESPACE

#endif
