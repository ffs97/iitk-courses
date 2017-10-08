/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_objects0.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/core/impl/olist.h>
#include <LEDA/graph/graph_obj_list.h>


LEDA_BEGIN_NAMESPACE

class __exportC node_struct;
class __exportC edge_struct;
class __exportC face_struct;
class __exportC graph;


//------------------------------------------------------------------------------
// class node_struct: internal representation of nodes
//------------------------------------------------------------------------------

class __exportC node_struct : public graph_object,
                              public obj_link  // used by node_list
{  
   friend class __exportC edge_struct;

   unsigned  ID;                       // internal id (index)  
   edge_struct*      FirstAdjEdge[2];  // head of adj/in list
   edge_struct*      LastAdjEdge[2];   // tail of adj/in list
   int       AdjLength[2];             // outdeg/indeg
   graph*    Owner;                    // graph of this node
   GenPtr    Data[1];                  // Data[0]: GRAPH

public:

node_struct(GenPtr i=0, int id=0);

int id() const;
void set_id(int i);

graph* owner() const;
void   set_owner(graph* g);

GenPtr&  data(int i);

void hide();
void unhide();
bool hidden() const;


edge_struct* first_adj_edge(int i) const;
edge_struct* last_adj_edge(int i) const;
int  adj_length(int i) const;


void clear_adj_lists();
void insert_adj_edge(edge_struct* e, edge_struct* e1, int i, int chain_e, int dir);
void append_adj_edge(edge_struct* e,int i, int chain_e);
void push_adj_edge(edge_struct* e,int i, int chain_e);
void del_adj_edge(edge_struct* e, int i, int chain_e);
void insert_adj_list(int i, edge_struct* e1, node_struct* v, int j);
void append_adj_list(int i, node_struct* v, int j);


// for compatibility with static graph (s.n.)

edge_struct* first_out_edge() const;
edge_struct* stop_out_edge() const;
edge_struct* first_out_edge_rev() const;
edge_struct* stop_out_edge_rev() const;
edge_struct* first_in_edge() const;
edge_struct* last_in_edge() const;
edge_struct* stop_in_edge() const;


LEDA_MEMORY(node_struct)

};


inline node_struct* construct_node(int slot_num, GenPtr inf, int index)
{ int sz = sizeof(node_struct) + slot_num*sizeof(GenPtr);
  node_struct* v = (node_struct*)std_memory.allocate_bytes(sz);
  new (v) node_struct(inf,index);
  return v;
 }

inline void destroy_node(node_struct* v, int slot_num) 
{ int sz = sizeof(node_struct) + slot_num*sizeof(GenPtr);
  std_memory.deallocate_bytes(v,sz); 
 }



inline void     node_struct::hide()   { ID |= 0x80000000; }
inline void     node_struct::unhide() { ID &= 0x7fffffff; }
inline bool     node_struct::hidden() const { return (ID & 0x80000000) != 0; }

inline graph*   node_struct::owner() const { return Owner; }
inline void     node_struct::set_owner(graph* g) { Owner = g; }

inline int      node_struct::id() const    { return ID & 0x7fffffff;  }
inline void     node_struct::set_id(int i) { ID = (ID & 0x80000000) | i; }

inline edge_struct* node_struct::first_adj_edge(int i) const 
{ return FirstAdjEdge[i]; }

inline edge_struct* node_struct::last_adj_edge(int i) const 
{ return LastAdjEdge[i]; }

inline int  node_struct::adj_length(int i) const { return AdjLength[i]; }

inline GenPtr& node_struct::data(int i) { return Data[i]; }


/* moved to _graph_objects.cpp 
inline edge_struct* node_struct::first_out_edge() const 
{ return first_adj_edge(0); }
*/

inline edge_struct* node_struct::stop_out_edge() const { return 0; }

inline edge_struct* node_struct::first_out_edge_rev() const 
{ return last_adj_edge(0); }

inline edge_struct* node_struct::stop_out_edge_rev() const { return 0; }

/* moved to _graph_objects.cpp 
inline edge_struct* node_struct::first_in_edge() const 
{ return first_adj_edge(1); }
*/

inline edge_struct* node_struct::last_in_edge() const 
{ return last_adj_edge(1); }

inline edge_struct* node_struct::stop_in_edge() const { return 0; }



//------------------------------------------------------------------------------
// class edge_struct: internal representation of edges
//------------------------------------------------------------------------------

class __exportC edge_struct  : public graph_object
{ 
   friend class __exportC node_struct;

   edge_struct*      SuccAdjEdge[2];  // chaining of adjacent out/in-edges
   edge_struct*      PredAdjEdge[2];  //
   node_struct*      term[2];         // term[0] = source and term[1] = target
   unsigned  ID;                      // internal id (index)  
   edge_struct*      rev;             // reverse edge
   GenPtr    Data[1];                 // space for data (Data[0] used by GRAPH)

public:

edge_struct(node_struct* v=0, node_struct* w=0, GenPtr i=0, int id=0);

int      id() const;
void     set_id(int i);
GenPtr&  data(int i);

graph*   owner() const;

void hide();
void unhide();
bool hidden() const;

edge_struct* succ_adj_edge(int i) const;
edge_struct* pred_adj_edge(int i) const;
edge_struct* reversal() const;
node_struct* terminal(int i) const;

void set_reversal(edge_struct* r);
void set_term(int i, node_struct* v);


// for compatibility with static graph (s.n.)

edge_struct* next_out_edge() const;
edge_struct* pred_out_edge() const;
edge_struct* next_in_edge()  const;
edge_struct* pred_in_edge()  const;

LEDA_MEMORY(edge_struct)

};


inline edge_struct* construct_edge(int slot_num, node_struct* v, node_struct* w, GenPtr inf, int index)
{ int sz = sizeof(edge_struct) + slot_num*sizeof(GenPtr);
  edge_struct* e = (edge_struct*)std_memory.allocate_bytes(sz);
  new (e) edge_struct(v,w,inf,index);
  return e;
 }

inline void destroy_edge(edge_struct* e, int slot_num) 
{ int sz = sizeof(edge_struct) + slot_num*sizeof(GenPtr);
  std_memory.deallocate_bytes(e,sz); 
 }


inline GenPtr&  edge_struct::data(int i)   { return Data[i]; }
inline int      edge_struct::id() const    { return ID & 0x7fffffff;  }
inline void     edge_struct::set_id(int i) { ID = (ID & 0x80000000) | i; }

inline graph*   edge_struct::owner() const { return term[0]->owner(); }

inline void edge_struct::hide()   { ID |= 0x80000000; }
inline void edge_struct::unhide() { ID &= 0x7fffffff; }
inline bool edge_struct::hidden() const { return (ID & 0x80000000) != 0; }


inline edge_struct* edge_struct::succ_adj_edge(int i) const { return SuccAdjEdge[i]; }
inline edge_struct* edge_struct::pred_adj_edge(int i) const { return PredAdjEdge[i]; }
inline edge_struct* edge_struct::reversal()           const { return rev; }
inline node_struct* edge_struct::terminal(int i)      const { return term[i]; }

inline void edge_struct::set_reversal(edge_struct* r)    { rev = r; }
inline void edge_struct::set_term(int i, node_struct* v) { term[i] = v; }


inline edge_struct* edge_struct::next_out_edge() const 
{ return succ_adj_edge(0); }

inline edge_struct* edge_struct::pred_out_edge() const 
{ return pred_adj_edge(0); }

inline edge_struct* edge_struct::next_in_edge() const 
{ return succ_adj_edge(1); }

inline edge_struct* edge_struct::pred_in_edge() const 
{ return pred_adj_edge(1); }

//------------------------------------------------------------------------------
// class face_struct: internal representation of faces
//------------------------------------------------------------------------------

class __exportC face_struct : public graph_object 
{

 unsigned  ID;       // internal id (index)  
 edge_struct*      head;     // first edge_struct* of face
 int       sz;       // size of face_struct* (# edges)
 graph*    Owner;
 GenPtr    Data[1];  // space for data (Data[0] used by PLANAR_MAP)

public:

 face_struct(GenPtr,int id=0);


 void hide();
 void unhide();
 bool hidden() const;
 int  id() const;
 void set_id(int i);

 graph* owner() const;
 void   set_owner(graph* g);

 edge_struct* first_edge() const;
 void set_first_edge(edge_struct* e);
 int  size() const;
 void set_size(int i);

 GenPtr&  data(int i);

 LEDA_MEMORY(face_struct)
};


inline face_struct::face_struct(GenPtr x, int id) 
{ Data[0] = x ; 
  ID = id;
  head = nil; 
  sz = 0;
 }

inline face_struct* construct_face(int slot_num, GenPtr inf, int index)
{ int sz = sizeof(face_struct) + slot_num*sizeof(GenPtr);
  face_struct* f = (face_struct*)std_memory.allocate_bytes(sz);
  new (f) face_struct(inf,index);
  return f;
 }

inline void destroy_face(face_struct* f, int slot_num) 
{ int sz = sizeof(face_struct) + slot_num*sizeof(GenPtr);
  std_memory.deallocate_bytes(f,sz); 
 }



inline GenPtr&  face_struct::data(int i)   { return Data[i]; }
inline int      face_struct::id() const    { return ID & 0x7fffffff;  }
inline void     face_struct::set_id(int i) { ID = (ID & 0x80000000) | i; }

inline void face_struct::hide()   { ID |= 0x80000000; }
inline void face_struct::unhide() { ID &= 0x7fffffff; }
inline bool face_struct::hidden() const { return (ID & 0x80000000) != 0; }

inline graph* face_struct::owner() const { return Owner; }
inline void   face_struct::set_owner(graph* g) { Owner = g; }

inline edge_struct* face_struct::first_edge() const { return head; }
inline void face_struct::set_first_edge(edge_struct* e) { head = e; }
inline int  face_struct::size()       const { return sz; }
inline void face_struct::set_size(int i) { sz = i; }



// parameterized access of adjacent edges (used in iteration macros)
// outgoing (i=0) or incoming (i=1) edges

inline edge_struct* First_Adj_Edge(node_struct* v,int i) { return v->first_adj_edge(i); }
inline edge_struct* Last_Adj_Edge(node_struct* v, int i) { return v->last_adj_edge(i);  }
inline edge_struct* Succ_Adj_Edge(edge_struct* e, int i) { return e ? e->succ_adj_edge(i) : 0; }
inline edge_struct* Pred_Adj_Edge(edge_struct* e, int i) { return e ? e->pred_adj_edge(i) : 0; }

inline edge_struct* Succ_In_Edge(edge_struct* e) 
{ return e ? e->succ_adj_edge(1) : 0; }

inline edge_struct* Stop_Adj_Edge(node_struct*,int)  { return 0; }


// successor or predecessor of e in adjacency list of node_struct* v
// precond: v = source(e) or v = target(e)

inline edge_struct* Succ_Adj_Edge(edge_struct* e, node_struct* v) 
{ return e ? e->succ_adj_edge((v==e->terminal(0)) ? 0:1) : 0; }

inline edge_struct* Pred_Adj_Edge(edge_struct* e, node_struct* v) 
{ return e ? e->pred_adj_edge((v==e->terminal(0)) ? 0:1) : 0; }


/*
inline edge_struct* First_Inout_Edge0(node_struct* v, int& i) 
{ edge_struct* e = v->first_adj_edge(0);
  i = 0;
  if (e == 0) { e = v->first_adj_edge(1); i = 1; }
  return e;
}

inline edge_struct* Succ_Inout_Edge0(edge_struct* e, node_struct* v, int& i) 
{ edge_struct* x = 0;
  if (e)
  { int j = (e->terminal(i) == v) ? i : 1-i;
    x = e->succ_adj_edge(j);
    if (x == 0 && i == 0) { x = v->first_adj_edge(1); i = 1; }
   }
  return x;
}
*/


bool __exportF First_Inout_Edge(edge_struct*& e, node_struct* v);


inline bool Succ_Inout_Edge(edge_struct*& e, bool e_out, node_struct* v) 
{ if (e_out)
  { e = e->succ_adj_edge(0);  // next_out_edge
    if (e) return true;
    e = v->first_adj_edge(1); // first_in_edge
    return false;
   }
  e = e->succ_adj_edge(1);  // next_in_edge
  return false;
}


inline edge_struct* First_Inout_Edge_Undirected(node_struct* v)
{ edge_struct* e = v->first_adj_edge(0); // first_out_edge
  return (e) ? e : v->first_adj_edge(1); 
}


inline edge_struct* Succ_Inout_Edge_Undirected(edge_struct* e, node_struct* v) 
{ if (v == e->terminal(1)) return e->succ_adj_edge(1); 
  e = e->succ_adj_edge(0);
  return e ? e : v->first_adj_edge(1); // always NULL for undirected graphs !
}



template<class T> 
class graph_obj_list : public graph_obj_list_base {

public:
  void push(T x)                  { graph_obj_list_base::push(x); }
  void append(T x)                { graph_obj_list_base::append(x); }
  void insert(T x, T y)           { graph_obj_list_base::insert(x,y); }
  void remove(T x)                { graph_obj_list_base::remove(x); }
  void conc(graph_obj_list<T>& L) { graph_obj_list_base::conc(L); }
  void clear()                    { graph_obj_list_base::clear(); }

  T pop() { return T(graph_obj_list_base::pop()); }

  T head()    const { return T(graph_obj_list_base::head());  }
  T tail()    const { return T(graph_obj_list_base::tail());  }
  T succ(T x) const { return T(graph_obj_list_base::succ(x)); }
  T pred(T x) const { return T(graph_obj_list_base::pred(x)); }

  bool empty()  const { return graph_obj_list_base::empty();  }
  int  length() const { return graph_obj_list_base::length(); }

  graph_obj_list() {}
};


typedef graph_obj_list<node_struct*> graph_node_list;
typedef graph_obj_list<edge_struct*> graph_edge_list;
typedef graph_obj_list<face_struct*> graph_face_list;

LEDA_END_NAMESPACE


