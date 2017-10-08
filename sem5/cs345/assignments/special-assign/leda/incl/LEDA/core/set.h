/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  set.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SET_H
#define LEDA_SET_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600024
#include <LEDA/internal/PREAMBLE.h>
#endif


//------------------------------------------------------------------------------
// set             
//------------------------------------------------------------------------------

#include <LEDA/system/basic.h>
#include <LEDA/core/impl/avl_tree.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {set} {E} {Sets}}*/

template<class E, class set_impl = avl_tree>

class set : public set_impl {

/*{\Mdefinition
An instance $S$ of the parameterized data type |\Mname| is a collection of
elements of the linearly ordered type $E$, called the element type of $S$. The
size of $S$ is the number of elements in $S$, a set of size zero is called the
empty set.}*/

int (*cmp_ptr)(const E&, const E&);

int  key_type_id() const { return LEDA_TYPE_ID(E); }

//int  cmp(GenPtr x, GenPtr y) const { return LEDA_COMPARE(E,x,y); }

int  cmp(GenPtr x, GenPtr y) const 
{ return (*cmp_ptr)(LEDA_CONST_ACCESS(E,x),LEDA_CONST_ACCESS(E,y)); }

void clear_key(GenPtr& x)    const { LEDA_CLEAR(E,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(E,x); }
void print_key(GenPtr x)     const { LEDA_PRINT(E,x,cout); }

E buf;

public:

typedef set<E,set_impl> this_type;

typedef typename set_impl::item item;

/*{\Mcreation S }*/

 set(int (*cmp)(const E&, const E&)) { cmp_ptr = cmp; }

 set() { cmp_ptr = compare; }
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes it to 
            the empty set.}*/

set(const set<E,set_impl>& S) : set_impl(S), cmp_ptr(S.cmp_ptr) {}

virtual ~set() { clear(); }

 set<E,set_impl>& operator=(const set<E,set_impl>& S) 
 { set_impl::operator=(S); return *this;}


/*{\Moperations 1.8 5}*/

void insert(const E& x) { set_impl::insert(leda_cast(x),0); }
/*{\Mop        adds $x$ to |\Mvar|.}*/

void del(const E& x) { set_impl::del(leda_cast(x)); }
/*{\Mop        deletes $x$ from |\Mvar|.}*/

bool member(const E& x) const 
{ return set_impl::lookup(leda_cast(x))!=nil; }
/*{\Mop        returns true if $x$ in |\Mvar|, false otherwise.}*/

const E& choose() const 
{ return LEDA_CONST_ACCESS(E,set_impl::key(set_impl::min())); }
/*{\Mop        returns an element of |\Mvar|.\\
               \precond |\Mvar| is not empty.}*/


set<E,set_impl> join(const set<E,set_impl>& T) const
{ set<E,set_impl> tmp = *this;
  tmp.set_impl::add(T); 
  return tmp;
}

/*{\Mop    returns |\Mvar| $\cup$ |T|. }*/

set<E,set_impl> diff(const set<E,set_impl>& T) const
{ set<E,set_impl> tmp = *this;
  tmp.set_impl::subtract(T); 
  return tmp;
}
/*{\Mop    returns |\Mvar| $-$ |T|. }*/


set<E,set_impl> intersect(const set<E,set_impl>& T) const 
{ set<E,set_impl> tmp = *this;
  tmp.set_impl::intersect_with(T); 
  return tmp;
}
/*{\Mop    returns |\Mvar| $\cap$ |T|. }*/

set<E,set_impl> symdiff(const set<E,set_impl>& T) const 
{ set<E,set_impl> tmp = join(T);
  tmp.subtract(intersect(T));
  return tmp;
}
/*{\Mop    returns the symetric difference of |\Mvar| and |T|. }*/




set<E,set_impl>  operator+(const set<E,set_impl>& T) const { return join(T); }
/*{\Mbinop    returns |\Mvar.join(T)|. }*/

set<E,set_impl>  operator-(const set<E,set_impl>& T) const { return diff(T); }
/*{\Mbinop    returns |\Mvar.diff(T)|. }*/

set<E,set_impl>  operator&(const set<E,set_impl>& T) const { return intersect(T); }
/*{\Mbinop    returns |\Mvar.intersect(T)|. }*/

set<E,set_impl>  operator%(const set<E,set_impl>& T) const { return symdiff(T); }
/*{\Mbinop    returns |\Mvar.symdiff(T)|. }*/


set<E,set_impl>& operator+=(const set<E,set_impl>& T) 
{ set_impl::add(T); return *this;}
/*{\Mbinop    assigns |\Mvar.join(T)| to |\Mvar| and returns |\Mvar|. }*/

set<E,set_impl>& operator-=(const set<E,set_impl>& T) 
{ set_impl::subtract(T); return *this;}
/*{\Mbinop    assigns |\Mvar.diff(T)| to |\Mvar| and returns |\Mvar|. }*/

set<E,set_impl>& operator&=(const set<E,set_impl>& T) 
{ set_impl::intersect_with(T); return *this;}
/*{\Mbinop    assigns |\Mvar.intersect(T)| to |\Mvar| and returns |\Mvar|. }*/

set<E,set_impl>& operator%=(const set<E,set_impl>& T) 
{ set<E,set_impl> tmp = *this;
  set_impl::add(T);
  tmp.intersect_with(T);
  subtract(tmp);
  return *this;
}
/*{\Mbinop    assigns |\Mvar.symdiff(T)| to |\Mvar| and returns |\Mvar|. }*/



bool operator<=(const set<E,set_impl>& T) const 
{ return T.set_impl::contains(*this); }
/*{\Mbinop    returns true if $\Mvar \subseteq T$, false otherwise. }*/

bool operator>=(const set<E,set_impl>& T) const 
{ return set_impl::contains(T); }
/*{\Mbinop    returns true if $T \subseteq \Mvar$, false otherwise. }*/

bool operator==(const set<E,set_impl>& T) const 
{ return set_impl::size() == T.set_impl::size() && set_impl::contains(T); }
/*{\Mbinop    returns true if $\Mvar = T$, false otherwise. }*/

bool operator!=(const set<E,set_impl>& T) const 
{ return set_impl::size() != T.set_impl::size() ||  !set_impl::contains(T); }
/*{\Mbinop    returns true if $\Mvar \not= T$, false otherwise. }*/

bool operator<(const set<E,set_impl>& T) const 
{ return set_impl::size() < T.set_impl::size() &&  T.set_impl::contains(*this); }
/*{\Mbinop    returns true if $\Mvar \subset T$, false otherwise. }*/

bool operator>(const set<E,set_impl>& T) const 
{ return set_impl::size() > T.set_impl::size() &&  set_impl::contains(T); }
/*{\Mbinop    returns true if $T \subset \Mvar$, false otherwise. }*/





bool empty() const {return set_impl::empty();}
/*{\Mop        returns true if |\Mvar| is empty, false otherwise.}*/

int size() const {return set_impl::size();}
/*{\Mop        returns the size of |\Mvar|.}*/

void clear() { set_impl::clear(); }
/*{\Mop        makes |\Mvar| the empty set.}*/


// iteration

item  first_item()       const { return set_impl::first_item(); }
item  last_item()        const { return set_impl::last_item(); }
item  next_item(item it) const { return set_impl::next_item(it);}

const E& inf(item it) const 
{ return LEDA_CONST_ACCESS(E,set_impl::key(it)); }

E& access(item it) const
{ //LEDA_EXCEPTION(1,"set::iterator->access() not allowed.");
  (E&)buf = LEDA_CONST_ACCESS(E,set_impl::key(it)); 
  return (E&)buf;
 }

friend ostream& operator<<(ostream& out, const set<E,set_impl>&) { return out; }
friend istream& operator>>(istream& in, set<E,set_impl>&) { return in; }

void print() const { set_impl::print(); }

#if defined(LEDA_STL_ITERATORS)
E&       stl_item_access(item it)       { return access(it); }
const E& stl_item_access(item it) const { return inf(it); }
DEFINE_STL_ITERATORS(this_type,item,E)
#endif

};


/*{\Mtext
\bigskip
{\bf Iteration}

\medskip
{\bf forall}($x,S$) 
$\{$  ``the elements of $S$ are successively assigned to $x$''  $\}$ }*/


/*{\Mimplementation
Sets are implemented by randomized search trees \cite{AS89}. Operations insert,
del, member take time $O(\log n)$, empty, size take time $O(1)$, and clear 
takes time $O(n)$, where $n$ is the current size of the set.

The operations join, intersect, and diff have the 
following running times: Let $S_1$ and $S_2$ be a two sets of type 
\texttt{T} with $\mid S_1\mid =n_1$ and $\mid S_2\mid =n_2$.
Then $S_1$.join($S_2$) and $S_1$.diff($S_2$) need time 
$O(n_2\log(n_1+n_2))$, $S_1$.intersect($S_2$) needs time 
$O(n_1\log(n_1+n_2)$.
}*/


#if LEDA_ROOT_INCL_ID == 600024
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

