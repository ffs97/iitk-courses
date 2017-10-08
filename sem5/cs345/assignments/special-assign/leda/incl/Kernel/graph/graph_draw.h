/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graph_draw.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GRAPH_DRAW_H
#define LEDA_GRAPH_DRAW_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600003
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/graph/graph.h>

LEDA_BEGIN_NAMESPACE


/*{\Manpage {graph_draw} {} {Graph Drawing Algorithms}}*/

/*{\Mtext
\bigskip
This section gives a summary of the graph drawing algorithms contained in 
LEDA. Before using them the header file {\tt <}LEDA/graph/graph\_draw.h{\tt >} 
has to be included.
\bigskip
}*/


extern __exportF int STRAIGHT_LINE_EMBED_MAP(graph& G, node_array<int>& xcoord, 
                                                       node_array<int>& ycoord);

/*{\Mfuncl
STRAIGHT\_LINE\_EMBED\_MAP takes as argument a graph $G$ representing
a planar map. It computes a straight line embedding of $G$ by assigning 
non-negative integer coordinates ($xcoord$ and $ycoord$) in the range 
$0..2(n-1)$ to the nodes. STRAIGHT\_LINE\_EMBED\_MAP returns the maximal 
coordinate.
The algorithm (\cite{Fa48}) has running time $O(\Labs{V}^2)$.
\bigskip
}*/


extern __exportF int STRAIGHT_LINE_EMBEDDING(graph& G, node_array<int>& xc,
                                                       node_array<int>& yc);
/*{\Mfuncl
STRAIGHT\_LINE\_EMBEDDING takes as argument a planar graph $G$
and computes a straight line embedding of $G$ by assigning 
non-negative integer coordinates ($xcoord$ and $ycoord$) in the range 
$0..2(n-1)$ to the nodes. The algorithm returns the maximal coordinate
and has running time $O(\Labs{V}^2)$.
\bigskip
}*/

extern __exportF void STRAIGHT_LINE_EMBEDDING(graph& G, node_array<double>& xc,
                                                        node_array<double>& yc);


extern __exportF int STRAIGHT_LINE_EMBEDDING2(graph& G, node_array<int>& xcoord,
                                       node_array<int>& ycoord);

extern __exportF void STRAIGHT_LINE_EMBEDDING2(graph& G,node_array<double>& xcoord,
                                       node_array<double>& ycoord);



extern __exportF bool VISIBILITY_REPRESENTATION(graph& G, 
                                                node_array<double>& x_pos,
                                                node_array<double>& y_pos,
                                                node_array<double>& x_rad,
                                                node_array<double>& y_rad,
                                                edge_array<double>& x_sanch,
                                                edge_array<double>& y_sanch,
                                                edge_array<double>& x_tanch,
                                                edge_array<double>& y_tanch);
/*{\Mfuncl
     computes a visibility representation of the graph |G|, i.e., each node is
     represented by a horizontal segment (or box) and each edge is represented
     by a vertical segment.\\
	 \precond $G$ must be planar and has to contain at least three nodes.}*/


extern __exportF bool TUTTE_EMBEDDING(const graph& G, 
                                      const list<node>& fixed_nodes,
                                      node_array<double>& xpos, 
                                      node_array<double>& ypos);
/*{\Mfuncl
   computes a convex drawing of the graph $G$ if possible. The list
   |fixed_nodes| contains nodes with prescribed coordinates already
   given in |xpos| and |ypos|. The computed node positions of the other 
   nodes are stored in |xpos| and |ypos|, too. If the operation is 
   successful, true is returned. }*/
 

extern __exportF void SPRING_EMBEDDING(const graph& G, const list<node>& fixed,
                                                node_array<double>& xpos,
                                                node_array<double>& ypos,
                                                node_array<double>& xrad,
                                                node_array<double>& yrad,
                                                double xleft,   double xright,
                                                double ybottom, double ytop,
                                                int iterations=250);

extern __exportF void SPRING_EMBEDDING(const graph& G, 
                                                node_array<double>& xpos,
                                                node_array<double>& ypos,
                                                node_array<double>& xrad,
                                                node_array<double>& yrad,
                                                int iterations=250);

                                                           
extern __exportF void SPRING_EMBEDDING(const graph& G, node_array<double>& xpos,
                                                node_array<double>& ypos,
                                                double xleft,   double xright,
                                                double ybottom, double ytop,
                                                int iterations=250);
/*{\Mfuncl computes a straight-line spring embedding of $G$ in the given 
           rectangular region. The coordinates of the computed node positions
           are returned in |xpos| and |ypos|. }*/

extern __exportF void SPRING_EMBEDDING(const graph& G, const list<node>& fixed,
                                                node_array<double>& xpos,
                                                node_array<double>& ypos,
                                                double xleft,   double xright,
                                                double ybottom, double ytop,
                                                int iterations=250);
/*{\Mfuncl as above, however, the positions of all nodes in the |fixed| list
           is not changed. }*/


extern __exportF void D2_SPRING_EMBEDDING(const graph& G, 
                                           node_array<double>& xpos, 
                                           node_array<double>& ypos,
                                           double xleft, double xright, 
                                           double ybottom, double ytop,
                                           int iterations=250);

extern __exportF void D2_SPRING_EMBEDDING1(const graph& G, 
                                           node_array<double>& xpos, 
                                           node_array<double>& ypos,
                                           double xleft, double xright, 
                                           double ybottom, double ytop,
                                           int iterations=250);


extern __exportF void D3_SPRING_EMBEDDING(const graph& G, 
                                          node_array<double>& xpos, 
                                          node_array<double>& ypos,
                                          node_array<double>& zpos,
                                          double xmin, double xmax, 
                                          double ymin, double ymax,
                                          double zmin, double zmax,
                                          int iterations=250);
/*{\Mfuncl computes a straight-line spring embedding of $G$ in the 3-dimensional
           space. The coordinates of the computed node positions
           are returned in |xpos|, |ypos|, and |zpos|. }*/




extern __exportF int ORTHO_EMBEDDING(const graph& G,
                                     const edge_array<int>& maxbends,
                                           node_array<int>& xcoord,
                                           node_array<int>& ycoord,
                                           edge_array<list<int> >& xbends,
                                           edge_array<list<int> >& ybends);
/*{\Mfuncl Produces an orthogonal (Tamassia) embedding such that each edge 
           $e$ has at most |maxbends[e]| bends. Returns |true| if such an 
           embedding exists and false otherwise. 
           \precond $G$ must be a planar 4-graph. }*/


inline int ORTHO_EMBEDDING(const graph& G, node_array<int>& xpos,
                                           node_array<int>& ypos,
                                           edge_array<list<int> >& xbends,
                                           edge_array<list<int> >& ybends) 
{ edge_array<int> maxbends(G, MAXINT);
  return ORTHO_EMBEDDING(G, maxbends, xpos, ypos, xbends, ybends);
}
/*{\Mfuncl as above, but with unbounded number of edge bends. }*/





// double versions

extern __exportF int ORTHO_EMBEDDING(const graph&,
                                     const edge_array<int>& maxbends,
                                           node_array<double>& xcoord,
                                           node_array<double>& ycoord,
                                           edge_array<list<double> >& xbends,
                                           edge_array<list<double> >& ybends);

inline int ORTHO_EMBEDDING(const graph& G, node_array<double>& xpos, 
                                           node_array<double>& ypos,
                                           edge_array<list<double> >& xbends,
                                           edge_array<list<double> >& ybends)
{ edge_array<int> maxbends(G, MAXINT);
  return ORTHO_EMBEDDING(G, maxbends, xpos, ypos, xbends, ybends);
}





extern __exportF bool ORTHO_DRAW(const graph& G0, 
                                 node_array<double>& xpos,
                                 node_array<double>& ypos,
                                 node_array<double>& xrad,
                                 node_array<double>& yrad,
                                 edge_array<list<double> >& xbends,
                                 edge_array<list<double> >& ybends,
                                 edge_array<double>& xsanch,
                                 edge_array<double>& ysanch,
                                 edge_array<double>& xtanch,
                                 edge_array<double>& ytanch);

/*{\Mfuncl computes a orthogonal drawing of an arbitrary planar graph 
           (nodes of degree larger than 4 are allowd) in the so-called
           Giotto-Model, i.e. high-degree vertices (of degree greater than 4)
           will be represented by larger rectangles. }*/




// series-parallel graph

extern __exportF bool SP_ORTHO_EMBEDDING(const graph& G, double dist,
                 			double rel_node_width,
                                        double rel_node_height, 
                 			node_array<vector>& coords,
                 			node_array<double>& node_width,
                 			node_array<double>& node_height,
                 			edge_array<list<vector> >& bends,
                 			edge_array<vector>& source_anchor,
                 			edge_array<vector>& target_anchor,
                 			double dx = 1, double dy = 1,
                                        double x0 = 0, double y0 = 0);

extern __exportF bool SP_EMBEDDING(const graph& G, double dist,
		                   node_array<vector>& coords,
		                   edge_array<list<vector> >& bends,
		                   double dx = 1, double dy = 1,
                                   double x0 = 0, double y0 = 0);
		  


extern __exportF bool D3_SP_EMBEDDING(const graph& G, double bend_dist,
		                      node_array<vector>& coords,
                 		      edge_array<list<vector> >& bends,
                 		      double dx = 1, double dy = 1,
                 		      double x0 = 0, double y0 = 0);




extern __exportF bool SP_EMBEDDING(graph& G,
		                   node_array<double>& x_coord,
                                   node_array<double>& y_coord,
		                   node_array<double>& x_radius,
                                   node_array<double>& y_radius,
                                   edge_array<list<double> >& x_bends,
                                   edge_array<list<double> >& y_bends,
                                   edge_array<double>& x_sanch,
                                   edge_array<double>& y_sanch,
                                   edge_array<double>& x_tanch,
                                   edge_array<double>& y_tanch);

/*{\Mfuncl computes a series-parallel drawing of $G$.\\
           \precond $G$ must be a series-parallel graph.}*/
 


#if LEDA_ROOT_INCL_ID == 600003
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif



LEDA_END_NAMESPACE

#endif

