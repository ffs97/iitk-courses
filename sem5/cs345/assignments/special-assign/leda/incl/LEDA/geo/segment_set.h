/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  segment_set.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SEGMENT_SET_H
#define LEDA_SEGMENT_SET_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600055
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/seg_tree.h>
#include <LEDA/geo/line.h>

LEDA_BEGIN_NAMESPACE

typedef seg_tree_item seg_item;


//------------------------------------------------------------------------------
// SegmentSet: a dictionary for line segments  with a fixed orientation
//------------------------------------------------------------------------------

class __exportC SegmentSet : public segment_tree<double,double,GenPtr> {

int    r90;   // for angles that are a multiple of pi / 2, i.e. 90 degrees         
double alpha; // orientation given by an angle

segment rotate_about_origin_by_alpha(const segment& s, bool pos_dir) const;

public:
  typedef double  coord_type;
  typedef segment segment_type;
  typedef line    line_type;
 
public:
 
segment  key(seg_item) const;

seg_item insert(const segment&, GenPtr);
seg_item lookup(const segment&) const;
void     del(const segment&);

list<seg_item>  intersection(const segment&) const;
list<seg_item>  intersection(const line&) const;
 
list<seg_item>  intersection_sorted(const segment&) const;
list<seg_item>  intersection_sorted(const line&) const;
 
 SegmentSet(int rot90 = 0);
 SegmentSet(double angle);
~SegmentSet() {} // Note: derived classes must call clear in their destructor!
};

#define forall_seg_items(i,S) forall_seg_tree_items(i,S)


//------------------------------------------------------------------------------
// class segment_set: derived from generic SegmentSet
//------------------------------------------------------------------------------

 
/*{\Manpage {segment_set} {I} {Sets of Parallel Segments}}*/

template<class I>

class segment_set : public SegmentSet {

/*{\Mdefinition
    An instance $S$ of the parameterized data type |\Mname| is a 
    collection of items ($seg\_item$). Every item in $S$ contains as key a
    line segment with a fixed direction $\alpha$ (see data type segment) and
    an information from data type $I$, called the information type of $S$.
    $\alpha$ is called the orientation of $S$. We use $\<s,i\>$ to denote the
    item with segment $s$ and information $i$. For each segment $s$ there is
    at most one item $\<s,i\> \in S$.}*/

void clear_info(GenPtr& x)    { LEDA_CLEAR(I,x); }
void copy_info(GenPtr& x)     { LEDA_COPY(I,x); }

public:
  typedef segment segment_type;

public:

/*{\Mcreation S }*/

segment_set() : SegmentSet(0) {}
/*{\Mcreate creates an empty instance |\Mvar| of type |\Mname| with orientation 
            zero, i.e., horizontal segments.}*/

explicit

segment_set(int rot) : SegmentSet(rot) {}
/*{\Mcreate creates an empty instance |\Mvar| of type |\Mname| with orientation 
            $|rot| \cdot \pi / 2$. }*/

explicit

segment_set(double a) : SegmentSet(a) {}
/*{\Mcreate creates an empty instance |\Mvar| of type |\Mname| with orientation 
            $a$. 
			(Note that there may be incorrect results due to rounding errors.)}*/

~segment_set() { clear(); }


/*{\Moperations 2.7 4.7}*/

segment key(seg_item it) const { return SegmentSet::key(it); }
/*{\Mop   returns the segment of item $it$.\\
	  \precond $it$ is an item in |\Mvar|.}*/

const I& inf(seg_item it) const { return LEDA_CONST_ACCESS(I,SegmentSet::inf(it)); }
/*{\Mop   returns the information of item $it$.\\
	  \precond $it$ is an item in |\Mvar|.}*/

seg_item insert(const segment& s, const I& i) { return SegmentSet::insert(s,leda_cast(i));}
/*{\Mop   associates the information $i$ with segment 
	  $s$. If there is an item $\<s,j\>$ in |\Mvar|  
	  then $j$ is replaced by $i$, else a new item
	  $\<s,i\>$ is added to $S$. In both cases the
	  item is returned.}*/	

seg_item lookup(const segment& s) const {return SegmentSet::lookup(s);}
/*{\Mop   returns the item with segment $s$ (nil if no 
	  such item exists in \Mvar).}*/

list<seg_item> intersection(const segment& q) const {return SegmentSet::intersection(q);}
/*{\Mop   returns all items $\<s,i\>\ \in\ S$ with 
	  $s \cap q \ne \emptyset$.\\
	  \precond $q$ is orthogonal to the segments in |\Mvar|.}*/

list<seg_item> intersection_sorted(const segment& q) const {return SegmentSet::intersection_sorted(q);}
/*{\Mop   as above, but the returned segments are ordered as they are intersected 
          by $q$ if one travels from |q.source()| to |q.target()|.}*/

list<seg_item> intersection(const line& l) const {return SegmentSet::intersection(l);}
/*{\Mop   returns all items $\<s,i\>\ \in\ S$ with 
	  $s \cap l \ne \emptyset$.\\
          \precond $l$ is orthogonal to the segments in |\Mvar|.}*/

list<seg_item> intersection_sorted(const line& l) const {return SegmentSet::intersection_sorted(l);}
/*{\Mop   as above, but the returned segments are ordered as they are intersected 
          by $l$ if one travels along $l$ in direction |l.direction()|.}*/

void del(const segment& s) {SegmentSet::del(s);}
/*{\Mop   deletes the item with segment $s$ from |\Mvar|.}*/

void del_item(seg_item it) {SegmentSet::del_item(it);}
/*{\Mop   removes item $it$ from |\Mvar|.\\
	  \precond $it$ is an item in |\Mvar|.}*/

void  change_inf(seg_item it, const I& i) 
{ SegmentSet::change_inf(it,leda_cast(i)); }
/*{\Mopl  makes $i$ the information of item $it$.\\
	  \precond $it$ is an item in |\Mvar|.}*/

void clear() { SegmentSet::clear(); }
/*{\Mop   makes |\Mvar| the empty |\Mtype|.}*/

bool empty() const {return SegmentSet::empty();}
/*{\Mop   returns true iff |\Mvar| is empty.}*/

int size() const {return SegmentSet::size();}
/*{\Mop   returns the size of |\Mvar|.}*/

};
 
/*{\Mimplementation
Segment sets are implemented by dynamic segment trees based on BB[$\alpha$]
trees (\cite{Wi85,Lu78}) trees. Operations key, inf, change\_inf, empty, and 
size take time $O(1)$, insert, lookup, del, and del\_item take time 
$O(\log^2 n)$ and an intersection operation takes time $O(k + \log^2 n)$, 
where $k$ is the size of the returned list. Here $n$ is the current size of 
the set. The space requirement is $O(n\log n)$.}*/

 

#if LEDA_ROOT_INCL_ID == 600055
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
