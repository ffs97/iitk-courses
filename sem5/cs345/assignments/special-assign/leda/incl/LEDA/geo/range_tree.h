/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  range_tree.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#ifndef LEDA_RANGE_TREE_H
#define LEDA_RANGE_TREE_H

#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>

LEDA_BEGIN_NAMESPACE



// the elements (an information together with its associated array 
// of keys) are stored in the class rt_elem
//
class __exportC rt_elem {

  GenPtr  rt_inf;			// the information
  GenPtr* rt_keys;			// an array of keys

 public:

  LEDA_MEMORY(rt_elem);

  // some constructors, destructor
  //
  rt_elem() { rt_keys=0; rt_inf=0; }
  rt_elem(GenPtr k0, GenPtr k1, GenPtr i);
  rt_elem(GenPtr k0, GenPtr k1, GenPtr k2, GenPtr i);
  rt_elem(int dim, GenPtr* k, GenPtr i);

  virtual ~rt_elem() { if( rt_keys ) delete rt_keys; }

  // accessing information and keys
  //
  const GenPtr& key(int d) { return rt_keys[d]; }
  GenPtr& inf()            { return rt_inf; }

  friend class __exportC range_tree;
};



// a pointer to a rt_elem is called rt_item
//
typedef rt_elem* rt_item;



// the range tree is derived from a binary tree of type base_tree
//

LEDA_END_NAMESPACE

#include <LEDA/core/impl/bb_tree.h>

LEDA_BEGIN_NAMESPACE

typedef bb_tree base_tree;
typedef bb_tree_item base_tree_item;



// Here comes the definition ...
//
class __exportC range_tree : public base_tree
{
  protected:

    list<base_tree_item> aux;	// auxiliary list of base_tree_items
    int dim;			// the dimension of the structure 
    int lev;			// the level of this tree 

    // some internal functions
    
    void rt_insert( rt_item rt_key );
    void rt_del( rt_item rt_key );
    void rt_query( rt_item&, rt_item&, list<rt_item>& ) const;
    void build_tree( rt_item* elem_array, int l, int r, base_tree_item p=0 ) ;
    int elements_in_subtree( rt_item* elem_array, base_tree_item subroot );
  
    // to compare two elements in the range tree, we first compare their
    // keys on the appropriate level.
    //
    int cmp( GenPtr x, GenPtr y ) const {
      register int c, d=lev;
      do {
        c=rt_cmp(d,rt_item(x)->rt_keys,rt_item(y)->rt_keys);
      } while( !c && (d=(d+1)%dim)!=lev );
      return c;
    }

    void clear() { base_tree::clear(); aux.clear(); }
  
    // this function is called for every structural change in the base_tree;
    // the first argument specifies the operation which forced this change.
    // (cf. the class bin_tree)
    //
    // if we're not on the highest level and a child of a node changes, 
    // we have to recompute its secondary structure. Hence we append the
    // parent node to an auxiliary list (which will be processed in "insert").
    // to avoid duplicate entries in the list, we clear the secondary structure
    // of an node as soon as it is appended. Then a node is in the list iff
    // its secondary structure is empty.
    //
    void propagate_modification( int where, GenPtr parent, GenPtr /* child */) 
    { 
      range_tree* sec = (range_tree*) inf((base_tree_item) parent);
  
      // sec==0 iff we are on the highest level (lev==dim-1)
      //
      if ( sec ) // select the "interesting" cases
       switch( where )
       { case 1: // insert
          aux.append((base_tree_item)parent); 
	  break;
  
         case 4:// rotation
         case 5:// rotation
         case 7:// double rotation 
         case 8:// double rotation 
         case 9:// double rotation
	  if ( sec->size() ) 
	  { // if parent is already in the list, don't append it again
	    sec->clear();
            aux.append((base_tree_item)parent); 
	  }
        }
     }

    // let's redefine the virtual functins of the base tree as we need them
    //

    // because both informations and keys in the base tree are pointers
    // we never need to copy them
    //
    void copy_key( GenPtr& ) const {}
    void copy_inf( GenPtr& ) const {}
  
    // keys are only cleared on the lowest level
    //
    void clear_key( GenPtr& x ) const {
      if( lev==0 ) {
        rt_clear_key( rt_item(x)->rt_keys );
        rt_clear_inf( rt_item(x)->rt_inf );
        delete rt_item(x);
      }
    }
  
    void print_key( GenPtr x ) const { rt_print_key(lev,rt_item(x)->rt_keys); }
  
    // informations are always a pointer to a range tree
    //
    void clear_inf( GenPtr& x ) const { if( x ) delete ((range_tree*) x); }
    void clear_iinf( GenPtr& x ) const { if( x ) delete ((range_tree*) x); }
    void print_inf( GenPtr x ) const { if( x ) ((range_tree*) x)->print(); }
  
    // here are the virtual functions which have to be changed by a derived
    // class of class range_tree
    //
  
    virtual int rt_cmp( int, GenPtr*, GenPtr* ) const { return 0; }
  
    virtual void rt_copy_key( GenPtr*& ) const {}
    virtual void rt_clear_key( GenPtr*& ) const {}
  
    virtual void rt_print_key( int, GenPtr*& ) const {}
  
    virtual void rt_copy_inf( GenPtr& ) const {}
    virtual void rt_clear_inf( GenPtr& ) const {}
  
  public:

/*
    LEDA_MEMORY( range_tree );
*/
  
    // constructor and virtual destructor
    //
    range_tree( int d, int l=0 ) { dim=d; lev=l; }
    virtual ~range_tree() { clear(); }

    virtual range_tree* new_range_tree( int dimension, int level=0 ) { 
      return new range_tree(dimension,level); 
    }

  
    rt_item rt_min( int d ) const;
    rt_item rt_max( int d ) const;
  
  
    rt_item lookup( rt_item elem ) const {
      base_tree_item p = base_tree::lookup(elem);
      return( p ? (rt_item) key(p) : 0 );
    }

    void del( rt_item elem ) {
      if( lookup(elem) )
        rt_del(elem);
    }
  
  
    rt_item insert( rt_item elem ) {
      rt_item old = lookup(elem);
      if( !old ) {
        rt_insert(elem);
        return elem;
      }
      else {
        rt_clear_key( elem->rt_keys );
        rt_clear_inf( old->rt_inf );	
        old->rt_inf = elem->rt_inf;
        delete elem;
        return old;
      }
    }
  
    list<rt_item> query( rt_item l, rt_item r ) const {
      list<rt_item> res;
      rt_query( l, r, res );
      return res;
    }


    list <rt_item> L;		// auxiliary list for iterations

    list<rt_item> all_items() const;
public:
    void init_iteration() const { 
      range_tree* T = (range_tree*)this;
      T->L = T->all_items(); 
    }
};

LEDA_END_NAMESPACE

#endif
