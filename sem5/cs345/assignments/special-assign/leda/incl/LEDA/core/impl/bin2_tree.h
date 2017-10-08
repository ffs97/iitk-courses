/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  bin2_tree.h
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

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

 
class bin_tree;
class bin_tree_node;

typedef bin_tree_node* bin_tree_item;

typedef void (*DRAW_BIN_NODE_FCT)(bin_tree_node*,double,double);
typedef void (*DRAW_BIN_EDGE_FCT)(double,double,double,double);


enum { LEDA_BIN_TREE=0, LEDA_AVL_TREE=1, LEDA_BB_TREE=2, LEDA_RB_TREE=3, 
       LEDA_RS_TREE=4, LEDA_NUM_TREES=5 };


//------------------------------------------------------------------------------
// class bin_tree_node 
//------------------------------------------------------------------------------

class __exportC bin_tree_node
{  
   friend class __exportC bin_tree;
   friend class __exportC avl_tree;
   friend class __exportC bb_tree;
   friend class __exportC rb_tree;
   friend class __exportC rs_tree;


   GenPtr   k;              // key
   GenPtr   i;              // info

   bin_tree_node* child[2]; // node: left and right child
                            // leaf: successor and predecessor

   bin_tree_node* parent;   // pointer to parent

   int   bal;               // rebalancing data
 



   bin_tree_node(GenPtr key, GenPtr inf, int b)
   { k = key;
     i = inf; 
     bal = b;
    }         

   bin_tree_node() { }         

   bin_tree_node(bin_tree_node* p)
   { k = p->k;
     i = p->i ;
     bal = p->bal;
    }         

   void set_bal(int x) { bal = x; }

   public:

   int    get_bal() { return bal; }
   GenPtr get_key() { return k; }

   LEDA_MEMORY(bin_tree_node)

};


 
//------------------------------------------------------------------------------
// class bin_tree
//------------------------------------------------------------------------------

class __exportC bin_tree
{ 
  protected:

  enum { left=0, right=1 };

  bin_tree_node ROOT;       // "super root" to avoid special cases in rotations
                            // ROOT.child[left] points to real root node
                            // ROOT.child[right] points to leftmost leaf

  int count;            


  // functions depending on used rebalancing method
  // will be defined in derived classes (rb_tree, avl_tree, ...)


  virtual int node_balance() { return 0; }  // default balance for nodes
  virtual int root_balance() { return 0; }  // root node

  virtual void insert_rebal(bin_tree_node*)   {}
  virtual void del_rebal(bin_tree_node*, bin_tree_node*) {}




public:

  bin_tree_node* get_root() const { return ROOT.child[left]; }
  bin_tree_node* get_min()  const { return ROOT.child[right]; }

protected:

  void set_root(bin_tree_node* p) { ROOT.child[left] = p;  }
  void set_min(bin_tree_node* p)  { ROOT.child[right] = p; }


  void rotation(bin_tree_node*, bin_tree_node*, int);
  void double_rotation(bin_tree_node*, bin_tree_node*, bin_tree_node*, int);

  void del_tree(bin_tree_node*);

  bin_tree_node* search(GenPtr) const;

  bin_tree_node* copy_tree(bin_tree_node*,bin_tree_item&) const;



  // functions depending on actual key type
  // will be defined in dictionary and sortseq templates

  virtual int  cmp(GenPtr x, GenPtr y) const 
                                        { return compare((long)x,(long)y); }
  virtual int  key_type_id()      const { return UNKNOWN_TYPE_ID; }
  virtual void clear_key(GenPtr&) const { }
  virtual void clear_inf(GenPtr&) const { }
  virtual void copy_key(GenPtr&)  const { }
  virtual void copy_inf(GenPtr&)  const { }
  virtual void print_key(GenPtr)  const { }
  virtual void print_inf(GenPtr)  const { }

public:

  void* owner; // pointer to data type object (e.h. sortseq)

  virtual int         tree_type() const { return LEDA_BIN_TREE; }
  virtual const char* tree_name() const { return "Binary Tree"; }


  typedef bin_tree_node* item;



  bin_tree_node* min() const { return get_min(); }
  bin_tree_node* max() const { return (count>0) ? get_min()->child[left] : 0; }


  bin_tree_node* insert(GenPtr,GenPtr);
  bin_tree_node* insert_at_item(bin_tree_node*,GenPtr,GenPtr,GenPtr=0);
  bin_tree_node* lookup(GenPtr) const;
  bin_tree_node* locate(GenPtr) const;
  bin_tree_node* locate(GenPtr,bool&) const;
  bin_tree_node* locate_succ(GenPtr) const; 
  bin_tree_node* locate_pred(GenPtr) const; 

  const GenPtr&  key(bin_tree_node* p)  const { return  p->k; }
  GenPtr&        inf(bin_tree_node* p)  const { return  p->i; }

  void del(GenPtr);
  void del_item(bin_tree_node* p);
  void change_inf(bin_tree_node*,GenPtr);

  void clear();

  int  size()   const { return count; } 
  bool empty()  const { return get_root() ? false : true ; }

// set operations

  void add(const bin_tree&);
  void intersect_with(const bin_tree&);
  void subtract(const bin_tree&);
  //void symdiff(const bin_tree&);

  bool contains(const bin_tree& T) const;




  // construction, assignment, destruction

  bin_tree() {  count = 0; set_root(nil); set_min(nil); }

  bin_tree(const bin_tree&);
  bin_tree& operator=(const bin_tree&);

  virtual ~bin_tree() { clear(); }



  // miscellaneous

  bool is_root(bin_tree_node* p) const { return p == get_root(); }
  int  get_bal(bin_tree_node* p) const { return p->get_bal(); }

  bin_tree_node*  get_last_node() { return last_inner_node; }

  void draw(DRAW_BIN_NODE_FCT, DRAW_BIN_EDGE_FCT, 
            bin_tree_node*, double, double, double, double, double);

  void draw(DRAW_BIN_NODE_FCT, DRAW_BIN_EDGE_FCT, 
            double, double, double, double);

  void print() const;
  void print_tree(bin_tree_node*,int) const;

  friend ostream& operator<<(ostream& out, const bin_tree&) { return out; }
  friend istream& operator>>(istream& in, bin_tree&) { return in; }
};



inline void bin_tree::rotation(bin_tree_node* p,bin_tree_node* q, int dir)
{ bin_tree_node* r = q->child[1-dir];
  bin_tree_node* x = p->parent;
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


inline void bin_tree::double_rotation(bin_tree_node* p, bin_tree_node* q, 
                                      bin_tree_node* r, int dir1)
{ int dir2 = 1-dir1;
  bin_tree_node* s = r->child[dir1];
  bin_tree_node* t = r->child[dir2];
  bin_tree_node* x = p->parent;
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
