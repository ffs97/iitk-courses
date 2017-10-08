/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  POINT_LOCATOR.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/geo/geo_global_enums.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {POINT_LOCATOR} {} {Point Location in Triangulations} {PL}}*/


/*{\Moptions nextwarning=no}*/
/*{\Mdefinition
An instance $PS$ of data type |\Mname| is a data structure for efficient
point location in triangulations.

There are three instantiations of |POINT_LOCATOR|: 
|point_locator| (floating point kernel),
|rat_point_locator| (rational kernel) and
|real_point_locator| (real kernel).
The respective header file name corresponds to the type name (with ``.h''
appended).
}*/



class __exportC POINT_LOCATOR {

const graph& G;
const node_array<POINT>& pos;

array2<node>* M;

double xmin;
double ymin;
double xmax;
double ymax;

double fx;
double fy;

void construct_matrix();

public:

 int number_of_empty_cells() const;
 int number_of_cells() const;


 /*{\Mcreation PS }*/


 POINT_LOCATOR(const GRAPH<POINT,int>& T) : G(T), pos(T.node_data())
 { construct_matrix(); }
 /*{\Mcreate  creates a point locator for a triangulation |T|.}*/ 

 POINT_LOCATOR(const GRAPH<POINT,SEGMENT>& T) : G(T), pos(T.node_data())
 { construct_matrix(); }
 /*{\Mcreate  creates a point locator for a constrained triangulation |T|.}*/ 

 POINT_LOCATOR(const graph& T, node_array<POINT>& p) : G(T), pos(p)
 { construct_matrix(); }
 /*{\Mcreate  creates a point locator for a general triangulation |T|.
              Node positions have to be provided in node\_array |p|. }*/ 

 ~POINT_LOCATOR() { if (M) delete M; }


 /*{\Moperations 1.0 4.5 }*/


 edge locate(POINT q) const;

 /*{\Mop returns an edge |e| of |\Mvar| that contains |q| or that 
  borders the face that contains $q$. In the former case, 
  a hull edge is returned if $q$ lies on the boundary of the convex hull.
  In the latter case we have |\Mvar.orientation(e,q) > 0| except if all 
  points of |\Mvar| are collinear and |q| lies on the induced line. 
  In this case |target(e)| is visible from |q|. 
  The operation returns |nil| if |\Mvar| is empty. }*/

 bool is_hull_edge(edge e) const;
 /*\Mop returns true if |e| is a hull edge of |\Mvar|. }*/

 bool check_locate(POINT q, edge e) const;
 /*{\Mop checks whether |e| could be the result of |\Mvar.locate(q)|. }*/

 bool intersection(const LINE& l, list<edge>& cuts) const;

 bool intersection(const RAY& l, list<edge>& cuts) const;

};


LEDA_END_NAMESPACE
