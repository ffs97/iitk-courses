/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  seg_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SEGMENT_TREE_H
#define LEDA_SEGMENT_TREE_H

// ------------------------------------------------------------------
//
// full dynamic Segment Trees
//
// Michael Wenzel     (1990)
//
// Implementation follows
// Kurt Mehlhorn: Data Structures and Algorithms, Vol. 3
//
// ------------------------------------------------------------------

#include <LEDA/geo/bb1_tree.h>
#include <LEDA/core/list.h>
#include <LEDA/core/slist.h>

LEDA_BEGIN_NAMESPACE

// ------------------------------------------------------------------
// declarations and definitions 
// ----------------------------------------------------------------- 


class h_segment;
typedef h_segment* h_segment_p;

class seg_node_tree;
typedef seg_node_tree* seg_node_list;

typedef bb1_node* seg_nd_tr_item;

//------------------------------------------------------------------
// class h_segment
//------------------------------------------------------------------

class __exportC Segment_Tree;

class __exportC h_segment {

  GenPtr _x0;
  GenPtr _x1;
  GenPtr _y;
  GenPtr _inf;

 public:

 GenPtr& x0()    { return _x0;   }
 GenPtr& x1()    { return _x1;   }
 GenPtr& y()     { return _y;    }
 GenPtr& info()  { return _inf;  }

 h_segment()
{ 
   _x0 = _x1 = _y = _inf = 0;
 }

 h_segment(GenPtr x0, GenPtr x1, GenPtr y, GenPtr i=0)
{ 
   _x0  = x0;
   _x1  = x1;
   _y   = y;
   _inf = i;
 }

 LEDA_MEMORY(h_segment)

 friend ostream& operator<<(ostream&, h_segment&);
 friend class __exportC Segment_Tree;
 friend class __exportC seg_node_tree;

};


/*------------------------------------------------------------------
   class seg_node_tree = Dictionary< h_segment_p, void* >
-------------------------------------------------------------------*/

class __exportC seg_node_tree : public bb1_tree {

public:

Segment_Tree* father;

int cmp(GenPtr hs_p1, GenPtr hs_p2) const;

list<h_segment_p> query(GenPtr y0, GenPtr y1) const;

list<seg_nd_tr_item> all_items() const;

bool           defined(h_segment_p hs_p) const { return bb1_tree::member(leda_cast(hs_p)); }
seg_nd_tr_item lookup(h_segment_p hs_p)  const { return bb1_tree::lookup(leda_cast(hs_p)); }
seg_nd_tr_item locate(h_segment_p hs_p)  const { return bb1_tree::locate(leda_cast(hs_p)); }
seg_nd_tr_item ord(int n)                const { return bb1_tree::ord(n); }
seg_nd_tr_item insert(h_segment_p hs_p, GenPtr i=0) { return bb1_tree::insert(leda_cast(hs_p),i); } 

seg_nd_tr_item locate_y(GenPtr y) const;

h_segment_p& key(seg_nd_tr_item it)   
{ if (!it) LEDA_EXCEPTION(1,"seg_nd_tr_item is nil");
  //return (h_segment_p&)it->key ; 
  return LEDA_ACCESS(h_segment_p,it->key);
}

GenPtr& info(seg_nd_tr_item it)  { return key(it)->info(); } 

void    change_inf(seg_nd_tr_item it, GenPtr i) { key(it)->info() = i; }

void del(h_segment_p hs_p)       { delete bb1_tree::del(leda_cast(hs_p)); } 
void del_item(seg_nd_tr_item it)  { del(key(it)); } 

seg_node_tree(Segment_Tree* p)   { father = p; }
virtual ~seg_node_tree()  {}

friend class __exportC Segment_Tree;

private:

h_segment_p   key(seg_nd_tr_item it)  const { return (h_segment_p)it->key; }
const GenPtr& info(seg_nd_tr_item it) const { return key(it)->info(); } 

static h_segment_p get_h_segment_p(seg_nd_tr_item it) { return (h_segment_p) (it->key); }
} ;



//------------------------------------------------------------------
// class Segment_Tree
//------------------------------------------------------------------

typedef h_segment_p seg_tree_item;

class __exportC Segment_Tree  : public bb1_tree {

  virtual  h_segment_p new_y_h_segment(GenPtr)
  { LEDA_EXCEPTION(1,"Segment_Tree::new_y_h_segment"); return 0; }

  virtual int cmp_dim1(GenPtr,GenPtr) const { return 0; }
  virtual int cmp_dim2(GenPtr,GenPtr) const { return 0; }

  virtual void clear_dim1(GenPtr&) {}
  virtual void clear_dim2(GenPtr&) {}
  virtual void clear_info(GenPtr&) {}

  virtual void copy_dim1(GenPtr&)  {}
  virtual void copy_dim2(GenPtr&)  {}
  virtual void copy_info(GenPtr&)  {}

  int seg_cmp(h_segment_p hsp1, h_segment_p hsp2);

  void lrot(bb1_item , bb1_item);
  void rrot(bb1_item , bb1_item);
  void ldrot(bb1_item , bb1_item);
  void rdrot(bb1_item , bb1_item);

  GenPtr&        key(bb1_item it)       
                 { if (!it) LEDA_EXCEPTION(1,"bb1_item in segment_tree is nil");
	               return it->key; }
  seg_node_list& info(bb1_item it)      { return (seg_node_list&)(bb1_tree::inf(it)); } 

  GenPtr        key(bb1_item it)  const { return it->key; }
  seg_node_list info(bb1_item it) const { return (seg_node_list)(bb1_tree::inf(it)); } 

  bool empty(bb1_item);
  void clear(bb1_item& );
  void print(bb1_item , string); 

protected:

  seg_node_tree r;                // tree with all segments
  slist<GenPtr> deleted_coord1;   // for lazy deletion of coord1 entries

  int start_coord(bb1_item& x,seg_nd_tr_item& it)
      { return (!cmp(key(x),x0(it))); }

  int end_coord(bb1_item& x,seg_nd_tr_item& it)
      { return (!cmp(key(x),x1(it))); }


public:
  
  int cmp(GenPtr, GenPtr)  const
  { LEDA_EXCEPTION(1,"Segment_Tree::cmp"); return 0; }

  GenPtr& x0 (h_segment_p hsp)  { return hsp->_x0; }
  GenPtr& x1 (h_segment_p hsp)  { return hsp->_x1; }
  GenPtr& y  (h_segment_p hsp)  { return hsp->_y; }
  GenPtr& inf(h_segment_p hsp)  { return hsp->_inf; }

  GenPtr x0(seg_tree_item it) const { return it->_x0;  }
  GenPtr x1(seg_tree_item it) const { return it->_x1;  }
  GenPtr y (seg_tree_item it) const { return it->_y;   }

  //GenPtr&       inf(seg_tree_item it)       { return it->_inf; }
  const GenPtr& inf(seg_tree_item it) const { return it->_inf; }
  void          change_inf(seg_tree_item it, GenPtr i) { it->_inf = i; }

  seg_tree_item insert(GenPtr, GenPtr, GenPtr, GenPtr i=0 );

  void del(GenPtr, GenPtr, GenPtr);
  void del_item(seg_tree_item it) { del(x0(it),x1(it),y(it)) ; }

  seg_tree_item lookup(GenPtr x0, GenPtr x1, GenPtr y) const;
  bool member(GenPtr x0, GenPtr x1, GenPtr y) const { return ( lookup(x0,x1,y) !=0 ); }

  list<seg_tree_item> query(GenPtr, GenPtr, GenPtr ) const;
  list<seg_tree_item> query_sorted(GenPtr, GenPtr, GenPtr ) const;
  list<seg_tree_item> x_infinity_query(GenPtr, GenPtr ) const;
  list<seg_tree_item> y_infinity_query(GenPtr ) const;
  list<seg_tree_item> y_infinity_query_sorted(GenPtr ) const;
  list<seg_tree_item> all_items() const;

  void clear_tree();
  void clear() { clear_tree(); }

  Segment_Tree();
  virtual ~Segment_Tree();

  int size() const             { return r.size(); }
  bool empty() const           { return (r.size()==0); }

  seg_tree_item y_min() const  { return to_seg_tree_item_safe(r.min()); }
  seg_tree_item y_max() const  { return to_seg_tree_item_safe(r.max()); }

  void          init_iterator() const { r.init_iterator(); }
  seg_tree_item move_iterator() const { return to_seg_tree_item_safe(r.move_iterator()); }

  void print_tree() { print(root,""); }

private:
  GenPtr x0(seg_nd_tr_item i) const { return seg_node_tree::get_h_segment_p(i)->_x0;  }
  GenPtr x1(seg_nd_tr_item i) const { return seg_node_tree::get_h_segment_p(i)->_x1;  }
  GenPtr y (seg_nd_tr_item i) const { return seg_node_tree::get_h_segment_p(i)->_y;   }

  static seg_tree_item to_seg_tree_item(seg_nd_tr_item i)
  { return seg_node_tree::get_h_segment_p(i); }

  static seg_tree_item to_seg_tree_item_safe(seg_nd_tr_item i)
  { return i != nil ? to_seg_tree_item(i) : nil; }

  friend class __exportC seg_node_tree;
  friend class           Adaptor_cmp_dim2_to_leda_cmp; // local class (no export)
};

#define forall_seg_tree_items(a,b) \
for ((b).init_iterator(); (a=(b).move_iterator()) != 0; )


//------------------------------------------------------------------
// class segment_tree<class coord1, class coord2, class itype>
//------------------------------------------------------------------

template <class type1, class type2, class itype>

class segment_tree : public Segment_Tree {

leda_cmp_base<type1>  cmp_def1;
leda_cmp_base<type2>  cmp_def2;

h_segment_p new_y_h_segment(GenPtr y)
{ GenPtr p; LEDA_CREATE(type1,p);
  GenPtr q; LEDA_CREATE(type2,q);
  return new h_segment(p,q,y);
 }

int cmp_dim1(GenPtr x,GenPtr y) const { return LEDA_COMPARE(type1,x,y); }
int cmp_dim2(GenPtr x,GenPtr y) const { return LEDA_COMPARE(type2,x,y); }

void clear_dim1(GenPtr& x)     { LEDA_CLEAR(type1,x); }
void clear_dim2(GenPtr& x)     { LEDA_CLEAR(type2,x); }
void clear_info(GenPtr& x)     { LEDA_CLEAR(itype,x); }

void copy_dim1(GenPtr& x)     { LEDA_COPY(type1,x); }
void copy_dim2(GenPtr& x)     { LEDA_COPY(type2,x); }
void copy_info(GenPtr& x)     { LEDA_COPY(itype,x); }

int cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(type1,x,y);}

public:

const type1& x0(seg_tree_item it) const
{ return LEDA_CONST_ACCESS(type1,Segment_Tree::x0(it)); }

const type1& x1(seg_tree_item it) const
{ return LEDA_CONST_ACCESS(type1,Segment_Tree::x1(it)); }

const type2&  y(seg_tree_item it) const
{ return LEDA_CONST_ACCESS(type2,Segment_Tree::y(it));  }

const itype& inf(seg_tree_item it) const
{ return LEDA_CONST_ACCESS(itype,Segment_Tree::inf(it));}

seg_tree_item insert(type1 x0, type1 x1, type2 y, itype i)
{ return Segment_Tree::insert(leda_cast(x0),leda_cast(x1),leda_cast(y),leda_cast(i)); }

void del(type1 x0, type1 x1, type2 y)
{ Segment_Tree::del(leda_cast(x0),leda_cast(x1),leda_cast(y)); }

seg_tree_item lookup(type1 x0, type1 x1, type2 y) const
{ return Segment_Tree::lookup(leda_cast(x0),leda_cast(x1),leda_cast(y)); }

bool member(type1 x0, type1 x1, type2 y)
{ return Segment_Tree::member(leda_cast(x0),leda_cast(x1),leda_cast(y)); }

list<seg_tree_item> query(type1 x,type2 y0,type2 y1) const
{ return Segment_Tree::query(leda_cast(x),leda_cast(y0),leda_cast(y1)); }

list<seg_tree_item> query_sorted(type1 x,type2 y0,type2 y1) const
{ return Segment_Tree::query_sorted(leda_cast(x),leda_cast(y0),leda_cast(y1)); }

list<seg_tree_item> x_infinity_query(type2 y0,type2 y1) const
{ return Segment_Tree::x_infinity_query(leda_cast(y0),leda_cast(y1)); }

list<seg_tree_item> y_infinity_query(type1 x) const
{ return Segment_Tree::y_infinity_query(leda_cast(x)); }

list<seg_tree_item> y_infinity_query_sorted(type1 x) const
{ return Segment_Tree::y_infinity_query_sorted(leda_cast(x)); }


segment_tree()  {}
virtual ~segment_tree() { clear(); }

} ;


LEDA_END_NAMESPACE

#endif
