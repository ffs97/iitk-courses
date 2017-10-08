/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_window.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_WINDOW_H
#define LEDA_D3_WINDOW_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600143
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/graph/graph.h>
#include <LEDA/numbers/rat_vector.h>
#include <LEDA/numbers/vector.h>
#include <LEDA/core/tuple.h>
#include <LEDA/graphics/window.h>

LEDA_BEGIN_NAMESPACE


typedef three_tuple<vector,vector,color> d3win_segment;

/*{\Manpage {d3_window} {} {Windows for 3d visualization} {D}}*/

class __exportC d3_window {

/*{\Mdefinition
The data type |\Mname| supports three-dimensional visualization.
It uses a LEDA window to visualize and animate
three-dimensional  drawings of graph.
For this purpose we need to assign positions in 3d space to all |nodes| of the
graph (see |init|-operations and |set_position|-operation). The |edges| of the  
visualized graph are drawn as straight-line-segments between the 3d positions of
their source and target |nodes|. Note all edges of the graph must have a reversal edge. \\
If the graph to be shown is a planar map the |faces| can be shaded in different
grey scales (if the |solid| flag is |true|).
\\
The graph can be drawn with the |draw|-operation and animated with the |move|-operation. 
The |draw|-operation draws a frontal projection of the graph on the output window.
The |move|-operation starts a simple animation mode.
First it |draws| the graph, then it rotates it (the rotation depends on the |x_rotation|
and |y_rotation| flags and the mouse position) and finally returns the pressed mouse button. 

Every object of type |\Mname| maintains a set of parameters:
\begin{itemize}
\item |x_rotation| (type |bool|); if |true|, rotation about the |x|-axis is enabled during a |move| operation
\item |y_rotation| (type |bool|); if |true|, rotation about the |y|-axis is enabled during a |move| operation
\item |elim| (type |bool|); if |true|, hidden lines will be eliminated
\item |solid| (type |bool|); if |true|, faces have to be drawn in different grey scales
\item |draw_edges| (type |bool|)  enables/disables the redraw of edges 
\item |message| (type |string|) is the message that will be displayed on top of the drawing area of the output window 
\end{itemize}

In addition, a |\Mname| stores information assigned to the nodes and edges of the visualized
graph.
\begin{itemize}
\item |color| (type |color|) information for nodes and edges 
\item |position| (three-dimensional |vector|s) information for the nodes 
\item |arrow| (type |bool|) information for the edges (define whether or not edges have to be drawn as arrows) 
\end{itemize}

}*/

protected:

d3_window() { }

public:

string message_str;

bool   grey_scales;
bool   draw_solid;
bool   elim;
bool   draw_edges;
bool   draw_graph;
bool   node_labels;

color* grey; // array of grey's

int    node_width;

double speed;
double total_rot1;
double total_rot2;
double rot1;
double rot2;
double zoom_f;
vector g_center;
bool   x_rotation;
bool   y_rotation;

//node_array<vector> pos1;
//we use a pointer to work around a bug in g++-2.8 
node_array<vector>* pos1;

node_map<color>    n_clr;
edge_map<color>    e_clr;
edge_map<bool>     draw_arrow;

edge_map<list<vector> > bends;

list<d3win_segment>  seg_list;

window*      Wp;
const graph* Hp;

bool moving;
int  show_bends;


color node_hcolor;
color edge_hcolor;

void init(window&, const graph*,double rotation1=0, double rotation2=0);
void draw_message();
void rotate(double,double, vector&);

enum { no_bends=0, poly_bends=1, bezier_bends=2 };

/*{\Mcreation}*/

d3_window(window& W) { init(W,0); }

d3_window(window& W, const graph& G,double rot1=0,double rot2=0) { init(W,&G,rot1,rot2); }
/*{\Mcreate creates an instance |\Mvar| of the data type |\Mname|.
The output window of |\Mvar| is |W|.
The visualized graph is |G|.}*/

d3_window(window& W, const graph& G, const node_array<vector>& pos) 
/*{\Mcreate creates an instance |\Mvar| of the data type |\Mname|.
The output window of |\Mvar| is |W|.
The visualized graph is |G|.
The positions of the nodes are given in |pos|.
\precond the vectors in |pos| are three-dimensional.}*/
{ init(W,&G);
  init(pos);
 }

d3_window(window& W, const graph& G, 
                                const node_array<rat_vector>& pos) 
/*{\Mcreate creates an instance |\Mvar| of the data type |\Mname|.
The output window of |\Mvar| is |W|.
The visualized graph is |G|.
The positions of the nodes are given in |pos|.
\precond the vectors in |pos| are three-dimensional.}*/
{ init(W,&G);
  init(pos);
 }

~d3_window() { delete[] grey; delete pos1; }


/*{\Moperations 2 5.0 }*/

void init(const node_array<vector>& pos);
/*{\Mop initializes |\Mvar| by setting the node positions of
the visualized graph to the positions given in |pos|.
\precond the vectors in |pos| are three-dimensional.}*/

void init(const node_array<rat_vector>& pos);
/*{\Mop initializes |\Mvar| by setting the node positions of
the visualized graph to the positions given in |pos|.
\precond the vectors in |pos| are three-dimensional.}*/

void init(const graph& G, const node_array<vector>& pos) 
/*{\Mop initializes |\Mvar| by setting the visualized graph
to |G| and the node positions of
the visualized graph to the positions given in |pos|.
\precond the vectors in |pos| are three-dimensional.}*/
{ init(*Wp,&G);
  init(pos);
}

void add_segment(const vector& vec1, const vector& vec2, color col=black)
{ seg_list.append(d3win_segment(vec1,vec2,col)); }

void clear_segments(const vector&, const vector&, color)
{ seg_list.clear(); }

void draw();
/*{\Mop draws the contents of |\Mvar| (see also |Definition|).}*/

void draw_poly();
void draw_segments();

void zoom(double);

void move_graph(double,double);

int  move();
/*{\Mop animates the contents of |\Mvar| until a button is pressed and returns the pressed mouse button.
If the movement is stopped or no mouse button is pressed, |NO_BUTTON|
will be returned, else the number of the pressed mouse button will be returned
(see also |Definition| and the |get_mouse| operation of the |window| data type).}*/

int  get_mouse();
/*{\Mop does the same as |move|.}*/

int  read_mouse();
/*{\Mop calls |move| as long as |move| returns |NO_BUTTON|. Else the movement is stopped,
and the number of the pressed mouse button is returned.}*/

void set_position(node v, double x, double y, double z) ;
/*{\Mop sets the position of node |v| in the visualized graph |\Mvar|
to |(x,y,z)|.}*/

void set_bends(edge e, const list<vector>&);

/*{\Mtext
    \medskip
    {\bf Get- and set-operations} \\
    The following operations can be used to get and set the parameters of |\Mvar|.
    The set-operations return the previous value of the parameter.
  }*/


bool get_x_rotation() const
/*{\Mop returns |true|, if |\Mvar| has rotation about the
|x|-axis enabled, |false| otherwise.}*/
{ return x_rotation; }

bool get_y_rotation() const
/*{\Mop returns |true|, if |\Mvar| has rotation about the
|y|-axis enabled, |false| otherwise.}*/
{ return y_rotation; }

bool set_x_rotation(bool b)  
/*{\Mop enables (disables) rotation about the |x|-axis.}*/
{ bool prev=x_rotation; x_rotation = b; return prev;}


bool set_y_rotation(bool b)  
/*{\Mop enables (disables) rotation about the |y|-axis.}*/
{ bool prev=y_rotation; y_rotation = b; return prev;}

double get_speed() const
/*{\Xop returns the animation speed of |\Mvar|.}*/
{ return speed; }

double set_speed(double s)        
/*{\Xop sets the animation speed of |\Mvar| to |s|.}*/
{ double prev=speed; speed = s; return prev; }

bool get_elim() const
/*{\Mop returns the hidden line elimination flag.}*/
{ return elim; }

bool set_elim(bool b)     
/*{\Mop sets the hidden line elimination flag to |b|. If |b| is |true|, 
hidden lines will be eliminated,
if |b| is |false|, hidden lines will be shown.}*/   
{ bool prev=elim; elim  = b; return prev; }

bool get_solid() const
/*{\Mop returns the |solid| flag of |\Mvar|.}*/
{ return draw_solid; }

bool set_solid(bool b)       
/*{\Mop sets the |solid| flag of |\Mvar| to |b|. If |b| is |true| and
the current graph of |\Mvar| is a planar map, its faces will be painted
in different grey scales, otherwise the faces will be painted white.}*/ 
{ bool prev=draw_solid; draw_solid = b; return prev;}

bool get_draw_edges() const
/*{\Mop return |true|, if edges will be drawn, |false| otherwise.}*/
{ return draw_edges; }

bool set_draw_edges(bool b)  
/*{\Mop enables (disables) the redraw of the edges of |\Mvar|.}*/
{ bool prev=draw_edges; draw_edges = b; return prev;}

int get_show_bends() const { return show_bends; }
int set_show_bends(int s)   
{ int prev=show_bends; show_bends = s; return prev;}

bool get_draw_graph() const { return draw_graph; }
bool set_draw_graph(bool b)  
{ bool prev=draw_graph; draw_graph = b; return prev; }

bool get_node_labels() const
/*{\Xop returns |true|, if node labels are enabled, |false| otherwise.}*/
{ return node_labels; }

bool set_node_labels(bool b) 
/*{\Xop enables (disables) node labels. }*/
{ bool prev=node_labels; node_labels = b; return prev;} 

int get_node_width() const
/*{\Xop returns the width of the nodes of |\Mvar|.}*/
{ return node_width; }

int set_node_width(int w)   
/*{\Xop sets the width of the nodes of |\Mvar| to |w|.}*/
{ int prev=node_width; node_width = w; return prev; } 

string get_message() const
/*{\Mop returns the message that will be displayed on top of the drawing area
of the window.}*/
{ return message_str; }

string set_message(string msg) 
/*{\Mop sets the message that will be displayed on top of the drawing area
of the window to |msg| .}*/
{ string prev=message_str; message_str = msg; return prev;}

void set_center(double x, double y, double z) 
{ g_center = vector(x,y,z); }


void set_node_color(color c);
/*{\Mop sets the color of all nodes of |\Mvar| to |c|.}*/

void set_edge_color(color c);
/*{\Mop sets the color of all edges of |\Mvar| to |c|.}*/

double get_zoom_factor() const;
/*{\Xop return the zoom factor of |\Mvar|.}*/

double set_zoom_factor(double new_factor);
/*{\Xop sets the zoom factor of |\Mvar| to |new_factor|.}*/

bool get_move() const;
bool set_move(bool m);

void set_node_hcolor(color c) { node_hcolor = c; }
void set_edge_hcolor(color c) { edge_hcolor = c; }

color get_color(node v) const;
/*{\Mop returns the color of node |v|.}*/

color set_color(node v,color c);
/*{\Mop sets the color of node |v| to |c|.}*/

color get_color(edge e) const;
/*{\Mop returns the color of edge |e|.}*/

color set_color(edge e,color c);
/*{\Mop sets the color of edge |e| to |c|.}*/

bool get_arrow(edge e) const;
/*{\Mop returns |true|, if |e| will be painted with an arrow, |false|
otherwise.}*/

bool set_arrow(edge e,bool ar);
/*{\Mop if |ar| is |true|, |e| will be painted with an arrow, 
otherwise without an arrow.}*/

void set_msg_string(string s) { message_str = s; }


void get_d2_position(node_array<point>& d2pos);
/*{\Mop returns the two-dimensional positions of the nodes of the graph of |\Mvar|
in |d2pos|.}*/


void get_d2_bends(edge_array<list<point> >& d2bends);

const graph&        get_graph()          { return *Hp; }
node_array<vector>& get_position_array() { return *pos1; }

};


#if LEDA_ROOT_INCL_ID == 600143
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

