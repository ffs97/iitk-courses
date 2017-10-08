/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  r_heap.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



/* This file has been automatically generated from "r_heap.w"
   by CTANGLE (Version 3.1 [km2c]) */

#ifndef LEDA_RHEAP_H
#define LEDA_RHEAP_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 350983
#include <LEDA/internal/PREAMBLE.h>
#endif




#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE



class __exportC r_heap_node {
  friend class __exportC r_heap;

  GenPtr key;			/* key */
  GenPtr inf;			/* information */
  int bucket;			/* number of bucket containing the node */
  r_heap_node *succ, *pred;	/* pointers for list maintenance */

 public:

  r_heap_node(GenPtr k, GenPtr i):
   key(k), inf(i), bucket(0), succ(nil),
   pred(nil) {
  }

   LEDA_MEMORY(r_heap_node)
};

typedef r_heap_node *r_heap_item;




class __exportC r_heap {
/* data kept in an |r_heap| */
  int C;			/* maximal difference between two keys in the
				 * heap */
  r_heap_item *buckets;		/* buckets of the |r_heap| */
  int *u;			/* upper bounds on the key intervals
				 * corresponding to the buckets */
  int B;			/* number of buckets */
  int si;			/* current number of elements stored in the
				 * heap */
  int *bsize;			/* table used to (re-)initialize the array
				 * |u| or part of it */

/* private functions that facilitate the descriptions of the |r_heap| operations */

  inline void set_bucket_bounds(int min, int upto);
  inline int findbucket(r_heap_item, int);
  void copy_heap(const r_heap &);
  inline void add_item(r_heap_item, int);
  inline void rm_item(r_heap_item);

/* non-public functions concerned with the use of |r_heap| within LEDA */

  virtual void print_key(GenPtr) const {
  }
  virtual void print_inf(GenPtr) const {
  }
  virtual void clear_key(GenPtr &) const {
  }
  virtual void clear_inf(GenPtr &) const {
  }
  virtual void copy_key(GenPtr &) const {
  }
  virtual void copy_inf(GenPtr &) const {
  }
  virtual int  key_type_id() const { 
    return UNKNOWN_TYPE_ID;
  }

 public:

  typedef r_heap_node* item;

   r_heap(int C);
/* the maximal difference between two keys in the heap needs to be provided upon initialization */

  r_heap() {
    LEDA_EXCEPTION(1, "r_heap: must initialize with int C>=0");
  }
  r_heap(int, int) {
    LEDA_EXCEPTION(1, "r_heap: must initialize with one int C>=0");
  }
  r_heap(const r_heap &);
  r_heap & operator = (const r_heap &);
  virtual ~ r_heap() {
    clear();
  }

  r_heap_item find_min() const;

  r_heap_item insert(GenPtr k, GenPtr i);	/* precondition: |k >=
						 * key(find_min())| */

  void del_item(r_heap_node * x);

  void del_min();

  void decrease_key(r_heap_node * x, GenPtr k);	/* precond.:
						 * |key(find_min())<=k<key(x)|
						 *  */

  void change_inf(r_heap_node * x, GenPtr i);

  const GenPtr& key(r_heap_node * x) const {
    return x->key;
  }

  GenPtr& inf(r_heap_node * x) const {
    return x->inf;
  }

  void clear();
  int size() const;
  int empty() const;

/* functions that are used by the LEDA iteration macros */

  r_heap_item first_item() const;
  r_heap_item next_item(r_heap_item p) const;

  r_heap_item last_item() const { return 0; }

  void print_contents(ostream & chk = cout) const;
};

LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 350983
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif
