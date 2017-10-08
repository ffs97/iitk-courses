/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  d3_window_stream.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_D3_WINDOW_STREAM_H
#define LEDA_D3_WINDOW_STREAM_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600139
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/geo/d3_float_kernel.h>
#include <LEDA/geo/d3_simplex.h>
#include <LEDA/geo/triangle.h>
#include <LEDA/geo/d3_ray.h>
#include <LEDA/graphics/d3_window.h>
#include <LEDA/geo/triangle.h>
#include <LEDA/geo/rat_circle.h>
#include <LEDA/core/map.h>
#include <LEDA/core/h_array.h>
#include <LEDA/graph/graph.h>
#include <LEDA/graphics/bitmaps/button21.h>

LEDA_BEGIN_NAMESPACE



static unsigned char* d3_window_stream_display_bits[] = {
  left_21_bits,
  right_21_bits,
  up_21_bits,
  down_21_bits,
  circle_21_bits,
  point_21_bits,
  stop_21_bits,
  ps_21_bits,
  rect_21_bits,   // select
  exit_21_bits
};

static void d3_window_stream_copy_graph(GRAPH<d3_point,int>& H, const GRAPH<d3_point,int>& G, \
                          node_map<node>& V, edge_map<edge>& E) 
// H target, G source, V,E are mapping the nodes/edges of G to H
{ 
  V.init(G);
  E.init(G);

  node_array<node> v_in_H(G); 
  edge e;
  node v;
  forall_nodes(v,G) 
  { node u = H.new_node(G[v]);
    v_in_H[v] = u;
    V[v] = u;
   }
  
  forall_nodes(v,G)
  { node u = v_in_H[v];
    forall_adj_edges(e,v) 
    { edge x = H.new_edge(u,v_in_H[target(e)],0);
      E[e] = x;
     }
   }
   
  // set reversal infos ... 
  forall_edges(e,G) {
    edge rev = G.reversal(e);
    edge eh = E[e];
    
    if (H.reversal(eh) == NULL){
      H.set_reversal(eh,E[rev]);
    }
  }
}

static void d3_window_stream_circle_segments(list<segment>& LS, circle C, int n)
{
  list<rat_point> L;
  point p = C.point1();
  point q = C.point2();
  point r = C.point3();
  
  rat_point rp(p), rq(q), rr(r);
  rat_circle R(rp,rq,rr);
  
  double d = (2*LEDA_PI)/n;
  double eps = 0.001;
  double a = 0;
  
  for(int i=0; i < n; i++)
    { 
      rat_point pp = R.point_on_circle(a,eps);
      L.append(pp);
      a += d;
    }
    
  // now generate the segments desribing the circle ...
  list_item lit = L.first();
  
  while(lit && L.succ(lit))
  {
    LS.append(segment(L[lit].to_point(), L[L.succ(lit)].to_point()));
    lit = L.succ(lit);
  }
  LS.append(segment(L.tail().to_point(), L.head().to_point()));  
}

static list<segment> zoom_segments(const list<segment>& LS, const point& pm, double zf)
{
 segment siter;
 list<segment> rt;
 forall(siter,LS){
   vector v1 = (siter.source() - pm) * zf;
   vector v2 = (siter.target() - pm) * zf;
   rt.append(segment(point(pm+v1),point(pm+v2)));
 }
 return rt;
}

static void generate_sphere_graph(const d3_sphere& Sph, GRAPH<d3_point,int>& G)
{
  list<d3_point> pts;
  
  d3_point ct = Sph.center();
  double r  = Sph.radius();
  double ymin = Sph.center().ycoord() - 0.99*r;
  double ymax = Sph.center().ycoord() + 0.99*r;
  double yakt = ymin, ystep = 0.15*r;
  double xmin,xmax;
  point ct2(ct.xcoord(), ct.ycoord());
  circle C(ct2,r);
  circle C2;
  d3_point iter;
  list_item lit;
  list<d3_point> pts1,pts2;
  list<node> NL,NL2,NL_first;
  edge e1,e2;
  list<segment> Lseg;
  d3_window_stream_circle_segments(Lseg, circle(ct2, 1.0), 30);
  
  
  while(yakt <= ymax){
    segment S(point(ct.xcoord()-r-100.0,yakt), point(ct.xcoord()+r+100.0,yakt));
    list<point> res = C.intersection(S);
    point p1 = res.pop(); point p2 = res.pop();
    if (p1.xcoord() < p2.xcoord()) { xmin = p1.xcoord(); xmax =p2.xcoord(); }
    else { xmin = p2.xcoord(); xmax =p1.xcoord(); }
    C2 = circle(ct2,ct.xcoord()-xmin); 
    
    list<segment> LS = zoom_segments(Lseg,ct2,ct.xcoord()-xmin);
    segment siter;
    forall(siter,LS){
      point akt = siter.source();
      pts.append(d3_point(akt.xcoord(),yakt,akt.ycoord()));
    }

    NL2.clear();
    forall(iter,pts) {
      node nn = G.new_node(iter);
      NL2.append(nn);
    }
    
    list_item prev = NL2.first();
    lit = NL2.cyclic_succ(prev);
    
    for(;lit != NL2.first(); prev = lit, lit = NL2.cyclic_succ(lit)){
       // insert edges and set reveral information
       e1 = G.new_edge(NL[prev],NL[lit]);
       e2 = G.new_edge(NL[lit],NL[prev]);
       G.set_reversal(e1,e2);
    }
    
    e1 = G.new_edge(NL2[NL2.first()], NL2[NL2.last()]);
    e2 = G.new_edge(NL2[NL2.last()], NL2[NL2.first()]);    
    G.set_reversal(e1,e2);
    
    pts.clear();

    if (! NL.empty()){
      //cout << NL.size() << " " << NL2.size() << "\n"; cout.flush();
      list_item lit2 = NL2.first();
      forall_items(lit,NL){
        e1 = G.new_edge(NL[lit], NL2[lit2]);
        e2 = G.new_edge(NL2[lit2] ,NL[lit]);	
	G.set_reversal(e1,e2);
	lit2 = NL2.cyclic_succ(lit2);
      }
    }
    else { NL_first = NL2; }

    NL = NL2; 
        
    yakt = yakt + ystep;
  }
  node NH1 = G.new_node(d3_point(ct.xcoord(),ct.ycoord()-r,ct.zcoord()));
  node NH2 = G.new_node(d3_point(ct.xcoord(),ct.ycoord()+r,ct.zcoord()));
  node niter;
  forall(niter,NL_first) {
       e1 = G.new_edge(niter, NH1);
       e2 = G.new_edge(NH1 ,niter);	
       G.set_reversal(e1,e2);    
  }
  forall(niter,NL2) {
       e1 = G.new_edge(niter, NH2);
       e2 = G.new_edge(NH2 ,niter);	
       G.set_reversal(e1,e2);    
  }
}

class /*__exportC*/ d3_window_item_buffer {
public:
 list<edge> LE;
 list<node> LN;

 d3_window_item_buffer() { }
 d3_window_item_buffer(edge e) { LE.append(e); }
 d3_window_item_buffer(node v) { LN.append(v); }
 d3_window_item_buffer(const list<node>& l1,const list<edge>& l2) { LN = l1; LE = l2; }
 d3_window_item_buffer(const list<edge>& l1) { LE=l1; }
 d3_window_item_buffer(const list<node>& l1) { LN=l1; }
};


typedef d3_window_item_buffer* d3_window_item;

int compare(const d3_window_item& i1, const d3_window_item& i2)
{
  if (i1 < i2) return -1;
  if (i1 ==i2) return 0;
  return 1;
}

LEDA_END_NAMESPACE

#include <LEDA/core/set.h>

LEDA_BEGIN_NAMESPACE

/*{\Manpage {d3_window_stream} {} {Three-dimensional output}}*/

class /*__exportC*/ d3_window_stream {
  /*{\Mdefinition
    An instance of data type |\Mname| is a LEDA window, that
    can be used easily for visualization of three-dimensional
    geometric objects.
    It buffers elements (|d3_window_items|) describing the three-dimensional objects
    that have to be displayed. 
    It is also possible to visualize two-dimensional
    geometric objects. Then a |z|- coordinate of |0| is used for these
    objects.}*/
  
 GRAPH<d3_point,int> G;       // graph stores all elements; must be bidirected with reversal informations set
 GRAPH<d3_point,int> SPHGR;   // sphere ... (used for creating other spheres ...)
 window* MyWin;                // pointer to the drawing window
 bool win_alloc;               // has the d3 window allocated its own window or not
 d3_window* win;               // pointer to d3 viewer
 color clr;                    // current color
 
 color selection_color;        // color of selected objects ...
 
 // color maps for vertices and edges 
 h_array<node,color> node_color_map;
 h_array<edge,color> edge_color_map;
 h_array<edge,bool>  arrow_map;
 map<void*,d3_window_item> my_item;
 
 // draw graph with nodes ?
 bool draw_graph;
 
 // lists of selected objects ...
 list<node> IN;
 list<edge> IE;
 
 // list of current d3_window items ...
 list<d3_window_item_buffer> ID3;
 
 void construct(float w, float h, const char* label);
 
 d3_window_item add_d3_window_item(edge e)
 { 
   list_item it = ID3.append(d3_window_item_buffer(e));
   d3_window_item d3i = &(ID3[it]);
   my_item[(void*)e] = d3i;
   return d3i;
 }
 
 d3_window_item add_d3_window_item(node v)
 {
   list_item it = ID3.append(d3_window_item_buffer(v)); 
   d3_window_item d3i = &(ID3[it]);
   my_item[(void*)v] = d3i;
   return d3i;
 }
 
 d3_window_item add_d3_window_item(const list<node>& l1, const list<edge>& l2)
 { 
   list_item it = ID3.append(d3_window_item_buffer(l1,l2));  
   d3_window_item d3i = &(ID3[it]);
   node v;
   edge e;
   forall(v,l1) my_item[(void*)v] = d3i;
   forall(e,l2) my_item[(void*)e] = d3i;
   return d3i;
 }
 
 d3_window_item add_d3_window_item(const list<edge>& l1)
 {
   list_item it = ID3.append(d3_window_item_buffer(l1));   
   d3_window_item d3i = &(ID3[it]);
   edge e;
   forall(e,l1) my_item[(void*)e] = d3i;
   return d3i;
 }
 
 d3_window_item add_d3_window_item(const list<node>& l1)
 {
   list_item it = ID3.append(d3_window_item_buffer(l1)); 
   d3_window_item d3i = &(ID3[it]);
   node v;
   forall(v,l1) my_item[(void*)v] = d3i;
   return d3i;  
 }
 
 void set_colors(const list<node>& LN, color c)
 {
   node v_iter;
   forall(v_iter,LN) node_color_map[v_iter] = c;
 }
 
 void set_colors(const list<edge>& LE, color c)
 {
   edge e_iter;
   forall(e_iter,LE) edge_color_map[e_iter] = c;
 } 
 
 void select_in_graph(double x0, double y0, double x1, double y1);
 
public: 
 double get_window_factor() const;
 
  /*{\Mcreation DW}*/
  
 d3_window_stream(window& W);
 /*{\Mcreate creates an instance |\Mvar| of type |\Mname|. The window |W| is used
     for visualization.}*/
 
 d3_window_stream();
 /*{\Mcreate creates an instance |\Mvar| of type |\Mname|.}*/
 
 d3_window_stream(const char* label);
 /*{\Mcreate creates an instance |\Mvar| of type |\Mname|. The drawing window will get the
 label |label|.}*/ 
 
 d3_window_stream(float w, float h);
 /*{\Mcreate creates an instance |\Mvar| of type |\Mname|. The drawing window will be created
 with width |w| and height |h|.}*/ 

 d3_window_stream(float w, float h,const char* label);
 /*{\Mcreate creates an instance |\Mvar| of type |\Mname|. The drawing window will get the
 label |label| and will be created with width |w| and height |h|.}*/ 
 
 ~d3_window_stream();

  /*{\Moperations }*/
 
 void clear();
 /*{\Mopl deletes all elements currently buffered in |\Mvar|. This operation deletes as well the 
 selected |d3_window_items| of |\Mvar|.}*/ 
 
 bool show();
 /*{\Mopl starts the display of the elements describing three-dimensional objects 
          currently buffered in |\Mvar|. The display is terminated by pressing the |exit| button 
	  or the right mouse button. In the first case |false| is returned, in the second case 
	  |true| is returned. }*/ 
 
 color set_color(color c);
 /*{\Mopl sets the element color of |\Mvar| to |c| and returns the previous color.}*/
 
 color get_selection_color() const;
 /*{\Mopl returns the color of selected elements of |\Mvar|.}*/
 
 color set_selection_color(color c);
 /*{\Mopl sets the color of selected elements of |\Mvar| to |c| and returns the 
    previous color.}*/
 
 color get_color() const;
 /*{\Mopl returns the element color of |\Mvar|.}*/
 
 void set_color(d3_window_item it, color c);
 /*{\Mopl assigns color |c| to element |it|. }*/
 
 bool get_color(d3_window_item it, color& c) const;
 /*{\Mopl if |it| is an element in |\Mvar|, its color is assigned to |c|, and the function
          returns |true|. Otherwise |false| is returned. }*/

 void set_arrow(edge eit, bool b);
 
 bool set_draw_graph(bool b);
 
 window& get_window();
 /*{\Mopl returns a reference to the drawing window of |\Mvar|.}*/

  /*{\Mtext
    \medskip
    The following operations insert elements (|d3_window_items|) describing a
    geometric object in |\Mvar|. The operations get as the first parameter the geometric
    object that has to be inserted. The second  returns the new created |d3_window_item|.}*/
  
 void insert_d3_segment(const d3_segment& s, list<node>& LN, list<edge>& LE);
 void insert_d3_ray(const d3_ray& r, list<node>& LN, list<edge>& LE);
 void insert_segment(const segment& s, list<node>& LN, list<edge>& LE); 
 
 // 3-d types
 void insert_d3_point(const d3_point& p, d3_window_item& n);
 /*{\Mopl inserts |p| in |\Mvar|.}*/
 
 void insert_d3_segment(const d3_segment& s, d3_window_item& n);
 /*{\Mopl inserts |s| in |\Mvar|.}*/
 
 void insert_d3_ray(const d3_ray& r, d3_window_item& n);
 /*{\Mopl inserts |r| in |\Mvar|.}*/
 
 void insert_d3_line(const d3_line& l, d3_window_item& n);
 /*{\Mopl inserts |l| in |\Mvar|.}*/
 
 void insert_d3_plane(const d3_plane& p, d3_window_item& n);
 /*{\Mopl inserts |p| in |\Mvar|.}*/
 
 void insert_d3_sphere(const d3_sphere& s, d3_window_item& n);
 /*{\Mopl inserts |s| in |\Mvar|.}*/
 
 void insert_d3_triangle(const d3_triangle& t, d3_window_item& n);
 /*{\Mopl inserts |t| in |\Mvar|.}*/
 
 void insert_d3_simplex(const d3_simplex& s, d3_window_item& n); 
 /*{\Mopl inserts |s| in |\Mvar|.}*/

 void insert_box(const d3_point& pllf, const d3_point& purb, d3_window_item& n);   
 /*{\Mopl inserts a iso-rectangular box with lower left front corner |pllf| and
          upper right back corner |purb| in |\Mvar|.}*/
 
 //.2-d types
 void insert_point(const point& p, d3_window_item& n);
 /*{\Mopl inserts |p| in |\Mvar|.}*/
 
 void insert_segment(const segment& s, d3_window_item& n);
 /*{\Mopl inserts |s| in |\Mvar|.}*/
 
 void insert_ray(const ray& r, d3_window_item& n);
 /*{\Mopl inserts |r| in |\Mvar|.}*/
 
 void insert_line(const line& l, d3_window_item& n);
 /*{\Mopl inserts |l| in |\Mvar|.}*/
 
 void insert_circle(const circle& c, d3_window_item& n);
 /*{\Mopl inserts |c| in |\Mvar|.}*/
 
 void insert_triangle(const triangle& t, d3_window_item& n);
 /*{\Mopl inserts |t| in |\Mvar|.}*/
 
 void insert_rectangle(const rectangle& r, d3_window_item& n);
 /*{\Mopl inserts |r| in |\Mvar|.}*/
 
 void insert_polygon(const polygon& p, d3_window_item& n); 
 /*{\Mopl inserts |p| in |\Mvar|.}*/
 
 void insert_gen_polygon(const gen_polygon& p, d3_window_item& n);   
 /*{\Mopl inserts |p| in |\Mvar|.}*/
 

  /*{\Mtext
    \medskip
  There are output operators available for |\Mvar| for the following types: \\
  |d3_point|, |d3_segment|, |d3_ray|, |d3_line|, |d3_plane|, |d3_sphere|, |d3_triangle|,
  |d3_simplex|, |point|, |circle|, |ray|, |line|, |segment|, |triangle|, |rectangle|,
  |polygon| and |gen_polygon|.

  For the rational kernel types use the conversion to the floating
  point kernel.
  All these operators insert elements (|d3_window_items|)
  in |\Mvar| describing the respective geometric object.   
 }*/
   
 // 3-d types
 d3_window_stream& operator<<(const d3_point& p);
 d3_window_stream& operator<<(const d3_segment& s);
 d3_window_stream& operator<<(const d3_ray& r);
 d3_window_stream& operator<<(const d3_line& l);
 d3_window_stream& operator<<(const d3_plane& C);
 d3_window_stream& operator<<(const d3_sphere& P);
 d3_window_stream& operator<<(const d3_simplex& P);
 d3_window_stream& operator<<(const d3_triangle& T);

 // 2-d types
 d3_window_stream& operator<<(const point& p);
 d3_window_stream& operator<<(const circle& c);
 d3_window_stream& operator<<(const ray& r); 
 d3_window_stream& operator<<(const line& l);  
 d3_window_stream& operator<<(const segment& s);
 d3_window_stream& operator<<(const triangle& t);
 d3_window_stream& operator<<(const rectangle& r);
 d3_window_stream& operator<<(const polygon& p);
 d3_window_stream& operator<<(const gen_polygon& gp);
 
 // graph (reversal information has to be set for all edges)
 d3_window_stream& operator<<(const GRAPH<d3_point,int>& G);
 
 
  /*{\Mtext
    \medskip 
    The following 2 operations insert |d3_window_items| describing parametrized graph in |\Mvar|.
    The edges of the graph will be painted as segments. Note that the reversal information for every edge
    of the graph has to be set.
 }*/
  
 void insert_graph(const GRAPH<d3_point,int>& G,d3_window_item& n);
 /*{\Mopl inserts graph |G| into |\Mvar|. }*/ 
 
 void insert_graph(const GRAPH<d3_point,int>& G,const map<node,color>& mnc, const map<edge,color>& mec, d3_window_item& n);
 /*{\Mopl inserts graph |G| into |\Mvar|. The nodes |v| of |G| in the domain of |mnc| and the edges |e| of |G| in
          the domain of |mec| will get color |mnc(v) (mec(e))| in |\Mvar|. }*/

 void get_selected_items(list<d3_window_item>& L) const;
 /*{\Mopl returns in |L| a list of the currently selected |d3_window_items| of |\Mvar|.}*/
};

// sources ...

double d3_window_stream::get_window_factor() const
{
 double w = MyWin->width();
 double h = MyWin->height();
 
 //cout << "w/h:" << w << " " << h << "\n";
 //cout << MyWin->pix_to_real(w) << "\n";
 
 if (w > h) return MyWin->pix_to_real(w/40); else return MyWin->pix_to_real(h/40);
}

d3_window_stream::d3_window_stream(window& W)
{
  MyWin = &W;
  win_alloc = false;
  win = NULL;
  clr = black;
  generate_sphere_graph(d3_sphere(d3_point(100,100,100),d3_point(-100,100,100),d3_point(100,-100,100),d3_point(-100,-100,-100)),SPHGR);
  selection_color = yellow;
  draw_graph = false;
}

void d3_window_stream::construct(float w, float h, const char* label)
{
  MyWin = new window(w,h,label);
  MyWin->init(-1000,1000,-1000);
  win_alloc = true;
  win = NULL;
  clr = black;
  int bt;
  generate_sphere_graph(d3_sphere(d3_point(100,100,100),d3_point(-100,100,100),d3_point(100,-100,100),d3_point(-100,-100,-100)),SPHGR);
  for(int i=0;i<10;i++) {
    bt = MyWin->button(21,21,d3_window_stream_display_bits[i],string(""));  
  }
  selection_color = yellow;
  draw_graph = false;
}

d3_window_stream::d3_window_stream()
{ string label("3d Window Stream");
  construct(500,500,label);
}

d3_window_stream::d3_window_stream(const char* label)
{ construct(500,500,label); }

d3_window_stream::d3_window_stream(float w, float h)
{ string label("3d Window Stream");
  construct(w,h,label);
}

d3_window_stream::d3_window_stream(float w, float h, const char* label)
{ construct(w,h,label); }
 
d3_window_stream::~d3_window_stream()
{
  if (win_alloc) delete MyWin;
  if (win != NULL) delete win;
}
 
void d3_window_stream::clear()
{
  node_color_map.clear();
  edge_color_map.clear();
  arrow_map.clear();
  G.clear();
  ID3.clear();
  
  // selection buffers ...
  IE.clear(); IN.clear();
  
  if (win != NULL) { delete win; win=NULL; }
}

void d3_window_stream::get_selected_items(list<d3_window_item>& L) const
{
  d3_window_item it;
  map<d3_window_item,bool> MB;
  
  node viter;
  forall(viter,IN) {
    it = my_item[(void*)viter];
    if (! MB.defined(it)) { L.append(it); MB[it]=true; }
  }
  
  edge eiter;
  forall(eiter,IE) {
    it = my_item[(void*)viter];
    if (! MB.defined(it)) { L.append(it); MB[it]=true; }
  }
}

void d3_window_stream::select_in_graph(double x0, double y0, double x1, double y1)
{
 // first get the current d2 positions of the edges and nodes ...
 IN.clear();
 IE.clear();
 node_array<point> d2_pos(G);
 
 if (win) {
   win->get_d2_position(d2_pos);
 }
 else return; // no d3 window allocated

 double xmin, xmax;
 double ymin, ymax, x, y;
 
 if (x0 < x1) { xmin=x0; xmax=x1; } else { xmin=x1; xmax=x0; }
 if (y0 < y1) { ymin=y0; ymax=y1; } else { ymin=y1; ymax=y0; }
 
 set<d3_window_item> Sd3;
 
 node v;
 forall_nodes(v,G){ // intersection ???
  x = d2_pos[v].xcoord(); y = d2_pos[v].ycoord();
  if ( (x>xmin) && (x<xmax) && (y>ymin) && (y<ymax) )  Sd3.insert(my_item[(void*)v]); 
 }
 
 edge e;
 rectangle rec(xmin, ymin, xmax, ymax);
 segment inter;
 
 forall_edges(e,G){ // intersection ???
  segment s(d2_pos[G.source(e)], d2_pos[G.target(e)] );
  bool b = rec.clip(s,inter);
  if (b) Sd3.insert(my_item[(void*)e]); 
 } 
 
 d3_window_item it;
 forall(it,Sd3){
   forall(v,it->LN) IN.append(v);
   forall(e,it->LE) IE.append(e);
 }
}

bool d3_window_stream::show()
{
  // clear old contents of selection buffers
  IE.clear(); IN.clear();

  int but;
  if (win != NULL) delete win;
  win = new d3_window(*MyWin,G);
  
  win->set_node_width(4);
  win->set_elim(false);
  win->set_solid(false);
  win->set_speed(10);
  win->set_message("");
  win->set_draw_graph(draw_graph);
  
  // setting colors ...
  node v;
  edge e;

  if (win_alloc || (! MyWin->is_open())) MyWin->open();
    
  string msg("\\bf\\blue ~~~~~left:  \\black zoom up \
              \\bf\\blue ~~~~~middle:\\black zoom down \
	      \\bf\\blue ~~~~~right: \\black done");

  win->set_message(msg);

  node_array<vector> pos(G);
  forall_nodes(v,G) { pos[v] = G[v].to_vector(); }
  win->init(pos);
  win->node_width = 4;  

  forall_defined(v,node_color_map) win->set_color(v,node_color_map[v]);
  forall_defined(e,edge_color_map) {
    win->set_color(e,edge_color_map[e]);
  }
  
  //setting arrow information ...
  forall_defined(e,arrow_map) win->set_arrow(e,arrow_map[e]);
  
  win->draw();
  
  double XF = get_window_factor()*2.0;
  double YF = get_window_factor()*2.0;
  
  for(;;) {
   do {
    but = win->move();
   }
   while ((but != MOUSE_BUTTON(3)) && ((but < 0) || (but>10)));
 
   if ((but == MOUSE_BUTTON(3)) || (but == 9)) {
     if (but == MOUSE_BUTTON(3)) return true;
     else return false;
   }
   
   switch(but)
   {
     case 0: // left
     {
       win->move_graph(-XF,0);
       win->draw();
       break;
     }
     case 1: // right
     {
       win->move_graph(XF,0);
       win->draw();
       break;
     }
     case 2: // up
     {
       win->move_graph(0,YF);
       win->draw();
       break;
     }
     case 3: // down
     {
       win->move_graph(0,-YF);
       win->draw();
       break;
     }
     case 4: // zoom +
     {
       double zf = win->get_zoom_factor();
       zf = zf * 1.15;
       win->set_zoom_factor(zf);
       win->draw();
       break;
     }
     case 5: // zoom -
     {
       double zf = win->get_zoom_factor();
       zf = zf * 0.85;
       win->set_zoom_factor(zf);
       win->draw();     
       break;
     }
     case 6: // stop
     {
       win->set_move(! win->get_move());
       break;
     }
     case 7: // output ps
     {
       MyWin->screenshot(string("screenshot.ps"),true); 
       break;
     }
     case 8: // select ...
     {
       point p0;
       MyWin->read_mouse(p0);
       double x0 = p0.xcoord(), y0 = p0.ycoord();
       int event;
       double x1,y1;
       char* buf = MyWin->get_window_pixrect();
       MyWin->start_buffering();
       int lold = MyWin->set_line_width(1);
  
      do { 
       int val;
       unsigned long t1;
       event = MyWin->read_event(val,x1,y1,t1);
       MyWin->put_pixrect(buf);
       MyWin->draw_rectangle(x0, y0, x1, y1); 
       MyWin->flush_buffer();
      }
      while(event != button_release_event);
  
      MyWin->put_pixrect(buf);  
      MyWin->flush_buffer(); 
      MyWin->del_pixrect(buf); 
      MyWin->stop_buffering();
      
      select_in_graph(x0,y0,x1,y1);
      
      MyWin->set_line_width(lold);
      // set colors ...
      forall_defined(v,node_color_map) win->set_color(v,node_color_map[v]);
      forall_defined(e,edge_color_map) {
        win->set_color(e,edge_color_map[e]);
      }
      
      forall(v,IN) win->set_color(v,selection_color);
      forall(e,IE) win->set_color(e,selection_color);
      break;
    }
   }
  }
}

color d3_window_stream::set_color(color c)
{ color old = clr;
  clr = c;
  return old;
}


color d3_window_stream::get_color() const
{ return clr; }

color d3_window_stream::set_selection_color(color c)
{ color old = selection_color;
  selection_color = c;
  return old;
}

color d3_window_stream::get_selection_color() const
{ return selection_color; }

void d3_window_stream::set_color(d3_window_item it, color c)
{
  node vit;
  edge eit;
  
  forall(vit, it->LN) node_color_map[vit] = c;
  forall(eit, it->LE) edge_color_map[eit] = c;
}
 
bool d3_window_stream::get_color(d3_window_item it, color& c) const
{
 if ((it->LE).empty() && (it->LN).empty()) return false;
 
 if ((it->LE).empty()) {
  node vit = (it->LN).head();
  if (! node_color_map.defined(vit)) return false;
 
  c = node_color_map[vit];
  return true;
 }
  edge eit = (it->LE).head();
  if (! edge_color_map.defined(eit)) return false;
 
  c = edge_color_map[eit];
  return true; 
}


void d3_window_stream::set_arrow(edge eit, bool b)
{
  arrow_map[eit] = b;
}

bool d3_window_stream::set_draw_graph(bool b)
{
  bool prev = draw_graph;
  draw_graph = b;
  return prev;
} 

window& d3_window_stream::get_window()
{
  return *MyWin;
}

// functions for inserting geometric objects ...

// ---------------------------------------------------------------------------------------
// old style ...
// ---------------------------------------------------------------------------------------

void d3_window_stream::insert_d3_segment(const d3_segment& s, list<node>& LN2, list<edge>& LE2)
{
   list<node> LN; list<edge> LE;
   node v1 = G.new_node(s.source());
   node v2 = G.new_node(s.target());   
   edge e1 = G.new_edge(v1,v2);
   edge e2 = G.new_edge(v2,v1);
   G.set_reversal(e1,e2); 
   LN.append(v1); LN.append(v2);
   LE.append(e1); LE.append(e2);
   set_colors(LN,clr); set_colors(LE,clr);
   LN2.conc(LN); LE2.conc(LE);
}

void d3_window_stream::insert_d3_ray(const d3_ray& r, list<node>& LN2, list<edge>& LE2)
{
   list<node> LN; list<edge> LE;
   double fc = get_window_factor();
   double fc2 = fc*40;
   
   vector v = r.point2() - r.point1();
   vector vec2 = v;
   double l1 = vec2.length();
   vec2 = vec2 * (fc2/l1);   
   
   d3_point p1=r.source(), p2=p1 + vec2;
   node v1 = G.new_node(p1);
   node v2 = G.new_node(p2);   
   edge e1 = G.new_edge(v1,v2);
   edge e2 = G.new_edge(v2,v1);
   G.set_reversal(e1,e2);   

   set_arrow(e1,true);
   
   LN.append(v1); LN.append(v2);
   LE.append(e1); LE.append(e2);

   // setting colors
   set_colors(LN, clr); set_colors(LE, clr);
   LN2.conc(LN); LE2.conc(LE);
}

void d3_window_stream::insert_segment(const segment& s, list<node>& LN2, list<edge>& LE2)
{
   list<node> LN; list<edge> LE;
   node v1 = G.new_node(d3_point(s.source().xcoord(),s.source().ycoord(),0));
   node v2 = G.new_node(d3_point(s.target().xcoord(),s.target().ycoord(),0));   
   edge e1 = G.new_edge(v1,v2);
   edge e2 = G.new_edge(v2,v1);
   G.set_reversal(e1,e2);
   LN.append(v1); LN.append(v2); LE.append(e1); LE.append(e2);
   set_colors(LN,clr); set_colors(LE,clr);
   LN2.conc(LN); LE2.conc(LE);
}

// ---------------------------------------------------------------------------------------
// new style ...
// ---------------------------------------------------------------------------------------

void d3_window_stream::insert_d3_point(const d3_point& p, d3_window_item& n)
{
   node v = G.new_node(p);
   // set color
   node_color_map[v] = clr;
   
   n = add_d3_window_item(v);
}

void d3_window_stream::insert_d3_segment(const d3_segment& s, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   node v1 = G.new_node(s.source());
   node v2 = G.new_node(s.target());   
   edge e1 = G.new_edge(v1,v2);
   edge e2 = G.new_edge(v2,v1);
   G.set_reversal(e1,e2);
 
   LN.append(v1); LN.append(v2);
   LE.append(e1); LE.append(e2);
   set_colors(LN,clr); set_colors(LE,clr);
   
   n = add_d3_window_item(LN, LE);
}

void d3_window_stream::insert_d3_ray(const d3_ray& r, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   double fc = get_window_factor();
   double fc2 = fc*40;
   
   vector v = r.point2() - r.point1();
   vector vec2 = v;
   double l1 = vec2.length();
   vec2 = vec2 * (fc2/l1);   
   
   d3_point p1=r.source(), p2=p1 + vec2;
   node v1 = G.new_node(p1);
   node v2 = G.new_node(p2);   
   edge e1 = G.new_edge(v1,v2);
   edge e2 = G.new_edge(v2,v1);
   G.set_reversal(e1,e2);   

   set_arrow(e1,true);
   
   LN.append(v1); LN.append(v2);
   LE.append(e1); LE.append(e2);
 
   // setting colors
   set_colors(LN, clr); set_colors(LE, clr);
   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_d3_line(const d3_line& l, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   double fc = get_window_factor();
   double fc2 = fc*40;
   
   d3_point p1, p2, m;
   m = midpoint(l.point1(), l.point2());
   vector v = l.point1() - l.point2();
   vector vec2 = v * 5;
   double l1 = vec2.length();

   vec2 = vec2 * (fc2/l1);
   
   p1=m+vec2; p2=m-vec2;
      
   node v1 = G.new_node(p1);
   node v2 = G.new_node(p2);   
   edge e1 = G.new_edge(v1,v2);
   edge e2 = G.new_edge(v2,v1);
   G.set_reversal(e1,e2);

   LN.append(v1); LN.append(v2); 
   LE.append(e1); LE.append(e2); 
   
   set_arrow(e1,true);
   set_arrow(e2,true);
   
   // setting colors
   set_colors(LN, clr); set_colors(LE, clr);
   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_d3_plane(const d3_plane& C, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   d3_point p1,p2,p3,m,mh;
   mh = midpoint(C.point1(), C.point2());   
   m  = midpoint(C.point3(), mh);
   vector vec  = m - C.point1();
   vector vec2 = m - C.point2();
   vector vec3 = m - C.point3();
   
   vec= vec * 10; vec2 = vec2 * 10; vec3 = vec3 * 10;
   p1= m-vec; p2 = m-vec2; p3 = m-vec3;      

   node v1 = G.new_node(p1);
   node v2 = G.new_node(p2); 
   node v3 = G.new_node(p3);   
   edge e1 = G.new_edge(v1,v2);
   edge e2 = G.new_edge(v2,v1);
   G.set_reversal(e1,e2);
   edge e3 = G.new_edge(v1,v3);
   edge e4 = G.new_edge(v3,v1);
   G.set_reversal(e3,e4);
   edge e5 = G.new_edge(v3,v2);
   edge e6 = G.new_edge(v2,v3);
   G.set_reversal(e5,e6);  
   
   LN.append(v1); LN.append(v2); LN.append(v3);
   LE.append(e1); LE.append(e2); LE.append(e3); LE.append(e4); LE.append(e5); LE.append(e6);

   // setting node colors
   set_colors(LN,clr); set_colors(LE,clr);
   
   d3_ray r1(m, C.point1());
   d3_ray r2(m, C.point2());
   d3_ray r3(m, C.point3());

   insert_d3_ray(r1, LN, LE); insert_d3_ray(r2, LN, LE); insert_d3_ray(r3, LN, LE); 
   
   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_d3_sphere(const d3_sphere& S, d3_window_item& n)
{
   list<edge> LE;
   list<node> LN;
   
   vector v;
   d3_point orig(0,0,0);
   double scale;
   node piter;
 
   d3_point ct = S.center();
   double rd = S.radius();
    
   //move vector origin->center
   v = ct - orig;
   scale = rd/173.2;
   
   GRAPH<d3_point,int> G2;
   G2=SPHGR;
    
   forall_nodes(piter,G2) {
      d3_point hp(G2[piter].xcoord()*scale, G2[piter].ycoord()*scale, G2[piter].zcoord()*scale);
      G2[piter]= hp + v;
      node_color_map[piter] = clr;
      LN.append(piter);
   }
   edge eiter;
   forall_edges(eiter,G2){
      edge_color_map[eiter] = clr;
      LE.append(eiter);
   } 
   G.join(G2);
   
   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_d3_triangle(const d3_triangle& t, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   node v1 = G.new_node(t.point1());
   node v2 = G.new_node(t.point2());  
   node v3 = G.new_node(t.point3());
      
   edge e1 = G.new_edge(v1,v2);
   edge e2 = G.new_edge(v2,v1);
   G.set_reversal(e1,e2);
   
   edge e3 = G.new_edge(v1,v3);
   edge e4 = G.new_edge(v3,v1);
   G.set_reversal(e3,e4);
   
   edge e5 = G.new_edge(v2,v3);
   edge e6 = G.new_edge(v3,v2);
   G.set_reversal(e5,e6);
   
   LN.append(v1); LN.append(v2); LN.append(v3); 
   LE.append(e1); LE.append(e2); LE.append(e3); LE.append(e4); LE.append(e5); LE.append(e6);
   
   set_colors(LN,clr); set_colors(LE,clr); 
   n = add_d3_window_item(LN,LE);    
}

void d3_window_stream::insert_d3_simplex(const d3_simplex& P, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   node v1 = G.new_node(P.point1());
   node v2 = G.new_node(P.point2());  
   node v3 = G.new_node(P.point3());
   node v4 = G.new_node(P.point4()); 
      
   edge e1 = G.new_edge(v1,v2);
   edge e2 = G.new_edge(v2,v1);
   G.set_reversal(e1,e2);
   
   edge e3 = G.new_edge(v1,v3);
   edge e4 = G.new_edge(v3,v1);
   G.set_reversal(e3,e4);

   edge e5 = G.new_edge(v1,v4);
   edge e6 = G.new_edge(v4,v1);
   G.set_reversal(e5,e6);   
   
   edge e7 = G.new_edge(v2,v3);
   edge e8 = G.new_edge(v3,v2);
   G.set_reversal(e7,e8);
   
   edge e9 = G.new_edge(v2,v4);
   edge e10 = G.new_edge(v4,v2);
   G.set_reversal(e9,e10);

   edge e11 = G.new_edge(v3,v4);
   edge e12 = G.new_edge(v4,v3);
   G.set_reversal(e11,e12);   
   
   LN.append(v1); LN.append(v2); LN.append(v3); LN.append(v4);
   LE.append(e1); LE.append(e2); LE.append(e3); LE.append(e4); LE.append(e5); LE.append(e6);
   LE.append(e7); LE.append(e8); LE.append(e9); LE.append(e10); LE.append(e11); LE.append(e12);
   
   //setting colors
   set_colors(LN,clr); set_colors(LE,clr);       
   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_box(const d3_point& pllf, const d3_point& purb, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   
   double x1 = pllf.xcoord() ,x2 = purb.xcoord();
   double y1 = pllf.ycoord() ,y2 = purb.ycoord();
   double z1 = pllf.zcoord(), z2 = purb.zcoord();
   d3_point plrf(x2,y1,z1),pllb(x1,y1,z2),plrb(x2,y1,z2);
   d3_point pulf(x1,y2,z1),purf(x2,y2,z1),pulb(x1,y2,z2);
   
   insert_d3_segment(d3_segment(pllf,plrf),LN ,LE);
   insert_d3_segment(d3_segment(plrf,plrb),LN ,LE );
   insert_d3_segment(d3_segment(plrb,pllb),LN ,LE );
   insert_d3_segment(d3_segment(pllb,pllf),LN ,LE );
 
   insert_d3_segment(d3_segment(pulf,purf),LN ,LE );
   insert_d3_segment(d3_segment(purf,purb),LN ,LE );      
   insert_d3_segment(d3_segment(purb,pulb),LN ,LE );
   insert_d3_segment(d3_segment(pulb,pulf),LN ,LE );
   
   insert_d3_segment(d3_segment(pllf,pulf),LN ,LE );
   insert_d3_segment(d3_segment(plrf,purf),LN ,LE );
   insert_d3_segment(d3_segment(plrb,purb),LN ,LE );
   insert_d3_segment(d3_segment(pllb,pulb),LN ,LE );      

   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_point(const point& p, d3_window_item& n)
{
   node v = G.new_node(d3_point(p.xcoord(),p.ycoord(),0));
   node_color_map[v] = clr;
   
   n = add_d3_window_item(v);
}

void d3_window_stream::insert_segment(const segment& s, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   node v1 = G.new_node(d3_point(s.source().xcoord(),s.source().ycoord(),0));
   node v2 = G.new_node(d3_point(s.target().xcoord(),s.target().ycoord(),0));   
   edge e1 = G.new_edge(v1,v2);
   edge e2 = G.new_edge(v2,v1);
   G.set_reversal(e1,e2);
   LN.append(v1); LN.append(v2); LE.append(e1); LE.append(e2);
   set_colors(LN,clr); set_colors(LE,clr);
   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_ray(const ray& r, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   point p1 = r.point1() ,p2 = r.point2();
   vector v = r.point2() - r.point1(); 
   point pt1 =  p1, pt2 = p1 + v*5;

   node vt1 = G.new_node(d3_point(pt1.xcoord(),pt1.ycoord(),0));
   node vt2 = G.new_node(d3_point(pt2.xcoord(),pt2.ycoord(),0));
   edge e1 = G.new_edge(vt1,vt2);
   edge e2 = G.new_edge(vt2,vt1);  

   G.set_reversal(e1,e2);   
    
   LN.append(vt1); LN.append(vt2); 
   LE.append(e1); LE.append(e2);
   
   set_arrow(e1,true);
   set_colors(LN,clr); set_colors(LE,clr);   
   n = add_d3_window_item(LN,LE); 
}

void d3_window_stream::insert_line(const line& l, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   point p1 = l.point1() ,p2 = l.point2();
   point pm = midpoint(p1,p2);
   vector v = l.point1() - pm;
   vector v2= v * 10;
   
   point pt1 =  pm + v2, pt2 = pm - v2;
   
   node vt1 = G.new_node(d3_point(pt1.xcoord(),pt1.ycoord(),0));
   node vt2 = G.new_node(d3_point(pt2.xcoord(),pt2.ycoord(),0));
   edge e1 = G.new_edge(vt1,vt2);
   edge e2 = G.new_edge(vt2,vt1);
 
   G.set_reversal(e1,e2);   
   
   LN.append(vt1); LN.append(vt2); 
   LE.append(e1); LE.append(e2); 
   set_arrow(e1,true); set_arrow(e2,true);
   set_colors(LN,clr); set_colors(LE,clr);  
   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_circle(const circle& c, d3_window_item& n)
{ 
   list<node> LN; list<edge> LE;
   list<segment> LS;
   d3_window_stream_circle_segments(LS,c,30);
   segment iter;
   forall(iter,LS) { 
     insert_segment(iter,LN,LE); 
   }
   n = add_d3_window_item(LN, LE);
}

void d3_window_stream::insert_triangle(const triangle& t, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   segment s1(t.point1(), t.point2());
   segment s2(t.point2(), t.point3());
   segment s3(t.point3(), t.point1());
   
   insert_segment(s1, LN, LE); insert_segment(s2, LN, LE); insert_segment(s3, LN, LE); 
   
   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_rectangle(const rectangle& r, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   segment s1(r.upper_left(), r.upper_right());
   segment s2(r.upper_left(), r.lower_left());
   segment s3(r.upper_right(), r.lower_right());
   segment s4(r.lower_left(), r.lower_right());
   
   insert_segment(s1, LN,LE); insert_segment(s2, LN,LE);
   insert_segment(s3, LN,LE); insert_segment(s4, LN,LE);
   
   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_polygon(const polygon& p, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   list<segment> LS = p.segments();
   segment siter; 
   forall(siter,LS) {
     insert_segment(siter, LN, LE);  
   }
   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_gen_polygon(const gen_polygon& gp, d3_window_item& n)
{
   list<node> LN; list<edge> LE;
   list<segment> LS = gp.edges();
   segment siter;
   forall(siter,LS) {
     insert_segment(siter, LN, LE);  
   }   
   n = add_d3_window_item(LN,LE);
}


d3_window_stream& d3_window_stream::operator<<(const d3_point& p)
{
   d3_window_item n;
   insert_d3_point(p,n);      
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const d3_segment& s)
{
   d3_window_item n;
   insert_d3_segment(s,n);
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const d3_ray& r)
{
   d3_window_item n;
   insert_d3_ray(r,n);
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const d3_line& l)
{
   d3_window_item n;
   insert_d3_line(l,n);   
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const d3_plane& C)
{
   d3_window_item n;
   insert_d3_plane(C,n);
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const d3_sphere& S)
{
   d3_window_item n;
   insert_d3_sphere(S,n);
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const d3_triangle& t)
{
   d3_window_item n;
   insert_d3_triangle(t,n);
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const d3_simplex& P)
{
   d3_window_item n;
   insert_d3_simplex(P,n);
   return *this;
}

// 2-d output operators for the d3 window stream ...

d3_window_stream& d3_window_stream::operator<<(const point& p)
{
   d3_window_item n;
   insert_point(p,n);      
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const circle& c)
{
   d3_window_item n;
   insert_circle(c,n);
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const ray& r)
{
   d3_window_item n;
   insert_ray(r,n);
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const line& l)
{
   d3_window_item n;
   insert_line(l,n);      
   return *this;
}  

d3_window_stream& d3_window_stream::operator<<(const segment& s)
{
   d3_window_item n;
   insert_segment(s,n);      
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const triangle& t)
{
   d3_window_item n;
   insert_triangle(t,n); 
   return *this;
}  

d3_window_stream& d3_window_stream::operator<<(const rectangle& r)
{
   d3_window_item n; 
   insert_rectangle(r,n);   
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const polygon& p)
{
   d3_window_item n; 
   insert_polygon(p,n);   
   return *this;
}

d3_window_stream& d3_window_stream::operator<<(const gen_polygon& gp)
{
   d3_window_item n;
   insert_gen_polygon(gp,n);   
   return *this;
}

// parametrized graph

d3_window_stream& d3_window_stream::operator<<(const GRAPH<d3_point,int>& G_in)
{
   d3_window_item n;
   insert_graph(G_in,n); 
   return *this;
}

void d3_window_stream::insert_graph(const GRAPH<d3_point,int>& G_in, d3_window_item& n)
{ 
   list<edge> LE;
   list<node> LN;
   node_map<node> V;
   edge_map<edge> E;
   d3_window_stream_copy_graph(G, G_in, V, E);    
   
   // set node and edge colors ...
   node v;
   edge e;
   
   forall_nodes(v,G_in)  { node_color_map[V[v]] = clr; LN.append(V[v]); }
   forall_edges(e,G_in)  { edge_color_map[E[e]] = clr; LE.append(E[e]); }
   
   n = add_d3_window_item(LN,LE);
}

void d3_window_stream::insert_graph(const GRAPH<d3_point,int>& G_in, \
                                     const map<node,color>& NM, const map<edge,color>& EM, \
				     d3_window_item& n)
{
   list<node> LN;
   list<edge> LE;
   node_map<node> V;
   edge_map<edge> E;
   d3_window_stream_copy_graph(G, G_in, V, E);    
   
   node v;
   edge e;  
   forall_nodes(v,G_in) { 
     if (NM.defined(v)) node_color_map[V[v]] = NM[v]; 
     else node_color_map[V[v]] = clr;
     LN.append(V[v]); 
   }
   forall_edges(e,G_in) { 
     if (EM.defined(e)) edge_color_map[E[e]] = EM[e]; 
     else edge_color_map[E[e]] = clr;
     LE.append(E[e]); 
   }
   n = add_d3_window_item(LN,LE);
} 


#if LEDA_ROOT_INCL_ID == 600139
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
