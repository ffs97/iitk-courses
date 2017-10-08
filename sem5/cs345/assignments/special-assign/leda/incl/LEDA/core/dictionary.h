/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  dictionary.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_DICTIONARY_H
#define LEDA_DICTIONARY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600044
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/impl/ab_tree.h> 

LEDA_BEGIN_NAMESPACE

typedef ab_tree::item dic_item;

/*{\Manpage {dictionary} {K,I} {Dictionaries} {D}}*/

template <class K, class I, class dic_impl = ab_tree>
class dictionary : public dic_impl {


/*{\Mdefinition
An instance $D$ of the parameterized data type |\Mname| is a collection
of items (|dic_item|). Every item in $D$ contains a key from the linearly
ordered data type $K$, called the key type of $D$, and an information from the
data type $I$, called the information type  of $D$. IF $K$ is a user-defined type, you 
have to provide a compare function (see Section~\ref{User Defined Parameter Types}).
The number of items in $D$ is called the size of $D$. A dictionary of size zero is 
called the empty dictionary. We use $\Litem{k,i}$ to denote an item with key $k$ and 
information $i$ ($i$ is said to be the information associated with key $k$).  For each
$k \in K$ there is at most one $i \in I$ with $\Litem{k,i} \in D$.}*/

mutable stl_access_pair<K,I>* value_buf;

int ktype_id;
int key_type_id()            const  { return ktype_id; }


// for implementations based on search trees

const leda_cmp_base<K>* cmp_ptr;

int (*cmp_ptr1)(const K&, const K&);

int cmp(GenPtr x, GenPtr y)  const 
{ if (cmp_ptr1)
     return LEDA_CALL2(*cmp_ptr1,K,x,y);
  else
     return LEDA_CALL2(*cmp_ptr,K,x,y);
}


// for implementations based on hashing 

bool equal_key(GenPtr x, GenPtr y) const { return LEDA_EQUAL(K,x,y); }
int  hash_fct(GenPtr x)            const { return LEDA_HASH(K,x);    }
/*
  cout << "hash_fct: x = " << x << endl; 
  int k = LEDA_CONST_ACCESS(K,x); 
  cout << "hash_fct: k = " << k << endl; 
  int h = Hash(k); 
  cout << "hash_fct: h = " << h << endl; 
  return h; 
*/


void clear_key(GenPtr& x)    const { LEDA_CLEAR(K,x); }
void clear_inf(GenPtr& x)    const { LEDA_CLEAR(I,x); }
void copy_key(GenPtr& x)     const { LEDA_COPY(K,x); }
void copy_inf(GenPtr& x)     const { LEDA_COPY(I,x); }

//void print_key(GenPtr x)     const { LEDA_PRINT(K,x,cout); }
//void print_inf(GenPtr x)     const { LEDA_PRINT(I,x,cout); }

public:
/*{\Mtypes 5}*/ 

typedef typename dic_impl::item item;
/*{\Mtypemember the item type.}*/

typedef typename dic_impl::item dic_item;

typedef K key_type;
/*{\Mtypemember the key type.}*/  

typedef I inf_type;
/*{\Mtypemember the information type.}*/ 


/*{\Mcreation 3 }*/

dictionary(const leda_cmp_base<K>& cmp): cmp_ptr1(0)
{ value_buf = 0;
  cmp_ptr = &cmp; 
  ktype_id = UNKNOWN_TYPE_ID; }

dictionary(int _a, int _b) : dic_impl(_a,_b)
{ value_buf = 0;
  cmp_ptr1 = compare;
  ktype_id = LEDA_TYPE_ID(K); 
 }

dictionary()
{ value_buf = 0;
  cmp_ptr1 = compare;
  ktype_id = LEDA_TYPE_ID(K); }
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| based on the
            linear order defined by the global |compare| function and 
            initializes it with the empty dictionary.}*/

dictionary(int (*cmp)(const K&, const K&))
{ value_buf = 0;
  cmp_ptr1 = cmp; 
  ktype_id = UNKNOWN_TYPE_ID; }
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| based on the
            linear order defined by the compare function |cmp| and 
            initializes it with the empty dictionary.}*/


dictionary(const dictionary<K,I,dic_impl>& D) : dic_impl(D) 
{ value_buf = 0;
  cmp_ptr  = D.cmp_ptr; 
  cmp_ptr1 = D.cmp_ptr1; 
  ktype_id = D.ktype_id; 
}

dictionary<K,I,dic_impl>& operator=(const dictionary<K,I,dic_impl>& D)
{ dic_impl::operator=(D); 
  cmp_ptr  = D.cmp_ptr; 
  cmp_ptr1 = D.cmp_ptr1; 
  ktype_id = D.ktype_id;  
  return *this; 
}
         
virtual ~dictionary()
{ dic_impl::clear();
  if (value_buf) delete value_buf;
}



/*{\Moperations 2 4.2 }*/

const K& key(dic_item it) const 
{ return LEDA_CONST_ACCESS(K,dic_impl::key(it));}

/*{\Mop   returns the key of item $it$.\\
	  \precond $it$ is an item in |\Mvar|.}*/

const I& inf(dic_item it) const
{ return LEDA_CONST_ACCESS(I,dic_impl::inf(it));}
/*{\Mop     returns the information of item $it$.\\
            \precond $it$ is an item in |\Mvar|.}*/
 

/*{\Moptions nextwarning=no}*/
const I& operator[](dic_item it)  const
{ return LEDA_CONST_ACCESS(I,dic_impl::inf(it));}

I& operator[](dic_item it) 
{ return LEDA_ACCESS(I,dic_impl::inf(it));}
/*{\Marrop  returns a reference to the information of item $it$.\\
            \precond $it$ is an item in |\Mvar|.}*/
 



dic_item insert(const K& k, const I& i)
{ return dic_impl::insert(leda_cast(k),leda_cast(i)); } 
/*{\Mop      associates the information $i$ with the key $k$.
             If there is an item $\<k,j\>$ in |\Mvar| then $j$ is
             replaced by $i$, else a new item $\Litem{k,i}$ is added
       	     to |\Mvar|. In both cases the item is returned.}*/

dic_item lookup(const K& k) const
{ return dic_impl::lookup(leda_cast(k));}
/*{\Mop        returns the item with key $k$ (nil if no such
	       item exists in \Mvar).}*/

I access(const K& k)  const { return inf(lookup(k));}
/*{\Mop       returns the information associated with key $k$.\\
              \precond there is an item with key $k$ in |\Mvar|.}*/

void  del(const K& k) { dic_impl::del(leda_cast(k)); } 
/*{\Mop       deletes the item with key $k$ from |\Mvar|
              (null operation, if no such item exists).}*/

void  del_item(dic_item it) { dic_impl::del_item(it); } 
/*{\Mop       removes item $it$ from |\Mvar|.\\
              \precond $it$ is an item in |\Mvar|.}*/

bool  defined(const K& k) const 
{ return (lookup(k) == nil) ? false : true; }
/*{\Mop returns true if there is an item with key $k$ in $D$, false otherwise.}*/

void undefine(const K& k) {dic_impl::del(leda_cast(k)); }
/*{\Mop deletes the item with key $k$from |\Mvar|
              (null operation, if no such item exists).}*/

// void change_inf(dic_item it, const I& i) { operator[](it) = i; }

void  change_inf(dic_item it, const I& i) 
{ dic_impl::change_inf(it,leda_cast(i)); }
/*{\Mopl      makes $i$ the information of item $it$.\\
              \precond $it$ is an item in |\Mvar|.}*/

void  clear() { dic_impl::clear(); }
/*{\Mop       makes |\Mvar| the empty dictionary.}*/ 

int   size()  const { return dic_impl::size(); }
/*{\Mop       returns the size of |\Mvar|.}*/

bool  empty() const { return (size()==0) ? true : false; }
/*{\Mop       returns true if |\Mvar| is empty, false otherwise.}*/

dic_item first_item() const { return dic_impl::first_item(); }
dic_item last_item() const { return dic_impl::last_item(); }
dic_item next_item(dic_item it) const { return dic_impl::next_item(it);}
dic_item pred_item(dic_item it) const { return dic_impl::pred_item(it);}

typedef dictionary<K,I,dic_impl> this_type;


#if defined(LEDA_STL_ITERATORS)

typedef stl_access_pair<K,I> pair_type;

pair_type& stl_item_access(dic_item it) const
{ if (value_buf) delete value_buf;
  value_buf = new pair_type(key(it),operator[](it)); 
  return *value_buf;
}

DEFINE_STL_ITERATORS(this_type,dic_item,pair_type)

#endif

};
//------------------------------------------------------------------------------
// Iteration Macros
//------------------------------------------------------------------------------


/*{\Mtext \headerline{Iteration}

{\bf forall\_items}($it,D$)       
$\{$ ``the items of $D$ are successively assigned to $it$'' $\}$

{\bf forall\_rev\_items}($it,D$)       
$\{$ ``the items of $D$ are successively assigned to $it$ in reverse order'' $\}$

{\bf forall}($i,D$)       
$\{$ ``the informations of all items of $D$ are successively assigned to $i$'' $\}$ 

{\bf forall\_defined}($k,D$)       
$\{$ ``the keys of all items of $D$ are successively assigned to $k$'' $\}$ 
\\\\
STL compatible iterators are provided when compiled with 
\texttt{-DLEDA\_STL\_ITERATORS} (see |LEDAROOT/demo/stl/dic.c| 
for an example).
}*/




/*{\Mimplementation
Dictionaries are implemented by $(2,4)$-trees. Operations 
insert, lookup, del\_item, del take time $O(\log n)$, key, inf, empty, size,
change\_inf take time $O(1)$, and clear takes time $O(n)$. Here $n$ is the 
current size of the dictionary. The space requirement is $O(n)$.}*/ 


/*{\Mexample
We count the number of occurrences of each string in a sequence of strings.

\begin{Mverb}
#include <LEDA/core/dictionary.h>

main()
{ dictionary<string,int> D;
  string s;
  dic_item it;

  while (cin >> s)
  { it = D.lookup(s);
    if (it==nil) D.insert(s,1);
    else D.change_inf(it,D.inf(it)+1);
  }

  forall_items(it,D) cout << D.key(it) << " : " <<  D.inf(it) << endl;

}

\end{Mverb}

}*/



#if LEDA_ROOT_INCL_ID == 600044
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
