/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  ab_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_AB_TREE_H
#define LEDA_AB_TREE_H

//------------------------------------------------------------------------------
// (a,b)-trees 
//------------------------------------------------------------------------------

#include<LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

class __exportC ab_tree_node;

class __exportC ab_tree_elem {

GenPtr key;
ab_tree_node* child;

friend class __exportC ab_tree;
friend class __exportC ab_tree_node;

};


class __exportC ab_tree_node {

friend class __exportC ab_tree;

  static atomic_counter id_count;

  int height;
  int size;
  unsigned long id;

  ab_tree_node* father;

  ab_tree_elem* last;
  ab_tree_elem  arr[1];

  ab_tree_node*& succ() { return arr[0].child; }
  ab_tree_node*& pred() { return arr[1].child; }

  GenPtr& key() { return arr[0].key; }
  GenPtr& inf() { return arr[1].key; }

friend unsigned long ID_Number(ab_tree_node* p) { return p->id; }

friend inline ab_tree_node* new_ab_tree_node(int,int,ab_tree_node*,int);

}; 
  


class __exportC ab_tree   
{
    int a;
    int b;

    int height;             /* height of tree   */
    int count;              /* number of leaves */

    ab_tree_node* root;
    ab_tree_node* minimum;  
    ab_tree_node* maximum;

    ab_tree_elem* expand(ab_tree_node*, ab_tree_node*, ab_tree_node*, int);
    ab_tree_elem* index(ab_tree_node* v, ab_tree_node* u);
    ab_tree_elem* same(ab_tree_node* v);

    GenPtr shrink(ab_tree_node* v, ab_tree_elem* pos);

    void split_node(ab_tree_node* v);
    void share(ab_tree_node* v,ab_tree_node* y,int direct);
    void fuse (ab_tree_node* v,ab_tree_node* w);
    void del_tree(ab_tree_node* localroot);
    void exchange_leaves(ab_tree_node*, ab_tree_node*);
    void flip_leaves(ab_tree_node*, ab_tree_node*, ab_tree_node*);
    void pr_ab_tree(ab_tree_node*,int) const;

    void remove_item(ab_tree_node*);


    ab_tree_node* copy_ab_tree(ab_tree_node*,ab_tree_node*&,int) const;

    virtual int key_type_id() const { return UNKNOWN_TYPE_ID; }

    virtual int cmp(GenPtr, GenPtr) const { return 0; }
    virtual void clear_key(GenPtr&) const {}
    virtual void clear_inf(GenPtr&) const {}
    virtual void copy_key(GenPtr&)  const {}
    virtual void copy_inf(GenPtr&)  const {}
    virtual void print_key(GenPtr)  const {}
    virtual void print_inf(GenPtr)  const {}


public:

    void* owner; // pointer to data type object (e.h. sortseq)


    typedef ab_tree_node* item;

    void clear();

    const GenPtr& key(ab_tree_node* p)  const { return p->key(); }
    GenPtr& inf(ab_tree_node* p)        const { return p->inf(); }

    ab_tree_node* insert(GenPtr, GenPtr);
    ab_tree_node* insert_item_at_item(ab_tree_node*,ab_tree_node*, int);
    ab_tree_node* insert_at_item(ab_tree_node*, GenPtr, GenPtr, int);
    ab_tree_node* insert_at_item(ab_tree_node*, GenPtr, GenPtr);
    ab_tree_node* locate_succ(GenPtr) const;
    ab_tree_node* locate_pred(GenPtr) const;
    ab_tree_node* locate(GenPtr,bool&) const;
    ab_tree_node* locate(GenPtr x) const { return locate_succ(x); }
    ab_tree_node* lookup(GenPtr) const;

    void del(GenPtr);
    void del_item(ab_tree_node*);
    void change_inf(ab_tree_node*, GenPtr);
    void reverse_items(ab_tree_node*, ab_tree_node*);

    void conc(ab_tree&, int);
    void split_at_item(ab_tree_node*, ab_tree&, ab_tree&, int);

    void del_min() { if (minimum) del_item(minimum); }

    void decrease_key(ab_tree_node* p, GenPtr k);

    bool member(GenPtr k)  const { return (lookup(k))? true: false ; }

    ab_tree_node* min()                      const { return minimum; }
    ab_tree_node* min_item()                 const { return minimum; }
    ab_tree_node* max_item()                 const { return maximum; }
    ab_tree_node* find_min()                 const { return minimum; }
    ab_tree_node* max()                      const { return maximum; }
    ab_tree_node* first_item()               const { return minimum; }
    ab_tree_node* last_item()                const { return maximum; }
    ab_tree_node* next_item(ab_tree_node* p) const { return p ? p->succ() : 0; }
    ab_tree_node* pred_item(ab_tree_node* p) const { return p ? p->pred() : 0; }

    ab_tree_node* succ(ab_tree_node* p)      const { return p->succ(); }
    ab_tree_node* pred(ab_tree_node* p)      const { return p->pred(); }

    ab_tree_node* stl_next_item(ab_tree_node* p) const 
    { return p ? p->succ() : minimum; }

    ab_tree_node* stl_pred_item(ab_tree_node* p) const 
    { return p ? p->pred() : maximum; }

    int  size()  const { return count;       }
    bool empty() const { return (count==0) ? true : false;   }
    void print() const { pr_ab_tree(root,1); }

    ab_tree(int a=2, int b=16); 

    ab_tree(const ab_tree& T);

    ab_tree& operator=(const ab_tree&);
 
    virtual ~ab_tree(){ clear(); }

    static ab_tree* my_collection(ab_tree_node*) { return 0; }
 };

LEDA_END_NAMESPACE

#endif
