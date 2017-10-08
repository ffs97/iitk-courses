/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  pp_bin_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_PP_BTREE_H
#define LEDA_PP_BTREE_H


//------------------------------------------------------------------------------
//
// pp_btree  
//
//     base class for persistent leaf oriented binary trees in LEDA
//
// Sven Thiel (1998); ephemeral version: Stefan Naeher (1993)
//
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/core/part_pers.h>

LEDA_BEGIN_NAMESPACE

class __exportC pp_btree;
class pp_btree_node;

typedef pp_btree_node* pp_btree_item;

typedef void (*PP_DRAW_BIN_NODE_FCT)(double,double,void*,int);
typedef void (*PP_DRAW_BIN_EDGE_FCT)(double,double,double,double);

//------------------------------------------------------------------------------
// class pp_btree_node 
//------------------------------------------------------------------------------

#define PP_BTREE_KEY 2
#define PP_BTREE_INF 3

class pp_btree_node : public LAnchor
{  
   enum { num_ps = 2, num_is = 2, num_rps = 4 };
				 
   friend class pp_btree;
   friend class pp_rb_tree;

   PPersGenPtr<PP_BTREE_KEY> k;       // key
   PPersGenPtr<PP_BTREE_INF> i;       // info

   PPersPtr<pp_btree_node, 0> left;   // node: left and right child
   PPersPtr<pp_btree_node, 1> right;  // leaf: successor and predecessor

   EphPtr<pp_btree_node> parent; // pointer to parent
   EphPtr<pp_btree_node> corr;   // leaf: pointer to corresponding inner node
                                 // inner node: nil

   int   bal;               // rebalancing data

/*
   public:
*/

   pp_btree_node(LPPersBase& base, GenPtr key, GenPtr inf, int b) :
     LAnchor(base, num_ps, num_is, num_rps), k(*this, key), i(*this, inf),
     left(*this), right(*this)
   {
     bal = b;
   }         

   pp_btree_node(LPPersBase& base, int b) :
     LAnchor(base, num_ps, num_is, num_rps), k(*this), i(*this),
     left(*this), right(*this)
   {
     bal = b;
   }         

   pp_btree_node(LPPersBase& base) : 
     LAnchor(base, num_ps, num_is, num_rps), k(*this), i(*this),
     left(*this), right(*this) {}

/*
   pp_btree_node(LPPersBase& base, pp_btree_node* p) : 
     LAnchor(base, num_ps, num_is, num_rps), k(*this, p->k), i(*this, p->i),
     left(*this), right(*this)
   {
     bal = p->bal;
   }         
*/

   PPersPtrRef<pp_btree_node> child(int c) 
   { return c == 0 ? 
       PPersPtrRef<pp_btree_node>(left) : PPersPtrRef<pp_btree_node>(right); } 
   pp_btree_node* child(int c) const
   { return c == 0 ? (pp_btree_node*)left : (pp_btree_node*)right; }

   virtual int get_id() const { return 1; }

/////////////////

   bool is_node() const  { return corr.is_nil(); } 
   bool is_leaf() const  { return !corr.is_nil(); } 

   void set_bal(int x) { bal = x; }
   int  get_bal()      { return bal; }

   LEDA_MEMORY(pp_btree_node)
};

//------------------------------------------------------------------------------
// class pp_btree_pers_data
//------------------------------------------------------------------------------

class pp_btree_pers_data : public Anchor {
public:
  pp_btree_pers_data(LPPersBase& base) : Anchor(base,0,1,0), count(*this) {}

  PPersInfo<int, 0> count;

  virtual void clear_info(GenPtr& p, InfoIndex) 
  { count.clear_info(p); }
};
 
//------------------------------------------------------------------------------
// class pp_btree
//------------------------------------------------------------------------------

class __exportC pp_btree : public LPPersBase
{ 
  enum { num_entry_ps = 2 };

  pp_btree(const pp_btree&);
  pp_btree& operator=(const pp_btree&);

  protected:

  enum { left=0, right=1 };

  PPersEntryPtr<pp_btree_node, 0> ROOT;
                            // "super root" to avoid special cases in rotations
                            // ROOT.left points to real root node
                            // ROOT.right points to leftmost leaf

  PPersEntryPtr<pp_btree_pers_data, 1> pers_data;

  // functions depending on used rebalancing method
  // will be defined in derived classes (pp_rb_tree, avl_tree, ...)

  virtual int leaf_balance() { return 0; }  // default balance value for leaves
  virtual int node_balance() { return 0; }  // inner nodes
  virtual int root_balance() { return 0; }  // root node

  virtual void insert_rebal(pp_btree_node*)   {}
  virtual void del_rebal(pp_btree_node*, pp_btree_node*) {}

  // other protected member functions

  PPersPtrRef<pp_btree_node> min_ptr() const 
  { return ROOT->right; }

  void rotation(pp_btree_node*, pp_btree_node*, int);
  void double_rotation(pp_btree_node*, pp_btree_node*, pp_btree_node*, int);

  void del_tree(pp_btree_node*);

  pp_btree_node* search(GenPtr) const;

  pp_btree_node* copy_tree(pp_btree_node*,pp_btree_node*&,pp_btree&) const;

  // functions depending on actual key type
  // will be defined in dictionary and sortseq templates

  virtual int  cmp(GenPtr x, GenPtr y) const 
                                        { return compare((char*)x,(char*)y); }
  virtual int  type_id()          const { return UNKNOWN_TYPE_ID; }
  virtual void clear_key(GenPtr&) const { }
  virtual void clear_inf(GenPtr&) const { }
  virtual void clear_iinf(GenPtr&)const { }
  virtual void copy_key(GenPtr&)  const { }
  virtual void copy_inf(GenPtr&)  const { }
  virtual void copy_iinf(GenPtr&) const { }
  virtual void print_key(GenPtr, ostream& = cout) const { }
  virtual void print_inf(GenPtr, ostream& = cout) const { }

  virtual void copy_info(GenPtr& p, InfoIndex idx, Anchor& a);
  virtual void clear_info(GenPtr& p, InfoIndex idx, Anchor& a);

public:

  typedef pp_btree_item item;

  PPersPtrRef<pp_btree_node> root() const 
  { return ROOT->left; }

  pp_btree_node* cyclic_succ(pp_btree_node* p) const { return p->right; }
  pp_btree_node* cyclic_pred(pp_btree_node* p) const { return p->left; }

  pp_btree_node* succ(pp_btree_node* p) const
  { return (p->right == min_ptr()) ? 0 : (pp_btree_node*)p->right; }

  pp_btree_node* pred(pp_btree_node* p) const
  { return (p == min_ptr())  ?  0 : (pp_btree_node*)p->left ; }

  pp_btree_node* first_item()  const               { return min_ptr(); }
  pp_btree_node* end_item()    const               { return 0; }
  pp_btree_node* next_item(pp_btree_node* p) const { return p ? succ(p) : 0; }

  pp_btree_node* min() const { return min_ptr(); }
  pp_btree_node* max() const 
  { return (size()>0) ? (pp_btree_node*)min_ptr()->left : 0; }

  pp_btree& conc(pp_btree&);
  void split_at_item(pp_btree_node*,pp_btree&,pp_btree&);
  void reverse_items(pp_btree_node*,pp_btree_node*);

  // insert ruft insert_at_item auf
  pp_btree_node* insert(GenPtr,GenPtr,GenPtr=0);
  pp_btree_node* insert_at_item(pp_btree_node*,GenPtr,GenPtr,GenPtr=0);
  pp_btree_node* lookup(GenPtr) const;
  pp_btree_node* locate(GenPtr) const;
  pp_btree_node* locate_succ(GenPtr) const; 
  pp_btree_node* locate_pred(GenPtr) const; 

  const GenPtr&  key(pp_btree_node* p)  const { return (const GenPtr&)p->k; }
  const GenPtr&  inf(pp_btree_node* p)  const { return (const GenPtr&)p->i; }

  void del(GenPtr);
  void del_item(pp_btree_node* p);
  void change_inf(pp_btree_node*,GenPtr);

  void clear();

  int size()   const { return pers_data->count; }
  int empty()  const { return root() ? false : true ; }

// set operations

  void add(const pp_btree&);
  void intersect_with(const pp_btree&);
  void subtract(const pp_btree&);
  //void symdiff(const pp_btree&);

  bool contains(const pp_btree& T) const;

  // construction, assignment, destruction

  pp_btree() : LPPersBase(num_entry_ps), ROOT(*this), pers_data(*this)
  {  
    pers_data = new pp_btree_pers_data(*this);
    ROOT = new pp_btree_node(*this);
    ROOT->corr = nil;
    min_ptr() = nil; 
  }

  virtual ~pp_btree() {}

  // additional operations used by range and segment trees

  virtual void propagate_modification(int, GenPtr, GenPtr) {}

  pp_btree_node* l_child(pp_btree_node* p) const
  { return p->is_leaf() ? 0 : (pp_btree_node*)p->left; }

  pp_btree_node* r_child(pp_btree_node* p) const
  { return p->is_leaf() ? 0 : (pp_btree_node*)p->right; }

  int is_inner(pp_btree_node* p)  const
  { return (p->corr).is_nil(); }

  pp_btree_node* parent(pp_btree_node* p)  const
  { return (p==root()) ? 0 : (pp_btree_node*)p->parent; }

  // miscellaneous

  void draw(PP_DRAW_BIN_NODE_FCT, PP_DRAW_BIN_NODE_FCT, PP_DRAW_BIN_EDGE_FCT, 
            pp_btree_node*, double, double, double, double, double) const;

  void draw(PP_DRAW_BIN_NODE_FCT, PP_DRAW_BIN_NODE_FCT, PP_DRAW_BIN_EDGE_FCT, 
            double, double, double, double) const;

  void print() const;
  void print_tree(pp_btree_node*,int) const;

  int depth() const { return depth(root()); }
  int depth(const pp_btree_node* r) const;

  friend ostream& operator<<(ostream& out, const pp_btree&) { return out; }
  friend istream& operator>>(istream& in, pp_btree&) { return in; }

public: // supporting functions for persistency

  static pp_btree_item convert_pers_item(pers_item it)
  { return it==0 ? 0 : &((pp_btree_node&)it->get_anchor()); }

  pers_item pers_insert(Version& v, GenPtr k, GenPtr i)
  { 
    begin_update(v); 
    pers_item it = convert_item(insert(k, i)); 
    end_update(); 
    return it;
  }

  void pers_del(Version& v, GenPtr k)
  { begin_update(v); del(k); end_update(); }

  void pers_del_item(Version& v, pers_item it)
  { 
    begin_update_at(v, it); 
    del_item(convert_pers_item(it)); 
    end_update(); 
  }

  pers_item pers_change_inf(Version& v, pers_item it, GenPtr i)
  { 
    begin_update_at(v, it);
    pp_btree_item pp_bt_it = convert_pers_item(it); 
    change_inf(pp_bt_it, i); // durch kopieren kann sich it "andern
    it = convert_item(pp_bt_it); 
    end_update(); 
    return it;
  }

  void pers_clear(Version& v)
  { begin_update(v); clear(); end_update(); }

  int pers_size(const Version& v) const 
  { access_version(v); return size(); }

  pers_item pers_lookup(const Version& v, GenPtr p) const
  { access_version(v); return convert_item(lookup(p)); }

  pers_item pers_locate(const Version& v, GenPtr p) const
  { access_version(v); return convert_item(locate(p)); }

  pers_item pers_locate_succ(const Version& v, GenPtr p) const
  { access_version(v); return convert_item(locate_succ(p)); }

  pers_item pers_locate_pred(const Version& v, GenPtr p) const
  { access_version(v); return convert_item(locate_pred(p)); }

  pers_item pers_min(const Version& v) const 
  { access_version(v); return convert_item(min()); }
  pers_item pers_max(const Version& v) const 
  { access_version(v); return convert_item(max()); }

  const GenPtr& pers_key(const Version&, pers_item it) const 
  { return it->info(PP_BTREE_KEY); }
  const GenPtr& pers_inf(const Version&, pers_item it) const 
  { return it->info(PP_BTREE_INF); }

  pers_item pers_succ(const Version& v, pers_item p) const
  { access_version_at(v, p); return convert_item(succ(convert_pers_item(p))); }
  pers_item pers_pred(const Version& v, pers_item p) const
  { access_version_at(v, p); return convert_item(pred(convert_pers_item(p))); }

  int pers_depth(const Version& v) const 
  { access_version(v); return depth(); }

  void pers_print(const Version& v) const 
  { access_version(v); print(); }

  void pers_draw(const Version& v, 
       PP_DRAW_BIN_NODE_FCT f, PP_DRAW_BIN_NODE_FCT g, PP_DRAW_BIN_EDGE_FCT h, 
       double x0, double x1, double y, double dy) const 
  { access_version(v); draw(f,g,h,x0,x1,y,dy); }
};

inline void pp_btree::rotation(pp_btree_node* p,pp_btree_node* q, int dir)
{ pp_btree_node* r = q->child(1-dir);
  pp_btree_node* x = p->parent;
  p->child(dir) = r;
  q->child(1-dir) = p;
  p->parent = q;
  r->parent = p;
  if (p == x->left)
     x->left = q;
  else
     x->right = q;
  q->parent = x;

  propagate_modification(4,p,r);
  propagate_modification(5,q,p);
  if( x != ROOT )
    propagate_modification(6,x,q);
 }

inline void pp_btree::double_rotation(pp_btree_node* p, pp_btree_node* q, 
                                      pp_btree_node* r, int dir1)
{ int dir2 = 1-dir1;
  pp_btree_node* s = r->child(dir1);
  pp_btree_node* t = r->child(dir2);
  pp_btree_node* x = p->parent;
  p->child(dir1) = t;
  q->child(dir2) = s;
  r->child(dir1) = q;
  r->child(dir2) = p;
  p->parent = r;
  q->parent = r;
  s->parent = q;
  t->parent = p;

  if (p == x->left)
     x->left = r;
  else
     x->right = r;

  r->parent = x;

  propagate_modification(7,p,t);
  propagate_modification(8,q,s);
  propagate_modification(9,r,p);
  propagate_modification(10,r,q);
  if( x != ROOT )
    propagate_modification(11,x,r);
}

LEDA_END_NAMESPACE

#endif

