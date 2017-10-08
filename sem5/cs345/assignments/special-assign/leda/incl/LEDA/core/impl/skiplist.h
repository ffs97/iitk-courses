/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  skiplist.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SKIPLIST_H
#define LEDA_SKIPLIST_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 350981
#include <LEDA/internal/PREAMBLE.h>
#endif



#include <LEDA/system/basic.h>

LEDA_BEGIN_NAMESPACE

class skip_list_bs_iterator;
class skip_list_iterator;

class __exportC skiplist;
class __exportC header_node;
class __exportC skiplist_node;

typedef skiplist_node* sl_item;
typedef header_node*     large_item;

const int MaxHeight = 32;

class __exportC skiplist_node
{ 
  friend class __exportC skiplist;
  friend class skip_list_bs_iterator;
  friend class skip_list_iterator;

  static atomic_counter id_count;

  GenPtr key;
  GenPtr inf;
  int    height;
  unsigned long id;  // id number
  sl_item pred;  
  sl_item backward; 
#ifdef SMEM 
  sl_item* forward;  // array of forward pointers 
#else
  sl_item forward[1];
#endif

 friend unsigned long ID_Number(skiplist_node* p) { return p->id; } 
};

class __exportC header_node : public skiplist_node
{ friend class skiplist;
  friend class skip_list_bs_iterator;
  friend class skip_list_iterator;
#ifndef SMEM
  sl_item more_forward_pointers[MaxHeight];
#endif
  int true_height;
  skiplist* myseq;
  
};


class __exportC skiplist
{ 
  friend class skip_list_bs_iterator;
  friend class skip_list_iterator;

protected:

  large_item  header;         
  sl_item STOP;  

  random_source ran;
       
  float prob; 
  int randomBits;           
  int randomsLeft; 

  
  virtual int cmp(GenPtr, GenPtr) const 
  { LEDA_EXCEPTION(1,"cmp should never be called"); return 0; }

  virtual void copy_key(GenPtr&)  const  {  }
  virtual void copy_inf(GenPtr&)  const  {  }

  virtual void clear_key(GenPtr&) const 
  { LEDA_EXCEPTION(1,"clear_key should never be called"); }

  virtual void clear_inf(GenPtr&) const
  { LEDA_EXCEPTION(1,"clear_inf should never be called"); }

  virtual void print_key(ostream&, GenPtr)  const 
  { LEDA_EXCEPTION(1,"print_key should never be called"); }

  virtual void print_inf(ostream&, GenPtr)  const 
  { LEDA_EXCEPTION(1,"print_inf should never be called"); }

  virtual int key_type_id() const 
  { LEDA_EXCEPTION(1,"key_type_id should never be called"); return 0; }

  
  void fill_random_source()
  { //randomBits = rand_int(0,std::numeric_limits<int>::max()-1);
    randomBits = ran(0,MAXINT-1);
    randomsLeft = 31;
  }



  int  randomLevel();

  sl_item search(sl_item,int,GenPtr,int&) const;
  sl_item gen_search(sl_item,int,GenPtr,int&) const;
  sl_item int_search(sl_item,int,GenPtr,int&) const;
  sl_item double_search(sl_item,int,GenPtr,int&) const;

  sl_item finger_search(sl_item,GenPtr,int&) const;
  sl_item gen_finger_search(sl_item,GenPtr,int&) const;
  sl_item int_finger_search(sl_item,GenPtr,int&) const;
  sl_item double_finger_search(sl_item,GenPtr,int&) const;

  sl_item finger_search(GenPtr,int&) const;
  sl_item gen_finger_search(GenPtr,int&) const;
  sl_item int_finger_search(GenPtr,int&) const;
  sl_item double_finger_search(GenPtr,int&) const;

  sl_item finger_search_from_front(GenPtr,int&) const;
  sl_item gen_finger_search_from_front(GenPtr,int&) const;
  sl_item int_finger_search_from_front(GenPtr,int&) const;
  sl_item double_finger_search_from_front(GenPtr,int&) const;

  sl_item finger_search_from_rear(GenPtr,int&) const;
  sl_item gen_finger_search_from_rear(GenPtr,int&) const;
  sl_item int_finger_search_from_rear(GenPtr,int&) const;
  sl_item double_finger_search_from_rear(GenPtr,int&) const;

  void remove_item(sl_item);
  void insert_item_at_item(sl_item,sl_item,int);
  void print(const skiplist &, ostream&, string s, char space) const;
  void check_data_structure(const skiplist&, string s);


public:

  void* owner; // pointer to the data type object (e.g. sortseq)
  
  typedef sl_item item;
  skiplist(float prob = 0.25);
  skiplist(const skiplist&);
  skiplist& operator=(const skiplist&);
  virtual ~skiplist();

  const GenPtr&  key(sl_item p) const;
  GenPtr&  inf(sl_item p) const;
  GenPtr&  info(sl_item p) const;
  int      get_height(sl_item p) const;

  sl_item insert(GenPtr key, GenPtr inf);

  sl_item locate(GenPtr key) const;
  sl_item locate(GenPtr key, bool& equal) const;
  sl_item locate_succ(GenPtr key) const;
  sl_item locate_pred(GenPtr key) const;
  sl_item lookup(GenPtr key) const;

  sl_item finger_locate(sl_item, GenPtr key) const;
  sl_item finger_locate_succ(sl_item,GenPtr key) const;
  sl_item finger_locate_pred(sl_item,GenPtr key) const;
  sl_item finger_lookup(sl_item,GenPtr key) const;
   
  sl_item finger_locate(GenPtr key) const;
  sl_item finger_locate_succ(GenPtr key) const;
  sl_item finger_locate_pred(GenPtr key) const;
  sl_item finger_lookup(GenPtr key) const;

  sl_item finger_locate_from_front(GenPtr key) const;
  sl_item finger_locate_succ_from_front(GenPtr key) const;
  sl_item finger_locate_pred_from_front(GenPtr key) const;
  sl_item finger_lookup_from_front(GenPtr key) const;

  sl_item finger_locate_from_rear(GenPtr key) const;
  sl_item finger_locate_succ_from_rear(GenPtr key) const;
  sl_item finger_locate_pred_from_rear(GenPtr key) const;
  sl_item finger_lookup_from_rear(GenPtr key) const;

  sl_item min_item() const;
  sl_item max_item() const;

  // for compatibility with skiplists 
  sl_item min() const;
  sl_item max() const;

  void reverse_items(sl_item,sl_item);
  void del(GenPtr key);
  void del1(GenPtr key);

  void conc(skiplist&,int = leda::behind);
  void split_at_item(sl_item,skiplist&,skiplist&,int = leda::behind);

  void merge(skiplist&);

  void delete_subsequence(sl_item,sl_item, skiplist&);


  void print(ostream& out, string s, char space) const
                  { print(*this,out,s,space); }

  void check_data_structure(string s)
     { check_data_structure(*this,s); }

  void validate_data_structure();

  sl_item insert_at_item(sl_item p, GenPtr key, GenPtr inf);
  sl_item insert_at_item(sl_item p, GenPtr key, GenPtr inf, int dir);



  void change_inf(sl_item p, GenPtr inf);
  void del_item(sl_item p);
  void clear();
  int  size() const;
  bool empty() const;

  sl_item first_item() const;
  sl_item last_item() const;
  sl_item next_item(sl_item p) const;
  sl_item pred_item(sl_item p) const;

  sl_item succ(sl_item p) const;
  sl_item pred(sl_item p) const;

  sl_item stl_next_item(sl_item p) const;
  sl_item stl_pred_item(sl_item p) const;


  static skiplist* my_collection(sl_item p);
  // returns a pointer to the skiplist containing p


  /* priority queue operations */

  sl_item find_min() const;
  void del_min();
  void decrease_key(sl_item p, GenPtr k);


};

inline const GenPtr&  skiplist::key(sl_item p) const  
{ return p->key; }

inline GenPtr&  skiplist::inf(sl_item p) const  
{ return p->inf; }

inline GenPtr& skiplist::info(sl_item p) const 
{ return p->inf; }

inline int     skiplist::get_height(sl_item p) const  
{ return p->height; }

inline sl_item skiplist::first_item() const  
{ sl_item q = header->forward[0];
  return (q == STOP) ? 0 : q;
}

inline sl_item skiplist::last_item() const  
{ sl_item q = STOP->pred;
  return (q == header) ? 0 : q;
}

inline sl_item skiplist::min_item() const  
{ sl_item q = header->forward[0];
  return (q == STOP) ? 0 : q;
}

inline sl_item skiplist::max_item() const  
{ sl_item q = STOP->pred;
  return (q == header) ? 0 : q;
}

inline sl_item skiplist::min() const
{ return min_item(); }

inline sl_item skiplist::max() const
{ return max_item(); }




inline sl_item skiplist::succ(sl_item p) const 
{ sl_item q =  p->forward[0]; 
  return (q->height < 0) ? 0 : q;
}

inline sl_item skiplist::pred(sl_item p) const 
{ sl_item q = p->pred; 
  return (q->height == MaxHeight) ? 0 : q;
}


inline sl_item skiplist::next_item(sl_item p) const 
{ return p ? succ(p) : 0; } 

inline sl_item skiplist::pred_item(sl_item p) const
{ return p ? pred(p) : 0; } 

inline sl_item skiplist::stl_next_item(sl_item p) const
{ return p ? succ(p) : first_item(); } 

inline sl_item skiplist::stl_pred_item(sl_item p) const
{ return p ? pred(p) : last_item(); } 


inline void skiplist::change_inf(sl_item p, GenPtr inf) 
{ clear_inf(p->inf); 
  copy_inf(inf);  
  p->inf = inf; 
 }

inline bool skiplist::empty() const  
{ return (header->forward[0] == STOP); }

inline sl_item skiplist::find_min() const  
{ return min_item(); }

inline void skiplist::del_min() 
{ sl_item p = min_item(); if (p) del_item(p); }

inline void skiplist::decrease_key(sl_item p, GenPtr k) 
{insert(k,p->inf); del_item(p);}


/* dummy I/O and cmp functions */

inline void Print(const skiplist&,ostream&) {  }
inline void Read(skiplist&, istream&) {  }
inline int  compare(const skiplist&,const skiplist&)  
{ return 0; }


LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 350981
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif


