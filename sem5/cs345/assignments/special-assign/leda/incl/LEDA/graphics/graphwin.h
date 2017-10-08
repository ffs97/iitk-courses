/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  graphwin.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GRAPHWIN_H
#define LEDA_GRAPHWIN_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600132
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graphics/window.h>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_map2.h>
#include <LEDA/core/map.h>
#include <LEDA/core/tuple.h>
#include <LEDA/system/event.h>
#include <LEDA/graphics/motion.h>

#include <LEDA/system/socket.h>

LEDA_BEGIN_NAMESPACE


class __exportC GraphWin;
class __exportC PostScript;
class __exportC gw_observer;
class __exportC graphwin_undo;


//------------------------------------------------------------------------------
// Global Constants
//------------------------------------------------------------------------------

#define CR_NONE	    0
#define CR_STUFF    (1 << 1)
#define CR_WIN	    (1 << 2)
#define CR_GRAPH    (1 << 3)


#define A_LEFT	    (1 << 0)
#define A_MIDDLE    (1 << 1)
#define A_RIGHT	    (1 << 2)
#define A_SCROLL_UP (1 << 3)
#define A_SCROLL_DN (1 << 4)

#define A_SHIFT	    (1 << 5)
#define A_CTRL	    (1 << 6)
#define A_ALT	    (1 << 7)
#define A_DOUBLE    (1 << 8)
#define A_DRAG      (1 << 9)
#define A_NODE      (1 << 10)
#define A_EDGE      (1 << 11)
#define A_FRAME     (1 << 12)
#define A_SLIDER    (1 << 13)
#define A_IMMEDIATE (1 << 14)
#define A_STOP      (1 << 15)
#define A_MASK      (A_STOP-1)

#define A_NODE_MENU (1 << 16)
#define A_EDGE_MENU (1 << 17)
  

#define M_NONE	   0
#define M_FILE	   (1 << 0)
#define M_EDIT	   (1 << 1)
#define M_GRAPH	   (1 << 2)
#define M_LAYOUT   (1 << 3)
#define M_WINDOW   (1 << 4)
#define M_OPTIONS  (1 << 5)
#define M_HELP	   (1 << 6)
#define M_DONE	   (1 << 7)
#define M_UNDO	   (1 << 8)
#define M_COMPLETE ((1 << 9) - 1)

#define M_DEFAULT  (1 << 15)

#define M_READONLY (1 << 31)


#define N_COLOR	   (1 << 0)
#define N_SHAPE	   (1 << 1)
#define N_BCOLOR   (1 << 2)
#define N_BWIDTH   (1 << 3)
#define N_WIDTH    (1 << 4)
#define N_HEIGHT   (1 << 5)
#define N_LABEL    (1 << 6)
#define N_LTYPE    (1 << 7)
#define N_LCOLOR   (1 << 8)
#define N_LPOS     (1 << 9)
#define N_SELECT   (1 << 10)
#define N_ALL      ((1 << 11) - 1)
#define N_XPOS     (1 << 11)
#define N_YPOS     (1 << 12)
#define N_PIXMAP   (1 << 13)
#define N_COMPLETE ((1 << 14) - 1)

#define N_POS      (N_XPOS | N_YPOS)
#define N_SIZE     (N_WIDTH | N_HEIGHT)


#define E_COLOR	   (1 << 0)
#define E_SHAPE    (1 << 1)
#define E_STYLE    (1 << 2)
#define E_DIR      (1 << 3)
#define E_WIDTH    (1 << 4)
#define E_LABEL    (1 << 5)
#define E_LTYPE    (1 << 6)
#define E_LCOLOR   (1 << 7)
#define E_LPOS     (1 << 8)
#define E_SELECT   (1 << 9)
#define E_ALL      ((1 << 10) - 1)
#define E_POLYGON  (1 << 10)
#define E_SANCHOR  (1 << 11)
#define E_TANCHOR  (1 << 12)
#define E_COMPLETE ((1 << 13) - 1)


#define STAT_CLEAR    0
#define STAT_NODES    1
#define STAT_EDGES    2
#define STAT_LOOPS    4
#define STAT_PLANAR   8
#define STAT_FLUSH   16
#define STAT_GRID    32
#define STAT_COORD   64 
#define STAT_UNDO   128 


#define MAX_SLIDERS   3



//------------------------------------------------------------------------------
// Global Types
//------------------------------------------------------------------------------

typedef void (*win_redraw_func)(window*, double, double, double, double);

typedef void (*gw_action)(GraphWin&,const point&);
inline ostream& operator<<(ostream& ostr, const gw_action&) { return ostr; }
inline istream& operator>>(istream& istr, gw_action&)       { return istr; }
inline int compare(const gw_action&, const gw_action&)      { return 0; }


class gw_base_function 
{ 
  friend class GraphWin;
  bool enabled;
  virtual void call(GraphWin&) = 0; 

public:
  gw_base_function() : enabled(true) {}
  virtual ~gw_base_function() {}
};



// attribute types


enum gw_node_shape { circle_node, ellipse_node, square_node, rectangle_node,
                     roundrect_node, ovalrect_node, rhombus_node, 
                     triang0_node, triang1_node, triang2_node, triang3_node };

enum gw_edge_style { solid_edge, dashed_edge, dotted_edge, dashed_dotted_edge };
enum gw_edge_shape { poly_edge, circle_edge, bezier_edge, spline_edge };

enum gw_edge_dir   { undirected_edge=0, directed_edge=1, redirected_edge=2, 
                     bidirected_edge=3, mid_directed_edge=4,
                     mid_redirected_edge=8, mid_bidirected_edge=12 };

enum gw_position   { northwest_pos, north_pos,   northeast_pos,
                     west_pos,      central_pos, east_pos,
                     southwest_pos, south_pos,   southeast_pos };

enum gw_label_type { no_label=0, user_label=1, data_label=2, index_label=4 };

enum gw_font_type  { roman_font, bold_font, italic_font, fixed_font,user_font };

enum node_move_t   { move_single_node , move_all_nodes };
    
enum ps_zoom_mode  { ps_zoom_window=0, ps_zoom_graph=1, ps_zoom_none=3 };



extern __exportD color gw_auto_color;


//------------------------------------------------------------------------------
// node_info & edge_info - structs   
//------------------------------------------------------------------------------

struct __exportC node_info 
{
  bool          init;
  color         clr;
  char*         pixmap;
  gw_node_shape shape;
  int           orient;
  color         border_clr;
  float         border_w;
  point         pos;
  float         r1;
  float         r2;
  gw_label_type label_t;
  string        label;
  color         label_clr;
  gw_position   label_pos;  
  int           label_curs;
  bool          selected;

  node_info() : init(false) {}
 ~node_info() {}
 
  LEDA_MEMORY(node_info)
};


struct __exportC edge_info 
{
  bool          init;
  color         clr;
  gw_edge_shape shape;
  gw_edge_style style;
  gw_edge_dir   dir;
  float         width;
  gw_label_type label_t;
  string        label;
  color         label_clr;
  gw_position   label_pos;
  int           label_curs;
  list<point>   p;
  point         s_anchor;
  point         t_anchor;
  bool          selected;  
  double*       slider_f;

  edge_info() : init(false), slider_f(NULL) {}
  edge_info(const edge_info& inf);
  const edge_info& operator=(const edge_info& inf);
 ~edge_info() { if (slider_f) delete[] slider_f; }

  LEDA_MEMORY(edge_info)  
};


struct cluster_box
{

 polygon shape;

 string label;
 color  bcol;
 color  fcol;
 color  tcol;

 int         lwidth;
 line_style  lstyle;

 cluster_box() {}

 cluster_box(const polygon& pol, string l, color bc, color fc, 
                                           color tc, int lw, line_style ls)
 : shape(pol),label(l),bcol(bc),fcol(fc),tcol(tc), lwidth(lw),lstyle(ls) {}

 cluster_box(double x0, double y0, double x1, double y1, string l, 
             color bc, color fc, color tc, int lw, line_style ls)
 : label(l),bcol(bc),fcol(fc),tcol(tc), lwidth(lw),lstyle(ls) 
 { 
   if (x0 > x1) leda_swap(x0,x1);
   if (y0 > y1) leda_swap(y0,y1);

   list<point> L;
   L.append(point(x0,y0));
   L.append(point(x1,y0));
   L.append(point(x1,y1));
   L.append(point(x0,y1));
   shape = polygon(L);
  }


 friend ostream& operator<<(ostream& ostr, const cluster_box&) { return ostr; }
 friend istream& operator>>(istream& istr, cluster_box&) { return istr; }

};


//------------------------------------------------------------------------------
//  class  GraphWin                                  */
//------------------------------------------------------------------------------

/*{\Manpage {GraphWin} {} {Graph Windows}}*/

class __exportC GraphWin {

friend class __exportC hp_graphwin;

/*{\Mdefinition
 
{\em GraphWin} combines the two types |graph| and |window| and
forms a bridge between the graph data types and algorithms
and the graphics interface of LEDA.
{\em GraphWin} can easily be used in LEDA programs for
constructing, displaying and manipulating graph and for
animating and debugging graph algorithms.
 
\begin{itemize}
 
\item
The user interface of GraphWin is simple and intuitive.
When clicking a mouse button inside the drawing area
a corresponding default action is performed that can be
redefined by users. With the initial default settings,
the left mouse button is used for creating and moving objects,
the middle button for selecting objects, and the right button for
destroying objects.
A number of menues at the top of the window give access to
graph generators, modifiers, basic algorithms, embeddings, setup panels,
and file input and output.
 
\item
Graphwin can display and manipulate the data associated with the
nodes and edges of LEDA's parameterized graph type $GRAPH<vtype,etype>$.
When a GraphWin is opened for such a graph the associated node and edge
labels of type $vtype$ and $etype$ can be displayed and edited.
 
\item
Most of the actions of GraphWin can be customized by modifying or
extending the menues of the main window or by defining call-back functions.
So the user can define what happens if a node or edge is created, selected,
moved, or deleted.
 
\item
Graphwin offers a collection of graph generators, modifiers
and tests. The generators include functions for constructing random,
planar, complete, bipartite, grid graph, connected graph, biconnected,
graph \dots

There are also methods for modifying existing graph (e.g.
by removing or adding a certain set of edges) to fit in one of
these categories and for testing whether a given graph is
planar, connected, bipartite \dots

\item
The standard menu includes a choice of fundamental graph algorithms
and basic embedding algorithms.
 
\end{itemize}
 
\bigskip
For every node and edge of the graph GraphWin maintains a set of parameters.

With every node is associated the following list of parameters.
Note that for every parameter there are corresponding set and get
operations ({\tt gw.set\_param()} and {\tt gw.get\_param}) where
{\tt param} has to be replaced by the corresponding parameter name.

\begin{description}
\item {\tt position}: the position of the node (type |point|),
\item {\tt shape}: the shape of the node (type |gw_node_shape|),
\item {\tt color}: the color of the interior of the node (type |color|), 
\item {\tt border\_color}: the color of the node's border (type |color|),
\item {\tt label\_color}:  the color of the node's label (type |color|),
\item {\tt pixmap}: the pixmap used to fill the interior of the node  (|char*|),
\item {\tt width}:  the width of the node in pixels (|int|),
\item {\tt height}: the height of the node in pixels (|int|),
\item {\tt radius1}: the horizontal radius in real world coordinates (|double|)
\item {\tt radius2}: the vertical radius in real world coordinates (|double|),
\item {\tt border\_width}: the  width of the  border in pixels (|int|),
\item {\tt label\_type}: the type of the node's label (type |gw_label_type|),
\item {\tt user\_label}: the user label of the node (type |string|), and
\item {\tt label\_pos}:  the position of the label (type |gw_position|).
\end{description}
 
With every edge is associated the following list of parameters
\begin{description}
\item {\tt color}: the color of the edge (type |color|),
\item {\tt label\_color}: the color of the edge label (type |color|),
\item {\tt shape}: the shape of the edge (type |gw_edge_shape|),
\item {\tt style}: the style of the edge (type |gw_edge_style|),
\item {\tt direction}: the direction of the edge  (type |gw_edge_dir|),
\item {\tt width}:  the width of the edge in pixels (type |int|),
\item {\tt label\_type}: the label type of the edge (type |gw_label_type|),
\item {\tt user\_label}: the user label of the edge (type |string|),
\item {\tt label\_pos}: the position of the edge's label (type |gw_position|),
\item {\tt bends}: the list of edge bends  (type |list<point>|),
\item {\tt source\_anchor}: the source anchor of the edge (type |point|), and
\item {\tt target\_anchor}: the target anchor of the edge (type |point|).
\end{description}
 
The corresponding types are:
 
\begin{verbatim}
gw_node_shape = { circle_node, ellipse_node, square_node, rectangle_node }
gw_edge_shape = { poly_edge, circle_edge, bezier_edge, spline_edge }

gw_position = { central_pos, northwest_pos, north_pos,
                northeast_pos, east_pos, southeast_pos, 
                south_pos, southwest_pos, west_pos }

gw_label_type = { no_label, user_label, data_label, index_label }

gw_edge_style = { solid_edge, dashed_edge, dotted_edge, dashed_dotted_edge }
gw_edge_dir   = { undirected_edge, directed_edge, bidirected_edge, rdirected_edge };
\end{verbatim}

}*/

  private :

    friend class __exportC gw_observer;
    friend class __exportC graphwin_undo;

    list<cluster_box> cluster_list;
    const list<cluster_box>& get_clusters() { return cluster_list; }
    void draw_clusters();

    graphwin_undo *gw_undo;

    void internal_del_node(node v);
    void internal_del_edge(edge e);
  
    graph  *gr_p;			// graph
    window *win_p;			// window
    window *menu_p;			// menu window
    window *msg_win;                    // message window
    
    node_map<node_info> n_info;
    edge_map<edge_info> e_info;

    node_map<node_info>* save_n_info;
    edge_map<edge_info>* save_e_info;

    map<node,motion_base*> node_motion;
    motion_base* node_default_motion;
    
    bool edges_embedded;
    bool adjust_to_label;

    bool ortho_mode;
    int  ortho_first_dir;

    long  property_mask;
    long  status_show_mask;

    bool  node_setup_active;
    bool  edge_setup_active;
    
    double xmin_def,xmax_def,ymin_def; // default window coords
    double x_min,x_max,y_min,y_max;    // current window coords

    gw_font_type node_label_font_type;
    double       node_label_font_size;
    string       node_label_user_font;

    gw_font_type edge_label_font_type;
    double       edge_label_font_size;
    string       edge_label_user_font;
    
    bool labelBoxEnabled;
    
    long menu_mask;

    node_move_t node_move;

    string  node_index_format;
    string  edge_index_format;

    bool edge_border;

    node_info n_model;
    edge_info e_model;

    list<node> selected_nodes;
    list<edge> selected_edges;

    list<node> bg_redraw_node_list;
    
    double edge_distance;
    
    
    long create_flag;

    int done_button;
    int undo_button;
    int redo_button;
    int exit_button;

    int max_move_items;

    string gw_dirname;
    string gw_filename;

    string graph_name;

    string print_cmd;

// ps options

    double       ps_factor;
    string       ps_fontname;
    list<string> ps_fontlist;

// svg options

    double svg_edge_transparence; 
    bool   svg_whole_graph; 



    list<string> help_list;

    string message_str;

//  agd client

    leda_socket sock;

    string agd_host;
    int    agd_port;
    bool   agd_option_diag;
    int    agd_input_format;
    int    agd_output_format;

    bool socket_connect();
    void socket_disconnect();


    int window_width();
    int window_height();

    void init_once();

    void init(graph*,window*);
    void init(graph*,int win_w, int win_h, string win_label);

    void init_default();
    void init_node(node v);
    void init_edge(edge e); 
    void init_matrix();
    void init_window(int=MAXINT,int=MAXINT);
    void init_menu();

    void init_agd_menu(int menu_id, bool silent);
    void init_gravis_menu(int menu_id);

    void init_static();
    void init_graph();
    
    node found_node;
    node found_frame;
    edge found_edge;
    int  found_slider;

    node current_node;
    edge current_edge;


    list<node> n_anim_list;
    char*      n_anim_buf;
    bool       n_anim_flush;

    list<edge> e_anim_list;
    char*      e_anim_buf;
    bool       e_anim_flush;

    // bounding box
    double     anim_x0;
    double     anim_y0;
    double     anim_x1;
    double     anim_y1;

void compute_bounding_box(node, double&, double&, double&, double&);
void compute_bounding_box(edge, double&, double&, double&, double&);
void adjust_bounding_box(node, double&, double&, double&, double&);
void adjust_bounding_box(edge, double&, double&, double&, double&);


void flush_node(node);
void flush_edge(edge);

void flip_y();

void get_extreme_coords(double& x0, double& y0, double& x1, double& y1);

void get_extreme_coords(double& x0, double& y0, double& x1, double& y1, 
                                                double& r1, double& r2);

list<point>& get_poly(edge e);

void set_gwin(window& w) { w.grawin_ptr = this; }

//------------------------------------------------------------------------------

    string (*coord_to_string_handler)(double, double);

static void s_redraw(window*,double,double,double,double);
static void coord_handler(window*,double,double);

    void update_status_line();

    void update_win_bounds();

    int edit_result;

    unsigned animation_steps;

    bool flush;			// make changes visible
    bool flush0;

    bool zoom_objects;
    bool zoom_labels;
    bool zoom_hide_labels;
    bool zoom_show_grid;

    int buttons_per_line;

    int  win_width;

    grid_style g_style;
    int  grid_dist;

    bool show_status;
    bool auto_create_target;
    bool d3_look;

    color bg_color;
    char* bg_pixmap;
    string bg_pmname;

    double zoom_factor;
    double zoom_x0;
    double zoom_x1;
    double zoom_y0;
    double zoom_y1;

    bool hide_labels;

    int gen_nodes;
    int gen_edges;
    string gen_layout;

    bool win_init_done;

    double max_edge_distance(node v, node w);
    point  compute_leaving_point(node v, point vp, point p);
    point  compute_leaving_circle_point(node v, point vp, point p, point q);
    double compute_ortho_leaving_points(node v,node w,point& pv,point& pw);
    point  first_hit(node v, const line& l, bool from_left, bool from_above);

    segment edge_segment(node v, point pv, node w, point pw);



//------------------------------------------------------------------------------
// dynamic adjacency matrix 
//------------------------------------------------------------------------------

   node_map2<edge> adj_matrix_first;
   edge_map<edge>  adj_matrix_succ;

#define forall_inc_edges(e,v,w)\
   for(e=adj_matrix_first(v,w); e; e = adj_matrix_succ[e])


//------------------------------------------------------------------------------
// handlers - called at certain events
//------------------------------------------------------------------------------

    bool (*pre_new_node_handler)(GraphWin& gw, const point& p);
    void (*post_new_node_handler)(GraphWin& gw, node v);

    bool (*pre_new_edge_handler)(GraphWin& gw, node s, node t);
    void (*post_new_edge_handler)(GraphWin& gw, edge e);
    
    bool (*pre_move_node_handler)(GraphWin& gw, node v, const point& p);
    void (*post_move_node_handler)(GraphWin& gw, node v);

    bool (*pre_start_move_node_handler)(GraphWin& gw, node v);
    void (*post_start_move_node_handler)(GraphWin& gw, node v);
    
    bool (*pre_end_move_node_handler)(GraphWin& gw, node v, const point& p);
    void (*post_end_move_node_handler)(GraphWin& gw, node v);

    bool (*pre_del_node_handler)(GraphWin& gw, node v);
    void (*post_del_node_handler)(GraphWin& gw); 

    bool (*pre_del_edge_handler)(GraphWin& gw, edge e);
    void (*post_del_edge_handler)(GraphWin& gw);

    bool (*pre_split_edge_handler)(GraphWin& gw, edge e);
    void (*post_split_edge_handler)(GraphWin& gw, node u);

    bool (*pre_move_component_handler)(GraphWin& gw, node v);
    void (*post_move_component_handler)(GraphWin& gw, node v);

    bool (*pre_init_graph_handler)(GraphWin& gw);
    void (*post_init_graph_handler)(GraphWin& gw);

//------------------------------------------------------------------------------

	window_handler* bg_redraw_obj;
	window_handler* bg_node_redraw_obj;
    void (*bg_redraw_func)(window*,double,double,double,double);
    void (*bg_node_redraw_func)(window*,double,double,double,double);

//------------------------------------------------------------------------------

   int  num_sliders;

   void (*edge_slider_handler[MAX_SLIDERS])(GraphWin&, edge, double);
   void (*start_edge_slider_handler[MAX_SLIDERS])(GraphWin&w, edge, double);
   void (*end_edge_slider_handler[MAX_SLIDERS])(GraphWin&, edge, double);

   color slider_color[MAX_SLIDERS];



//------------------------------------------------------------------------------
// menus: representation and default-menus
//------------------------------------------------------------------------------

   gw_base_function* menu_functions[MAX_ITEM_NUM];
   window*           sub_menus[MAX_ITEM_NUM];

   int call_entry_counter;
   int sub_menu_counter;
   
   void call_menu_func(int);



//------------------------------------------------------------------------------
//  table of edit actions
//------------------------------------------------------------------------------

   gw_action action[A_STOP];
   long      action_mask;


   list<two_tuple<string,gw_action> > node_action_menu;
   list<two_tuple<string,gw_action> > edge_action_menu;


// private copy constructor

   GraphWin(const GraphWin&) {} 


//------------------------------------------------------------------------------
//  Drawing & Embedding
//------------------------------------------------------------------------------


void clear_node_box(node);
void split_label(node,edge,string,list<string>&,
                 double&,double&,double&,double&,double&);
void draw_obj_label(node,edge,string,double,double);
void ps_draw_obj_label(PostScript&,node,edge,string);

void draw_node(node v);
void redraw_node(node v);
void draw_node(node v, double dx, double dy);
void draw_node_with_edges(node v);
void draw_node_with_edges(node v, double dx, double dy);

point compute_label_pos(edge,double,double);
point compute_label_pos(node,double,double);

int   text_color(int);

void draw_edge(edge e);
void redraw_edge(edge e);
void draw_edge(edge e, double dx, double dy);
void draw_edges(node v, node w);
void draw_edges(node v, node w, double dx, double dy);

void draw_message();

void draw_graph(double x0, double y0, double x1, double y1, 
                                      double dx=0, double dy=0);

void draw_graph(node v, edge e, double x0, double y0, double x1, double y1, 
                                              double dx=0, double dy=0);
public:
void draw_graph(double dx=0, double dy=0);

private:
void draw_graph(node v, double dx=0, double dy=0);
void draw_graph(edge e, double dx=0, double dy=0); 
void draw_graph(const list<node>& L, double dx=0, double dy=0); 
void draw_graph(const list<edge>& L, double dx=0, double dy=0);

void redraw(node);
void redraw(edge);

void embed_node_with_edges(node v);
void embed_edges(node v, node w);

void move_node(node v, const vector& trans);

void move_edge(edge e, const vector& trans);
void move_edge(edge e, const list<vector>& trans); 

void move_nodes_with_edges(const list<node>& L, const vector& trans);

void move_node_with_edges(node v, const point& dest, unsigned anim=0);


void sort_edges_by_angle();

void normalize_polygon(list<point>& P, double min_dist = 0.00001);

void set_edge_points(edge e, const list<point>& P);

void window_init(double xmin, double xmax, double ymin, double f=0);

void extract_labels(ostream&, string psname, string pspath);

void print_ps(ostream&, bool tex=false);

void print_svg(ostream&);

void load_finish(double,double,double,double);

const list<string>& get_ps_fontlist() const { return ps_fontlist; }

string get_index_label(node v) const;
string get_index_label(edge e) const;

void set_font(gw_font_type font_t, double size, string user_fn);

point snap_point(const point&);


string  xpm_path(string);

    
bool check_start_move_node_handler(node v);
void call_start_move_node_handler(node v);

bool check_end_move_node_handler(node v, const point& p);
void call_end_move_node_handler(node v);

bool check_new_node_handler(const point& p);
void call_new_node_handler(node v);
    
bool check_new_edge_handler(node s, node t);
void call_new_edge_handler(edge e);
        

bool has_move_node_handler() { return post_move_node_handler != 0; }
bool check_move_node_handler(node v, const point& p);
void call_move_node_handler(node v);
    
bool check_del_node_handler(node v);
void call_del_node_handler();
    
bool check_del_edge_handler(edge e);
void call_del_edge_handler();
    
bool check_split_edge_handler(edge e);
void call_split_edge_handler(node u);
    
bool check_move_component_handler(node v);
void call_move_component_handler(node v);
    


public:

bool check_init_graph_handler();
void call_init_graph_handler();

void handle_edit_event(int but, double x, double y);

void edit_loop(int event, window*& wp, int& but, double& x, double& y);

int  get_edit_result() const { return edit_result; }
void reset_edit_result() { edit_result = -1; }


static double version();
static string version_str();

unsigned long read_mouse_rect(double&,double&,double&,double&);

float  width2radius(int w) const { return float(win_p->pix_to_real(w)*0.5); }
float  height2radius(int h) const { return width2radius(h); }
int    radius2width(double r) const { return 2*win_p->real_to_pix(r); }
int    radius2height(double r) const { return radius2width(r); }


void embed_edges();

void n_animation_start(const list<node>&, bool=false);
void n_animation_start(node, bool=false);
void e_animation_start(const list<edge>&);
void e_animation_start(edge);

const list<node>& get_n_animation_list() { return n_anim_list; }
const list<edge>& get_e_animation_list() { return e_anim_list; }

void n_animation_end();
void e_animation_end();

void n_animation_step(bool=false);
void e_animation_step();


//------------------------------------------------------------------------------
//                          EVENTS
//------------------------------------------------------------------------------


EVENT3<GraphWin&,const list<node>&, const list<edge>&> changing_layout_event;
EVENT3<GraphWin&,const list<node>&, const list<edge>&> change_layout_event;

  
//------------------------------------------------------------------------------
//                          CONSTRUCTORS
//------------------------------------------------------------------------------

/*{\Mtext \def\gw{\mbox{$gw$}} }*/
/*{\Mtext \def\graphwin{\mbox{Graph\hspace{-0.1em}Win}} }*/


/*{\Mcreation gw }*/

GraphWin(graph& G, int w, int h, const char* win_label=""); 
/*{\Mcreate creates a graph window for graph $G$ with a display window 
            of size $w$ pixels $\times\ h$ pixels. If |win_label| is not
            empty it is used as the frame label of the window, otherwise,
            a default frame label is used. }*/


GraphWin(graph& G, const char* win_label=""); 
/*{\Mcreate creates a graph window for graph $G$ with a display window
            of default size and frame label |win_label|. }*/


GraphWin(int w, int h, const char* win_label=""); 
/*{\Mcreate creates a graph window for a new empty graph with a display
            window of size $w$ pixels $\times\ h$ pixels, and
            frame label |win_label|. }*/


GraphWin(const char* win_label=""); 
/*{\Mcreate creates a graph window for a new empty graph with a
            display window of default size and frame label |win_label|. }*/


GraphWin (window& W);
/*{\Mcreate as above, but |W| is used as display window. }*/  

GraphWin (graph& G, window& W);
/*{\Mcreate as above, but makes |G| the graph of \gw. }*/  



    
~GraphWin();
    

/*{\Moperations 2.5 4.6}*/
    
//------------------------------------------------------------------------------
//                            Window Operations
//------------------------------------------------------------------------------
    
/*{\Mtext
\medskip
{\bf a) Window Operations} }*/

void display(int x, int y);
/*{\Mop displays \Mvar with upper left corner at $(x,y)$. The predefined
        constant |window::center| can be used to center the window
        horizontally (if passed as |x|) or vertically (if passed as |y|). }*/

void display();
/*{\Mop displays \Mvar at default position. }*/


bool edit();
/*{\Mop enters the edit mode of |GraphWin| that allows to change the
        graph interactively by operations associated with certain mouse 
        events or by choosing operations from the windows menu bar (see section
        about edit-mode) for a description of the available commands and
        operations). Edit mode is terminated by either pressing the {\em done}
        button or by selecting {\em exit} from the file menu. In the first case
        the result of the edit operation is |true| and in the latter case the 
        result is |false|. }*/
    
bool open(int x, int y);
/*{\Mop  displays the window at position $(x,y)$, enters edit mode and return
         the corresponding result. }*/

bool open();
/*{\Mop as above, but displays the window at default position. }*/

void close();
/*{\Mop closes the window. }*/

void message(const char* msg);
/*{\Mop displays the message |msg| at the top of the window. }*/

string get_message() { return message_str; }
/*{\Mop returns the current messsage string. }*/
    
void del_messages() { message(""); }

void del_message() { message(""); }
/*{\Mop deletes a previously written message. }*/

double get_xmin() const;
/*{\Mopl  returns the minimal x-coordinate of the window. }*/

double get_ymin() const;
/*{\Mopl  returns the minimal y-coordinate of the window. }*/

double get_xmax() const;
/*{\Mopl  returns the maximal x-coordinate of the window. }*/

double get_ymax() const;
/*{\Mopl  returns the maximal y-coordinate of the window. }*/

void win_init(double xmin, double xmax, double ymin)
{ bool b = set_zoom_objects(false);
  window_init(xmin,xmax,ymin); 
  set_zoom_objects(b);
}
/*{\Mopl sets the coordinates of the window to $(xmin,xmax,ymin)$. }*/


void get_default_win_coords(double& xmin, double& xmax, double& ymin);
void set_default_win_coords(double xmin, double xmax, double ymin);


void redraw(double x0, double y0, double x1, double y1);			

void redraw();			
/*{\Mop	  redraws the graph. If the |flush| parameter of |\Mvar| is set
          to false (see |set_flush|) this operation can be used to
          display the current state of the graph after a number of
          update operations. }*/

void set_frame_label(const char* label) { menu_p->set_frame_label(label); }
/*{\Mopl   makes |label| the frame label of the window. }*/


int open_panel(panel& P);
/*{\Mop   displays panel |P| centered on the drawing area of |\Mvar|,
          disables the menu bar of |\Mvar| and returns the result
          of |P.open()|. }*/


window& get_window() const { return *win_p; }
/*{\Mop    returns a reference to the window of |\Mvar|. }*/


void finish_menu_bar()
{ init_menu();
  menu_p->make_menu_bar(2);
}
/*{\Mop  this operation has to called before additional buttons
         are added to the panel section of |gw.get_window()|.
}*/


  
//------------------------------------------------------------------------------
//                         Graph Operations
//------------------------------------------------------------------------------


/*{\Mtext
\medskip       
{\bf b) Graph Operations} }*/

node new_node(const point& p);
/*{\Mopl  adds a new node at position $p$ to \gw. }*/

void del_node(node v);
/*{\Mop   deletes $v$ and all edges incident to $v$ from \gw. }*/

edge new_edge(node v, node w);
/*{\Mopl  adds a new edge $(v,w)$ to \gw. }*/

edge new_edge(node v, node w, const list<point>& P);
/*{\Mopl  adds a new edge $(v,w)$ with bend sequence $P$ to \gw. }*/

                
void del_edge(edge e);
/*{\Mop   deletes edge $e$ from \gw. }*/

void clear_graph();
/*{\Mop  deletes all nodes and egdes. }*/


graph& get_graph() const { return *gr_p; }
/*{\Mop   returns a reference of the graph of |\Mvar|. }*/


int  test_update_nodes();
int  test_update_edges();
bool test_update_graph();
void update_nodes();
void update_edges();

void update_graph();
/*{\Mopl  this operation has to be called after any update operation
          that has been performed directly (not by \graphwin) on the 
          underlying graph, e.g., deleting or inserting nodes or edges. }*/


/*{\Mtext
\medskip
{\bf c) Node Parameters } 

Node parameters can be retrieved or changed by a collection of |get|-
and |set|- operations.  We use |param_type| for the type and |param|
for the value of the corresponding parameter. }*/


/*{\Mtext
\bigskip
{Individual Parameters}
}*/

/*{\Moptions nextwarning=no}*/
/*
param_type  get_param(node v);
*/
/*{\Mopl  returns the value of parameter |param| for node |v|. }*/


/*{\Moptions nextwarning=no}*/
/*
param_type  set_param(node v, param_type x);
*/
/*{\Mopl  sets the value of parameter |param| for node |v| to |x|.
          and returns its previous value.}*/


/*{\Moptions nextwarning=no}*/
/*
void  set_param(list<node>& L, param_type x);
*/
/*{\Mopl  sets the value of parameter |param| for all nodes in |L| to |x|. }*/



/*{\Mtext
\medskip
{Default Parameters}
}*/

/*{\Moptions nextwarning=no}*/
/*
param_type  get_node_param();
*/
/*{\Mopl  returns the current default value of parameter |param|. }*/

/*{\Moptions nextwarning=no}*/
/*
param_type  set_node_param(param_type x, bool apply=true);
*/
/*{\Mopl  sets the default value of parameter |param| to |x|. 
          and returns its previous value. If |apply == true|
          the parameter is changed for all existing nodes as well. }*/



/*{\Mtext
\medskip
{\bf d) Edge Parameters } 
}*/

/*{\Mtext
\smallskip
{Individual Parameters}
}*/

/*{\Moptions nextwarning=no}*/
/*
param_type  get_param(edge e);
*/
/*{\Mopl  returns the value of parameter |param| for edge |e|. }*/

/*{\Moptions nextwarning=no}*/
/*
param_type  set_param(edge e, param_type x);
*/
/*{\Mopl  sets the value of parameter |param| for edge |e| to |x|.
          and returns its previous value.}*/

/*{\Moptions nextwarning=no}*/
/*
void  set_param(list<edge>& L, param_type x);
*/
/*{\Mopl  sets the value of parameter |param| for all edges in |L| to |x|. }*/


/*{\Mtext
\smallskip
{Default Parameters}
}*/

/*{\Moptions nextwarning=no}*/
/*
param_type  get_edge_param();
*/
/*{\Mopl  returns the current default value of parameter |param|. }*/

/*{\Moptions nextwarning=no}*/
/*
param_type  set_edge_param(param_type x, bool apply=true);
*/
/*{\Mopl  sets the default value of parameter |param| to |x|. 
          and returns its previous value. If |apply == true|
          the parameter is changed for all existing edges as well. }*/


//------------------------------------------------------------------------------
//  Attributes of existing nodes
//------------------------------------------------------------------------------

string get_data(node v) const;
string set_data(node v, string data);

const node_info& get_info(node v) const;

void set_info(const list <node>& L, const node_info& n, long mask=N_ALL);
void set_info(node v, const node_info& n, long mask=N_ALL);

int  get_index(node v) const { return gr_p->index(v); }

point get_position(node v) const;
point set_position(node v, const point& p);
void  set_position(const list<node>& L, const point& p);


color get_color(node v) const;
color set_color(node v, color c);
void  set_color(const list <node>& L, color c);

char* get_pixmap(node v) const;
char* set_pixmap(node v, char* pmap);
void  set_pixmap(const list <node>& L, char* pmap);


gw_node_shape get_shape(node v) const;
gw_node_shape set_shape(node v, gw_node_shape t);
void          set_shape(const list <node>& L, gw_node_shape t);


color get_border_color(node v) const;
color set_border_color(node v, color c);
void  set_border_color(const list <node>& L, color c);

double get_border_thickness(node v) const;
double set_border_thickness(node v, double t);
void   set_border_thickness(const list <node>& L, double t);

int  get_border_width(node v) const;
int  set_border_width(node v, int w);
void set_border_width(const list <node>& L, int w);


double get_radius1(node v) const;
double set_radius1(node v, double r);
void   set_radius1(const list <node>& L, double r);

double get_radius(node v) const { return get_radius1(v); }
double set_radius(node v, double r) { return set_radius1(v,r); }
void   set_radius(const list <node>& L, double r) { set_radius1(L,r); }
    
double get_radius2(node v) const;
double set_radius2(node v, double r);
void   set_radius2(const list <node>& L, double r);
    
int  get_width(node v) const;
void set_width(const list <node>& L, int w);
int  set_width(node v, int w);

int  get_height(node v) const;
void set_height(const list <node>& L, int h);
int  set_height(node v, int h);


string get_user_label(node v) const;
void   set_user_label(const list<node>& L, string s);
string set_user_label(node v, string s);

void adjust_node_to_label(node v);

gw_label_type get_label_type(node v) const;
void          set_label_type(const list<node>& L, gw_label_type lt);
gw_label_type set_label_type(node v, gw_label_type lt);



string get_label(node v) const;
void   set_label(const list<node>& L, string s);
string set_label(node v, string s, bool adjust=false);


gw_position get_label_pos(node v) const;
void        set_label_pos(const list<node>& L, gw_position lp);
gw_position set_label_pos(node v, gw_position lp);

color get_label_color(node v) const;
void  set_label_color(const list<node>& L, color c);
color set_label_color(node v, color c);

bool get_select(node v) const;
void set_select(const list<node>& L, bool b);
bool set_select(node v, bool b, bool rd=true);

    
    
//------------------------------------------------------------------------------
//  Attributes of existing edges
//------------------------------------------------------------------------------

string get_data(edge e) const;
string set_data(edge e, string data);

const edge_info& get_info(edge e) const;

void set_info(const list <edge>& L, const edge_info& i, long mask=E_ALL);
void set_info(edge e, const edge_info& i, long mask=E_ALL);

list<point> get_bends(edge e) const;
void        set_bends(edge e, const list<point>& P, double min_dist=0.00001);

list<point> get_polygon(edge e) const { return get_bends(e); }
void        set_polygon(edge e, const list<point>& P) { set_bends(e,P); }


const list<point>& get_edge_points(edge e) const;


color get_color(edge e) const;
color set_color(edge e, color c);
void  set_color(const list <edge>& L, color c);

gw_edge_shape get_shape(edge e) const;
gw_edge_shape set_shape(edge e, gw_edge_shape t);
void          set_shape(const list <edge>& L, gw_edge_shape t);

gw_edge_style get_style(edge e) const;
gw_edge_style set_style(edge e, gw_edge_style st);
void          set_style(const list <edge>& L, gw_edge_style st);

gw_edge_dir get_direction(edge e) const;
gw_edge_dir set_direction(edge e, gw_edge_dir dir);
void        set_direction(const list <edge>& L, gw_edge_dir dir);

int  get_width(edge e) const;
int  set_width(edge e, int w);
void set_width(const list<edge>& L, int w);

double  get_thickness(edge e) const;
double  set_thickness(edge e, double w);
void    set_thickness(const list<edge>& L, double w);

string get_user_label(edge e) const;
string set_user_label(edge e, string s);
void   set_user_label(const list<edge>& L, string s);

string get_label(edge e) const;
string set_label(edge e, string s);
void   set_label(const list<edge>& L, string s);

gw_label_type get_label_type(edge e) const;
gw_label_type set_label_type(edge e, gw_label_type lt);
void          set_label_type(const list<edge>& L, gw_label_type lt);


gw_position get_label_pos(edge e) const;
gw_position set_label_pos(edge e, gw_position lp);
void        set_label_pos(const list<edge>& L, gw_position lp);

color get_label_color(edge e) const;
color set_label_color(edge e, color c);
void  set_label_color(const list<edge>& L, color c);

point get_source_anchor(edge e) const;
point set_source_anchor(edge e, point p);
void  set_source_anchor(const list<edge>& L, point p);

point get_target_anchor(edge e) const;
point set_target_anchor(edge e, point p);
void  set_target_anchor(const list<edge>& L, point p);


point get_source_anchor_pos(edge e) const;
point set_source_anchor_pos(edge e, point p);

point get_target_anchor_pos(edge e) const;
point set_target_anchor_pos(edge e, point p);



bool get_select(edge e) const;
void set_select(const list<edge>& L, bool b);
bool set_select(edge e, bool b, bool rd=true);


double get_slider_value(edge e, int i=0) const;
double set_slider_value(edge e, double f, int i=0);

    
//------------------------------------------------------------------------------
//  Default Attributes of nodes
//------------------------------------------------------------------------------


const node_info& get_node_info() const;
void             set_node_info(const node_info& n, long mask=N_ALL);

color get_node_color() const;
color set_node_color(color c,bool=true);

char* get_node_pixmap() const;
char* set_node_pixmap(char* pmap, bool=true);

gw_node_shape get_node_shape() const;
gw_node_shape set_node_shape(gw_node_shape shape,bool=true);

color get_node_border_color() const;
color set_node_border_color(color c,bool=true);

int get_node_border_width() const;
int set_node_border_width(int w,bool=true);

double get_node_border_thickness() const;	
double set_node_border_thickness(double t,bool=true);		

double get_node_radius1() const;
double set_node_radius1(double r,bool=true);		

double get_node_radius() const { return get_node_radius1(); }
double set_node_radius(double r,bool b=true) { return set_node_radius1(r,b); }

double get_node_radius2() const;
double set_node_radius2(double r,bool=true);		

int get_node_width() const;
int set_node_width(int w,bool=true);

int get_node_height() const;
int set_node_height(int h,bool=true);

gw_label_type get_node_label_type() const;
gw_label_type set_node_label_type(gw_label_type lt,bool=true);


gw_position get_node_label_pos() const;
gw_position set_node_label_pos(gw_position lp,bool=true);

color get_node_label_color() const;
color set_node_label_color(color c,bool=true);



//------------------------------------------------------------------------------
//  Default Attributes of edges
//------------------------------------------------------------------------------

const edge_info& get_edge_info() const;
void             set_edge_info(const edge_info& i, long mask=E_ALL);

color get_edge_color() const;
color set_edge_color(color c,bool=true);

gw_edge_shape get_edge_shape() const;
gw_edge_shape set_edge_shape(gw_edge_shape shape,bool=true);

gw_edge_style get_edge_style() const;
gw_edge_style set_edge_style(gw_edge_style st,bool=true);

gw_edge_dir get_edge_direction() const;
gw_edge_dir set_edge_direction(gw_edge_dir dir,bool=true);

int get_edge_width() const;	
int set_edge_width(int w,bool=true);		

double get_edge_thickness() const;	
double set_edge_thickness(double w,bool=true);		

gw_label_type get_edge_label_type() const;
gw_label_type set_edge_label_type(gw_label_type lt,bool=true);

gw_position get_edge_label_pos() const;
gw_position set_edge_label_pos(gw_position lp,bool=true);

color get_edge_label_color() const;
color set_edge_label_color(color c,bool=true);




//------------------------------------------------------------------------------
//  Global Attributes and Options
//------------------------------------------------------------------------------

/*{\Mtext
\medskip
{\bf e) Global Options} }*/


int get_gen_nodes() const;

int set_gen_nodes(int n);
/*{\Mopl sets the default number of nodes |n| for all graph generator
         dialog panels. }*/
    
int get_gen_edges() const;

int set_gen_edges(int m);
/*{\Mopl sets the default number of edges |m| for all graph generator
         dialog panels. }*/

    
int get_edge_distance() const;

int set_edge_distance(int d);
/*{\Mopl sets the distance of multi-edges to $d$ pixels. }*/


void set_edge_slider_color(color c, int i=0);

point grid_point(double,double);



grid_style get_grid_style() const;

grid_style set_grid_style(grid_style s);
/*{\Mopl  sets the grid style to $s$. }*/


int get_grid_dist() const;

int set_grid_dist(int d);
/*{\Mopl  sets the grid distance to $d$. }*/


bool get_grid_axees() const;
bool set_grid_axees(bool);

// for backward comp.
int set_grid_mode(int d);


int get_grid_size() const;

int set_grid_size(int n);
/*{\Mopl  sets the grid distance such that $n$ vertical grid lines
          lie inside the drawin area. }*/


void snap_to_grid();

bool get_auto_create_target() const;
bool set_auto_create_target(bool);


bool get_ortho_mode() const;
bool set_ortho_mode(bool);


int  get_ortho_first_dir() const { return ortho_first_dir; }
int  set_ortho_first_dir(int d)
{ int d_old = ortho_first_dir;
  ortho_first_dir = d;
  return d_old;
}

bool get_adjust_to_label() const { return adjust_to_label; }

bool set_adjust_to_label(bool b)
{ bool b_old = adjust_to_label; 
  adjust_to_label = b;
  return b_old;
}

bool get_d3_look() const;
bool set_d3_look(bool);

bool get_show_status() const;

bool set_show_status(bool b);
/*{\Mopl  display a status window ($b$={\it true}) or not ($b$={\it false}). }*/

color get_bg_color();

color set_bg_color(color c);
/*{\Mopl  sets the window background color to $c$. }*/

char* get_bg_pixmap();

char* set_bg_pixmap(char* pr, double xorig=0, double yorig=0);
/*{\Mopl  sets the window background pixmap to $pr$ and the tiling 
          origin to $(xorig,yorig)$. }*/

void set_bg_xpm(const char** xpm_data);
/*{\Mopl  sets the window background pixmap to the pixmap defined
          by |xpm_data|. }*/

void set_bg_redraw(void (*f)(window*,double,double,double,double))
{ bg_redraw_func = f; bg_redraw_obj = 0; }
/*{\Mopl  sets the window background redraw function to |f|. }*/

void set_coord_to_string_handler(string (*f)(double,double))
{ coord_to_string_handler = f; }

void set_bg_redraw(window_handler& obj)
{ bg_redraw_func = 0; bg_redraw_obj = &obj; }

win_redraw_func get_bg_redraw_func() const { return bg_redraw_func; }
window_handler* get_bg_redraw_obj() const  { return bg_redraw_obj; }

void set_bg_node_redraw(void (*f)(window*,double,double,double,double))
{ bg_node_redraw_func = f; bg_node_redraw_obj = 0; }

void set_bg_node_redraw(window_handler& obj)
{ bg_node_redraw_func = 0; bg_node_redraw_obj = &obj; }

win_redraw_func get_bg_node_redraw_func() const { return bg_node_redraw_func; }
window_handler* get_bg_node_redraw_obj() const  { return bg_node_redraw_obj; }

void add_redraw_list(node v) { bg_redraw_node_list.append(v); }
void clear_redraw_list()     { bg_redraw_node_list.clear();   }


string get_bg_pmname()            { return bg_pmname; }
void   set_bg_pmname(string name) { bg_pmname = name; }

void get_node_label_font(gw_font_type& t, int& sz) const;


void set_node_label_font(gw_font_type t, int sz);
/*{\Mopl sets the node label font type and size. Possible types are
         |roman_font|, |bold_font|, |italic_font|, and |fixed_font|. }*/

void set_node_label_font(string fn) 
{ node_label_font_type = user_font; node_label_user_font = fn; }
/*{\Mopl sets the node label font to the font with name |fn|. }*/ 


void get_edge_label_font(gw_font_type& t, int& sz) const;

void set_edge_label_font(gw_font_type t, int sz);
/*{\Mopl sets the edge label font type and size.
         |roman_font|, |bold_font|, |italic_font|, and |fixed_font|. }*/

void set_edge_label_font(string fn)
{ edge_label_font_type = user_font; edge_label_user_font = fn; }
/*{\Mopl sets the edge label font to the font with name |fn|. }*/ 



string get_node_index_format() const { return node_index_format; }

string set_node_index_format(string s);
/*{\Mopl  sets the node index format string to |s|. }*/


string get_edge_index_format() const { return edge_index_format; }

bool get_edge_border() const { return edge_border; }

string set_edge_index_format(string s);
/*{\Mopl  sets the edge index format string |s|. }*/

bool set_edge_border(bool b);
/*{\Mopl  sets the edge border flag to |b|. }*/

bool enable_label_box(bool b);
/*{\Mopl enables/disables drawing of blue label boxes. Label boxes are
    enabled per default.}*/


/*{\Mtext
\bigskip
{Animation and Zooming}
}*/

int get_animation_steps() const;

int set_animation_steps(int s);
/*{\Mopl  move a node in $s$ steps to its new position. }*/


node_move_t get_animation_mode() const;
node_move_t set_animation_mode(node_move_t nm);


bool get_flush() const { return flush; }

bool set_flush(bool b);
/*{\Mopl  show operations on $gw$ instantly ($b$={\it true}) 
               or not ($b$={\it false}). }*/


int get_max_move_items() const;
int set_max_move_items(int);


double get_zoom_factor() const;

double set_zoom_factor(double f);
/*{\Mopl  sets the zoom factor to f used when zooming from menu. }*/


bool get_zoom_objects() const;

bool set_zoom_objects(bool b); 
/*{\Mopl  resize nodes and edges when zooming (|b==true|) or not (|b==false|). 
  }*/

bool get_zoom_labels() const;

bool set_zoom_labels(bool b); 
/*{\Mopl  resize labels when zooming (|b==true|) or not (|b==false|).   }*/


bool get_zoom_hide_labels() const;
bool set_zoom_hide_labels(bool b); 



/*{\Mtext
\medskip
{\bf  f) Node and Edge Selections} }*/

void select(node v) { set_select(v,true); }
/*{\Mopl  adds $v$ to the list of selected nodes. }*/

void select_all_nodes();
/*{\Mop selects all nodes. }*/

void deselect(node v) { set_select(v,false); }    
/*{\Mopl  deletes $v$ from the list of selected nodes. }*/

void deselect_all_nodes();
/*{\Mop clears the current node selection. }*/

bool is_selected(node v) const { return get_select(v); }
/*{\Mopl  returns |true| if |v| is selected and |false| otherwise. }*/

const list<node>& get_selected_nodes() const { return selected_nodes; }
/*{\Mop returns the current node selection. }*/


void select(edge e) { set_select(e,true); }
/*{\Mopl  adds $e$ to the list of selected edges. }*/

void select_all_edges();
/*{\Mop selects all edges. }*/

void deselect(edge e) { set_select(e,false); }    
/*{\Mopl  deletes $e$ from the list of selected edges. }*/

void deselect_all_edges();
/*{\Mop clears the current node selection. }*/

bool is_selected(edge e) const { return get_select(e); }
/*{\Mopl  returns |true| if |e| is selected and |false| otherwise. }*/

const list<edge>& get_selected_edges() const { return selected_edges; }
/*{\Mop returns the current edge selection. }*/


void deselect_all() { deselect_all_nodes(); deselect_all_edges(); }
/*{\Mop clears node and edge selections. }*/



//------------------------------------------------------------------------------
//  Layout Operations
//------------------------------------------------------------------------------

/*{\Mtext
\medskip
{\bf g) Layout Operations} }*/

void set_position(const node_array<point>& pos);
/*{\Mopl  for every node $v$ of $G$ the position of $v$ is set 
          to $pos[v]$. }*/

void set_position(const node_array<double>& x, const node_array<double>& y);
/*{\Mopl  for every node $v$ of $G$ the position of $v$ is set 
          to $(x[v],y[v])$. }*/

void get_position(node_array<point>& pos);
/*{\Mopl  for every node $v$ of $G$ the position of $v$ is assigned to
          $pos[v]$. }*/


node_array<point>* move_nodes_dest;
node_array<vector>* move_nodes_trans;


unsigned move_nodes_current_step;

void move_nodes_begin();
void move_nodes_set_position(node v, point p) { (*move_nodes_dest)[v] = p; }
int  move_nodes_end();
void move_nodes_step();
void move_nodes_finish();

void move_nodes(const node_array<point>& dest);

void set_node_motion(motion_base* m)    { node_default_motion = m; }
void set_motion(node v, motion_base* m) { node_motion[v] = m; }


void set_layout_start(const node_array<point>& n_dest, 
                      const node_array<double>& n_rad1, 
                      const node_array<double>& n_rad2, 
                      const edge_array<list<point> >& e_dest, 
                      const edge_array<point>& e_sanch, 
                      const edge_array<point>& e_tanch, 
                      list<node>& L, list<edge>& E,
                      node_array<vector>& p_trans,
                      node_array<double>& r1_trans,
                      node_array<double>& r2_trans,
                      edge_array<list<vector> >& e_trans,
                      edge_array<vector>& sa_trans,
                      edge_array<vector>& ta_trans);


void set_layout_step(const list<node>& L, const list<edge>& E,
                     const node_array<vector>& p_trans,
                     const node_array<double>& r1_trans,
                     const node_array<double>& r2_trans,
                     const edge_array<list<vector> >& e_trans,
                     const edge_array<vector>& sa_trans,
                     const edge_array<vector>& ta_trans);

void set_layout_end(const list<node>& L, const list<edge>& E,
                    const node_array<point>& n_dest, 
                    const node_array<double>& n_rad1, 
                    const node_array<double>& n_rad2, 
                    const edge_array<list<point> >& e_dest, 
                    const edge_array<point>& e_sanch, 
                    const edge_array<point>& e_tanch);



void set_layout(const node_array<point>& pos,
                const node_array<double>& r1,
                const node_array<double>& r2,
                const edge_array<list<point> >& bends,
                const edge_array<point>& sanch,
                const edge_array<point>& tanch);
/*{\Mopl  for every node $v$ the position is set to |pos[v]| and $radius_i$ is
          set to $r_i[v]$. For every edge $e$ the list of bends is set to 
          |bends[e]| and source (target) anchor is set to |sanch[e]|
          (|tanch[e]|). }*/

void set_layout(const node_array<double>& x, 
                const node_array<double>& y,
                const node_array<double>& r1, 
                const node_array<double>& r2,
                const edge_array<list<double> >& xbends,
                const edge_array<list<double> >& ybends,
                const edge_array<double>& x_sanch, 
                const edge_array<double>& y_sanch,
                const edge_array<double>& x_tanch, 
                const edge_array<double>& y_tanch);



void set_layout(const node_array<point>& pos,
                const edge_array<list<point> >& bends, bool reset_anchors=true);
/*{\Mopl  for every node $v$ the position is set to |pos[v]| and for
          every edge $e$ the list of bends is set to |bends[e]|. }*/

void set_layout(const node_array<double>& x, const node_array<double>& y,
                const edge_array<list<double> >& xbends,
                const edge_array<list<double> >& ybends,
                bool reset_anchors=true);

void set_layout(const node_array<point>& pos);
/*{\Mopl  for every node $v$ the position is set to |pos[v]| and for
          every edge $e$ the list of bends is made empty. }*/

void set_layout(const node_array<double>& x,const node_array<double>& y);
/*{\Mopl  for every node $v$ the position is set to $(x[v],y[v])$ and for
          every edge $e$ the list of bends is made empty. }*/

void set_layout();
/*{\Mop  same as |\Mvar.remove_bends()|. }*/


void transform_layout(node_array<double>& xpos, 
                      node_array<double>& ypos,
                      edge_array<list<double> >& xbends,
                      edge_array<list<double> >& ybends,
                      double dx, double dy, double fx, double fy);
/*{\Mop transforms the layout given by |xpos|, |ypos|, |xbends|, and |ybends|
        by transforming every node position or edge bend $(x,y)$ to 
        $(dx+fx*x,dy+fy*y)$. The actual layout of the current graph is not 
        changed by this operation. }*/

void transform_layout(node_array<double>& xpos, 
                      node_array<double>& ypos,
                      node_array<double>& xrad,
                      node_array<double>& yrad,
                      edge_array<list<double> >& xbends,
                      edge_array<list<double> >& ybends,
                      double dx, double dy, double fx, double fy);

/*{\Mop as above, in addition the horizontal and vertical radius of every 
        node (given in the arrays |xrad| and |yrad|) are enlarged by a factor
        of $fx$ and $fy$, respectively. }*/


void transform_coords(node_array<double>& xpos, 
                      node_array<double>& ypos,
                      edge_array<list<double> >& xbends,
                      edge_array<list<double> >& ybends,
                      double dx, double dy, double fx, double fy);


void fill_win_params(double wx0,double wy0, double wx1, double wy1,
                     double x0,double y0,double x1, double y1,
                     double& dx, double& dy, double& fx, double& fy);
/*{\Mop  computes  parameters |dx|, |dy|, |fx|, and |fy|
         for transforming rectangle |x0|,|y0|,|x1|,|y1| into (window) rectangle
         |wx0|,|wy0|,|wx1|,|wy1|. }*/ 
         

void fill_win_params(double wx0,double wy0,double wx1, double wy1,
                     node_array<double>& xpos, node_array<double>& ypos,
                     edge_array<list<double> >& xbends,
                     edge_array<list<double> >& ybends,
                     double& dx, double& dy, double& fx, double& fy);
/*{\Mop  computes  parameters |dx|, |dy|, |fx|, and |fy|
         for transforming the layout given |xpos|,|ypos|,|xbends|,|ybends|
         to fill the (window) rectangle |wx0|,|wy0|,|wx1|,|wy1|. }*/


void fill_win_params(double wx0,double wy0,double wx1, double wy1,
                     node_array<double>& xpos, node_array<double>& ypos,
                     node_array<double>& xrad, node_array<double>& yrad,
                     edge_array<list<double> >& xbends,
                     edge_array<list<double> >& ybends,
                     double& dx, double& dy, double& fx, double& fy);
/*{\Mop  computes  parameters |dx|, |dy|, |fx|, and |fy|
         for transforming the layout given |xpos|,|ypos|,|xbends|,|ybends|,
         |xrad|,|yrad| to fill the (window) rectangle 
         $wx0$,$wy0$,$wx1$,$wy1$. }*/



void fill_win_params(node_array<double>& xpos,
                     node_array<double>& ypos,
                     edge_array<list<double> >& xbends,
                     edge_array<list<double> >& ybends,
                     double& dx, double& dy, double& fx, double& fy);

void fill_win_params(node_array<double>& xpos,
                     node_array<double>& ypos,
                     node_array<double>& xrad,
                     node_array<double>& yrad,
                     edge_array<list<double> >& xbends,
                     edge_array<list<double> >& ybends,
                     double& dx, double& dy, double& fx, double& fy);



void place_into_box(double x0, double y0, double x1, double y1);
/*{\Mop moves and stretches the graph to fill the given rectangular box
        $(x0,y0,x1,y1)$ by appropriate scaling and translating
        operations. }*/

void place_into_win();
/*{\Mop moves and stretches the graph to fill the entire window 
        by appropriate scaling and translating operations. }*/



void adjust_coords_to_box(node_array<double>& xpos, node_array<double>& ypos,
                          edge_array<list<double> >& xbends,
                          edge_array<list<double> >& ybends,
                          double x0, double y0, double x1, double y1);
/*{\Mop transforms the layout given by |xpos|, |ypos|, |xbends|, and |ybends|
        in such way as a call of |place_into_box(x0,y0,x1,y1)| would do. 
        However, the actual layout of the current graph is not changed by this
        operation. }*/

void adjust_coords_to_box(node_array<double>& xpos, node_array<double>& ypos,
                          double x0, double y0, double x1, double y1);
/*{\Mop transforms the layout given by |xpos|, |ypos| in such way as a call of 
        |place_into_box(x0,y0,x1,y1)| would do ignoring any edge bends.
        The actual layout of the current graph is not changed by this
        operation. }*/


void adjust_coords_to_win(node_array<double>& xpos, node_array<double>& ypos,
                          edge_array<list<double> >& xbends,
                          edge_array<list<double> >& ybends);
/*{\Mop same as |adjust_coords_to_box(xpos,ypos,xbends,ybends,wx0,wy0,wx1,wy1)|
        for the current window rectangle $(wx0,wy0,wx1,wy1)$. }*/

void adjust_coords_to_win(node_array<double>& xpos, node_array<double>& ypos);
/*{\Mop same as |adjust_coords_to_box(xpos,ypos,wx0,wy0,wx1,wy1)|
        for the current window rectangle $(wx0,wy0,wx1,wy1)$. }*/



void remove_bends(edge e);
/*{\Mop removes all bends from edge |e|. }*/

void remove_bends();
/*{\Mop removes the bends of all edges of the graph. }*/

void reset_edge_anchors();
/*{\Mop resets all edge anchor positions to $(0,0)$. }*/


int read_layout(istream& istr, const list<node>& V,double=0,double=0,double=0,double=0);

int drop_file(string fname, double x, double y);

int load_layout(istream& istr);
/*{\Mopl read layout from stream |istr|.}*/

bool save_layout(ostream& ostr);
/*{\Mopl save layout to stream |ostr|.}*/

bool save_layout(string fname, bool ask_override=false);
/*{\Mopl save layout to file |fname|.}*/


int load_gml_layout(istream&);


    
//------------------------------------------------------------------------------
//  Zooming
//------------------------------------------------------------------------------

/*{\Mtext
\medskip
{\bf h) Zooming} 
}*/

void zoom(double f);
/*{\Mop zooms the window by factor $f$. }*/


void zoom_area(double x0, double y0, double x1, double y1);
/*{\Mop performs a zoom operation for the rectangular area
        with current coordinates $(x0,y0,x1,y0)$. }*/

void zoom_graph();
/*{\Mop performs a zoom operation, such that the graph fills 
        the entire window. }*/


// for backward compatibility
void fill_window() { zoom_graph(); }
/*{\Xop same as |zoom_graph()|. }*/


void unzoom();
/*{\Mop undoes last zoom operation. }*/


// experimental
void zoom_to_point(point,point,double);
void zoom_to_point(point);
void zoom_up_to_point(point);
void zoom_down_to_point(point);

void center_graph();


//------------------------------------------------------------------------------
//  Edit Mode
//------------------------------------------------------------------------------

/*{\Mtext
\medskip
{\bf i) Operations in Edit-mode} 

Before entering edit mode  ...

}*/
		
gw_action set_action(long mask, gw_action func);
/*{\Mopl	sets action associated with condition |mask| to |func| and 
                returns previous action for this condition. Here |gw_action| 
                is the type |void (*func)(GraphWin&, const point&)|. 
                For |func = NULL| the corresponding action is deleted. }*/


gw_action get_action(long mask);
/*{\Mopl	returns the action associated with condition $mask$. }*/


void reset_actions();
/*{\Mopl	resets all actions to their defaults. }*/

void clear_actions();
/*{\Mopl	deletes all actions. }*/


void add_node_menu(string label, gw_action func)
{ node_action_menu.append(two_tuple<string,gw_action>(label,func)); }
/*{\Mopl	appends action function |func| with label |label| to the 
                context menu for nodes (opened by clicking with the right 
                mouse button on a node). }*/

void add_edge_menu(string label, gw_action func)
{ edge_action_menu.append(two_tuple<string,gw_action>(label,func)); }
/*{\Mopl	appends action function |func| with label |label| to the 
                context menu for edges (opened by clicking with the right 
                mouse button on an edge). }*/



void set_new_node_handler(bool (*f)(GraphWin&,const point&));
/*{\Mopl	$f(gw,p)$ is called every time before a node is to 
                be created at position $p$. }*/

void set_new_node_handler(void (*f)(GraphWin&,node)=NULL);
/*{\Mopl	$f(gw,v)$ is called after node $v$ has been created. }*/ 

void set_new_edge_handler(bool (*f)(GraphWin&,node, node));
/*{\Mopl	$f(gw,v,w)$ is called before the edge $(v,w)$ 
                is to be created. }*/

void set_new_edge_handler(void (*f)(GraphWin&,edge)=NULL);
/*{\Mopl	$f(gw,e)$ is called after the edge $e$ has been
                created. }*/

void set_start_move_node_handler(void (*f)(GraphWin&,node));
void set_end_move_node_handler(bool (*f)(GraphWin&,node,const point&)=NULL);
void set_move_node_handler(bool (*f)(GraphWin&, node, const point&));

void set_start_move_node_handler(bool (*f)(GraphWin&,node)=NULL);
/*{\Mopl	$f(gw,v)$ is called before node $v$ is to be moved. }*/

void set_move_node_handler(void (*f)(GraphWin&,node)=NULL);
/*{\Mopl	$f(gw,v)$ is called every time node $v$ reaches a new
                position during a move operation. }*/

void set_end_move_node_handler(void (*f)(GraphWin&,node));
/*{\Mopl	$f(gw,v)$ is called after node $v$ has been moved. }*/


void set_del_node_handler(bool (*f)(GraphWin&,node));
/*{\Mopl	$f(gw,v)$ is called before the node $v$ is to be deleted. }*/

void set_del_node_handler(void (*f)(GraphWin&)=NULL);
/*{\Mopl        $f(gw)$ is called every time after a node was deleted. }*/

void set_del_edge_handler(bool (*f)(GraphWin&, edge));
/*{\Mopl	$f(gw,e)$ is called before the edge $e$ is to be deleted. }*/

void set_del_edge_handler(void (*f)(GraphWin&)=NULL);
/*{\Mopl        $f(gw)$ is called every time after an edge was deleted. }*/


void set_start_edge_slider_handler(void (*f)(GraphWin&,edge,double)=NULL,int sl=0);
/*{\Mopl	$f(gw,e,pos)$ is called before slider $sl$ of edge $e$
                is to be moved. Here $pos$ is the current slider position. }*/

void set_edge_slider_handler(void (*f)(GraphWin&,edge,double)=NULL, int sl=0);
/*{\Mopl	$f(gw,e,pos)$ is called every time slider $sl$ of edge $e$
                reaches a new position $pos$ during a slider move. }*/

void set_end_edge_slider_handler(void (*f)(GraphWin&,edge,double)=NULL,int sl=0);
/*{\Mopl	$f(gw,e,pos)$ is called after slider $sl$ of edge $e$
                has been moved to the final position $pos$. }*/


void set_split_edge_handler(bool (*f)(GraphWin& gw, edge e));
void set_split_edge_handler(void (*f)(GraphWin&,node)=NULL);

void set_move_component_handler(bool (*f)(GraphWin&,node));
void set_move_component_handler(void (*f)(GraphWin&,node)=NULL);
    
void set_init_graph_handler(bool (*f)(GraphWin&));
/*{\Mopl	$f$ is called every time before the entire graph is replaced,
		e.g. by a clear, generate, or load operation. }*/

void set_init_graph_handler(void (*f)(GraphWin&)=NULL);
/*{\Mopl	$f$ is called every time after the entire graph was replaced.}*/




//------------------------------------------------------------------------------
//  Menus 
//------------------------------------------------------------------------------

/*{\Mtext
\medskip
{\bf j) Menus}

The default menu  \dots
}*/

void set_default_menu(long mask);
/*{\Mop	... }*/

void add_menu(long menu_id) { menu_mask |= menu_id; }
/*{\Mop	... }*/

void del_menu(long menu_id) { menu_mask &= ~menu_id; }
/*{\Mop	... }*/



 
/*{\Mtext
\smallskip
Extending menus by new buttons and sub-menus \dots
}*/


int add_call(gw_base_function *f, string label, int menu_id=0, char* pmap=0);

int add_call(gw_base_function *f, string label, int menu_id, 
                                                int bm_w, int bm_h,
                                                unsigned char* bm_bits);

int add_menu(string label, window& M, char* pmap=0);



int add_menu(string label, int menu_id=0, char* pmap=0);
/*{\Mop	... }*/


int add_simple_call(void (*func)(GraphWin&), string label, int menu_id=0, char* pmap=0);
/*{\Mop	... }*/

int add_simple_call(void (*func)(GraphWin&), string label, int menu_id, 
                                                      int bm_w, int bm_h,
                                                      unsigned char* bm_bits);
/*{\Mop	... }*/



int add_member_call(void (GraphWin::*func)(), string label, int menu_id=0, char* pmap=0);
/*{\Mop	... }*/

int add_member_call(void (GraphWin::*func)(), string label, int menu_id, 
                                                       int bm_w, int bm_h,
                                                       unsigned char* bm_bits);
/*{\Mop	... }*/

void add_separator(int menu_id);
/*{\Mop	... }*/


void display_help_text(string fname) { win_p->display_help_text(fname); }
/*{\Mop  displays the help text contained in |name.hlp|. The file 
         |name.hlp| must exist either in the current working directory or in
         |\$LEDAROOT/incl/Help|. }*/


void add_help_text(string name) 
{ if (win_p->is_open()) 
    LEDA_EXCEPTION(0,"gw.add_help_text() must be called before gw.display().");
   help_list.append(name);
}
/*{\Mop  adds the help text contained in |name.hlp| with label |name|
         to the help menu of the main window. The file |name.hlp| must
         exist either in the current working directory or in
         |\$LEDAROOT/incl/Help|. Note that this operation must be called
         before |gw.display()|. }*/






int get_menu(string label);
/*{\Mopl  returns the number of the submenu with label |label| or
          |-1| if no such menu exists. }*/


void enable_call(int id);
/*{\Mopl	enable call with id $id$. }*/
  
void disable_call(int id);
/*{\Mopl	disable call with id $id$. }*/

bool is_call_enabled(int id);
/*{\Mopl        check if call with $id$ is enabled. }*/	
  

void enable_calls();
/*{\Mop	... }*/

void disable_calls();
/*{\Mop	... }*/

int set_buttons_per_line(int n);


//------------------------------------------------------------------------------
//  Input/Output
//------------------------------------------------------------------------------

/*{\Mtext
\medskip
{\bf k) Input/Output} }*/

string get_xpm_path();

string get_dirname() const { return gw_dirname; }
string set_dirname(string s);

string get_graphname() const { return graph_name; }
string set_graphname(string s);

//for backward compatibility

string get_gml_filename() const { return get_graphname() + ".gml"; }
string set_gml_filename(string s)
{ return set_graphname(s.replace(".gml","")) + ".gml"; }

string get_filename() const { return get_graphname() + ".gw"; }
string set_filename(string s)
{ return set_graphname(s.replace(".gw","")) + ".gw"; }

//end backward compatibility


string get_ps_fontname() const { return ps_fontname; }
string set_ps_fontname(string s); // "Times", "Helvetica", "Courier"

ps_zoom_mode get_ps_zoom_mode() const;
ps_zoom_mode set_ps_zoom_mode(ps_zoom_mode);


string get_print_cmd() const   { return print_cmd; }
string set_print_cmd(string s);

void file_handler(int);


int  read_dimacs(istream& in);
int  read_dimacs(string fname, bool adk_override=false);

bool save_dimacs(ostream& out);
bool save_dimacs(string fname, bool ask_override=false);



int read(istream& in, bool clr=true);
int read(string fname, bool clr=true, bool ask_override=false);


int read_gw(istream& in)  { return read(in,true); }
/*{\Mopl reads graph in |gw| format from stream |in|.}*/

int read_gw(string fname) { return read(fname,true); }
/*{\Mopl reads graph in |gw| format from file |fname|.}*/


bool save_gw(ostream& out);
/*{\Mopl writes graph in |gw| format to output stream |out|.}*/

bool save_gw(string fname, bool ask_overwrite=false);
/*{\Mopl saves graph in |gw| format to file |fname|.}*/


bool save(ostream& out) { return save_gw(out); }



int read_gml(istream& in);
/*{\Mopl reads graph in GML format from stream |in|.}*/

int read_gml_string(string s);
/*{\Mopl reads graph in GML format from string |s|.}*/

int read_gml(string fname, bool ask_override=false);
/*{\Mopl reads graph in GML format from file |fname|.
         Returns $1$ if $fname$ cannot be opened, $2$ if a parser error
         occurs, and $0$ on success. }*/

bool save_gml(ostream& out);
/*{\Mopl writes graph in GML format to output stream |out|.}*/

bool save_gml(string fname, bool ask_override=false);
/*{\Mopl saves graph to file |fname| in GML format.}*/




bool save_ps(string fname, bool ask_override=false);
/*{\Mop saves a postscript representation of the graph to |fname|. }*/

bool save_svg(string fname, bool ask_override=false);
/*{\Mop saves a SVG representation of the graph to |fname|. }*/


bool save_latex(string fname, string pspath, double x0, double y0, 
                                             double x1, double y1, 
                                             double width, bool ask_override);

bool save_latex(string fname, bool ask_override=false)
{ return save_latex(fname,"",0,0,0,0,0,ask_override); }
/*{\Mop saves a postscript/latex representation of the graph to |fname|. }*/

bool save_wmf(string fname, bool ask_override=false);
/*{\Mop saves a windows metafile representation of the graph to |fname|. }*/


bool unsaved_changes();
/*{\Mop  returns true if the graph has been changed after the last
         save (gw or gml) operation. }*/


bool save_defaults(string fname);
/*{\Mop saves the default attributes of nodes and edges to file |fname|. }*/

bool read_defaults(string fname);
/*{\Mop reads the default attributes of nodes and edges from file |fname|. }*/


bool save_defaults();
bool read_defaults();



//------------------------------------------------------------------------------
//  Miscellaneous Operations
//------------------------------------------------------------------------------

/*{\Mtext
\medskip
{\bf l) Miscellaneous} }*/

void set_window(window& W);
/*{\Mop makes |W| the window of |\Mvar|. }*/

void set_graph(graph& G);
/*{\Mop makes |G| the graph of |\Mvar|. }*/


void new_cluster(const polygon& pol,
                 string label, color border_c, color fill_c = invisible, 
                 color text_c = black, int lw = 1, line_style ls = solid) 
{ 
  cluster_list.append(cluster_box(pol,label,border_c,fill_c,text_c,lw,ls)); 
 }


void new_cluster(double x0, double y0, double x1, double y1, 
                 string label, color border_c, color fill_c = invisible, 
                 color text_c = black, int lw = 1, line_style ls = solid) 
{ 
  cluster_list.append(cluster_box(x0,y0,x1,y1,label,
                                  border_c,fill_c,text_c,lw,ls)); 
 }

void del_all_clusters() { cluster_list.clear(); }

               
bool get_directed() const  { return get_edge_direction()==directed_edge; }
bool set_directed(bool b)  
{ bool old = get_directed();
  set_edge_direction((b) ? directed_edge : undirected_edge);
  return old;
}

void open_msg_win();
void show_msg(const char*);
void close_msg_win() { msg_win->close(); }


void undo_clear();
/*{\Mop  empties the undo and redo stacks. }*/


bool wait();
/*{\Mop	   waits until the done button is pressed (|true| returned) or 
           exit is selected from the file menu (|false| returned). }*/

bool wait(const char* msg);
/*{\Mop	  displays $msg$ and waits until the done button is pressed 
          (|true| returned) or exit is selected from the file menu 
          (|false| returned). }*/


bool wait(float sec, const char* msg="");
/*{\Mopl   as above but waits no longer than |sec| seconds
           returns ?? if neither button was pressed within this 
           time interval. }*/

void acknowledge(string s1, string s2);

void acknowledge(string s);
/*{\Mopl     displays string $s$ and asks for acknowledgement. }*/


node read_node();
edge read_edge();

node ask_node() { return read_node(); }
/*{\Mop     asks the user to select a node with the left mouse button.
            If a node is selected it is returned otherwise nil is returned.}*/
          
edge ask_edge() { return read_edge(); }
/*{\Mop     asks the user to select an edge with the left mouse button.
            If an edge is selected it is returned otherwise nil is returned.}*/

bool define_area(double& x0, double& y0, double& x1, double& y1, 
                 const char* msg="");
/*{\Mop     displays message |msg| and returns the coordinates of a rectangular
            area defined by clicking and dragging the mouse. }*/

list<node> get_nodes_in_area(double x0, double y0, double x1, double y1);
/*{\Mop     returns the list of nodes intersecting the rectangular area 
            $(x0,y0,x1,y1)$. }*/

list<edge> get_edges_in_area(double x0, double y0, double x1, double y1);
/*{\Mop     returns the list of edges intersecting the rectangular area 
            $(x0,y0,x1,y1)$. }*/


void save_node_attributes();
/*{\Mopl  ... }*/

void save_edge_attributes();
/*{\Mopl  ... }*/

void save_all_attributes();
/*{\Mopl  ... }*/
    

void restore_node_attributes();
/*{\Mopl  ... }*/

void restore_edge_attributes();
/*{\Mopl  ... }*/

void restore_all_attributes();
/*{\Mopl  ... }*/



void reset_nodes(long mask=N_ALL);
/*{\Mopl  reset node parameters to their default values. }*/
    
void reset_edges(long mask=E_ALL);
/*{\Mopl  reset edge parameters to their default values. }*/

void reset();
/*{\Mopl  reset node and edge parameters to their default values. }*/

void reset_defaults() { init_default(); flush = flush0; }
/*{\Mopl  resets default parameters to their original values. }*/


void   set_agd_host(string host) { agd_host = host; }
string get_agd_host()            { return agd_host; }

int    get_agd_port() { return agd_port; }

bool   run_agd(string alg);

node get_found_node()  const { return found_node; }
node get_found_frame() const { return found_frame; }
edge get_found_edge()  const { return found_edge; }

    
node find_node(const point& p, node from=nil);
edge find_edge(const point& p, edge from=nil);
int  find_slider(const point& p, edge e);


node get_edit_node() const { return found_node; }
/*{\Mop returns a node under the current mouse pointer position
        (|nil| if there is no node at the current position) }*/


edge get_edit_edge() const { return found_edge; }
/*{\Mop returns an edge under the current mouse pointer position
        (|nil| if there is no edge at the current position). }*/

int  get_edit_slider() const { return found_slider; }
/*{\Mop returns the number of the slider under the current mouse pointer 
        position (|0| if there is no edge slider at the current position). }*/




void get_bounding_box(double& x0, double& y0, double& x1, double& y1);
/*{\Mop computes the coordinates $(x0,y0,x1,y1)$ of a minimal bounding box
        for the current layout of the graph. }*/

void get_bounding_box(const list<node>& V, const list<edge>& E,
                      double& x0, double& y0, double& x1, double& y1);
/*{\Mop computes the coordinates $(x0,y0,x1,y1)$ of a minimal bounding box
        for the current layout of subgraph $(V,E)$. }*/



// help functions

void help_about();
void help_mouse_left();
void help_mouse_middle();
void help_mouse_right();
void help_news();


//----------------------------------------------------------------------------
// friend functions
//----------------------------------------------------------------------------


friend void gw_bg_redraw_func(window*, double, double, double, double);

friend void scroll_redraw(window* wp, double x0,double y0,double x1,double y1);


friend __exportF void gw_call_menu_func(int);


friend __exportF void gw_edit_node_label(GraphWin& gw, const point& p);
friend __exportF void gw_edit_edge_label(GraphWin& gw, const point& p);

friend __exportF void gw_zoom_area(GraphWin& gw, const point&);
friend __exportF void gw_zoom_point(GraphWin& gw, const point&);
friend __exportF void gw_zoom_edge(GraphWin& gw, const point&);
friend __exportF void gw_setup_component(GraphWin& gw, const point&);
friend __exportF void gw_move_node(GraphWin& gw, const point& p);
friend __exportF void gw_resize_node(GraphWin& gw, const point& p);
friend __exportF void gw_move_edge(GraphWin& gw, const point& p);
friend __exportF void gw_set_edge_slider(GraphWin& gw, const point& p);
friend __exportF void gw_move_edge_slider(GraphWin& gw, const point& p);
friend __exportF void gw_slide_edge(GraphWin& gw, const point& p);
friend __exportF void gw_move_nodes(GraphWin&, const point&, const list<node>&);
friend __exportF void gw_move_component(GraphWin& gw, const point& p);
friend __exportF void gw_select_component(GraphWin& gw, const point&);
friend __exportF void gw_select_all_nodes(GraphWin& gw, const point&);
friend __exportF void gw_deselect_all_nodes(GraphWin& gw, const point&);
friend __exportF void gw_select_node_toggle(GraphWin& gw, const point&);
friend __exportF void gw_select_node(GraphWin& gw, const point&);
friend __exportF void gw_select_edge_toggle(GraphWin& gw, const point&);
friend __exportF void gw_select_edge(GraphWin& gw, const point&);
friend __exportF void gw_new_node(GraphWin& gw, const point& p);
friend __exportF void gw_new_edge(GraphWin& gw, const point&);
friend __exportF void gw_del_node(GraphWin& gw, node);
friend __exportF void gw_del_node(GraphWin& gw, const point&);
friend __exportF void gw_del_edge(GraphWin& gw, edge);
friend __exportF void gw_del_edge(GraphWin& gw, const point&);
friend __exportF void gw_del_component(GraphWin& gw, const point&);
friend __exportF void gw_setup_node(GraphWin& gw, const point&);
friend __exportF void gw_setup_edge(GraphWin& gw, const point&);
friend __exportF void gw_scroll_graph(GraphWin& gw, const point& p);
friend __exportF void gw_select_area(GraphWin& gw, const point& p);
friend __exportF void gw_split_edge(GraphWin& gw, const point& p);
friend __exportF void gw_del_selected_nodes(GraphWin& gw, const point&);

friend __exportF bool gw_setup(GraphWin& gw,const list<node>& L,
                                            long mask);
friend __exportF bool gw_setup(GraphWin& gw,const list<edge>& L,
                                            long mask);


friend __exportF void gw_window_setup(GraphWin& gw);
friend __exportF void gw_global_setup(GraphWin& gw);
friend __exportF void gw_agd_setup(GraphWin& gw);
friend __exportF void gw_animation_setup(GraphWin& gw);
friend __exportF void gw_zooming_setup(GraphWin& gw);
friend __exportF void gw_postscript_setup(GraphWin& gw);
friend __exportF void gw_svg_setup(GraphWin& gw);

friend __exportF void save_metafile(string);

friend __exportF void gw_load_dimacs_handler(GraphWin& gw);
friend __exportF void gw_save_dimacs_handler(GraphWin& gw);

friend __exportF void gw_load_handler(GraphWin& gw);
friend __exportF void gw_save_handler(GraphWin& gw);
friend __exportF void gw_load_gml_handler(GraphWin& gw);
friend __exportF void gw_save_gml_handler(GraphWin& gw);
friend __exportF void gw_save_ps_handler(GraphWin& gw);
friend __exportF void gw_save_svg_handler(GraphWin& gw);
friend __exportF void gw_save_tex_handler(GraphWin& gw);
friend __exportF void gw_print_ps_handler(GraphWin& gw);
friend __exportF void gw_clipboard_handler(GraphWin& gw);
friend __exportF void gw_metafile_handler(GraphWin& gw);
friend __exportF void gw_screenshot_handler(GraphWin& gw);

friend __exportF void gw_file_handler(GraphWin& gw);

friend __exportF void gw_open_root_menu(GraphWin& gw, const point& p);
friend __exportF void gw_open_node_menu(GraphWin& gw, const point& p);
friend __exportF void gw_open_edge_menu(GraphWin& gw, const point& p);

// algorithms 

friend __exportF void gw_maxflow(GraphWin& gw);
friend __exportF void gw_maxflowS(GraphWin& gw);


friend __exportF int gw_components(GraphWin& gw, node_array<int>& compnum);
friend __exportF int gw_strong_components(GraphWin& gw, node_array<int>& compnum);
friend __exportF int gw_biconnected_components(GraphWin& gw, 
                                     edge_array<int>& compnum);

friend __exportF list<edge> gw_spanning_tree(GraphWin& gw);

friend __exportF GRAPH<node,edge> gw_transitive_closure(GraphWin& gw);
friend __exportF GRAPH<node,edge> gw_transitive_reduction(GraphWin& gw);
friend __exportF void gw_make_transitively_closed(GraphWin& gw);
friend __exportF void gw_make_transitively_reduced(GraphWin& gw);

friend __exportF bool gw_is_planar(GraphWin& gw);

friend __exportF int gw_kuratowski(GraphWin& gw, list<node>& V, list<edge>& E, 
                                                         node_array<int>& deg);

friend __exportF void gw_straight_line_embedding(GraphWin& gw, 
                                                 node_array<double>& x,
                                                 node_array<double>& y);

friend __exportF void gw_d3_spring_embedding(const graph& G, 
                                            node_array<double>& xpos, 
                                            node_array<double>& ypos,
                                            node_array<double>& zpos,
                                            double xmin, double xmax, 
                                            double ymin, double ymax,
                                            double zmin, double zmax,
                                            int n);

friend __exportF int gw_max_flow(GraphWin& gw, node s, node t, 
                                                   const edge_array<int>& cap, 
                                                   edge_array<int>& flow);

friend __exportF int gw_min_cost_flow(GraphWin& gw, const edge_array<int>& lcap, 
                                         const edge_array<int>& ucap,
                                         const edge_array<int>& cost,
                                         const node_array<int>& supply,
                                         edge_array<int>& flow);

friend __exportF int gw_min_cost_max_flow(GraphWin& gw, node s, node t, 
                                         const edge_array<int>& cap, 
                                         const edge_array<int>& cost,
                                         edge_array<int>& flow);



// embedder

friend __exportF void gw_straight_embed1(GraphWin& gw);
friend __exportF void gw_straight_embed2(GraphWin& gw);
friend __exportF void gw_circle_embed(GraphWin& gw);
friend __exportF void gw_random_embed(GraphWin& gw);
friend __exportF void gw_quod_embed(GraphWin& gw);
friend __exportF void gw_ortho_embed(GraphWin& gw);
friend __exportF void gw_spring_embed(GraphWin& gw, bool rand);
friend __exportF void gw_spring_embed(GraphWin& gw);
friend __exportF void gw_d3_spring_embed(GraphWin& gw);
friend __exportF void gw_visrep_embed(GraphWin& gw);
friend __exportF void gw_sp_embed(GraphWin& gw);
friend __exportF void gw_d3_sp_embed(GraphWin& gw);

friend __exportF void gw_hyperbol_embed(GraphWin& gw);


// tester

friend __exportF void gw_test_con(GraphWin& gw);
friend __exportF void gw_test_bicon(GraphWin& gw);
friend __exportF void gw_test_tricon(GraphWin& gw);
friend __exportF void gw_test_planar(GraphWin& gw); 
friend __exportF void gw_test_series_parallel(GraphWin& gw); 
friend __exportF void gw_test_bipart(GraphWin& gw); 

friend __exportF void gw_test_planarity(GraphWin& gw); 
friend __exportF void gw_test_spness(GraphWin& gw);
  

// zoomer

friend __exportF void gw_center_graph(GraphWin& gw);
friend __exportF void gw_zoom_graph(GraphWin& gw);
friend __exportF void gw_unzoom(GraphWin& gw);
friend __exportF void gw_zoom_up(GraphWin& gw);
friend __exportF void gw_zoom_down(GraphWin& gw);

friend __exportF void gw_zoom_up(GraphWin& gw,const point&);
friend __exportF void gw_zoom_up_step(GraphWin& gw, const point&);
friend __exportF void gw_zoom_up_to_point(GraphWin& gw, const point&);

friend __exportF void gw_zoom_down(GraphWin& gw,const point&);
friend __exportF void gw_zoom_down_step(GraphWin& gw, const point&);
friend __exportF void gw_zoom_down_to_point(GraphWin& gw, const point&);

friend __exportF void gw_zoom_area(GraphWin& gw);
friend __exportF void gw_zoom_default(GraphWin& gw);

friend __exportF void gw_place_into_box(GraphWin& gw);
friend __exportF void gw_place_into_win(GraphWin& gw);
friend __exportF void gw_shrink_graph(GraphWin& gw);
friend __exportF void gw_expand_graph(GraphWin& gw);


// AGD
friend void gw_run_agd_alg(GraphWin& gw);
friend void gw_about_agd_alg(GraphWin& gw, string alg);

// Misc
friend void gw_new_graph(GraphWin& gw);


// done/exit handler
friend void done_handler(GraphWin& gw);
friend void exit_handler(GraphWin& gw);

// undo/redo handler
friend void undo_handler(GraphWin& gw);
friend void redo_handler(GraphWin& gw);

void undo();
void redo();


void changing_layout(const list<node>& N, const list<edge>& E);
void changing_layout(const list<node>& N);
void changing_layout(const list<edge>& E);

void changing_layout(node v) { changing_layout((list<node>)v); }
void changing_layout(edge e) { changing_layout((list<edge>)e); }

};



void gw_bg_redraw_func(window*, double, double, double, double);

inline void scroll_redraw(window* wp, double x0,double y0,double x1,double y1) 
{  wp->get_graphwin()->draw_graph(x0,y0,x1,y1); }

__exportF void gw_call_menu_func(int);


__exportF void gw_edit_node_label(GraphWin& gw, const point& p);
__exportF void gw_edit_edge_label(GraphWin& gw, const point& p);

__exportF void gw_zoom_area(GraphWin& gw, const point&);
__exportF void gw_zoom_point(GraphWin& gw, const point&);
__exportF void gw_zoom_edge(GraphWin& gw, const point&);
__exportF void gw_setup_component(GraphWin& gw, const point&);
__exportF void gw_move_node(GraphWin& gw, const point& p);
__exportF void gw_resize_node(GraphWin& gw, const point& p);
__exportF void gw_move_edge(GraphWin& gw, const point& p);
__exportF void gw_set_edge_slider(GraphWin& gw, const point& p);
__exportF void gw_move_edge_slider(GraphWin& gw, const point& p);
__exportF void gw_slide_edge(GraphWin& gw, const point& p);
__exportF void gw_move_nodes(GraphWin&, const point&, const list<node>&);
__exportF void gw_move_component(GraphWin& gw, const point& p);
__exportF void gw_select_component(GraphWin& gw, const point&);
__exportF void gw_select_all_nodes(GraphWin& gw, const point&);
__exportF void gw_deselect_all_nodes(GraphWin& gw, const point&);
__exportF void gw_select_node_toggle(GraphWin& gw, const point&);
__exportF void gw_select_node(GraphWin& gw, const point&);
__exportF void gw_select_edge_toggle(GraphWin& gw, const point&);
__exportF void gw_select_edge(GraphWin& gw, const point&);
__exportF void gw_new_node(GraphWin& gw, const point& p);
__exportF void gw_new_edge(GraphWin& gw, const point&);
__exportF void gw_del_node(GraphWin& gw, node);
__exportF void gw_del_node(GraphWin& gw, const point&);
__exportF void gw_del_edge(GraphWin& gw, edge);
__exportF void gw_del_edge(GraphWin& gw, const point&);
__exportF void gw_del_component(GraphWin& gw, const point&);
__exportF void gw_setup_node(GraphWin& gw, const point&);
__exportF void gw_setup_edge(GraphWin& gw, const point&);
__exportF void gw_scroll_graph(GraphWin& gw, const point& p);
__exportF void gw_select_area(GraphWin& gw, const point& p);
__exportF void gw_split_edge(GraphWin& gw, const point& p);
__exportF void gw_del_selected_nodes(GraphWin& gw, const point&);

__exportF bool gw_setup(GraphWin& gw,const list<node>& L,
                                            long mask=N_COMPLETE);
__exportF bool gw_setup(GraphWin& gw,const list<edge>& L,
                                            long mask=E_COMPLETE);


__exportF void gw_window_setup(GraphWin& gw);
__exportF void gw_global_setup(GraphWin& gw);
__exportF void gw_agd_setup(GraphWin& gw);
__exportF void gw_animation_setup(GraphWin& gw);
__exportF void gw_zooming_setup(GraphWin& gw);
__exportF void gw_postscript_setup(GraphWin& gw);
__exportF void gw_svg_setup(GraphWin& gw);

__exportF void save_metafile(string);

__exportF void gw_load_dimacs_handler(GraphWin& gw);
__exportF void gw_save_dimacs_handler(GraphWin& gw);

__exportF void gw_load_handler(GraphWin& gw);
__exportF void gw_save_handler(GraphWin& gw);
__exportF void gw_load_gml_handler(GraphWin& gw);
__exportF void gw_save_gml_handler(GraphWin& gw);
__exportF void gw_save_ps_handler(GraphWin& gw);
__exportF void gw_save_svg_handler(GraphWin& gw);
__exportF void gw_save_tex_handler(GraphWin& gw);
__exportF void gw_print_ps_handler(GraphWin& gw);
__exportF void gw_clipboard_handler(GraphWin& gw);
__exportF void gw_metafile_handler(GraphWin& gw);
__exportF void gw_screenshot_handler(GraphWin& gw);

__exportF void gw_file_handler(GraphWin& gw);

__exportF void gw_open_root_menu(GraphWin& gw, const point& p);
__exportF void gw_open_node_menu(GraphWin& gw, const point& p);
__exportF void gw_open_edge_menu(GraphWin& gw, const point& p);


// algorithms 

__exportF void gw_maxflow(GraphWin& gw);
__exportF void gw_maxflowS(GraphWin& gw);


__exportF int gw_components(GraphWin& gw, node_array<int>& compnum);
__exportF int gw_strong_components(GraphWin& gw, node_array<int>& compnum);
__exportF int gw_biconnected_components(GraphWin& gw, 
                                     edge_array<int>& compnum);

__exportF list<edge> gw_spanning_tree(GraphWin& gw);

__exportF GRAPH<node,edge> gw_transitive_closure(GraphWin& gw);
__exportF GRAPH<node,edge> gw_transitive_reduction(GraphWin& gw);
__exportF void gw_make_transitively_closed(GraphWin& gw);
__exportF void gw_make_transitively_reduced(GraphWin& gw);

__exportF bool gw_is_planar(GraphWin& gw);

__exportF int gw_kuratowski(GraphWin& gw, list<node>& V, list<edge>& E, 
                                                         node_array<int>& deg);

__exportF void gw_straight_line_embedding(GraphWin& gw, 
                                                 node_array<double>& x,
                                                 node_array<double>& y);

__exportF void gw_d3_spring_embedding(const graph& G, node_array<double>& xpos, 
                                                      node_array<double>& ypos,
                                                      node_array<double>& zpos,
                                                      double xmin, double xmax, 
                                                      double ymin, double ymax,
                                                      double zmin, double zmax,
                                                      int n);

__exportF int gw_max_flow(GraphWin& gw, node s, node t, 
                                                   const edge_array<int>& cap, 
                                                   edge_array<int>& flow);

__exportF int gw_min_cost_flow(GraphWin& gw, const edge_array<int>& lcap, 
                                         const edge_array<int>& ucap,
                                         const edge_array<int>& cost,
                                         const node_array<int>& supply,
                                         edge_array<int>& flow);

__exportF int gw_min_cost_max_flow(GraphWin& gw, node s, node t, 
                                         const edge_array<int>& cap, 
                                         const edge_array<int>& cost,
                                         edge_array<int>& flow);




// embedder

__exportF void gw_straight_embed1(GraphWin& gw);
__exportF void gw_straight_embed2(GraphWin& gw);
__exportF void gw_circle_embed(GraphWin& gw);
__exportF void gw_random_embed(GraphWin& gw);
__exportF void gw_quod_embed(GraphWin& gw);
__exportF void gw_ortho_embed(GraphWin& gw);
__exportF void gw_spring_embed(GraphWin& gw, bool rand);
__exportF void gw_spring_embed(GraphWin& gw);
__exportF void gw_d3_spring_embed(GraphWin& gw);
__exportF void gw_visrep_embed(GraphWin& gw);
__exportF void gw_sp_embed(GraphWin& gw);
__exportF void gw_d3_sp_embed(GraphWin& gw);

__exportF void gw_hyperbol_embed(GraphWin& gw);


// tester

__exportF void gw_test_con(GraphWin& gw);
__exportF void gw_test_bicon(GraphWin& gw);
__exportF void gw_test_tricon(GraphWin& gw);
__exportF void gw_test_planar(GraphWin& gw); 
__exportF void gw_test_series_parallel(GraphWin& gw); 
__exportF void gw_test_bipart(GraphWin& gw); 

__exportF void gw_test_planarity(GraphWin& gw); 
__exportF void gw_test_spness(GraphWin& gw);
  
// zoomer

__exportF void gw_center_graph(GraphWin& gw);
__exportF void gw_zoom_graph(GraphWin& gw);
__exportF void gw_unzoom(GraphWin& gw);

__exportF void gw_zoom_up(GraphWin& gw);
__exportF void gw_zoom_up_step(GraphWin& gw, const point&);
__exportF void gw_zoom_up_to_point(GraphWin& gw, const point&);

__exportF void gw_zoom_down(GraphWin& gw);
__exportF void gw_zoom_down_step(GraphWin& gw, const point&);
__exportF void gw_zoom_down_to_point(GraphWin& gw, const point&);

__exportF void gw_zoom_area(GraphWin& gw);
__exportF void gw_zoom_default(GraphWin& gw);

__exportF void gw_place_into_box(GraphWin& gw);
__exportF void gw_place_into_win(GraphWin& gw);
__exportF void gw_shrink_graph(GraphWin& gw);
__exportF void gw_expand_graph(GraphWin& gw);


// AGD
void gw_run_agd_alg(GraphWin& gw);
void gw_about_agd_alg(GraphWin& gw, string alg);

// Misc
void gw_new_graph(GraphWin& gw);

// done/exit handler
inline void done_handler(GraphWin& gw) { gw.edit_result = 1; }
inline void exit_handler(GraphWin& gw) { gw.edit_result = 0; }

// undo/redo handler
inline void undo_handler(GraphWin& gw) { gw.undo(); }
inline void redo_handler(GraphWin& gw) { gw.redo(); }





//----------------------------------------------------------------------------
// gw_add_call templates
//----------------------------------------------------------------------------

template<class func_t>
class gw_function : public gw_base_function 
{
  typedef void (*call_t)(GraphWin&,func_t);

  func_t func;
  call_t caller;

public:
  gw_function(func_t f, call_t c) : func(f), caller(c) {};
  void call(GraphWin& gw) { caller(gw,func); }
};



template<class func_t>
inline int gw_add_call(GraphWin& gw, func_t f, void (*c)(GraphWin&,func_t), 
                                               string label, int menu_id) 
{ gw_base_function* bf = new gw_function<func_t>(f,c);
  return gw.add_call(bf,label,menu_id);
} 


template<class func_t>
inline int gw_add_call(GraphWin& gw, func_t f, void (*c)(GraphWin&,func_t), 
                                               string label) 
{ gw_base_function* bf = new gw_function<func_t>(f,c);
  return gw.add_call(bf,label,0);
} 


/*
template<class func_t>
inline int gw_add_call(GraphWin& gw, func_t f, void (*c)(GraphWin&,func_t),
                                               const char* label,int menu_id)
{ gw_base_function* bf = new gw_function<func_t>(f,c);
  return gw.add_call(bf,label,menu_id);
} 


template<class func_t>
inline int gw_add_call(GraphWin& gw, func_t f, void (*c)(GraphWin&,func_t), 
                                               const char* label) 
{ gw_base_function* bf = new gw_function<func_t>(f,c);
  return gw.add_call(bf,label,0);
} 

*/



//----------------------------------------------------------------------------
// non-member versions gw_add_menu, gw_add_simple/member_call 
// (for backward compatibility only)
//----------------------------------------------------------------------------

inline int gw_add_menu(GraphWin& gw, string l, int id=0) 
{ return gw.add_menu(l,id); }


inline int gw_add_simple_call(GraphWin& gw, void (*f)(GraphWin&), string l,
                                                                  int id=0) 
{ return gw.add_simple_call(f,l,id); } 


inline int gw_add_member_call(GraphWin& gw, void (GraphWin::*f)(), string l,
                                                                   int id=0) 
{ return gw.add_member_call(f,l,id); } 




#if LEDA_ROOT_INCL_ID == 600132
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif



LEDA_END_NAMESPACE

#endif
