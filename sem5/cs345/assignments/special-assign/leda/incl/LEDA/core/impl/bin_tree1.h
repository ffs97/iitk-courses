/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bin_tree1.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_BIN_TREE_H
#define LEDA_BIN_TREE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 350980
#include <LEDA/internal/PREAMBLE.h>
#endif

//------------------------------------------------------------------------------
//
// bin_tree1  
//
//     base class for all leaf oriented binary trees in LEDA
//
// Stefan N"aher (1993)
//
//------------------------------------------------------------------------------


#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

 
class bin_tree1;
class bin_tree1_node;

typedef bin_tree1_node* bin_tree1_item;

typedef void (*DRAW_BIN_NODE_FCT)(bin_tree1_node*,double,double);
typedef void (*DRAW_BIN_EDGE_FCT)(double,double,double,double);


enum { LEDA_BIN_TREE=0, LEDA_AVL_TREE=1, LEDA_BB_TREE=2, LEDA_RB_TREE=3, 
       LEDA_RS_TREE=4, LEDA_NUM_TREES=5 };


//------------------------------------------------------------------------------
// class bin_tree1_node 
//------------------------------------------------------------------------------

class __exportC bin_tree1_node
{  
   friend class __exportC bin_tree1;
   friend class __exportC avl_tree;
   friend class __exportC bb_tree;
   friend class __exportC rb_tree;
   friend class __exportC rs_tree;


   bin_tree1_node* child[2]; // node: left and right child
                            // leaf: successor and predecessor

   bin_tree1_node* parent;   // pointer to parent

   bin_tree1_node* corr;     // leaf: pointer to corresponding inner node
                            // node: nil

   int   bal;               // rebalancing data
 



   bin_tree1_node(int b) : bal(b) {}

   bin_tree1_node() { }         

   bin_tree1_node(bin_tree1_node* p) : bal(p->bal) {}

   void set_bal(int x) { bal = x; }

   public:

   bool is_node() { return (corr == nil);  }
   bool is_leaf() { return (corr != nil); }
   int  get_bal() { return bal; }

   LEDA_MEMORY(bin_tree1_node)

};


 
//------------------------------------------------------------------------------
// class bin_tree1
//------------------------------------------------------------------------------

class __exportC bin_tree1
{ 
  protected:

  enum { left=0, right=1 };

  bin_tree1_node ROOT;       // "super root" to avoid special cases in rotations
                            // ROOT.child[left] points to real root node
                            // ROOT.child[right] points to leftmost leaf

  int count;            

  bin_tree1_node* last_inner_node;  // pointer to last inner node created
                                   // by an insert operation



  // functions depending on used rebalancing method
  // will be defined in derived classes (rb_tree, avl_tree, ...)


  virtual int leaf_balance() { return 0; }  // default balance value for leaves
  virtual int node_balance() { return 0; }  // inner nodes
  virtual int root_balance() { return 0; }  // root node

  virtual void insert_rebal(bin_tree1_node*)   {}
  virtual void del_rebal(bin_tree1_node*, bin_tree1_node*) {}




public:

  bin_tree1_node* get_root() const { return ROOT.child[left]; }
  bin_tree1_node* get_min()  const { return ROOT.child[right]; }

protected:

  void set_root(bin_tree1_node* p) { ROOT.child[left] = p;  }
  void set_min(bin_tree1_node* p)  { ROOT.child[right] = p; }


  void rotation(bin_tree1_node*, bin_tree1_node*, int);
  void double_rotation(bin_tree1_node*, bin_tree1_node*, bin_tree1_node*, int);

  void del_tree(bin_tree1_node*);

  bin_tree1_node* search(GenPtr) const;

  bin_tree1_node* copy_tree(bin_tree1_node*,bin_tree1_item&) const;



  virtual int  cmp(bin_tree1_node* p, GenPtr x) const  = 0;
  virtual int  key_type_id() const = 0;

public:

  void* owner; // pointer to data type object (e.h. sortseq)

  virtual int         tree_type() const { return LEDA_BIN_TREE; }
  virtual const char* tree_name() const { return "Binary Tree"; }


  typedef bin_tree1_node* item;



  bin_tree1_node* cyclic_succ(bin_tree1_node* p) const { return p->child[right]; }
  bin_tree1_node* cyclic_pred(bin_tree1_node* p) const { return p->child[left]; }

  bin_tree1_node* succ(bin_tree1_node* p) const
  { return (p->child[right] == get_min()) ? 0 : p->child[right]; }

  bin_tree1_node* pred(bin_tree1_node* p) const
  { return (p == get_min())  ?  0 : p->child[left] ; }

  bin_tree1_node* min() const { return get_min(); }
  bin_tree1_node* max() const { return (count>0) ? get_min()->child[left] : 0; }

  bin_tree1_node* first_item()  const               { return min(); }
  bin_tree1_node* last_item()   const               { return max(); }
  bin_tree1_node* next_item(bin_tree1_node* p) const { return p ? succ(p) : 0; }
  bin_tree1_node* pred_item(bin_tree1_node* p) const { return p ? pred(p) : 0; }

  bin_tree1_node* stl_pred_item(bin_tree1_node* p) const 
  { return p ? pred(p) : last_item(); }

  bin_tree1_node* stl_next_item(bin_tree1_node* p) const 
  { return p ? succ(p) : first_item(); }

  bin_tree1& conc(bin_tree1&,int);
  void split_at_item(bin_tree1_node*,bin_tree1&,bin_tree1&,int);
  void reverse_items(bin_tree1_node*,bin_tree1_node*);


  bin_tree1_node* insert(bin_tree1_node*);
  bin_tree1_node* insert_at_item(bin_tree1_node*,bin_tree1_node*);
  bin_tree1_node* lookup(GenPtr) const;
  bin_tree1_node* locate(GenPtr) const;
  bin_tree1_node* locate(GenPtr,bool&) const;
  bin_tree1_node* locate_succ(GenPtr) const; 
  bin_tree1_node* locate_pred(GenPtr) const; 

  void del(GenPtr);
  void del_item(bin_tree1_node* p);

  void clear();

  int  size()   const { return count; } 
  bool empty()  const { return get_root() ? false : true ; }

// set operations

  void add(const bin_tree1&);
  void intersect_with(const bin_tree1&);
  void subtract(const bin_tree1&);
  //void symdiff(const bin_tree1&);

  bool contains(const bin_tree1& T) const;




  // construction, assignment, destruction

  bin_tree1() {  count = 0; set_root(nil); set_min(nil); }

  bin_tree1(const bin_tree1&);
  bin_tree1& operator=(const bin_tree1&);

  virtual ~bin_tree1() { clear(); }



  // additional operations used by range and segment trees

/*
  virtual void propagate_modification(int, GenPtr, GenPtr) {}
*/

  bin_tree1_node* l_child(bin_tree1_node* p) const
  { return p->is_leaf() ? 0 : p->child[left]; }

  bin_tree1_node* r_child(bin_tree1_node* p) const
  { return p->is_leaf() ? 0 : p->child[right]; }

  int is_inner(bin_tree1_node* p)  const
  { return p->corr == 0; }

  bin_tree1_node* parent(bin_tree1_node* p)  const
  { return (p==get_root()) ? 0 : p->parent; }


  // miscellaneous

  bool is_leaf(bin_tree1_node* p) const { return p->is_leaf(); }
  bool is_root(bin_tree1_node* p) const { return p == get_root(); }
  int  get_bal(bin_tree1_node* p) const { return p->get_bal(); }

  bin_tree1_node*  get_last_node() { return last_inner_node; }

  void draw(DRAW_BIN_NODE_FCT, DRAW_BIN_EDGE_FCT, 
            bin_tree1_node*, double, double, double, double, double);

  void draw(DRAW_BIN_NODE_FCT, DRAW_BIN_EDGE_FCT, 
            double, double, double, double);

  void print() const;
  void print_tree(bin_tree1_node*,int) const;

  friend ostream& operator<<(ostream& out, const bin_tree1&) { return out; }
  friend istream& operator>>(istream& in, bin_tree1&) { return in; }
};



inline void bin_tree1::rotation(bin_tree1_node* p,bin_tree1_node* q, int dir)
{ bin_tree1_node* r = q->child[1-dir];
  bin_tree1_node* x = p->parent;
  p->child[dir] = r;
  q->child[1-dir] = p;
  p->parent = q;
  r->parent = p;
  if (p == x->child[left])
     x->child[left] = q;
  else
     x->child[right] = q;
  q->parent = x;

  propagate_modification(4,p,r);
  propagate_modification(5,q,p);
  if( x!=&ROOT )
    propagate_modification(6,x,q);
 }


inline void bin_tree1::double_rotation(bin_tree1_node* p, bin_tree1_node* q, 
                                      bin_tree1_node* r, int dir1)
{ int dir2 = 1-dir1;
  bin_tree1_node* s = r->child[dir1];
  bin_tree1_node* t = r->child[dir2];
  bin_tree1_node* x = p->parent;
  p->child[dir1] = t;
  q->child[dir2] = s;
  r->child[dir1] = q;
  r->child[dir2] = p;
  p->parent = r;
  q->parent = r;
  s->parent = q;
  t->parent = p;

  if (p == x->child[left])
     x->child[left] = r;
  else
     x->child[right] = r;

  r->parent = x;

  propagate_modification(7,p,t);
  propagate_modification(8,q,s);
  propagate_modification(9,r,p);
  propagate_modification(10,r,q);
  if( x!=&ROOT )
    propagate_modification(11,x,r);
}



LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 350980
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif
