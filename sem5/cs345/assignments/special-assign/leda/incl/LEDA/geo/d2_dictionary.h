/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d2_dictionary.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_d2_dictionary_H
#define LEDA_d2_dictionary_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600061
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/range_tree.h>

LEDA_BEGIN_NAMESPACE

typedef rt_item dic2_item;


/*{\Manpage {d2_dictionary} {K1,K2,I} {Two-Dimensional Dictionaries} }*/ 

template<class K1, class K2, class I>
class d2_dictionary : public range_tree {


/*{\Mdefinition
An instance $D$ of the parameterized data type |\Mname| is a
collection of items ($dic2\_item$). Every item in $D$ contains a key from the
linearly ordered data type $K1$, a key from the linearly ordered data type $K2$,
and an information from data type $I$. $K1$ and $K2$ are called the key types
of $D$, and $I$ is called the information type of $D$. If $K1$ resp. $K2$ are user-defined
types, you have to provide a compare function for type $K1$ resp. $K2$ (see Section
\ref{User Defined Parameter Types}). The number
of items in $D$ is called the size of $D$. A two-dimensional dictionary of size
zero is said to be  empty. We use $\<k_1,k_2,i\>$ to denote the item with first
key $k_1$, second key $k_2$, and information $i$. For each pair
$(k_1,k_2) \in K1 \times K2$ there is at most one item $\<k_1,k_2,i\> \in D$.
Additionally to the normal dictionary operations, the data type $d2\_dictionary$
supports rectangular range queries on $K1\times K2$.}*/

  // redefine the virtual functions of class range_tree
 
  void rt_clear_key(GenPtr*& x) const { 
    LEDA_CLEAR(K1,x[0]); LEDA_CLEAR(K2,x[1]); 
  }

  void rt_copy_key(GenPtr*& x) const { 
    LEDA_COPY(K1,x[0]); LEDA_COPY(K2,x[1]); 
  }

/*
  void rt_print_key(int d,GenPtr*& x) const { 
    if( d==0 ) LEDA_PRINT(K1,x[0],cout);
    else       LEDA_PRINT(K2,x[1],cout);
  }
*/
  
  void rt_clear_inf(GenPtr& x) const { LEDA_CLEAR(I,x);}
  void rt_copy_inf(GenPtr& x)  const { LEDA_COPY(I,x);}
  
  int rt_cmp(int d,GenPtr* x,GenPtr* y) const { 
    if( d==0 ) return LEDA_COMPARE(K1,x[0],y[0]);
    else       return LEDA_COMPARE(K2,x[1],y[1]);
  }
  
  range_tree* new_range_tree(int /*dim*/, int l ) { 
    return new d2_dictionary<K1,K2,I>(l); 
  }

  public:
  
/*{\Mcreation D }*/  

d2_dictionary(int l=0) : range_tree(2,l) {}

/*{\Moptions nextwarning=no }*/

/*
d2_dictionary();
*/

/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes |\Mvar| to 
            the empty dictionary.}*/

~d2_dictionary() { clear(); }



/*{\Moperations 2.8 5.0 }*/
 
const K1& key1(dic2_item it) const { return LEDA_CONST_ACCESS(K1,it->key(0)); }
/*{\Mop    returns the first key of item $it$.\\
	   \precond $it$ is an item in |\Mvar|.}*/

const K2& key2(dic2_item it) const { return LEDA_CONST_ACCESS(K2,it->key(1)); }
/*{\Mop  returns the second key of item $it$.\\
	 \precond $it$ is an item in |\Mvar|.}*/

const I&  inf(dic2_item it) const  { return LEDA_CONST_ACCESS(I,it->inf());}
/*{\Mop     returns the information of item $it$.\\
	    \precond $it$ is an item in |\Mvar|.}*/

/*{\Moptions nextwarning=no}*/
const I& operator[](dic2_item it)  const { return inf(it); }

I& operator[](dic2_item it) { return LEDA_ACCESS(I,it->inf()); }
/*{\Marrop  returns a reference to the information of item $it$.\\
            \precond $it$ is an item in |\Mvar|.}*/
 
    
    dic2_item min_key1() const { return range_tree::rt_min(0); }
/*{\Mop   returns the item with minimal first key.}*/

    dic2_item min_key2() const { return range_tree::rt_min(1); }
/*{\Mop   returns the item with minimal second key.}*/

    dic2_item max_key1() const { return range_tree::rt_max(0); }
/*{\Mop    returns the item with maximal first key.}*/

    dic2_item max_key2() const { return range_tree::rt_max(1); }
/*{\Mop   returns the item with maximal second key.}*/
    
    dic2_item insert(const K1& x, const K2& y, const I& i) { 
      dic2_item p = new rt_elem(leda_copy(x),leda_copy(y),leda_copy(i));
      return range_tree::insert(p);
     }
/*{\Mopl  associates the information $i$ with the keys $x$
	  and $y$. If there is an item \<$x,y,j$\> in 
	  |\Mvar| then $j$ is replaced by $i$, else a new item 
	  \<$x,y,i$\> is added to $D$. In both cases the
	  item is returned.}*/

    dic2_item lookup(const K1& x, const K2& y) const
    { rt_elem p(leda_cast(x), leda_cast(y),0);
      return range_tree::lookup(&p);
    }
/*{\Mopl   returns the item with keys $x$ and $y$ 
          (nil if no such item exists in \Mvar).}*/
    
list<dic2_item> range_search(const K1& x0, const K1& x1, const K2& y0, const K2& y1)  const
{  rt_elem p(leda_cast(x0),leda_cast(y0),0); 
   rt_elem q(leda_cast(x1),leda_cast(y1),0);
   return range_tree::query(&p,&q); }
/*{\Mopl    returns the list of all items \<$k_1,k_2,i$\> in 
	    |\Mvar| with $x_0\le k_1 \le x_1$ and $y_0\le k_2 \le y_1$.}*/
    
list<dic2_item> all_items() const { return range_tree::all_items(); }
/*{\Mop     returns the list of all items of |\Mvar|.}*/

void del(const K1& x, const K2& y)  
    { rt_elem p(leda_cast(x),leda_cast(y),0);
      range_tree::del(&p);
    }
/*{\Mop       deletes the item with keys $x$ and $y$ 
              from |\Mvar|.}*/
    
void del_item(dic2_item it) { range_tree::del(it); }
/*{\Mop       removes item $it$ from |\Mvar|.\\
	     \precond $it$ is an item in |\Mvar|.}*/

void  change_inf(dic2_item it, const I& i) {
    LEDA_CLEAR(I,it->inf());
    it->inf() = leda_copy(i);
    }
/*{\Mopl      makes $i$ the information of item $it$.\\
	     \precond $it$ is an item in |\Mvar|.}*/

void clear(){range_tree::clear();}
/*{\Mop    makes |\Mvar| the empty d2\_dictionary.}*/

bool empty() const {return range_tree::empty();}
/*{\Mop   returns true if |\Mvar| is empty, false otherwise.}*/

int size() const {return range_tree::size();}
/*{\Mop    returns the size of |\Mvar|.}*/


};


// iteration macro
// 
#define forall_dic2_items(x,T)  (T).init_iteration(); forall(x,(T).L )

/*{\Mimplementation
Two-dimensional dictionaries are implemented by dynamic two-dimensional range
trees \cite{Wi85,Lu78} based on BB[$\alpha$] trees. Operations insert, lookup, 
del\_item, del take time $O(\log^2 n)$,  range\_search takes time 
$O(k + \log^2 n)$, where $k$ is the size of the returned list, key, inf, 
empty, size, change\_inf take time $O(1)$, and clear takes time $O(n\log n)$.
Here $n$ is the current size of the dictionary. The space requirement is 
$O(n\log n)$.}*/


#if LEDA_ROOT_INCL_ID == 600061
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
