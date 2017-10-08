/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  p_dictionary.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_P_DICTIONARY_H
#define LEDA_P_DICTIONARY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600017
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/core/impl/pers_tree.h>

LEDA_BEGIN_NAMESPACE


typedef pers_tree_node* p_dic_item;


template <class K,class I>

class PERS_DIC: public pers_rb_tree, public handle_rep {

void copy_key(GenPtr& x)  const { LEDA_COPY(K,x); }
void copy_inf(GenPtr& x)  const { LEDA_COPY(I,x); }
void clear_key(GenPtr& x) const { LEDA_CLEAR(K,x); }
void clear_inf(GenPtr& x) const { LEDA_CLEAR(I,x); }

//void print_key(GenPtr x)  const { LEDA_PRINT(K,x,cout); }
//void print_inf(GenPtr x)  const { LEDA_PRINT(I,x,cout); }

int  cmp_keys(GenPtr x, GenPtr y) const { return LEDA_COMPARE(K,x,y); }

Version V;

public:

 typedef p_dic_item item;

 PERS_DIC() { init_tree(); V = v_list->vl.first(); }
 PERS_DIC(V_LIST* vl,Version v) { v_list=vl; V=v;  }
 void CLEAR() { if (--v_list->count==0) del_tree(); }
~PERS_DIC() { CLEAR(); }

PERS_DIC(const PERS_DIC<K,I>& D) : handle_rep(D)
{ v_list = D.v_list; v_list->count++; V = D.V; }

PERS_DIC<K,I>& operator=(PERS_DIC<K,I>& D)
{ CLEAR(); v_list = D.v_list; v_list->count++; V = D.V; count = D.count;
  return *this; }

const K& key(p_dic_item p) const 
{ return LEDA_CONST_ACCESS(K,pers_rb_tree::key(p)); }

const I& inf(p_dic_item p) const 
{ return LEDA_CONST_ACCESS(I,pers_rb_tree::inf(p)); }

p_dic_item locate(const K& k) const
{ return pers_rb_tree::locate(leda_cast(k),V); }

p_dic_item locate_pred(const K& k) const
{ return pers_rb_tree::locate_pred(leda_cast(k),V); }

p_dic_item lookup(const K& k) const
{ return pers_rb_tree::lookup(leda_cast(k),V); }

PERS_DIC<K,I>  insert(const K& k, const I& i)
{ return PERS_DIC<K,I>(v_list,pers_rb_tree::insert(leda_cast(k),leda_cast(i),V)); }

PERS_DIC<K,I>  del(const K& k)
{ return PERS_DIC<K,I>(v_list,pers_rb_tree::del(leda_cast(k),V)); }

PERS_DIC<K,I>  change_inf(p_dic_item p, const I& i)
{ return PERS_DIC<K,I>(v_list,pers_rb_tree::change_inf(p,leda_cast(i),V)); }

p_dic_item min() const    { return pers_rb_tree::min(V); }
p_dic_item max() const    { return pers_rb_tree::max(V); }
p_dic_item succ(p_dic_item p) const { return pers_rb_tree::succ(p,V); }
p_dic_item pred(p_dic_item p) const { return pers_rb_tree::pred(p,V); }
int   size()  const { return pers_rb_tree::size(V); }
void  print() const { pers_rb_tree::print(V); }

void  draw(DRAW_NODE_FCT f, DRAW_EDGE_FCT g, double x0, double x1, double y, double dy) const 
{ pers_rb_tree::draw(f,g,V,x0,x1,y,dy); }

double get_version() const { return ver_num(V); }

};



/*{\Manpage {p_dictionary} {K,I} {Persistent Dictionaries}}*/

template <class K, class I, class dummy=PERS_DIC<K,I> >
class p_dictionary : public HANDLE_BASE(dummy) {
/*{\Mdefinition

An instance $D$ of the parameterized data type |\Mname| is a set
of items (type |p_dic_item|). Every item in $D$ contains a key from the
linearly ordered data type $K$, called the key type of $D$, and an information
from data type $I$, called the information type  of $D$. If $K$ is a user-defined
type, you have to provide a compare function (see Section~\ref{User Defined Parameter Types}).The number of items in
$D$ is called the size of $D$. A dictionary of size zero is called empty.
We use $\Litem{k,i}$ to denote an item with key $k$ and information
$i$ ($i$ is said to be the information associated with key $k$).  For each
$k \in K$ there is at most one item $\Litem{k,i} \in D$.

The difference between dictionaries (cf. section \ref{Dictionaries}) and 
persistent dictionaries lies in the fact that update operations performed on 
a persistent dictionary $D$ do not change $D$ but create and return a new 
dictionary $D'$. For example, $D$.del($k$) returns the dictionary 
$D'$ containing all items $it$ of $D$ with key($it$) $\ne$ $k$. Also, 
an assignment $D1 = D2$ does not assign a copy of $D2$ (with new items) to 
$D1$ but the value of $D2$ itself.
}*/

typedef HANDLE_BASE(dummy) base_class;


PERS_DIC<K,I>* ptr() const { return (PERS_DIC<K,I>*)base_class::PTR; }

public:


 typedef p_dic_item item;


/*{\Mcreation D }*/

 p_dictionary()      { base_class::PTR = new PERS_DIC<K,I>; }
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes |\Mvar| to an
    empty persistent dictionary.}*/

 p_dictionary(PERS_DIC<K,I>* p) { base_class::PTR = (PERS_DIC<K,I>*)p; }
 p_dictionary(const p_dictionary<K,I>& p) : HANDLE_BASE(dummy)(p) {}
~p_dictionary()     {}

 p_dictionary<K,I>& operator=(const p_dictionary<K,I>& p)
 { HANDLE_BASE(dummy)::operator=(p); return *this; }


/*{\Moperations 3.8 4}*/

K key(p_dic_item it) const { return ptr()->key(it); }
/*{\Mop        returns the key of item $it$.\\
               \precond $it$ $\in$ |\Mvar|.}*/

I inf(p_dic_item it) const { return ptr()->inf(it); }
/*{\Mop        returns the information of item $it$.\\
	       \precond $it$ $\in$ |\Mvar|.}*/

/*{\Moptions nextwarning=no}*/
p_dic_item locate(const K& k) const { return ptr()->locate(k); }
p_dic_item lookup(const K& k) const { return ptr()->lookup(k); }
/*{\Mop        returns the item with key $k$ (nil if no such
	       item exists in \Mvar).}*/


p_dictionary<K,I> del(const K& k)
{ return new PERS_DIC<K,I>(ptr()->del(k)); }
/*{\Mop        returns $\{x \in |\Mvar| \Lvert\ key(x) \ne k\}$.}*/

p_dictionary<K,I> del_item(p_dic_item it)
{ return new PERS_DIC<K,I>(ptr()->del(ptr()->key(it))); }
/*{\Mopl       returns $\{x \in |\Mvar| \Lvert\ x \ne it\}$.}*/

p_dictionary<K,I> insert(const K& k, const I& i)
{ return new PERS_DIC<K,I>(ptr()->insert(k,i)); }
/*{\Mop        returns |\Mvar|.del($k$) $\cup$ $\{\Litem{k,i}\}$.}*/

p_dictionary<K,I> change_inf(p_dic_item it, const I& i)
{ return new PERS_DIC<K,I>(ptr()->change_inf(it,i)); }
/*{\Mopl       returns |\Mvar|.del\_item($it$) $\cup$
               $\{\Litem{k,i}\}$, where $k = key(it)$.\\
	       \precond $it$ $\in$ |\Mvar|.}*/

p_dic_item min() const   { return ptr()->min();     }
p_dic_item max() const   { return ptr()->max();     }

p_dic_item succ(p_dic_item p) const { return ptr()->succ(p);   }
p_dic_item pred(p_dic_item p) const { return ptr()->pred(p);   }

p_dic_item locate_succ(const K& k) const { return ptr()->locate(k); }
p_dic_item locate_pred(const K& k) const { return ptr()->locate_pred(k); }

p_dic_item first_item()            const { return ptr()->min();     }
p_dic_item next_item(p_dic_item p) const { return p ? ptr()->succ(p) : 0;   }

int   size() const     { return ptr()->size();    }
/*{\Mop        returns the size of |\Mvar|.}*/

bool  empty() const   { return ptr()->size()==0; }
/*{\Mop        returns true if |\Mvar| is empty, false otherwise.}*/


void print() const { ptr()->print(); }

void draw(DRAW_NODE_FCT f,DRAW_EDGE_FCT g,double x0,double x1,double y,double dy) const { ptr()->draw(f,g,x0,x1,y,dy); }

}; 


template <class K, class I>
inline ostream& operator<<(ostream& out, const p_dictionary<K,I>&) 
{ return out; }

template <class K, class I>
inline istream& operator>>(istream&  in, const p_dictionary<K,I>&) 
{ return in; }


/*{\Mimplementation
Persistent dictionaries are implemented by leaf oriented 
persistent red black trees.
Operations insert, lookup, del\_item, del take time $O(\log^2 n)$, key, inf, 
empty, size and change\_inf take time $O(1)$. The space requirement is
$O(1)$ for each update operation.}*/
 

#if LEDA_ROOT_INCL_ID == 600017
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

