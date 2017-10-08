/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  pp_dictionary.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_PP_DICTIONARY_H
#define LEDA_PP_DICTIONARY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600046
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/impl/pp_rb_tree.h>

LEDA_BEGIN_NAMESPACE

// Standard compare function, but no indirection through another pointer!
template <class K>
class PP_DICT_STD_COMPARE {
public:
  int operator() (const K& k1, const K& k2) const { return compare(k1, k2); }
};


typedef pp_rb_tree::pers_item pp_dic_item;

template <class K,class I, class CMP>
class pp_dic_rep: private pp_rb_tree {

  void copy_key(GenPtr& x)  const { LEDA_COPY(K,x); }
  void copy_inf(GenPtr& x)  const { LEDA_COPY(I,x); }
  void clear_key(GenPtr& x) const { LEDA_CLEAR(K,x); }
  void clear_inf(GenPtr& x) const { LEDA_CLEAR(I,x); }
  void print_key(GenPtr x, ostream& o)  const { LEDA_PRINT(K,x,o); }
  void print_inf(GenPtr x, ostream& o)  const { LEDA_PRINT(I,x,o); }

  int  cmp(GenPtr x, GenPtr y) const { return LEDA_CALL2(Compare,K,x,y); }

  CMP Compare;

public:

  typedef pp_dic_item item;
  typedef pp_rb_tree::Version Version;
  
  pp_dic_rep(const CMP& cmp) : Compare(cmp) {}
  ~pp_dic_rep() { pp_rb_tree::destroy(); }

  pp_rb_tree::Version get_empty_version() const
  { return pp_rb_tree::get_empty_version(); }

  item insert(Version& v, const K& k, const I& i)
  { return pp_rb_tree::pers_insert(v, leda_cast(k),leda_cast(i)); }

  void del(Version& v, const K& k)
  { pp_rb_tree::pers_del(v, leda_cast(k)); }

  void del_item(Version& v, item p)
  { pp_rb_tree::pers_del_item(v, p); }

  item change_inf(Version& v, item p, const I& i)
  { return pp_rb_tree::pers_change_inf(v, p,leda_cast(i)); }

  void clear(Version& v)
  { pp_rb_tree::pers_clear(v); }

  const K& key(const Version& v, item p) const 
  { return LEDA_CONST_ACCESS(K, pp_rb_tree::pers_key(v, p)); }

  const I& inf(const Version& v, item p) const 
  { return LEDA_CONST_ACCESS(I, pp_rb_tree::pers_inf(v, p)); }

  item locate(const Version& v, const K& k) const
  { return pp_rb_tree::pers_locate(v, leda_cast(k)); }

  item locate_pred(const Version& v, const K& k) const
  { return pp_rb_tree::pers_locate_pred(v, leda_cast(k)); }

  item locate_succ(const Version& v, const K& k) const
  { return pp_rb_tree::pers_locate_succ(v, leda_cast(k)); }

  item lookup(const Version& v, const K& k) const
  { return pp_rb_tree::pers_lookup(v, leda_cast(k)); }

  int size(const Version& v)  const { return pp_rb_tree::pers_size(v); }

  int depth(const Version& v) const { return pp_rb_tree::pers_depth(v); }

  item min(const Version& v) const { return pp_rb_tree::pers_min(v); }
  item max(const Version& v) const { return pp_rb_tree::pers_max(v); }
  item succ(const Version& v,item p) const {return pp_rb_tree::pers_succ(v,p);}
  item pred(const Version& v,item p) const {return pp_rb_tree::pers_pred(v,p);}

  void  print(const Version& v) const { pp_rb_tree::pers_print(v); }
  void  draw(const Version& v, 
	PP_DRAW_BIN_NODE_FCT f, PP_DRAW_BIN_NODE_FCT g, PP_DRAW_BIN_EDGE_FCT h,
        double x0, double x1, double y, double dy) const 
  { pp_rb_tree::pers_draw(v,f,g,h,x0,x1,y,dy); }

  typedef pp_dic_rep<K,I,CMP> self;
  LEDA_MEMORY(self)
};

/*{\Manpage {pp_dictionary} {K,I,CMP} {Partially Persistent Dictionaries} {D}}*/

template <class K, class I, class CMP = PP_DICT_STD_COMPARE<K> >
class pp_dictionary {
/*{\Mdefinition

A partially persistent dictionary stores the history of a set of items 
$\<k,i\>$ (type |pp_dic_item|).
Here |k| is a key from a linearly ordered type |K|, and |i| is an information 
from the data type |I|. An instance |\Mvar| of the parameterized data type 
|\Mname| references a version of the set in the history. \\
An update operation may only be performed on an instance |\Mvar| containing the
newest version |v|. The operation changes the set of items and creates a new 
version $v'$ which represents the new state of the set. After the operation, 
|\Mvar| references version $v'$, and all other instances which have referenced
version |v| remain unchanged. 
An access operation may be applied to any instance |\Mvar|. Let |v| denote the
version referenced by |\Mvar|. Then the operation is carried out on the set 
which existed when |v| was created. \\
The main advantage of a partially persistent dictionary compared to a 
dictionary is that copy and assignment operations use only constant time and 
space, because only the versions and not the whole sets are assigned. 

The linear order for the data type |K| is given by a compare object of type 
|CMP|. It must provide a function call operator which has the same syntax and 
semantics as a LEDA |compare| function (see Section~\ref{User Defined Parameter Types}). 
The data type |\Mtype| may be instantitiated without specifying the third 
template argument, i.e. as type $|\Mtype|\<K,I\>$. 
This data type uses the linear order which is defined by the |compare| function
for the type |K|.
}*/

public:
 typedef pp_dic_item item;
 typedef typename pp_dic_rep<K,I,CMP>::Version Version;

private:
  Version v;

  pp_dic_rep<K,I,CMP>* ptr() const 
  { return (pp_dic_rep<K,I,CMP>*)((void*) v.get_base()); }

public:

/*{\Mcreation D }*/

 explicit

 pp_dictionary(const CMP& cmp=CMP()) 
 { 
   pp_dic_rep<K,I,CMP>* pp_dic_ptr = new pp_dic_rep<K,I,CMP>(cmp); 
   v = pp_dic_ptr->get_empty_version();
 }
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes 
  |\Mvar| to an empty persistent dictionary. \\
  When the argument |cmp| is provided, the object is used as compare function 
  object which defines the linear order for the data type |K|.
}*/

 ~pp_dictionary() {}

 pp_dictionary(const pp_dictionary<K,I,CMP>& p) : v(p.v) {}

 pp_dictionary<K,I,CMP>& operator=(const pp_dictionary<K,I,CMP>& p)
 { v=p.v; return *this; }

/*{\Moperations 3.8 4}*/

/*{\Mtext
\headerline{3.1 Update operations}
}*/

pp_dic_item insert(const K& k, const I& i) { return ptr()->insert(v,k,i); }
/*{\Mop        associates the information $i$ with the key $k$.
               If there is an item $\<k,j\>$ in the set then it is
               deleted. A new item $\<k,i\>$ is added and returned.}*/

void del(const K& k) { ptr()->del(v, k); }
/*{\Mop        deletes the item with key $k$ from the set
               (null operation, if no such item exists).}*/

void del_item(pp_dic_item it) { ptr()->del_item(v, it); }
/*{\Mop        removes item $it$ from the set.\\
               \precond $it$ is an item in the set.}*/

pp_dic_item change_inf(pp_dic_item it, const I& i)
{ return ptr()->change_inf(v,it,i); }
/*{\Mopl       makes $i$ the information of item $it$.
               This may create a new item; the item $\<|key|(|it|),i\>$ in
	       the new set is returned.\\
               \precond $it$ is an item in the set.}*/

void clear()
{ ptr()->clear(v); }
/*{\Mop        makes |\Mvar| the empty dictionary.}*/ 

/*{\Mtext
\headerline{3.2 Access operations}
}*/

const K& key(pp_dic_item it) const { return ptr()->key(v, it); }
/*{\Mop        returns the key of item $it$.\\
               \precond $it$ $\in$ |\Mvar|.}*/

const I& inf(pp_dic_item it) const { return ptr()->inf(v, it); }
/*{\Mop        returns the information of item $it$.\\
	       \precond $it$ $\in$ |\Mvar|.}*/

pp_dic_item lookup(const K& k) const { return ptr()->lookup(v, k); }
/*{\Mop        returns the item with key $k$ (nil if no such
	       item exists in \Mvar).}*/

pp_dic_item locate(const K& k) const { return ptr()->locate(v, k); }

pp_dic_item locate_succ(const K& k) const { return ptr()->locate_succ(v, k); }
/*{\Mop        returns the item |x| in |\Mvar| so that 
  $|key(x)|=\min\{|key(y)|\Lvert\ y \in |\Mvar| \ \wedge \ |key(y)| \geq k \}$.
  }*/
pp_dic_item locate_pred(const K& k) const { return ptr()->locate_pred(v, k); }
/*{\Mop        returns the item |x| in |\Mvar| so that 
  $|key(x)|=\max\{|key(y)|\Lvert\ y \in |\Mvar| \ \wedge \ |key(y)| \leq k \}$.
  }*/

pp_dic_item min() const   { return ptr()->min(v);     }
pp_dic_item max() const   { return ptr()->max(v);     }

pp_dic_item succ(pp_dic_item p) const { return ptr()->succ(v, p);   }
pp_dic_item pred(pp_dic_item p) const { return ptr()->pred(v, p);   }

pp_dic_item first_item()            const { return ptr()->min(v); }
pp_dic_item next_item(pp_dic_item p) const { return p ? ptr()->succ(v, p):0; }

int size() const     { return ptr()->size(v); }
/*{\Mop        returns the size of |\Mvar|.}*/

bool empty() const   { return ptr()->size(v)==0; }
/*{\Mop        returns true if |\Mvar| is empty, false otherwise.}*/

int depth() const { return ptr()->depth(v); }

void print() const { ptr()->print(v); }

void draw(PP_DRAW_BIN_NODE_FCT f,PP_DRAW_BIN_NODE_FCT g,PP_DRAW_BIN_EDGE_FCT h,
	  double x0, double x1, double y, double dy) const 
{ ptr()->draw(v,f,g,h,x0,x1,y,dy); }
}; 

template <class K, class I, class CMP>
inline ostream& operator<<(ostream& out, const pp_dictionary<K,I,CMP>&) 
{ return out; }

template <class K, class I, class CMP>
inline istream& operator>>(istream&  in, const pp_dictionary<K,I,CMP>&) 
{ return in; }

/*{\Mimplementation
Partially persistent dictionaries are implemented by leaf oriented 
partially persistent red-black trees.
Operations insert, del, del\_item and lookup take expected time $O(\log n)$, 
the expected time for key, inf, empty, size and change\_inf is $O(1)$, and 
clear takes expected time $O(n)$. 
Here $n$ is the number of items in the version of the dictionary to which the 
operation is applied.
The expected space requirement is $O(1)$ for each update operation.}*/

#if LEDA_ROOT_INCL_ID == 600046
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

