/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  m_heap.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_M_HEAP_H
#define LEDA_M_HEAP_H

//------------------------------------------------------------------------------
// m_heap : monotonic heaps 
//
// a) the sequence of minimum keys (over time) is monotonic (non-decreasing)
// b) the difference of minimum and maximum key is bounded by a constant M
//
// Implementation: cyclic array of lists
//
// Stefan Naeher  (1991)
//------------------------------------------------------------------------------


#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE


class __exportC m_heap_elem;
class __exportC m_heap;


typedef m_heap_elem* m_heap_item;

class m_heap_elem {

   GenPtr       inf;
   m_heap_item succ;
   m_heap_item pred;

friend class m_heap;

LEDA_MEMORY(m_heap_elem)
};




class __exportC m_heap {

    m_heap_item T;
    m_heap_item T_last;
    m_heap_item min_ptr;
    int          min_key;
    int          M;
    int          count;

virtual void copy_inf(GenPtr&)  const {}
virtual void clear_inf(GenPtr&) const {}


void insert(m_heap_item, m_heap_elem&);
void remove(m_heap_item);

public:

typedef m_heap_item item;


m_heap_item insert(GenPtr,GenPtr);
m_heap_item find_min() const;
m_heap_item first_item() const;
m_heap_item next_item(m_heap_item) const;

GenPtr       del_min();

void change_key(m_heap_item,GenPtr);
void decrease_key(m_heap_item it,GenPtr x) { change_key(it,x); }
void change_inf(m_heap_item it, GenPtr x)  
{ clear_inf(it->inf);
  it->inf = x; 
  copy_inf(it->inf); 
};

void del_item(m_heap_item);
void clear();

const GenPtr& inf(m_heap_item it) const { return it->inf; }

GenPtr key(m_heap_item) const
{ LEDA_EXCEPTION(1,"m_heap::key not implemented");
  return 0; 
 }

int    size()   const     { return count; }
int    empty()  const     { return count==0; }

void   print() const;

 m_heap(int M=1024);         
 m_heap(int,int) { LEDA_EXCEPTION(1,"illegal constuctor"); }

virtual ~m_heap() { delete[] T; }


// still to do: copy operations

 m_heap& operator=(const m_heap&) { return *this; }
 m_heap(const m_heap&) {}

};

LEDA_END_NAMESPACE

#endif

