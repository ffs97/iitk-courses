/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  slist.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_IMPL_SLIST_H
#define LEDA_IMPL_SLIST_H

//------------------------------------------------------------------------------
// simply linked lists
//------------------------------------------------------------------------------

/*{\Manpage {slist} {E} {Singly Linked Lists}}*/

#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE


template <class E> class slist {

/*{\Mdefinition
An instance $L$ of the parameterized data type |\Mname| is a sequence of items
(|slist<E>::item|). Each item in $L$ contains an element of data type $E$, 
called the element or value type of $L$. The number of items in $L$ is called 
the length of $L$. If $L$ has length zero it is called the empty list. In the 
sequel $\Litem{x}$ is used to denote a list item containing the element $x$ and 
$L[i]$ is used to denote the contents of list item $i$ in $L$.}*/

  class slink {
    friend class slist<E>;
    slink* succ;
    E      inf;
    slink(const E& i,slink* s) : succ(s), inf(i) {}
    LEDA_MEMORY(slink)
  };


  slink* h;                     //head
  slink* t;                     //tail
  int    count;                 //length of List

public:

/*{\Mtypes 5}*/

typedef slink* item;
/*{\Mtypemember the item type.}*/

typedef E value_type;
/*{\Mtypemember the value type.}*/


// additional type definitions ...
typedef value_type*         pointer;
typedef const value_type*   const_pointer;
typedef value_type&         reference;
typedef const value_type&   const_reference;
typedef int                 size_type;
typedef int                 difference_type;


/*{\Mcreation L }*/

slist() : h(0), t(0), count(0) {}
/*{\Mcreate creates  an instance |\Mvar| of type |\Mname| and initializes it to
            the empty list.}*/

slist(const E& x) : h(0),t(0),count(0) { append(x); }
/*{\Mcreate creates  an instance |\Mvar| of type |\Mname| and initializes it to
            the one-element list $\Litem{x}$. }*/


slist(const slist& x)
{ h=t=0; 
  count = 0; 
  for (item p = x.h; p; p = p->succ) append(p->inf); 
}

slist& operator=(const slist& x)
{ if (this == &x) return *this;
  clear();
  for (item p = x.h; p; p = p->succ) append(p->inf); 
  return *this;
}

virtual ~slist() { clear(); }


/*{\Moperations 2 5 }*/

int length() const {return count;}
/*{\Mop      returns the length of $L$.}*/

int size() const {return length();}
/*{\Mop      returns $L$.length().}*/

bool empty() const { return h==0;}
/*{\Mop      returns true if $L$ is empty, false otherwise.}*/

item first() const { return h; }
/*{\Mop      returns the first item of $L$.}*/

item last() const { return t; }
/*{\Mop      returns the last item of $L$.}*/

item succ(item it) const { return it->succ; }
/*{\Mop      returns the successor item of item $it$, nil if
             $it=L$.last().\\
             \precond $it$ is an item in $L$.}*/

item cyclic_succ(item l) const { return l->succ? l->succ : h; }
/*{\Mop      returns the cyclic successor of item $it$, i.e.,
             $L$.first() if $it = L$.last(), $L$.succ($it$) otherwise.}*/

const E& contents(item it) const { return it->inf; }
/*{\Mop      returns the contents $L[it]$ of item $it$.\\
             \precond $it$ is an item in $L$.}*/

const E& inf(item it) const { return contents(it); }
/*{\Mop      returns $L$.contents($it$).\\
             \precond $it$ is an item in $L$.}*/


const E& head() const 
{ if (h==0) LEDA_EXCEPTION(1,"slist<E>::head: empty list");
  return h->inf; 
}
/*{\Mop      returns the first element of $L$, i.e. the contents
             of $L$.first().\\
             \precond $L$ is not empty.}*/

const E& tail() const 
{ if (h==0) LEDA_EXCEPTION(1,"slist<E>::tail: empty list");
  return t->inf; 
}
/*{\Mop      returns the last element of $L$, i.e. the contents
             of $L$.last().\\
             \precond $L$ is not empty.}*/


item push(const E& x)   
{ count++;
  h = new slink(x,h);
  if (t==0) t = h;
  return h;
}
/*{\Mop      adds a new item $\Litem{x}$ at the front of $L$ and returns it. }*/


item append(const E& x)
{ count++;
  item q = new slink(x,0);
  if (t) t->succ = q; else h = q;
  t = q;
  return q;
} 
/*{\Mop      appends a new item $\Litem{x}$ to $L$ and returns it. }*/



item insert(const E& x, item pos)
{ count++;
  item q = new slink(x,pos->succ);
  pos->succ = q;
  if (t == pos) t = q;
  return q;
}
/*{\Mopl     inserts a new item $\Litem{x}$ after item $pos$ into $L$ and
             returns it.\\
             \precond $it$ is an item in $L$.}*/
                                                    


E pop()    
{ if (h == 0) LEDA_EXCEPTION(1,"slist: pop on empty list.");
  item q = h; 
  h = q->succ; 
  if (--count == 0) t = 0;
  E x = q->inf;
  delete q;
  return x;
}
/*{\Mop      deletes the first item from $L$ and returns its contents. \\
             \precond $L$ is not empty.}*/


void del_succ_item(item it)    
{ item q = it->succ;
  if (q)
  { if (q == t) t = it;
    it->succ = q->succ; 
    delete q;
    count--;
  }
}
/*{\Mop  deletes the successor of item $it$ from $L$.\\
         \precond $it$ is an item in $L$ and has a successor. }*/


void conc(slist<E>& L)
{ if (L.count == 0) return;
  if (count > 0) t->succ = L.h;
  else h = L.h; 
  t = L.t; 
  count = count+L.count;
  L.h = L.t = 0;
  L.count = 0;
}
/*{\Mop      appends list $L_1$ to list $L$ and makes $L_1$ the empty list.\\
             \precond $L\ !=\ L_1$.}*/

void clear()
{ if (h==0) return;
  int id = LEDA_TYPE_ID(E);
  if (id != INT_TYPE_ID && id != FLOAT_TYPE_ID &&
      id != DOUBLE_TYPE_ID && id != PTR_TYPE_ID) {
    for (item p = h; p; p = p->succ) p->inf.~E();
  }
  std_memory.deallocate_list(h,t,sizeof(slink));
  h=t=0;
  count=0;
}
/*{\Mop    makes $L$ the empty list. }*/



E& operator[](item it) { return it->inf; }
/*{\Marrop   returns a reference to the contents of $it$.}*/

const E& operator[](item it) const { return it->inf; }


item operator+=(const E& x)  { return append(x); }
/*{\Mbinop      appends a new item $\Litem{x}$ to $L$ and returns it. }*/



item first_item()      const { return h; }
item last_item()       const { return t; }
item next_item(item p) const { return p ? p->succ : 0; }



};


LEDA_END_NAMESPACE

#endif
