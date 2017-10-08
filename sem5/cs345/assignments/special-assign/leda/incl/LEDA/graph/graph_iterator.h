/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_iterator.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


/*******************************************************************
*                                                                  *
*      Graphiterators + Data Accessors + Algorithms                *
*                                                                  *
*      (C) 1996-2010                                               *
*       Marco Nissen (marco@mpi-sb.mpg.de)                         *
*       Karsten Weihe (weihe@informatik.uni-konstanz.de)           *
*                                                                  *
*******************************************************************/

// VERSION December, 1st 1999

/* Changes (Marco Nissen):
 
   12.8.1998:
   1. typedefs are now public in safe iterators
   2. member methods of algorithmic iterators are now inline
   3. member_edge/node, mygit_edge/node are now static

   13.8.1998:
   1. order make_invalid / reset-methods of algorithmic iterators exchanged
   
   2.12.1998:
   1. NodeIt_n, EdgeIt_e, AdjIt_n, AdjIt_e, Out.., In.. added
      (STL like iterators which yield node or edge for *operator)

   1.3.1999:
   1. member method name 'stack' replaced into 'get_stack'  
      (same for in_stack, out_stack and queue)

   10.6.1999:
   1. LEDA_PREFIX adaption done

   14.7.1999: 
   1. new: FaceIt iterators over the set of faces
   2. new: OutAdjCirc and InAdjCirc are circulators (for OutAdjIt and InAdjIt)
   3. new: FaceCirc is a circulator for one face 

   1.12.1999:
   1. node_member_da and edge_member_da corrected

   Tested with:
   - g++ 2.8.1, eg++ 1.1, sunpro 4.2 , MSVC++5.0, g++ 2.7.1

  REMARKS:

  * there is a set problem -- please refer to the SET-BUG-SECTION
  * http://www.mpi-sb.mpg.de/LEDA/friends/git.html 
    has up-to-date information on this file
*/

#ifndef LEDA_GRAPHIT_H
#define LEDA_GRAPHIT_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600216
#include <LEDA/internal/PREAMBLE.h>
#endif

#if ((__GNUC__== 3) && (__GNUC_MINOR__>=4) || (__GNUC__ >=4))
#define USE_ITER_MEMBERS \
  using Iter::valid; \
  using Iter::make_invalid; \
  using Iter::reset; \
  using Iter::reset_end;
#else
#define USE_ITER_MEMBERS
#endif

/*

SET-BUG:
========

There are two possibilities for using data accessors together
with LEDA sets:

1. Use the LEDA-prefixing mechanism

2. A workaround for not using the LEDA prefixing mechanism is
   pasting the following code at the beginning of data accessor
   code. Besides that you have to compile all your code with
   -DGITSET_ (set the flag GITSETWK).

      #ifdef set 
      #undef set 
      #endif 
      #define set da_set 
      #define get da_get 

   At the end of the data accessor code you should insert this
   if you use the set class of LEDA:

      #ifdef set 
      #undef set 
      #endif 
      #define set set 

   Probably there will be a warning saying that there
   is a redefinition of 'set' - a workaround is to move 
   graph_iterator.h in the include list to the lowest
   position (can actually be ignored).

*/


#ifdef GITSETWK
#ifdef set
#undef set
#endif
#define set da_set
#define get da_get     
#endif

#include <LEDA/graph/graph.h>
#include <LEDA/core/h_array.h>
#include <LEDA/core/p_queue.h>
#include <LEDA/core/queue.h>
#include <LEDA/core/stack.h>
#include <LEDA/system/stream.h>

#ifdef LEDA_PREFIX
#undef set
#endif   

// earlier gnu compilers or sunpro cannot handle typename
// this should be adapted in future for other compilers like KCC

#define __GIT_VERSION__ 3


#define LIMITS(T,NULL,MAX)\
template <> class git_limits<T> {\
public:\
  T value_null;\
  T value_max;\
  git_limits() : value_null(T(NULL)),value_max(T(MAX)) {}\
};

// specialized version for e.g. leda::integer

LEDA_BEGIN_NAMESPACE

template<class T>
class git_limits {
public:
  T value_null;
  T value_max;
git_limits()  { } };




#ifdef LONG_MAX
LIMITS(long,0,LONG_MAX)
#endif
#ifdef DBL_MAX
LIMITS(double,0,DBL_MAX)
#else
LIMITS(double,0,INT_MAX)
#endif                      
LIMITS(int,0,INT_MAX)





/* This enumeration is needed for safe iterators to decide
   which reaction is the correct one .. 
   
   mode_forward:  deletion of node/edge leads to a forward search for 
                  a new object
   mode_backward: deletion of node/edge leads to a backward search for
                  a new object
   mode_invalid:  deletion of node/edge leads to setting the object to 
                  invalid 
*/

enum refresh_mode {
  mode_forward = 0x01,
  mode_backward = 0x02,
  mode_invalid = 0x04
};

// all kinds of safe iterators are inherited from this base class:

//template<class leda::node>
template<class nodet>
class SafeIteratorBase  {
  public:
  virtual void refresh_object_nil() { }
  virtual void refresh_object(nodet) { }
  virtual ~SafeIteratorBase() { }
};

template <class grapht>
class SafeGraph {
public:
  typedef leda::node                   nodetype;
  typedef leda::edge                   edgetype;
  typedef SafeGraph<grapht>  self;
  typedef grapht                 graphtype;
private:
  typedef SafeIteratorBase<leda::node>*safeptr;
  graphtype* _g;
  leda::h_array<leda::node,leda::list<safeptr> >   iterators;
  leda::h_array<leda::edge,leda::list<safeptr> >   edge_iterators;
public:
  SafeGraph(graphtype& G) : _g(&G) {
    leda::node v; leda::edge e;
    forall_nodes(v,*_g) iterators[v].clear();
    forall_edges(e,*_g) edge_iterators[e].clear(); }
  
  SafeGraph(const SafeGraph& S) :
    _g(S._g), iterators(S.iterators), edge_iterators(S.edge_iterators) { }
  
  graphtype& get_graph() { return *_g; }
  const graphtype& get_graph() const { return *_g; }
  operator graphtype& () { return *_g; }
  
  void insert(nodetype v, safeptr iter) {
    if (v!=nil) {
      (iterators)[v].append(iter); }
  }
  
  void insert(edgetype e, safeptr iter) {
    if (e!=nil) {
      (edge_iterators)[e].append(iter);
      (iterators)[source(e)].append(iter);
      if (source(e)!=target(e))
	(iterators)[target(e)].append(iter);
    }
  }
  
  void remove(nodetype v, safeptr iter) {
    if (v!=nil) {
      list_item it;
      forall_items(it,(iterators)[v]) {
        if ((iterators)[v][it]==iter) {
          (iterators)[v].del_item(it);
          break;
        } 
      } 
    } 
  }
  
  void remove(edgetype e, safeptr iter) {
    if (e!=nil) {
      list_item it;
      forall_items(it,(edge_iterators)[e]) {
        if ((edge_iterators)[e][it]==iter) {
          (edge_iterators)[e].del_item(it);
	  break;  } }
      forall_items(it,(iterators)[source(e)]) {
        if ((iterators)[source(e)][it]==iter) {
          (iterators)[source(e)].del_item(it);
	  break;  } }
      if (source(e)!=target(e)) {
        forall_items(it,(iterators)[target(e)]) {
          if ((iterators)[target(e)][it]==iter) {
            (iterators)[target(e)].del_item(it);
            break;
	  }
	} 
      } 
    } 
  }
  
  
  void removeall(nodetype v) {
    if (v!=nil) {
      (iterators[v]).clear();
      iterators.undefine(v);
    } 
  }
  
  void removeall(edgetype v) {
    if (v!=nil) {
      (edge_iterators[v]).clear();
      edge_iterators.undefine(v);
    } 
  }
  
  
  void create_entry(nodetype v) { 
    if (v!=nil) {
      iterators[v].clear(); 
    }
  }
  
  void create_entry(edgetype e) { 
    if (e!=nil) {
      edge_iterators[e].clear(); 
    } 
  }
  
  
  void update_and_delete(nodetype v, safeptr iter) {
    if (v!=nil) {
      list_item it;
      leda::list<safeptr> itlist((iterators)[v]); // really a copy !
      _g->del_node(v);
      forall_items(it,itlist) {
        if (iter!=itlist[it])
	  (*(itlist[it])).refresh_object(v); 
      }
      removeall(v);
    } 
  }
  
  void update_and_delete(edgetype e, safeptr iter) {
    if (e!=nil) {
      list_item it;
      leda::list<safeptr> itlist((edge_iterators)[e]); // really a copy !
      _g->del_edge(e);
      forall_items(it,itlist) {
        if (iter!=itlist[it])
	  (*(itlist[it])).refresh_object_nil(); 
      }
    } 
  }
  
  
  nodetype insert() {
    nodetype temp(_g->new_node());
    create_entry(temp);
    return temp; 
  }
  
  
  void del(nodetype v) {
    if (v!=nil) {
      _g->del_node(v);
      removeall(v); 
    }
  }
  
};

/*{\Mtext
  \section{Introduction}
  
  \subsection{Iterators}
  \index{iterator} \index{design pattern}
  Iterators are a powerful technique in object-oriented programming and one
  of the fundamental design patterns~\cite{bs:GHJV95a}. Roughly speaking, an
  iterator is a small, light-weight object, which is associated with a specific
  kind of linear sequence. An iterator can be used to access all items in a
  linear sequence step-by-step. In this section, different iterator classes are
  introduced for traversing the nodes and the edges of a graph, and
  for traversing all ingoing and/or outgoing edges of a single node.
  
  Iterators are an alternative to the iteration macros introduced in
  sect.~\ref{Graphs}.3.(i).
  For example, consider the following iteration pattern:
  \begin{verbatim}
    node v;
    forall_nodes (n, G) { ... }
  \end{verbatim}
  Using the class {\em NodeIt}\/ introduced in sect.~\ref{Node Iterators}, this
  iteration can be re-written as follows:
  
  \begin{verbatim}
    for (NodeIt it (G); it.valid(); ++it) { ... }
  \end{verbatim}
  
  
  The crucial differences are:
  
  \begin{itemize}
  \item Iterators provide an intuitive means of  
        movement through the topology of a graph.

  \item Iterators are not bound to a loop, which means that the user has finer
  control over the iteration process. For example, the continuation condition
  {\em it.valid()}\/ in the above loop could be replaced by another condition to
  terminate the loop once a specific node has been found (and the loop may be
  re-started at the same position later on).
  
  \item The meaning of iteration may be modified seamlessly. For example, the
  filter iterators defined in sect.~\ref{Filter Node Iterator}
  restrict the iteration to a subset that is specified by an
  arbitrary logical condition ({\em predicate}\/). In other words, the nodes or
  edges that do not fulfill this predicate are filtered out automatically during
  iteration.
  
  \item The functionality of iteration may be extended seamlessly. For example,
  the observer iterators defined in sect.~\ref{Observer Node Iterator}
  can be used to record details of the
  iteration. A concrete example is given in sect.~\ref{Observer Node Iterator}:
  an observer iterator can be initialized such that it records the number of
  iterations performed by the iterator.
  
  \item Iterator-based implementations of algorithms can be easily
  integrated into environments that are implemented according to the STL
  style~\cite{bs:MS95a}, (this style has been adopted for the standard C++
  library). For this purpose, sect.~\ref{STL Iterator Wrapper} define adapters,
  which convert graph iterators into STL iterators.
  
  \end{itemize}
  
  \subsection{Handles and Iterators}
  
  Iterators can be used whenever the corresponding handle can be used. 
  For example, node iterators can be used where a node is requested
  or edge iterators can be used where an edge is requested.
  For adjacency iterators, it is possible to use them whenever
  an edge is requested\footnote{Since 
  the edge of an adjacency iterator changes while the fixed node
  remains fixed, we decided to focus on the edge.}.
 
  An example shows how iterators can be used as handles:

  \begin{verbatim}
    NodeIt it(G);
    leda::node_array<int> index(G);
    leda::node v;
    int i=0;
    forall_nodes(v,G) index[v]=++i;
    while (it.valid()) { 
     cout << "current node " << index(it) << endl; }
  \end{verbatim}

  \subsection{STL Iterators}

  Those who are more used to STL may take advantage from the following
  iterator classes: [[NodeIt_n]], [[EdgeIt_e]], [[AdjIt_n]], 
  [[AdjIt_e]], [[OutAdjIt_n]], [[OutAdjIt_e]], [[InAdjIt_n]],
  [[InAdjIt_e]]. The purpose of each iterator is the same as in
  the corresponding standard iterator classes [[NodeIt]], [[EdgeIt]] \dots
  The difference is the interface, which is exactly that of
  the STL iterator wrapper classe (see sect.~\ref{stlwrap} for more
  information).
  
  An example shows why these classes are useful (remember the example
  from the beginning):

  \begin{verbatim}
    NodeIt_n base(G);
    for(NodeIt_n::iterator it=base.begin();it!=base.end(); ++it) {
     cout << "current node " << index(*it) << endl; }
  \end{verbatim}

  As in STL collections there are public type definitions in all 
  STL style graph iterators. The advantage is that algorithms can
  be written that operate independingly of the underlying type
  (note: [[NodeIt_n]] and [[NodeIt_n::iterator]] are equal types).

  \subsection{Circulators}

  Circulators differ from Iterators in their semantics. Instead
  of becoming invalid at the end of a sequence, they perform cyclic
  iteration. This type of ''none--ending--iterator`` is
  heavily used in the CGAL \index{CGAL}.
  
  \subsection{Data Accessors}
  \index{data accessor}
  {\em Data accessor}\/ is a design pattern\cite{Ni99} that decouples data
  access from underlying implementation. Here, the pattern is used to
  decouple data access in graph algorithms from how data is actually
  stored outside the algorithm.
  
  Generally, an attributed graph consists of a (directed or undirected)
  graph and an arbitrary number of node and edge attributes. For example, the
  nodes of a graph are often assigned attributes such as names, flags, and
  coordinates, and likewise, the edges are assigned attributes such as lengths,
  costs, and capacities.
  
  More formally, an {\em attribute}\/ $a$ of a set $S$ has a certain type $T$
  and assigns a value of $T$ to every element of $S$ (in other words, $a$
  may be viewed as a function $a:S\rightarrow T$). An {\em attributed set}\/
  $A=(S,a_1,\ldots,a_m)$ consists of a set $S$ and attributes $a_1,\ldots,a_m$.
  An attributed graph is a (directed or undirected) graph $G=(V,E)$ such that
  the node set $V$ and the edge set $E$ are attributed.
  
  Basically, LEDA provides two features to define attributes for graph:
  
  \begin{itemize}
  
  \item Classes {\em GRAPH}\/ and {\em UGRAPH}\/ 
  (sects.~\ref{Parameterized Graphs} and ~\ref{Parameterized Ugraph}) are
  templates with two arguments, {\em vtype}\/ and {\em etype}, which are
  reserved for a node and an edge attribute, respectively. To attach several
  attributes to nodes and edges, {\em vtype}\/ and {\em etype}\/ must be
  instantiated by structs whose members are the attributes.
  
  \item A {\em node array}\/ (sect.~\ref{Node Arrays}) 
  or {\em node map}\/ (sect.~{Node Maps})
  represents a node attribute, and analogously, {\em edge arrays}\/ 
  (sect.~{Edge Arrays})
  and {\em edge maps}\/ (sect.~\ref{Edge Maps}), 
  represent edge attributes. Several
  attributes can be attached to nodes and edges by instantiating several arrays
  or maps.
  
  \end{itemize}
  
  Data accessors provide a uniform interface to access attributes, and the
  concrete organization of the attributes is hidden behind this interface.
  Hence, if an implementation of an algorithm does not access attributes
  directly, but solely in terms of data accessors, it may be applied to any
  organization of the attributes 
  (in contrast, the algorithms in sect.~{Graph Algorithms}
  require an organization of all attributes as node and edge arrays).
  
  Every data accessor class {\em DA}\/ comes with a function template {\em
  get}\/: 
\begin{verbatim}
    T get(DA da, Iter it); 
\end{verbatim}
  
  This function returns the value of the attribute managed by the data accessor
  {\em da}\/ for the node or edge marked by the iterator {\em it}. Moreover,
  most data accessor classes also come with a function template {\em set}\/:

\begin{verbatim}
    void set(DA da, Iter it, T value); 
\end{verbatim}
  
  This function overwrites the value of the attribute managed by the data
  accessor {\em da}\/ for the node or edge marked by the iterator {\em it}\/ by
  {\em value}.
  
  The data accessor classes that do not provide a function template {\em
  set}\/ realize attributes in such a way that a function {\em set}\/ does
  not make sense or is even impossible. The {\em constant accessor}\/ in
  sect.~\ref{Constant Accessors} is a concrete example: it realizes an attribute
  that is constant over the whole attributed set and over the whole time
  of the program. Hence, it does not make sense to provide a function {\em
  set}. Moreover, since the constant accessor class organizes its attribute
  in a non-materialized fashion, an overwriting function {\em set}\/ is even
  impossible.
  
  {\bf Example:} The following trivial algorithm may serve as an example to
  demonstrate the usage of data accessors and their interplay with various
  iterator types. The first, nested loop accesses all edges once. More
  specifically, the outer loop iterates over all nodes of the graph, and the
  inner loop iterates over all edges leaving the current node of the outer loop.
  Hence, for each edge, the value of the attribute managed by the data accessor
  {\em da}\/ is overwritten by $t$. In the second loop, a linear edge iterator
  is used to check whether the first loop has set all values correctly.
  
  \begin{verbatim}
  template <class T, class DA>
  void set_and_check (graph& G, DA da, T t) {
    for (NodeIt nit(G); nit.valid(); ++nit)
      for (OutAdjIt oait(nit); oait.valid(); ++oait)
        set (da, eit, t);
    for (EdgeIt eit(G); eit.valid(); ++eit)
      if (get(da,it) != t) cout << "Error!" << endl;
  }
  \end{verbatim}
  
  To demonstrate the application of function {\em set\_and\_check}, we first
  consider the case that $G$ is an object of the class {\em GRAPH}\/ derived
  from {\em graph}\/ (sect.~\ref{Graphs}), that the 
  template argument {\em vtype}\/
  is instantiated by a struct type {\em attributes}, and that the {\tt
  int}-member {\em my\_attr}\/ of {\em attributes}\/ shall be 
  processed by {\em
  set\_and\_check}\/ with value $1$. Then {\em DA}\/ can be instantiated as a
  {\em node\_member\_da}\/:
  
  \begin{verbatim}
    node_member_da<attributes,int> da (&attributes::my_attr);
    set_and_check (G, da, 1);
  \end{verbatim}
  
  Now we consider the case that the attribute to be processed is stored in an
  {\em edge\_array$<$int$>$}\/ named {\em my\_attr\_array}\/:
  
  \begin{verbatim}
    node_array_da<int> da (my_attr_array);
    set_and_check (G, da, 1);
  \end{verbatim}
  
  Hence, all differences between these two cases are factored out into a single
  declaration statement.

   
\subsection{Graphiterator Algorithms}

Several basic graph algorithms were re--implemented to use only 
graph iterators and data accessors. Moreover they share three design
decisions:
\begin{enumerate}
\item {\bf algorithms are instances} of classes
\item algorithm instances have the {\bf ability to ``advance''}
\item algorithm instances provide {\bf access to their internal states}
\end{enumerate}

An example for an algorithm that supports the first two decisions is:

\begin{verbatim}
    class Algorithm {
      int state, endstate;
    public:
      Algorithm(int max) : endstate(max), state(0) { }
      void next() { state++; }
      bool finished() { return state>=endstate; } 
    };
\end{verbatim}

With this class {\tt Algorithm} we can easily instantiate an 
algorithm object: 

\begin{verbatim}
    Algorithm alg(5); 
    while (!alg.finished()) alg.next();
\end{verbatim}

This small piece of code creates an algorithm object and invokes
``next()'' until it has reached an end state.

An advantage of this design is that we can write basic algorithms, which
can be used in a standardized way and if needed, inspection of
internal states and variables can be provided without writing complex
code.
Additionally, it makes it possible to write persistent algorithms,
if the member variables are persistent.

Actually, those algorithms are quite more flexible than ordinary
written algorithm functions:

\begin{verbatim}
    template<class Alg>
    class OutputAlg {
      Alg alg;
    public:
      OutputAlg(int m) : alg(m) {
        cout << "max state: " << m << endl; }
      void next() {
        cout << "old state: " << alg.state;
        alg.next(); 
        cout << " new state: " << alg.state << endl; }
      bool finished() { return alg.finished(); }
    };
\end{verbatim}
   
This wrapper algorithm can be used like this:

\begin{verbatim}
    OutputAlg<Algorithm> alg(5);
    while (!alg.finished()) alg.next();
\end{verbatim}

In addition to the algorithm mentioned earlier
this wrapper writes the internal states to the
standard output.

This is as efficient as rewriting the ``{\tt Algorithm}''-class
with an output mechanism, but provides more flexibility.
}*/


/*******************************************************************************

 (I) Iterator-Classes
 
     * NodeIt, EdgeIt, InAdjIt, OutAdjIt, AdjIt
     * SafeNodeIt, SafeEdgeIt, SafeInAdjIt, SafeOutAdjIt, SafeAdjIt
      
 Note: 
 The c++-comments contain manual pages in the leda manual style format.

*******************************************************************************/

/*{\Manpage {NodeIt} {} {Node Iterators} {it} }*/
/*{\Moptions latexruns=2 numbered=yes}*/
/*{\Mdefinition a variable |it| of class  |\Mname| is
  a linear node iterator that iterates over the node set
  of a graph;
  the current node of an
  iterator object is said to be ``marked'' by this object.
}*/
class NodeIt {
  /*{\Mcreation }*/
public:
  typedef leda::node      nodetype;
  typedef leda::graph     graphtype;
  typedef NodeIt self;
private:
  leda::node                      _n;
  const leda::graph*              _g;
public:
  
  NodeIt() { _g=nil; }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    associated with no graph. }*/
  
  NodeIt(const leda::graph& G) : _n(G.first_node()), _g(&G) { }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    associated with |G|.
    
    The graph is initialized by |G|.
    The node is initialized by |G.first_node()|.
    }*/
  
  
  NodeIt(const leda::graph& G, leda::node n ) : _n(n), _g(&G) { }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    marked with |n| and
    associated with |G|.
    
    Precondition: |n| is a node of |G|.}*/
  
  void init(const NodeIt& p)  {
    _g=p._g; update(p._n); }
  
  
  NodeIt (const NodeIt& p) :  _n(p._n), _g(p._g) { }
  /*{\Moperations}*/
  
  void init(const leda::graph& G)  {
    init(G,G.first_node()); }
  
  /*{\Mop associates |\Mvar| with |G| and marks it with
    |G.first_node()|. }*/
  
  void init(const leda::graph& G, const leda::node& v) {
    _g=(graphtype*)(&G); update(v); }
  
  /*{\Mop associates |\Mvar| with |G| and marks it with
    |v|. }*/
  
  void reset() { _n=_g->first_node(); }
  /*{\Mop  resets |\Mvar| to |G.first_node()|, where |G| is the associated graph. }*/
  
  void make_invalid() { _n=nil; }
  /*{\Mop makes |\Mvar| invalid, i.e. |\Mvar.valid()| will be false afterwards and
    |\Mvar| marks no node.}*/
  
  void reset_end() { _n=_g->last_node(); }
  /*{\Mop  resets |\Mvar| to |G.last_node()|, where |G| is the associated graph. }*/
  
  void update(leda::node  n) { _n=n; }
  /*{\Mop |it| marks |n| afterwards. }*/
  
  void insert() {
    leda::graph* Gp=(leda::graph*)_g;
    _n=Gp->new_node();
  }
  /*{\Mop    creates a new node and |\Mvar| marks it afterwards.}*/
  
  void del()  {
    leda::graph* Gp=(leda::graph*)_g;
    Gp->del_node(_n);
    _n=nil; }
  /*{\Mop  deletes the marked node, i.e. |\Mvar.valid()| returns false afterwards.
    
    Precondition: |\Mvar.valid()| returns true.  }*/
  
  NodeIt& operator= (const NodeIt& it2)  { _n=it2._n;  _g=it2._g;  return *this; }
  /*{\Mbinop |it| is afterwards associated with the same graph and node as |it2|.
    This method returns a reference to |\Mvar|.
    }*/
  
  bool operator== (const NodeIt& it2) const { return _n==it2._n; }
  /*{\Mbinop returns true if and only if |it| and |it2| are equal, i.e. if the marked nodes are equal.}*/
  
  operator leda::node() const { return _n; }
  operator leda::node() { return _n; }
  
  leda::node& get_node() { return _n; }

  leda::node  get_node() const { return _n; }
  /*{\Mop         returns the marked node or nil if |\Mvar.valid()| returns false.
    }*/
  
  const leda::graph& get_graph() const { return *_g; }
  /*{\Mop         returns the associated graph. }*/
  
  bool valid() const { return _n!=nil; }
  /*{\Mop        returns true if and only if end of sequence not yet passed, i.e.
    if there is a node in the node set that was not yet passed.
    }*/
  
  bool eol() const { return _n==nil; }
  /*{\Mop   returns |!\Mvar.valid()| which is
    true if and only if there is no successor node left,
    i.e. if all nodes of the node set are passed (eol: end of list). }*/
  
  NodeIt& operator++ ()  { _n=_g->succ_node(_n); return *this; }
  /*{\Munop       performs one step forward in the list of nodes of the associated graph.
    If there is no successor node, |\Mvar.eol()| will be true afterwards.
    This method returns a reference to |\Mvar|.
    
    Precondition: |\Mvar.valid()| returns true.
    }*/
  
  NodeIt operator++ (int) {
    NodeIt tmp(*this);
    _n=_g->succ_node(_n);
    return tmp; }
  
  NodeIt& operator-- () {
    _n=_g->pred_node(_n);
    return *this; }
    /*{\Munop performs one step backward in the list of nodes 
    of the associated graph.
    If there is no predecessor node, |\Mvar.eol()| will be true afterwards.
    This method returns a reference to |\Mvar|.
    
    Precondition: |\Mvar.valid()| returns true.
    }*/
  
  NodeIt operator-- (int) {
    NodeIt tmp(*this);
    _n=_g->pred_node(_n);
    return tmp; }
  
  friend ostream& operator<<(ostream& out, const self& )
    {  return out;}
  
  friend istream& operator>>(istream& in,  self& )
    {  return in;}
  
  
};
/*{\Mimplementation
  Creation of an iterator and all methods take constant time.
}*/
template<class grapht>
class SafeNodeIt  : public SafeIteratorBase<leda::node> {
public:
  typedef leda::node       nodetype;
  typedef SafeGraph<grapht> graphtype;
  typedef typename SafeGraph<grapht>::graphtype basetype;
private:
  leda::node                       _n;
  SafeGraph<grapht>*                     _g;
  refresh_mode                   RefreshMode;
  bool _escaped;
  typedef SafeNodeIt<grapht> self;
public:
  
  SafeNodeIt() { _g=nil; }
	  
  SafeNodeIt(SafeGraph<grapht>& G, refresh_mode RM) : _g(&G), RefreshMode(RM) {
    _escaped=false;
    _n=_g->get_graph().first_node();
    _g->insert(_n,this); }
	  
  SafeNodeIt(SafeGraph<grapht>& G, leda::node n=nil, refresh_mode RM=mode_forward) 
    : _n(n), _g(&G), RefreshMode(RM) {
    _escaped=false;
    if (_n==nil) _n=_g->get_graph().first_node();
    _g->insert(_n,this); 
  }
	  
  ~SafeNodeIt() { 
    if (valid()) _g->remove(_n,this);  
  }
	  
  void init(const graphtype& G) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
    graphtype& Gp(*((graphtype*)(&G)));
    init(G,((basetype)Gp).first_node()); 
  }
	  
  void init(const graphtype& G, const leda::node& n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
    if (_g!=nil) _g->remove(_n,this);
    _g=(graphtype*)(&G);
    _n=n;
    _g->insert(_n,this);
  }
  
void reset() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_n,this);
  _n=((leda::graph&)(*_g)).first_node();
  _g->insert(_n,this); 
}
	  
bool escaped() const {
  return _escaped; }
	  
void make_not_escaped() {
  _escaped=false; }
	  
void reset_end() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_n,this);
  _n=((leda::graph&)(*_g)).last_node();
  _g->insert(_n,this); }
	  
void make_invalid() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_n,this);
  _n=nil;
  _g->insert(_n,this);  }
	  
void insert() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  _g->remove(_n,this);
  _n=((leda::graph&)(*_g)).new_node();
  _g->insert(_n,this); }
	  
void update(leda::node  n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_n,this);
  _n=n;
  _g->insert(_n,this);  }
	  
void init(const SafeNodeIt& p) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  _g=p._g;
  _n=p._n;
  _g->insert(_n,this);
  RefreshMode=p.RefreshMode;
  _escaped=p._escaped; }
	  
SafeNodeIt(const SafeNodeIt& p) : _n(p._n), _g(p._g),
  RefreshMode(p.RefreshMode) ,   _escaped(p._escaped){
  _g->insert(_n,this);  }
	  
SafeNodeIt& operator= (const SafeNodeIt& it2) {
  _g=it2._g;
  _g->remove(_n,this);
  _n=it2._n;
  _g->insert(_n,this); return *this; }
	  
bool operator== (const SafeNodeIt& it2) const {
  return _n==it2._n; }
	  
operator leda::node() { return _n; }
	  
bool eol() const { return _n==nil; }
	  
bool valid() const { return _n!=nil; }
	  
leda::node& get_node() { return _n; }

leda::node  get_node() const { return _n; }

const SafeGraph<grapht>& get_graph() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  return *_g; }
	  
SafeNodeIt& operator++ () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_n,this);
  _n=((leda::graph&)(*_g)).succ_node(_n);
  _g->insert(_n,this); return *this; }
	  
SafeNodeIt operator++ (int)  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  SafeNodeIt tmp(*this);
  _g->remove(_n,this);
  _n=((leda::graph&)(*_g)).succ_node(_n);
  _g->insert(_n,this); return tmp; }
	  
	  
SafeNodeIt& operator-- () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_n,this);
  _n=((leda::graph&)(*_g)).pred_node(_n);
  _g->insert(_n,this); return *this; }
	  
SafeNodeIt operator-- (int)  {
  SafeNodeIt tmp(*this);
  _g->remove(_n,this);
  _n=((leda::graph&)(*_g)).pred_node(_n);
  _g->insert(_n,this); return tmp;  }
	  
void refresh_object(leda::node) {
  _escaped=true;
  _g->remove(_n,this);
  next_node();
  if (_n!=nil)
    _g->insert(_n,this);   }
	  
void next_node() {
  if (RefreshMode==mode_forward) {
    leda::node temp(_n);
    _n=((leda::graph&)(*_g)).succ_node(_n);
    if (_n==nil)  _n=((leda::graph&)(*_g)).first_node();
    if (temp==_n) _n=nil;
  } else if (RefreshMode==mode_backward) {
    leda::node temp(_n);
    _n=((leda::graph&)(*_g)).pred_node(_n);
    if (_n==nil) _n=((leda::graph&)(*_g)).last_node();
    if (_n==temp) _n=nil;
  } else
    _n=nil;
}
	  
	  
void del() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(503,"Iterator: del(), leda::graph not initialized\n");
#endif
  if (valid()) {
    leda::node temp(_n);
    next_node();
    _g->update_and_delete(temp,this);
    _g->remove(temp,this);
    if (_n!=nil) _g->insert(_n,this);
  }
}
friend ostream& operator<<(ostream& out, const self& )
{  return out;}
	  
friend istream& operator>>(istream& in,  self&)
{  return in;}
	  
};
/*{\Manpage {EdgeIt} {} {Edge Iterators} {it} }*/
/*{\Mdefinition a variable |it| of class  |\Mname| is
  a linear edge iterator that iterates over the edge set
  of a graph;
  the current edge of an
  iterator object is said to be ``marked'' by this object.
}*/
class EdgeIt {
public:
  /*{\Mcreation}*/
  typedef leda::node      nodetype;
  typedef leda::edge      edgetype;
  typedef leda::graph     graphtype;
private:
  leda::edge                      _e;
  const leda::graph*              _g;
public:
  typedef EdgeIt self;
  
  EdgeIt() { _g=nil; }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    associated with no graph. }*/
  
  EdgeIt(const leda::graph& G) : _e(G.first_edge()), _g(&G) { }
/*{\Mcreate introduces a variable |\Mvar| of this class
  associated with |G| and marked with |G.first_edge()|.
  }*/
  
  
EdgeIt(const leda::graph& G,leda::edge  e) : _e(e), _g(&G) { }
/*{\Mcreate introduces a variable |\Mvar| of this class marked with |e| and
  associated with |G|.
    
  Precondition: |e| is an edge of |G|.}*/
  
/*{\Moperations }*/
  
void init(const leda::graph& G) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  init(G,G.first_edge()); }
/*{\Mop associates |\Mvar| with |G| and marks it with
  |G.first_edge()|. }*/
  
void init(const leda::graph& G, const leda::edge& e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  _g=(graphtype*)(&G); update(e); }
/*{\Mop associates |\Mvar| with |G| and marks it with
  |e|. }*/
  
void update(leda::edge  e) { _e=e; }
/*{\Mop |it| marks |e| afterwards. }*/
  
void reset() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=_g->first_edge(); }
/*{\Mop resets |\Mvar| to |G.first_edge()| where
  |G| is the associated graph.
  }*/
  
void reset_end()  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=_g->last_edge(); }
/*{\Mop resets |\Mvar| to |G.last_edge()| where
  |G| is the associated graph.
  }*/
  
void make_invalid() { _e=nil; }
/*{\Mop makes |\Mvar| invalid, i.e. |\Mvar.valid()| will be false afterwards and
  |\Mvar| marks no node.}*/
  
void insert(leda::node v1, leda::node v2) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  leda::graph* Gp=(leda::graph*)_g;
  _e=Gp->new_edge(v1,v2); }
/*{\Mop    creates a new edge from |v1| to |v2| and |\Mvar| marks it afterwards.
  }*/
  
void del() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(503,"Iterator: del(), leda::graph not initialized\n");
#endif
  leda::graph* Gp=(leda::graph*)_g;
  Gp->del_edge(_e); _e=nil; }
/*{\Mop  deletes the marked edge, i.e. |\Mvar.valid()| returns false afterwards.
    
  Precondition: |\Mvar.valid()| returns true.  }*/
  
void init(const EdgeIt& p)  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  _g=p._g; update(p._e); }
  
EdgeIt (const EdgeIt& p) : _e(p._e), _g(p._g) {  }
  
  
EdgeIt& operator= (const EdgeIt& it2) { _e=it2._e;  _g=it2._g;  return *this; }
/*{\Mbinop assigns |it2| to |\Mvar|.
  This method returns a reference to |\Mvar|.}*/
  
bool operator== (const EdgeIt& it2) const {
  return _e==it2._e; }
/*{\Mbinop returns true if and only if |it| and |it2| are equal, i.e. if the marked edges are equal.}*/
  
bool eol() const { return _e==nil; }
/*{\Mop   returns |!\Mvar.valid()| which is
  true if and only if there is no successor edge left,
  i.e. if all edges leaving the marked node
  are passed (eol: end of list). }*/
  
  
bool valid() const { return _e!=nil; }
/*{\Mop        returns true if and only if end of sequence not yet passed, i.e.
  if there is an edge  leaving the marked node
  that was not yet passed.
  }*/
  
operator leda::edge() const{ return _e; }
operator leda::edge() { return _e; }
  
leda::edge&  get_edge() { return _e; }
    
leda::edge  get_edge() const { return _e; }
/*{\Mop     returns the marked edge or nil if |\Mvar.valid()| returns false.
  }*/
  
const leda::graph& get_graph() const  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  return *_g; }
/*{\Mop         returns the associated graph. }*/
  
EdgeIt& operator++ () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=_g->succ_edge(_e);
  return *this; }
/*{\Munop       performs one step forward in the list of edges of the associated graph.
  If there is no successor edge, |\Mvar.eol()| will be true afterwards.
  This method returns a reference to |\Mvar|.
    
Precondition: |\Mvar.valid()| returns true.
}*/
  
EdgeIt operator++ (int)   {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  EdgeIt tmp(*this);
  _e=_g->succ_edge(_e);
  return tmp; }
  
EdgeIt& operator-- () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=_g->pred_edge(_e);
  return *this; }
/*{\Munop       performs one step backward in the list of edges of the associated graph.
  If there is no predecessor edge, |\Mvar.eol()| will be true afterwards.
  This method returns a reference to |\Mvar|.
    
Precondition: |\Mvar.valid()| returns true.
}*/
  
EdgeIt operator-- (int)   {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  EdgeIt tmp(*this);
  _e=_g->pred_edge(_e);
  return tmp; }
  
friend ostream& operator<<(ostream& out, const self& )
{  return out;}
  
friend istream& operator>>(istream& in,  self&)
{  return in;}
  
  
};
/*{\Mimplementation
  Creation of an iterator and all methods take constant time.
}*/

template<class grapht>
class SafeEdgeIt : public SafeIteratorBase<leda::node>  {
public:
  typedef leda::edge       edgetype;
  typedef leda::node       nodetype;
  typedef SafeGraph<grapht> graphtype;
  typedef typename SafeGraph<grapht>::graphtype basetype;
private:
  edgetype                       _e;
  SafeGraph<grapht>*                     _g;
  refresh_mode                   RefreshMode;
  bool                  _escaped;
  typedef SafeEdgeIt<grapht> self;
public:
  
  SafeEdgeIt() { _g=nil; }
  
  SafeEdgeIt(SafeGraph<grapht>& G, leda::edge  e=nil,
	     refresh_mode RM=mode_forward) : _e(e), _g(&G), RefreshMode(RM) {
    if (_e==nil) _e=_g->get_graph().first_edge();
    _g->insert(_e,this);
    _escaped=false;
}
  
SafeEdgeIt(SafeGraph<grapht>& G, refresh_mode RM) :  _g(&G), RefreshMode(RM) {
  _escaped=false;
  _e=G->get_graph().first_edge();
  _g->insert(_e,this);
}
  
~SafeEdgeIt() { if (valid()) _g->remove(_e,this); }
  
  
void init(const graphtype& G) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  graphtype& Gp(*((graphtype*)(&G)));
  init(G,((basetype)Gp).first_edge()); }
  
void init(const graphtype& G, const leda::edge& e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  if (_g!=nil) _g->remove(_e,this);
  _g=(graphtype*)(&G);
  _e=e;
  _g->insert(_e,this); }
  
void reset() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).first_edge();
  _g->insert(_e,this); }
  
bool escaped() const {
  return _escaped; }
  
void make_not_escaped() {
  _escaped=false; }
  
void reset_end(){
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).last_edge();
  _g->insert(_e,this); }
  
void make_invalid() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=nil;
  _g->insert(_e,this);}
  
void insert(leda::node v1, leda::node v2) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).new_edge(v1,v2);
  _g->insert(_e,this); }
  
void refresh_object(leda::node v) {
  _escaped=true;
  _g->remove(_e,this);
  next_edge(v);
  if (_e!=nil) {
    _g->insert(_e,this); }
}
  
void next_edge(leda::node v=nil) {
  if (RefreshMode==mode_forward) {
    leda::edge temp(_e);
    while(1) {
      _e=((leda::graph&)(*_g)).succ_edge(_e);
      if (_e==nil)  _e=((leda::graph&)(*_g)).first_edge();
      if (_e==nil) break;
      if (_e==temp) break;
      if (v!=source(_e) && v!=target(_e)) break;
    }
    if (temp==_e) _e=nil;
  } else if (RefreshMode==mode_backward) {
    edgetype temp(_e);
    while(1) {
      _e=((leda::graph&)(*_g)).pred_edge(_e);
      if (_e==nil) _e=((leda::graph&)(*_g)).last_edge();
      if (_e==nil) break;
      if (_e==temp) break;
      if (v!=source(_e) && v!=target(_e)) break;
    }
    if (temp==_e) _e=nil;
  } else
    _e=nil;
}
  
  
void del() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(503,"Iterator: del(), leda::graph not initialized\n");
#endif
  if (valid()) {
    edgetype temp(_e);
    next_edge();
    _g->update_and_delete(temp,this);
    _g->remove(temp,this);
    if (_e!=nil) {
      _g->insert(_e,this);
    }
  }
}
  
void update(leda::edge  e)  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=e;
  _g->insert(_e,this); }
  
void init(const SafeEdgeIt& p) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  _escaped=p._escaped;
  _e=p._e;
  _g=p._g;
  RefreshMode=p.RefreshMode;
  _g->insert(_e,this); }
  
  
SafeEdgeIt (const SafeEdgeIt& p) :   _e(p._e), _g(p._g) ,
  RefreshMode(p.RefreshMode), _escaped(p._escaped) { _g->insert(_e,this); }
  
SafeEdgeIt& operator= (const SafeEdgeIt& it2) {
  _g=it2._g;
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=it2._e;
  return *this;
  _g->insert(_e,this); }
  
bool operator== (const SafeEdgeIt& it2) const {
  return _e==it2._e; }
  
bool eol() const { return _e==nil; }
  
bool valid() const { return _e!=nil; }
 
operator leda::edge() const{ return _e; }
operator leda::edge() { return _e; }
    
leda::edge&  get_edge() { return _e; }
    
leda::edge  get_edge() const { return _e; }
 
const SafeGraph<grapht>& get_graph() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  return *_g; }
  
SafeEdgeIt& operator++ () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).succ_edge(_e);
  _g->insert(_e,this);
  return *this; }
  
SafeEdgeIt operator++ (int) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  SafeEdgeIt tmp(*this);
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).succ_edge(_e);
  _g->insert(_e,this);
  return tmp; }
  
SafeEdgeIt& operator-- () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).pred_edge(_e);
  _g->insert(_e,this);
  return *this; }
  
SafeEdgeIt operator-- (int) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  SafeEdgeIt tmp(*this);
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).pred_edge(_e);
  _g->insert(_e,this);
  return tmp; }
friend ostream& operator<<(ostream& out, const self& )
{  return out;}
  
friend istream& operator>>(istream& in,  self& )
{  return in;}
  
};


/*{\Manpage {FaceIt} {} {Face Iterators} {it} }*/
/*{\Moptions latexruns=2 numbered=yes}*/
/*{\Mdefinition a variable |it| of class  |\Mname| is
  a linear face iterator that iterates over the face set
  of a graph;
  the current face of an
  iterator object is said to be ``marked'' by this object.

{\bf Precondition:} Before using any face iterator
the list of faces
has to be computed by calling |G.compute_faces()|. Note, that
any update operation invalidates this list.  
}*/
class FaceIt {
  /*{\Mcreation }*/
public:
  typedef leda::face      facetype;
  typedef leda::graph     graphtype;
  typedef FaceIt self;
private:
  leda::face                      _n;
  const leda::graph*              _g;
public:
  
  FaceIt() { _g=nil; }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    associated with no graph. }*/
  
  FaceIt(const leda::graph& G) : _n(G.first_face()), _g(&G) { }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    associated with |G|.
    
    The graph is initialized by |G|.
    The face is initialized by |G.first_face()|.
    }*/
  
  
  FaceIt(const leda::graph& G, leda::face n ) : _n(n), _g(&G) { }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    marked with |n| and
    associated with |G|.
    
    Precondition: |n| is a face of |G|.}*/
  
  void init(const FaceIt& p)  {
    _g=p._g; update(p._n); }
  
  
  FaceIt (const FaceIt& p) :  _n(p._n), _g(p._g) { }
  /*{\Moperations}*/
  
  void init(const leda::graph& G)  {
    init(G,G.first_face()); }
  
  /*{\Mop associates |\Mvar| with |G| and marks it with
    |G.first_face()|. }*/
  
  void init(const leda::graph& G, const leda::face& v) {
    _g=(graphtype*)(&G); update(v); }
  
  /*{\Mop associates |\Mvar| with |G| and marks it with
    |v|. }*/
  
  void reset() { _n=_g->first_face(); }
  /*{\Mop  resets |\Mvar| to |G.first_face()|, where |G| is the associated graph. }*/
  
  void make_invalid() { _n=nil; }
  /*{\Mop makes |\Mvar| invalid, i.e. |\Mvar.valid()| will be false afterwards and
    |\Mvar| marks no face.}*/
  
  void reset_end() { _n=_g->last_face(); }
  /*{\Mop  resets |\Mvar| to |G.last_face()|, where |G| is the associated graph. }*/
  
  void update(leda::face  n) { _n=n; }
  /*{\Mop |it| marks |n| afterwards. }*/
  
  
  FaceIt& operator= (const FaceIt& it2)  { _n=it2._n;  _g=it2._g;  return *this; }
  /*{\Mbinop |it| is afterwards associated with the same graph and face as |it2|.
    This method returns a reference to |\Mvar|.
    }*/
  
  bool operator== (const FaceIt& it2) const { return _n==it2._n; }
  /*{\Mbinop returns true if and only if |it| and |it2| are equal, i.e. if the marked faces are equal.}*/
  
  operator leda::face() const { return _n; }
  operator leda::face() { return _n; }
  
  leda::face& get_face() { return _n; }

  leda::face  get_face() const { return _n; }
  /*{\Mop         returns the marked face or nil if |\Mvar.valid()| returns false.
    }*/
  
  const leda::graph& get_graph() const { return *_g; }
  /*{\Mop         returns the associated graph. }*/
  
  bool valid() const { return _n!=nil; }
  /*{\Mop        returns true if and only if end of sequence not yet passed, i.e.
    if there is a face in the face set that was not yet passed.
    }*/
  
  bool eol() const { return _n==nil; }
  /*{\Mop   returns |!\Mvar.valid()| which is
    true if and only if there is no successor face left,
    i.e. if all faces of the face set are passed (eol: end of list). }*/
  
  FaceIt& operator++ ()  { _n=_g->succ_face(_n); return *this; }
  /*{\Munop       performs one step forward in the list of faces of the associated graph.
    If there is no successor face, |\Mvar.eol()| will be true afterwards.
    This method returns a reference to |\Mvar|.
    
    Precondition: |\Mvar.valid()| returns true.
    }*/
  
  FaceIt operator++ (int) {
    FaceIt tmp(*this);
    _n=_g->succ_face(_n);
    return tmp; }
  
  FaceIt& operator-- () {
    _n=_g->pred_face(_n);
    return *this; }
    /*{\Munop performs one step backward in the list of faces 
    of the associated graph.
    If there is no predecessor face, |\Mvar.eol()| will be true afterwards.
    This method returns a reference to |\Mvar|.
    
    Precondition: |\Mvar.valid()| returns true.
    }*/
  
  FaceIt operator-- (int) {
    FaceIt tmp(*this);
    _n=_g->pred_face(_n);
    return tmp; }
  
  friend ostream& operator<<(ostream& out, const self& )
    {  return out;}
  
  friend istream& operator>>(istream& in,  self& )
    {  return in;}
  
  
};

/*{\Mimplementation
  Creation of an iterator and all methods take constant time.
}*/


class OutAdjCirc {
public:
  typedef leda::edge      edgetype;
  typedef leda::node      nodetype;
  typedef leda::graph     graphtype;
private:
  nodetype                      _n;
  edgetype                      _e;
  graphtype*                    _g;
public:
  typedef OutAdjCirc self;
  
  OutAdjCirc() { _g=nil; }
  
  void init(const OutAdjCirc& p) {
#if !defined(LEDA_CHECKING_OFF)
    if (_g!=nil) LEDA_EXCEPTION(501,"Circulators: leda::graph already initialized\n");
#endif
    _n=p._n; _e=p._e; _g=p._g; }
  
  
OutAdjCirc(const OutAdjCirc& p) :
  _n(p._n), _e(p._e), _g(p._g) {  }
  
OutAdjCirc(const leda::graph& G) : _n(G.first_node()), _g((graphtype*)&G) {
  if (_n==nil) _e=nil; else
    _e=G.first_adj_edge(_n);  }
  
OutAdjCirc(const leda::graph& G,leda::node  n) :  _n(n),  _g((graphtype*)&G)  {
  if (_n==nil) _e=nil; else
    _e=G.first_adj_edge(_n); }
  
OutAdjCirc(const leda::graph& G,leda::node  n,leda::edge e) :  _n(n) ,_e(e),  _g((graphtype*)&G) { }
  
void init(const leda::graph& G) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Circulators: leda::graph already initialized\n");
#endif
  init(G,G.first_node()); }

void init(const leda::graph& G, const leda::node& n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Circulators: leda::graph already initialized\n");
#endif
  init(G,n,G.first_adj_edge(n)); }

void init(const leda::graph& G, const leda::node& n, const leda::edge& e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Circulators: leda::graph already initialized\n");
#endif
  _g=(graphtype*)(&G); update(n,e); }

void update(leda::edge  e) { _e=e; }

void reset() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _e=_g->first_adj_edge(_n); }

void insert(leda::node v1, leda::node v2) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Circulators: insert(..), leda::graph not initialized\n");
#endif
  _e=_g->new_edge(v1,v2); }

void insert(leda::node v) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Circulators: insert(..), leda::graph not initialized\n");
#endif
  _e=_g->new_edge(_n,v); }

void insert(const OutAdjCirc& other) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Circulators: insert(..), leda::graph not initialized\n");
#endif
  leda::node v2(other.get_node());
  _e=_g->new_edge(_n,v2); }

void del() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(503,"Circulators: del(), leda::graph not initialized\n");
#endif
  _g->del_edge(_e); _e=nil;}

void reset_end() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _e=_g->last_adj_edge(_n); }

void make_invalid() {
  _e=nil; }

void update(leda::node  n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _n=n;
  _e=_g->first_adj_edge(_n); }

void update(leda::node  n,    leda::edge  e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _n=n; _e=e; }

OutAdjCirc& operator= (const OutAdjCirc& it2) { _n=it2._n; _e=it2._e;  _g=it2._g;
 return *this; }

bool operator== (const OutAdjCirc& it2) const  {
  return _e==it2._e; }

bool has_node() const{ return _n!=nil; }

bool eol() const { return _e==nil; }

bool valid() const { return _e!=nil; }

leda::edge&  get_edge() { return _e; }
    
leda::edge  get_edge() const { return _e; }

leda::node& get_node() { return _n; }

leda::node  get_node() const { return _n; }

const leda::graph& get_graph() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  return *_g; }

operator leda::edge() const{ return _e; }
operator leda::edge() { return _e; }
   
OutAdjCirc curr_adj() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  OutAdjCirc intmp(*this);
  intmp._n=target(intmp._e);
  intmp._e=_g->first_adj_edge(intmp._n);
  return intmp; }

OutAdjCirc& operator++ () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _e=_g->cyclic_adj_succ(_e);
  return *this; }

OutAdjCirc operator++ (int) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  OutAdjCirc tmp(*this);
  _e=_g->cyclic_adj_succ(_e);
  return tmp; }

OutAdjCirc& operator-- () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _e=_g->cyclic_adj_pred(_e);
  return *this; }

OutAdjCirc operator-- (int)  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  OutAdjCirc tmp(*this);
  _e=_g->cyclic_adj_pred(_e);
  return tmp; }

friend ostream& operator<<(ostream& out, const self&)
{  return out;}

friend istream& operator>>(istream& in,  self&)
{  return in;}

};

class InAdjCirc {
public:
  typedef leda::edge      edgetype;
  typedef leda::node      nodetype;
  typedef leda::graph     graphtype;
private:
  nodetype                      _n;
  edgetype                      _e;
  graphtype*                    _g;
public:
  typedef InAdjCirc self;

  InAdjCirc() { _g=nil; }

  void init(const InAdjCirc& p ) {
#if !defined(LEDA_CHECKING_OFF)
    if (_g!=nil) LEDA_EXCEPTION(501,"Circulators: leda::graph already initialized\n");
#endif
    _n=p._n; _e=p._e; _g=p._g; }

InAdjCirc(const InAdjCirc& p) : _n(p._n), _e(p._e), _g(p._g) {  }

InAdjCirc(const leda::graph& G) : _n(G.first_node()), _g((graphtype*)&G) {
  if (_n==nil) _e=nil; else
    _e=G.first_in_edge(_n);  }

InAdjCirc(const leda::graph& G,leda::node  n) :  _n(n), _g((graphtype*)&G)  {
  if (_n==nil) _e=nil; else
    _e=G.first_in_edge(_n); }

InAdjCirc(const leda::graph& G,leda::node  n,leda::edge e) :  _n(n),_e(e), _g((graphtype*)&G)  { }


void init(const leda::graph& G) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Circulators: leda::graph already initialized\n");
#endif
  init(G,G.first_node()); }

void init(const leda::graph& G, const leda::node& n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Circulators: leda::graph already initialized\n");
#endif
  init(G,n,G.first_in_edge(n)); }


void init(const leda::graph& G,
	  const leda::node& n,
	  const leda::edge& e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Circulators: leda::graph already initialized\n");
#endif
  _g=(graphtype*)(&G); update(n,e); }


void update(leda::edge  e) { _e=e; }

void reset() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _e=((leda::graph&)(*_g)).first_in_edge(_n); }

void insert(leda::node v1, leda::node v2) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Circulators: insert(..), leda::graph not initialized\n");
#endif
  _e=((leda::graph&)(*_g)).new_edge(v1,v2); }

void insert(const InAdjCirc& other) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Circulators: insert(..), leda::graph not initialized\n");
#endif
  leda::node v2(other.get_node());
  _e=((leda::graph&)(*_g)).new_edge(_n,v2); }

void insert(leda::node v) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Circulators: insert(..), leda::graph not initialized\n");
#endif
  _e=((leda::graph&)(*_g)).new_edge(_n,v); }

void del() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(503,"Circulators: del(), leda::graph not initialized\n");
#endif
  ((leda::graph&)(*_g)).del_edge(_e); _e=nil; }

void reset_end() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _e=((leda::graph&)(*_g)).last_in_edge(_n); }

void make_invalid() {
  _e=nil; }

void update(leda::node  n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _n=n;
  _e=((leda::graph&)(*_g)).first_in_edge(_n); }

void update(leda::node  n,
	    leda::edge  e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _n=n; _e=e; }

InAdjCirc& operator= (const InAdjCirc& it2) { _n=it2._n; _e=it2._e;  _g=it2._g;
 return *this; }

bool operator== (const InAdjCirc& it2) const  {
  return _e==it2._e; }

bool has_node() const{ return _n!=nil; }

bool eol() const { return _e==nil; }

bool valid() const { return _e!=nil; }

leda::edge&  get_edge() { return _e; }
    
leda::edge  get_edge() const { return _e; }

leda::node& get_node() { return _n; }

leda::node  get_node() const { return _n; }

const leda::graph& get_graph() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iintrator: leda::graph not initialized\n");
#endif
  return *_g; }

operator leda::edge() const{ return _e; }
operator leda::edge() { return _e; }
   
InAdjCirc curr_adj() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  InAdjCirc intmp(*this);
  intmp._n=source(intmp._e);
  intmp._e=((leda::graph&)(*_g)).first_in_edge(intmp._n);
  return intmp; }

InAdjCirc& operator++ () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _e=((leda::graph&)(*_g)).in_succ(_e);
  return *this; }

InAdjCirc operator++ (int)  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  InAdjCirc tmp(*this);
  _e=((leda::graph&)(*_g)).in_succ(_e);
  return tmp; }

InAdjCirc& operator-- () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  _e=_g->in_pred(_e);
  return *this; }

InAdjCirc operator-- (int)  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Circulators: leda::graph not initialized\n");
#endif
  InAdjCirc tmp(*this);
  _e=_g->in_pred(_e);
  return tmp; }

friend ostream& operator<<(ostream& out, const self& )
{  return out;}

friend istream& operator>>(istream& in,  self& )
{  return in;}

};


/*{\Manpage {OutAdjIt} {} {Adjacency Iterators for leaving edges} {it} }*/
/*{\Mdefinition
  a variable |it| of class  |\Mname| is
  an adjacency iterator that marks a node
  (which is fixed in contrast to linear node iterators)
  and iterates over the edges that leave this node.

There is a  variant of the adjacency iterators, so--called
circulators which are heavily used in the CGAL\footnote{See the CGAL homepage at {\tt http://www.cs.uu.nl/CGAL/}.}. 
\index{CGAL} \index{circulators}
The
names of the classes are {\tt OutAdjCirc} and {\tt InAdjCirc}
and their interfaces are completely equal to the iterator
versions while they internally use e.g. {\tt cyclic\_adj\_succ()}
instead of {\tt adj\_succ()}.


}*/
class OutAdjIt {
  /*{\Mcreation}*/
public:
  typedef leda::edge      edgetype;
  typedef leda::node      nodetype;
  typedef leda::graph     graphtype;
private:
  nodetype                      _n;
  edgetype                      _e;
  graphtype*                    _g;
public:
  typedef OutAdjIt self;
  
  OutAdjIt() { _g=nil; }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    associated with no graph. }*/
  
  void init(const OutAdjIt& p) {
#if !defined(LEDA_CHECKING_OFF)
    if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
    _n=p._n; _e=p._e; _g=p._g; }
  
  
OutAdjIt(const OutAdjIt& p) :
  _n(p._n), _e(p._e), _g(p._g) {  }
  
OutAdjIt(const leda::graph& G) : _n(G.first_node()), _g((graphtype*)&G) {
  if (_n==nil) _e=nil; else
    _e=G.first_adj_edge(_n);  }
/*{\Mcreate introduces a variable |\Mvar| of this class
  associated with |G|.
    
  The node is initialized
  by |G.first_node()| and the edge by |G.first_adj_edge(n)| where
  |n| is the marked node.
  }*/
  
OutAdjIt(const leda::graph& G,leda::node  n) :  _n(n),  _g((graphtype*)&G)  {
  if (_n==nil) _e=nil; else
    _e=G.first_adj_edge(_n); }
/*{\Mcreate introduces a variable |\Mvar| of this class marked with |n|
  and associated with |G|.
  The marked edge is initialized by  |G.first_adj_edge(n)|.
    
  Precondition: |n| is a node of |G|.}*/
  
OutAdjIt(const leda::graph& G,leda::node  n,leda::edge e) :  _n(n) ,_e(e),  _g((graphtype*)&G) { }
/*{\Mcreate introduces a variable |\Mvar| of this class marked with |n|
  and |e|
  and associated with |G|.
    
  Precondition: |n| is a node and |e| an edge of |G| and |source(e)=n|.}*/
  
/*{\Moperations }*/
void init(const leda::graph& G) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  init(G,G.first_node()); }
/*{\Mop associates |\Mvar| with |G| and marks it with
  |n'=G.first_node()| and |G.first_adj_edge(n')|. }*/

void init(const leda::graph& G, const leda::node& n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  init(G,n,G.first_adj_edge(n)); }
/*{\Mop associates |\Mvar| with |G| and marks it with
  |n| and |G.first_adj_edge(n)|.
  
  Precondition: |n| is a node of |G|.}*/

void init(const leda::graph& G, const leda::node& n, const leda::edge& e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  _g=(graphtype*)(&G); update(n,e); }
/*{\Mop associates |\Mvar| with |G| and marks it with
  |n| and |e|.
  
  Precondition: |n| is a node and |e| an edge of |G| and |source(e)=n|.}*/

void update(leda::edge  e) { _e=e; }
/*{\Mop |it| marks |e| afterwards. }*/

void reset() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=_g->first_adj_edge(_n); }
/*{\Mop resets |\Mvar| to |G.first_adj_edge(n)| where |G|
  and |n| are the marked node and associated graph.
  }*/

void insert(leda::node v1, leda::node v2) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  _e=_g->new_edge(v1,v2); }

void insert(leda::node v) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  _e=_g->new_edge(_n,v); }

void insert(const OutAdjIt& other) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  leda::node v2(other.get_node());
  _e=_g->new_edge(_n,v2); }
/*{\Mop    creates a new leaving edge from the marked node of |\Mvar|
  to the marked node of |other|. |\Mvar| is marked with the new edge afterwards. The marked node of |\Mvar| does not change. }*/

void del() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(503,"Iterator: del(), leda::graph not initialized\n");
#endif
  _g->del_edge(_e); _e=nil;}
/*{\Mop  deletes the marked leaving edge, i.e. |\Mvar.valid()| returns false afterwards.
  
  Precondition: |\Mvar.valid()| returns true.  }*/

void reset_end() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=_g->last_adj_edge(_n); }
/*{\Mop resets |\Mvar| to |G.last_adj_edge(n)| where |G|
  and |n| are the marked node and associated graph.
  }*/

void make_invalid() {
  _e=nil; }
/*{\Mop makes |\Mvar| invalid, i.e. |\Mvar.valid()| will be false afterwards and
  |\Mvar| marks no node.}*/

void update(leda::node  n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _n=n;
  _e=_g->first_adj_edge(_n); }
/*{\Mop  |\Mvar| marks |n| and the first leaving edge of |n| afterwards. }*/

void update(leda::node  n,    leda::edge  e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _n=n; _e=e; }
/*{\Mop  |it| marks |n| and |e| afterwards. }*/

OutAdjIt& operator= (const OutAdjIt& it2) { _n=it2._n; _e=it2._e;  _g=it2._g;
 return *this; }
/*{\Mbinop assigns |it2| to |\Mvar|. This method returns a reference to |\Mvar|.}*/

bool operator== (const OutAdjIt& it2) const  {
  return _e==it2._e; }
/*{\Mbinop returns true if and only if |it| and |it2| are equal, i.e. if the marked nodes and edges are equal.}*/

bool has_node() const{ return _n!=nil; }
/*{\Mop returns  true if and only if |\Mvar| marks a node.}*/

bool eol() const { return _e==nil; }
/*{\Mop   returns |!\Mvar.valid()| which is
  true if and only if there is no successor edge left,
  i.e. if all edges of the edge set are passed (eol: end of list).
  }*/

bool valid() const { return _e!=nil; }
/*{\Mop
  returns true if and only if end of sequence not yet passed, i.e.
  if there is an edge in the  edge set that was not yet passed.
  }*/

leda::edge&  get_edge() { return _e; }
    
leda::edge  get_edge() const { return _e; }
/*{\Mop         returns the marked edge or nil if |\Mvar.valid()| returns false.
  }*/

leda::node& get_node() { return _n; }

leda::node  get_node() const { return _n; }
/*{\Mop         returns the marked node or nil if |\Mvar.has_node()| returns false.
  }*/

const leda::graph& get_graph() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  return *_g; }
/*{\Mop         returns the associated graph. }*/

operator leda::edge() const{ return _e; }
operator leda::edge() { return _e; }
   
OutAdjIt curr_adj() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  OutAdjIt intmp(*this);
  intmp._n=target(intmp._e);
  intmp._e=_g->first_adj_edge(intmp._n);
  return intmp; }
/*{\Mop returns a new adjacency iterator that is
  associated with |n'=target(e)| and |G.first_adj_edge(n')|
  where |G| is the associated graph.

  Precondition: |\Mvar.valid()| returns true.
  }*/

OutAdjIt& operator++ () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=_g->adj_succ(_e);
  return *this; }
/*{\Munop       performs one step forward in the list of outgoing edges of the marked node.
  If there is no successor edge, |\Mvar.eol()| will be true afterwards.
  This method returns a reference to |\Mvar|.

Precondition: |\Mvar.valid()| returns true.
}*/

OutAdjIt operator++ (int) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  OutAdjIt tmp(*this);
  _e=_g->adj_succ(_e);
  return tmp; }

OutAdjIt& operator-- () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=_g->adj_pred(_e);
  return *this; }
/*{\Munop       performs one step backward in the list of outgoing edges of the marked node.
  If there is no predecesssor edge, |\Mvar.eol()| will be true afterwards.
  This method returns a reference to |\Mvar|.

Precondition: |\Mvar.valid()| returns true.
}*/

OutAdjIt operator-- (int)  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  OutAdjIt tmp(*this);
  _e=_g->adj_pred(_e);
  return tmp; }

friend ostream& operator<<(ostream& out, const self&)
{  return out;}

friend istream& operator>>(istream& in,  self&)
{  return in;}

};
/*{\Mimplementation
Creation of an iterator and all methods take constant time.
}*/

template<class grapht>
class SafeOutAdjIt  : public SafeIteratorBase<leda::node> {
public:
  typedef leda::edge      edgetype;
  typedef leda::node      nodetype;
  typedef SafeGraph<grapht> graphtype;
  typedef typename SafeGraph<grapht>::graphtype basetype;
private:
  nodetype                      _n;
  edgetype                      _e;
  SafeGraph<grapht>*                    _g;
  refresh_mode                   RefreshMode;
  bool                          _escaped;
  typedef SafeOutAdjIt<grapht> self;
public:

  SafeOutAdjIt() { _g=nil; }

  SafeOutAdjIt(SafeGraph<grapht>& G,
	       leda::node  n=nil,
	       refresh_mode RM=mode_forward) :  _n(n), _g(&G), RefreshMode(RM) {
    if (_n==nil) _n=((leda::graph&)(*_g)).first_node();
    _e=_g->get_graph().first_adj_edge(_n);
    _escaped=false;
    _g->insert(_e,this);
}

SafeOutAdjIt(SafeGraph<grapht>& G, refresh_mode RM) : _g(&G), RefreshMode(RM) {
  _n=_g->get_graph().first_node();
  _e=_g->get_graph().first_adj_edge(_n);
  _g->insert(_e,this);
  _escaped=false;
}

SafeOutAdjIt(SafeGraph<grapht>& G,
	     leda::node  n, leda::edge e,
	     refresh_mode RM=mode_forward) :   _n(n), _e(e), _g(&G),RefreshMode(RM) {
  _escaped=false;
  _g->insert(_e,this);
}

~SafeOutAdjIt() {
  if (valid())
    _g->remove(_e,this); }

void insert(leda::node v1, leda::node v2) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).new_edge(v1,v2);
  _g->insert(_e,this); }

void init(const graphtype& G) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  graphtype& Gp(*((graphtype*)(&G)));
  init(G,((basetype)Gp).first_node()); }

void init(const graphtype& G, const leda::node& n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  graphtype& Gp(*((graphtype*)(&G)));
  init(G,n,((basetype)Gp).first_adj_edge(n)); }

void init(const graphtype& G,
	  const leda::node& n,
	  const leda::edge& e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  if (_g!=nil) _g->remove(_e,this);
  _g=(graphtype*)(&G);
  _n=n;
  _e=e;
  _g->insert(_e,this); }


void insert(leda::node v) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).new_edge(_n,v);
  _g->insert(_e,this); }

void insert(const SafeOutAdjIt& other) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  leda::node v2(other.get_node());
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).new_edge(_n,v2);
  _g->insert(_e,this); }

void del() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(503,"Iterator: del(), leda::graph not initialized\n");
#endif
  if (valid()) {
    edgetype temp(_e);
    next_edge();
    _g->update_and_delete(temp,this);
    _g->remove(temp,this);
    if (_e!=nil) {
      _g->insert(_e,this);
    }
  }
}


void reset() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).first_adj_edge(_n);
  _g->insert(_e,this); }

bool escaped() const {
  return _escaped; }

void make_not_escaped() {
  _escaped=false; }

void reset_end() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).last_adj_edge(_n);
  _g->insert(_e,this); }

void make_invalid() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=nil;
  _g->insert(_e,this);}

void  refresh_object(leda::node v) {
  _escaped=true;
  _g->remove(_e,this);
  next_edge(v);
  if (_e!=nil) {
    _g->insert(_e,this); }
}
void next_edge(leda::node v=nil) {
  if (RefreshMode==mode_forward) {
    edgetype temp(_e);
    if (v==source(_e)) _e=nil;
    while(_e!=nil) {
      _e=((leda::graph&)(*_g)).adj_succ(_e);
      if (_e==nil) _e=((leda::graph&)(*_g)).first_adj_edge(source(_e));
      if (_e==nil) break;
      if (_e==temp) break;
      if (v!=target(_e)) break;
    }
    if (temp==_e) _e=nil;
  } else if (RefreshMode==mode_backward) {
    edgetype temp(_e);
    if (v==source(_e)) _e=nil;
    while(_e!=nil) {
      _e=((leda::graph&)(*_g)).adj_pred(_e);
      if (_e==nil) _e=((leda::graph&)(*_g)).last_adj_edge(source(_e));
      if (_e==nil) break;
      if (temp==_e) break;
      if (v!=source(_e) && v!=target(_e)) break;
    }
    if (temp==_e) _e=nil;
  } else _e=nil;
}

void init(const SafeOutAdjIt& p) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  _escaped=p._escaped;
  _n=p._n;
  _e=p._e; _g=p._g;
  RefreshMode=p.RefreshMode;
  _g->insert(_e,this); }

SafeOutAdjIt(const SafeOutAdjIt& p) :  _n(p._n), _e(p._e), _g(p._g),
  RefreshMode(p.RefreshMode),  _escaped(p._escaped) {
  _g->insert(_e,this);  }

void update(leda::edge  e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=e;
  _g->insert(_e,this); }

void update(leda::node  n,
	    leda::edge  e=nil) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _n=n;
  if (e==nil) _e=((leda::graph&)(*_g)).first_adj_edge(_n);
  else _e=e;
  _g->insert(_e,this);
}

SafeOutAdjIt& operator= (const SafeOutAdjIt& it2) {
  _g=it2._g;
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _n=it2._n; _e=it2._e;
  _g->insert(_e,this);
  return *this; }

bool operator== (const SafeOutAdjIt& it2) const  {
  return _e==it2._e; }

bool has_node() const{ return _n!=nil; }

bool eol() const { return _e==nil; }

bool valid() const { return _e!=nil; }

leda::edge&  get_edge() { return _e; }
    
leda::edge  get_edge() const { return _e; }
 
leda::node& get_node() { return _n; }

leda::node  get_node() const { return _n; }

const SafeGraph<grapht>& get_graph() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  return *_g; }

operator leda::edge() const{ return _e; }
operator leda::edge() { return _e; }
   
SafeOutAdjIt curr_adj() const  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  SafeOutAdjIt temp(*this);
  temp._n=target(temp._e);
  temp._e=((leda::graph&)(*_g)).first_adj_edge(temp._n);
  return temp; }

SafeOutAdjIt& operator++ () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).adj_succ(_e);
  _g->insert(_e,this);
  return *this; }

SafeOutAdjIt operator++ (int) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  SafeOutAdjIt tmp(*this);
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).adj_succ(_e);
  _g->insert(_e,this);
  return tmp; }

SafeOutAdjIt& operator-- () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).adj_pred(_e);
  _g->insert(_e,this);
  return *this; }

SafeOutAdjIt operator-- (int) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  SafeOutAdjIt tmp(*this);
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).adj_pred(_e);
  _g->insert(_e,this);
  return tmp; }

friend ostream& operator<<(ostream& out, const self& )
{  return out;}

friend istream& operator>>(istream& in,  self& )
{  return in;}

};
/*{\Manpage {InAdjIt} {} {Adjacency Iterators for incoming edges} {it} }*/
/*{\Mdefinition
a variable |it| of class  |\Mname| is
an adjacency iterator that marks a node
(which is fixed in contrast to linear node iterators)
and iterates over the incoming edges of this node.
}*/
class InAdjIt {
  /*{\Mcreation}*/
public:
  typedef leda::edge      edgetype;
  typedef leda::node      nodetype;
  typedef leda::graph     graphtype;
private:
  nodetype                      _n;
  edgetype                      _e;
  graphtype*                    _g;
public:
  typedef InAdjIt self;

  InAdjIt() { _g=nil; }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    associated with no graph. }*/

  void init(const InAdjIt& p ) {
#if !defined(LEDA_CHECKING_OFF)
    if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
    _n=p._n; _e=p._e; _g=p._g; }

InAdjIt(const InAdjIt& p) : _n(p._n), _e(p._e), _g(p._g) {  }

InAdjIt(const leda::graph& G) : _n(G.first_node()), _g((graphtype*)&G) {
  if (_n==nil) _e=nil; else
    _e=G.first_in_edge(_n);  }
/*{\Mcreate introduces a variable |\Mvar| of this class
  associated with |G|.

  The node is initialized
  by |G.first_node()| and the edge by |G.first_in_edge(n)|
  where |n| is the marked node.
  }*/

InAdjIt(const leda::graph& G,leda::node  n) :  _n(n), _g((graphtype*)&G)  {
  if (_n==nil) _e=nil; else
    _e=G.first_in_edge(_n); }
/*{\Mcreate introduces a variable |\Mvar| of this class marked with |n| and
  associated with |G|.
  The marked edge is initialized by |G.first_in_edge(n)|.

  Precondition: |n| is a node of |G|. }*/

InAdjIt(const leda::graph& G,leda::node  n,leda::edge e) :  _n(n),_e(e), _g((graphtype*)&G)  { }
/*{\Mcreate introduces a variable |\Mvar| of this class marked with |n|
  and |e|
  and associated with |G|.

  Precondition: |n| is a node and |e| an edge of |G| and |target(e)=n|.}*/

/*{\Moperations }*/

void init(const leda::graph& G) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  init(G,G.first_node()); }
/*{\Mop associates |\Mvar| with |G| and marks it with
  |n'=G.first_node()| and |G.first_adj_edge(n')|. }*/

void init(const leda::graph& G, const leda::node& n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  init(G,n,G.first_in_edge(n)); }
/*{\Mop associates |\Mvar| with |G| and marks it with
  |n| and |G.first_adj_edge(n)|.

  Precondition: |n| is a node of |G|. }*/


void init(const leda::graph& G,
	  const leda::node& n,
	  const leda::edge& e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  _g=(graphtype*)(&G); update(n,e); }
/*{\Mop associates |\Mvar| with |G| and marks it with
  |n| and |e|.

  Precondition: |n| is a node and |e| an edge of |G| and |target(e)=n|.}*/


void update(leda::edge  e) { _e=e; }
/*{\Mop |it| marks |e| afterwards. }*/

void reset() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=((leda::graph&)(*_g)).first_in_edge(_n); }
/*{\Mop resets |\Mvar| to |G.first_in_edge(n)| where |G|
  and |n| are the marked node and associated graph.
  }*/

void insert(leda::node v1, leda::node v2) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  _e=((leda::graph&)(*_g)).new_edge(v1,v2); }

void insert(const InAdjIt& other) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  leda::node v2(other.get_node());
  _e=((leda::graph&)(*_g)).new_edge(_n,v2); }
/*{\Mop    creates a new incoming edge from the marked node of |\Mvar|
  to the marked node of |other|. |\Mvar| is marked with the new edge afterwards. The marked node of |\Mvar| does not change. }*/

void insert(leda::node v) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  _e=((leda::graph&)(*_g)).new_edge(_n,v); }

void del() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(503,"Iterator: del(), leda::graph not initialized\n");
#endif
  ((leda::graph&)(*_g)).del_edge(_e); _e=nil; }
/*{\Mop  deletes the marked incoming edge, i.e. |\Mvar.valid()| returns false afterwards.

  Precondition: |\Mvar.valid()| returns true. }*/

void reset_end() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=((leda::graph&)(*_g)).last_in_edge(_n); }
/*{\Mop resets |\Mvar| to |G.last_in_edge(n)| where |G|
  and |n| are the marked node and associated graph.
  }*/

void make_invalid() {
  _e=nil; }
/*{\Mop makes |\Mvar| invalid, i.e. |\Mvar.valid()| will be false afterwards and
  |\Mvar| marks no node.}*/

void update(leda::node  n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _n=n;
  _e=((leda::graph&)(*_g)).first_in_edge(_n); }
/*{\Mop  |\Mvar| marks |n| and the first incoming edge of |n| afterwards.
  }*/

void update(leda::node  n,
	    leda::edge  e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _n=n; _e=e; }
/*{\Mop  |it| marks |n| and |e| afterwards. }*/

InAdjIt& operator= (const InAdjIt& it2) { _n=it2._n; _e=it2._e;  _g=it2._g;
 return *this; }
/*{\Mbinop assigns |it2| to |\Mvar|. This method returns a reference this method returns a
  reference to |\Mvar|.}*/

bool operator== (const InAdjIt& it2) const  {
  return _e==it2._e; }
/*{\Mbinop returns true if and only if |it| and |it2| are equal, i.e. if the marked nodes and edges are equal.}*/

bool has_node() const{ return _n!=nil; }
/*{\Mop returns  true if and only if |\Mvar| marks a node.}*/

bool eol() const { return _e==nil; }
/*{\Mop   returns |!\Mvar.valid()| which is
  true if and only if there is no successor edge left,
  i.e. if all edges of the edge set are passed (eol: end of list).
  }*/

bool valid() const { return _e!=nil; }
/*{\Mop
  returns true if and only if end of sequence not yet passed, i.e.
  if there is an edge in the  edge set that was not yet passed.
  }*/

leda::edge&  get_edge() { return _e; }
    
leda::edge  get_edge() const { return _e; }
/*{\Mop          returns the marked edge or nil if |\Mvar.valid()| returns false.
  }*/

leda::node& get_node() { return _n; }

leda::node  get_node() const { return _n; }
/*{\Mop   returns the marked node or nil if |\Mvar.has_node()| returns false.
  }*/

const leda::graph& get_graph() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iintrator: leda::graph not initialized\n");
#endif
  return *_g; }
/*{\Mop         returns the associated graph. }*/

operator leda::edge() const{ return _e; }
operator leda::edge() { return _e; }
   

InAdjIt curr_adj() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  InAdjIt intmp(*this);
  intmp._n=source(intmp._e);
  intmp._e=((leda::graph&)(*_g)).first_in_edge(intmp._n);
  return intmp; }
/*{\Mop returns a new adjacency iterator that is
  associated with |n'=source(e)| and |G.first_in_edge(n')|
  where |G| is the associated graph.

  Precondition: |\Mvar.valid()| returns true.
  }*/

InAdjIt& operator++ () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=((leda::graph&)(*_g)).in_succ(_e);
  return *this; }
/*{\Munop       performs one step forward in the list of incoming edges of the marked node.
  If there is no successor edge, |\Mvar.eol()| will be true afterwards.
  This method returns a reference to |\Mvar|.

Precondition: |\Mvar.valid()| returns true.
}*/

InAdjIt operator++ (int)  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  InAdjIt tmp(*this);
  _e=((leda::graph&)(*_g)).in_succ(_e);
  return tmp; }

InAdjIt& operator-- () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=_g->in_pred(_e);
  return *this; }
/*{\Munop       performs one step backward in the list of incoming edges of the marked node.
  If there is no predecesssor edge, |\Mvar.eol()| will be true afterwards.
  This method returns a reference to |\Mvar|.

Precondition: |\Mvar.valid()| returns true.
}*/

InAdjIt operator-- (int)  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  InAdjIt tmp(*this);
  _e=_g->in_pred(_e);
  return tmp; }

friend ostream& operator<<(ostream& out, const self& )
{  return out;}

friend istream& operator>>(istream& in,  self& )
{  return in;}

};
/*{\Mimplementation
Creation of an iterator and all methods take constant time.
}*/

template<class grapht>
class SafeInAdjIt  : public SafeIteratorBase<leda::node> {
public:
  typedef leda::edge      edgetype;
  typedef leda::node      nodetype;
  typedef SafeGraph<grapht> graphtype;
  typedef typename SafeGraph<grapht>::graphtype basetype;
private:
  nodetype                      _n;
  edgetype                      _e;
  SafeGraph<grapht>*                    _g;
  refresh_mode                   RefreshMode;
  bool                          _escaped;
  typedef SafeInAdjIt<grapht> self;
public:

  SafeInAdjIt() { _g=nil; }

  void init(const SafeInAdjIt& p) {
#if !defined(LEDA_CHECKING_OFF)
    if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
    _escaped=p._escaped;
    RefreshMode=p.RefreshMode;
    _n=p._n ; _e=p._e; _g=p._g; _g->insert(_e,this); }

SafeInAdjIt(const SafeInAdjIt& p) :   _n(p._n), _e(p._e), _g(p._g),
    RefreshMode(p.RefreshMode),_escaped(p._escaped) {
  _g->insert(_e,this);  }


SafeInAdjIt(SafeGraph<grapht>& G,
	    leda::node  n=nil,
	    refresh_mode RM=mode_forward) :  _n(n), _g(&G),RefreshMode(RM) {
  if (_n==nil) _n=((leda::graph&)(*_g)).first_node();
  _e=_g->get_graph().first_in_edge(_n);
  _g->insert(_e,this);
  _escaped=false;
}

SafeInAdjIt(SafeGraph<grapht>& G, refresh_mode RM) :
  _g(&G), RefreshMode(RM) {
  _n=((leda::graph&)(*_g)).first_node();
  _e=((leda::graph&)(*_g)).first_in_edge(_n);
  _g->insert(_e,this);
  _escaped=false;
}

SafeInAdjIt(SafeGraph<grapht>& G,
	    leda::node  n,leda::edge e,
	    refresh_mode RM=mode_forward) :  _n(n), _e(e),_g(&G),RefreshMode(RM) {
  _g->insert(_e,this);
  _escaped=false;
}

~SafeInAdjIt() {
  if (valid())
    _g->remove(_e,this); }


void init(const graphtype& G) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  graphtype& Gp(*((graphtype*)(&G)));
  init(G,((basetype)Gp).first_node()); }

void init(const graphtype& G, const nodetype& n) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  graphtype& Gp(*((graphtype*)(&G)));
  init(G,n,((basetype)Gp).first_in_edge(n)); }

void init(const graphtype& G,
	  const nodetype& n,
	  const edgetype& e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  if (_g!=nil) _g->remove(_e,this);
  _n=n;
  _g=(graphtype*)(&G); _e=e;
  _g->insert(_e,this); }


void insert(leda::node v1, leda::node v2) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).new_edge(v1,v2);
  _g->insert(_e,this); }

void insert(leda::node v) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).new_edge(_n,v);
  _g->insert(_e,this); }

void insert(const SafeInAdjIt& other) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(504,"Iterator: insert(..), leda::graph not initialized\n");
#endif
  leda::node v2(other.get_node());
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).new_edge(_n,v2);
  _g->insert(_e,this); }

void del() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(503,"Iterator: del(), leda::graph not initialized\n");
#endif
  if (valid()) {
    edgetype temp(_e);
    next_edge();
    _g->update_and_delete(temp,this);
    _g->remove(temp,this);
    if (_e!=nil) {
      _g->insert(_e,this);
    }
  }
}


void reset() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).first_in_edge(_n);
  _g->insert(_e,this); }

bool escaped() const {
  return _escaped; }

void make_not_escaped() {
  _escaped=false; }

void reset_end() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).last_in_edge(_n);
  _g->insert(_e,this); }

void make_invalid() {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=nil;
  _g->insert(_e,this);}

void  refresh_object(leda::node v) {
  _escaped=true;

  _g->remove(_e,this);
  next_edge(v);
  if (_e!=nil) {
    _g->insert(_e,this); }
}

void next_edge(leda::node v=nil) {
  if (RefreshMode==mode_forward) {
    edgetype temp(_e);
    if (v==target(_e)) _e=nil;
    while(_e!=nil) {
      _e=((leda::graph&)(*_g)).in_succ(_e);
      if (_e==nil) _e=((leda::graph&)(*_g)).first_in_edge(target(_e));
      if (_e==nil) break;
      if (_e==temp) break;
      if (v!=source(_e)) break;
    }
    if (temp==_e) _e=nil;
  } else if (RefreshMode==mode_backward) {
    edgetype temp(_e);
    if (v==target(_e)) _e=nil;
    while(_e!=nil) {
      _e=((leda::graph&)(*_g)).in_pred(_e);
      if (_e==nil) _e=((leda::graph&)(*_g)).last_in_edge(target(_e));
      if (_e==nil) break;
      if (temp==_e) break;
      if (v!=source(_e)) break;
    }
    if (temp==_e) _e=nil;
  } else _e=nil;
}

void update(leda::edge  e) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=e;
  _g->insert(_e,this); }

void update(leda::node  n,
	    leda::edge e=nil) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _n=n;
  if (e==nil) _e=((leda::graph&)(*_g)).first_in_edge(_n);
  else _e=e;
  _g->insert(_e,this);
}

SafeInAdjIt& operator= (const SafeInAdjIt& it2) {
  _g=it2._g;
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _n=it2._n; _e=it2._e;
  _g->insert(_e,this);
  return *this; }

bool operator== (const SafeInAdjIt& it2) const  {
  return _e==it2._e; }

bool has_node() const{ return _n!=nil; }

bool eol() const { return _e==nil; }

bool valid() const { return _e!=nil; }

leda::edge&  get_edge() { return _e; }
    
leda::edge  get_edge() const { return _e; }
 
leda::node& get_node() { return _n; }

leda::node  get_node() const { return _n; }

const SafeGraph<grapht>& get_graph() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  return *_g; }


operator leda::edge() const{ return _e; }
operator leda::edge() { return _e; }
   
SafeInAdjIt curr_adj() const  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  SafeInAdjIt temp(*this);
  temp._n=source(temp._e);
  temp._e=((leda::graph&)(*_g)).first_in_edge(temp._n);
  return temp; }


SafeInAdjIt& operator++ () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).in_succ(_e);
  _g->insert(_e,this);
  return *this; }

SafeInAdjIt operator++ (int) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  SafeInAdjIt tmp(*this);
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).in_succ(_e);
  _g->insert(_e,this);
  return tmp; }

SafeInAdjIt& operator-- () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).in_pred(_e);
  _g->insert(_e,this);
  return *this; }


SafeInAdjIt operator-- (int) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  SafeInAdjIt tmp(*this);
  _g->remove(_e,this);
  _e=((leda::graph&)(*_g)).in_pred(_e);
  _g->insert(_e,this);
  return tmp; }

friend ostream& operator<<(ostream& out, const self& )
{  return out;}

friend istream& operator>>(istream& in,  self& )
{  return in;}

};

/*{\Manpage {AdjIt} {} {Adjacency Iterators} {it} }*/
/*{\Mdefinition
a variable |it| of class  |\Mname| is
an adjacency iterator that marks a node
(which is fixed in contrast to linear node iterators)
and iterates over the edges that leave or enter this node.
At first, all outgoing edges will be traversed.

Internally, this iterator creates two instances of 
[[OutAdjIt]] and [[InAdjIt]]. The iteration is a
sequenced iteration over both iterators. Note that
this only fits for directed graph, for undirected graph
you should use [[OutAdjIt]] instead.
}*/
class AdjIt {
  /*{\Mcreation}*/
public:
  typedef leda::edge      edgetype;
  typedef leda::node      nodetype;
  typedef leda::graph     graphtype;
private:
  OutAdjIt oai;
  InAdjIt  iai;
public:
  int      flag; // 0: oai, 1:iai
public:
  typedef AdjIt self;

  AdjIt() {  }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    associated with no graph. }*/

  void init(const AdjIt& p) {
    oai.init(p.oai);
    iai.init(p.iai);
    flag=p.flag; }

AdjIt(const AdjIt& p) : oai(p.oai), iai(p.iai), flag(p.flag) { }

AdjIt(const leda::graph& G) : oai(G), iai(G), flag(0) { if (!oai.valid()) flag=1;}
/*{\Mcreate introduces a variable |\Mvar| of this class
  associated with |G|.
  The marked node is initialized
  by |n=G.first_node()| and the edge by |G.first_adj_edge(n)|.
  }*/

AdjIt(const leda::graph& G,leda::node  n) : oai(G,n), iai(G,n), flag(0) { if (!oai.valid()) flag=1;}
/*{\Mcreate introduces a variable |\Mvar| of this class marked with |n| and
  associated with |G|.
  The marked edge is initialized by
  by |G.first_adj_edge(n)|.

  Precondition: |n| is a node of |G|.}*/

AdjIt(const leda::graph& G,leda::node  n, leda::edge e) : oai(G,n,e), iai(G,n), flag(0) { if (!oai.valid()) flag=1;}
/*{\Mcreate introduces a variable |\Mvar| of this class marked with |n|
  and |e|
  and associated with |G|.

  Precondition: |n| is a node and |e| an edge of |G| and |source(e)=n|.}*/


/*{\Moperations }*/

void init(const graphtype& G) {
  oai.init(G); iai.init(G); flag=0; }
/*{\Mop associates |\Mvar| with |G| and marks it with
  |n'=G.first_node()| and |G.first_adj_edge(n')|. }*/

void init(const graphtype& G, const nodetype& n) {
  oai.init(G,n); iai.init(G,n); flag=0; if (!oai.valid()) flag=1;}
/*{\Mop associates |\Mvar| with |G| and marks it with
  |n| and |G.first_adj_edge(v)|.

  Precondition: |n| is a node of |G|. }*/


void init(const graphtype& G,
	  const nodetype& n,
	  const edgetype& e) {
  oai.init(G,n,e); iai.init(G,n,e); flag=0; if (!oai.valid()) flag=1;}
/*{\Mop associates |\Mvar| with |G| and marks it with
  |n| and |e|.

  Precondition: |n| is a node and |e| an edge of |G| and |source(e)=n|.}*/


void update(leda::edge  e) { if (flag==0) oai.update(e); else iai.update(e);
}
/*{\Mop |it| marks |e| afterwards. }*/

void reset(){ oai.reset(); iai.reset(); flag=0; }
/*{\Mop resets |\Mvar| to |G.first_adj_edge(n)| where |G|
  and |n| are the marked node and associated graph.
  }*/

void insert(leda::node v1, leda::node v2) { if (flag==0) oai.insert(v1,v2); else iai.insert(v1,v2); }
void insert(leda::node v) { if (flag==0) oai.insert(v); else iai.insert(v); }

void insert(const AdjIt& other) {
  if (flag==0) oai.insert(other.get_node()); else iai.insert(other.get_node());
}
/*{\Mop    creates a new edge from the marked node of |\Mvar|
  to the marked node of |other|. |\Mvar| is marked with the new edge afterwards. The marked node of |\Mvar| does not change. }*/

void del() {  if (flag==0) oai.del(); else iai.del(); }
/*{\Mop  deletes the marked leaving edge, i.e. |\Mvar.valid()| returns false afterwards.

  Precondition: |\Mvar.valid()| returns true. }*/

void reset_end() { if (flag==0) oai.reset_end(); else iai.reset_end();  }
/*{\Mop resets |\Mvar| to |G.last_adj_edge(n)| where |G|
  and |n| are the marked node and associated graph.
  }*/

void make_invalid() {  oai.make_invalid();  iai.reset(); flag=0; }
/*{\Mop makes |\Mvar| invalid, i.e. |\Mvar.valid()| will be false afterwards and
  |\Mvar| marks no node.}*/

void update(leda::node  n) { flag=0;
 oai.update(n);  iai.update(n);
}
/*{\Mop  |\Mvar| marks |n| and the first leaving edge of |n| afterwards.
  }*/


void update(leda::node  n,
	    leda::edge  e) { if (flag==0) { iai.update(n); oai.update(n,e); } else {
	      iai.update(n,e); oai.update(n,nil); } }
/*{\Mop  |it| marks |n| and |e| afterwards. }*/

AdjIt& operator= (const AdjIt& it2) { oai=it2.oai;
 iai=it2.iai; flag=it2.flag;
 return *this; }
/*{\Mbinop assigns |it2| to |\Mvar|. This method returns a reference to |\Mvar|.}*/

bool operator== (const AdjIt& it2) const  {
  if (flag!=it2.flag) return false;
  if (flag==0) return oai==it2.oai;
  return iai==it2.iai; }
/*{\Mbinop returns true if and only if |it| and |it2| are equal, i.e. if the marked nodes and edges are equal.}*/

bool has_node() const{ return oai.has_node(); }
/*{\Mop returns  true if and only if |\Mvar| marks a node.}*/

bool eol() const { return !valid(); }
/*{\Mop   returns |!\Mvar.valid()| which is
  true if and only if there is no successor edge left,
  i.e. if all edges of the edge set are passed (eol: end of list).
  }*/

bool valid() const { if (flag==0) { return oai.valid(); }
 else return iai.valid(); }
/*{\Mop
  returns true if and only if end of sequence not yet passed, i.e.
  if there is an edge in the  edge set that was not yet passed.
  }*/

leda::edge&  get_edge() 
{ if (flag==0) return oai.get_edge(); else return iai.get_edge(); }
  
leda::edge  get_edge() const { if (flag==0) return oai.get_edge(); else return iai.get_edge(); }  
/*{\Mop   returns the marked edge or nil if |\Mvar.valid()| returns false.
  }*/

leda::node& get_node() { return oai.get_node(); }


leda::node  get_node() const { return oai.get_node(); }
/*{\Mop   returns the marked node or nil if |\Mvar.has_node()| returns false.
  }*/

const leda::graph& get_graph() const { return oai.get_graph(); }
/*{\Mop         returns the associated graph. }*/

operator leda::edge() const{ return get_edge(); }
operator leda::edge() { return get_edge(); }

AdjIt curr_adj() const {  //===
  leda::graph* Gp= (leda::graph*)(&oai.get_graph());
  if (flag==0) {
    AdjIt intmp(*Gp,oai.curr_adj().get_node());
    if (!intmp.valid()) intmp.flag=1;
    return intmp;
  }
  AdjIt intmp(*Gp,iai.curr_adj().get_node());
  if (!intmp.valid())  intmp.flag=1;
  return intmp;
}
/*{\Mop
  If the currently
  associated edge leaves the marked node, this method returns a
  new adjacency iterator that is associated with |n'=target(e)| and |G.first_adj_edge(n')|
  where |G| is the associated graph.
  Otherwise it returns
  a new adjacency iterator that is
  associated with |n'=source(e)| and |G.first_in_edge(n')|
  where |G| is the associated graph.

  Precondition: |\Mvar.valid()| returns true.
  }*/

AdjIt& operator++ () {
  if (flag==0) {
    edgetype intmp(oai.get_edge());
    ++oai;
    if (!oai.valid()) {
      flag=1;
      iai.update(oai.get_node());
      if (iai.get_edge()==intmp) iai++;
    }
    return *this;
  }
  ++iai;
  flag=1;
  return *this;
}
/*{\Munop       performs one step forward in the list of incident edges of the marked node.
  If the formerly marked edge was a leaving edge and there
  is no successor edge, |\Mvar| is associated to |G.first_in_edge(n)|
  where |G| and |n| are the associated graph and node.
  If the formerly marked edge was an incoming edge and there
  is no successor edge,
  |\Mvar.eol()| will be true afterwards.
  This method returns a reference to |\Mvar|.

  Precondition: |\Mvar.valid()| returns true.
  }*/

AdjIt operator++ (int)   {
  AdjIt tmp(*this);
  if (flag==0) {
    edgetype intmp(oai.get_edge());
    ++oai;
    if (!oai.valid()) {
      flag=1;
      iai.update(oai.get_node());
      if (iai.get_edge()==intmp) iai++;
    }
    return tmp;
  }
  ++iai;
  flag=1;
  return tmp; }

AdjIt& operator-- () {
  if (flag==0) {
    --oai;
    return *this; }
  --iai;
  flag=0;
  return *this; }
/*{\Munop       performs one step backward in the list of incident edges of the marked node.
  If the formerly marked edge was an incoming edge and there
  is no predecessor edge, |\Mvar| is associated to |G.last_adj_edge(n)|
  where |G| and |n| are the associated graph and node.
  If the formerly marked edge was a leaving edge and there
  is no successor edge,
  |\Mvar.eol()| will be true afterwards.
  This method returns a reference to |\Mvar|.

  Precondition: |\Mvar.valid()| returns true.
  }*/

AdjIt operator-- (int)  {
  AdjIt tmp(*this);
  if (flag==0) {
    --oai;
    return tmp; }
  --iai;
  flag=0;
  return tmp; }

friend ostream& operator<<(ostream& out, const self& )
{  return out;}

friend istream& operator>>(istream& in,  self& )
{  return in;}

};
/*{\Mimplementation
Creation of an iterator and all methods take constant time.
}*/

template<class grapht>
class SafeAdjIt {
public:
  typedef leda::edge      edgetype;
  typedef leda::node      nodetype;
  typedef SafeGraph<grapht>    graphtype;
  typedef typename SafeGraph<grapht>::graphtype basetype;
  typedef SafeAdjIt<grapht> self;
private:
  SafeOutAdjIt<grapht> oai;
  SafeInAdjIt<grapht>  iai;
  int      flag; // 0: oai, 1:iai
public:

  SafeAdjIt() {  }

  void init(const SafeAdjIt<grapht>& p) {
    oai.init(p.oai); iai.init(p.iai); flag=p.flag; }

SafeAdjIt(const SafeAdjIt<grapht>& p) : oai(p.oai), iai(p.iai), flag(p.flag) { }

SafeAdjIt(SafeGraph<grapht>& G) : oai(G), iai(G), flag(0) { if (!oai.valid()) flag=1;}

SafeAdjIt(SafeGraph<grapht>& G,leda::node  n) : oai(G,n), iai(G,n), flag(0) { if (!oai.valid()) flag=1;}

SafeAdjIt(SafeGraph<grapht>& G,leda::node  n,leda::edge e) : oai(G,n,e), iai(G,n), flag(0) { if (!oai.valid()) flag=1;}

void init(const graphtype& G) {
  oai.init(G); iai.init(G); flag=0; if (!oai.valid()) flag=1;}

void init(const graphtype& G, const nodetype& n) {
  oai.init(G,n); iai.init(G,n); flag=0; if (!oai.valid()) flag=1;}


void init(const graphtype& G,
	  const nodetype& n,
	  const edgetype& e) {
  oai.init(G,n,e); iai.init(G,n,e); flag=0; if (!oai.valid()) flag=1;}


void update(leda::edge  e) { if (flag==0) oai.update(e); else iai.update(e); }

void reset() {  oai.reset(); iai.reset(); flag=0;  }

void insert(leda::node v1, leda::node v2) { if (flag==0) oai.insert(v1,v2); else iai.insert(v1,v2); }
void insert(leda::node v) { if (flag==0) oai.insert(v); else iai.insert(v); }

void insert(const SafeAdjIt<grapht>& other) {
  if (flag==0) oai.insert(other.get_node()); else iai.insert(other.get_node()); }

void del() {  if (flag==0) oai.del(); else iai.del(); }

void reset_end() { if (flag==0) oai.reset_end(); else iai.reset_end(); }

void make_invalid() {  oai.make_invalid();  iai.make_invalid(); flag=0; }

void update(leda::node  n) { flag=0;
 oai.update(n);  iai.update(n); if (!oai.valid()) flag=1;
}


void update(leda::node  n,
	    leda::edge  e) { if (flag==0) { iai.update(n); oai.update(n,e); } else {
	      iai.update(n,e); oai.update(n); } }

SafeAdjIt& operator= (const SafeAdjIt<grapht>& it2) { oai=it2.oai; iai=it2.iai; flag=it2.flag;
 return *this; }

bool operator== (const SafeAdjIt<grapht>& it2) const  {
  if (flag!=it2.flag) return false;
  if (flag==0) return oai==it2.oai;
  return iai==it2.iai; }

bool has_node() const{ return oai.has_node(); }

bool eol() const { if (flag==0) return oai.eol(); else return iai.eol(); }

bool valid() const { if (flag==0) return oai.valid(); else return iai.valid(); }

leda::edge&  get_edge() 
{ if (flag==0) return oai.get_edge(); else return iai.get_edge(); }

leda::edge  get_edge()  const
{ if (flag==0) return oai.get_edge(); else return iai.get_edge(); }



leda::node& get_node() { return oai.get_node(); }


leda::node  get_node() const { return oai.get_node(); }

const SafeGraph<grapht>& get_graph() const { return oai.get_graph(); }

operator leda::edge() const{ return get_edge(); }
operator leda::edge() { return get_edge(); }
   

SafeAdjIt curr_adj() const {
  SafeGraph<grapht>& Gp(*((SafeGraph<grapht>*)(&oai.get_graph())));
  if (flag==0) {
    SafeAdjIt<grapht> temp(Gp,oai.curr_adj().get_node());
    if (!temp.valid()) temp.flag=1;
    return temp;
  }
  SafeAdjIt<grapht> temp(Gp,iai.curr_adj().get_node());
  if (!temp.valid())  temp.flag=1;
  return temp;
}

SafeAdjIt& operator++ () {
  if (flag==0) {
    edgetype temp(oai.get_edge());
    ++oai;
    if (!oai.valid()) {
      flag=1;
      iai.update(oai.get_node());
      if (iai.get_edge()==temp) iai++;
    }
    return *this;
  }
  ++iai;
  flag=1;
  return *this;
}

SafeAdjIt operator++ (int)  {
  SafeAdjIt<grapht> tmp(*this);
  if (flag==0) {
    edgetype temp(oai.get_edge());
    ++oai;
    if (!oai.valid()) {
      flag=1;
      iai.update(oai.get_node());
      if (iai.get_edge()==temp) iai++;
    }
    return tmp;
  }
  ++iai;
  flag=1;
  return tmp; }

SafeAdjIt& operator-- () {
  if (flag==0) {
    --oai;
    return *this; }
  --iai;
  flag=0;
  return *this; }

SafeAdjIt operator-- (int)  {
  SafeAdjIt<grapht> tmp(*this);
  if (flag==0) {
    --oai;
    return tmp; }
  --iai;
  flag=0;
  return tmp; }

friend ostream& operator<<(ostream& out, const self& )
{  return out;}

friend istream& operator>>(istream& in,  self& )
{  return in;}

};


/*{\Manpage {FaceCirc} {} {Face Circulators} {fc} }*/
/*{\Mdefinition
  a variable |fc| of class  |\Mname| is
  a face circulator that circulates through the set of edges
of a face as long as the graph is embedded combinatorically correct,
i.e. the graph has to be bidirected and a map
(see \ref{graph}).
}*/
class FaceCirc {
  /*{\Mcreation}*/
public:
  typedef leda::edge      edgetype;
  typedef leda::node      nodetype;
  typedef leda::graph     graphtype;
private:
  edgetype                      _e;
  graphtype*                    _g;
public:
  typedef FaceCirc self;
  
  FaceCirc() { _g=nil; _e=nil; }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    associated with no graph. }*/
  
  void init(const FaceCirc& p) {
#if !defined(LEDA_CHECKING_OFF)
    if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
    _e=p._e; _g=p._g; }
  
  
FaceCirc(const FaceCirc& p) :
   _e(p._e), _g(p._g) {  }
  
FaceCirc(const leda::graph& G) : _g((graphtype*)&G) { 
  _e=nil; }
/*{\Mcreate introduces a variable |\Mvar| of this class
  associated with |G|.
 The edge is initialized to {\tt nil}.
  }*/
  
FaceCirc(const leda::graph& G,leda::edge  e) :  _e(e),  _g((graphtype*)&G)  { }
/*{\Mcreate introduces a variable |\Mvar| of this class marked with |e|
  and associated with |G|.

  Precondition: |e| is an edge of |G|.}*/
  
/*{\Moperations }*/
void init(const leda::graph& G) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
  }
/*{\Mop associates |\Mvar| with |G|. }*/

void init(const leda::graph& G, const leda::edge & e) { 
#if !defined(LEDA_CHECKING_OFF)
  if (_g!=nil) LEDA_EXCEPTION(501,"Iterator: leda::graph already initialized\n");
#endif
}
/*{\Mop associates |\Mvar| with |G| and marks it with |e|.
  
  Precondition: |e| is an edge of |G|.}*/

void update(leda::edge  e) { _e=e; }
/*{\Mop |fc| marks |e| afterwards. }*/

void make_invalid() {
  _e=nil; }
/*{\Mop makes |\Mvar| invalid, i.e. |\Mvar.valid()| will be false afterwards and
  |\Mvar| marks no edge.}*/

FaceCirc& operator= (const FaceCirc& fc2) {  _e=fc2._e;  _g=fc2._g;
 return *this; }
/*{\Mbinop assigns |fc2| to |\Mvar|. This method returns a reference to |\Mvar|.}*/

bool operator== (const FaceCirc& fc2) const  {
  return _e==fc2._e; }
/*{\Mbinop returns true if and only if |fc| and |fc2| are equal, i.e. if the marked edges are equal.}*/

bool has_edge() const{ return _e!=nil; }
/*{\Mop returns  true if and only if |\Mvar| marks an edge.}*/

bool eol() const { return _e==nil; }
/*{\Mop   returns |!\Mvar.valid()|.
  }*/

bool valid() const { return _e!=nil; }
/*{\Mop
  returns true if and only if the circulator is marked with an edge.
  }*/

leda::edge&  get_edge() { return _e; }
    
leda::edge  get_edge() const { return _e; }
/*{\Mop         returns the marked edge or nil if |\Mvar.valid()| returns false.
  }*/


const leda::graph& get_graph() const {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  return *_g; }
/*{\Mop         returns the associated graph. }*/


operator leda::edge() const{ return _e; }
operator leda::edge() { return _e; }
   
FaceCirc& operator++ () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=_g->face_cycle_succ(_e);
  return *this; }
/*{\Munop  redirects the circulator to the cyclic
adjacency predecessor of |reversal(e)|, where |e| is the marked edge.
  This method returns a reference to |\Mvar|.

Precondition: |\Mvar.valid()| returns true.
}*/

FaceCirc operator++ (int) {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  FaceCirc tmp(*this);
  _e=_g->face_cycle_succ(_e);
  return tmp; }

FaceCirc& operator-- () {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  _e=_g->face_cycle_pred(_e);
  return *this; }
/*{\Munop redirects the circulator to the cyclic                               
adjacency successor of |e|, where |e| is the marked edge.
  This method returns a reference to |\Mvar|.         

Precondition: |\Mvar.valid()| returns true.
}*/

FaceCirc operator-- (int)  {
#if !defined(LEDA_CHECKING_OFF)
  if (_g==nil) LEDA_EXCEPTION(502,"Iterator: leda::graph not initialized\n");
#endif
  FaceCirc tmp(*this);
  _e=_g->face_cycle_pred(_e);
  return tmp; }

friend ostream& operator<<(ostream& out, const self&)
{  return out;}

friend istream& operator>>(istream& in,  self&)
{  return in;}

};
/*{\Mimplementation
Creation of a circulator and all methods take constant time.
}*/


/*******************************************************************************

 (II) Additional Classes
 
     * Decorator (Wrapper) - Pattern: 
       => FilterNodeIt, FilterEdgeIt, FilterAdjIt
     * Strategy-Pattern: 
       => several Predicate Classes
     * Adapter-Pattern:
       => CompPred adapts Predicates and DataAccessors
       => STLWrapper adapts Iterators and DataAccessors
      
 Note: 
 The c++-comments contain manual pages in the leda manual style format.

*******************************************************************************/

/*{\Manpage {FilterNodeIt} {Predicate,Iter} {Filter Node Iterator} {it} }*/
/*{\Mdefinition 
An instance |it| of class  |\Mname|
encapsulates
        an object of type [[Iter]] and creates a restricted view on the
        set of nodes over which this internal iterator iterates.  More
        specifically, all nodes that do not fulfill the predicate
        defined by [[Predicate]] are filtered out during this traversal.

  Class [[FilterEdgeIt]] and [[FilterAdjIt]] are defined analogously, i.e.
  can be used for edge iterators or adjacency iterators, respectively.

Precondition: The template parameter [[Iter]] must be a
node iterator, e.g. [[NodeIt]] or [[FilterNodeIt<pred,NodeIt>]].
[[Predicate]] must be 
a class which provides a
        method |operator()| according to the following signature:
           [[bool operator() (Iter)]].
}*/
template<class Predicate, class Iter>
class FilterNodeIt : public Iter {
  USE_ITER_MEMBERS
  const Predicate*     _pred;
  typedef FilterNodeIt<Predicate,Iter> self;
public:
  typedef  typename Iter::nodetype nodetype;
  /*{\Mcreation }*/
  
  FilterNodeIt() {  }
  /*{\Mcreate introduces a variable |\Mvar| of this class, not bound
    to a predicate or iterator.}*/
  
  FilterNodeIt(const Predicate& pred, const Iter& base_it) :
    Iter(base_it), _pred((Predicate*)(&pred)) { control(); }
/*{\Mcreate introduces a variable |\Mvar| of this class
  bound to |pred| and |base_it|.
  }*/
  
/*{\Moperations }*/
  
void init(const Predicate& pred, const Iter& base_it) {
  Iter::init(base_it);  _pred=(Predicate*)(&pred); control(); }
/*{\Mop initializes |\Mvar|, which is bound to |pred| and |base_it| afterwards.
    
  Precondition: |\Mvar| is not yet bound to a predicate or iterator. }*/
  
void update(nodetype  n) {
  Iter::update(n); control(); }
void reset() { Iter::reset(); control(); }
void reset_end() { Iter::reset_end(); control(); }
void control() {
  while (valid() && !(*_pred)((Iter)(*this)))
    Iter::operator++(); }
FilterNodeIt(const self& it) : Iter(it), _pred(it._pred) { control(); }
self& operator= (const self& it2) {
  _pred=it2._pred; Iter::operator=(it2); return *this; }
self& operator= (const Iter& it2) {
  Iter::operator=(it2); control(); return *this; }
self& operator++() {
  Iter::operator++();
  control();
  return *this; }
self operator++(int) {
  self tmp=*this;
  Iter::operator++();
  control();
  return tmp; }
};
/*{\Mimplementation Constant overhead. }*/

/*{\Mexample
  Suppose each node has an own colour and we only want
  to see those with a specific colour, for example red (we use
  the LEDA colours).
  At first the data structures:
  \begin{verbatim}
  GRAPH<color,double> G;
  NodeIt it(G);
  \end{verbatim}
  
  We would have to write something like this:
  \begin{verbatim}
  while(it.valid()) {
    if (G[it.get_node()]==red) do_something(it);
    ++it; 
  }
  \end{verbatim}
  
  With the filter wrapper class we can add the test
  if the node is red to the behaviour of the iterator.
  
  \begin{verbatim}
  struct RedPred {
    bool operator() (const NodeIt& it) const {
    return G[it.get_node()]==red; }
  } redpred;
  FilterNodeIt<RedPred,NodeIt> red_it(redpred,it);
  \end{verbatim}
  
  This simplifies the loop to the following:
  \begin{verbatim}
  while(red_it.valid()) {
    do_something(red_it);
    ++red_it; }
  \end{verbatim}
  
  All ingredients of the comparison are hard-wired
  in struct [[RedPred]]: the type of the compared values ([[color]]), the
  comparison value (red) and the binary comparison (equality).
  The following class [[CompPred]] renders these three choices
  flexible.
}*/
/*{\Manpage {CompPred} {Iter,DA,Comp} {Comparison Predicate} {cp} }*/
/*{\Mdefinition An instance |\Mvar| of class  |\Mname| is
  a predicate comparator that produces boolean values with the
  given compare function and the attribute associated with an iterator.
}*/
template<class Iter, class DA, class Comp>
class CompPred {
  const DA&             _da;
  Comp                  _comp;
  typename DA::value_type        _val;
public:
  /*{\Mcreation }*/
  
  CompPred(const DA& da, const Comp& comp,  typename DA::value_type val) :
    _da(da), _comp(comp), _val(val) { }
  /*{\Mcreate introduces a variable |\Mvar| of this class and associates
    it to the given data accessor |da|, compare function |comp| and value |val|.}*/
  /*{\Mtext
    Precondition: [[Comp]] is a pointer-to-function type which takes
    two values of type |typename DA::value_type|
  and produces a boolean return value. [[Comp]] might also be
  a class with member function |bool operator()(typename DA::value_type,typename DA::value_type)|.
  }*/
  CompPred<Iter,DA,Comp>& operator=(const CompPred<Iter,DA,Comp>&P) {
    _comp=P._comp; _val=P._val; return *this;}
bool operator() (const Iter& it) {
  return _comp(get(_da,it),_val); }
};

/*{\Mexample
  In the following example, a node iterator for red nodes will be
  created. At first the basic part (see sect.~\ref{node_array_da} for explanation
  of the data accessor {\tt node\_array\_da}):
  \begin{verbatim}
  graph G;
  NodeIt it(G);
  node_array<color> na_colour(G,black);
  node_array_da<color> da_colour(na_colour);
  assign_some_color_to_each_node();
  \end{verbatim}
  
  Now follows the definition of a ``red iterator'' ([[Equal<T>]] yields
  true, if the given two values are equal):
  
  \begin{verbatim}
  template<class T>
  class Equal {
    public:
    bool operator() (T t1, T t2) const {
    return t1==t2; }
  };
  
  typedef CompPred<NodeIt,node_array_da<color>,Equal<color> > Predicate;
  Predicate PredColour(da_colour,Equal<color>(),red);
  FilterNodeIt<Predicate,NodeIt> red_it(PredColour,it);
  \end{verbatim}
  
  This simplifies the loop to the following:
  \begin{verbatim}
  while(red_it.valid()) {
    do_something(red_it);
  ++red_it; }
  \end{verbatim}
  
  [[Equal<T>]]  is a class that compares two items of the
  template parameter [[T]] by means of a method
  [[bool operator()(T,T);]]. There are some classes available for this purpose:
  [[Equal<T>]], [[Unequal<T>]], [[LessThan<T>]], [[LessEqual<T>]],
  [[GreaterThan<T>]] and [[GreaterEqual<T>]] with obvious semantics,
  where [[T]] is the type of the values. Predicates of the STL can be used
  as well since they have the same interface.
  
}*/
/*{\Manpage {ObserverNodeIt} {Obs,Iter} {Observer Node Iterator} {it} }*/
/*{\Mdefinition An instance |it| of class  |\Mname| is
  an observer  iterator. Any method call of iterators will be
  ``observed'' by an internal object of class |Obs|.
  
  Class [[ObserverEdgeIt]] and [[ObserverAdjIt]] are defined analogously, i.e.
  can be used for edge iterators or adjacency iterators, respectively.

  Precondition: The template parameter [[Iter]] must be a
  node iterator.
}*/
template<class Obs, class Iter>
class ObserverNodeIt : public Iter {
  USE_ITER_MEMBERS
  const Obs*           _obs;
  typedef ObserverNodeIt<Obs,Iter> self;
public:
  typedef  typename Iter::nodetype nodetype;
  /*{\Mcreation}*/
  
  ObserverNodeIt() {  }
  /*{\Mcreate introduces a variable |\Mvar| of this class, not bound
    to an observer or iterator.}*/
  
  ObserverNodeIt(Obs& obs, const Iter& base_it) :
    Iter(base_it), _obs(&obs) {
    observe_constructor(); }
/*{\Mcreate introduces a variable |\Mvar| of this class
  bound to the observer |obs| and |base_it|.
    
  Precondition: [[Obs]] must have methods [[observe_constructor()]],
  [[observe_forward()]], [[observe_update()]].
These three methods may have arbitrary
return types (incl. void).  }*/
/*{\Moperations}*/
  
void init(const Obs& obs, const Iter& base_it) {
  Iter::init(base_it); _obs=(Obs*)&obs; observe_constructor(); }
/*{\Mop initializes |\Mvar|, which is bound to |obs| and |base_it| afterwards.
    
  Precondition: |\Mvar| is not bound to an observer or iterator. }*/
  
void observe_constructor() {
  _obs->notify_constructor((Iter)(*this)); }
void observe_forward() {
  _obs->notify_forward((Iter)(*this)); }
void observe_update() {
  _obs->notify_update((Iter)(*this)); }
void update(nodetype  n) {
  Iter::update(n); observe_update(); }
ObserverNodeIt(const ObserverNodeIt& it) : Iter(it) , _obs(it._obs) {
  observe_constructor(); }
bool operator== (const ObserverNodeIt& it2) const {
  return (Iter::operator==(it2) && (*_obs==(*it2._obs))); }
self& operator=(const Iter& it2) {
  Iter::operator=(it2);
  observe_constructor(); return *this; }
self& operator=(const self& it2) {
  _obs=it2._obs; Iter::operator=(it2);
  observe_constructor(); return *this;}
self& operator++() {
  Iter::operator++();
  observe_forward();
  return *this; }
self operator++(int) {
  self tmp=*this;
  Iter::operator++();
  observe_forward();
  return tmp; }

Obs& get_observer() const { return *_obs; }
/*{\Mop returns a reference to the observer
  to which it is bound.
  }*/
  
/*{\Mexample
  First two simple observer classes.  The first one is a dummy class,
  which ignores all notifications.  The second one merely counts
the number
of calls to [[operator++]] for all iterators that
share the same observer
object through copy construction or assignment
(of course, a real implementation should apply some kind
of reference counting or other garbage collection).
    
In this example, the counter variable |_count| of class [[SimpleCountObserver]]
will be initialized with the counter variable |_count| of
class [[DummyObserver]], i.e. the variable is created only once.
    
\begin{verbatim}
template <class Iter>
class DummyObserver {
  int* _count;
public:
  DummyObserver() : _count(new int(0)) { }
  void notify_constructor(const Iter& ) { }
  void notify_forward(const Iter& ) { }
  void notify_update(const Iter& ) { }
  int  counter() const { return *_count; }
  int* counter_ptr() const { return _count; }
  bool operator==(const DummyObserver& D) const {
    return _count==D._count; }
};
    
template <class Iter, class Observer>
class SimpleCountObserver {
  int*  _count;
public:
  SimpleCountObserver() : _count(new int(0)) { }
  SimpleCountObserver(Observer& obs) :
    _count(obs.counter_ptr()) { }
  void notify_constructor(const Iter& ) { }
  void notify_forward(const Iter& ) { ++(*_count); }
  void notify_update(const Iter& ) { }
  int  counter() const {  return *_count; }
  int* counter_ptr() const { return _count; }
  bool operator==(const SimpleCountObserver& S) const {
    return _count==S._count; }
};
\end{verbatim}
    
Next an exemplary application, which counts the number
of calls to [[operator++]]
of all adjacency iterator objects inside [[dummy_algorithm]].
Here the dummy observer class is used only as a ``Trojan horse,''
which carries
the pointer to the counter without affecting the code of the algorithm.
    
\begin{verbatim}
template<class Iter>
bool break_condition (const Iter&) { ... }
    
template<class ONodeIt, class OAdjIt>
void  dummy_algorithm(ONodeIt& it, OAdjIt& it2) {
  while (it.valid()) {
    for (it2.update(it); it2.valid() && !break_condition(it2); ++it2)
    ++it;
  }
}
    
int write_count(graph& G) {
  typedef DummyObserver<NodeIt>                  DummyObs;
  typedef SimpleCountObserver<AdjIt,DummyObs>    CountObs;
  typedef ObserverNodeIt<DummyObs,NodeIt>        ONodeIt;
  typedef ObserverAdjIt<CountObs,AdjIt>          OAdjIt;
      
  DummyObs observer;
  ONodeIt   it(observer,NodeIt(G));
  CountObs  observer2(observer);
  OAdjIt    it2(observer2,AdjIt(G));
  dummy_algorithm(it,it2);
  return it2.get_observer().counter();
}
\end{verbatim}
}*/
};
/*{\Manpage {STLNodeIt} {DataAccessor,Iter} {STL Iterator Wrapper} {it} }*/
/*{\Mdefinition An instance |it| of class  |\Mname| is
  a STL iterator wrapper for node iterators (e.g. [[NodeIt]],
  [[FilterNodeIt<pred,NodeIt>]]). \label{stlwrap}
  It adds all type tags and methods that are necessary
  for STL conformance; see the standard draft working paper
  for details.  The type tag [[value_type]] is equal to
  [[typename DataAccessor::value_type]] and the return value of [[operator*]].
  
  Class [[STLEdgeIt]] and [[STLAdjIt]] are defined analogously, i.e.
  can be used for edge iterators or adjacency iterators, respectively.

  {\bf Precondition:} The template parameter [[Iter]] must be a
  node iterator. |DataAccessor| must be a data accessor.

  {\bf Note:} There are specialized versions of STL wrapper iterator classes
  for each kind of iterator that return different LEDA graph objects.
 
  \begin{tabular}{|l|l|}
   \hline
   {\bf class name}        &   {\tt operator*()} returns \\
   \hline \hline
   {\tt NodeIt\_n}          &   {\tt node} \\ 
   {\tt EdgeIt\_e}          &   {\tt edge} \\
   {\tt AdjIt\_n}           &   {\tt node} \\
   {\tt AdjIt\_e}           &   {\tt edge} \\
   {\tt OutAdjIt\_n}        &   {\tt node} \\
   {\tt OutAdjIt\_e}        &   {\tt edge} \\
   {\tt InAdjIt\_n}         &   {\tt node} \\
   {\tt InAdjIt\_e}         &   {\tt edge} \\
   \hline
  \end{tabular}
}*/

template<class DataAccessor, class Iter>
class STLNodeIt : public Iter
{
  USE_ITER_MEMBERS
  DataAccessor     _da;
  typedef STLNodeIt<DataAccessor,Iter> self;
public:
  typedef STLNodeIt<DataAccessor,Iter> iterator;
  typedef  typename Iter::nodetype nodetype;
  /*{\Mcreation}*/
  typedef  typename DataAccessor::value_type value_type;
  
  STLNodeIt(const leda::graph& G) : Iter(G),_da() { }
  STLNodeIt(const leda::graph& G, leda::node n) : Iter(G,n), _da() { }

  STLNodeIt(DataAccessor da, const Iter& base_it) :
    Iter(base_it), _da(da) {  }
  /*{\Mcreate
    introduces a variable |\Mvar| of this class
    bound to |da| and |base_it|. }*/
  STLNodeIt(const STLNodeIt& it) : Iter(it),  _da(it._da) { }
  STLNodeIt& operator= (const STLNodeIt& it2) {
    _da=it2._da; Iter::operator=(it2); return *this; }
  STLNodeIt& operator= (const Iter& it2) {
    Iter::operator=(it2); return *this; }
  /*{\Moperations}*/
  
  STLNodeIt<DataAccessor,Iter>& operator= ( typename DataAccessor::value_type i) {
    set(_da,(Iter&)*this,i); }
  /*{\Mbinop assigns the value |i|, i.e. |set(DA,it,i)| will be invoked where
    |DA| is the associated data accessor and |it| the associated iterator.
    }*/
  
  bool operator==(const STLNodeIt<DataAccessor,Iter>& it2) const {
    if (valid()) {
      if (!it2.valid()) return false;
      return operator*()==(*it2); }
    if (!it2.valid()) return true;
    return false; }
  /*{\Mbinop
    returns [[true]] if the associated values of |it| and |it2| are equal,
    i.e. |get(DA,cit)==get(DA,cit2)| is true where |cit| is the associated
    iterator of |it| and |cit2| is the associated iterator of |it2| and
    |DA| is the associated data accessor.
    }*/
  
  bool operator!=(const STLNodeIt<DataAccessor,Iter>& it2) {
    return !operator==(it2); }
  /*{\Mbinop returns [[false]] if the associated value equals the one of the given
    iterator.}*/
  
  self& operator++() {
    Iter::operator++();
    return *this; }
  self operator++(int) {
    self tmp=*this;
    Iter::operator++();
    return tmp; }
  self& operator--() {
    Iter::operator--();
    return *this; }
  self operator--(int) {
    self tmp=*this;
    Iter::operator--();
    return tmp; }
  
  STLNodeIt<DataAccessor,Iter>& begin() {
    reset();
    return *this; }
  /*{\Mop resets the iterator to the beginning of the sequence.}*/
  
  STLNodeIt<DataAccessor,Iter>& last() {
    reset_end(); return *this; }
  /*{\Mop resets the iterator to the ending of the sequence.}*/
  
  STLNodeIt<DataAccessor,Iter>& end() {
    make_invalid();  return *this; }
  /*{\Mop makes the iterators invalid, i.e. past-the-end-value.}*/
  
  typename DataAccessor::value_type dump_val;
  
  typename DataAccessor::value_type& operator*()  {
    if (!valid()) return dump_val;
    return access(_da,(Iter&)*this); }
  /*{\Munop returns a reference to the associated value, which originally
    comes from data accessor |da|. If the associated iterator |it| is not
    valid, a dummy value reference is returned and should not be used.
    
    Precondition: |access(DA,it)| returns a non constant reference to the
    data associated to |it| in |DA|. This functions is defined for all implemented data
    accessors (e.g. [[node_array_da]], [[edge_array_da]]).
    }*/
  
  typename DataAccessor::value_type operator*() const {
    if (!valid()) return  typename DataAccessor::value_type();
    return get(_da,(Iter)*this); }
};



template<class DataAccessor, class Iter>
istream& operator>>(istream& st,STLNodeIt<DataAccessor,Iter>& A  ) {
  st >> *A; return st; }
template<class DataAccessor, class Iter>
ostream& operator<<(ostream& st, STLNodeIt<DataAccessor,Iter> A  ) {
  if (!A.valid()) st << "(nil)";
  st << *A; return st; }
template<class Iter>
class GIT_ConstantIt {
  typedef GIT_ConstantIt<Iter> self;
  Iter current;
public:
  typedef  typename Iter::value_type value_type;
  typedef Iter reverse_iterator;
  GIT_ConstantIt(const Iter& _it) : current(_it) { }
  GIT_ConstantIt(const self& _it) : current(_it.current) { }
  operator Iter() { return current; }
  value_type  operator*() const { return *current; }
  self operator++() {
    current++;
    return *this; }
  self operator++(int) {
    self temp(*this);
    current++;
    return temp; }
  self operator--() {
    current--;
    return *this; }
  self operator--(int) {
    self temp(*this);
    current--;
    return temp; }
  bool operator==(const self& it) const { return (current==it.current); }
  self begin() { current.last(); return *this; }
  self end() { current.end(); return *this; }
  self last() { current.begin(); return *this; }
  friend ostream& operator<<(ostream& out, const self& )
    {  return out;}
  
  friend istream& operator>>(istream& in,  self& )
    {  return in;}
  
};
template<class Iter>
class GIT_ReverseIt {
  typedef GIT_ReverseIt<Iter> self;
  Iter current;
public:
  typedef  typename Iter::value_type value_type;
  typedef Iter reverse_iterator;
  GIT_ReverseIt(const Iter& _it) : current(_it) { }
  GIT_ReverseIt(const self& _it) : current(_it.current) { }
  operator Iter() { return current; }
  value_type  operator*() const { return *current; }
  value_type& operator*() { return *current; }
  self operator++() {
    current--;
    return *this; }
  self operator++(int) {
    self temp(*this);
    current--;
    return temp; }
  self operator--() {
    current++;
    return *this; }
  self operator--(int) {
    self temp(*this);
    current++;
    return temp; }
  bool operator==(const self& it) const { return (current==it.current); }
  self begin() { current.last(); return *this; }
  self end() { current.end(); return *this; }
  self last() { current.begin(); return *this; }
  friend ostream& operator<<(ostream& out, const self& it)
    {  return out;}
  
  friend istream& operator>>(istream& in,  self& )
    {  return in;}
  
};

template<class Predicate, class Iter>
class FilterEdgeIt : public Iter {
  USE_ITER_MEMBERS
  const Predicate*     _pred;
  typedef FilterEdgeIt<Predicate,Iter> self;
public:
  typedef  typename Iter::edgetype edgetype;
  void update(edgetype  e) {
    Iter::update(e); control(); }
  void reset() {
    Iter::reset();
    control(); }
  void reset_end() {
    Iter::reset_end();
    control(); }
  void control() {
    while (valid() && !(*_pred)((Iter)(*this)))
      Iter::operator++();  }
  FilterEdgeIt() {  }
  void init(const Predicate& pred, const Iter& base_it) {
    Iter::init(base_it); _pred=&pred; control(); }
  FilterEdgeIt(const Predicate& pred, const Iter& base_it) :
    Iter(base_it), _pred(&pred) { control(); }
  FilterEdgeIt(const self& it) : Iter(it), _pred(it._pred) { control(); }
  self& operator= (const self& it2) {
    _pred=it2._pred; Iter::operator=(it2); return *this; }
  self& operator= (const Iter& it2) {
    Iter::operator=(it2); control(); return *this; }
  self& operator++() {
    Iter::operator++();
    control();
    return *this; }
  self operator++(int) {
    self tmp=*this;
    Iter::operator++();
    control();
    return tmp; }
};
template<class Obs, class Iter>
class ObserverEdgeIt : public Iter {
  USE_ITER_MEMBERS
  const Obs*           _obs;
  typedef ObserverEdgeIt<Obs,Iter> self;
public:
  typedef  typename Iter::edgetype edgetype;
  void update(edgetype  n) {
    Iter::update(n); observe_update(); }
  void observe_constructor() {
    _obs->notify_constructor((Iter)(*this)); }
  void observe_forward() {
    _obs->notify_forward((Iter)(*this)); }
  void observe_update() {
    _obs->notify_update((Iter)(*this)); }
  ObserverEdgeIt() {  }
  void init(const Obs& obs, const Iter& base_it) {
    Iter::init(base_it); _obs=(Obs*)&obs; observe_constructor(); }
  ObserverEdgeIt(Obs& obs, const Iter& base_it) :
    Iter(base_it), _obs(&obs) {
    observe_constructor(); }
  ObserverEdgeIt(const ObserverEdgeIt& it) :  Iter(it), _obs(it._obs) {
    observe_constructor(); }
  bool operator== (const ObserverEdgeIt& it2) const {
    return (Iter::operator==(it2) && (*_obs==*(it2._obs))); }
  self& operator=(const Iter& it2) {
    Iter::operator=(it2);
    observe_constructor(); return *this; }
  self& operator=(const self& it2) {
    _obs=it2._obs; Iter::operator=(it2);
    observe_constructor(); return *this;}
  self& operator++() {
    Iter::operator++();
    observe_forward();
    return *this; }
  self operator++(int) {
    self tmp=*this;
    Iter::operator++();
    observe_forward();
    return tmp; }
  Obs& get_observer() const { return *_obs; }
};
template<class DataAccessor, class Iter>
class STLEdgeIt : public Iter
{
  USE_ITER_MEMBERS
  DataAccessor     _da;
  typedef STLEdgeIt<DataAccessor,Iter> self;
public:
  typedef  typename DataAccessor::value_type value_type;
  typedef  typename Iter::edgetype edgetype;
  STLEdgeIt(const leda::graph& G) : Iter(G),_da() { }
  STLEdgeIt(const leda::graph& G, leda::edge e) : Iter(G,e),_da() { }
  STLEdgeIt(DataAccessor da, const Iter& base_it) :
    Iter(base_it), _da(da) {  }
  STLEdgeIt(const STLEdgeIt& it) : Iter(it), _da(it._da) { }
  STLEdgeIt& operator= (const STLEdgeIt& it2) {
    _da=it2._da; Iter::operator=(it2); return *this; }
  STLEdgeIt& operator= (const Iter& it2) {
    Iter::operator=(it2); return *this; }
  self& operator= ( typename DataAccessor::value_type i) {
    set(_da,(Iter&)*this,i); }
  bool operator==(const self& k) const {
    if (valid()) {
      if (!k.valid()) return false;
      return operator*()==(*k); }
    if (!k.valid()) return true;
    return false; }
  bool operator!=(const self& k) {
    return !operator==(k); }
  self& operator++() {
    Iter::operator++();
    return *this; }
  self operator++(int) {
    self tmp=*this;
    Iter::operator++();
    return tmp; }
  self& begin() {
    reset();
    return *this; }
  self& last() { reset_end(); return *this; }
  self& end() {
    make_invalid();  return *this; }
  typename DataAccessor::value_type dump_val;
  typename DataAccessor::value_type& operator*()  {
    if (!valid()) return dump_val;
    return access(_da,(Iter&)*this); }
  typename DataAccessor::value_type operator*() const {
    if (!valid()) return  typename DataAccessor::value_type();
    return get(_da,(Iter)*this); }
};

template<class DataAccessor, class Iter>
istream& operator>>(istream& st,STLEdgeIt<DataAccessor,Iter>& A  ) {
  st >> *A; return st; }
template<class DataAccessor, class Iter>
ostream& operator<<(ostream& st, STLEdgeIt<DataAccessor,Iter> A  ) {
  if (!A.valid()) st << "(nil)";
  st << *A; return st;  }

template<class Predicate, class Iter>
class FilterAdjIt : public Iter {
  USE_ITER_MEMBERS
  const Predicate*     _pred;
  typedef FilterAdjIt<Predicate,Iter> self;
public:
  typedef  typename Iter::edgetype edgetype;
  typedef  typename Iter::nodetype nodetype;
  void update(nodetype  n) {
    Iter::update(n); control(); }
  void update(edgetype  e) {
    Iter::update(e); control(); }
  void update(nodetype  n, edgetype  e) {
    Iter::update(n,e); control(); }
  void reset() {
    Iter::reset();
    control(); }
  void reset_end() { Iter::reset_end(); control(); }
  void control() {
    while (valid() && !(*_pred)((Iter)(*this)))
      Iter::operator++();  }
  FilterAdjIt() { }
  void init(const Predicate& pred, const Iter& base_it) {
    Iter::init(base_it); _pred=(Predicate*)(&pred); control(); }
  FilterAdjIt(const Predicate& pred, const Iter& base_it) :
    Iter(base_it), _pred((Predicate*)(&pred)) { control(); }
  FilterAdjIt(const self& it) : Iter(it),  _pred(it._pred) { control(); }
  self& operator= (const self& it2) {
    _pred=it2._pred; Iter::operator=(it2); return *this; }
  self& operator= (const Iter& it2) {
    Iter::operator=(it2); control(); return *this; }
  self& operator++() {
    Iter::operator++();
    control();
    return *this; }
  self operator++(int) {
    self tmp=*this;
    Iter::operator++();
    control();
    return tmp; }
  self curr_adj() const {
    self temp(*this);
    temp=self(*_pred,temp.curr_adj());
    return temp; }
};

template<class Obs, class Iter>
class ObserverAdjIt : public Iter {
  USE_ITER_MEMBERS
  const Obs*           _obs;
  typedef ObserverAdjIt<Obs,Iter> self;
public:
  typedef  typename Iter::edgetype edgetype;
  typedef  typename Iter::nodetype nodetype;
  void update(nodetype  n) {
    Iter::update(n); observe_update(); }
  void update(edgetype  e) {
    Iter::update(e);  observe_update(); }
  void update(nodetype  n, edgetype  e) {
    Iter::update(n,e);  observe_update(); }
  void observe_constructor() {
    _obs->notify_constructor((Iter)(*this)); }
  void observe_forward() {
    _obs->notify_forward((Iter)(*this)); }
  void observe_update() {
    _obs->notify_update((Iter)(*this)); }
  ObserverAdjIt() {  }
  void init(const  Obs& obs, const Iter& base_it) {
    Iter::init(base_it); _obs=(Obs*)&obs; observe_constructor(); }
  ObserverAdjIt(Obs& obs, const Iter& base_it) :
    Iter(base_it), _obs(&obs) {  observe_constructor(); }
  ObserverAdjIt(const ObserverAdjIt& it) : Iter(it), _obs(it._obs) {
    observe_constructor(); }
  bool operator== (const ObserverAdjIt& it2) const {
    return (Iter::operator==(it2) && (*_obs==*(it2._obs))); }
  self& operator=(const Iter& it2) {
    Iter::operator=(it2);
    observe_constructor(); return *this; }
  self& operator=(const self& it2) {
    _obs=it2._obs; Iter::operator=(it2);
    observe_constructor(); return *this;}
  self& operator++() {
    Iter::operator++();
    observe_forward();
    return *this; }
  self operator++(int) {
    self tmp=*this;
    Iter::operator++();
    observe_forward();
    return tmp; }
  Obs& get_observer() { return *_obs; }
  self curr_adj() const {
    self temp(*this);
    temp=self(*_obs,temp.curr_adj());
    return temp; }
};

template<class DataAccessor, class Iter>
class STLAdjIt : public Iter
{
  USE_ITER_MEMBERS
  DataAccessor     _da;
  typedef STLAdjIt<DataAccessor,Iter> self;
public:
  typedef  typename Iter::edgetype edgetype;
  typedef  typename Iter::nodetype nodetype;
  typedef  typename DataAccessor::value_type value_type;
  STLAdjIt(DataAccessor da, const Iter& base_it) :
    Iter(base_it), _da(da) {  }
  STLAdjIt(const leda::graph& G) : Iter(G), _da() { }
  STLAdjIt(const leda::graph& G,leda::node n) : Iter(G,n), _da() { }
  STLAdjIt(const leda::graph& G,leda::node n, leda::edge e) : Iter(G,n,e), _da() { }
  STLAdjIt(const STLAdjIt& it) :  Iter(it), _da(it._da) { }
  STLAdjIt& operator= (const STLAdjIt& it2) {
    _da=it2._da; Iter::operator=(it2); return *this; }
  STLAdjIt& operator= (const Iter& it2) {
    Iter::operator=(it2); return *this; }
  self& operator= ( typename DataAccessor::value_type i) {
    set(_da,(Iter&)*this,i); }
  bool operator==(const self& k) const {
    if (valid()) {
      if (!k.valid()) return false;
      return operator*()==(*k); }
    if (!k.valid()) return true;
    return false; }
  bool operator!=(const self& k) {
    return !operator==(k); }
  self& operator++() {
    Iter::operator++();
    return *this; }
  self operator++(int) {
    self tmp=*this;
    Iter::operator++();
    return tmp; }
  self& begin() {
    reset();
    return *this; }
  self& last() {
    reset_end(); return *this; }
  self& end() {
    make_invalid();  return *this; }
  typename DataAccessor::value_type dump_val;
  typename DataAccessor::value_type& operator*()  {
    if (!valid()) return dump_val;
    return access(_da,(Iter&)*this); }
  typename DataAccessor::value_type operator*() const {
    if (!valid()) return  typename DataAccessor::value_type();
    return get(_da,(Iter)*this); }
  self curr_adj() const {
    self temp(*this);
    temp=self(_da,temp.curr_adj());
    return temp; }
};

template<class DataAccessor, class Iter> 
istream& operator>>(istream& st,STLAdjIt<DataAccessor,Iter>& A ) { 
  st >> *A; return st; }
template<class DataAccessor, class Iter> 
ostream& operator<<(ostream& st, STLAdjIt<DataAccessor,Iter> A ) { 
  if (!A.valid()) st << "(nil)"; 
  st << *A; return st; }

#include <LEDA/graph/graph.h>


template<class nodet, class edget, class grapht, class Out_AdjIt,class In_AdjIt>
class _AdjIt {
public:
  typedef edget      edgetype;
  typedef nodet      nodetype;
  typedef grapht     graphtype;
private:
  Out_AdjIt oai;
  In_AdjIt  iai;
  int      flag; // 0: oai, 1:iai
public:
  typedef _AdjIt<nodet,edget,grapht,Out_AdjIt,In_AdjIt> self;
  _AdjIt() {  }
  
  void init(const typename Out_AdjIt::graphtype& G) {
    oai.init(G); iai.init(G); flag=0; if (!oai.valid()) flag=1; }
  void init(const typename Out_AdjIt::graphtype& G, const typename Out_AdjIt::nodetype& v) {
    oai.init(G,v); iai.init(G,v); flag=0; if (!oai.valid()) flag=1;  }
  void init(const typename Out_AdjIt::graphtype& G,
	    const typename Out_AdjIt::nodetype& v,
	    const typename Out_AdjIt::edgetype& e) {
    oai.init(G,v,e); iai.init(G,v,e); flag=0; if (!oai.valid()) flag=1; }
  
  void init(const _AdjIt& p) {
    oai.init(p.oai);
    iai.init(p.iai);
    flag=p.flag; }
  
  _AdjIt(const _AdjIt& p) : oai(p.oai), iai(p.iai), flag(p.flag) { }
  
  _AdjIt(const graphtype& G) : oai(G), iai(G), flag(0) { if (!oai.valid()) flag=1; }
  
  _AdjIt(const graphtype& G,nodet  n) : oai(G,n), iai(G,n), flag(0) { if (!oai.valid()) flag=1; }
  
  _AdjIt(const graphtype& G,nodet n, edget e) : oai(G,n,e), iai(G,n,e), flag(0) { if (!oai.valid()) flag=1; }
  
  void update(edget e) { if (flag==0) oai.update(e); else iai.update(e); }
  
  void reset() { if (flag==0) oai.reset(); else {iai.reset(); flag=0; } }
  
  void insert(nodet v1, nodet v2) { if (flag==0) oai.insert(v1,v2); else iai.insert(v1,v2); }
  void insert(nodet v) { if (flag==0) oai.insert(v); else iai.insert(v); }
  
  void insert(const _AdjIt& other) {
    if (flag==0) oai.insert(other.get_node()); else iai.insert(other.get_node()); }
  
  void del() {  if (flag==0) oai.del(); else iai.del(); }
  
  
  void reset_end() { if (flag==0) oai.reset_end(); else iai.reset_end(); }
  
  void make_invalid() { if (flag==0) oai.make_invalid(); else iai.make_invalid(); }
  
  void update(nodet  n) { flag=0;
  oai.update(n);  iai.update(n);
  if (!oai.valid()) flag=1;
  }
  
  void update(nodet  n, edget  e) 
  { if (flag==0) { iai.update(n); oai.update(n,e); } else {
		iai.update(n,e); oai.update(n); } }
  
  _AdjIt& operator= (const _AdjIt& it2) { oai=it2.oai; iai=it2.iai; flag=it2.flag;
  return *this; }
  
  bool operator== (const _AdjIt& it2) const  {
    if (flag!=it2.flag) return false;
    if (flag==0) return oai==it2.oai;
    return iai==it2.iai; }
  
  bool has_node() const{ return oai.has_node(); }
  
  bool eol() const { if (flag==0) return oai.eol(); else return iai.eol(); }
  
  bool valid() const { if (flag==0) return oai.valid(); else return iai.valid(); }
  
  edget&  get_edge()  { if (flag==0) return oai.get_edge(); else return iai.get_edge(); }

  edget  get_edge() const { if (flag==0) return oai.get_edge(); else return iai.get_edge(); }
  
  nodet& get_node() { return oai.get_node(); }

  nodet  get_node() const { return oai.get_node(); }

  const graphtype& get_graph() const { return oai.get_graph(); }
  
operator edget() const{ return get_edge(); }
operator edget() { return get_edge(); }
  
  _AdjIt curr_adj() const {
    graphtype& Gp(*((graphtype*)(&oai.get_graph())));
    if (flag==0) {
      _AdjIt temp(Gp,oai.curr_adj().get_node());
      if (!temp.valid()) temp.flag=1;
      return temp;
    }
    _AdjIt temp(Gp,iai.curr_adj().get_node());
    if (!temp.valid())  temp.flag=1;
    return temp;
  }
  
  _AdjIt& operator++ () {
    if (flag==0) {
      edgetype temp(oai.get_edge());
      ++oai;
      if (!oai.valid()) {
        flag=1;
        iai.update(oai.get_node());
        if (iai.get_edge()==temp) iai++;
      }
      return *this;
    }
    ++iai;
    flag=1;
    return *this;
  }
  _AdjIt operator++ (int)  {
    _AdjIt tmp(*this);
    if (flag==0) {
      edgetype temp(oai.get_edge());
      ++oai;
      if (!oai.valid()) {
        flag=1;
        iai.update(oai.get_node());
        if (iai.get_edge()==temp) iai++;
      }
      return tmp;
    }
    ++iai;
    flag=1;
    return tmp; }
  
  _AdjIt& operator-- () {
    if (flag==0) {
      --oai;
      return *this; }
    edgetype temp(iai.get_edge());
    --iai;
    if (!iai.valid()) {
      flag=0; oai.reset_end();
      if (temp==oai.get_edge()) oai--; }
    return *this; }
  
  _AdjIt operator-- (int)  {
    _AdjIt tmp(*this);
    if (flag==0) {
      --oai;
      return tmp; }
    edgetype temp(iai.get_edge());
    --iai;
    if (!iai.valid()) {
      flag=0; oai.reset_end();
      if (temp==oai.get_edge()) oai--; }
    return tmp; }
  friend ostream& operator<<(ostream& out, const self& )
    {  return out;}
  
friend istream& operator>>(istream& in,  self& )
{  return in;}
  
};



struct LedaNodeAccessor { };
static LedaNodeAccessor mygit_node;
struct LedaEdgeAccessor { };
static LedaEdgeAccessor mygit_edge;

template<class Iter>
inline
leda::node get_object(LedaNodeAccessor, const Iter& it) {
  return it.get_node(); }

template< class Iter>
inline
leda::edge get_object(LedaEdgeAccessor, const Iter& it)  {
  return it.get_edge(); }

template<class Iter> 
inline 
leda::node& access_object(LedaNodeAccessor, Iter& it) { 
  return it.get_node(); }

template< class Iter>
inline
leda::edge& access_object(LedaEdgeAccessor,  Iter& it) {
  return it.get_edge(); }          

template<class Iter, class T>
inline
void set_object(Iter& it, T n) {
  it.update(n); }
   


LIMITS(bool,false,true)

template<class T>
struct GIT_dummygraph : leda::graph {
  T& get_entry(leda::node v) { return LEDA_ACCESS(T,entry(v)); }
  T& get_entry(leda::edge e) { return LEDA_ACCESS(T,entry(e)); }
};

struct LedaNodeMemberAccessor {  };
struct LedaEdgeMemberAccessor {  };

static LedaNodeMemberAccessor  leda_member_node;
static LedaEdgeMemberAccessor  leda_member_edge;

template<class SN, class Iter>
inline
SN& get_object(const LedaNodeMemberAccessor&,const SN&, const Iter& it) {
  return ((GIT_dummygraph<SN>*)(graph_of(it.get_node())))->get_entry(it.get_node()); }

template<class SE, class Iter>
inline
SE& get_object(const LedaEdgeMemberAccessor&, const SE&,const Iter& it) {
  return ((GIT_dummygraph<SE>*)(graph_of(it.get_edge())))->get_entry(it.get_edge()); }

#define Create_SafeTypes(graphtype) \
typedef GIT_SafeNodeIt<SafeLedaNode<SafeGraph<graphtype> > > LSafeNodeIt; \
typedef GIT_SafeEdgeIt<SafeLedaEdge<SafeGraph<graphtype> > > LSafeEdgeIt; \
typedef GIT_SafeAdjIt<SafeLedaOutNodeEdge<SafeGraph<graphtype> > > LSafeOutAdjIt; \
typedef GIT_SafeAdjIt<SafeLedaInNodeEdge<SafeGraph<graphtype> > > LSafeInAdjIt; \
typedef SafeGraph<graphtype> lsafegraph;


template<class T, class Handler, class StructAccessor>
struct HandlerAccessor : git_limits<T> {
  typedef       T       value_type;
  Handler&          i_handler;
  StructAccessor        i_sa;
  
  HandlerAccessor(Handler& handler, StructAccessor sa)
    : git_limits<T>(), i_handler(handler), i_sa(sa) { }
  
  StructAccessor& internal_sa()   { return i_sa; }
  const StructAccessor& internal_sa() const   { return i_sa; }
  Handler& internal_map()  { return i_handler; }
  const Handler& internal_map() const { return i_handler; }
  
  HandlerAccessor& operator=(const
			   HandlerAccessor<T,Handler,StructAccessor>& new_DA) {
  i_handler=new_DA.i_handler;
  i_sa=new_DA.i_sa; return *this; }
};

template<class Iter, class Handler, class StructAccessor, class T>
inline

T get(const HandlerAccessor<T,Handler, StructAccessor>&  da,
      const Iter& it)  {
  return da.internal_map()[get_object(da.internal_sa(),it)]; }

template<class Iter, class Handler, class StructAccessor, class T>
inline

T& access(HandlerAccessor<T,Handler, StructAccessor>&  da, const Iter& it)  {
  return da.internal_map()[get_object(da.internal_sa(),it)]; }

template<class Iter, class Handler, class StructAccessor, class T>
inline

void set(HandlerAccessor<T,Handler, StructAccessor>& da,
	 const Iter & it, T val) {
  da.internal_map()[get_object(da.internal_sa(),it)]=val;
}
template<class Str, class StructAccessor, class T>
struct MemberAccessor : git_limits<T> {
  StructAccessor        i_sa;
  typedef       Str     str_type;
  typedef       T       value_type;
  typedef       T       Str::*Ptr;
  Ptr                   _ptr;
  Str                   _dummy;
  MemberAccessor(StructAccessor sa, Ptr ptr) : git_limits<T>(), i_sa(sa), _ptr(ptr) { }
  
  const Str& dummy() const { return _dummy; }
StructAccessor&  internal_sa()  { return i_sa; }
const StructAccessor& internal_sa()  const { return i_sa; }
Ptr& internal_ptr()  { return _ptr; }
const Ptr& internal_ptr() const  { return _ptr; }
};

template<class Str, class StructAccessor, class T, class Iter>
inline
T get(const MemberAccessor<Str, StructAccessor, T>& ma, const Iter& it) {
  return get_object(ma.internal_sa(),ma.dummy(),it).*(ma.internal_ptr()); }

template<class Str, class StructAccessor, class T, class Iter>
inline
T& access(MemberAccessor<Str, StructAccessor, T>& ma,  Iter it) {
  T& t=get_object(ma.internal_sa(),ma.dummy(),it).*(ma.internal_ptr());
  return t; }

template<class Str, class StructAccessor, class T, class Iter>
inline
void set(MemberAccessor<Str, StructAccessor, T>& ma, const Iter & it, T val) {
  get_object( ma.internal_sa(),ma.dummy(),it).*(ma.internal_ptr()) = val; }
template<class Str, class StructAccessor, class T>
struct MethodAccessor : git_limits<T> {
  StructAccessor          i_sa;
  typedef       Str     str_type;
  typedef       T       value_type;
  typedef T     (Str::*Ptr)();
  Ptr           _ptr;
  
  MethodAccessor(StructAccessor sa, Ptr ptr) : git_limits<T>(), i_sa(sa), _ptr(ptr) { }
  StructAccessor&  internal_sa()  { return i_sa; }
const StructAccessor&  internal_sa() const { return i_sa; }
Ptr  internal_ptr()  { return _ptr; }
const Ptr&  internal_ptr() const { return _ptr; }
};

template<class Str, class StructAccessor, class T, class Iter>
inline
T get(const MethodAccessor<Str,StructAccessor,T>& ma, const Iter& it) {
  return (get_object(ma.internal_sa(),it).*(ma.internal_ptr()))(); }

template<class Str, class StructAccessor, class T, class Iter>
inline
T& access(MethodAccessor<Str,StructAccessor,T>& ma, const Iter& it) {
  return (get_object(ma.internal_sa(),it).*(ma.internal_ptr()))(); }
template<class T>
struct ConstantAccessor : git_limits<T> {
  typedef       T       value_type;
  const T       _t;
  
  ConstantAccessor(T t) :  git_limits<T>() ,_t(t){ }
  
  const T&  internal_value() const { return _t; }
};

template<class T, class Iter>
inline
T get(const ConstantAccessor<T>& ca, const Iter& it) {
  return ca.internal_value(); }

template<class T, class Iter>
inline
T& access(const ConstantAccessor<T>& ca, const Iter& it) {
  return ca.internal_value(); }
template<class T, class SourceAcc, class TargetAcc, class Calc>
struct CalcAccessor : git_limits<T> {
  typedef       T       value_type;
  Calc           _calc;
  SourceAcc      _sa;
  TargetAcc      _ta;
  
  CalcAccessor(const SourceAcc& sa, const TargetAcc& ta,
	       const Calc& calc) : git_limits<T>(), _sa(sa), _ta(ta), _calc(calc) { }
  const SourceAcc& internal_sa() const { return _sa; }
const TargetAcc& internal_ta() const { return _ta; }
const Calc&      internal_calc() const { return _calc; }
};

template<class T,class Calc, class SourceAcc,  class TargetAcc, class Iter>
inline
T get(const CalcAccessor<T,SourceAcc,TargetAcc,Calc>& ca,
      const Iter& it) {
  const SourceAcc& sa(ca.internal_sa());
  const TargetAcc& ta(ca.internal_ta());
  T result;
  calculate(ca.internal_calc(),result,get(sa,it),get(ta,it));
  return result; }

template<class T,class Calc, class SourceAcc,  class TargetAcc, class Iter>
inline
T& access(const CalcAccessor<T,SourceAcc,TargetAcc,Calc>& ca,
	  const Iter& it) { return get(ca,it); }


/*******************************************************************************

 (III) DataAccessor-Classes
 
     * node_array_da, edge_array_da
     * node_map_da, edge_map_da 
     * constant_da
     * node_member_da, edge_member_da
     * node_attribute_da, edge_attribute_da
      
 Note: 
 The c++-comments contain manual pages in the leda manual style format.

*******************************************************************************/

/*{\Manpage {node_array_da} {T}
{Node Array Data Accessor} {da} }*/
/*{\Mdefinition
  An instance |da| of class  |\Mname| is instantiated with a LEDA
  {\tt node\_array<T>}.
  
  The data in the node array
  can be accessed
  by the functions |get(\Mvar,it)| and |set(\Mvar,it,value)| that
  take as parameters an  instance of
  |\Mname| and an iterator, see below.
  
  |\Mname|{\tt ::value\_type} is a type and equals {\tt T}.

  For |node_map<T>| there is the variant |node_map_da<T>|
  which is defined completely analogous to |\Mname|.
  Classes |edge_array_da<T>| and |edge_map_da<T>| are defined
  analogously, as well.
}*/

template<class T>
struct node_array_da : git_limits<T> {
  /*{\Mcreation }*/
  typedef       T       value_type;
  leda::node_array<T>*          i_handler;
  
  node_array_da() : git_limits<T>() { i_handler=nil; }
  /*{\Mcreate introduces a variable |\Mvar| of this class
    that is not bound. }*/
  
  node_array_da(leda::node_array<T>& na)
    : git_limits<T>(), i_handler(&na)  { }
/*{\Mcreate introduces a variable |\Mvar| of this class
  bound to |na|.}*/
  
leda::node_array<T>& internal_map()  {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler==nil) LEDA_EXCEPTION(512,"Data Accessor: leda::node leda::array not initialized\n");
#endif
  return *i_handler; }
  
const leda::node_array<T>& internal_map() const {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler==nil) LEDA_EXCEPTION(512,"Data Accessor: leda::node leda::array not initialized\n");
#endif
  return *i_handler; }
  
/*{\Moperations }*/
  
void init(leda::node_array<T>& na) {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler!=nil) LEDA_EXCEPTION(511,"Data Accessor: leda::node leda::array already initialized\n");
#endif
  i_handler=&na; }
/*{Bounds |\Mvar| to |na|. }*/
  
node_array_da& operator=(const
			 node_array_da<T>& n) {
  i_handler=n.i_handler;
  return *this; }
  
friend ostream& operator<<(ostream& out, const node_array_da<T>& )  {  return out;}
friend istream& operator>>(istream& in,  node_array_da<T>& ) {  return in;}
};

template<class Iter, class T>
inline

T get(const node_array_da<T>&  da,
  const Iter& it)  {
return da.internal_map()[get_object(mygit_node,it)]; }
/*{\Mfunc    returns the associated value of |it| for this accessor. }*/

template<class Iter,  class T>
inline

T& access(node_array_da<T>&  da, const Iter& it)  {
return da.internal_map()[get_object(mygit_node,it)]; }

template<class Iter,  class T>
inline

void set(node_array_da<T>& da,
  const Iter & it, T val) {
  da.internal_map()[get_object(mygit_node,it)]=val;
}
/*{\Mfunc   sets the associated value of |it| for this accessor to the
given value. }*/
/*{\Mimplementation Constant Overhead.
}*/
/*{\Mexample
  We count the number of 'red nodes' in a parameterized graph G.
  \begin{verbatim}
  int count_red(graph G, node_array<color> COL) {
    node_array_da<color> Color(COL);
    int counter=0;
    NodeIt it(G);
    while (it.valid()) {
      if (get(Color,it)==red) counter++;
      it++; }
    return counter;
  }
  \end{verbatim}
  
  Suppose we want to make this 'algorithm' flexible in the
  representation of colors. Then we could write this version:
  
  \begin{verbatim}
  template<class DA>
  int count_red_t(graph G, DA Color) {
    int counter=0;
    NodeIt it(G);
    while (it.valid()) {
      if (get(Color,it)==red) counter++;
      it++; }
    return counter;
  }
  \end{verbatim}
  
  With the templatized version it is easily to customize it
  to match the interface of the version:
  
  \begin{verbatim}
  int count_red(graph G, node_array<color> COL) {
    node_array_da<color> Color(COL);
    return count_red_t(G,Color); }
  \end{verbatim}
}*/

template<class T>
struct edge_array_da : git_limits<T> {
  
  typedef       T       value_type;
  leda::edge_array<T>*          i_handler;
  
  edge_array_da() : git_limits<T>() { }
  
  edge_array_da(leda::edge_array<T>& ea)
    : git_limits<T>(), i_handler(&ea) { }
  
leda::edge_array<T>& internal_map()  {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler==nil) LEDA_EXCEPTION(512,"Data Accessor: leda::edge leda::array not initialized\n");
#endif
  return *i_handler; }
const leda::edge_array<T>& internal_map() const {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler==nil) LEDA_EXCEPTION(512,"Data Accessor: leda::edge leda::array not initialized\n");
#endif
  return *i_handler; }
  
void init(leda::edge_array<T>& ea) {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler!=nil) LEDA_EXCEPTION(511,"Data Accessor: leda::edge leda::array already initialized\n");
#endif
  i_handler=&ea; }
  
edge_array_da& operator=(const
			 edge_array_da<T>& n) {
  i_handler=n.i_handler;
  return *this; }
  
friend ostream& operator<<(ostream& out, const edge_array_da<T>& )  {  return out;}
friend istream& operator>>(istream& in,  edge_array_da<T>& ) {  return in;}
  
};

template<class Iter, class T>
inline

T get(const edge_array_da<T>&  da,
      const Iter& it)  {
  return da.internal_map()[get_object(mygit_edge,it)]; }

template<class Iter, class T>
inline

T& access(edge_array_da<T>&  da, const Iter& it)  {
  return da.internal_map()[get_object(mygit_edge,it)]; }

template<class Iter,  class T>
inline

void set(edge_array_da<T>& da,
	 const Iter & it, T val) {
  da.internal_map()[get_object(mygit_edge,it)]=val;
}


struct node_da : git_limits<leda::node> {
  typedef  leda::node       value_type;
  node_da() : git_limits<leda::node>() { }
  node_da& operator=(const node_da& n_) { return *this; } 
friend ostream& operator<<(ostream& out, const node_da& )  {  return out;}
friend istream& operator>>(istream& in,  node_da& ) {  return in;}
};

template<class Iter>
inline
leda::node get(const node_da&  da,  const Iter& it)  {
  return get_object(mygit_node,it); }

template<class Iter>
inline
leda::node& access(const node_da&  da,   Iter& it)  {
  return access_object(mygit_node,it); }
 
template<class Iter>
inline
void set(node_da& da, Iter & it, leda::node val) {
  set_object(it,val); }

struct edge_da : git_limits<leda::edge> {
  typedef  leda::edge       value_type;
  edge_da() : git_limits<leda::edge>() { }
  edge_da& operator=(const edge_da& n_) { return *this; } 
friend ostream& operator<<(ostream& out, const edge_da& )  {  return out;}
friend istream& operator>>(istream& in,  edge_da& ) {  return in;}
};

template<class Iter>
inline
leda::edge get(const edge_da&  da,  const Iter& it)  {
  return get_object(mygit_edge,it); }

template<class Iter>
inline
leda::edge& access(const edge_da&  da,   Iter& it)  {
  return access_object(mygit_edge,it); }

template<class Iter>
inline
void set(edge_da& da,  Iter & it, leda::edge val) {
  set_object(it,val); }

static node_da NodeDa;
static edge_da EdgeDa;

/* The following typedef statements define specialized versions
   of all kinds of iterators to provide a simple alternative to
   the LEDA forall macro definitions.
   The suffix of each type name is indicates the kind of object that
   is returned by operator*().
*/

typedef STLNodeIt<node_da,NodeIt> NodeIt_n;
typedef STLEdgeIt<edge_da,EdgeIt> EdgeIt_e;
typedef STLAdjIt<node_da,AdjIt> AdjIt_n;
typedef STLAdjIt<edge_da,AdjIt> AdjIt_e;

typedef STLAdjIt<node_da,OutAdjIt> OutAdjIt_n;
typedef STLAdjIt<edge_da,OutAdjIt> OutAdjIt_e;

typedef STLAdjIt<node_da,InAdjIt> InAdjIt_n;
typedef STLAdjIt<edge_da,InAdjIt> InAdjIt_e;

template<class T>
struct node_map_da : git_limits<T> {
  typedef       T       value_type;
  leda::node_map<T>*          i_handler;
  
  node_map_da() : git_limits<T> () { i_handler=nil;}
  
  node_map_da(leda::node_map<T>& na)
    : git_limits<T>(), i_handler(&na) { }
  
leda::node_map<T>& internal_map()  {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler==nil) LEDA_EXCEPTION(512,"Data Accessor: leda::node leda::map not initialized\n");
#endif
  return *i_handler; }
  
const leda::node_map<T>& internal_map() const {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler==nil) LEDA_EXCEPTION(512,"Data Accessor: leda::node leda::map not initialized\n");
#endif
  return *i_handler; }
  
node_map_da& operator=(const
		       node_map_da<T>& n) {
  i_handler=n.i_handler;
  return *this; }
  
void init(leda::node_map<T>& na) {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler!=nil) LEDA_EXCEPTION(511,"Data Accessor: leda::node leda::map already initialized\n");
#endif
  i_handler=&na; }
  
};

template<class Iter, class T>
inline

T get(const node_map_da<T>&  da,
      const Iter& it)  {
  return da.internal_map()[get_object(mygit_node,it)]; }

template<class Iter,  class T>
inline

T& access(node_map_da<T>&  da, const Iter& it)  {
  return da.internal_map()[get_object(mygit_node,it)]; }

template<class Iter,  class T>
inline

void set(node_map_da<T>& da,
	 const Iter & it, T val) {
  da.internal_map()[get_object(mygit_node,it)]=val;
}




template<class T>
struct edge_map_da : git_limits<T> {
  typedef       T       value_type;
  leda::edge_map<T>*          i_handler;
  
  edge_map_da() : git_limits<T> () { i_handler=nil; }
  
  edge_map_da(leda::edge_map<T>& na)
    : git_limits<T>(), i_handler(&na) { }
  
leda::edge_map<T>& internal_map()  {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler==nil) LEDA_EXCEPTION(512,"Data Accessor: leda::edge leda::map not initialized\n");
#endif
  return *i_handler; }
const leda::edge_map<T>& internal_map() const {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler==nil) LEDA_EXCEPTION(512,"Data Accessor: leda::edge leda::map not initialized\n");
#endif
  return *i_handler; }
  
edge_map_da& operator=(const
		       edge_map_da<T>& n) {
  i_handler=n.i_handler;
  return *this; }
  
void init(leda::edge_map<T>& ea) {
#if !defined(LEDA_CHECKING_OFF)
  if (i_handler!=nil) LEDA_EXCEPTION(511,"Data Accessor: leda::edge leda::map already initialized\n");
#endif
  i_handler=&ea; }
  
};

template<class Iter, class T>
inline

T get(const edge_map_da<T>&  da,
      const Iter& it)  {
  return da.internal_map()[get_object(mygit_edge,it)]; }

template<class Iter,  class T>
inline

T& access(edge_map_da<T>&  da, const Iter& it)  {
  return da.internal_map()[get_object(mygit_edge,it)]; }

template<class Iter,  class T>
inline

void set(edge_map_da<T>& da,
	 const Iter & it, T val) {
  da.internal_map()[get_object(mygit_edge,it)]=val;
}


/*{\Manpage {constant_da} {T}
{Constant Accessors} {ca} }*/
/*{\Mdefinition
  An instance |ca| of class  |\Mname|
  is bound to a specific
  value of type [[T]], and the function
  |get(\Mvar,it)| simply returns this value for each iterator.
}*/

template<class T>
struct constant_da : git_limits<T> {
  /*{\Mcreation }*/
  
  typedef       T       value_type;
  const T       _t;
  
  constant_da(T t) :  git_limits<T>() ,_t(t) { }
  /*{\Mcreate introduces a variable |\Mvar| of this class bound to the
  given value |t|. }*/
  
  const T&  internal_value() const { return _t; }
/*{\Moperations }*/
  
};

template<class T, class Iter>
inline

T get(const constant_da<T>& ca, const Iter& it) {
  return ca.internal_value(); }
/*{\Mfunc    returns the value to which |\Mvar| is bound. }*/

template<class T, class Iter>
inline

T& access(const constant_da<T>& ca, const Iter& it) {
  return ca.internal_value(); }

/*{\Mexample
  With the template function of sect.~\ref{node_array_da} we can write
  a function that counts the number of nodes in a graph:
  
  \begin{verbatim}
  int count_all(graph G) {
    constant_da<color> Color(red);
    return count_red_t(G,Color); }
  \end{verbatim}
}*/
/*{\Manpage {node_member_da} {Str,T}
{Node Member Accessors} {da} }*/
/*{\Mdefinition
  An instance |\Mvar| of class |\Mname| manages
  the access to a node parameter that is organized as a member
  of a struct type, which is the first template argument of a parameterized
  graph [[GRAPH<Str,?>]].
  The parameter is of type [[T]] and the struct of type [[Str]].

  Classes |edge_member_da<Str,T>| is defined completely
  analogously.
}*/

template<class Str, class T>
struct node_member_da : git_limits<T> {
  /*{\Mcreation }*/
  
  typedef       Str     str_type;
  typedef       T       value_type;
  typedef       T       Str::*Ptr;
  Ptr                   _ptr;
  Str                   _dummy;
  bool initialized;
  
  node_member_da() : git_limits<T>() { initialized=false; }
  /*{\Mcreate introduces a variable |\Mvar| of this class
  that is not bound. }*/
  
  node_member_da(Ptr ptr) : git_limits<T>(), _ptr(ptr), initialized(true) { }
/*{\Mcreate introduces a variable |\Mvar| of this class,
    which is bound to |ptr|.
  }*/
  
const Str& dummy() const { return _dummy; }
  
Ptr& internal_ptr()  {
#if !defined(LEDA_CHECKING_OFF)
  if (initialized==false) LEDA_EXCEPTION(512,"Data Accessor: member ptr not initialized\n");
#endif
  return _ptr; }
  
const Ptr& internal_ptr() const  {
#if !defined(LEDA_CHECKING_OFF)
  if (initialized==false) LEDA_EXCEPTION(512,"Data Accessor: member ptr not initialized\n");
#endif
  return _ptr; }
  
/*{\Moperations }*/
  
void init(Ptr ptr) {
#if !defined(LEDA_CHECKING_OFF)
  if (initialized==true) LEDA_EXCEPTION(512,"Data Accessor: member ptr already initialized\n");
#endif
  _ptr=ptr; initialized=true; }
/*{Bounds |\Mvar| to |ptr|. }*/
  
};

template<class Str, class T, class Iter>
inline

T get(const node_member_da<Str,T>& ma, const Iter& it) {
  return get_object(leda_member_node,ma.dummy(),it).*(ma.internal_ptr()); }
/*{\Mfunc    returns the associated value of |it| for this accessor. }*/

template<class Str, class T, class Iter>
inline
T& access(node_member_da<Str,T>& ma,  Iter it) {
  T& t=get_object(leda_member_node,ma.dummy(),it).*(ma.internal_ptr());
  return t; }

template<class Str, class T, class Iter>
inline

void set(node_member_da<Str,T>& ma, const Iter & it, T val) {
  get_object( leda_member_node,ma.dummy(),it).*(ma.internal_ptr()) = val; }
/*{\Mfunc   sets the associated value of |it| for this accessor to the
given value. }*/

/*{\Mimplementation  Constant Overhead.
  
  The instance |\Mvar| accesses its parameter through a pointer to
  member of type [[Ptr]], which is defined for example by
{\tt typedef T Str::*Ptr}. }*/
/*{\Mexample
  We have a parameterized graph G where the node information type
  is the following struct type [[Str]]:
  
  \begin{verbatim}
  struct Str {
    int x;
    color col; };
  \end{verbatim}
  
  We want to count the number of red nodes.
  Since we have the template function of sect.~\ref{node_array_da} we can
  easily use it to do the computation:
  
  \begin{verbatim}
  int count_red(GRAPH<Str,double> G) {
    node_member_da<Str,color> Color(&Str::col);
    return count_red_t(G,Color); }
  \end{verbatim}
}*/

template<class Str, class T>
struct edge_member_da : git_limits<T> {
  typedef       Str     str_type;
  typedef       T       value_type;
  typedef       T       Str::*Ptr;
  Ptr                   _ptr;
  Str                   _dummy;
  bool initialized;
  
  edge_member_da() : git_limits<T> () , initialized(false) { }
  
  edge_member_da(Ptr ptr) : git_limits<T>(), _ptr(ptr),initialized(true) { }
  
const Str& dummy() const { return _dummy; }
Ptr& internal_ptr()  {
#if !defined(LEDA_CHECKING_OFF)
  if (initialized==false) LEDA_EXCEPTION(512,"Data Accessor: member ptr not initialized\n");
#endif
  return _ptr; }
const Ptr& internal_ptr() const  {
#if !defined(LEDA_CHECKING_OFF)
  if (initialized==false) LEDA_EXCEPTION(512,"Data Accessor: member ptr not initialized\n");
#endif
  return _ptr; }
  
  
void init(Ptr ptr) {
#if !defined(LEDA_CHECKING_OFF)
  if (initialized==true) LEDA_EXCEPTION(512,"Data Accessor: member ptr already initialized\n");
#endif
  _ptr=ptr; initialized=true; }
  
};


template<class Str, class T, class Iter>
inline

T get(const edge_member_da<Str,T>& ma, const Iter& it) {
  return get_object(leda_member_edge,ma.dummy(),it).*(ma.internal_ptr()); }

template<class Str, class T, class Iter>
inline

T& access(edge_member_da<Str,T>& ma,  Iter it) {
  T& t=get_object(leda_member_edge,ma.dummy(),it).*(ma.internal_ptr());
  return t; }

template<class Str, class T, class Iter>
inline

void set(edge_member_da<Str,T>& ma, const Iter & it, T val) {
  get_object( leda_member_edge,ma.dummy(),it).*(ma.internal_ptr()) = val; }


/*{\Manpage {node_attribute_da} {T}
{Node Attribute Accessors} {da} }*/
/*{\Mdefinition
  An instance |\Mvar| of class |\Mname| manages
  the access to a node parameter with type [[T]]
  of a parameterized
  graph [[GRAPH<T,?>]].

  Classes |edge_attribute_da<T>| is defined completely
  analogously.
}*/

template<class T>
struct node_attribute_da : git_limits<T> {
  /*{\Mcreation }*/
  typedef       T       value_type;
  
  node_attribute_da() :  git_limits<T>() { }
  /*{\Mcreate introduces a variable |\Mvar| of this class.
    }*/
};
/*{\Moperations }*/

template<class T, class Iter>
inline

T get(const node_attribute_da<T>& ma, const Iter& it) {
  return get_object(leda_member_node,T(),it); }
/*{\Mfunc    returns the associated value of |it| for this accessor. }*/

template<class T, class Iter>
inline
T& access(node_attribute_da<T>& ma,  Iter it) {
  T& t=get_object(leda_member_node,T(),it);
  return t; }

template<class T, class Iter>
inline

void set(node_attribute_da<T>& ma, const Iter & it, T val) {
  get_object( leda_member_node,T(),it) = val; }
/*{\Mfunc   sets the associated value of |it| for this accessor to the
given value. }*/

/*{\Mimplementation  Constant Overhead.
}*/

/*{\Mexample
  Given a parameterized graph G with nodes associated with colours,
  we want to count the number of red nodes.
  Since we have the template function of sect.~\ref{node_array_da} we can
  easily use it to do the computation:
  
  \begin{verbatim}
  int count_red(GRAPH<color,double> G) {
    node_attribute_da<color> Color;
    return count_red_t(G,Color); }
  \end{verbatim}
  
}*/

template<class T>
struct edge_attribute_da : git_limits<T> {
  typedef       T       value_type;
  
  edge_attribute_da() :  git_limits<T>() { }
};

template<class T, class Iter>
inline

T get(const edge_attribute_da<T>& ma, const Iter& it) {
  return get_object(leda_member_edge,T(),it); }

template<class T, class Iter>
inline
T& access(edge_attribute_da<T>& ma,  Iter it) {
  T& t=get_object(leda_member_edge,T(),it);
  return t; }

template<class T, class Iter>
inline

void set(edge_attribute_da<T>& ma, const Iter & it, T val) {
  get_object( leda_member_edge,T(),it) = val; }


template<class T>
class Equal {
public:
  bool operator() (T t1, T t2) const {
    return t1==t2; }
};
template<class T>
class Unequal {
public:
  bool operator() (T t1, T t2) const { return t1!=t2; }
};
template<class T>
class LessThan {
public:
  bool operator() (T t1, T t2) const { return t1<t2; }
};
template<class T>
class LessEqual {
public:
  bool operator() (T t1, T t2) const { return t1<=t2; }
};
template<class T>
class GreaterThan {
public:
  bool operator() (T t1, T t2) const { return t1>t2; }
};
template<class T>
class GreaterEqual {
public:
  bool operator() (T t1, T t2) const { return t1>=t2; }
};
template<class Iter, class DA, class Comp>
class ComparePred {
  const DA&             _da;
  Comp                  _comp;
  typename DA::value_type        _val;
public:
  ComparePred(const DA& da, const Comp& comp,  typename DA::value_type val) :
    _da(da), _comp(comp), _val(val) { }
  ComparePred<Iter,DA,Comp>& operator=(const ComparePred<Iter,DA,Comp>&P) {
    _comp=P._comp; _val=P._val; return *this;}
bool operator() (const Iter& it) {
  return _comp(get(_da,it),_val); }
};


/*{\Manpage {GIT_BFS} {OutAdjIt,Queuetype,Mark} 
{Breadth First Search (flexible)}
 {algorithm} }*/
template<class OutAdjIt, class Queuetype, class Mark>
class GIT_BFS {
  /*{\Mdefinition An instance |\Mvar| of class  |\Mname| is
    an implementation of an algorithm that traverses
    a graph in a breadth first order. The queue used for the
    search must be provided by the caller and contains
    the source(s) of the search.
    \begin{itemize} \item 
    If the queue is only modified by appending the iterator
    representing the source node onto the queue, a normal
    breadth first search beginning at the node of the graph is
    performed.

    \item It is possible to initialize the queue with several
    iterators that represent different roots of breadth
    first trees.

    \item By modifying the queue while running the algorithm the
    behaviour of the algorithm can be changed.

    \item After the algorithm performed a breadth first search,
    one may append another iterator onto the queue to restart 
    the algorithm.
    \end{itemize}

    {\bf Iterator version:} There is an iterator version of this algorithm:
    {\tt BFS\_It}. Usage is similar to that of node iterators without
    the ability to go backward in the sequence.

    }*/
  Queuetype                   _q;
  Mark&                       _ma;
   
public:
  /*{\Mcreation }*/
  GIT_BFS(const Queuetype& q, Mark& ma) :
    _q(q), _ma(ma) {  } 
  /*{\Mcreate creates an instance |\Mvar| of this class bound to
the Queue |q| and data accessor |ma|.
 }*/
/*{\Mtext
{\bf Preconditions:} 
\begin{itemize}
\item [[Queuetype]] is a queue parameterized with
items of type [[OutAdjIt]].
\item |q| contains the sources of the
traversal (for each source node an adjacency iterator 
referring to it) and
\item  |ma| is a data accessor
that provides
read and write access to a boolean value for each node
(accessed through iterators). This value is assumed
to be freely usable by |\Mvar|.
\end{itemize}
}*/

GIT_BFS(const Queuetype& q, Mark& ma, const OutAdjIt& ai) :
  _q(q), _ma(ma) { _q.append(ai);  }
/*{\Mcreate creates an instance |\Mvar| of this class bound to
the queue |q|, data accessor |ma| and the 
adjacency iterator |ai| representing the source node 
of the breadth first traversal.
}*/

/*{\Moperations }*/

void next();
/*{\Mop
Performs one iteration of the core loop of the algorithm.
 }*/

OutAdjIt current() const {  return _q.top(); }

OutAdjIt current() {  return _q.top(); }
/*{\Mop returns the ``current'' iterator.}*/

void finish_algo() { while (!_q.empty()) next(); }
/*{\Mop executes the algorithm until |finished()| is true, i.e.
exactly if the Queue is empty.}*/

bool finished() const { return _q.empty(); }
/*{\Mop returns [[true]] if the internal Queue is empty.}*/

//Queuetype & leda::queue() { return _q; }
//deprecated

Queuetype & get_queue() { return _q; }
/*{\Mop gives direct access to internal Queue.}*/
};
template<class OutAdjIt, class Queuetype, class Mark>
void GIT_BFS<OutAdjIt,Queuetype,Mark>::next() {
  OutAdjIt _ai(_q.pop());
  while (_ai.valid()) {
    OutAdjIt _temp=_ai.curr_adj();
    if (get(_ma,_temp)!=true) { 
      set(_ma,_temp,(typename Mark::value_type)true);
      _q.append(_temp); }
    ++_ai;
  } 
}
/*{\Mexample
This example shows how to implement an algorithmic iterator
for breadth first search:

\begin{verbatim}
class BFS_It {
  AdjIt               _source;
  node_array<da>      _handler;
  node_array_da<bool> _mark;
  queue<AdjIt>        _q;
  GIT_BFS<AdjIt,queue<AdjIt>,node_array_da<bool> >  _search;
public:
  BFS_It(graph& G) :
   _source(AdjIt(G)), _handler(G,false),
   _mark(_handler), _search(_q,_mark) 
   {
    _search.get_queue().clear();
    _search.get_queue().append(_source); 
   }
  bool valid() const { return !_search.finished();  }
  node get_node() const { return _search.current().get_node(); }
  BFS_It& operator++() {
   _search.next(); return *this; }
};
\end{verbatim}
 
With this iterator you can easily iterate through a graph 
in breadth first fashion :

\begin{verbatim}
  graph G;
  BFS_It it(G);
  while (it.valid()) { 
    // do something reasonable with 'it.get_node()'
    ++it; 
  } 
\end{verbatim}
}*/
/*{\Mimplementation 
Each operation requires constant time.
Therefore, a normal breadth-first search needs
${\cal O}(m+n)$ time.
}*/

template<class Queuetype, class MyMark, class MyList, class MyOutAdjIt>
inline void GIT_bfs_list(const Queuetype& q, MyMark& ma, 
                        MyList& vlist, MyOutAdjIt ai) 
{
  GIT_BFS<OutAdjIt,Queuetype,MyMark> _bfs(q,ma);
  _bfs.get_queue().clear();
  if (get(ma,ai)!=true) {
    _bfs.get_queue().append(ai);
    set(ma,ai,(typename MyMark::value_type)true); }
  while (!_bfs.get_queue().empty())  {
    vlist.append(_bfs.get_queue().top().get_node());
    _bfs.next();
  }
}
inline leda::list<leda::node> GIT_bfs(leda::graph& G, leda::node v, leda::node_array<bool>& reached);

typedef enum { dfs_leaf, dfs_shrink, dfs_grow_depth, dfs_grow_breadth } dfs_return;
/*{\Manpage {GIT_DFS} {OutAdjIt,Stacktype,Mark} 
{Depth First Search (flexible)}
 {algorithm} }*/
template<class OutAdjIt, class Stacktype, class Mark>
class GIT_DFS {
  /*{\Mdefinition An instance |\Mvar| of class  |\Mname| is
    an implementation of an algorithm that traverses
    a graph in a depth first order. The stack used for the
    search must be provided by the caller and contains
    the source(s) of the search.
    \begin{itemize} \item 
    If the stack is only modified by pushing the iterator
    representing the source node onto the stack, a normal
    depth first search beginning at the node of the graph is
    performed.

    \item It is possible to initialize the stack with several
    iterators that represent different roots of depth
    first trees.

    \item By modifying the stack while running the algorithm the
    behaviour of the algorithm can be changed.

    \item After the algorithm performed a depth first search,
    one may push another iterator onto the stack to restart 
    the algorithm.

    \end{itemize}

    A next step may return a state which describes the last action.
    There are the following three possibilities:

    \begin{enumerate}
    \item {\tt dfs\_shrink}: an adjacency iterator was popped from the stack, i.e.
    the treewalk returns in root-direction
    \item {\tt dfs\_leaf}: same as {\tt dfs\_shrink}, but a leaf occured
    \item {\tt dfs\_grow\_depth}: a new adjacency iterator was appended to the stack
    because it was detected as not seen before, i.e. the treewalk goes in depth-direction
    \item {\tt dfs\_grow\_breadth}: the former current adjacency iterator was replaced
    by the successor iterator, i.e. the treewalk goes in breadth-direction
    \end{enumerate}

    {\bf Iterator version:} There is an iterator version of this algorithm: 
    {\tt DFS\_It}. Usage is similar to that of node iterators without
    the ability to go backward in the sequence.
    }*/
  Stacktype                   _st;
  Mark&                       _ma;
public:
  /*{\Mcreation }*/
  GIT_DFS(const GIT_DFS& other) : _st(other._st), _ma(other._ma) { }


  GIT_DFS(const Stacktype& st, Mark& ma) :
    _st(st), _ma(ma) {  } 
/*{\Mcreate creates an instance |\Mvar| of this class bound to
the stack |st| and data accessor |ma|.
 }*/
/*{\Mtext
{\bf Preconditions:} 
\begin{itemize}
\item [[Stacktype]] is a stack parameterized with
items of type [[OutAdjIt]].
\item |st| contains the sources of the
traversal (for each source node an adjacency iterator 
referring to it) and
\item  |ma| is a data accessor
that provides
read and write access to a boolean value for each node
(accessed through iterators). This value is assumed
to be freely usable by |\Mvar|.
\end{itemize}
}*/

GIT_DFS(const Stacktype& st, Mark& ma, const OutAdjIt& ai) :
  _st(st), _ma(ma) { _st.push(ai);  }
/*{\Mcreate creates an instance |\Mvar| of this class bound to
the stack |st|, data accessor |ma| and the 
adjacency iterator |ai| representing the source node 
of the depth first traversal.
}*/

/*{\Moperations }*/

void next_unseen() ;
/*{\Mop
Performs one iteration of the core loop of the algorithm
for one unseen node of the graph.
 }*/

GIT_DFS& operator=(const GIT_DFS& other) {
  _st=other._st; _ma=other._ma; return *this; }

dfs_return next() ;
/*{\Mop
Performs one iteration of the core loop of the algorithm.
 }*/

OutAdjIt current() const { return _st.top(); }

OutAdjIt current() { return _st.top(); }
/*{\Mop returns the ``current'' iterator.}*/

void finish_algo() { while (!_st.empty()) next(); }
/*{\Mop executes the algorithm until |finished()| is true, i.e.
exactly if the stack is empty.}*/

bool finished() const { return _st.empty(); }
/*{\Mop returns [[true]] if the internal stack is empty.}*/

void init(OutAdjIt s) {
  _st.push(s); set(_ma,s,(typename Mark::value_type)true);  }
/*{\Mop initializes the internal stack with |s|.}*/

//Stacktype & leda::stack() { return _st; }
//deprecated
Stacktype & get_stack() { return _st; }
/*{\Mop gives direct access to internal stack.}*/
};
template<class OutAdjIt, class Stacktype, class Mark>
void GIT_DFS<OutAdjIt,Stacktype,Mark>::next_unseen() {
  OutAdjIt _ai(_st.pop());
  while (_ai.valid()) {
    OutAdjIt _temp=_ai.curr_adj();
    if (get(_ma,_temp)!=true) { 
      set(_ma,_temp,(typename Mark::value_type)true);
      _st.push(_temp); }
    ++_ai;
  } 
}
template<class OutAdjIt, class Stacktype, class Mark>
dfs_return GIT_DFS<OutAdjIt,Stacktype,Mark>::next() {
  OutAdjIt _ai(_st.top ());
  if (_ai.valid()) {
    OutAdjIt _temp( _ai.curr_adj());
    if (!get(_ma,_temp)) {
      _st.push(_temp);    
      set(_ma,_temp,(typename Mark::value_type)true); 
      return dfs_grow_depth;
    } else {
      ++_ai; _st.pop(); 
      if (_ai.valid()) { 
        _st.push(_ai); return dfs_grow_breadth; }
      else return dfs_shrink; } }
else { _st.pop(); return dfs_leaf; }
}
/*{\Mimplementation 
Each operation requires constant time.
Therefore, a normal depth-first search needs
${\cal O}(m+n)$ time.
}*/
template<class Stacktype, class Mark, class List, class OutAdjIt>
inline void GIT_dfs_list(const Stacktype& st, Mark& ma, List& vlist, OutAdjIt ai) 
{
  GIT_DFS<OutAdjIt,Stacktype,Mark> _dfs(st,ma);
  _dfs.get_stack().clear();
  if (get(ma,ai)!=true) _dfs.init(ai);
  while (!_dfs.get_stack().empty())  {
    vlist.push(_dfs.current().get_node());
    _dfs.next_unseen();
  }
}

template<class Stacktype, class Mark, class Order, class OutAdjIt>
inline void GIT_dfs_num(const Stacktype& st, 
			Mark& ma, Order& num_first, 
			Order& num_last, OutAdjIt ai, int& counter1, int& counter2) {
  GIT_DFS<OutAdjIt,Stacktype,Mark> _dfs(st,ma);
  _dfs.get_stack().clear();
  if (!get(ma,ai)) {
    _dfs.init(ai);
    set(num_first,ai,counter1++); }
  while (!_dfs.get_stack().empty())  {
    ai=_dfs.current();
    int rettype=_dfs.next();
    if (rettype==dfs_shrink || rettype==dfs_leaf) {
      if (get(num_first,ai)>0)  
	set(num_last,ai,counter2++);
      else
	set(num_first,ai,counter1++); } 
    if (get(num_first,ai)>0)  
      set(num_last,ai,counter2++);
    else  
      set(num_first,ai,counter1++); 
  }
}
inline leda::list<leda::node> GIT_dfs(leda::graph& G, leda::node s, leda::node_array<bool>& reached);
inline int GIT_dfs_numbering(leda::graph& G,
			     leda::node_array<int>& dfsnum, leda::node_array<int>& compnum);

/*{\Manpage {GIT_TOPOSORT} {OutAdjIt,Indeg,Queuetype} 
{Topological Sort (flexible)}
{algorithm} }*/
template<class OutAdjIt, class Indeg, class Queuetype>
class GIT_TOPOSORT {
  /*{\Mdefinition An instance |\Mvar| of class  |\Mname| is
    an implementation of an algorithm that 
    iterates over all nodes in some topological order, 
    if the underlying graph is acyclic. 
    An object of this class maintains an {\em internal queue},
    which contains all nodes (in form of adjacency iterators
    where the current node is equal to the fixed node)
    that are not yet passed, but all
    its predecessors have been passed.

    {\bf Iterator version:} There is an iterator version of this algorithm:
    {\tt TOPO\_It}. Usage is similar to that of node iterators without
    the ability to go backward in the sequence and only a graph is
    allowed at creation time. Additionally there is {\tt TOPO\_rev\_It} 
    which traverses the graph in reversed topological order.
    }*/
  Indeg &                   indeg;
  Queuetype                            zl;
  bool          _cycle_found;
public:
  /*{\Mcreation }*/
  GIT_TOPOSORT (Indeg& indegree) :
    indeg (indegree), _cycle_found(false) { }
  /*{\Mcreate creates an instance |\Mvar| of this class bound to
|indeg|. 
The internal queue of adjacency iterators is empty. 
}*/
/*{\Mtext
{\bf Preconditions:} 
\begin{itemize}
\item  [[Indeg]] is a data accessor that must
provide both read and write access
\item |indegree| 
stores for every node that corresponds to any iterator
the number of incoming edges (has to be
to computed before)
\item [[Queuetype]] is a queue parameterized with elements of
type [[OutAdjIt]]
\end{itemize}
The underlying graph need not be acyclic.  
Whether or not it is acyclic
can be tested after execution of the algorithm 
(function [[cycle_found()]]).
}*/

/*{\Moperations }*/
void next() {
  OutAdjIt ai(zl.pop());
  while (ai.valid()) {
    OutAdjIt temp(ai.curr_adj());
    typename Indeg::value_type tempval(get(indeg,temp)-1);
    set(indeg,temp,tempval);
    if (tempval == indeg.value_null)
      zl.append(temp);
    ++ai;
  }
}
/*{\Mop 
Performs one iteration of the core loop of the algorithm.
More specifically, the first element of [[get_queue()]] is
removed from the queue, and every immediate successor |n| of
this node for which currently holds  [[get(indeg,n)==0]]  is
inserted in [[get_queue()]]. }*/

void finish_algo() { while (!zl.empty()) next(); }
/*{\Mop executes the algorithm until |finished()| is true, i.e.
exactly if the queue is empty.}*/

bool finished() const { return zl.empty(); }
/*{\Mop returns [[true]] if the internal queue is empty.}*/

OutAdjIt current() const { return get_queue().top(); }

OutAdjIt current() { return get_queue().top(); }
/*{\Mop returns the current adjacency iterator.}*/

const Queuetype & queue() { return zl; }
// deprecated

const Queuetype & get_queue() const { return zl; }

//Queuetype & leda::queue() { return zl; }
//deprecated

Queuetype & get_queue() { return zl; }
/*{\Mop gives direct access to internal queue.}*/

bool cycle_found() const { return _cycle_found; }
/*{\Mop returns [[true]] if a cycle was found.}*/

void reset_acyclic() { _cycle_found=false; }
/*{\Mop resets the internal flag that a cycle was found.}*/
};

/*{\Mimplementation 
The asymptotic complexity is ${\cal O}(m+n)$, where
|m| is the number of edges and |n| the number of nodes.
}*/
/*{\Mexample  
This algorithm performs a normal topological sort if the queue 
is initialized by the set of all nodes with
indegree zero:

Definition of |algorithm|, where |indeg| is a data accessor that
provides full data access to the number of incoming edges
for each node:
\begin{verbatim}
GIT_TOPOSORT<OutAdjIt,Indeg,Queuetype<Nodehandle> > algorithm(indeg);
\end{verbatim}
Initialization of [[get_queue()]] with all nodes of type
[[OutAdjIt::nodetype]] that
have zero indegree, i.e. [[get(indeg,it)==indeg.value_null]].
\begin{verbatim}
while ( !algorithm.finished() ) {
  // do something reasonable with algo.current()
  algo.next();
}
\end{verbatim}
The source code of function [[toposort_count()]] is implemented according
to this pattern and may serve as a concrete example.
}*/
template<class OutAdjIt, class EdgeIt, class Indeg>
inline void GIT_indegree(Indeg& indeg, OutAdjIt ai, EdgeIt it) {
  while (it.valid()) {
    ai.update(it);
    ai=ai.curr_adj();
    set(indeg,ai,get(indeg,ai)+1);
    ++it;
  }
}

template<class Indeg, class Order, class Queuetype, class OutAdjIt, 
  class NodeIt>
inline bool GIT_toposort_count(Indeg& da_ind, Order& da_ord, Queuetype& nl,
			       OutAdjIt ai, NodeIt it_n);

template<class Indeg, class Order, class Queuetype, class OutAdjIt, 
  class NodeIt>
inline bool GIT_toposort_count(Indeg& da_ind, Order& da_ord, Queuetype& nl,
			       OutAdjIt ai, NodeIt it_n) {
  int size=1;
  int counter=1;
  GIT_TOPOSORT<OutAdjIt,Indeg,Queuetype> gtopo(da_ind);
  while (it_n.valid()) {
    if (get(da_ind,it_n) == da_ind.value_null) {
      ai.update(it_n);
      nl.append(ai); }
    ++it_n; ++size;
  }
  while (!nl.empty())  {
    set(da_ord,gtopo.current(),counter++);
    gtopo.next(); }
if (counter==size) return true; return false; 
}

inline bool GIT_toposort_indegree(leda::graph& G, leda::node_array<int>& ord);

inline bool GIT_toposort(leda::graph& G, leda::node_array<int>& ord);

const int NEXT_OUT = 1;
const int NEXT_IN  = 2;
const int NEXT_ORDER = 4;
const int NEXT_DONE = 8;
/*{\Manpage {GIT_SCC} {Out,In,It,OutSt,InSt,NSt,Mark}
{Strongly Connected Components (flexible) }
 {algorithm} }*/
template<class Out, class In, class It, class OutSt,
		class InSt, class NSt, class Mark>
class GIT_SCC {
  /*{\Mdefinition An instance |\Mvar| of class  |\Mname| is
    an implementation of an algorithm that computes
    the strongly connected components.

    {\bf Iterator version:} There is an iterator version of this algorithm:
    {\tt SCC\_It}. Usage is similar to that of node iterators without
    the ability to go backward in the sequence and only a graph is
    allowed at creation time.  Method {\tt compnumb()} returns the component
    number of the current node.
    }*/
  In                      _iai;
  Out                     _oai;
  Mark                    _ma;
  GIT_DFS<Out,OutSt,Mark> _dfs;
  GIT_DFS<In,InSt,Mark>   _rec_dfs;
  It                      global_iterator;
  int                     _state; int component;
  typedef typename It::nodetype    itnodetype;
  NSt                  V;
  itnodetype                  currnode;
public: 
  /*{\Mcreation }*/
  
  GIT_SCC(OutSt ost, InSt ist, Mark ma, Out oai, const It& it,
	  In iai) : _iai(iai),_oai(oai), 
    _ma(ma) , _dfs(ost,_ma), _rec_dfs(ist,_ma),
    global_iterator(it) {
    component=0;
    _state=NEXT_OUT;
    _dfs.get_stack().clear(); 
    global_iterator.make_invalid();
    currnode=global_iterator.get_node();
    global_iterator.reset();
    _oai.update(global_iterator); 
    if (!get(_ma,_oai)) 
      _dfs.init(_oai);
  }
  /*{\Mcreate creates an instance |\Mvar| of this class bound to
    the stack |st| and data accessor |ma|.
    }*/
  /*{\Mtext
    {\bf Preconditions:}
    \begin{itemize}
    \item [[Out]] is an adjacency iterator that iterates over the
    outgoing edges of a fixed vertex
    \item [[In]] is an adjacency iterator that iterates over the
    incoming edges of a fixed vertex
    \item [[OutSt]] is stack parameterized with items of type [[Out]]
    \item [[InSt]] is stack parameterized with items of type [[In]]
    \item [[Mark]] is a data accessor that has access to a boolean value
    that is associated with each node of the graph
    \end{itemize}
    }*/
  
  /*{\Moperations }*/
  
  int state() { return _state; }
/*{\Mop  returns the internal state.}*/
/*{\Mtext
  \begin{itemize}
  \item
  {\tt NEXT\_OUT} =first phase,
  \item
  {\tt NEXT\_ORDER} =order phase,
  \item
  {\tt NEXT\_IN}=second phase,
  \item
  {\tt NEXT\_DONE} =algorithm finished
  \end{itemize}
  }*/

void finish_algo() { while (_state!=NEXT_DONE) next(); }
/*{\Mop executes the algorithm until |finished()| is true.
  }*/

bool finished() { return (_state==NEXT_DONE);  }
/*{\Mop returns [[true]] if the algorithm is finished.}*/

InSt & in_stack() { return _rec_dfs.get_stack(); }

InSt & get_in_stack() { return _rec_dfs.get_stack(); }
/*{\Mop gives direct access to the internal stack of incoming adjacency iterators.}*/

In in_current() const { return _rec_dfs.current(); }
/*{\Mop returns the current iterator of the internal stack of incoming 
  adjacency iterators. }*/

OutSt & out_stack() { return _dfs.get_stack(); }

OutSt & get_out_stack()  { return _dfs.get_stack(); }
/*{\Mop gives direct access to the internal stack of outgoing adjacency iterators.}*/

Out out_current() const { return _dfs.current(); }
/*{\Mop returns the current iterator of the internal stack of outgoing
  adjacency iterators. }*/

itnodetype current_node() const { return currnode; }
/*{\Mop returns the current node.}*/

int compnumb() const {  return component; }
/*{\Mop returns the component number of the fixed node of the current 
  iterator if current state is {\tt NEXT\_IN}. }*/

int next() {
  /*{\Mop 
    Performs one iteration of the core loop of the algorithm.
    }*/
  if (_state == NEXT_OUT) {
    if (_dfs.finished ()) {
      global_iterator++;
      while (global_iterator.valid () && get (_ma, global_iterator))
	global_iterator++;
      if (global_iterator.valid ()) {
	_oai.update (global_iterator);
	_dfs.init (_oai);
      }
      else {
	_state = NEXT_ORDER;
	It tempit (global_iterator);
	tempit.reset ();
	while (tempit.valid ())
	  set (_ma, tempit++, (typename Mark::value_type) false);
	_rec_dfs.get_stack ().clear ();
	_state = NEXT_IN;
	_iai.update (V.pop ());
	component = 0;
	_rec_dfs.init (_iai);
	return _state;
      }
    }
    _oai = _dfs.current ();
    int rettype = _dfs.next ();
    if (rettype == dfs_shrink || rettype == dfs_leaf)
      V.push (_oai.get_node ());
  }
  else if (_state == NEXT_IN) {
    if (_rec_dfs.finished ()) {
      while (!V.empty ()) {
	_iai.update (V.pop ());
	if (!get (_ma, _iai))
	  break;
      }
      if (V.empty () || !_iai.has_node ()) {
	_state = NEXT_DONE;
	return _state;
      }
      component++;
      _rec_dfs.init (_iai);
    }
    if (!_rec_dfs.finished ()) {
      currnode = _rec_dfs.current ().get_node ();
      _rec_dfs.next_unseen ();
      return _state;
    }
  }
  return _state;
}
};

/*{\Mimplementation 
Each operation requires constant time.
The algorithm has running time ${\cal O}(\Labs{V}+\Labs{E})$.
}*/

/*{\Manpage {GIT_DIJKSTRA} 
{OutAdjIt,Length,Distance,PriorityQueue,QueueItem} 
{Dijkstra(flexible)}
 {algorithm} }*/
template<class OutAdjIt, class Length, class Distance, 
		class PriorityQueue,class QueueItem>
class GIT_DIJKSTRA {
  /*{\Mdefinition
    An instance |\Mvar| of this class  is
    an implementation of Dijkstra 
    that can be flexibly initialized, stopped after each
    iteration of the core loop, and continued, time and again. 

    {\bf Iterator version:} There is an iterator version of this algorithm:
    {\tt DIJKSTRA\_It}. Usage is more complex and is documented in the
    graphiterator leda extension package. 
    }*/
  PriorityQueue           pq;
  Length           length;         
  Distance         distance;
  QueueItem          QI;
  bool                        _valid;
  bool                  _relaxed;
  OutAdjIt*                _current;
  OutAdjIt*                _curr_adj; 
  typename Distance::value_type         _currdist;
  typedef GIT_DIJKSTRA<OutAdjIt,Length, Distance, PriorityQueue,QueueItem>   self;
  typedef typename Distance::value_type                              disttype;
public:
  /*{\Mcreation }*/
  
  GIT_DIJKSTRA(const Length& l, Distance& d,const QueueItem& qi) :
    length(l), distance(d) ,QI(qi), _current(nil),
    _curr_adj(nil) { pq.clear(); }
  /*{\Mcreate creates an instance |\Mvar| of this class. }*/
  /*{\Mtext
    The length and distance data accessors
    are initialized by the parameter list. The set of sources is empty.
    [[Length]] is a read only data accessor that gives access to the length of
    edges and [[Distance]] is a read/write data accessor that stores the 
    distance of the nodes. [[PriorityQueue]] is a Queue parameterized
    with element of type [[OutAdjIt]] and [[QueueItem]] is a data
    accessor gives access to elements of type [[PriorityQueue::pq_item]].

    {\bf Precondition:} All edge lengths are initialized by values that
    are large enough to be taken as infinity.

    {\bf Remark:} This precondition is not necessary for the algorithm to
    have a defined behavior.  In fact, it may even make sense to
    break this precondition deliberately.  For example, if the
    distances have been computed before and shall only be updated
    after inserting new edges, it makes perfect sense to start
    the algorithm with these distances.

    For a completely new computation,
    the node distances of all nodes are initialized to infinity(i.e.
    [[distance.value_max]]).
    }*/
  
  GIT_DIJKSTRA(const self& D): 
    pq(D.pq), length(D.length), distance(D.distance), QI(D.QI), _current(D._current), _curr_adj(D._curr_adj) { }

/*{\Moperations }*/

// PriorityQueue & leda::queue() { return pq; }
// deprecated

PriorityQueue& get_queue() {  return pq; }
/*{\Mop gives direct access to internal priority queue.}*/

void init(OutAdjIt s) { 
  pq.insert((typename Distance::value_type)0,s); 
  set(distance,s,(typename Distance::value_type)0); 
  _valid=false; _relaxed=false;
}
/*{\Mop $s$ is added to the set of sources.}*/

bool finished()  { return (pq.empty() && !_valid); }
/*{\Mop is true iff the algorithm is finished, i.e. the priority
  queue is empty.}*/

OutAdjIt current() const { 
  if (_current!=nil) return *_current; 
  return pq.inf(pq.find_min()); }
/*{\Mop returns the current adjacency iterator.}*/
OutAdjIt curr_adj() const {
  if (_curr_adj!=nil) return *_curr_adj;
  return current().curr_adj(); }
/*{\Mop returns the an adjacency iterator that is currently
  adjacent to |current()|.}*/

void next_old();

bool is_pred() const {  return _valid && _relaxed; }
/*{\Mop returns true if the current iterator satisfies the
  dijkstra condition. Can be used to compute the predecessors. }*/

void next();
/*{\Mop performs one iteration of the core loop of the algorithm.}*/

void finish_algo() { while(!finished()) next(); }
/*{\Mop executes the algorithm until |finished()| is true, i.e.
  exactly if the priority queue is empty.}*/
};


template<class OutAdjIt, class Length, class Distance,
  class PriorityQueue,class QueueItem>
inline
void GIT_DIJKSTRA<OutAdjIt,Length,Distance,PriorityQueue,QueueItem>::next_old() {
  OutAdjIt it(pq.inf(pq.find_min()));
  pq.del_min();
  while(it.valid()) {
    OutAdjIt itc(it.curr_adj());
    const disttype c(get(distance,it)+(disttype)get(length,it));
    if (c < get(distance,itc)) {
      if (itc.valid()) {
	if (get(QI,itc)!=QI.value_null) 
	  pq.decrease_p(get(QI,itc),c);
	else
	  set(QI,itc, pq.insert(c,itc));
      }
      set(distance,itc,c);
    }
    ++it;
  }
}

template<class OutAdjIt, class Length, class Distance,
  class PriorityQueue,class QueueItem>
inline
void GIT_DIJKSTRA<OutAdjIt,Length,Distance,PriorityQueue,QueueItem>::next() {
  _relaxed=false;
  if (_valid==false) {
    /* current item is not valid, i.e. we have to look for a new 
       item */
    // cout << "size " << pq.size() << endl;
    if (_current==nil) _current=new OutAdjIt(pq.inf(pq.find_min()));
    else  *_current=pq.inf(pq.find_min());
    pq.del_min();
    set(QI,*_current,QI.value_null); 
    //cout << "1current" << index(_current->get_node()) << endl;
    if (_current->valid()) { 
      _valid=true;
      if (_curr_adj==nil) _curr_adj=new OutAdjIt(_current->curr_adj());
      else *_curr_adj=_current->curr_adj();
      //cout << "curr adj" << index(_curr_adj->get_node()) << endl;
      _currdist=get(distance,*_current);
      const disttype c(_currdist+(disttype)get(length,*_current));
      const disttype d(get(distance,*_curr_adj));
      if (_currdist+(disttype)get(length,*_current)<d) { 
        _relaxed=true;
        if (d<distance.value_max)   
          pq.decrease_p(get(QI,*_curr_adj),c);  
        else                                        { 
	  //cout << "inserted\n";
          set(QI,*_curr_adj,pq.insert(c,*_curr_adj));       }
        set(distance, *_curr_adj, c);
      } else _valid=true; 
    }
  }
  else { 
    ++(*_current);
    // cout << "2current" << index(_current->get_node()) << endl;
    if (_current->valid())   {
      _valid=true; 
      *_curr_adj=_current->curr_adj();
      //cout << "curr adj" << index(_curr_adj->get_node()) << endl;
      const disttype c(_currdist+(disttype)get(length,*_current));
      const disttype d(get(distance,*_curr_adj));
      if (c<d) { 
        _relaxed=true;
        if (d<distance.value_max)
          pq.decrease_p(get(QI,*_curr_adj),c);
        else                                           {
	  //cout << "inserted\n";
          set(QI,*_curr_adj,pq.insert(c,*_curr_adj)); 
	}
        set(distance, *_curr_adj, c);
      } 
    }
    else  { _valid=false;  if (!pq.empty()) next(); }
  }
}
/*{\Mexample

Class {\tt GIT\_DIJKSTRA} may be used in a deeper layer in a hierarchy of classes
and functions. For example, you may write a function which 
computes shortes path distances with given iterators and data accessors:

\begin{verbatim}
template<class OutAdjIt, class Length, class Distance, 
         class PriorityQueue, class QueueItem>
void GIT_dijkstra_core(OutAdjIt s, Length& length, Distance& distance,
  PriorityQueue& pq, QueueItem& qi) {
  GIT_DIJKSTRA<OutAdjIt,Length,Distance,PriorityQueue,QueueItem>
    internal_dijk(length,distance,qi);
  internal_dijk.get_queue()=pq;
  set(distance,s,distance.value_null);
  if (s.valid()) {
    internal_dijk.init(s);
    internal_dijk.finish_algo();
  }
}
\end{verbatim}

In another layer, you would instantiate these iterators and data acessors
for a graph and invoke this function.

%Another possibility to use the algorithm are dijkstra algorithm iterators,
%for example in the following way:

%\begin{verbatim}
  %// G is of type graph 
  %// v is of type node 
  %// distance is of type node_array<int> (node distances)
  %// cost is of type edge_array<int> (edge costs)
  %DIJKSTRA_It<int> it(G,v,distance,cost); 
  %while (it.valid()) { 
    %++it;  
  %}
%\end{verbatim}
}*/
/*{\Mimplementation
The asymptotic complexity is ${\cal O}(m+n \cdot T(n))$, where
$T(n)$ is the(possibly amortized) complexity of a single
queue update.

For the priority queues described in Chapter~\ref{Priority Queues}, it is
$T(n)={\cal O}(\log n)$. }*/
template<class OutAdjIt, class Length, class Distance,class PriorityQueue>
void GIT_dijkstra_core(OutAdjIt s, const Length& length, Distance& distance,
		       PriorityQueue& pq);
template<class OutAdjIt, class Length, class Distance, class PriorityQueue,
  class QueueItem>
void GIT_dijkstra_core(OutAdjIt s, Length& length, Distance& distance,
		       PriorityQueue& pq, QueueItem& qi) {
  GIT_DIJKSTRA<OutAdjIt,Length,Distance,PriorityQueue,QueueItem>
    internal_dijk(length,distance,qi); 
  internal_dijk.get_queue()=pq;
  set(distance,s,distance.value_null);
  if (s.valid()) {
    internal_dijk.init(s);
    internal_dijk.finish_algo();
  }
}

template<class OutAdjIt, class Length, class Distance, class PriorityQueue,
  class QueueItem, class Pred>
void GIT_dijkstra_core(OutAdjIt s, Length length, Distance distance,
		       PriorityQueue& pq, QueueItem qi, Pred pred) {
  GIT_DIJKSTRA<OutAdjIt,Length,Distance,PriorityQueue,QueueItem>
    internal_dijk(length,distance,qi);
  internal_dijk.get_queue()=pq;
  set(distance,s,distance.value_null);
  if (s.valid()) {
    internal_dijk.init(s);
    while (!internal_dijk.finished()) {
      internal_dijk.next(); 
      if (internal_dijk.is_pred()) {
        set(pred,internal_dijk.current().curr_adj(),internal_dijk.current().get_edge());
      }
    }
  }
}

LIMITS(leda::pq_item,nil,nil)

template<class OutAdjIt, class LinNodeIt, class Length, class Distance>
void GIT_dijkstra_init(OutAdjIt s, LinNodeIt it,
		       const Length& length, Distance& distance);

template<class OutAdjIt, class LinNodeIt, class Length, class Distance>
void GIT_dijkstra_init(OutAdjIt s, LinNodeIt it, Length& length, Distance& distance) {  
  leda::p_queue<typename Distance::value_type,OutAdjIt> pq;
  while(it.valid()) {
    set(distance,it,distance.value_max);
    ++it; } 
  GIT_dijkstra_core(s, length, distance, pq);
}

template<class TN, class TE>
inline
void GIT_dijkstra_pred(leda::graph & G, leda::node s, leda::edge_array<TE> & length,
		       leda::node_array<TN>& dist, leda::node_array<leda::edge>& pred );

template<class TN, class TE>
inline
void GIT_dijkstra_pred(leda::graph& G, leda::node s, leda::edge_array<TE> & length,
		       leda::node_array<TN>& dist, leda::node_array<leda::edge>& pred) {
  typedef leda::p_queue<TN,OutAdjIt>    PriorityQueue;
  OutAdjIt         ai(G,s);
  NodeIt           it(G);
  node_array_da<TN> distance(dist);
  edge_array_da<TE> length2(length);
  node_array_da<leda::edge> pred2(pred);
  PriorityQueue pq; 
  leda::node_array<pq_item> pq_items(G,nil); 
  node_array_da<pq_item> qi(pq_items);
  while(it.valid()) {
    set(distance,it,distance.value_max);
    ++it; }
  GIT_dijkstra_core(ai,length2,distance,pq,qi,pred2);
}

template<class TN, class TE>
void GIT_dijkstra_all(leda::graph& G, leda::node s, leda::edge_array<TE> & length, 
		      leda::node_array<TN>& dist) { 
  typedef leda::p_queue<TN,OutAdjIt>    PriorityQueue;
  OutAdjIt               ai(G,s);
  NodeIt           it(G);
  node_array_da<TN> distance(dist);
  edge_array_da<TE> length2(length);
  PriorityQueue pq; pq.clear();
  leda::node_array<pq_item> pq_items(G,nil);
  node_array_da<pq_item> qi(pq_items);
  GIT_DIJKSTRA<OutAdjIt, edge_array_da<TE>, node_array_da<TN>, 
    PriorityQueue,node_array_da<pq_item> >
    internal_dijk(length2,distance,qi);
  while(it.valid()) {
    dist[it.get_node()]=distance.value_max;
    ++it; }
  internal_dijk.get_queue()=pq;
  //set(distance,ai,(TN)0);
  internal_dijk.init(ai);
  //internal_dijk.get_queue().insert(0,ai);
  while(!internal_dijk.finished()) {
    internal_dijk.next_old(); 
  }
}


template<class TN, class TE>
void GIT_dijkstra_dist(leda::graph & G, leda::node s,
		       const leda::edge_array<TE>& length, leda::node_array<TN>& dist ) ;

template<class TN, class TE>
void GIT_dijkstra_dist(leda::graph & G, leda::node s,
		       leda::edge_array<TE>& length, leda::node_array<TN>& dist ) {
  OutAdjIt         ai(G,s);        
  NodeIt     it(G);               
  node_array_da<TN> distance(dist);
  edge_array_da<TE> length2(length);
  GIT_dijkstra_init(ai, it, length2, distance);
}

template<class TE>
TE GIT_dijkstra_pair(leda::graph& G, leda::node s, leda::node t,
		     const leda::edge_array<TE> & length) ;

template<class TE>
TE GIT_dijkstra_pair(leda::graph& G, leda::node s, leda::node t,
		     leda::edge_array<TE> & length) { 
  typedef TE                   TN;
  typedef leda::p_queue<TN,OutAdjIt>    PriorityQueue;
  leda::node_array<TN>               dist(G);
  if (s==t) return 0;
  OutAdjIt      ai(G,s);
  NodeIt     it(G); 
  node_array_da<TN> distance(dist);
  edge_array_da<TE> length2(length);
  PriorityQueue pq;
  leda::node_array<pq_item> pq_items(G,nil);
  node_array_da<pq_item> qi(pq_items);

  GIT_DIJKSTRA<OutAdjIt, edge_array_da<TE>, node_array_da<TN>, 
    PriorityQueue,node_array_da<pq_item> >
    internal_dijk(length2,distance,qi); 
  while(it.valid()) {
    dist[it.get_node()]=distance.value_max;
    ++it; }
  if (ai.eol()) return distance.value_max;
  internal_dijk.get_queue()=pq;
  dist[s]=0;
  internal_dijk.init(ai);
  while(dist[t]==distance.value_max && !(internal_dijk.finished()))
    internal_dijk.next();
  return dist[t];
}

template<class TE>
TE GIT_dijkstra_pair(leda::graph & G, leda::node s, leda::node t,
		     const leda::edge_array<TE> & length, leda::list<leda::edge>& path);

template<class TE>
TE GIT_dijkstra_pair(leda::graph& G, leda::node s, leda::node t,
		     leda::edge_array<TE> & length, leda::list<leda::edge>& path) {
  typedef TE                   TN;
  typedef leda::p_queue<TN,OutAdjIt>    PriorityQueue;
  leda::node_array<TN>               dist(G);
  leda::node_array<leda::edge>             pred(G);
  if (s==t) return 0;
  OutAdjIt               ai(G,s);
  NodeIt              it(G);
  node_array_da<TN> distance(dist);
  edge_array_da<TE> length2(length);
  PriorityQueue pq;
  path.clear();
  leda::node_array<pq_item> pq_items(G,nil);
  node_array_da<pq_item> qi(pq_items);
  GIT_DIJKSTRA<OutAdjIt, edge_array_da<TE>, node_array_da<TN>, 
    PriorityQueue,node_array_da<pq_item> >
    internal_dijk(length2,distance,qi);
  while(it.valid()) {
    dist[it.get_node()]=distance.value_max;
    pred[it.get_node()]=nil;
    ++it; }
  if (ai.eol()) return distance.value_max;
  internal_dijk.get_queue()=pq;
  set(distance,ai,(TN)0); 
  internal_dijk.init(ai);
  while(pred[t]==nil && !(internal_dijk.finished()))  {
    internal_dijk.next(); 
    if (internal_dijk.is_pred()) {
      OutAdjIt temp(internal_dijk.current());
      pred[temp.curr_adj().get_node()]=temp.get_edge();
    }
  }
if (dist[t]<distance.value_max) {
  leda::edge e=pred[t];
  while(e!=nil) {
    path.append(e); 
    e=pred[source(e)]; }
}
return dist[t];
}

 
template<class T>
class DIJKSTRA_It {
  typedef leda::node_array<T> handler_distance;
  typedef leda::edge_array<T> handler_length;
  typedef leda::node_array<pq_item> handler_pq;
  typedef T handlertype;
  typedef HandlerAccessor<handlertype,handler_distance,LedaNodeAccessor> Distance;
  typedef HandlerAccessor<handlertype,handler_length,LedaEdgeAccessor> Length;
  typedef HandlerAccessor<pq_item,handler_pq,LedaNodeAccessor> QueueItem;
  typedef leda::p_queue<T,OutAdjIt> PriorityQueue;
  typedef GIT_DIJKSTRA<OutAdjIt,Length,Distance,PriorityQueue,QueueItem> Search;
  OutAdjIt _source;
  Distance _distance;
  Length _length;
  handler_pq qi;
  QueueItem _qi;
  Search _search;
public:
    
  DIJKSTRA_It(leda::graph& G, leda::node v, handler_distance& dist, handler_length& len) :
    _source(OutAdjIt(G,v)),
    _distance(dist, mygit_node), _length(len,mygit_edge), qi(G,nil),
    _qi(qi,mygit_node),
    _search(_length,_distance,_qi) {
    _search.get_queue().clear();
    NodeIt it(G);
    while(it.valid()) {  
      set(_distance,it,_distance.value_max); ++it; }
    _search.init(_source);
  }
  
  Distance& distance() { return _distance; }

DIJKSTRA_It(leda::graph& G, handler_distance& dist, handler_length& len) :
  _source(OutAdjIt(G)),
  _distance(dist, mygit_node), _length(len,mygit_edge), qi(G,nil),
  _qi(qi,mygit_node),
  _search(_length,_distance,_qi) {
  _search.get_queue().clear();
  NodeIt it(G);
  while(it.valid()) {
    set(_distance,it,_distance.value_max); ++it; }
  _search.init(_source);
}
  
void reset() { make_invalid();}
  
void make_invalid() { _search.get_queue().clear(); }
  
bool valid() { return !_search.finished(); }
  
bool eol() { return _search.finished(); }
  
leda::node get_node() const {
  return _search.current().get_node(); }
  
bool has_edge() const {
  return _search.current().get_edge()!=nil; }
  
leda::edge get_edge() const { return _search.current().get_edge(); }
  
DIJKSTRA_It& operator++() {
  _search.next(); return *this; }
 
bool operator==(const DIJKSTRA_It<T> & p) const { 
  if (has_edge()) { 
    if (!p.has_edge()) return false; 
    return get_edge() == p.get_edge(); } 
  return get_node()==p.get_node(); }

bool is_pred() const {
  return _search.is_pred() ; } 
  
Search current_search() const {
  return _search; }
};

template<class T>
class Rev_DIJKSTRA_It {
  typedef leda::node_array<T> handler_distance;
  typedef leda::edge_array<T> handler_length;
  typedef leda::node_array<pq_item> handler_pq;
  typedef T handlertype;
  typedef HandlerAccessor<handlertype,handler_distance,LedaNodeAccessor> Distance;
  typedef HandlerAccessor<handlertype,handler_length,LedaEdgeAccessor> Length;
  typedef HandlerAccessor<pq_item,handler_pq,LedaNodeAccessor> QueueItem;
  typedef leda::p_queue<T,InAdjIt> PriorityQueue;
  typedef GIT_DIJKSTRA<InAdjIt,Length,Distance,PriorityQueue,QueueItem> Search;
  InAdjIt _source;
  Distance _distance;
  Length _length;
  handler_pq qi;
  QueueItem _qi;
  Search _search;
public:

  Rev_DIJKSTRA_It(leda::graph& G, leda::node v, handler_distance& dist, handler_length& len) :
    _source(InAdjIt(G,v)),
    _distance(dist, mygit_node), _length(len,mygit_edge), qi(G,nil),
    _qi(qi,mygit_node),
    _search(_length,_distance,_qi) {
    _search.get_queue().clear();
    NodeIt it(G);
    while(it.valid()) {
      set(_distance,it,_distance.value_max); ++it; }
    _search.init(_source);
  }    

  Rev_DIJKSTRA_It(leda::graph& G, handler_distance& dist, handler_length& len) :
    _source(InAdjIt(G)),
    _distance(dist, mygit_node), _length(len,mygit_edge), qi(G,nil),
    _qi(qi,mygit_node),
    _search(_length,_distance,_qi) {
    _search.get_queue().clear();
    NodeIt it(G);
    while(it.valid()) {
      set(_distance,it,_distance.value_max); ++it; }
    _search.init(_source);
}

Distance& distance() { return _distance; }

void reset() { make_invalid();}

void make_invalid() { _search.get_queue().clear(); }

bool valid() { return !_search.finished(); }

bool eol() { return _search.finished(); }

leda::node get_node() const {
  return _search.current().get_node(); }

bool has_edge() const {
  return _search.current().get_edge()!=nil; }

leda::edge get_edge() const { return _search.current().get_edge(); }

Rev_DIJKSTRA_It& operator++() {
  _search.next(); return *this; }

bool operator==(const Rev_DIJKSTRA_It<T> & p) const { 
  if (has_edge()) { 
    if (!p.has_edge()) return false;  
    return get_edge() == p.get_edge(); } 
  return get_node()==p.get_node(); }

bool is_pred() const {
  return _search.is_pred() ; } 
Search current_search() const {
  return _search; } 
};                                                                 



class  BFS_It {
  typedef leda::node_array<bool> handler;
  typedef bool handlertype;
  typedef HandlerAccessor<handlertype,handler,LedaNodeAccessor> Mark;
  typedef leda::queue<AdjIt> Stacktype;
  typedef GIT_BFS<AdjIt,Stacktype,Mark> Search;
  AdjIt _source;
  handler _handler;
  Mark _mark;
  Stacktype _st;
  Search _search;
public:
    
  BFS_It(leda::graph& G, leda::node v);
    
  BFS_It(leda::graph& G);
    
  void reset() ;
    
  void make_invalid() ;
    
  bool valid() const;
    
  bool eol() const;
    
  leda::node get_node() const ;
    
  leda::edge get_edge() const;
    
  BFS_It& operator++();
    
};

class TOPO_It {
  typedef leda::node_array<int> handler;
  typedef int handlertype;
  typedef HandlerAccessor<handlertype,handler,LedaNodeAccessor> Indeg;
  typedef leda::queue<OutAdjIt> Queuetype;
  typedef GIT_TOPOSORT<OutAdjIt,Indeg,Queuetype> Search;
  handler _handler;
  leda::graph& _g;
  Indeg _indeg;
  Search _search;
public:
    
  TOPO_It(leda::graph & G) ;
    
  void reset() ;
    
  void make_invalid() ;
    
  bool valid()  const;
    
  bool eol() const;
    
  leda::node get_node() const ;
    
  leda::edge get_edge() const ;
    
  TOPO_It& operator++() ;
    
};

class TOPO_rev_It {
  typedef leda::node_array<int> handler;
  typedef int handlertype;
  typedef HandlerAccessor<handlertype,handler,LedaNodeAccessor> Indeg;
  typedef leda::queue<InAdjIt> Queuetype;
  typedef GIT_TOPOSORT<InAdjIt,Indeg,Queuetype> Search;
  handler _handler;
  leda::graph& _g;
  Indeg _indeg;
  Search _search; 
public:
    
  TOPO_rev_It(leda::graph & G) ;
    
  void reset();
    
  void make_invalid() ;
    
  bool valid() const ;
    
  bool eol() const ;
    
  leda::node get_node() const ;
    
  leda::edge get_edge() const ;
    
  TOPO_rev_It& operator++() ;
    
};

class DFS_It_directed {
  
  typedef leda::node_array<bool> handler;
  typedef bool handlertype;
  typedef HandlerAccessor<handlertype,handler,LedaNodeAccessor> Mark;
  typedef leda::stack<AdjIt> Stacktype;
  typedef GIT_DFS<AdjIt,Stacktype,Mark> Search;
  AdjIt _source;
  handler _handler;
  Mark _mark;
  Stacktype _st;
  Search _search;
  int _state;
public:
    
  DFS_It_directed(leda::graph& G, leda::node v) ;
    
    
  DFS_It_directed(leda::graph& G);
    
  void reset() ;
    
  void make_invalid() ;
    
  bool valid() const;
    
  bool eol() const ;
    
  leda::node get_node() const ;
    
  leda::edge get_edge() const ;
    
  int state() const ;
    
  DFS_It_directed& operator++() ;
    
};

class SCC_It {
  
  typedef   leda::stack<OutAdjIt> OutStack;
  typedef   leda::stack<InAdjIt> InStack;
  typedef leda::node_array<bool> _Mark;
  typedef HandlerAccessor<bool,_Mark,LedaNodeAccessor> Mark;
  typedef GIT_SCC<OutAdjIt,InAdjIt,NodeIt,OutStack,InStack,leda::stack<leda::node>,Mark> Search;
  _Mark _mark;
  Mark  mark;
  Search _search;
public:
    
  SCC_It(leda::graph& G) : _mark(G,false), mark(_mark,mygit_node),
    _search(OutStack(),InStack(),mark,OutAdjIt(G),NodeIt(G),
	    InAdjIt(G)) {
    while (_search.state()==NEXT_OUT) _search.next();
    _search.next();
  }
  
  void reset() { make_invalid();}
  
void make_invalid() {  }
  
bool valid() { return !_search.finished(); }
  
int compnumb() { return _search.compnumb(); }
  
bool eol() { return _search.finished(); }
  
leda::node get_node() const { return _search.current_node(); }
  
SCC_It& operator++() {
  _search.next(); return *this; }
  
};

class DFS_It {
public:
  typedef leda::node_array<bool> handler;
  typedef bool handlertype;
  typedef HandlerAccessor<handlertype,handler,LedaNodeAccessor> Mark;
  typedef leda::stack<AdjIt> Stacktype;
  typedef GIT_DFS<AdjIt,Stacktype,Mark> Search;
private:
  AdjIt _source;
  handler _handler;
  Mark _mark;
  Stacktype _st;
  Search _search;
public:
    
  DFS_It(leda::graph& G, leda::node v);
    
  DFS_It(leda::graph& G);
    
  void reset();
    
  void make_invalid();
    
  bool valid() const;
    
  bool eol() const;
    
  leda::node get_node() const ;
    
  leda::edge get_edge() const ;
    
  DFS_It& operator++() ;
    
};



inline DFS_It::DFS_It(leda::graph& G, leda::node v) : _source(AdjIt(G,v)), 
  _handler(G,false),
  _mark(_handler,mygit_node), _search(_st,_mark) {
  _search.get_stack().clear();
  _search.get_stack().push(_source); }

inline DFS_It::DFS_It(leda::graph& G) : _source(AdjIt(G)), 
    _handler(G,false),
    _mark(_handler,mygit_node), _search(_st,_mark) {
  _search.get_stack().clear();
  _search.get_stack().push(_source); }

inline   void DFS_It::make_invalid() { _search.get_stack().clear(); }

inline void DFS_It::reset() { make_invalid();} 


inline bool DFS_It::valid() const { return !_search.finished(); }

inline bool DFS_It::eol()const { return _search.finished(); }

inline leda::node DFS_It::get_node() const { return _search.current().get_node(); }

inline leda::edge DFS_It::get_edge() const { return _search.current().get_edge(); }

inline DFS_It& DFS_It::operator++() {
  _search.next_unseen(); return *this; }

inline 
BFS_It::BFS_It(leda::graph& G, leda::node v) : _source(AdjIt(G,v)), _handler(G,false),
  _mark(_handler,mygit_node), _search(_st,_mark) {
  _search.get_queue().clear();
  _search.get_queue().append(_source); }

inline BFS_It::BFS_It(leda::graph& G) : _source(AdjIt(G)), _handler(G,false),
    _mark(_handler,mygit_node), _search(_st,_mark) {
  _search.get_queue().clear();
  _search.get_queue().append(_source); }

inline void BFS_It::make_invalid() { _search.get_queue().clear(); }

inline void BFS_It::reset() { make_invalid();} 


inline bool BFS_It::valid() const { return !_search.finished(); }

inline bool BFS_It::eol() const { return _search.finished(); }

inline leda::node BFS_It::get_node() const { return _search.current().get_node(); }

inline leda::edge BFS_It::get_edge() const { return _search.current().get_edge(); }

inline BFS_It& BFS_It::operator++() {
  _search.next(); return *this; }
inline 
TOPO_It::TOPO_It(leda::graph & G) : 
  _handler(G), _g(G),
  _indeg(_handler,mygit_node), _search(_indeg) { 
  NodeIt it(G);
  while (it.valid()) {
    set(_indeg,it,indeg(it.get_node())); 
    ++it; } 
  it.reset(); 
  OutAdjIt ai(G);
  while (it.valid()) {
    if (get(_indeg,it)==0) { 
      ai.update(it); 
      _search.get_queue().append(ai); }
    ++it; }
}


inline void TOPO_It::make_invalid() { _search.get_queue().clear();
 NodeIt it(_g);
 while (it.valid()) {
   set(_indeg,it,indeg(it.get_node())); 
   ++it; }
 it.reset(); 
 OutAdjIt ai(_g);
 while (it.valid()) {
   if (get(_indeg,it)==0) { 
     ai.update(it); 
     _search.get_queue().append(ai); }
   ++it; } 

}
inline void TOPO_It::reset() { make_invalid();}

inline bool TOPO_It::valid() const { return !_search.finished(); }

inline bool TOPO_It::eol() const { return _search.finished(); }

inline leda::node TOPO_It::get_node() const { return _search.current().get_node(); }

inline leda::edge TOPO_It::get_edge() const { return _search.current().get_edge(); }

inline TOPO_It& TOPO_It::operator++() {
  _search.next(); return *this; }

inline TOPO_rev_It::TOPO_rev_It(leda::graph & G) : 
  _handler(G), _g(G) , 
  _indeg(_handler,mygit_node), _search(_indeg) { 
  NodeIt it(G);
  while (it.valid()) {
    set(_indeg,it,outdeg(it.get_node())); 
    ++it; } 
  it.reset(); 
  InAdjIt ai(G);
  while (it.valid()) {
    if (get(_indeg,it)==0) { 
      ai.update(it); 
      _search.get_queue().append(ai); }
    ++it; }
}


inline void TOPO_rev_It::make_invalid() { _search.get_queue().clear();
 NodeIt it(_g);
 while (it.valid()) {
   set(_indeg,it,outdeg(it.get_node())); 
   ++it; }
 it.reset(); 
 InAdjIt ai(_g);
 while (it.valid()) {
   if (get(_indeg,it)==0) { 
     ai.update(it); 
     _search.get_queue().append(ai); }
   ++it; } 
}
inline void TOPO_rev_It::reset() { make_invalid();}

inline bool TOPO_rev_It::valid() const { return !_search.finished(); }

inline bool TOPO_rev_It::eol() const { return _search.finished(); }

inline leda::node TOPO_rev_It::get_node() const { return _search.current().get_node(); }

inline leda::edge TOPO_rev_It::get_edge() const { return _search.current().get_edge(); }

inline TOPO_rev_It& TOPO_rev_It::operator++() {
  _search.next(); return *this; }
inline 
DFS_It_directed::DFS_It_directed(leda::graph& G, leda::node v) : _source(AdjIt(G,v)), 
  _handler(G,false),
  _mark(_handler,mygit_node), _search(_st,_mark) {
  _state=dfs_grow_depth;
  _search.get_stack().clear();
  _search.get_stack().push(_source); }


inline DFS_It_directed::DFS_It_directed(leda::graph& G) : _source(AdjIt(G)), 
    _handler(G,false),
    _mark(_handler,mygit_node), _search(_st,_mark) {
  _state=dfs_grow_depth;
  _search.get_stack().clear();
  _search.get_stack().push(_source); }


inline void DFS_It_directed::make_invalid() { _search.get_stack().clear(); _state=dfs_grow_depth;  }

inline void DFS_It_directed::reset() { make_invalid();}

inline bool DFS_It_directed::valid() const { return !_search.finished(); }

inline bool DFS_It_directed::eol() const { return _search.finished(); }

inline leda::node DFS_It_directed::get_node() const { return _search.current().get_node(); }

inline leda::edge DFS_It_directed::get_edge() const { return _search.current().get_edge(); }

inline int DFS_It_directed::state() const { return _state; }

inline DFS_It_directed& DFS_It_directed::operator++() {
  _state=_search.next(); return *this; }



inline leda::list<leda::node> GIT_dfs(leda::graph& G, leda::node v, leda::node_array<bool>& reached) {
  leda::list<leda::node> list_nodes; list_nodes.clear();
  node_array_da<bool> da_mark(reached);
  OutAdjIt       ai(G,v);
  leda::stack<OutAdjIt>        st;
  GIT_dfs_list(st,da_mark,list_nodes,ai); 
  return list_nodes; }


inline int GIT_dfs_numbering(leda::graph& G, 
			     leda::node_array<int>& num_first, leda::node_array<int>& num_last) {
  leda::node_array<bool>        mark(G,false);
  node_array_da<bool> da_mark(mark);
  leda::node v;
  int c1=0,c2=1; 
  node_array_da<int> nf(num_first); 
  node_array_da<int> nl(num_last);
  OutAdjIt ai(G);
  leda::stack<OutAdjIt> st;
  st.clear();
  forall_nodes(v,G) { 
    if (!mark[v]) {
      ai.update(v);
      GIT_dfs_num(st,da_mark,nf,nl,ai,c1,c2);   
    }
  }
  return c2;
}


inline int GIT_dfs_scc(leda::graph& G, leda::node_array<int>& compnum) {
  leda::node v,w; int i;
  int count = 0;
  leda::list<leda::node> S;
  leda::node_array<int> _dfs_num(G,0);
  leda::node_array<int> compl_num(G,0);
  leda::node_array<bool> reached(G,false);
  int max=GIT_dfs_numbering(G,_dfs_num,compl_num); 
  leda::node* V = new leda::node[max];
  for(i=max-1; i>0; i--) V[i]=nil;
  forall_nodes(v,G)  V[compl_num[v]] = v;
  G.rev_all_edges();
  for(i=max-1; i>0; i--) {
    if ( V[i]!=nil && !reached[V[i]] ) {
      S = GIT_dfs(G,V[i],reached);
      forall(w,S) compnum[w] = count;
      count++;
    }
  } 
  delete[] V;
  G.rev_all_edges();
  return count;
}



inline leda::list<leda::node> GIT_bfs(leda::graph& G, leda::node v, leda::node_array<bool>& reached) {
  leda::list<leda::node> list_nodes; list_nodes.clear();
  node_array_da<bool> da_mark(reached);
  OutAdjIt           ai(G,v);
  leda::queue<OutAdjIt>  st;
  GIT_bfs_list(st,da_mark,list_nodes,ai); 
  return list_nodes; }

 
inline bool GIT_toposort_indegree(leda::graph& G, leda::node_array<int>& ord) {
  leda::node_array<int> indegval(G,0);
  node_array_da<int> da_indegree(indegval);
  node_array_da<int> da_ord(ord);
  OutAdjIt       ai(G); 
  NodeIt  it_n(G);
  EdgeIt   it_e(G);
  leda::queue<OutAdjIt>     nl; nl.clear();
  GIT_indegree(da_indegree,ai,it_e);
  return GIT_toposort_count(da_indegree,da_ord,nl,ai,it_n); 
}

inline bool GIT_toposort(leda::graph& G, leda::node_array<int>& ord) {
  leda::node_array<int> indegval(G,0);
  node_array_da<int> da_indegree(indegval);
  node_array_da<int> da_ord(ord);
  OutAdjIt       ai(G);
  NodeIt   it_n(G);
  leda::queue<OutAdjIt>  nl; nl.clear();
  leda::node v;
  forall_nodes(v,G) { 
    ai.update(v);
    set(da_indegree,ai,indeg(v)); }
  return GIT_toposort_count(da_indegree,da_ord,nl,ai,it_n); 
}
#if LEDA_ROOT_INCL_ID == 600216
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

//LEDA_GRAPHIT_H
