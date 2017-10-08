/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_dictionary.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_d3_dictionary_H
#define LEDA_d3_dictionary_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600065
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/range_tree.h>

LEDA_BEGIN_NAMESPACE

typedef rt_item dic3_item;



template<class type1, class type2, class type3, class itype>
class d3_dictionary : public range_tree {


  // redefine the virtual functions of class range_tree

  void rt_clear_key(GenPtr*& x) const { 
    LEDA_CLEAR(type1,x[0]); 
    LEDA_CLEAR(type2,x[1]); 
    LEDA_CLEAR(type3,x[2]); 
  }
  
  void rt_copy_key(GenPtr*& x) const { 
    LEDA_COPY(type1,x[0]); 
    LEDA_COPY(type2,x[1]); 
    LEDA_COPY(type3,x[2]); 
  }

/*
  void rt_print_key(int d,GenPtr*& x) const {
    switch(d) {
      case 0: LEDA_PRINT(type1,x[0],cout);
      	      break;
      case 1: LEDA_PRINT(type2,x[1],cout);
      	      break;
      case 2: LEDA_PRINT(type3,x[2],cout);
      	      break;
    }
  }
*/

  void rt_clear_inf(GenPtr& x) const { LEDA_CLEAR(itype,x);}
  void rt_copy_inf(GenPtr& x)  const { LEDA_COPY(itype,x); }

  int rt_cmp(int d, GenPtr* p, GenPtr* q) const { 
    switch(d) {
      case 0: return LEDA_COMPARE(type1,p[0],q[0]);
      case 1: return LEDA_COMPARE(type2,p[1],q[1]);
      case 2: return LEDA_COMPARE(type3,p[2],q[2]);
    }
  }

  range_tree* new_range_tree(int d, int level) { 
    return new d3_dictionary<type1,type2,type3,itype>(level); 
  }

  public:
  
    d3_dictionary( int level=0 ) : range_tree(3,level) {}
    ~d3_dictionary() { clear(); }
  
const itype& inf(dic3_item x)    { return LEDA_CONST_ACCESS(itype,x->inf()); }
const type1& key1(dic3_item x)   { return LEDA_CONST_ACCESS(type1,x->key(0)); }
const type2& key2(dic3_item x)   { return LEDA_CONST_ACCESS(type2,x->key(1)); }
const type3& key3(dic3_item x)   { return LEDA_CONST_ACCESS(type3,x->key(2)); }
  
  
    void  change_inf(dic3_item x, itype i) { 
      leda_clear(LEDA_ACCESS(itype,x->inf())); 
      x->inf() = leda_copy(i); 
    }
  
    dic3_item min_key1() { return range_tree::rt_min(0); }
    dic3_item max_key1() { return range_tree::rt_max(0); }
    dic3_item min_key2() { return range_tree::rt_min(1); }
    dic3_item max_key2() { return range_tree::rt_max(1); }
    dic3_item min_key3() { return range_tree::rt_min(2); }
    dic3_item max_key3() { return range_tree::rt_max(2); }
  
    dic3_item insert(type1 x,type2 y,type3 z,itype i)
    { 
      rt_item p = new rt_elem(leda_copy(x),leda_copy(y),leda_copy(z),leda_copy(i));
      return range_tree::insert(p);
     }
    
    list<rt_item> range_search( type1 x0, type1 x1,
  			        type2 y0, type2 y1,
  			        type3 z0, type3 z1 )
    { 
      rt_elem p(leda_cast(x0),leda_cast(y0),leda_cast(z0),0);
      rt_elem q(leda_cast(x1),leda_cast(y1),leda_cast(z1),0);
      return range_tree::query(&p,&q);
    }
  
    dic3_item lookup( type1 x, type2 y, type3 z ) { 
      rt_elem p(leda_cast(x),leda_cast(y),leda_cast(z),0);
      return range_tree::lookup(&p);
    }
  
    void del(type1 x,type2 y, type3 z) { 
      rt_elem p(leda_cast(x),leda_cast(y),leda_cast(z),0);
      range_tree::del(&p);
     }
  
    void del_item(dic3_item it) { range_tree::del(it); }
    list<dic3_item> all_items() { return range_tree::all_items(); }
};

// iteration macro
//
#define forall_dic3_items(x,T)  (T).init_iteration(); forall(x,(T).L )


#if LEDA_ROOT_INCL_ID == 600065
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
