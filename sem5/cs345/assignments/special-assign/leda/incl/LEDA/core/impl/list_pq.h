/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  list_pq.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE


class __exportC list_pq_elem
{
  friend class __exportC list_pq;

  GenPtr key;
  GenPtr inf;

  list_pq_elem* succ;
  list_pq_elem* pred;

  list_pq_elem(GenPtr k, GenPtr i, list_pq_elem* p, list_pq_elem* s) 
  { key = k; 
    inf = i; 
    pred = p;
    succ = s;
   }

  LEDA_MEMORY(list_pq_elem)

};


#define PRIO_IMPL_CLASS list_pq
#define PRIO_IMPL_ITEM  list_pq_item

typedef list_pq_elem*   list_pq_item;


#define PRIO_IMPL_DATA  list_pq_item head;\
                        int          count;

LEDA_END_NAMESPACE

#include <LEDA/core/impl/prio_impl.h>

LEDA_BEGIN_NAMESPACE


inline list_pq::list_pq()    { head = nil; count = 0; }
inline list_pq::list_pq(int) { LEDA_EXCEPTION(1,"no list_pq(int) constructor");}
inline list_pq::list_pq(int,int) { LEDA_EXCEPTION(1,"no list_pq(int,int) constructor");}
inline list_pq::~list_pq() { clear(); }

inline list_pq_item list_pq::first_item()              const { return head;   }
inline list_pq_item list_pq::next_item(list_pq_item x) const 
{ return x ? x->succ : 0;}

inline const GenPtr& list_pq::key(list_pq_item it) const { return it->key; }
inline const GenPtr& list_pq::inf(list_pq_item it) const { return it->inf; }

inline void list_pq::del_min()  { del_item(find_min());   }

inline void list_pq::decrease_key(list_pq_item it, GenPtr x)  
{ copy_key(x);
  clear_key(it->key);
  it->key = x; 
 }

inline void list_pq::change_inf(list_pq_item it, GenPtr y)
{ copy_inf(y);
  clear_inf(it->inf);
  it->inf = y; 
 }

inline int  list_pq::size()  const  { return count; }

LEDA_END_NAMESPACE

