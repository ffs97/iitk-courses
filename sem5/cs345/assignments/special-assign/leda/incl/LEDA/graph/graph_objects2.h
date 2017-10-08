/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_objects2.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/core/impl/olist.h>

LEDA_BEGIN_NAMESPACE

class __exportC node_struct;
typedef node_struct* node;

class __exportC edge_struct;
typedef edge_struct* edge;

class __exportC face_struct;
typedef face_struct* face;

class __exportC graph;

inline void not_implemented(string msg) 
{ LEDA_EXCEPTION(1, "GraphRep2: " + msg + " not implemented."); }


//------------------------------------------------------------------------------
// class node_struct: internal representation of nodes
//------------------------------------------------------------------------------

class __exportC node_struct // : public graph_object,
                            //   public obj_link  // used by node_list
{  
   friend class __exportC edge_struct;

   edge      FirstAdjEdge[2];  // head of adj/in list
// edge      LastAdjEdge[2];   // tail of adj/in list
// int       AdjLength[2];     // outdeg/indeg
   unsigned  ID;               // internal id (index)  
   graph*    Owner;            // graph of this node
   GenPtr    Data[1];          // Data[0]: GRAPH

public:

node_struct(GenPtr i=0, int id=0);

int id() const;
void set_id(int i);
GenPtr&  data(int i);

graph* owner() const;
void   set_owner(graph* g);


void hide();
void unhide();
bool hidden() const;

edge first_adj_edge(int i) const;
edge last_adj_edge(int i) const;
int  adj_length(int i) const;

void clear_adj_lists();
void append_adj_edge(edge e,int i, int chain_e);
void push_adj_edge(edge e,int i, int chain_e);
void append_adj_list(int i, node v, int j);


// expensive on sinlgy linked lists
void insert_adj_edge(edge e, edge e1, int i, int chain_e, int dir);
void del_adj_edge(edge e, int i, int chain_e);
void insert_adj_list(int i, edge e1, node v, int j);


// for compatibility with static graph (s.n.)

edge_struct* first_out_edge() const { return first_adj_edge(0); }
edge_struct* last_out_edge() const { return last_adj_edge(0); }
edge_struct* stop_out_edge() const { return 0; }

edge_struct* first_in_edge() const { return first_adj_edge(1); }
edge_struct* last_in_edge() const { return last_adj_edge(1); }
edge_struct* stop_in_edge() const { return 0; }



LEDA_MEMORY(node_struct)

};


inline node construct_node(int slot_num, GenPtr inf, int index)
{ cout << "construct node " << endl;
  int sz = sizeof(node_struct) + slot_num*sizeof(GenPtr);
  node v = (node)std_memory.allocate_bytes(sz);
  new (v) node_struct(inf,index);
  return v;
 }


inline void destroy_node(node, int) {} 

/*
inline void destroy_node(node v, int slot_num) 
{ int sz = sizeof(node_struct) + slot_num*sizeof(GenPtr);
  std_memory.deallocate_bytes(v,sz); 
 }
*/



inline void     node_struct::hide()   { ID |= 0x80000000; }
inline void     node_struct::unhide() { ID &= 0x7fffffff; }
inline bool     node_struct::hidden() const 
{ return (ID & 0x80000000) != 0; }

inline graph*   node_struct::owner() const { return Owner; }
inline void     node_struct::set_owner(graph* g) { Owner = g; }

inline int      node_struct::id() const            { return ID & 0x7fffffff;  }
inline void     node_struct::set_id(int i)         { ID = (unsigned)i; }

inline edge node_struct::first_adj_edge(int i) const { return FirstAdjEdge[i]; }

inline GenPtr& node_struct::data(int i) { return Data[i]; }


// inline int  node_struct::adj_length(int i)     const { return AdjLength[i]; 


inline edge node_struct::last_adj_edge(int i)  const 
{ //return LastAdjEdge[i]; 
  not_implemented(string("node_struct::last_adj_edge(%d)",i));
  return 0;
}



//------------------------------------------------------------------------------
// class edge_struct: internal representation of edges
//------------------------------------------------------------------------------

class __exportC edge_struct //: public graph_object
{ 
   friend class __exportC node_struct;

   edge      SuccAdjEdge[2];   // chaining of adjacent out/in-edges
 //edge      PredAdjEdge[2];   //
 //edge      rev;                // reverse edge
   unsigned  ID;                 // internal id (index)  
   node      term[2];            // term[0] = source and term[1] = target
   GenPtr    Data[1];            // space for data (Data[0] used by GRAPH)

public:

edge_struct(node v=0, node w=0, GenPtr i=0, int id=0);
edge_struct(GenPtr i, int id);

int      id() const;
void     set_id(int i);
graph*   owner() const;
GenPtr&  data(int i);

void hide();
void unhide();
bool hidden() const;

edge succ_adj_edge(int i) const;
edge pred_adj_edge(int i) const;
edge reversal() const;
node terminal(int i) const;

void set_reversal(edge r);
void set_term(int i, node v);


// for compatibility with static graph (s.n.)

edge_struct* next_out_edge() const { return succ_adj_edge(0); }
edge_struct* next_in_edge()  const { return succ_adj_edge(1); }


LEDA_MEMORY(edge_struct)

};

// expensive:
inline int  node_struct::adj_length(int i)     const 
{ int count = 0;
  for(edge e = FirstAdjEdge[i]; e; e = e->SuccAdjEdge[i]) count++;
  return count;
}



inline edge construct_edge(int slot_num, node v, node w, GenPtr inf, int index)
{ cout << "construct edge " << endl;
  int sz = sizeof(edge_struct) + slot_num*sizeof(GenPtr);
  edge e = (edge)std_memory.allocate_bytes(sz);
  new (e) edge_struct(v,w,inf,index);
  return e;
 }

inline void destroy_edge(edge, int) {} 

/*
inline void destroy_edge(edge e, int slot_num) 
{ int sz = sizeof(edge_struct) + slot_num*sizeof(GenPtr);
  std_memory.deallocate_bytes(e,sz); 
 }
*/


inline GenPtr&  edge_struct::data(int i)   { return Data[i]; }
inline int      edge_struct::id() const    { return ID & 0x7fffffff; }
inline void     edge_struct::set_id(int i) { ID = (unsigned)i; }

inline graph*   edge_struct::owner() const { return term[0]->owner(); }

inline void edge_struct::hide()   { ID |= 0x80000000; }
inline void edge_struct::unhide() { ID &= 0x7fffffff; }
inline bool edge_struct::hidden() const { return (ID & 0x80000000) != 0; }


inline edge edge_struct::succ_adj_edge(int i) const { return SuccAdjEdge[i]; }
//inline edge edge_struct::pred_adj_edge(int i) const { return PredAdjEdge[i]; }

inline node edge_struct::terminal(int i)      const { return term[i]; }
inline void edge_struct::set_term(int i, node v) { term[i] = v; }

inline edge edge_struct::pred_adj_edge(int i) const 
{ not_implemented(string("pred_adj_edge(%d)",i)); return 0; }

inline edge edge_struct::reversal() const 
{ /* not_implemented("reversal"); */ return 0; }


inline void edge_struct::set_reversal(edge)    
{ not_implemented("set_reversal"); }




//------------------------------------------------------------------------------
// class face_struct: internal representation of faces
//------------------------------------------------------------------------------

class __exportC face_struct //: public graph_object 
{

 unsigned  ID;       // internal id (index)  
 edge      head;     // first edge of face
 int       sz;       // size of face (# edges)
 graph*    Owner;
 GenPtr    Data[1];  // space for data (Data[0] used by PLANAR_MAP)

public:

 face_struct(GenPtr=0,int id=0);


 void hide();
 void unhide();
 bool hidden() const;
 int  id() const;
 void set_id(int i);

 graph* owner() const;
 void   set_owner(graph* g);

 edge first_edge() const;
 void set_first_edge(edge e);
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


inline face construct_face(int slot_num, GenPtr inf, int index)
{ int sz = sizeof(face_struct) + slot_num*sizeof(GenPtr);
  face f = (face)std_memory.allocate_bytes(sz);
  new (f) face_struct(inf,index);
  return f;
 }

inline void destroy_face(face f, int slot_num) 
{ int sz = sizeof(face_struct) + slot_num*sizeof(GenPtr);
  std_memory.deallocate_bytes(f,sz); 
 }


inline GenPtr&  face_struct::data(int i)   { return Data[i]; }
inline int      face_struct::id() const    { return ID & 0x7fffffff;  }
inline void     face_struct::set_id(int i) { ID = (unsigned)i; }

inline void face_struct::hide()   { ID |= 0x80000000; }
inline void face_struct::unhide() { ID &= 0x7fffffff; }
inline bool face_struct::hidden() const { return (ID & 0x80000000) != 0; }

inline graph* face_struct::owner() const { return Owner; }
inline void   face_struct::set_owner(graph* g) { Owner = g; }

inline edge face_struct::first_edge() const { return head; }
inline void face_struct::set_first_edge(edge e) { head = e; }
inline int  face_struct::size()       const { return sz; }
inline void face_struct::set_size(int i) { sz = i; }



// parameterized access of adjacent edges (used in iteration macros)
// outgoing (i=0) or incoming (i=1) edges

inline edge First_Adj_Edge(node v,int i) { return v->first_adj_edge(i); }
inline edge Last_Adj_Edge(node v, int i) { return v->last_adj_edge(i);  }
inline edge Succ_Adj_Edge(edge e, int i) { return e ? e->succ_adj_edge(i) : 0; }
inline edge Pred_Adj_Edge(edge e, int i) { return e ? e->pred_adj_edge(i) : 0; }
inline edge Stop_Adj_Edge(node,int)  { return 0; }

inline edge Succ_In_Edge(edge e) { return e ? e->succ_adj_edge(1) : 0; }


// successor or predecessor of e in adjacency list of node v
// precond: v = source(e) or v = target(e)

inline edge Succ_Adj_Edge(edge e, node v) 
{ return e ? e->succ_adj_edge((v==e->terminal(0)) ? 0:1) : 0; }

inline edge Pred_Adj_Edge(edge e, node v) 
{ return e ? e->pred_adj_edge((v==e->terminal(0)) ? 0:1) : 0; }


inline edge First_Inout_Edge(node v, int& i) 
{ edge e = v->first_adj_edge(0);
  i = 0;
  if (e == 0) { e = v->first_adj_edge(1); i = 1; }
  return e;
}


/*
inline edge Succ_Inout_Edge(edge e, int& i) 
{ edge x = 0;
  if (e)
  { x = e->succ_adj_edge(i);
    if (x == 0 && i == 0) { x = source(e)->first_adj_edge(1); i = 1; }
   }
  return x;
}
*/

inline edge Succ_Inout_Edge(edge e, node v, int& i) 
{ edge x = 0;
  if (e)
  { int j = (e->terminal(i) == v) ? i : 1-i;
    x = e->succ_adj_edge(j);
    if (x == 0 && i == 0) { x = v->first_adj_edge(1); i = 1; }
   }
  return x;
}



template<class T> 
class graph_obj_list {

char* vec;
char* first;
char* last;
int   obj_sz;
int   list_sz;

public:
  graph_obj_list() : obj_sz(0),list_sz(0) { first = last = vec = 0; }

 ~graph_obj_list()  { delete[] vec; }

  void init(int n, int sz) 
  { if (vec) delete[] vec; 
    obj_sz = sizeof(T) + sz*sizeof(void*);
    list_sz = n*obj_sz;
    vec = new char[(n+1)*obj_sz];
    last  = vec;
    first = vec+obj_sz;
   }

  T* construct(GenPtr inf, int i)
  { if (last - vec >= list_sz)
      LEDA_EXCEPTION(1,"GraphRep2: node or edge overflow.");
    last += obj_sz;
    T* x = (T*)last;
    new (x) T(inf,i);
    return x;
   }

  void push(T*)       { not_implemented("push node/edge.");  }
  void insert(T*, T*) { not_implemented("insert node/edge.");}
  void append(T*)     { not_implemented("append node/edge.");}
  T*    pop()         { not_implemented("pop node/edge."); return 0; }

  void conc(graph_obj_list<T>&) 
  { not_implemented("concatenation of node/edge lists."); }

  void remove(T* x) 
  { if (x != tail()) not_implemented("removing non-last node/edge.");
    last -= obj_sz; 
   }

  void clear() { last = vec; }

  T* head()     const { return (T*)((last >= first) ? first : 0);  }
  T* tail()     const { return (T*)((last >= first) ? last  : 0);  }

  T* succ(T* x) const 
  { return (x == tail())? 0 : (T*)((char*)x+obj_sz); }

  T* pred(T* x) const 
  { return (x == head()) ? 0 : (T*)((char*)x-obj_sz); }

  bool empty()  const { return last < first; }
  int  length() const { return (last - vec)/obj_sz; }

};


typedef graph_obj_list<node_struct> graph_node_list;
typedef graph_obj_list<edge_struct> graph_edge_list;
typedef graph_obj_list<face_struct> graph_face_list;

LEDA_END_NAMESPACE


