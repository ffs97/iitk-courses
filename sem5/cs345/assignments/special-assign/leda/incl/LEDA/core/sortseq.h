/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sortseq.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SORTSEQ_H
#define LEDA_SORTSEQ_H 

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600043
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/system/basic.h>
#include <LEDA/core/impl/skiplist.h>

LEDA_BEGIN_NAMESPACE

typedef skiplist::item seq_item;

/*{\Manpage {sortseq} {K,I} {Sorted Sequences} {S}}*/

template<class K, class I, class seq_impl = skiplist>

class sortseq : public seq_impl {

/*{\Mdefinition
 
An instance |S| of the parameterized data type |\Mname| is a sequence of
items (|seq_item|). Every item contains a key from a linearly ordered data
type |K|, called the key type of |S|, and an information from a data type |I|,
called the information type  of |S|. If $K$ is a user-defined type, you have to
provide a compare function (see Section~\ref{User Defined Parameter Types}).
The number of items in |S| is called the size of |S|. A sorted sequence of size zero 
is called empty. We use
\Litem{ |k|,|i|} to
denote a |seq_item| with key |k| and information |i|
(called the information associated with key |k|). 
For each |k| in |K| there is at most one item
\Litem{ |k|,|i|} in |S| and if item \Litem{ |k1|,|i1|}
precedes item \Litem{|k2|,|i2|} in |S| then |k1 < k2|.
 
Sorted sequences are a very powerful data type. They can do everything that
dictionaries and priority queues can do. They also support many other operations,
in particular \emph{finger
searches} and operations |conc|, |split|, |merge|, |reverse_items|,
 and |delete_subsequence|.
 
The key type |K| must be linearly ordered. The 
linear order on |K| may change over time subject to the condition 
that the order of the elements that are currently in the
sorted sequence remains stable. More precisely,
whenever an operation (except for |reverse_items|) is applied to a
sorted sequence |S|, the keys of |S| must form an increasing sequence according
to the currently valid linear order on |K|. For operation |reverse_items| this
must hold after the execution of the operation. An application of sorted sequences
where 
the linear order on the keys 
evolves over time is the plane sweep algorithm for line segment
intersection. This algorithm sweeps an arrangement of segments by a
vertical sweep line and keeps the intersected segments in a
sorted sequence sorted
according to the y-coordinates of their intersections with the sweep line. For
intersecting segments this order depends on the position of the sweep line.

Sorted sequences support finger searches. A finger search takes an item |it| 
in a sorted sequence and a key |k| and searches for the 
key in the sorted sequence containing the item. The cost of a finger 
search is proportional to the 
logarithm of the distance of the key from the start of the search. A finger
search does not need to know the sequence containing the item.
We use |IT| to denote the sequence
containing |it|. In a call
|S.finger_search(it,k)| the types of $S$ and |IT| must agree but $S$ may
or may not be the sequence containing |it|. 

%\input sortseq.funcs

}*/
  

  const leda_cmp_base<K>* cmp_ptr;

  int (*cmp_ptr1)(const K&, const K&);
  
  int cmp(GenPtr x, GenPtr y)  const 
  { if (cmp_ptr1)
       return (*cmp_ptr1)(LEDA_CONST_ACCESS(K,x),LEDA_CONST_ACCESS(K,y)); 
    else
       return (*cmp_ptr)(LEDA_CONST_ACCESS(K,x),LEDA_CONST_ACCESS(K,y)); 
  }
  
  mutable stl_access_pair<K,I>* value_buf;

  int ktype_id;
  int key_type_id () const { return ktype_id; }

  void clear_key(GenPtr& x) const  { LEDA_CLEAR(K,x); }
  void clear_inf(GenPtr& x) const  { LEDA_CLEAR(I,x); }
  void copy_key(GenPtr& x)  const  { LEDA_COPY(K,x);  }
  void copy_inf(GenPtr& x)  const  { LEDA_COPY(I,x);  }

  void print_key(ostream& out, GenPtr x)  const  { LEDA_PRINT(K,x,out);  }
//void print_inf(ostream& out, GenPtr x)  const  { LEDA_PRINT(I,x,out);  }

public:
  
  /*{\Xoptions outfile=sortseq.funcs}*/
  /*{\Mtypes 5}*/  

  typedef typename seq_impl::item item;
  /*{\Mtypemember the item type |seq_item|.}*/ 

  typedef typename seq_impl::item seq_item;


  typedef K key_type;
  /*{\Mtypemember the key type |K|.}*/   

  typedef I inf_type;
  /*{\Mtypemember the information type |I|.}*/  


  /*{\Mcreation 3 }*/
   
  sortseq (const leda_cmp_base<K> &cmp)
  { value_buf = 0;
    cmp_ptr  = &cmp;
    cmp_ptr1 = 0;
    ktype_id = UNKNOWN_TYPE_ID;
    seq_impl::owner = this;
  }

  sortseq ()
  { value_buf = 0;
    cmp_ptr1 = compare;
    ktype_id = LEDA_TYPE_ID(K);
    seq_impl::owner = this;
  }
  /*{\Mcreate creates an instance |\Mvar| of type |\Mname| based on the
     linear order defined by the global |compare| function and 
     and initializes it to the empty sorted sequence. }*/

  sortseq (int (*cmp) (const K &, const K &))
  { value_buf = 0;
    cmp_ptr1 = cmp;
    ktype_id = UNKNOWN_TYPE_ID;
    seq_impl::owner = this;
  }
  /*{\Mcreate creates an instance |\Mvar| of type |\Mname| based on the
     linear order defined by the compare function |cmp| and 
     initializes it with the empty sorted sequence. }*/

  sortseq<K,I,seq_impl> &operator=(const sortseq<K,I,seq_impl> &S1)
  { seq_impl::operator=(S1);
    cmp_ptr  = S1.cmp_ptr;
    cmp_ptr1 = S1.cmp_ptr1;
    ktype_id = S1.ktype_id;
    return *this;
  }


  sortseq (const sortseq<K,I,seq_impl> &S1):seq_impl (S1)
  { value_buf = 0;
    cmp_ptr  = S1.cmp_ptr;
    cmp_ptr1 = S1.cmp_ptr1;
    ktype_id = S1.ktype_id;
    seq_impl::owner = this;
  }


  /*{\Moperations 2.8 5.0 }*/

  /*{\Mtext \settowidth{\typewidth}{|seq_item|}
            \setlength{\colsep}{1em}
            \addtolength{\typewidth}{\colsep}
            \computewidths}*/


  const K& key(seq_item it) const 
  { return LEDA_CONST_ACCESS(K,seq_impl::key(it)); }
  /*{\Mop returns the key of item |it|. }*/

  const I& inf(seq_item it) const
  { return LEDA_CONST_ACCESS(I,seq_impl::inf(it)); }
  /*{\Mop returns the information of item |it|.}*/


// insertion by sn:

  const I& operator[](seq_item it) const
  { return LEDA_CONST_ACCESS(I,seq_impl::inf(it)); }

  I& operator[](seq_item it)
  { return LEDA_ACCESS(I,seq_impl::inf(it));}
  /*{\Marrop  returns a reference to the information of item $it$.\\
              \precond $it$ is an item in |\Mvar|.}*/

// end of insertion by sn


  seq_item lookup(const K& k) const  
  { return seq_impl::lookup(leda_cast(k)); }
  /*{\Mop returns the item with key |k| (|nil| if there is no such item).}*/


  seq_item finger_lookup(const K& k) const   
                { return seq_impl::finger_lookup(leda_cast(k)); }
  /*{\Mop        equivalent to |S.lookup(k)| }*/

  seq_item finger_lookup_from_front(const K& k) const   
                { return seq_impl::finger_lookup_from_front(leda_cast(k)); }
  /*{\Mop        equivalent to |S.lookup(k)| }*/

  seq_item finger_lookup_from_rear(const K& k) const   
                { return seq_impl::finger_lookup_from_rear(leda_cast(k)); }
  /*{\Mop        equivalent to |S.lookup(k)| }*/

  seq_item locate(const K& k) const  
                       { return seq_impl::locate(leda_cast(k)); }
  /*{\Mop        returns the item \Litem{|k1|,|i|} in |S| such that
                |k1| is minimal with $|k1| \ge k$ (|nil| if no
                such item exists).}*/

  seq_item finger_locate(const K& k) const   
           { return seq_impl::finger_locate_succ(leda_cast(k)); }
  /*{\Mop        equivalent to |S.locate(k)| }*/

  seq_item finger_locate_from_front(const K& k) const   
           { return seq_impl::finger_locate_succ_from_front(leda_cast(k)); }
  /*{\Mop        equivalent to |S.locate(k)| }*/

  seq_item finger_locate_from_rear(const K& k) const   
           { return seq_impl::finger_locate_succ_from_rear(leda_cast(k)); }
  /*{\Mop        equivalent to |S.locate(k)| }*/

  seq_item locate_succ(const K& k) const  
                  { return seq_impl::locate_succ(leda_cast(k)); }
  /*{\Mop        equivalent to |S.locate(k)| }*/

  seq_item succ(const K& k) const  { return locate_succ(k); }
  /*{\Mop        equivalent to |S.locate(k)| }*/

  seq_item finger_locate_succ(const K& k) const  
          { return seq_impl::finger_locate_succ(leda_cast(k)); }
  /*{\Mop        equivalent to |S.locate(k)| }*/

  seq_item finger_locate_succ_from_front(const K& k) const  
          { return seq_impl::finger_locate_succ_from_front(leda_cast(k)); }
  /*{\Mop        equivalent to |S.locate(k)| }*/

  seq_item finger_locate_succ_from_rear(const K& k) const  
          { return seq_impl::finger_locate_succ_from_rear(leda_cast(k)); }
  /*{\Mop        equivalent to |S.locate(k)| }*/

  seq_item locate_pred(const K& k) const  
                 { return seq_impl::locate_pred(leda_cast(k)); }
  /*{\Mop  returns the item \Litem{|k1|,|i|} in |S|  such that
          |k1| is maximal with $|k1| \le k$ ( |nil| if no such item exists). }*/

  seq_item pred(const K& k) const   { return locate_pred(k); }
  /*{\Mop  equivalent to |S.locate_pred(k)| }*/

  seq_item finger_locate_pred(const K& k) const 
          { return seq_impl::finger_locate_pred(leda_cast(k)); }
  /*{\Mop  equivalent to |S.locate_pred(k)| }*/

  seq_item finger_locate_pred_from_front(const K& k) const 
          { return seq_impl::finger_locate_pred_from_front(leda_cast(k)); }
  /*{\Mop  equivalent to |S.locate_pred(k)| }*/

  seq_item finger_locate_pred_from_rear(const K& k) const 
          { return seq_impl::finger_locate_pred_from_rear(leda_cast(k)); }
  /*{\Mop  equivalent to |S.locate_pred(k)| }*/

  seq_item finger_lookup(seq_item it, const K& k) const 
            { return seq_impl::finger_lookup(it,leda_cast(k)); }
  /*{\Mopl  equivalent to |IT.lookup(k)| where |IT| is the sorted sequence 
           containing |it|. \\
           \precond |S| and |IT| must have the same type }*/

  seq_item finger_locate(seq_item it, const K& k) const 
       { return seq_impl::finger_locate_succ(it,leda_cast(k)); }
  /*{\Mopl  equivalent to |IT.locate(k)| where |IT| is the sorted sequence 
           containing |it|. \\
           \precond |S| and |IT| must have the same type. }*/

  seq_item finger_locate_succ(seq_item it, const K& k) const 
       { return seq_impl::finger_locate_succ(it,leda_cast(k)); }
  /*{\Mopl  equivalent to |IT.locate_succ(k)| where |IT| is the sorted sequence 
           containing |it|. \\
           \precond |S| and |IT| must have the same type }*/


  seq_item finger_locate_pred(seq_item it, const K& k) const 
       { return seq_impl::finger_locate_pred(it,leda_cast(k)); }

  /*{\Mopl  equivalent to |IT.locate_pred(k)| where |IT| is the sorted sequence 
           containing |it|. \\
           \precond |S| and |IT| must have the same type. }*/

  seq_item min() const        { return seq_impl::min_item(); }


  seq_item min_item() const   { return seq_impl::min_item(); }
  /*{\Mop  returns the item with minimal key (|nil| if  |S|  is empty). }*/

  seq_item max() const        { return seq_impl::max_item(); }

  seq_item max_item() const   { return seq_impl::max_item(); }
  /*{\Mop  returns the item with maximal key (|nil| if  |S| is empty). }*/


  //iteration
  seq_item first_item() const 
  { return seq_impl::first_item(); }

  seq_item last_item() const 
  { return seq_impl::last_item(); }

  seq_item next_item(seq_item it) const  
  { return seq_impl::next_item(it); }

  seq_item pred_item(seq_item it) const  
  { return seq_impl::pred_item(it); }


  seq_item succ(seq_item it) const  
                                  { return seq_impl::succ(it); }
  /*{\Mop  returns the successor item of |it| in the sequence containing |it| 
          (|nil| if there is no such item). }*/

  seq_item pred(seq_item x) const  
                                   { return seq_impl::pred(x); }
  /*{\Mop  returns the predecessor item of |it| in the sequence containing |it| 
          (|nil| if there is no such item). }*/

  seq_item insert(const K& k, const I& i)
         { return seq_impl::insert(leda_cast(k),leda_cast(i)); } 
  /*{\Mop  associates information |i| with key |k|: If
          there is an item  \Litem{|k|,|j|}  in  |S| then |j| is
          replaced by |i|, else a new item \Litem{|k|,|i|} is
          added to  |S|. In both cases the item is returned. }*/


  seq_item insert_at(seq_item it, const K& k, const I& i)
  { return seq_impl::insert_at_item(it,leda_cast(k), leda_cast(i)); } 
  /*{\Mopl Like |IT.insert(k,i)| where |IT| is the sequence containing 
           item |it|.\\
          \precond |it| is an item in |IT| with \\
          |key(it)| is maximal with $|key(it)| < k$ or\\
          |key(it)| is minimal with $|key(it)| > k$ or\\
          if |key(it)=k| then |inf(it)| is replaced by $i$.
          |S| and |IT| have the same type. }*/

  seq_item insert_at(seq_item it, const K& k, const I& i, int dir)
  { return seq_impl::insert_at_item(it,leda_cast(k), leda_cast(i),dir); }
  /*{\Mopl  Like |IT.insert(k,i)| where |IT| is the sequence containing item 
     |it|. \\
     \precond |it| is an item in |IT| with\\
     |key(it)| is maximal with $|key(it)| < k$ if |dir = leda::before| or\\
     |key(it)| is minimal with $k < |key(it)|$ if |dir = leda::behind| or\\
     if |key(it)=k| then |inf(it)| is replaced by $i$.
     |S| and |IT| have the same type. }*/

  int      size()  const            { return seq_impl::size(); }
  /*{\Mop  returns the size of  |S|. }*/


  bool     empty() const           { return seq_impl::empty(); }
  /*{\Mop  returns true if  |S| is empty, false otherwise. }*/

  void     clear()                        { seq_impl::clear(); }
  /*{\Mop  makes  |S| the empty sorted sequence. }*/

  void reverse_items(seq_item a, seq_item b)  
                               { seq_impl::reverse_items(a,b); }
  /*{\Mopl  the subsequence of |IT| from |a| to |b| is reversed, where |IT| is 
           the sequence containing |a| and |b|. \\
           \precond |a| appears before 
           |b| in |IT|. }*/

  void flip_items(seq_item a, seq_item b)     
                                     { reverse_items(a,b); }
  /*{\Mopl  equivalent to |S.reverse_items(a,b)|. }*/

  /*{\Mtext \settowidth{\typewidth}{|void|}
            \addtolength{\typewidth}{\colsep}
            \setlength{\callwidth}{2cm}
            \computewidths}*/

  void del(const K& k)  { seq_impl::del(leda_cast(k)); }
  /*{\Mop  removes the item with key |k| from |S| 
          (null operation if no such item exists). }*/ 

  void del_item(seq_item it)       { seq_impl::del_item(it); }
  /*{\Mop  removes the item |it| from the sequence containing |it|. }*/

  void change_inf(seq_item it, const I& i)  
                      { seq_impl::change_inf(it,leda_cast(i)); }
  /*{\Mopl  makes |i| the information of item |it|. }*/

  void split(seq_item it,sortseq<K,I,seq_impl>& S1,
                                 sortseq<K,I,seq_impl>& S2,int dir=leda::behind)
        { seq_impl::split_at_item(it,(seq_impl&)S1,(seq_impl&)S2,dir); }
  /*{\Mopl splits |IT| at item |it|, where |IT| is the sequence containing |it|,
           into sequences |S1| and |S2| and makes |IT| empty 
           (if distinct from |S1| and |S2|). More precisely, if
            $|IT| = x_1, \ldots,x_{k-1}, |it| ,x_{k+1}, \ldots,x_n$ 
           and |dir = leda::behind| then $|S1| = x_1, \ldots,x_{k-1},|it|$ 
           and $S2 = x_{k+1}, \ldots,x_n$. If |dir = leda::before| then |S2| 
           starts with |it| after the split. }*/

  // for backward compatibility
  void split_at_item(seq_item it,sortseq<K,I,seq_impl>& S1,
                                 sortseq<K,I,seq_impl>& S2,int dir=leda::behind)
        { split(it,S1,S2,dir); }

         
  void delete_subsequence(seq_item a,seq_item b,
                                         sortseq<K,I,seq_impl>& S1)
                { seq_impl::delete_subsequence(a,b,(seq_impl&)S1); }
  /*{\Mopl  deletes the subsequence starting at |a| and ending at |b| from the
           sequence |IT| containing both and assigns the subsequence to |S1|.\\
           \precond |a| and |b| belong to the same sequence |IT|, 
           |a| is equal to or before |b| and |IT| and |S1| have the same type. }*/

  /*{\Mtext \newlength{\oldcallwidth}\setlength{\oldcallwidth}{\callwidth}
            \newlength{\oldtypewidth}\setlength{\oldtypewidth}{\typewidth}
            \settowidth{\typewidth}{|sortseq<K,I>&|}
            \addtolength{\typewidth}{\colsep} 
            \addtolength{\callwidth}{+\oldtypewidth}
            \addtolength{\callwidth}{-\typewidth}
            \computewidths}*/

  sortseq<K,I,seq_impl>& conc(sortseq<K,I,seq_impl>& S1,int dir = leda::behind)
                { seq_impl::conc((seq_impl&)S1,dir); return *this; }
  /*{\Mopl  appends |S1| at the front (|dir = leda::before|) or rear (|dir = leda::behind|)
           end of |S|, makes |S1| empty and returns |S|.\\ 
           \precond $|S.key(S.max_item())| < |S1.key(S1.min_item())|$ if |dir = leda::behind|
           and $|S1.key(S1.max_item())| < |S.key(S.min_item()|$ if |dir = leda::before|.}*/

  /*{\Mtext \setlength{\typewidth}{\oldtypewidth}
            \setlength{\callwidth}{\oldcallwidth}
            \computewidths}*/

  void merge(sortseq<K,I,seq_impl>& S1) { seq_impl::merge((seq_impl&)S1); }
  /*{\Mopl  merges the sequence |S1| into sequence |S| and makes |S1| empty.\\
           \precond all keys are distinct. }*/


  void print(ostream& out,string s,char c=' ') const
                                   { seq_impl::print(out,s,c); }
  /*{\Mopl  prints |s| and all elements of |S| separated by |c| onto stream 
           |out|. }*/

  void print(string s,char c=' ') const
                                  { seq_impl::print(cout,s,c); }
  /*{\Mopl  equivalent to |S.print(cout,s,c)|. }*/

  void check_data_structure(string s)
                          { seq_impl::check_data_structure(s); }


  bool operator==(const sortseq<K,I,seq_impl>& S1)
  { seq_item sit = min_item();  
    seq_item sit1 = S1.min_item();
    while (sit != nil && sit1 != nil)
     if (compare(key(sit),S1.key(sit)) == 0)
       { sit = succ(sit);  sit1 = S1.succ(sit1);}
     else return false;
    return (sit == nil && sit1 == nil);
  }
  /*{\Mbinop
  returns |true| if |S| agrees with |S1| componentwise and |false| otherwise
  }*/

  bool operator!=(const sortseq<K,I,seq_impl>& S1) 
                             { return (!(operator==(S1))); }
  /*{\Xop
  returns |false| if |S| agrees with |S1| componentwise and |true| otherwise
  }*/



  static sortseq<K,I,seq_impl>* my_sortseq(seq_item it) 
  { return (sortseq<K,I,seq_impl>*)(seq_impl::my_collection(it)->owner); }
  /*{\Mstatic  returns a pointer to the |sortseq| containing |it|. \\
               \precond The type of the |sortseq| containing |it| must be 
                        |sortseq<K,I>|.}*/




  virtual ~sortseq()  
  { seq_impl::clear(); 
    if (value_buf) delete value_buf;
   }

  typedef sortseq<K,I,seq_impl> this_type;


#if defined(LEDA_STL_ITERATORS)

typedef stl_access_pair<K,I> pair_type;

pair_type& stl_item_access(seq_item it)  const
{ if (value_buf) delete value_buf;
  value_buf = new pair_type(key(it),operator[](it)); 
  return *value_buf;
}

DEFINE_STL_ITERATORS(this_type,seq_item,pair_type)

#endif



};
//------------------------------------------------------------------------------
// Iteration Macros
//------------------------------------------------------------------------------


/*{\Mtext \headerline{Iteration}

{\bf forall\_items}($it,S$)       
$\{$ ``the items of $S$ are successively assigned to $it$'' $\}$

{\bf forall\_rev\_items}($it,S$)       
$\{$ ``the items of $S$ are successively assigned to $it$ in reverse order'' $\}$

{\bf forall}($i,S$)       
$\{$ ``the informations of all items of $S$ are successively assigned to $i$'' $\}$ 

{\bf forall\_defined}($k,S$)       
$\{$ ``the keys of all items of $S$ are successively assigned to $k$'' $\}$ 
}*/


/*{\Mimplementation
Sorted sequences are implemented by skiplists \cite{Pugh}. 
Let $n$ denote the current size of the sequence. Operations |insert|, |locate|, |lookup| and |del| 
take time $O(\log n)$, operations |succ|, |pred|, |max|, |min_item|, |key|, |inf|, 
|insert_at| and |del_item| take time $O(1)$. |clear| takes 
time $O(n)$ and |reverse_items| $O(l)$, where $l$ is the length of the 
reversed subsequence. |Finger_lookup(x)| and |finger_locate(x)|  take time 
$O(\log min (d, n - d))$ if |x| is the $d$-th item in |S|.
|Finger_lookup_from_front(x)| and |finger_locate_from_front(x)|  take time 
$O(\log d)$ if |x| is the $d$-th item in |S|.
|Finger_lookup_from_rear(x)| and |finger_locate_from_rear(x)|  take time 
$O(\log d)$ if |x| is the $n - d$-th item in |S|.
 |Finger_lookup(it,x)|
and |finger_locate(it,x)| take time $O(\log min (d, n - d))$ where $d$ is the
number of items between |it| and the item containing |x|. Note that
|min(d,n - d)| is the smaller of the distances from |it| to |x| if
sequences are viewed as circularly closed. |Split|, 
|delete_subsequence| and |conc| take time $O(\log min (n_1,n_2))$ 
where $n_1$ and $n_2$ are the
sizes of the results of |split| and |delete_subsequence| and the
arguments of |conc| respectively. |Merge| takes time 
$O(\log ((n_1 + n_2)/n_1))$ where $n_1$ and $n_2$ are 
the sizes of the two arguments. The
space requirement of sorted sequences is linear in the length of the
sequence (about $25.5n$ Bytes for a sequence of size $n$ plus the space for the
keys and the informations.).
}*/

/*{\Mexample
We use a sorted sequence to list all elements in a sequence of 
strings lying
lexicographically between two given search strings.

\begin{verbatim}

#include <LEDA/core/sortseq.h>
#include <iostream>

using leda::sortseq;
using leda::string;
using leda::seq_item;
using std::cin;
using std::cout;


int main()
{
   sortseq<string, int> S;
   string s1, s2;

   cout << "Input a sequence of strings terminated by 'STOP'\n";
   while (cin >> s1 && s1 != "STOP")
     S.insert(s1, 0);

   while(true) {
     cout << "\n\nInput a pair of strings:\n";
     cin >> s1 >> s2;
     cout << "All strings s with " << s1 <<" <= s <= " << s2 << ":";
     if(s2 < s1) continue;
     seq_item last  = S.locate_pred(s2);
     seq_item first = S.locate(s1);
     if ( !first || !last || first == S.succ(last) ) continue;
     seq_item it = first;
     while(true) {
       cout << "\n" << S.key(it);
       if(it == last) break;
       it = S.succ(it);
     }
   }
}

\end{verbatim}

\noindent
Further examples can be found in section Sorted Sequences of
\cite{LEDAbook}.

}*/

#if LEDA_ROOT_INCL_ID == 600043
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
