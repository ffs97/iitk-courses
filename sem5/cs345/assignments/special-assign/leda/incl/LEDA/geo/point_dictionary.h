/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  point_dictionary.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_POINT_DICTIONARY_H
#define LEDA_POINT_DICTIONARY_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600118
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/geo/point.h>
#include <LEDA/geo/d2_dictionary.h>

LEDA_BEGIN_NAMESPACE

typedef dic2_item  pd_item;


class __exportC Point_Dictionary : public d2_dictionary<double,double,void*>
{

public:

 Point_Dictionary() {}
~Point_Dictionary() {}

 pd_item  nearest_neighbor(double x, double y);

 list<point> all_points();

// list<pd_item> convex_hull();

};




/*{\Manpage {point_dictionary} {I} {Dictionaries for Two-Dimensional Points}}*/ 	

template<class I>

class point_dictionary : public Point_Dictionary {

/*{\Mdefinition
An instance $S$ of the parameterized data type |\Mname| is a collection
of items ($ps\_item$). Every item in $S$ contains a two-dimensional point as
key (data type $point$), and an information from data type $I$, called the 
information type of $S$. The number of items in $S$ is called the size of $S$. 
A point set of size zero is said to be empty. We use $\<p,i\>$ to denote the
item with point $p$, and information $i$. For each  point $p$ there is at most
one item $\<p,i\> \in S$. Beside the normal dictionary operations, the data
type $point\_set$ provides operations for rectangular range queries and
nearest neighbor queries.}*/


void rt_clear_inf(GenPtr& x) const { LEDA_CLEAR(I,x); }
void rt_copy_inf(GenPtr& x) const  { LEDA_COPY(I,x);  }

public:

/*{\Mcreation S }*/

 point_dictionary() {}
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| and initializes |\Mvar| to 
            the empty set.}*/

~point_dictionary() { clear(); }

/*{\Moperations 2.5 5.2}*/

point key(pd_item it) {return point(Point_Dictionary::key1(it),Point_Dictionary::key2(it)); }
/*{\Mop     returns the point of item $it$.\\
            \precond $it$ is an item in |\Mvar|.}*/

const I& inf(pd_item it)  { return LEDA_CONST_ACCESS(I,Point_Dictionary::inf(it)); }
/*{\Mop     returns the information of item $it$.\\
	    \precond $it$ is an item in |\Mvar|.}*/

pd_item insert(point p, const I& i) 
{ return Point_Dictionary::insert(p.xcoord(),p.ycoord(),leda_copy(i));}
/*{\Mop    associates the information $i$ with point $p$. 
	   If there is an item $\<p,j\>$ in |\Mvar| then $j$ 
	   is replaced by $i$, else a new item $\<p,i\>$ 
	   is added to $S$. In both cases the item is 
	   returned.}*/

pd_item lookup(point p) {return Point_Dictionary::lookup(p.xcoord(),p.ycoord()); }
/*{\Mop    returns the item with point $p$ (nil if no 
	   such item exists in \Mvar).}*/

pd_item nearest_neighbor(point q) 
{ return Point_Dictionary::nearest_neighbor(q.xcoord(),q.ycoord()); }
/*{\Mop    returns the item $\<p,i\>\ \in\ S$ such that 
	   the distance between $p$ and $q$ is minimal.}*/

list<pd_item> range_search(double x0, double x1, double y0, double y1)
{ return Point_Dictionary::range_search(x0,x1,y0,y1);}
/*{\Mopl   returns all items $\<p,i\>\ \in\ S$ with\\
	   $x_0 \le p$.xcoord() $\le x_1$ and\\
	   $y_0 \le p$.ycoord() $\le y_1$.}*/


// list<pd_item> convex_hull() {return Point_Dictionary::convex_hull();}
 /*{\Xop    returns the list of items containing all points 
            of the convex hull of |\Mvar| in clockwise order.}*/


void del(point p) { Point_Dictionary::del(p.xcoord(),p.ycoord()); }
/*{\Mop    deletes the item with point $p$ from |\Mvar|.}*/

void del_item(pd_item it) { Point_Dictionary::del_item(it); }
/*{\Mop    removes item $it$ from |\Mvar|.\\
           \precond $it$ is an item in |\Mvar|.}*/

void    change_inf(pd_item it, const I& i) 
{ Point_Dictionary::change_inf(it,leda_copy(i)); }
/*{\Mop    makes $i$ the information of item $it$.\\
	   \precond $it$ is an item in |\Mvar|.}*/

list<pd_item> all_items() { return Point_Dictionary::all_items(); }
/*{\Mop    returns the list of all items in $S$.}*/ 

list<point> all_points()  { return Point_Dictionary::all_points(); }
/*{\Mop    returns the list of all points in $S$.}*/ 

void clear() { Point_Dictionary::clear(); }
/*{\Mop    makes |\Mvar| the empty point\_set.}*/

bool empty() { return Point_Dictionary::empty(); }
/*{\Mop    returns true iff |\Mvar| is empty.}*/

int size()  { return Point_Dictionary::size(); }
/*{\Mop    returns the size of |\Mvar|.}*/

};

#define forall_pd_items(i,D) forall(i, (D.all_items()) )

/*{\Mimplementation
Point dictionaries are implemented by a combination of two-dimensional range 
trees \cite{Wi85,Lu78} and Voronoi diagrams. Operations insert, lookup, 
del\_item, del take time $O(\log^2 n)$, key, inf, empty, size, change\_inf 
take time $O(1)$, and clear takes time $O(n\log n)$. A range\_search operation 
takes time $O(k+\log^2 n)$, where $k$ is the size of the returned list. 
A nearest\_neighbor query takes time $O(n)$. Here $n$ is the current size of 
the point set. The space requirement is $O(n\log n)$.}*/


#if LEDA_ROOT_INCL_ID == 600118
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

