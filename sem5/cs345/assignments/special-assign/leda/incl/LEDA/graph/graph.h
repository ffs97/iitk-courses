/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GRAPH_H
#define LEDA_GRAPH_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600222
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// graph (base for all graph types)
//
// last modified: april    1998 (embedded graph)
//                august   1998 (hide_node)
//                november 1998 (events)
//                october  1999 (start_del_node_event)
//
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>
#include <LEDA/numbers/vector.h>
#include <LEDA/core/map.h>
#include <LEDA/system/event.h>

#include <LEDA/graph/graph_objects.h>
#include <LEDA/graph/graph_map.h>
#include <LEDA/graph/graph_register.h>
#include <LEDA/graph/graph_cat_enum.h>



#if defined(WORD_LENGTH_64)
#define MASK_00 0xfffffffffffffffc
#define MASK_01 0xfffffffffffffffd
#else
#define MASK_00 0xfffffffc
#define MASK_01 0xfffffffd
#endif


LEDA_BEGIN_NAMESPACE

class __exportC ugraph;
class __exportC planar_map;
//class __exportC GraphWin;
class __exportC graph_undo;


/*{\Manpage {graph} {} {Graphs} }*/


class __exportC graph /* : public g_register<graph> */ {

/*{\Mdefinition

An instance $G$ of the data type $graph$ consists of a list $V$  of nodes
and a list $E$  of edges ($node$ and $edge$ are item types). 
Distinct graph have disjoint node and edge lists. 
The value of a variable of type node is either the node of some graph, or the
special value $nil$ (which is distinct from all nodes), or is undefined 
(before the first assignment to the variable). A corresponding statement is 
true for the variables of type edge.

A graph with empty node list is called {\em empty}.   
A pair of nodes $(v,w) \in V\times V$ is associated with every 
edge $e\in E$; $v$ is called the {\em source} of $e$ and $w$ is called the 
{\em target} of $e$, and $v$ and $w$ are called {\em endpoints} of $e$.  
The edge $e$ is said to be {\em incident} to its endpoints.

A graph is either {\em directed} or {\em undirected}. The difference between
directed and undirected graph is the way the edges incident to a node 
are stored and how the concept {\em adjacent} is defined.

In directed graph two lists of edges are associated with every node $v$: 
|adj_edges(v)| = $\{ e \in E\ \Lvert v = source(e) \}$, 
i.e., the list of edges starting in $v$, and 
|in_edges(v)| = $\{ e \in E\ \Lvert v = target(e) \}$, i.e., the list of
edges ending in $v$.  The list $adj\_edges(v)$ is called the adjacency list 
of node $v$ and the edges in $adj\_edges(v)$ are called the edges
{\em adjacent} to node $v$. For directed graph we often use $out\_edges(v)$ 
as a synonym for $adj\_edges(v)$.

In undirected graph only the list $adj\_edges(v)$ is defined
for every every node $v$. Here it contains all edges incident to $v$, i.e., 
|adj_edges(v)| = $\{ e \in E\ \Lvert v \in \{ source(e),target(e) \}\}$.
An undirected graph may not contain selfloops, i.e., it may not contain an 
edge whose source is equal to its target.

In a directed graph an edge is adjacent to its source and in an undirected 
graph it is adjacent to its source and target. In a directed graph a node $w$ 
is adjacent to a node $v$ if there is an edge $(v,w) \in E$; in an undirected 
graph $w$ is adjacent to $v$ if there is an edge $(v,w)$ or $(w,v)$ in the 
graph. 

A directed graph can be made undirected and vice versa:
$G$.make\_undirected() makes the directed graph $G$ undirected by 
appending for each node $v$ the list $in\_edges(v)$ to the list 
$adj\_edges(v)$ (removing selfloops).  Conversely, $G$.make\_directed() 
makes the undirected graph $G$ directed by splitting for
each node $v$ the list $adj\_edges(v)$ into the lists $out\_edges(v)$ and
$in\_edges(v)$. Note that these two operations are not exactly inverse to 
each other.
The data type $ugraph$ (cf. section \ref{Undirected Graphs})  can only
represent undirected graph. 
\smallskip
 
{\bf Reversal Information, Maps and Faces}

The reversal information of an edge $e$ is accessed through
|G.reversal(e)|, it has type |edge| and may or may not be defined
(|=nil|).  Assume that |G.reversal(e)| is defined and let $e' =
|G.reversal(e)|$. Then $e = (v,w)$ and $e' = (w,v)$ for some nodes $v$ and 
$w$, $|G.reversal|(e')$ is defined and $e = |G.reversal|(e')$. In addtion,
$e \not= e'$. In other words, |reversal| deserves its name.

We call a directed graph \emph{bidirected} if the reversal information can
be properly defined for all edges in $G$, resp. if there exists a bijective function
$|rev| : E \rightarrow E$ with the properties of |reversal|
as described above and we call a bidirected graph a \emph{map} 
if all edges have their reversal information defined. Maps are the data 
structure of choice for embedded graph. For an edge $e$ of a map $G$ let 
|face_cycle_succ(e) = cyclic_adj_pred(reversal(e))| and consider the sequence 
$e$, |face_cycle_succ(e)|, |face_cycle_succ(face_cycle_succ(e))|, \ldots\ . The 
first edge to repeat in this sequence is $e$ (why?) and the set of edges 
appearing in this sequence is called the \emph{face cycle} containing $e$. 
Each edge is contained in some face cycle and face cycles are pairwise 
disjoint. Let $f$ be the number of face cycles, $n$ be the number of (non-isolated) 
nodes, $m$ be the number of edges, and let $c$ be the number of (non-singleton) 
connected components. 
Then $g = (m/2 - n - f)/2 + c$ is called the \emph{genus} of the map 
\cite{Whi73} (note that $m/2$ is the number of edges in the underlying 
undirected graph). The genus is zero if and only if the map is planar, i.e., 
there is an embedding of $G$ into the plane such that for every node $v$ the 
counter-clockwise ordering of the edges around $v$ agrees with the cyclic 
ordering of $v$'s adjacency list. (In order to check whether a map is planar,
you may use the function |Is_Plane_Map()| in \ref{Miscellaneous Graph Functions}.) 

If a graph $G$ is a map the faces of $G$ can be constructed explicitly
by |G.compute_faces()|. Afterwards, the faces of $G$ can be traversed
by different iterators, e.g., |forall_faces(f,G)| iterates over
all faces , |forall_adj_faces(v)| iterates over all faces adjacent 
to node $v$. By using face maps or arrays (data types |face_map|
and |face_array|) additional information can be associated with
the faces of a graph. Note that any update operation performed on
$G$ invalidates the list of faces. See the section on face operations 
for a complete list of available operations for faces. 

}*/


friend class __exportC ugraph;
friend class __exportC planar_map;
friend class __exportC GraphWin;
friend class __exportC graph_undo;


public:

enum { static_tag = 0, dynamic_tag = 1 };

enum { n_base_sz = 1, e_base_sz = 1 };

typedef graph category_tag;

typedef node_struct* node;
typedef edge_struct* edge;
typedef face_struct* face;


g_register<graph> g_regist;

list_item register_map(graph_map<graph>* p, int& slot_i, bool use_slots=false)
{ return g_regist.register_map(p,slot_i,use_slots); }

void unregister_map(graph_map<graph>* p)
{ g_regist.unregister_map(p); }

list<graph_map<graph>*> get_map_list(int i )
{ return g_regist.get_map_list(i); }


private:

graph_node_list v_list;   // list of all nodes 
graph_node_list v_free;   // list of free nodes
graph_node_list v_hide;   // list of hidden nodes

graph_edge_list e_list;   // list of all edges
graph_edge_list e_free;   // list of free edges
graph_edge_list e_hide;   // list of hidden edges

graph_face_list f_list;   // list of all faces
graph_face_list f_free;   // list of free faces

list<node> v_list_tmp;  // temporary list of nodes
list<edge> e_list_tmp;  // temporary list of edges
list<face> f_list_tmp;  // temporary list of faces

int max_n_index;     // maximal node index 
int max_e_index;     // maximal edge index
int max_f_index;     // maximal face index

bool undirected;

int  data_sz[3];   // number of additional node/edge/face data slots


list<graph_map<graph>*> io_map_list[3]; // I/O maps

graph_map<graph>* FaceOf;


protected:

bool handlers_enabled;   // virtual pre- and post-handlers are called only
                         // if this flag is true

private:

node add_node(GenPtr,node=0);
edge add_edge(node,node,GenPtr);
face add_face(GenPtr);

void dealloc_node(node);
void dealloc_edge(edge);
void dealloc_face(face);

face new_face(GenPtr);
face new_face();

void del_face(face);

void ins_adj_edge(edge, node, edge, node, edge, int d1, int d2);  
void del_adj_edge(edge,node,node);
void copy_graph(const graph&);


// virtual functions defined in parameterized graph (GRAPH<vtype,etype>)

virtual void init_node_entry(GenPtr&) const {}
virtual void init_edge_entry(GenPtr&) const {}
virtual void init_face_entry(GenPtr&) const {}

virtual void copy_node_entry(GenPtr&) const {}
virtual void copy_edge_entry(GenPtr&) const {}
virtual void copy_face_entry(GenPtr&) const {}

virtual void clear_node_entry(GenPtr&) const {}
virtual void clear_edge_entry(GenPtr&) const {}
virtual void clear_face_entry(GenPtr&) const {}

virtual void read_node_entry(istream&, GenPtr&) {}
virtual void read_edge_entry(istream&, GenPtr&) {}
virtual void read_face_entry(istream&, GenPtr&) {}

virtual void write_node_entry(ostream&, GenPtr&) const {}
virtual void write_edge_entry(ostream&, GenPtr&) const {}
virtual void write_face_entry(ostream&, GenPtr&) const {}


void write_maps(ostream& O, const list<graph_map<graph>*>&, 
                            const list<graph_map<graph>*>&) const;

void read_maps(istream& I,  const list<graph_map<graph>*>&, 
                            const list<graph_map<graph>*>&) const;


protected:

graph*     parent;

// not used
graph_map<graph>* node_data_map;
graph_map<graph>* edge_data_map;


// virtual functions called before and after update operations

// node handler                                // called
virtual void pre_new_node_handler();           // before inserting a node
virtual void post_new_node_handler(node);      // after inserting node v
virtual void pre_del_node_handler(node);       // before deleting node v 
virtual void post_del_node_handler();          // after deleting a node

virtual void start_del_node_handler(node);     

virtual void pre_hide_node_handler(node);      // before hiding node v
virtual void post_hide_node_handler(node);     // after hiding node v
virtual void pre_restore_node_handler(node);   // before restoring node v
virtual void post_restore_node_handler(node);  // after restoring node v

 
// edge handler
virtual void pre_new_edge_handler(node, node); // before creating (v,w)
virtual void post_new_edge_handler(edge);      // after insertion of e

virtual void pre_del_edge_handler(edge);       // before deleteing edge e
virtual void post_del_edge_handler(node, node);// after deletion of (v,w)

virtual void pre_move_edge_handler(edge,node,node);  // before moving e to (v,w)
virtual void post_move_edge_handler(edge,node,node); // after moved e from (v,w)

virtual void pre_hide_edge_handler(edge);      // before hiding edge e
virtual void post_hide_edge_handler(edge);     // after hiding edge e
virtual void pre_restore_edge_handler(edge);   // before restoring edge e
virtual void post_restore_edge_handler(edge);  // after restoring edge e

virtual void pre_join_from_graph_handler(graph&);  
virtual void post_join_from_graph_handler(graph&,const list<node>&);  
  
virtual void pre_join_to_graph_handler(graph&);  
virtual void post_join_to_graph_handler(graph&,const list<node>&);  

// global handler
virtual void pre_clear_handler();              // before deleting graph
virtual void post_clear_handler();             // after deleting graph
 


void copy_all_entries() const;
void clear_all_entries() const;


public:

virtual vector get_position(node) { return vector(0); }

// events

void   enable_events();
void   disable_events();


static 
  EVENT1<graph&>                          create_event;

  EVENT1<graph&>                          destroy_event;

  EVENT1<graph&>                          new_node_pre_event;
  EVENT2<graph&,node>                     new_node_post_event;

  EVENT3<graph&,node,node>                new_edge_pre_event;
  EVENT2<graph&,edge>                     new_edge_post_event;

  EVENT2<graph&,node>                     del_node_start_event;
  EVENT2<graph&,node>                     del_node_pre_event;
  EVENT1<graph&>                          del_node_post_event;

  EVENT2<graph&,edge>                     del_edge_pre_event;
  EVENT3<graph&,node,node>                del_edge_post_event;

  EVENT4<graph&,edge,node,node>           move_edge_pre_event;
  EVENT4<graph&,edge,node,node>           move_edge_post_event;

  EVENT2<graph&,edge>                     hide_edge_pre_event;
  EVENT2<graph&,edge>                     hide_edge_post_event;

  EVENT2<graph&,edge>                     restore_edge_pre_event;
  EVENT2<graph&,edge>                     restore_edge_post_event;

  EVENT1<graph&>                          clear_pre_event;
  EVENT1<graph&>                          clear_post_event;

  EVENT2<graph&,graph&>                   join_to_pre_event;
  EVENT3<graph&,graph&,const list<node>&> join_to_post_event;

  EVENT2<graph&,graph&>                   join_from_pre_event;
  EVENT3<graph&,graph&,const list<node>&> join_from_post_event;



virtual int cmp_node_entry(node, node) const { return 0; }
virtual int cmp_edge_entry(edge, edge) const { return 0; }


virtual const char* node_type() const { return "void"; }
virtual const char* edge_type() const { return "void"; }
virtual const char* face_type() const { return "void"; }


string get_node_entry_string(node) const;
string get_edge_entry_string(edge) const;

void   set_node_entry(node, string);
void   set_edge_entry(edge, string);



/*{\Mcreation G }*/
graph();
/*{\Mcreate  creates an object $G$ of type $graph$ and initializes it to 
             the empty directed graph. }*/

graph(int n_slots, int e_slots);
/*{\Mcreate  this constructor specifies the numbers of free data slots in
             the nodes and edges of $G$ that can be used for storing the 
             entries of node and edge arrays. See also the description
             of the  |use_node_data()| and |use_edge_data()| operations
             in \ref{Node Arrays} and \ref{Edge Arrays}. }*/



graph(const graph&);

graph& operator=(const graph&); 

virtual ~graph();


/*
//subgraph constructors
graph(graph&, const list<node>&, const list<edge>&);
graph(graph&, const list<edge>&);
*/

   graph* super()        const;

   int max_node_index()  const;
   int max_edge_index()  const;
   int max_face_index()  const;

   int  space() const;


/*{\Moperations 1.9 4.8 }*/

void set_node_bound(int n);
void set_edge_bound(int m);

void init(int n, int m)
{ clear(); set_node_bound(n); set_edge_bound(m); }
/*{\Mop   this operation has to be called for semi-dynamic graph
          (if compiled with |-DGRAPH_REP=2|) immediately after the constructor
          to specify upper bounds $n$ and $m$ for the number of nodes and
          edges respectively.
          This operation has no effect if called for the (fully-dynamic) 
          standard graph representation. }*/


/*{\Mtext       
\bigskip
{\bf a) Access operations} }*/

   int  outdeg(node v)    const;
/*{\Mop     returns the number of edges adjacent to node $v$ 
            ($\Lvert${\it adj\_edges}$(v)\Lvert$). }*/


   int  indeg(node v)     const;
/*{\Mop     returns the number of edges ending at $v$ 
            ($\Lvert${\it in\_edges}$(v)\Lvert$)
            if |\Mvar| is directed and zero if |\Mvar| is undirected). }*/

   int  degree(node v)    const;
/*{\Mop     returns  $outdeg(v) + indeg(v)$. }*/


   node source(edge e)    const;
/*{\Mop     returns the source node of edge $e$.}*/

// for compatibility with static_fgraph
   node hidden_source(edge e) const { return e->terminal(0); }

   node target(edge e)    const;
/*{\Mop     returns the target node of edge $e$.}*/

   node opposite(node v, edge e) const;
/*{\Mop     returns $target(e)$ if $v = source(e)$ and $source(e)$ 
            otherwise. }*/

   node opposite(edge e, node v) const;
/*{\Mop     same as above. }*/

   int  number_of_nodes() const;
/*{\Mop     returns the number of nodes in $G$. }*/

   int  number_of_edges() const;
/*{\Mop     returns the number of edges in $G$. }*/


const list<node>& all_nodes()  const;
/*{\Mop     returns the list $V$ of all nodes of $G$. }*/

const list<edge>& all_edges()  const;
/*{\Mop     returns the list $E$ of all edges of $G$. }*/


  list<edge> adj_edges(node v) const;
/*{\Mop     returns $adj\_edges(v)$. }*/

   list<edge> out_edges(node v) const;
/*{\Mop     returns $adj\_edges(v)$ if |\Mvar| is directed and
            the empty list otherwise.}*/

   list<edge> in_edges(node v) const;
/*{\Mop     returns $in\_edges(v)$ if |\Mvar| is directed and
            the empty list otherwise.}*/


   list<node> adj_nodes(node v) const;
/*{\Mop     returns the list of all nodes adjacent to $v$. }*/

   node first_node()      const;
/*{\Mop       returns the first node in $V$. }*/

   node last_node()       const;
/*{\Mop       returns the last node in $V$. }*/

   node choose_node()     const;
/*{\Mop       returns a random node of $G$ (nil if $G$ is empty). }*/

   node succ_node(node v) const;
/*{\Mop       returns the successor of node $v$ in $V$\\
              (nil if it does not exist). }*/

   node pred_node(node v) const;
/*{\Mop       returns the predecessor of node $v$ in $V$\\
              (nil if it does not exist). }*/


   edge first_edge()      const;
/*{\Mop       returns the first edge in $E$. }*/

   edge last_edge()       const;
/*{\Mop       returns the last edge in $E$. }*/

   edge choose_edge()     const;
/*{\Mop       returns a random edge of $G$ (nil if $G$ is empty). }*/

   edge succ_edge(edge e) const;
/*{\Mop       returns the successor of edge $e$ in $E$\\
              (nil if it does not exist). }*/

   edge pred_edge(edge e) const;
/*{\Mop       returns the predecessor of edge $e$ in $E$\\
              (nil if it does not exist). }*/


   edge first_adj_edge(node v) const;
/*{\Mop       returns the first edge in the adjacency list of $v$\\
              (nil if this list is empty). }*/


   edge last_adj_edge(node v)  const;
/*{\Mop       returns the last edge in the adjacency list of $v$\\
              (nil if this list is empty). }*/

   edge adj_succ(edge e)  const;
/*{\Mop       returns the successor of edge $e$ in the adjacency list of 
              node $source(e)$ (nil if it does not exist). }*/

   edge adj_pred(edge e)  const;
/*{\Mop       returns the predecessor of edge $e$ in the adjacency list of 
              node $source(e)$ (nil if it does not exist). }*/

   edge cyclic_adj_succ(edge e) const;
/*{\Mop       returns the cyclic successor of edge $e$ in the adjacency list
              of node $source(e)$. }*/

   edge cyclic_adj_pred(edge e) const;
/*{\Mop       returns the cyclic predecessor of edge $e$ in the adjacency list
              of node $source(e)$. }*/


   edge first_out_edge(node v) const;
   edge last_out_edge(node v)  const;
   edge stop_out_edge(node v)  const;

   edge next_out_edge(edge e)  const;

   edge out_succ(edge e)       const;
   edge out_pred(edge e)       const;


   edge first_in_edge(node v)  const;
/*{\Mop       returns the first edge of $in\_edges(v)$\\
              (nil if this list is empty). }*/

   edge last_in_edge(node v)   const;
/*{\Mop       returns the last edge of $in\_edges(v)$\\
              (nil if this list is empty). }*/

   edge in_succ(edge e)  const;
/*{\Mop       returns the successor of edge $e$ in $in\_edges(target(e))$
              (nil if it does not exist). }*/

   edge in_pred(edge e)  const;
/*{\Mop       returns the predecessor of edge $e$ in $in\_edges(target(e))$ 
              (nil if it does not exist). }*/

   edge cyclic_in_succ(edge e) const;
/*{\Mop       returns the cyclic successor of edge $e$ in 
              $in\_edges(target(e))$ (nil if it does not exist). }*/

   edge cyclic_in_pred(edge e) const;
/*{\Mop       returns the cyclic predecessor of edge $e$ in 
              $in\_edges(target(e))$ (nil if it does not exist). }*/


   bool is_directed() const { return !undirected; }
/*{\Mop       returns true iff $G$  is directed. }*/

   bool is_undirected() const { return undirected; }
/*{\Mop       returns true iff $G$  is undirected. }*/

   bool empty() const { return v_list.empty(); }
/*{\Mop       returns true iff $G$  is empty. }*/


node   next_node(node v) const;
edge   next_edge(edge e) const;
face   next_face(face f) const;

node   prev_node(node v) const;
edge   prev_edge(edge e) const;
face   prev_face(face f) const;

node   stop_node() const { return 0; }
edge   stop_edge() const { return 0; }
face   stop_face() const { return 0; }

int    index(node v) const { return v->id(); }
int    index(edge e) const { return e->id(); }
int    index(face f) const { return f->id(); }

bool   member(node v) const { return v->owner() == this; }
bool   member(edge e) const { return e->owner() == this; }
bool   member(face f) const { return f->owner() == this; }

protected:

   GenPtr& entry(node v) const;
   GenPtr& entry(edge e) const;
   GenPtr& entry(face f) const;


   const GenPtr& inf(node v) const;
   const GenPtr& inf(edge e) const;
   const GenPtr& inf(face f) const;

   node new_node(GenPtr,node=0,int d=leda::behind);

   edge new_edge(node, node, GenPtr);
   edge new_edge(edge, node, GenPtr, int d);
   edge new_edge(node, edge, GenPtr, int d);
   edge new_edge(edge, edge, GenPtr, int d1, int d2);

   // undirected graph
   edge new_edge(node, edge, node, edge, GenPtr, int d1, int d2);  
   edge new_edge(node, edge, node, GenPtr, int d);  
   edge new_edge(node, node, edge, GenPtr, int d);  


   node split_edge(edge, GenPtr, edge&, edge&);

   void assign(node v,GenPtr x);
   void assign(edge e,GenPtr x);
   void assign(face f,GenPtr x);


//sort using the virtual cmp_node/edge_entry functions
void sort_nodes();
void sort_edges();


face& access_face(edge e)  const;


public:


/*{\Mtext 
\bigskip 
{\bf b) Update operations} }*/ 


   node new_node()
   { GenPtr x=0; init_node_entry(x); return graph::new_node(x); }
/*{\Mop        adds a new node to |\Mvar| and returns it. }*/

   node new_node(node u, int dir)
   { GenPtr x=0; init_node_entry(x); return graph::new_node(x,u,dir);}
/*{\Mop        adds a new node $v$ to |\Mvar| and returns it. $v$ is 
               inserted in front of (|dir=leda::before|) or behind 
               (|dir=leda::behind|) node $u$ into the list of all nodes. }*/


   edge new_edge(node v, node w) 
   { GenPtr x=0; init_edge_entry(x); return graph::new_edge(v,w,x);}

/*{\Mopl        adds a new edge $(v,w)$ to |\Mvar| by appending it to 
                $adj\_edges(v)$ and to $in\_edges(w)$ (if |\Mvar| is 
                directed) or $adj\_edges(w)$ (if |\Mvar| is undirected), 
                and returns it. }*/


   edge new_edge(edge e, node w, int dir=leda::behind) 
   { GenPtr x=0; init_edge_entry(x); return graph::new_edge(e,w,x,dir); }

/*{\Mopl       adds a new edge $x = (source(e),w)$ to |\Mvar|. $x$ is inserted
               in front of (|dir=leda::before|) or behind (|dir=leda::behind|) edge 
               $e$ into $adj\_edges(source(e))$ and appended to $in\_edges(w)$ 
               (if |\Mvar| is directed) or $adj\_edges(w)$ (if |\Mvar| is 
               undirected). Here |leda::before| and |leda::behind| are 
               predefined constants. The operation returns the new edge $x$.\\
               \precond $source(e) \ne w$ if |\Mvar| is undirected. }*/


   edge new_edge(node v, edge e, int dir=leda::behind) 
   { GenPtr x=0; init_edge_entry(x); return graph::new_edge(v,e,x,dir); }

/*{\Mopl       adds a new edge $x = (v,target(e))$ to |\Mvar|. $x$ is appended
               to $adj\_edges(v)$ and inserted in front of (|dir=leda::before|) or 
               behind (|dir=leda::behind|) edge $e$ into $in\_edges(target(e))$ 
               (if |\Mvar| is directed) or $adj\_edges(target(e))$ (if |\Mvar| 
               is undirected).
               The operation returns the new edge $x$.\\
               \precond $target(e) \ne v$ if |\Mvar| is undirected. }*/


   edge new_edge(edge e1, edge e2, int d1=leda::behind, int d2=leda::behind) 
   { GenPtr x=0; init_edge_entry(x); return graph::new_edge(e1,e2,x,d1,d2); }

/*{\Mopl       adds a new edge $x = (source(e1),target(e2))$ to |\Mvar|. $x$ is 
               inserted in front of (if |d1=leda::before|) or behind (if 
               |d1=leda::behind|) edge $e1$ into $adj\_edges(source(e1))$ and 
               in front of (if |d2=leda::before|) or behind (if |d2=leda::behind|) 
               edge $e2$ into $in\_edges(target(e2))$ (if |\Mvar| is directed) 
               or $adj\_edges(target(e2))$ (if |\Mvar| is undirected).
               The operation returns the new edge $x$.}*/


   node merge_nodes(node v1, node v2);
/*{\Mop   experimental. }*/

   node merge_nodes(edge e1, node v2);
/*{\Mop   experimental. }*/


   node split_edge(edge e, edge& e1, edge& e2)
   { GenPtr x=0; init_node_entry(x);
     return split_edge(e,x,e1,e2); }
/*{\Mop   experimental }*/


// hiding nodes and edges

   void hide_adj_edge(edge e);
   void restore_adj_edge(edge e);

   void hide_edge(edge e);
/*{\Mop        removes edge $e$ temporarily from |\Mvar| until restored
               by |\Mvar.restore_edge(e)|. }*/


   void hide_edges(const list<edge>& el) { edge e; forall(e, el) hide_edge(e); }
/*{\Mop        hides all edges in |el|. }*/

   bool is_hidden(edge e) const;
/*{\Mop        returns |true| if |e| is hidden and |false| otherwise. }*/

   list<edge> hidden_edges() const;
/*{\Mop        returns the list of all hidden edges of |\Mvar|. }*/

// iteration (forall_hidden_edges)
   edge first_hidden_edge()       const { return e_hide.head(); }
   edge next_hidden_edge(edge e)  const { return e_hide.succ(e); }
   edge stop_hidden_edge()        const { return 0; }


   void restore_edge(edge e);
/*{\Mop        restores $e$ by appending it to $adj\_edges(source(e))$
               and to $in\_edges(target(e))$ ($adj\_edges(target(e))$ if
               |\Mvar| is undirected). \precond $e$ is hidden and 
               neither $source(e)$ nor $target(e)$ is hidden. }*/

   void restore_edges(const list<edge>& el)
   { edge e; forall(e, el) restore_edge(e); }
/*{\Mop        restores all edges in |el|. }*/

   void restore_all_edges();
/*{\Mop        restores all hidden edges. }*/




   void hide_node(node v);
/*{\Mop        removes node $v$ temporarily from |\Mvar| until restored
               by |\Mvar.restore_node(v)|. All non-hidden edges in 
               $adj\_edges(v)$ and $in\_edges(v)$ are hidden too. }*/

   void hide_node(node v, list<edge>& h_edges);
/*{\Mop        as above, in addition, the list of leaving or entering edges
               which are hidden as a result of hiding $v$ are appended to
               |h_edges|. }*/


   bool is_hidden(node v) const;
/*{\Mop        returns |true| if |v| is hidden and |false| otherwise. }*/

   list<node> hidden_nodes() const;
/*{\Mop        returns the list of all hidden nodes of |\Mvar|. }*/


   void restore_node(node v);
/*{\Mop        restores $v$ by appending it to the list of all nodes.
               Note that no edge adjacent to $v$ that was hidden by 
               |\Mvar.hide_node(v)| is restored by this operation. }*/

   void restore_all_nodes();
/*{\Mop        restores all hidden nodes. }*/




   void del_node(node v);
/*{\Mop       deletes $v$ and all edges incident to $v$ from $G$. }*/

   void del_edge(edge e);
/*{\Mop       deletes the edge $e$ from $G$. }*/

   void del_nodes(const list<node>& L);
/*{\Mop       deletes all nodes in |L| from $G$. }*/

   void del_edges(const list<edge>& L);
/*{\Mop       deletes all edges in |L| from $G$. }*/

   void del_all_nodes(); 
/*{\Mop       deletes all nodes from $G$.  }*/

   void del_all_edges(); 
/*{\Mop       deletes all edges from $G$.  }*/

   void del_all_faces(); 
/*{\Mop       deletes all faces from $G$.  }*/

   void move_edge(edge e, node v, node w);
/*{\Mop       moves edge $e$ to source $v$ and target $w$ by appending it to
              $adj\_edges(v)$ and to $in\_edges(w)$ (if |\Mvar| is directed)
              or $adj\_edges(w))$ (if |\Mvar| is undirected). }*/

   void move_edge(edge e, edge e1, node w, int d=leda::behind);
/*{\Mop       moves edge $e$ to source $source(e1)$ and target $w$ by 
              inserting it in front of (if |d=leda::before|) or behind 
              (if |d=leda::behind|) 
              edge $e1$ into $adj\_edges(source(e1))$ and by appending
              it to $in\_edges(w)$ (if |\Mvar| is directed)
              or $adj\_edges(w))$ (if |\Mvar| is undirected). }*/

   void move_edge(edge e, node v, edge e2, int d=leda::behind);
/*{\Mop       moves edge $e$ to source $v$ and target $target(e2)$ by 
              appending it to $adj\_edges(v))$ and inserting it in front of 
              (if |d=leda::before|) or behind (if |d=leda::behind|) edge $e2$ 
              into $in\_edges(target(e2))$ (if |\Mvar| is directed) or 
              $adj\_edges(target(e2))$ (if |\Mvar| is undirected). }*/

   void move_edge(edge e, edge e1, edge e2, int d1=leda::behind, 
                                            int d2=leda::behind);
/*{\Mop       moves edge $e$ to source $source(e1)$ and target $target(e2)$ by 
              inserting it in front of (if |d1=leda::before|) or behind 
              (if |d1=leda::behind|) 
              edge $e1$ into $adj\_edges(source(e1))$ and in front of 
              (if |d2=leda::before|) or behind (if |d2=leda::behind|) edge $e2$ 
              into $in\_edges(target(e2))$ (if |\Mvar| is directed) or 
              $adj\_edges(target(e2))$ (if |\Mvar| is undirected). }*/


   edge rev_edge(edge e);
/*{\Mop       reverses $e$ (|move_edge(e,target(e),source(e))|). }*/

   void rev_all_edges();
/*{\Mop        reverses all edges of |\Mvar|. }*/

   graph& rev() { rev_all_edges(); return *this; }


void sort_nodes(int (*cmp)(const node&, const node&));
/*{\Mopl      the nodes of $G$ are sorted according to the
              ordering defined by the comparing function
              $cmp$. Subsequent executions of forall\_nodes
              step through the nodes in this order.
              (cf.~TOPSORT1 in section \ref{Graph and network algorithms}). }*/

void sort_nodes(const leda_cmp_base<node>& cmp);

/*{\Moptions nextwarning=no }*/
void sort_edges(int (*cmp)(const edge&, const edge&));
/*{\Mopl      the edges of $G$ and all adjacency lists
              are sorted according to the
              ordering defined by the comparing function
              $cmp$. Subsequent executions of forall\_edges
              step through the edges in this order.
              (cf.~TOPSORT1 in section \ref{Graph and network algorithms}). }*/

void sort_edges(const leda_cmp_base<edge>& cmp);

/*{\Moptions nextwarning=no }*/
void sort_nodes(const graph_map<graph>& A);
/*
void sort_nodes(const node_array<T>& A);
*/
/*{\Mopl      the nodes of $G$ are sorted according to the entries of
              node\_array $A$ (cf. section \ref{Node Arrays}).\\
              \precond $T$ must be numerical, i.e., number type |int, float, double,
              integer, rational or real|. }*/


/*{\Moptions nextwarning=no }*/
void sort_edges(const graph_map<graph>& A);
/*
void sort_edges(const edge_array<T>& A);
*/
/*{\Mopl      the edges of $G$ are sorted according to the entries of 
              edge\_array $A$ (cf. section \ref{Edge Arrays}).\\
              \precond $T$ must be numerical, i.e., number type |int, float, double,
              integer, rational or real|. }*/


void sort_nodes(const list<node>& vl);
/*{/Mopl the nodes of $G$ are sorted into the ordering given by $vl$.\\
         \precond  $vl$ contains exactly the nodes of $G$. }*/

void sort_edges(const list<edge>& el);
/*{/Mopl the edges of $G$ are sorted into the ordering given by $el$.\\
         \precond  $el$ contains exactly the edges of $G$. }*/



void bucket_sort_nodes(int l, int h, int (*ord)(const node&));
/*{\Mopl      sorts the nodes of $G$ using {\em bucket sort}\\
              \precond  $l \le ord(v) \le h$ for all nodes $v$. }*/

void bucket_sort_edges(int l, int h, int (*ord)(const edge&));
/*{\Mopl      sorts the edges of $G$ using {\em bucket sort}\\
              \precond  $l \le ord(e) \le h$ for all edges $e$. }*/


void bucket_sort_nodes(int (*ord)(const node&));
/*{\Mopl      same as |\Mvar.bucket_sort_nodes(l,h,ord| with $l$ ($h$) 
              equal to the minimal (maximal) value of $ord(v)$. }*/

void bucket_sort_edges(int (*ord)(const edge&));
/*{\Mopl      same as |\Mvar.bucket_sort_edges(l,h,ord| with $l$ ($h$)
              equal to the minimal (maximal) value of $ord(e)$. }*/



/*{\Moptions nextwarning=no }*/
void bucket_sort_nodes(const graph_map<graph>& A);
/*
void bucket_sort_nodes(const node_array<int>& A);
*/
/*{\Mopl      same as |\Mvar.bucket_sort_nodes(ord)| with
              $ord(v) = A[v]$ for all nodes $v$ of |\Mvar|. }*/


/*{\Moptions nextwarning=no }*/
void bucket_sort_edges(const graph_map<graph>& A);
/*
void bucket_sort_edges(const edge_array<int>& A);
*/
/*{\Mopl      same as |\Mvar.bucket_sort_edges(ord)| with
              $ord(e) = A[e]$ for all edges $e$ of |\Mvar|. }*/



void set_node_position(node v, node p);
/*{\Mop       moves node |v| in the list |V| of all nodes such that
              |p| becomes the predecessor of |v|. If |p = nil| then
              |v| is moved to the front of |V|. }*/
              

void set_edge_position(edge e, edge p);
/*{\Mop       moves edge |e| in the list |E| of all edges such that
              |p| becomes the predecessor of |e|. If |p = nil| then
              |e| is moved to the front of |E|. }*/
              

void permute_edges() 
{ list<edge> L = all_edges();
  L.permute();
  sort_edges(L);
}
/*{\Mopl      the edges of $G$ and all adjacency lists
              are randomly permuted. }*/


   list<edge> insert_reverse_edges();
/*{\Mop       for every edge $(v,w)$ in $G$ the reverse edge $(w,v)$ 
              is inserted into $G$. Returns the list of all inserted 
              edges. \\
	      Remark: the reversal information is not set by this function.}*/


   void make_undirected();
/*{\Mop       makes $G$  undirected by appending $in\_edges(v)$ to 
              $adj\_edges(v)$ for all nodes $v$. }*/

   void make_directed();
/*{\Mop       makes  $G$  directed by splitting $adj\_edges(v)$ into
              $out\_edges(v)$ and $in\_edges(v)$. }*/


virtual void clear();
/*{\Mop       makes $G$ the empty graph. }*/

   void join(graph& H);
/*{\Mop       merges $H$ into $G$ by moving all objects (nodes,edges, and 
              faces) from $H$ to $G$. $H$ is empty afterwards. }*/


 
/*{\Mtext
\bigskip
{\bf c) Reversal Edges and Maps} 
}*/

void  make_bidirected();
/*{\Mop  makes $G$ bidirected by inserting missing reversal
         edges. }*/

void  make_bidirected(list<edge>& R);
/*{\Mop  makes $G$ bidirected by inserting missing reversal
         edges. Appends all inserted edges to list $R$. }*/

bool is_bidirected() const;
/*{\Mop  returns true if every edge has a reversal 
         and false otherwise. }*/

bool make_map();
/*{\Mop  sets the reversal information of a maximal number of 
edges of |\Mvar|.
         Returns |true| if |\Mvar| is bidirected and |false| otherwise. }*/

void make_map(list<edge>& R);
/*{\Mop  makes |\Mvar| bidirected by inserting missing reversal edges and
         then turns it into a map setting the reversals for all edges. 
         Appends all inserted edges to list $R$. }*/

bool is_map() const;
/*{\Mop  tests whether $G$ is a map. }*/        


edge reversal(edge e) const { return e->reversal(); }
/*{\Mop returns the reversal information of edge $e$ 
        (|nil| if not defined).}*/

void set_reversal(edge e, edge r);
/*{\Mop makes |r| the reversal of |e| and vice versa. If the reversal
information of |e| was defined prior to the operation, say as |e'|, 
the reversal information of |e'| is set to nil. The same holds for
|r|.\\
\precond $e = (v,w)$ and $|r| = (w,v)$ for some nodes $v$ and $w$.}*/


edge face_cycle_succ(edge e) const;
/*{\Mop returns the cyclic adjacency predecessor of |reversal(e)|.\\
        \precond |reversal(e)| is defined.}*/


edge face_cycle_pred(edge e) const;
/*{\Mop returns the reversal of the cyclic adjacency successor $s$ of $e$.\\
        \precond |reversal(s)| is defined.}*/
 
edge split_map_edge(edge e);
/*{\Mop     splits edge $e=(v,w)$ and its reversal $r=(w,v)$
            into edges $(v,u)$, $(u,w)$, $(w,u)$, and $(u,v)$.
            Returns the edge $(u,w)$. }*/

edge new_map_edge(edge e1, edge e2);
/*{\Mop     inserts a new edge $e=(source(e1),source(e2)$ after $e1$ 
            into the adjacency list of $source(e1)$ and an edge $r$
            reversal to $e$ after $e2$ into the adjacency list of 
            $source(e2)$. }*/

list<edge> triangulate_map();
/*{\Mop    triangulates the map $G$ by inserting additional edges. The list 
           of inserted edges is returned.\\
           \precond $G$ must be connected.\\
           The algorithm (\cite{HU89}) has running time $O(\Labs{V}+\Labs{E})$.
}*/

void dual_map(graph& D) const;
/*{\Mop constructs the dual of $G$ in $D$. The algorithm has linear running
time.\\
\precond $G$ must be a map. 
}*/



/*{\Mtext
{\bf For backward compatibility}
}*/

edge reverse(edge e) const { return e->reversal(); }
/*{\Mop returns |reversal(e)| (historical). }*/

edge succ_face_edge(edge e) const;
/*{\Mop returns |face_cycle_succ(e)| (historical). }*/

edge next_face_edge(edge e) const { return face_cycle_succ(e); }
/*{\Mop returns |face_cycle_succ(e)| (historical). }*/

edge pred_face_edge(edge e) const;
/*{\Mop returns |face_cycle_pred(e)| (historical). }*/

/*{\Mtext
\bigskip
{\bf d) Faces and Planar Maps}
}*/

void compute_faces();
/*{\Mop constructs the list of face cycles of |\Mvar|.\\
        \precond |\Mvar| is a map. }*/

face       face_of(edge e)  const { return access_face(e); }
/*{\Mop      returns the face of |\Mvar| to the left of edge $e$.}*/

face       adj_face(edge e)  const { return face_of(e); }
/*{\Mop      returns |\Mvar.face_of(e)|. }*/

void print_face(face f) const;
/*{\Mop     prints face $f$. }*/

int number_of_faces()       const { return f_list.length(); }
/*{\Mop      returns the number of faces of |\Mvar|.}*/

face first_face()      const;
/*{\Mop       returns the first face of $G$.\\
              (nil if empty). }*/

face last_face()      const;
/*{\Mop       returns the last face of $G$. }*/

face choose_face()     const;
/*{\Mop       returns a random face of $G$ (nil if $G$ is empty). }*/


face succ_face(face f) const;
/*{\Mop       returns the successor of face $f$ in the face list of $G$\\
              (nil if it does not exist). }*/

face pred_face(face f) const;
/*{\Mop       returns the predecessor of face $f$ in the face list of $G$\\
              (nil if it does not exist). }*/

const list<face>& all_faces() const;
/*{\Mop      returns the list of all faces of |\Mvar|.}*/


list<face> adj_faces(node v)   const;
/*{\Mop      returns the list of all faces of |\Mvar| adjacent
             to node $v$ in counter-clockwise order.}*/


list<node> adj_nodes(face f)   const;
/*{\Mop      returns the list of all nodes of |\Mvar| adjacent
             to face $f$ in counter-clockwise order.}*/


list<edge> adj_edges(face)   const;
/*{\Mop      returns the list of all edges of |\Mvar| bounding
             face $f$ in counter-clockwise order.}*/

int        size(face f)  const { return f->size(); }
/*{\Mop      returns the number of edges bounding face $f$. }*/


edge first_face_edge(face f) const { return f->first_edge(); }
/*{\Mop      returns the first edge of face $f$ in |\Mvar|. }*/


edge split_face(edge e1, edge e2);
/*{\Mopl    inserts the edge $e=(source(e_1),source(e_2))$
            and its reversal into $G$ and returns $e$.\\
            \precond $e_1$ and $e_2$ are bounding the same face $F$.\\
            The operation splits $F$ into two new faces. }*/

face join_faces(edge e);
/*{\Mopl    deletes edge $e$ and its reversal $r$ and updates the list of
faces accordingly. The function returns a face that is affected by the operations (see the LEDA book for details).  }*/

void make_planar_map();
/*{\Mop  makes |\Mvar| a planar map by reordering the edges such that
         for every node $v$ the ordering of the edges in the adjacency list of 
         $v$ corresponds to the counter-clockwise ordering of these edges 
         around $v$ for some planar embedding of |\Mvar| and constructs the
         list of faces.\\
         \precond |\Mvar| is a planar bidirected graph (map). }*/


list<edge> triangulate_planar_map()
{ list<edge> L = triangulate_map();
  compute_faces();
  return L;
 }
/*{\Mopl   triangulates planar map $G$ and 
           recomputes its list of faces }*/


// used by iterators

edge next_face_edge(edge e, face F)  const  
{ e = succ_face_edge(e);
  return (e == F->first_edge()) ? nil : e;
}




/*{\Mtext       
\bigskip
{\bf e) Operations for undirected graphs} }*/

edge new_edge(node v, edge e1, node w, edge e2, int d1=leda::behind, 
                                                int d2=leda::behind)  
{ GenPtr x=0; init_edge_entry(x); return graph::new_edge(v,e1,w,e2,x,d1,d2); }

/*{\Mopl       adds a new edge $(v,w)$ to |\Mvar| by inserting it in front of (if 
               |d1=leda::before|) or behind (if |d1=leda::behind|) edge $e1$ into 
               $adj\_edges(v)$ and in front of (if |d2=leda::before|) or behind (if 
               |d2=leda::behind|) edge $e2$ into $adj\_edges(w)$, and returns
               it.\\
               \precond  $e1$ is incident to $v$ and $e2$ is incident to 
               $w$ and $v \ne w$. }*/


edge new_edge(node v, edge e, node w,int d=leda::behind)  
{ GenPtr x=0; init_edge_entry(x); return graph::new_edge(v,e,w,x,d); }

/*{\Mopl       adds a new edge $(v,w)$ to |\Mvar| by inserting it in front of (if 
               |d=leda::before|) or behind (if |d=leda::behind|) edge $e$ into 
               $adj\_edges(v)$ and appending it to $adj\_edges(w)$, and 
               returns it.\\
               \precond  $e$ is incident to $v$  and $v \ne w$. }*/

edge new_edge(node v, node w, edge e, int d=leda::behind)  
{ GenPtr x=0; init_edge_entry(x); return graph::new_edge(v,e,w,x,d); }

/*{\Mopl       adds a new edge $(v,w)$ to |\Mvar| by appending it to
               to $adj\_edges(v)$, and by inserting it in front of (if 
               |d=leda::before|) or behind (if |d=leda::behind|) edge 
               $e$ into $adj\_edges(w)$, and returns it.\\
               \precond  $e$ is incident to $w$  and $v \ne w$. }*/




edge adj_succ(edge e, node v) const;
/*{\Mop        returns the successor of edge $e$ in the adjacency list of 
               $v$.\\
               \precond  $e$ is incident to $v$. }*/


edge adj_pred(edge e, node v) const;
/*{\Mop        returns the predecessor of edge $e$ in the adjacency list of 
               $v$.\\
               \precond  $e$ is incident to $v$. }*/


edge cyclic_adj_succ(edge e, node v)  const;
/*{\Mopl      returns the cyclic successor of edge $e$ in the adjacency
              list of $v$.\\
              \precond  $e$ is incident to $v$. }*/


edge cyclic_adj_pred(edge e, node v) const;
/*{\Mopl       returns the cyclic predecessor of edge $e$ in the adjacency 
               list of $v$.\\
               \precond  $e$ is incident to $v$. }*/




/*{\Mtext
\bigskip
{\bf f) I/O Operations} }*/


void add_io_map(graph_map<graph>& m);
void clear_io_maps(); 


void write(ostream& O = cout) const;
/*{\Mopl      writes $G$ to the output stream $O$.}*/

void write(string s) const;
/*{\Mop       writes $G$ to the file with name $s$.}*/

int  read(istream& I = cin);
/*{\Mopl      reads a graph from the input stream $I$ and assigns
              it to |\Mvar|.}*/

int  read(string s);
/*{\Mop       reads a graph from the file with name $s$ and assigns
              it to |\Mvar|. Returns $1$ if file |s| does not exist,
              $2$ if the edge and node parameter types of |*this| and
              the graph in the file |s| do not match, $3$ if file |s|
              does not contain a graph, and $0$ otherwise.}*/


bool write_gml(ostream& O = cout,
               void (*node_cb)(ostream&,const graph*,const node)=0,
               void (*edge_cb)(ostream&,const graph*,const edge)=0) const;
/*{\Mopl      writes $G$ to the output stream $O$ in GML format (\cite{HI97}).
              If |node_cb| is not equal to 0, it is called while writing
              a node $v$ with output stream |O|, the graph and |v| as parameters.
              It can be used to write additional user defined node data.
              The output should conform with GML format (see manual
              page |gml_graph|). |edge_cb| is called while writing edges.
              If the operation fails, $false$ is returned.}*/

bool write_gml(string s,
               void (*node_cb)(ostream&,const graph*,const node)=0,
               void (*edge_cb)(ostream&,const graph*,const edge)=0) const;
/*{\Mop       writes $G$ to the file with name $s$ in GML format. For a 
              description of |node_cb| and |edge_cb|, see above. If
              the operation fails, $false$ is returned.}*/

bool read_gml(string s);
/*{\Mop       reads a graph in GML format from the file with name $s$
              and assigns it to |\Mvar|. Returns $true$ if the graph is 
			  successfully read; otherwise $false$ is returned." }*/

bool read_gml(istream& I = cin);
/*{\Mop       reads a graph in GML format from the input stream $I$
              and assigns it to |\Mvar|. Returns $true$ if the graph is 
			  successfully read; otherwise $false$ is returned."}*/


void print_node(node v, ostream& O = cout)  const;
/*{\Mopl      prints node $v$ on the output stream $O$. }*/


virtual void print_edge(edge e, ostream& O = cout) const;
/*{\Mopl      prints  edge $e$ on the output stream $O$. If |\Mvar|
              is directed $e$ is represented by an arrow 
              pointing from source to target. If |\Mvar| is undirected
              $e$ is printed as an undirected line segment. }*/



void print(string s, ostream& O = cout) const;
/*{\Mopl      prints $G$ with header line $s$ on the output 
              stream $O$. }*/

void print(ostream& O = cout) const { print("",O); }
/*{\Mopl      prints $G$ on the output stream $O$. }*/


void print_node1(node v, ostream& O = cout) const;
void print_edge1(edge e, ostream& O = cout) const;

void print1(string s, ostream& O = cout) const;
void print1(ostream& O = cout) const { print1("",O); }


static int category() { return leda_graph_category; }


// for compatibility with static graph

void finish_construction() {
#if GRAPH_REP == 2
sort_edges(all_edges());
#endif
}


struct incident_edge {

  typedef size_t address;

  address xx;

  typedef graph::edge edge;

  bool out() const { return xx & 0x1; }
  bool in()  const { return !out(); }

  incident_edge(address x=0) : xx(x) {}

  incident_edge(edge e, bool out)
  { xx = (address)e + (address)out; }

  incident_edge reverse() const { return incident_edge(xx ^ 0x1); }

  edge get_edge() const { return edge(xx & MASK_00); }
  operator edge() const { return get_edge(); }

  void mark()   { xx |= 0x2; }
  void unmark() { xx &= MASK_01; }
  bool is_marked() { return (xx & 0x2) != 0; }

};

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


};

typedef graph::node node;
typedef graph::edge edge;
typedef graph::face face;


inline void First_Inc_Edge(graph::incident_edge& x, node v) 
{ edge e = v->first_out_edge();
  if (e != v->stop_out_edge())
    x = graph::incident_edge(e,true);
  else
    x = graph::incident_edge(v->first_in_edge(),false);
}

inline void Next_Inc_Edge(graph::incident_edge& x, node v)
{ edge e = x;
  if (x.out())
    { e = e->next_out_edge();
      if (e == v->stop_out_edge())
        x = graph::incident_edge(v->first_in_edge(),false);
      else
        x = graph::incident_edge(e,true);
     }
  else
   x = graph::incident_edge(e->next_in_edge(),false);
}


inline graph::incident_edge Next_Cyclic_Inc_Edge(graph::incident_edge x,
                                                 graph::incident_edge e_stop, 
                                                 node v)
{ Next_Inc_Edge(x,v);
  if (x == 0) First_Inc_Edge(x,v);
  return (x == e_stop) ? graph::incident_edge(0) : x;
 }




/*{\Mtext
\bigskip
{\bf g) Non-Member Functions} 
}*/

inline node source(edge e) { return e->terminal(0); }
/*{\Mfuncl    returns the source node of edge $e$.   }*/


inline node target(edge e) { return e->terminal(1); }
/*{\Mfuncl    returns the target node of edge $e$.   }*/

inline graph* graph_of(node v) { return v->owner(); }
/*{\Mfuncl    returns a pointer to the graph that $v$ belongs to. }*/

inline graph* graph_of(edge e) { return e->owner(); }
/*{\Mfuncl    returns a pointer to the graph that $e$ belongs to. }*/

inline graph* graph_of(face f) { return f->owner(); }
/*{\Mfuncl    returns a pointer to the graph that $f$ belongs to. }*/


inline face face_of(edge e) { return graph_of(e)->face_of(e); }
/*{\Mfuncl    returns the face of edge $e$. }*/


inline int    outdeg(node v) { return v->adj_length(0); }
inline int    indeg(node v)  { return v->adj_length(1);  }
inline int    degree(node v) { return indeg(v) + outdeg(v); }

inline node   terminal(edge e, int i) { return e->terminal(i); }

inline int    index(node v)  { return v->id(); }
inline int    index(edge e)  { return e->id(); }
inline int    index(face f)  { return f->id(); }

inline node   opposite(node v, edge e)
{ node s = e->terminal(0); return (v==s) ? e->terminal(1) : s; }

inline node   opposite(edge e, node v) { return opposite(v,e); }


inline bool graph::is_hidden(node v) const { return v->hidden(); }
inline bool graph::is_hidden(edge e) const { return e->hidden(); }


inline int  graph::outdeg(node v) const { return v->adj_length(0); }
inline int  graph::indeg(node v)  const { return v->adj_length(1); }
inline int  graph::degree(node v) const { return outdeg(v) + indeg(v); }

inline node graph::source(edge e)    const   { return e->terminal(0); }
inline node graph::target(edge e)    const   { return e->terminal(1); }

inline node graph::opposite(node v,edge e) const 
{ return (v==source(e)) ? target(e) : source(e); }

inline node graph::opposite(edge e, node v) const  
{ return (v==source(e)) ? target(e) : source(e); }

inline graph* graph::super()       const   { return parent; }

inline int graph::max_node_index() const   { return max_n_index; }
inline int graph::max_edge_index() const   { return max_e_index; }
inline int graph::max_face_index() const   { return max_f_index; }

inline int  graph::number_of_nodes() const   { return v_list.length(); }
inline int  graph::number_of_edges() const   { return e_list.length(); }

inline GenPtr& graph::entry(node v) const  { return v->data(0); }
inline GenPtr& graph::entry(edge e) const  { return e->data(0); }
inline GenPtr& graph::entry(face f) const  { return f->data(0); }

inline const GenPtr& graph::inf(node v) const { return v->data(0); }
inline const GenPtr& graph::inf(edge e) const { return e->data(0); }
inline const GenPtr& graph::inf(face f) const { return f->data(0); }

inline void graph::assign(node v,GenPtr x) { v->data(0) = x; }
inline void graph::assign(edge e,GenPtr x) { e->data(0) = x; }
inline void graph::assign(face f,GenPtr x) { f->data(0) = x; }


inline node graph::first_node()      const { return v_list.head(); }
inline node graph::last_node()       const { return v_list.tail(); }
inline node graph::succ_node(node v) const { return v_list.succ(v); }
inline node graph::pred_node(node v) const { return v_list.pred(v); }
inline node graph::next_node(node v) const { return v?v_list.succ(v):0; }
inline node graph::prev_node(node v) const { return v?v_list.pred(v):0; }

inline edge graph::first_edge()      const { return e_list.head(); }
inline edge graph::last_edge()       const { return e_list.tail(); }
inline edge graph::succ_edge(edge e) const { return e_list.succ(e); }
inline edge graph::pred_edge(edge e) const { return e_list.pred(e); } 
inline edge graph::next_edge(edge e) const { return e?e_list.succ(e):0; }
inline edge graph::prev_edge(edge e) const { return e?e_list.pred(e):0; }

inline face graph::first_face()      const { return f_list.head(); }
inline face graph::last_face()       const { return f_list.tail(); }
inline face graph::succ_face(face f) const { return f_list.succ(f); }
inline face graph::pred_face(face f) const { return f_list.pred(f); } 
inline face graph::next_face(face f) const { return f?f_list.succ(f):0; }
inline face graph::prev_face(face f) const { return f?f_list.pred(f):0; } 



inline edge graph::first_adj_edge(node v) const { return v->first_adj_edge(0); }
inline edge graph::last_adj_edge(node v) const { return v->last_adj_edge(0); }

inline edge graph::adj_succ(edge e) const { return e->succ_adj_edge(0); }
inline edge graph::adj_pred(edge e) const { return e->pred_adj_edge(0); }

inline edge graph::first_out_edge(node v) const { return first_adj_edge(v); }
inline edge graph::last_out_edge(node v)  const { return last_adj_edge(v);  }
inline edge graph::stop_out_edge(node v)  const { return 0;  }

inline edge graph::next_out_edge(edge e)  const { return adj_succ(e);       }

inline edge graph::out_succ(edge e)       const { return adj_succ(e);       }
inline edge graph::out_pred(edge e)       const { return adj_pred(e);       }


inline edge graph::cyclic_adj_succ(edge e) const 
{ edge e1 = adj_succ(e);
  return e1 ? e1 : first_adj_edge(source(e)); }

inline edge graph::cyclic_adj_pred(edge e) const 
{ edge e1 = adj_pred(e);
  return e1 ? e1 : last_adj_edge(source(e)); }



inline edge graph::first_in_edge(node v) const { return v->first_adj_edge(1); }
inline edge graph::last_in_edge(node v) const { return v->last_adj_edge(1); }

inline edge graph::in_succ(edge e) const { return e->succ_adj_edge(1); }
inline edge graph::in_pred(edge e) const { return e->pred_adj_edge(1); }

inline edge graph::cyclic_in_succ(edge e) const 
{ edge e1 = in_succ(e);
  return e1 ? e1 : first_in_edge(target(e)); }

inline edge graph::cyclic_in_pred(edge e) const 
{ edge e1 = in_pred(e);
  return e1 ? e1 : last_in_edge(target(e)); }


// undirected 

inline edge graph::adj_succ(edge e, node v) const 
{ return e->succ_adj_edge((v==source(e)) ? 0 : 1); }

inline edge graph::adj_pred(edge e, node v) const 
{ return e->pred_adj_edge((v==source(e)) ? 0 : 1); }

inline edge graph::cyclic_adj_succ(edge e, node v)  const
{ edge r = adj_succ(e,v);
  return (r) ? r :  v->first_adj_edge(0);
 }

inline edge graph::cyclic_adj_pred(edge e, node v) const
{ edge r = adj_pred(e,v);
  return (r) ? r : v->last_adj_edge(0);
 }



inline ostream& operator<<(ostream& O, const graph& G) { G.write(O); return O; }
inline istream& operator>>(istream& I, graph& G)  { G.read(I);  return I; }


//------------------------------------------------------------------------------
// Iteration   (macros)
//------------------------------------------------------------------------------

/*{\Mtext            
\bigskip
{\bf h) Iteration} }*/

/*{\Mtext            
All iteration macros listed in this section traverse the corresponding
node and edge lists of the graph, i.e. they visit nodes and edges in 
the order in which they are stored in these lists.
}*/


#define forall_nodes(v,G)\
LEDA_FORALL_PREAMBLE \
for(void* loop_var=(G).first_node();\
LOOP_ASSIGN(v,loop_var), loop_var=(G).next_node(v), v != (G).stop_node();)

#define forall_edges(e,G)\
LEDA_FORALL_PREAMBLE \
for(void* loop_var=(G).first_edge();\
LOOP_ASSIGN(e,loop_var), loop_var=(G).next_edge(e), e != (G).stop_edge();)


#define forall_rev_nodes(v,G)\
LEDA_FORALL_PREAMBLE \
for(void* loop_var=(G).last_node();\
LOOP_ASSIGN(v,loop_var), loop_var=(G).prev_node(v), v != (G).stop_node();)

#define forall_rev_edges(e,G)\
LEDA_FORALL_PREAMBLE \
for(void* loop_var=(G).last_edge();\
LOOP_ASSIGN(e,loop_var), loop_var=(G).prev_edge(e), e != (G).stop_edge();)

#define forall_hidden_edges(e,G) \
for(e = G.first_hidden_edge(); e != G.stop_hidden_edge(); e = G.next_hidden_edge(e))



/*{\Mtext            
{\bf forall\_nodes}($v,G$)\newline
$\{$ ``the nodes of $G$ are successively assigned to $v$" $\}$

{\bf forall\_edges}($e,G$)\newline
$\{$ ``the edges of $G$ are successively assigned to $e$" $\}$ 

{\bf forall\_rev\_nodes}($v,G$)\newline
$\{$ ``the nodes of $G$ are successively assigned to $v$ in reverse order" $\}$

{\bf forall\_rev\_edges}($e,G$)\newline
$\{$ ``the edges of $G$ are successively assigned to $e$ in reverse order" $\}$

{\bf forall\_hidden\_edges}($e,G$)\newline
$\{$ ``all hidden edges of $G$ are successively assigned to $e$" $\}$ 
}*/


#define forall_out_edges(e,v)\
LEDA_FORALL_PREAMBLE \
for(void *loop_var=v->first_out_edge(), *loop_stop=v->stop_out_edge();\
LOOP_ASSIGN(e,loop_var), loop_var=e?e->next_out_edge():0, (void*)e != loop_stop;)

#define forall_in_edges(e,v)\
LEDA_FORALL_PREAMBLE \
for(void *loop_var=v->first_in_edge(), *loop_stop=v->stop_in_edge();\
LOOP_ASSIGN(e,loop_var), loop_var=e?e->next_in_edge():0, (void*)e != loop_stop;)


#define forall_rev_out_edges(e,v) \
for(void* loop_stop = ((e = v->first_out_edge_rev()), v->stop_out_edge_rev());\
    e != loop_stop; e = e->pred_out_edge())


#define forall_adj_edges(e,v)\
LEDA_FORALL_PREAMBLE \
for(leda_edge loop_var=First_Adj_Edge(v,0);e=loop_var,\
loop_var=Succ_Adj_Edge(loop_var,v), e;)

#define forall_inout_edges(e,v)\
LEDA_FORALL_PREAMBLE \
for(bool out = First_Inout_Edge(e,v); e; out = Succ_Inout_Edge(e,out,v))

#define forall_adj_undirected_edges(e,v)\
LEDA_FORALL_PREAMBLE \
for(e=First_Inout_Edge_Undirected(v); e; e = Succ_Inout_Edge_Undirected(e,v))


#define forall_incident_edges(e,v) \
for(First_Inc_Edge(e,v); e; Next_Inc_Edge(e,v))

#define forall_cyclic_incident_edges(e,e_start,v) \
for(e = e_start; e; e=Next_Cyclic_Inc_Edge(e,e_start,v))



/*{\Mtext
{\bf forall\_adj\_edges}($e,w$)\newline
$\{$ ``the edges adjacent to node $w$ are successively assigned to $e$" $\}$ 

{\bf forall\_out\_edges}($e,w$)\newline
a faster version of {\bf forall\_adj\_edges} for directed graph. 

{\bf forall\_in\_edges}($e,w$)\newline
$\{$ ``the edges of $in\_edges(w)$ are successively assigned to $e$" $\}$ 

{\bf forall\_inout\_edges}($e,w$)\newline
$\{$ ``the edges of $out\_edges(w)$ and $in\_edges(w)$ are successively 
       assigned to $e$" $\}$ 

{\bf forall\_adj\_undirected\_edges}($e,w$)\newline
like {\bf forall\_adj\_edges} on the underlying undirected graph, 
no matter whether the graph is directed or undirected actually.

}*/


#define forall_adj_nodes(u,v)\
LEDA_FORALL_PREAMBLE \
for(leda_edge loop_var=First_Adj_Edge(v,0);\
u = loop_var ? opposite(v,loop_var) : 0,\
loop_var=Succ_Adj_Edge(loop_var,v), u;)



/*{\Mtext
\bigskip
{\bf forall\_adj\_nodes}($v,w$)\newline
$\{$ ``the nodes adjacent to node $w$ are successively assigned to v" $\}$ }*/




// Iteration for faces

#define forall_faces(f,G)\
for(f=(G).first_face(); f; f=(G).succ_face(f))

#define forall_face_edges(e,F)\
for(e=LEDA_SCOPE graph_of(F)->first_face_edge(F); e; e=LEDA_SCOPE graph_of(F)->next_face_edge(e,F))

#define forall_adj_faces(f,v)\
LEDA_FORALL_PREAMBLE \
for(leda_edge loop_var=LEDA_SCOPE graph_of(v)->first_adj_edge(v);\
loop_var && (f = LEDA_SCOPE graph_of(v)->adj_face(loop_var)) != 0;\
loop_var=LEDA_SCOPE graph_of(v)->adj_succ(loop_var))


/*{\Mtext
\bigskip
{\bf Faces}

Before using any of the following face iterators the list of faces
has to be computed by calling |G.compute_faces()|. Note, that
any update operation invalidates this list.


{\bf forall\_faces}($f,M$)\newline
$\{$ ``the faces of $M$ are successively assigned to $f$" $\}$

{\bf forall\_face\_edges}($e,f$)\newline
$\{$ ``the edges of face $f$ are successively assigned to $e$" $\}$

{\bf forall\_adj\_faces}($f,v$)\newline
$\{$ ``the faces adjacent to node $v$ are successively assigned to $f$" $\}$ }*/



/*{\Mimplementation

Graphs are implemented by doubly linked lists of nodes and edges. Most
operations take constant time, except for all\_nodes, all\_edges,
del\_all\_nodes, del\_all\_edges, make\_map, make\_planar\_map,
compute\_faces, all\_faces, make\_map, clear, write, and read which
take time $O(n+m)$, and adj\_edges, adj\_nodes, out\_edges, in\_edges,
and adj\_faces which take time $O(output\ size)$ where $n$ is the
current number of nodes and $m$ is the current number of edges. The
space requirement is $O(n+m)$.}*/

LEDA_END_NAMESPACE

#include <LEDA/graph/node_array.h>
#include <LEDA/graph/edge_array.h>
#include <LEDA/graph/node_map.h>
#include <LEDA/graph/edge_map.h>


LEDA_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
// GRAPH<vtype,etype> : parameterized directed graph
//------------------------------------------------------------------------------



/*{\Manpage {GRAPH} {vtype,etype} {Parameterized Graphs}}*/


template<class vtype, class etype> 
class GRAPH : public graph {

/*{\Mdefinition 
A parameterized graph $G$ is a graph whose nodes and edges contain additional
(user defined) data. Every node contains an element of a data type $vtype$, 
called the node type of $G$ and every edge contains an element of a data type 
$etype$ called the edge type of $G$. We use $\<v,w,y\>$ to denote an edge 
$(v,w)$ with information $y$ and $\<x\>$ to denote a node with information $x$.

All operations defined for the basic graph type $graph$ are also defined on
instances of any parameterized graph type |\Mname|. For parameterized graph 
there are additional operations to access or update the information 
associated with its nodes and edges.  
Instances of a parameterized graph type can be used wherever an instance 
of the data type $graph$ can be used, e.g., in assignments and as 
arguments to functions with formal parameters of type $graph\&$. 
If a function $f(graph\&\ G)$ is called with an argument $Q$ of type 
|\Mname| then inside $f$ only the basic graph structure of $Q$ can be 
accessed. The node and edge entries are hidden. This allows the design 
of generic graph algorithms, i.e., algorithms accepting
instances of any parametrized graph type as argument.}*/


leda_cmp_base<vtype> cmp_node_def;
leda_cmp_base<etype> cmp_edge_def;

int (*cmp_node_ptr)(const vtype&, const vtype&);
int (*cmp_edge_ptr)(const etype&, const etype&);

node_array<vtype,graph> node_data_array;
edge_array<etype,graph> edge_data_array;

void init_node_entry(GenPtr& x) const { LEDA_CREATE(vtype,x); }

void init_edge_entry(GenPtr& x) const { LEDA_CREATE(etype,x); }

void copy_node_entry(GenPtr& x) const  { LEDA_COPY(vtype,x); }
void copy_edge_entry(GenPtr& x) const  { LEDA_COPY(etype,x); }

void clear_node_entry(GenPtr& x) const { LEDA_CLEAR(vtype,x); }
void clear_edge_entry(GenPtr& x) const { LEDA_CLEAR(etype,x); }

void write_node_entry(ostream& o, GenPtr& x) const { LEDA_PRINT(vtype,x,o);}
void write_edge_entry(ostream& o, GenPtr& x) const { LEDA_PRINT(etype,x,o);}

void read_node_entry(istream& i, GenPtr& x) { LEDA_READ(vtype,x,i); }
void read_edge_entry(istream& i, GenPtr& x) { LEDA_READ(etype,x,i); }


public:

/*
list_item register_map(graph_map<GRAPH<vtype,etype> >* p, int& slot_i, bool use_slots=false)
{ return graph::register_map((graph_map<graph>*)p,slot_i,use_slots); }

void unregister_map(graph_map<GRAPH<vtype,etype> >* p)
{ graph::unregister_map((graph_map<graph>*)p); }
*/


/*{\Mtypes 5}*/ 

typedef vtype  node_value_type;
/*{\Mtypemember the type of node data ($vtype$).}*/

typedef etype  edge_value_type;
/*{\Mtypemember the type of edge data ($etype$).}*/


const char* node_type()  const { return LEDA_TYPE_NAME(vtype); }
const char* edge_type()  const { return LEDA_TYPE_NAME(etype); }
//const char* face_type()  const { return LEDA_TYPE_NAME(ftype); }

int cmp_node_entry(node x, node y) const 
{ return cmp_node_ptr(inf(x),inf(y)); }

int cmp_edge_entry(edge x, edge y) const
{ return cmp_edge_ptr(inf(x),inf(y)); }


/*{\Mcreation G }*/


GRAPH() // : node_data_array((graph*)this), edge_data_array((graph*)this) {}
{ node_data_array.connect((graph*)this);
  edge_data_array.connect((graph*)this);
}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it to 
            the empty graph. }*/


GRAPH(int sz1, int sz2) : graph(sz1,sz2)
{ node_data_array.connect((graph*)this);
  edge_data_array.connect((graph*)this);
 }


GRAPH<vtype,etype>& operator=(const GRAPH<vtype,etype>& a)
{ if (this != &a)
  { clear_all_entries();
    graph::operator=(a);
    copy_all_entries();
   }
  return *this; 
 }

GRAPH(const GRAPH<vtype,etype>& a) : graph(a)
{ node_data_array.connect(this);
  edge_data_array.connect(this);
  a.copy_all_entries(); 
 }

// subgraph
/*
GRAPH(GRAPH<vtype,etype>& a, const list<node>& b, const list<edge>& c) 
: graph(a,b,c) {}
GRAPH(GRAPH<vtype,etype>& a, const list<edge>& c) 
: graph(a,c) {}
*/

virtual ~GRAPH()   { if (parent==0) clear_all_entries(); }


/*{\Moperations 1.5 5.0 }*/

const vtype& inf(node v) const { return LEDA_CONST_ACCESS(vtype,graph::inf(v));}
/*{\Mop    returns the information of node $v$. }*/

/*{\Moptions nextwarning=no }*/
const vtype& operator[] (node v) const { return inf(v);                      }
vtype&       operator[] (node v)       { return LEDA_ACCESS(vtype,entry(v)); }
/*{\Marrop      returns a reference to $G$.inf($v$).}*/



const etype& inf(edge e) const { return LEDA_CONST_ACCESS(etype,graph::inf(e));}
/*{\Mop    returns the information of edge $e$. }*/

/*{\Moptions nextwarning=no }*/
const etype& operator[] (edge e) const { return inf(e);                      }
etype&       operator[] (edge e)       { return LEDA_ACCESS(etype,entry(e)); }
/*{\Marrop      returns a reference to $G$.inf($e$).}*/



node_array<vtype>& node_data() { return node_data_array; }
/*{\Mopl   makes the information associated with the nodes of $G$ 
           available as a node array of type |node_array<vtype>|. }*/

edge_array<etype>& edge_data() { return edge_data_array; }
/*{\Mopl   makes the information associated with the edges of $G$ 
           available as an edge array of type |edge_array<etype>|. }*/

const node_array<vtype>& node_data() const { return node_data_array; }
const edge_array<etype>& edge_data() const { return edge_data_array; }


void   assign(node v, const vtype& x) { operator[](v) = x; }
/*{\Mopl   makes $x$ the information of node $v$. }*/

void   assign(edge e, const etype& x) { operator[](e) = x; }
/*{\Mopl   makes $x$ the information of edge $e$. }*/
                
node   new_node(const vtype& x)  
{ return graph::new_node(leda_copy(x)); 
  //node v = graph::new_node();
  //assign(v,x);
  //return v;
}
/*{\Mop    adds a new node $\<x\>$ to $G$ and returns it. }*/

node   new_node(node u, const vtype& x, int dir)
{ return graph::new_node(leda_copy(x),u,dir);
  //node v = graph::new_node(u,dir);
  //assign(v,x);
  //return v;
}

/*{\Mop        adds a new node $v=\<x\>$ to |\Mvar| and returns it. $v$ is 
               inserted in front of (|dir=leda::before|) or behind 
               (|dir=leda::behind|) node $u$ into the list of all nodes. }*/



edge   new_edge(node v, node w, const etype& x) 
{ return graph::new_edge(v,w,leda_copy(x)); 
  //edge e = graph::new_edge(v,w); 
  //assign(e,x);
  //return e;
}
/*{\Mopl   adds a new edge $\<v,w,x\>$ to $G$ by appending it 
           to $adj\_edges(v)$ and to $in\_edges(w)$ and returns it. }*/


edge   new_edge(edge e, node w, const etype& x, int dir=leda::behind)
{ return graph::new_edge(e,w,leda_copy(x),dir);
  //edge y = graph::new_edge(e,w,dir);
  //assign(y,x);
  //return y;
}
/*{\Mopl   adds a new edge $\<source(e),w,x\>$ to $G$ by inserting it 
           behind (|dir=leda::behind|) or in front of (|dir=leda::before|) 
           edge $e$ into $adj\_edges(source(e))$ and appending it to 
           $in\_edges(w)$. Returns the new edge.}*/



edge   new_edge(node v, edge e, const etype& x, int dir=leda::behind)
{ return graph::new_edge(v,e,leda_copy(x),dir);
  //edge y = graph::new_edge(v,e,dir);
  //assign(y,x);
  //return y;
}
/*{\Mopl   adds a new edge $\<v,target(e),x\>$ to $G$ by inserting it 
           behind (|dir=leda::behind|) or in front of (|dir=leda::before|) 
           edge $e$ into $in\_edges(target(e))$ and appending it to 
           $adj\_edges(v)$. Returns the new edge.}*/


edge new_edge(edge e1, edge e2, const etype& x, int d1=leda::behind,
                                                int d2=leda::behind) 
{ return graph::new_edge(e1,e2,leda_copy(x),d1,d2);
  //edge e = graph::new_edge(e1,e2,d1,d2);
  //assign(e,x);
  //return e;
}
/*{\Mopl   adds a new edge $x = (source(e1),target(e2),x)$ to |\Mvar|. $x$ is 
           inserted in front of (if |d1=leda::before|) or behind (if |d1=leda::behind|)
           edge $e1$ into $adj\_edges(source(e1))$ and in front of 
           (if |d2=leda::before|) or behind (if |d2=leda::behind|) edge $e2$ 
           into $in\_edges(target(e2))$ (if |\Mvar| is directed) or 
           $adj\_edges(target(e2))$ (if |\Mvar| is undirected).
           The operation returns the new edge $x$.}*/


// undirected graph only

edge new_edge(node v, edge e1, node w, edge e2, const etype& x, 
                                                int d1=leda::behind, 
                                                int d2=leda::behind)
{ return graph::new_edge(v,e1,w,e2,leda_copy(x),d1,d2);
  //edge e = graph::new_edge(v,e1,w,e2,d1,d2);
  //assign(e,x);
  //return e;
}
/*{\Mopl       adds a new edge $(v,w,x)$ to |\Mvar| by inserting it in front of 
               (if |d1=leda::before|) or behind (if |d1=leda::behind|) edge $e1$ 
               into $adj\_edges(v)$ and in front (if |d2=leda::before|) or behind 
               (if |d2=leda::behind|) edge $e2$ into $adj\_edges(w)$, 
               and returns it.\\
               \precond  |\Mvar| is undirected, $v \ne w$,
               $e1$ is incident to $v$, and $e2$ is incident to $w$. }*/ 


edge new_edge(node v, edge e, node w, const etype& x, int d=leda::behind)
{ return graph::new_edge(v,e,w,leda_copy(x),d);
  //edge y = graph::new_edge(v,e,w,d);
  //assign(y,x);
  //return y;
}
/*{\Mopl       adds a new edge $(v,w,x)$ to |\Mvar| by inserting it in front of 
               (if |d=leda::before|) or behind (if |d=leda::behind|) edge $e$ 
               into $adj\_edges(v)$ and appending it to $adj\_edges(w)$, 
               and returns it.\\
               \precond  |\Mvar| is undirected, $v \ne w$,
               $e1$ is incident to $v$, and $e$ is incident to $v$. }*/ 




//------------------------------------------------------------------
// new_node & new_edge operations using default values for  node and 
// edge information
//------------------------------------------------------------------

node new_node()  
{ return graph::new_node(); }

node new_node(node u, int dir)  
{ return graph::new_node(u,dir); }

edge new_edge(node v, node w) 
{ return graph::new_edge(v,w); }

edge new_edge(edge e, node w, int dir=leda::behind)
{ return graph::new_edge(e,w,dir); }

edge new_edge(node v, edge e, int dir=leda::behind)
{ return graph::new_edge(v,e,dir); }

edge new_edge(edge e1, edge e2, int d1=leda::behind, int d2=leda::behind) 
{ return graph::new_edge(e1,e2,d1,d2); }


edge new_edge(node v, edge e1, node w, edge e2, int d1=leda::behind, 
                                                int d2=leda::behind)
{ return graph::new_edge(v,e1,w,e2,d1,d2); }

edge new_edge(node v, edge e, node w, int d)
{ return graph::new_edge(v,e,w,d); }

void join(GRAPH<vtype,etype>& H) { graph::join(H); }


node split_edge(edge e, const vtype& x, edge& e1, edge& e2)
{ return graph::split_edge(e,leda_copy(x),e1,e2); }
/*{\Xop   experimental }*/



void sort_nodes(int (*cmp)(const node&, const node&)){ graph::sort_nodes(cmp); }
void sort_edges(int (*cmp)(const edge&, const edge&)){ graph::sort_edges(cmp); }
void sort_nodes(const graph_map<graph>& A) { graph::sort_nodes(A); }
void sort_edges(const graph_map<graph>& A) { graph::sort_edges(A); }

void sort_nodes(const list<node>& vl) { graph::sort_nodes(vl); }
/*{\Mop    makes |vl| the node list of $G$.\\
           \precond |vl| contains exactly the nodes of $G$.}*/

void sort_edges(const list<edge>& el) { graph::sort_edges(el); }
/*{\Mop    makes |el| the edge list of $G$.\\
           \precond |el| contains exactly the edges of $G$.}*/


void sort_nodes() { cmp_node_ptr = compare; graph::sort_nodes(); }
/*{\Mop    the nodes of $G$ are sorted increasingly according to their
           contents.\\
           \precond $vtype$ is linearly ordered.}*/

void sort_edges() { cmp_edge_ptr = compare; graph::sort_edges(); }
/*{\Mop    the edges of $G$ are sorted increasingly according to their
	   contents.\\
           \precond $etype$ is linearly ordered.}*/




/*{\Moptions nextwarning=no }*/
/* inherited 
void write(string fname) const { graph::write(fname); }
*/

/*{\Mop    writes $G$ to the file with name $fname$. The
           output operators |operator<<(ostream&, const vtype&)| and
           |operator<<(ostream&, const etype&)|(cf. section 1.6) must
           be defined.}*/

/*{\Moptions nextwarning=no }*/
/* inherited 
int read(string fname) { return graph::read(fname); }
*/

/*{\Mop    reads $G$ from the file with name $fname$. The 
	   input operators |operator>>(istream&,vtype&)| and 
	   |operator>>(istream&,etype&)| (cf.~section 1.6) must
	   be defined. Returns error code\\
	   1 \quad if file $fname$ does not exist\\
	   2 \quad if graph is not of type |\Mname|\\
	   3 \quad if file $fname$ does not contain a graph\\
	   0 \quad if reading was successful.}*/

void   clear()  
{ bool call_handler = handlers_enabled;
  handlers_enabled = false;
  if (call_handler) pre_clear_handler();
  clear_all_entries(); 
  graph::clear();
  if (call_handler) post_clear_handler();
  handlers_enabled = call_handler;
}

};

//LEDA_END_NAMESPACE


/*{\Mimplementation
Parameterized graph are derived from directed graph. All additional 
operations for manipulating the node and edge entries take constant
time.}*/

LEDA_END_NAMESPACE

#include <LEDA/graph/node_list.h>
#include <LEDA/graph/graph_gen.h>
#include <LEDA/graph/graph_misc.h>



#if LEDA_ROOT_INCL_ID == 600222
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif



#endif

