/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  rat_segment_set.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_RAT_SEGMENT_SET_H
#define LEDA_RAT_SEGMENT_SET_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600129
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/numbers/rational.h>
#include <LEDA/geo/seg_tree.h>
#include <LEDA/geo/rat_line.h>

LEDA_BEGIN_NAMESPACE

typedef seg_tree_item seg_item;


//------------------------------------------------------------------------------
// RatSegmentSet: a dictionary for line segments  with a fixed orientation
//------------------------------------------------------------------------------

class __exportC RatSegmentSet : public segment_tree<rational,rational,GenPtr> {

int r90; // angle = r90 * pi/2, i.e. angles are a multiple 90 degrees         

rat_segment rotate_about_origin_by_alpha(const rat_segment& s, bool pos_dir) const
{ return s.rotate90(rat_point(), pos_dir ? r90 : -r90); }

public:
  typedef rational    coord_type;
  typedef rat_segment segment_type;
  typedef rat_line    line_type;
 
public:
 
rat_segment  key(seg_item) const;

seg_item insert(const rat_segment&, GenPtr);
seg_item lookup(const rat_segment&) const;
void     del(const rat_segment&);

list<seg_item>  intersection(const rat_segment&) const;
list<seg_item>  intersection(const rat_line&) const;
 
list<seg_item>  intersection_sorted(const rat_segment&) const;
list<seg_item>  intersection_sorted(const rat_line&) const;
 
 RatSegmentSet(int rot90 = 0);
~RatSegmentSet() {} // Note: derived classes must call clear in their destructor!
};

#define forall_seg_items(i,S) forall_seg_tree_items(i,S)


//------------------------------------------------------------------------------
// class rat_segment_set: derived from generic RatSegmentSet
//------------------------------------------------------------------------------

 
/*{\Manpage {rat_segment_set} {I} {Sets of Parallel Rational Segments}}*/

template<class I>

class rat_segment_set : public RatSegmentSet {

/*{\Mdefinition
    An instance $S$ of the parameterized data type |\Mname| is a 
    collection of items ($seg\_item$). Every item in $S$ contains as key a
    rational line segment with a fixed direction $\alpha$ (see data type 
	|rat_segment|) and an information from data type $I$, called the information 
	type of $S$.
    $\alpha$ is called the orientation of $S$, it must be a multiple of $\pi/2$. 
	We use $\<s,i\>$ to denote the item with segment $s$ and information $i$. 
	For each segment $s$ there is at most one item $\<s,i\> \in S$.}*/

void clear_info(GenPtr& x)    { LEDA_CLEAR(I,x); }
void copy_info(GenPtr& x)     { LEDA_COPY(I,x); }

public:
  typedef rat_segment segment_type;

public:

/*{\Mcreation S }*/

rat_segment_set() : RatSegmentSet(0) {}
/*{\Mcreate creates an empty instance |\Mvar| of type |\Mname| with orientation 
            zero, i.e., horizontal segments.}*/

explicit

rat_segment_set(int rot) : RatSegmentSet(rot) {}
/*{\Mcreate creates an empty instance |\Mvar| of type |\Mname| with orientation 
            $|rot| \cdot \pi / 2$. }*/

~rat_segment_set() { clear(); }


/*{\Moperations 2.7 4.7}*/

rat_segment key(seg_item it) const { return RatSegmentSet::key(it); }
/*{\Mop   returns the segment of item $it$.\\
	  \precond $it$ is an item in |\Mvar|.}*/

const I& inf(seg_item it) const { return LEDA_CONST_ACCESS(I,RatSegmentSet::inf(it)); }
/*{\Mop   returns the information of item $it$.\\
	  \precond $it$ is an item in |\Mvar|.}*/

seg_item insert(const rat_segment& s, const I& i) { return RatSegmentSet::insert(s,leda_cast(i));}
/*{\Mop   associates the information $i$ with segment 
	  $s$. If there is an item $\<s,j\>$ in |\Mvar|  
	  then $j$ is replaced by $i$, else a new item
	  $\<s,i\>$ is added to $S$. In both cases the
	  item is returned.}*/	

seg_item lookup(const rat_segment& s) const {return RatSegmentSet::lookup(s);}
/*{\Mop   returns the item with segment $s$ (nil if no 
	  such item exists in \Mvar).}*/

list<seg_item> intersection(const rat_segment& q) const {return RatSegmentSet::intersection(q);}
/*{\Mop   returns all items $\<s,i\>\ \in\ S$ with 
	  $s \cap q \ne \emptyset$.\\
	  \precond $q$ is orthogonal to the segments in |\Mvar|.}*/

list<seg_item> intersection_sorted(const rat_segment& q) const {return RatSegmentSet::intersection_sorted(q);}
/*{\Mop   as above, but the returned segments are ordered as they are intersected 
          by $q$ if one travels from |q.source()| to |q.target()|.}*/

list<seg_item> intersection(const rat_line& l) const {return RatSegmentSet::intersection(l);}
/*{\Mop   returns all items $\<s,i\>\ \in\ S$ with 
	  $s \cap l \ne \emptyset$.\\
          \precond $l$ is orthogonal to the segments in |\Mvar|.}*/

list<seg_item> intersection_sorted(const rat_line& l) const {return RatSegmentSet::intersection_sorted(l);}
/*{\Mop   as above, but the returned segments are ordered as they are intersected 
          by $l$ if one travels along $l$ in direction |l.direction()|.}*/

void del(const rat_segment& s) {RatSegmentSet::del(s);}
/*{\Mop   deletes the item with segment $s$ from |\Mvar|.}*/

void del_item(seg_item it) {RatSegmentSet::del_item(it);}
/*{\Mop   removes item $it$ from |\Mvar|.\\
	  \precond $it$ is an item in |\Mvar|.}*/

void  change_inf(seg_item it, const I& i) 
{ RatSegmentSet::change_inf(it,leda_cast(i)); }
/*{\Mopl  makes $i$ the information of item $it$.\\
	  \precond $it$ is an item in |\Mvar|.}*/

void clear() { RatSegmentSet::clear(); }
/*{\Mop   makes |\Mvar| the empty |\Mtype|.}*/

bool empty() const {return RatSegmentSet::empty();}
/*{\Mop   returns true iff |\Mvar| is empty.}*/

int size() const {return RatSegmentSet::size();}
/*{\Mop   returns the size of |\Mvar|.}*/

};
 
/*{\Mimplementation
Segment sets are implemented by dynamic segment trees based on BB[$\alpha$]
trees (\cite{Wi85,Lu78}) trees. Operations key, inf, change\_inf, empty, and 
size take time $O(1)$, insert, lookup, del, and del\_item take time 
$O(\log^2 n)$ and an intersection operation takes time $O(k + \log^2 n)$, 
where $k$ is the size of the returned list. Here $n$ is the current size of 
the set. The space requirement is $O(n\log n)$.}*/

 

#if LEDA_ROOT_INCL_ID == 600129
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
