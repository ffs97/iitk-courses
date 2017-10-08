/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  POINT_SET.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/geo/geo_global_enums.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {POINT_SET} {} {Point Sets and Delaunay Triangulations} {T}}*/


/*{\Moptions nextwarning=no}*/
/*{\Mdefinition

There are three instantiations of |POINT_SET|: 
|point_set| (floating point kernel),
|rat_point_set| (rational kernel) and
|real_point_set| (real kernel).
The respective header file name corresponds to the type name (with ``.h''
appended).

An instance $T$ of data type |\Mname| is a planar embedded bidirected
graph (map) representing the {\em Delaunay Triangulation} of its vertex set.
The position of a vertex $v$ is given by |T.pos(v)| and we use 
$S = \{ |T.pos(v)| \mid v \in T \}$ to denote the underlying point set.
Each face of $T$ (except for the outer face) is  a triangle whose 
circumscribing circle does not contain any point of $S$ in its interior.
For every edge $e$, the sequence 
\[e, |T.face_cycle_succ(e)|, |T.face_cycle_succ(T.face_cycle_succ(e))|,\ldots\]
traces the boundary of the face to the left of $e$.
The edges of the outer face of $T$ form the convex hull of $S$; the trace of 
the convex hull is clockwise.
The subgraph obtained from $T$ by removing all diagonals of
co-circular quadrilaterals  is called the {\em Delaunay Diagram}
of $S$.

|\Mname| provides all {\em constant} graph operations, 
e.g., |T.reversal(e)| returns the reversal of edge $e$, |T.all_edges()|
returns the list of all edges of $T$, and |forall_edges(e,T)| iterates over
all edges of $T$. In addition, |\Mname| provides
operations for inserting and deleting points,
point location, nearest neighbor searches, and navigation in both
the triangulation and the diagram.

|\Mname|s are essentially objects of type |GRAPH<POINT,int>|, where the 
node information is the position of the node and the edge information is 
irrelevant. For a graph $G$ of type |GRAPH<POINT,int>| the function
|Is_Delaunay(G)| tests whether $G$ is a Delaunay triangulation of its vertices.

The data type |\Mname| is illustrated by the |point_set_demo| in the 
LEDA demo directory.

Be aware that the nearest neighbor queries for a point (not for a node) and
the range search queries for circles, triangles, and rectangles are non-const 
operations and modify the underlying graph. The set of nodes and edges is 
not changed; however, it is not guaranteed that the underlying Delaunay 
triangulation is unchanged.

}*/

class __exportC POINT_SET : public  GRAPH<POINT,int>
{
protected:

   // for gw_observer
   vector get_position(node v)
   { point p = inf(v).to_point();
     return p.to_vector();
    }


   edge cur_dart;
   edge hull_dart;

   bool check; // functions are checked if true

   // for marking nodes in search procedures
   int cur_mark;
   node_map<int> mark;

   void init_node_marks()
   { mark.init(*this,-1);
     cur_mark = 0;
   }

   void mark_node(node v) const 
   { ((node_map<int>&)mark)[v] = cur_mark; }

   void unmark_node(node v) const 
   { ((node_map<int>&)mark)[v] = cur_mark - 1; }

   bool is_marked(node v)  const { return mark[v] == cur_mark; }

   void unmark_all_nodes() const 
   { ((int&)cur_mark)++; 
     if ( cur_mark == MAXINT)
     ((POINT_SET*)this) -> init_node_marks();  
                                   //cast away constness
   }

   
   void mark_edge(edge e, delaunay_edge_info k) 
   { assign(e,k); }


   //void del_hull_node(node);

   node  new_node(POINT p) 
   { node v = GRAPH<POINT,int>::new_node(p); 
     mark[v] = -1;
     return v;
   }

   edge  new_edge(node v, node w) 
   { return GRAPH<POINT,int>::new_edge(v,w,0); }

   edge  new_edge(edge e, node w) 
   { return GRAPH<POINT,int>::new_edge(e,w,0,0); }

   void  del_node(node v)  { GRAPH<POINT,int>::del_node(v); }
   void  del_edge(edge e)  { GRAPH<POINT,int>::del_edge(e); }



   void init_hull();
   void make_delaunay(list<edge>&);
   void make_delaunay();


   void check_locate(edge answer,const POINT& p) const;


   //bool simplify_face(node v, list<edge>& E);


   void dfs(node s, const CIRCLE& C, list<node>& L) const;
   /* a procedure used in range_search(const CIRCLE& C) */


   void dfs(node s, const POINT& pv, const POINT& p, list<node>& L) const;
   /* a procedure used in range_search(nodev, const POINT& p) */



   void  draw_voro_edge(const CIRCLE& c1, const CIRCLE& c2,
                        void (*draw_edge)(const POINT&,const POINT&),
                        void (*draw_ray) (const POINT&,const POINT&));


public:

   /*{\Mcreation T }*/

   POINT_SET();
   /*{\Mcreate  creates an empty |POINT_SET| |T|.}*/ 

   POINT_SET(const list<POINT>& S); 
   /*{\Mcreate  creates a |POINT_SET| |T| of the points in $S$. 
                If $S$ contains multiple occurrences of points only the last 
                occurrence of each point is retained.}*/


   POINT_SET(const GRAPH<POINT,int>& G);
   /*{\Mcreate  initializes |T| with a copy of $G$.\\
                \precond |Is_Delaunay(G)| is true.}*/

   POINT_SET(const POINT_SET&);

   POINT_SET& operator=(const POINT_SET&);
   ~POINT_SET() {}

   /*{\Moperations 2.5 5.0 }*/

   void  init(const list<POINT>& L);
   /*{\Mop makes |T|  a |POINT_SET| for the points in $S$.}*/



   POINT  pos(node v) const;
   /*{\Mop  returns the position of node $v$. }*/

   POINT  pos_source(edge e) const;
   /*{\Mop  returns the position of $source(e)$. }*/

   POINT  pos_target(edge e) const;
   /*{\Mop  returns the position of $target(e)$. }*/

   SEGMENT seg(edge e) const;
   /*{\Mop   returns the line segment corresponding to edge $e$ 
             (|SEGMENT(T.pos_source(e),T.pos_target(e))|). }*/

   LINE    supporting_line(edge e) const;
   /*{\Mop   returns the supporting line of edge $e$ 
             (|LINE(T.pos_source(e),T.pos_target(e))|). }*/

   int     orientation(edge e, POINT p) const;
   /*{\Mop   returns $orientation(T.seg(e),p)$.}*/

   int     dim() const;
   /*{\Mop   returns $-1$ if $S$ is empty, returns $0$ if $S$
   consists of only one point,     
             returns $1$ if $S$ consists of at least two points 
   and all points in $S$ 
             are collinear, and returns $2$ otherwise.
   }*/

   list<POINT>  points() const;
   /*{\Mop  returns $S$. }*/
   
   bool         get_bounding_box(POINT& lower_left, POINT& upper_right) const;
   /*{\Mop  returns the lower left and upper right corner of the bounding box of |\Mvar|. The operation returns
   |true|, if |\Mvar| is not empty, |false| otherwise. }*/
   
   list<node>   get_convex_hull() const;
   /*{\Mop  returns the convex hull of |\Mvar|.}*/

   edge   get_hull_dart() const;
   /*{\Mop  returns a dart of the outer face  of |T| (i.e., a dart of the 
            convex hull). }*/

   edge   get_hull_edge() const;
   /*{\Mop  as above. }*/

   bool   is_hull_dart(edge e) const;
   /*{\Mop  returns true if $e$ is a dart of the convex hull of |T|, i.e.,
   a dart on the face cycle of the outer face. }*/

   bool   is_hull_edge(edge e) const;
   /*{\Mop  as above. }*/

   bool   is_diagram_dart(edge e) const;
   /*{\Mop  returns true if $e$ is a dart of the Delaunay diagram, i.e., either
   a dart on the convex hull or a dart where the incident triangles have 
   distinct circumcircles.}*/

   bool   is_diagram_edge(edge e) const;
   /*{\Mop  as above. }*/


   edge   d_face_cycle_succ(edge e) const;
   /*{\Mop  returns the face cycle successor of $e$ in the Delaunay diagram 
            of |T|. \precond  $e$ belongs to the Delaunay diagram.}*/

   edge   d_face_cycle_pred(edge e) const;
   /*{\Mop  returns the face cycle predecessor of $e$ in the Delaunay diagram 
            of |T|. \precond  $e$ belongs to the Delaunay diagram.}*/

   bool   empty() { return number_of_nodes() == 0; }
   /*{\Mop  decides whether |T| is empty. }*/

   void   clear() { GRAPH<POINT,int>::clear(); cur_dart = hull_dart = nil; }
   /*{\Mop  makes |T| empty. }*/



   edge   locate(POINT p, edge loc_start = NULL) const;
   /*{\Mop  returns an edge |e| of |T| that contains |p| or that 
   borders the face that contains $p$. In the former case, 
   a hull dart is returned if $p$ lies on the boundary of the convex hull.
   In the latter case we have 
   |T.orientation(e,p) > 0| except if all points of |T| are collinear and 
   |p| lies on the induced line. In this case |target(e)| is visible from 
   |p|. The function returns |nil| if |T| has no edge.
   The optional second argument is an edge of |\Mvar|, where the |locate| 
   operation starts searching. }*/


   edge   locate(POINT p, const list<edge>& loc_start) const;
   /*{\Mop  returns |locate(p,e)| with |e| in |loc_start|. 
	    If |loc_start| is empty, we return |locate(p, NULL)|. 
	    The operation tries to choose a good starting edge for the $locate$ 
	    operation from |loc_start|.
	    \precond All edges in |loc_start| must be edges of |\Mvar|. }*/

   node   lookup(POINT p, edge loc_start = NULL) const;
   /*{\Mop  if |T| contains a node $v$ with $|pos(v)| = p$ 
   the result is $v$ otherwise the result is |nil|. 
   The optional second argument is an edge of |\Mvar|, where the |locate| 
   operation starts searching |p|. }*/

   node   lookup(POINT p, const list<edge>& loc_start) const;
   /*{\Mop  returns |lookup(p,e)| with |e| in |loc_start|.
            If |loc_start| is empty, we return |lookup(p, NULL)|. 
	    The operation tries to choose a good starting edge for the $lookup$ 
	    operation from |loc_start|.
	    \precond All edges in |loc_start| must be edges of |\Mvar|.}*/   

   node   insert(POINT p);
   /*{\Mop  inserts point $p$ into |T| and returns the corresponding node. 
            More precisely, if there is already a node |v| in |T| positioned 
            at $p$ (i.e., |pos(v)| is equal to |p|) then |pos(v)| is changed to 
            |p| (i.e., |pos(v)| is made identical to |p|) and if there is no 
            such node then a new node $v$ with |pos(v) = p| is added to |T|. 
            In either case, $v$ is returned.}*/


   void del(node v);

   /*{\Mop removes the node $v$, i.e., makes |T| a Delaunay triangulation 
   for $S \setminus \sset{|pos(v)|}$. }*/

   void del(POINT p);

   /*{\Mop removes the node $p$, i.e., makes |T| a Delaunay triangulation 
   for $S \setminus p$. }*/


   node   nearest_neighbor(POINT p);
   /*{\Mop  computes a node $v$ of |T| that 
   is closest to $p$,
   i.e., $|dist(p,pos(v))| = \min\{\ |dist(p,pos(u))| \mid u\in T\ \}$. This is
   a non-const operation.}*/

   node   nearest_neighbor(node w) const;
   /*{\Mop  computes a node $v$ of |T| that 
   is closest to $p = T[w]$,
   i.e., $|dist(p,pos(v))| = \min\{\ |dist(p,pos(u))| \mid u\in T\ \}$.}*/

   node   nearest_neighborA(node w) const;
   node   nearest_neighborC(POINT p);
   node   nearest_neighborD(POINT p) const; 

   list<node>   nearest_neighbors(POINT p, int k);
   /*{\Mop  returns the $k$ nearest neighbors of $p$, i.e., a list 
   of the $\min(k,\Labs{S})$ nodes of |T| closest to $p$. The list is
   ordered by distance from |p|. This is a non-const operation.}*/ 

   list<node>   nearest_neighbors(node w, int k) const;
   /*{\Mop  returns the $k$ nearest neighbors of $p = T[w]$, i.e., a list 
   of the $\min(k,\Labs{S})$ nodes of |T| closest to $p$. The list is
   ordered by distance from |p|.}*/ 


   list<node> range_search(const CIRCLE& C);
   /*{\Mop returns the list of all nodes contained in the closure of disk $C$.\\
           \precond $C$ must be a proper circle (not a straight line). 
   This is a non-const operation.}*/

   list<node> range_search(node v,const POINT& p) const;
   /*{\Mop returns the list of all nodes contained in the closure of disk $C$
   with center $pos[v]$ and having $p$ in its boundary.}*/


   list<node> range_search(const POINT& a, const POINT& b, const POINT& c);
   /*{\Mop returns the list of all nodes contained in the closure 
           of the triangle $(a,b,c)$.\\
           \precond $a$, $b$, and $c$ must not be collinear. 
           This is a non-const operation.}*/
   
   
   list<node> range_search_parallelogram(const POINT& a, const POINT& b, 
                                                         const POINT& c);
   /*{\Mop returns the list of all nodes contained in the closure of the 
           parallelogram $(a,b,c,d)$  with $d = a + (c-b)$.\\
           \precond $a$, $b$, and $c$ must not be collinear. 
           This is a non-const operation.}*/   

   list<node> range_search(const POINT& a, const POINT& b);
   /*{\Mop returns the list of all nodes contained in the closure 
           of the rectangle with diagonal $(a,b)$. 
           This is a non-const operation.}*/


   list<edge> minimum_spanning_tree() const; 
   /*{\Mop returns the list of edges of |T| that comprise a minimum spanning
           tree of |S|. }*/
   
   list<edge> relative_neighborhood_graph();
   /*{\Mop returns the list of edges of |T| that comprise a relative 
           neighborhood graph of |S|. }*/   

   void   compute_voronoi(GRAPH<CIRCLE,POINT>& V) const;
   /*{\Mop  computes the corresponding Voronoi diagram $V$. 
            Each node of $VD$ is labeled with its defining circle.
            Each edge is labeled with the site lying in the face to
            its left. }*/


   /*{\Mtext {\bf Drawing Routines}

   The functions in this section were designed to support the drawing of 
   Delaunay triangulations and Voronoi diagrams. 
   \setlength{\typewidth}{0cm}
   \setlength{\callwidth}{3cm}
   \computewidths
   }*/

   void   draw_nodes(void (*draw_node)(const POINT&)); 
   /*{\Mopl calls |draw_node(pos(v))| for every node $v$ of $T$. }*/


   void   draw_edge(edge e,
                    void (*draw_diagram_edge)(const POINT&,const POINT&),
                    void (*draw_triang_edge) (const POINT&,const POINT&),
                    void (*draw_hull_dart)   (const POINT&,const POINT&));

   /*{\Mopl calls
      |draw_diagram_edge(pos_source(e),pos_target(e)| if |e| is a diagram dart,
      |draw_hull_dart(pos_source(e),pos_target(e)| if |e| is a hull dart,
      and |draw_triang_edge(pos_source(e),pos_target(e)| if |e| is a
      non-diagram edge. }*/


   void   draw_edges( void (*draw_diagram_edge)(const POINT&,const POINT&),
                      void (*draw_triang_edge) (const POINT&,const POINT&),
                      void (*draw_hull_dart)   (const POINT&,const POINT&));
   /*{\Mopl calls the corresponding function for all edges of |T|.}*/


   void   draw_edges(const list<edge>& L,
                     void (*draw_edge)(const POINT&, const POINT&));
   /*{\Mopl calls |draw_edge(pos_source(e),pos_target(e)| for 
   every edge $e \in L$. }*/


   void   draw_voro_edges(void (*draw_edge)(const POINT&,const POINT&),
                          void (*draw_ray) (const POINT&,const POINT&));

   /*{\Mopl calls |draw_edge| and |draw_ray| for the edges of the 
   Voronoi diagram.}*/


   void   draw_hull(void (*draw_poly)(const list<POINT>&));
   /*{\Mopl calls |draw_poly| with the list of vertices of the convex hull. }*/


   void   draw_voro(const GRAPH<CIRCLE,POINT>&,
                    void (*draw_node)(const POINT&), 
                    void (*draw_edge)(const POINT&,const POINT&),
                    void (*draw_ray) (const POINT&,const POINT&));
   /*{\Mopl calls ... }*/



   
   void checking_on()
   { check = true; }
   /*{\Xop enables checking mode.}*/

   void checking_off()
   { check = false; }
   /*{\Xop disables checking mode.}*/

   void save_state(const POINT& p) const;
   /*{\Xop in checking mode: saves the state of the data structure on 
   files [[point_set_error.graph]] and [[point_set_error.aux]]. 
   The graph part is written on the former file and
   |hull_dart|, |cur_dart| and $p$ are written on the latter file.
   No action is performed if the checking mode is disabled.
   }*/

   void save_state(const POINT& p, const edge& answer) const;
   /*{\Xop in checking mode: saves the state of the data structure on 
   files [[point_error.graph]] and [[point_error.aux]]. 
   The graph part is written on the former file and
   |hull_dart|, |cur_dart|, $p$, and |answer| are written on the latter file.
   No action is performed if the checking mode is disabled.
   }*/

   bool check_state(const string& location) const;
   /*{\Xop in checking mode: 
   checks the current state of the data structure. 
   If it finds an error it writes a diagnostic messages to |cerr|.
   No action is performed if the checking mode is disabled.}*/

   bool     IS_NON_DIAGRAM_DART(edge e) const;
   /*{\Xop checks whether $e$ is a non-diagram dart. }*/

   bool     IS_NON_DIAGRAM_EDGE(edge e) const;
   /*{\Xop as above. }*/

   // for testing
   edge get_cur_dart()  const { return cur_dart; }
   void set_cur_dart(edge e)  { cur_dart = e;    }
   void set_hull_dart(edge e) { hull_dart = e;   }

   /*
     edge get_cur_edge()  const { return cur_dart; }
     void set_cur_edge(edge e)  { cur_dart = e;    }
     void set_hull_edge(edge e) { hull_dart = e;   }
     */

};



inline POINT POINT_SET::pos(node v) const
{ return inf(v); }

inline POINT POINT_SET::pos_source(edge e) const
{ return inf(source(e)); }

inline POINT POINT_SET::pos_target(edge e) const
{ return inf(target(e)); }

inline SEGMENT POINT_SET::seg(edge e) const           
{ return SEGMENT(inf(source(e)),inf(target(e))); }

inline LINE POINT_SET::supporting_line(edge e) const           
{ return LINE(inf(source(e)),inf(target(e))); }

inline edge  POINT_SET::get_hull_dart() const
{ return hull_dart; }

inline edge  POINT_SET::get_hull_edge() const
{ return hull_dart; }

inline bool  POINT_SET::is_diagram_dart(edge e) const
{ return (inf(e) & NON_DIAGRAM_DART) == 0; }

inline bool  POINT_SET::is_diagram_edge(edge e) const
{ return POINT_SET::is_diagram_dart(e); }

inline bool  POINT_SET::is_hull_dart(edge e) const
{ return (inf(e) & HULL_DART) != 0; }

inline bool  POINT_SET::is_hull_edge(edge e) const
{ return POINT_SET::is_hull_dart(e); }


inline int POINT_SET::orientation(edge e, POINT p) const
{ return pos(source(e)).orientation(pos(target(e)),p); }

inline int  POINT_SET::dim() const
{ int n = number_of_nodes();
  if (n <= 1) 
    return n - 1;
  else
    return (is_hull_dart(reversal(hull_dart))) ? 1 : 2 ;
}




/*{\Mimplementation
The main ingredients for the implementation are Delaunay flipping, 
segment walking, and plane sweep.

The constructor |POINT_SET(list<POINT> S)| first constructs a 
triangulation of |S| by sweeping and then makes the triangulation Delaunay by
a sequence of Delaunay flips. 

|Locate| walks through the triangulation along the segment from some 
fixed point of $T$ to the query point. |Insert| first locates the point, 
then updates the triangulation locally, and finally performs flips to 
reestablish the Delaunay property. |Delete| deletes the node, 
retriangulates the resulting face, and then performs flips. Nearest 
neighbor searching, circular range queries, and triangular range 
queries insert the query point into the triangulation, then perform 
an appropriate graph search on the triangulation, and finally remove
the query point.

All algorithms show good expected behavior.

For details we refer the reader to the LEDA implementation report 
"Point Sets and Dynamic Delaunay Triangulations".

}*/


LEDA_END_NAMESPACE
