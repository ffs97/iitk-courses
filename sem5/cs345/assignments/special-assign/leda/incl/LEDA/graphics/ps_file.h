/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  ps_file.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_PS_FILE_H
#define LEDA_PS_FILE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600149
#include <LEDA/internal/PREAMBLE.h>
#endif

// changed 6/99 Mattthias Baesken
// fill_color and point styles added
// testing, if color = invisible before drawing
// output operators changed
// old FILLMODE flag removed
// 7/99 geo_graph support added (output operator)
// 8/00 added from graphwin - ps output:
// - pixmap support
// - draw_roundrect/draw_filled_roundrect
// - draw_arrow_head_2 -> draw_arrow_head_poly
// - draw_polygon_edge/draw_bezier_edge
// - draw_spline_edge/draw_circle_edge
// - some clipping operations
// - pixmap support (put_pixrect/include_xpm)

/*{\Manpage {ps_file} {} {Postscript Files}}*/

#include <LEDA/system/basic.h>
#include <LEDA/core/list.h>
#include <LEDA/system/stream.h>
#include <LEDA/geo/point.h>
#include <LEDA/geo/segment.h>
#include <LEDA/geo/line.h>
#include <LEDA/geo/circle.h>
#include <LEDA/geo/polygon.h>
#include <LEDA/geo/rectangle.h>
#include <LEDA/graphics/color.h>
#include <LEDA/graphics/window.h>
#include <LEDA/graphics/window.h>
#include <LEDA/graphics/geo_graph.h>

LEDA_BEGIN_NAMESPACE

enum output_mode {gray_mode,colored_mode};

static const double PS_PIXEL    =2.54/72.0; 
static const double PS_HEIGHT   =23.5;
static const double PS_WIDTH    =16.0;

class __exportC ps_file_pair {
  friend class __exportC ps_file;
  color c;
  double grayvalue;
public:
  ps_file_pair(): c(black),grayvalue(0) {}
  ps_file_pair(color col,double d): c(col),grayvalue(d) {}
  friend istream& operator>>(istream& in,ps_file_pair&) { return in; }
  friend ostream& operator<<(ostream& out,const ps_file_pair&) { return out; }
};



class __exportC ps_file {

/*{\Mdefinition
The date type |\Mname| is a graphical input/output interface for the
familiar LEDA drawing operations of two-dimensional geometry. Unlike
the data type |window|, the output produced by a |\Mname| object is
\emph{permanent}, i.e., it is not lost after exiting the C++-program as
it is saved in an output file. 

An instance of type |\Mname| is (as far as the user takes notice of it)
an ordinary ASCII file that contains the source code of the graphics
output in the PostScript description language. After running the
C++-program, the file is created in the user's current working
directory and can later be handled like any other PostScript file, i.e., it
may be viewed, printed etc.

Of course, features like a panel section (as in |window| type
instances) don't make sense for a representation that is not supposed
to be displayed on the screen and interactively worked with by the
user. Therefore, only drawing operations are applicable to a |\Mname|
instance.

|\Mname| was implemented by 
\begin{center}
Thomas Wahl\newline 
Lehrstuhl f\"ur Informatik I\newline
Universit\"at W\"urzburg\newline
\end{center}
The complete user manual can be found in LEDAROOT/Manual/contrib.
}*/


private:
  static const string VERSION;
  static const short  X_SHIFT;
  static const short  Y_SHIFT;
#ifndef PI
  static const double PI;
#endif
  static const double SIZE;
  static const string DEFAULT_FONT;
  static const double DEFAULT_FACTOR;

  string       filename;
  file_ostream file;
  double       width, height;
  double       X0,X1,Y0,Y1;
  double       scaling;
  bool         must_scale;
  double       bbx,bby;
  bool         draw_bb;
  short        page;
  output_mode  outputmode;
  list<ps_file_pair>   grayvalue_list;
  color        fg_col;
  color        fill_color;
  double       linewidth;
  double       unitlinewidth;
  line_style   linestyle;
  point_style  pt_style;
  double       nodewidth;
  double       crosswidth;
  double       arrowlength;
  double       arrowangle;
  string       textfont;
  double       fontsize;
  text_mode    textmode;

  void initialize();
  void scale_xcoord(double&);
  void scale_xcoord(double&,double&);
  void scale_ycoord(double&);
  void scale_ycoord(double&,double&);
  void scale_radius(double&);
  list_item grayvalue_item(color);
  void set_defaults();
  void set_old_values();
  void change_color(color);
  void change_rgb(color);
  void change_line_style(line_style);
  void new_font();

  static inline int round(double d);
  static inline bool exor(bool,bool);
  static inline double dist(double,double,double,double);
  static inline double dist(point,point);

  // operators for writing to file
  ps_file& operator<<(const char* s);
  ps_file& operator<<(double d);
  ps_file& operator<<(int i);  


public:
  // Constructors, destructors, initializers
  ps_file(double w, double h=PS_HEIGHT, string name="unnamed.ps");
  ps_file(string name="unnamed.ps");
  void init(double, double, double);
  void newpage();
  void clear() { newpage(); }
  void finish_old_page();
  void newfile(double w, double h=PS_HEIGHT, string name="unnamed.ps");
  void newfile(string name="unnamed.ps");
  void close();
  ~ps_file();

  // Setting parameters
  bool        set_draw_bb(bool);
  output_mode set_output_mode(output_mode);
  double      set_gray_value(color,double);
  color       set_color(color);
  color       set_fill_color(color);
  double      set_line_width(double);
  double      set_unit_line_width(double);
  point_style set_point_style(point_style);
  line_style  set_line_style(line_style);
  double      set_node_width(double);
  double      set_cross_width(double);
  double      set_arrow_length(double);
  double      set_arrow_angle(double);
  string      set_text_font(string);
  double      set_font_size(double);
  text_mode   set_text_mode(text_mode);

  // Reading parameters 
  bool        get_draw_bb();
  output_mode get_output_mode();
  double      get_gray_value(color);
  color       get_color();
  color       get_fill_color();
  double      get_line_width();
  double      get_unit_line_width();
  point_style get_point_style();
  line_style  get_line_style();
  double      get_node_width();
  double      get_cross_width();
  double      get_arrow_length();
  double      get_arrow_angle();
  string      get_text_font();
  double      get_font_size();
  text_mode   get_text_mode();

  double xmin() const { return X0; }
  double xmax() const { return X1; }
  double ymin() const { return Y0; }
  double ymax() const { return Y1; }

  // Drawing operations. I use the DEF_COLOR constant from the
  // x_window.h header file, as the ps_file::fg_col parameter is not a
  // static member (it may be different for several ps_file instances).

  // Drawing points
  void draw_point(double, double, color c=DEF_COLOR);
  void draw_point(point, color c=DEF_COLOR);
  void draw_pixel(double, double, color c=DEF_COLOR);
  void draw_pixel(point, color c=DEF_COLOR);

  // Drawing line segments
  void draw_segment(double, double, double, double, color c=DEF_COLOR);
  void draw_segment(point, point, color c=DEF_COLOR);
  void draw_segment(segment, color c=DEF_COLOR);

  void draw_ray(double, double, double, double, color c=DEF_COLOR);
  void draw_ray(const point&, const point&, color c=DEF_COLOR);
  void draw_ray(const segment&, color c=DEF_COLOR);
  void draw_ray(const ray&, color c=DEF_COLOR);

  // Drawing lines
  void draw_line(double, double, double, double, color c=DEF_COLOR);
  void draw_line(point, point, color c=DEF_COLOR);
  void draw_line(segment, color c=DEF_COLOR);
  void draw_line(line, color c=DEF_COLOR);
  void draw_hline(double, color c=DEF_COLOR);
  void draw_vline(double, color c=DEF_COLOR);
  void draw_arc(double, double, double, double, double, color c=DEF_COLOR);
  void draw_arc(point, point, double, color c=DEF_COLOR);
  void draw_arc(segment, double, color c=DEF_COLOR);

  // Drawing arrows
  void draw_arrow_head(point, double, color c=DEF_COLOR);
  void draw_arrow(double, double, double, double, color c=DEF_COLOR);
  void draw_arrow(point, point, color c=DEF_COLOR);
  void draw_arrow(segment, color c=DEF_COLOR);
  void draw_arc_arrow(double, double, double, double, double, color c=DEF_COLOR);
  void draw_arc_arrow(point, point, double, color c=DEF_COLOR);
  void draw_arc_arrow(segment, double, color c=DEF_COLOR);

  // Drawing circles
  void draw_circle(double, double, double, color c=DEF_COLOR);
  void draw_circle(point, double, color c=DEF_COLOR);
  void draw_circle(circle, color c=DEF_COLOR);
  void draw_ellipse(double, double, double, double, color c=DEF_COLOR);
  void draw_ellipse(point, double, double, color c=DEF_COLOR);

  // Drawing discs
  void draw_disc(double, double, double, color c=DEF_COLOR);
  void draw_disc(point, double, color c=DEF_COLOR);
  void draw_disc(circle, color c=DEF_COLOR);
  void draw_filled_ellipse(double, double, double, double, color c=DEF_COLOR);
  void draw_filled_ellipse(point, double, double, color c=DEF_COLOR);
 
  // Drawing polygons
  void draw_polygon(const list<point>&, color c=DEF_COLOR);
  void draw_polygon(polygon, color c=DEF_COLOR);
  void draw_filled_polygon(const list<point>&, color c=DEF_COLOR);
  void draw_filled_polygon(polygon, color c=DEF_COLOR);
  //new
  void draw_filled_polygon(gen_polygon, color c=DEF_COLOR);
  
  void draw_rectangle(double, double, double, double, color c=DEF_COLOR);
  void draw_rectangle(point, point, color c=DEF_COLOR);
  void draw_rectangle(rectangle, color c=DEF_COLOR); 
  void draw_box(double, double, double, double, color c=DEF_COLOR);
  void draw_box(point, point, color c=DEF_COLOR);
  void draw_box(rectangle, color c=DEF_COLOR);
  void draw_triangle(point, point, point, color c=DEF_COLOR);
  void draw_filled_triangle(point, point, point, color c=DEF_COLOR);
  // new polygons from graphwin ...
  void draw_roundrect(double x0, double y0, double x1, double y1, double rnd, color c=DEF_COLOR);
  void draw_filled_roundrect(double x0, double y0, double x1, double y1, double rnd, color c=DEF_COLOR);

  // Drawing functions
  void plot_xy(double, double, win_draw_func, color c=DEF_COLOR);
  void plot_yx(double, double, win_draw_func, color c=DEF_COLOR);

  // Drawing text
  void draw_text(double, double, string, color c=DEF_COLOR);
  void draw_text(point, string, color c=DEF_COLOR);
  void draw_ctext(double, double, string, color c=DEF_COLOR);
  void draw_ctext(point, string, color c=DEF_COLOR);

  // Drawing nodes
  void draw_node(double, double, color c=DEF_COLOR);
  void draw_node(point, color c=DEF_COLOR);
  void draw_filled_node(double, double, color c=DEF_COLOR);
  void draw_filled_node(point, color c=DEF_COLOR);
  void draw_text_node(double, double, string, color c=DEF_COLOR);
  void draw_text_node(point, string, color c=DEF_COLOR);
  void draw_int_node(double, double, int, color c=DEF_COLOR);
  void draw_int_node(point, int, color c=DEF_COLOR);

  // Drawing edges
  void draw_edge(double, double, double, double, color c=DEF_COLOR);
  void draw_edge(point, point, color c=DEF_COLOR);
  void draw_edge(segment, color c=DEF_COLOR);
  void draw_edge_arrow(double, double, double, double, color c=DEF_COLOR);
  void draw_edge_arrow(point, point, color c=DEF_COLOR);
  void draw_edge_arrow(segment, color c=DEF_COLOR);
  void draw_arc_edge(double, double, double, double, double, color c=DEF_COLOR);
  void draw_arc_edge(point, point, double, color c=DEF_COLOR);
  void draw_arc_edge(segment, double, color c=DEF_COLOR);
  void draw_arc_edge_arrow(double, double, double, double, double, color c=DEF_COLOR);
  void draw_arc_edge_arrow(point, point, double, color c=DEF_COLOR);
  void draw_arc_edge_arrow(segment, double, color c=DEF_COLOR);
  // new from graphwin
  point draw_arrow_head_poly(const point& q, double a, color c=DEF_COLOR);
  void draw_polygon_edge(const list<point>& P, int arrow, color c=DEF_COLOR);
  void draw_bezier_edge(const list<point>& P, int arrow, color c=DEF_COLOR);
  void draw_spline_edge(const list<point>& P, int arrow, color c=DEF_COLOR);
  void draw_circle_edge(point p, point q, point r, int arrow, color c=DEF_COLOR);
  
  //clipping operations from graphwin
  void clip_reset();
  void clip_ellipse(double x, double y, double r1, double r2);
  void clip_rectangle(double x0, double y0, double x1, double y1);
  void clip_polygon(const list<point>& P); 
  
  //pixmap support from graphwin
  void put_pixrect(char* pmap, double x, double y, double w1, double h1);
  void include_xpm(string fname, double x, double y, double w1, double h1);

  // overloaded output stream operators
  ps_file& operator << (point p)     {draw_point(p);   return *this; }
  ps_file& operator << (segment s)   {draw_segment(s); return *this; }
  ps_file& operator << (triangle t)  {draw_filled_triangle(t[1],t[2],t[3],get_fill_color()); draw_triangle(t[1],t[2],t[3]); return *this; }
  ps_file& operator << (line l)      {draw_line(l);    return *this; }
  ps_file& operator << (ray r)       {draw_ray(r);      return *this; }
  ps_file& operator << (circle C)    {draw_disc(C,get_fill_color()); draw_circle(C); return *this; }
  ps_file& operator << (polygon P)   {draw_filled_polygon(P,get_fill_color()); draw_polygon(P); return *this; }
  ps_file& operator << (rectangle R) {draw_box(R,get_fill_color()); draw_rectangle(R);  return *this; }

  ps_file& operator<< (gen_polygon p) {  
    list<polygon> lp=p.polygons();
    draw_filled_polygon(p, get_fill_color());
    
    polygon h;
    forall(h,lp){ 
      draw_polygon(h); 
    }
    
    return *this;
  }
  
  ps_file& operator<< (geo_graph G)
  {
    list<segment> slist;
    list<ray> rlist;
    list<line> llist;
    
    G.get_objects(slist,rlist,llist);

    segment s;
    forall(s,slist) (*this) << s;

    ray r;
    forall(r,rlist) (*this) << r;

    line l;
    forall(l,llist) (*this) << l;    
    
    return *this;  
  }

  ps_file& operator<<(rat_point p)
  {
    draw_point( p.to_point());
    return *this;
  }

  ps_file& operator<<(const rat_triangle t)
  {
    (*this) << t.to_float();
    return *this;
  }

  ps_file& operator<<(const rat_circle c)
  {
    (*this) << c.to_circle();
    return *this;
  }

  ps_file& operator<<(rat_segment s)
  { draw_segment( s.to_segment());
    return *this;
  }

  ps_file& operator<<(rat_line l)
  { draw_line(l.to_line());
    return *this;
  }

  ps_file& operator<<(rat_ray r)
  { draw_ray(r.to_ray()); return *this; }

  ps_file& operator<<(rat_polygon p)
  { (*this) << p.to_polygon();
    return *this;
  }

  ps_file& operator<<(rat_rectangle R)
  { (*this) << R.to_rectangle();
    return *this;
  }

  ps_file& operator<<(rat_gen_polygon p)
  { (*this) << p.to_gen_polygon();
    return *this;
  }
 


};  // End class ps_file

#if LEDA_ROOT_INCL_ID == 600149
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
