/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  geowin.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GEOWIN_H
#define LEDA_GEOWIN_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600151
#include <LEDA/internal/PREAMBLE.h>
#endif

#include<LEDA/internal/std/ctype.h>
#include<LEDA/graph/graph.h>
#include<LEDA/core/list.h>
#include<LEDA/core/array.h>
#include<LEDA/core/d_array.h>
#include<LEDA/core/h_array.h>
#include<LEDA/core/map.h>
#include<LEDA/core/set.h>
#include<LEDA/graphics/window.h>
#include<LEDA/graphics/color.h>
#include<LEDA/graphics/rat_window.h>
#include<LEDA/graphics/ps_file.h>
#include<LEDA/graphics/d3_window.h>
#include<LEDA/graphics/geo_graph.h>
#include<LEDA/graphics/file_panel.h>

#include<LEDA/geo/d3_point.h>
#include<LEDA/geo/d3_rat_point.h>
#include<LEDA/geo/triangle.h>
#include<LEDA/geo/rat_triangle.h>
#include<LEDA/geo/transform.h>
#include<LEDA/geo/rat_transform.h>

// GeoWin version macro
#define __GEOWIN_VERSION__ 150

//#define DEBUG

// use the advanced LEDA window ...
#define __ADVANCED_WINDOW__


#if defined(_MSC_VER) || defined(__KCC) || defined(NO_STATIC_DATA_MEMBER)
#define NO_STATIC_DATA_MEMBER2
#endif

// Metrowerks compiler ...
#if defined(__MWERKS__)
#define NO_STATIC_DATA_MEMBER
#define NO_STATIC_DATA_MEMBER2
#define GEOWIN_FU_TEMPL <T>
#else

// special handling for SunPro CC 5.2 or higher
#if defined(__SUNPRO_CC) && (__SUNPRO_CC > 0x510)
#define GEOWIN_FU_TEMPL <T>
#else

#if defined(__BORLANDC__)
#define GEOWIN_FU_TEMPL <T>
#else
#define GEOWIN_FU_TEMPL
#endif

#endif

#endif


#if defined(LEDA_INSTALL)
#define INIT_GEOWIN_LEDA_DEFAULT_TYPES
#endif

// LEDA namespace support added ...
#if defined(LEDA_NAMESPACE)
#define GEOWIN_USE_NAMESPACE
#define GEOWIN_NAMESPACE_NAME leda
#endif


#if defined GEOWIN_USE_NAMESPACE

#if !defined GEOWIN_NAMESPACE_NAME
#define GEOWIN_NAMESPACE_NAME CGAL
#endif

#define GEOWIN_BEGIN_NAMESPACE namespace GEOWIN_NAMESPACE_NAME {
#define GEOWIN_END_NAMESPACE }
#else
#  define GEOWIN_BEGIN_NAMESPACE
#  define GEOWIN_END_NAMESPACE
#endif


GEOWIN_BEGIN_NAMESPACE

// for d3_points/d3_rat_points in GeoWin...
inline window& operator<<(window& w,const d3_point& p)
{ point p2d(p.xcoord(), p.ycoord());
  w << p2d; return w;
}

inline window& operator<<(window& w,const d3_rat_point& p)
{ rat_point p2d(p.xcoord(), p.ycoord());
  w << p2d;  return w;
}


inline ps_file& operator<<(ps_file& F,const d3_point& p) 
{ F << point(p.xcoord(),p.ycoord()); return F; }

inline ps_file& operator<<(ps_file& F,const d3_rat_point& p) 
{ F << point(p.xcoordD(),p.ycoordD()); return F; }

inline window& operator >> (window& w, d3_point& p)
{ point p1;
  if( w >> p1 ) p = d3_point(p1.xcoord(), p1.ycoord(),0);
  return w;
}

inline window& operator >> (window& w, d3_rat_point& p)
{ rat_point p1;
  if( w >> p1 ) p = d3_rat_point(p1.xcoord(), p1.ycoord(),0);
  return w;
}

// for list<iterator> ...

#if defined(_MSC_VER)
template<class T>
inline istream& operator>>(istream& i,typename list<T>::iterator&)
{ return i; }

template<class T>
inline ostream& operator<<(ostream& o, const typename list<T>::iterator&)
{ return o; }
#endif


class __exportGeoW geo_paint_object
{
  list<point>     LP;
  list<rectangle> USER_R;
  list<circle>    USER_C;
  list<segment>   USER_S;
  list<point>     USER_P;
  
  color cl;
  int lw;
  
public:
  geo_paint_object() : cl(black), lw(1) { }
  
  geo_paint_object(list<point>& INP, color c, int w) : LP(INP), cl(c), lw(w) { }
  
  geo_paint_object(const point& INP, color c, int w) :  cl(c), lw(w) { USER_P.append(INP); }
  geo_paint_object(const segment& INP, color c, int w) :  cl(c), lw(w) { USER_S.append(INP); }
  geo_paint_object(const circle& INP, color c, int w) :  cl(c), lw(w) { USER_C.append(INP); }
  geo_paint_object(const rectangle& INP, color c, int w) :  cl(c), lw(w) { USER_R.append(INP); }        
  geo_paint_object(const geo_paint_object& gp); 
  
  const list<point>& get_container() const;  
  color get_color() const;
  int   get_line_width() const; 
  const list<point>&     get_user_points() const;
  const list<segment>&   get_user_segments() const;
  const list<rectangle>& get_user_rectangles() const;
  const list<circle>&    get_user_circles() const;
};


// redraw objects ....

class __exportGeoW geowin_redraw
{
public:
 geowin_redraw() {}
 virtual ~geowin_redraw() {}
 virtual void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2) { }
 virtual bool draw_container();
 virtual bool write_postscript(ps_file& PS,color c1,color c2);
 virtual bool operator() (window& W,color c1,color c2,double x1,double y1,double x2,double y2); 
};


// templated variant of geowin redraw redraw class
template<class T>
class __exportGeoW geowin_redraw_container
{
public:
 geowin_redraw_container() {}
 virtual ~geowin_redraw_container() {}
 virtual bool draw(const T&, window& W,color c1,color c2,double,double,double,double) 
 { return false; }
  
 virtual bool write_postscript(const T&, ps_file& PS,color c1,color c2) { return false; }
  
 virtual bool operator() (const T& cnt, window& W,color c1,color c2,double x1,double y1,double x2,double y2 ) 
 { return draw(cnt,W,c1,c2,x1,y1,x2,y2);
   //return draw_container(); 
 }
};


template<class S,class R> class geowin_update
{
 typedef typename S::value_type InpObject;
 typedef typename R::value_type ResObject;
 typedef typename R::iterator   iterator;

 void* generic_function;

 void (*ptr0)(const S& in,R& out);
 void (*ptr1)(const S& in, ResObject& obj);
 ResObject (*ptr2)(const S& in);
 
 void clear_result(R& out)
 {
    iterator it1 = out.begin();
    iterator it2 = out.end();
    out.erase(it1,it2);    
 }
  
public:
  void* get_gen_fcn() { return generic_function; }

  virtual bool insert(const InpObject& new_in)
  // new_in: new inserted object
  {
    return false;
  }
 
  virtual bool del(const InpObject& new_out)
  // new_out: deleted object 
  {
    return false;
  }
 
  // change: move/rotate
  virtual bool change(const InpObject& old_obj, const InpObject& new_obj)
  // old   : object before change ...
  // new   : object after change ...  
  {
    return false;
  }

  virtual void update(const S& in,R& out) 
  {
    operator()(in,out);
  }
   
  virtual void operator() (const S& in,R& out)
  {
    if (ptr0 != NULL) 
    { ptr0(in,out);
      return;
     }

    if (ptr1 != NULL) 
    { clear_result(out);
      ResObject obj;
      ptr1(in,obj);
      out.push_back(obj);
      return;
     }

    if (ptr2 != NULL) 
    { clear_result(out);
      out.push_back(ptr2(in));
      return;
     }  
  }
  
  geowin_update() { ptr0 = NULL; ptr1= NULL; ptr2=NULL; generic_function = NULL; }
  virtual ~geowin_update() {}
  
  // new constructor for generic functions ...
  // not for SUN PRO (has no member templates)...
#if !defined(__SUNPRO_CC)  
  template<class T>
  geowin_update(T fptr, int dummy) { ptr0 = NULL; ptr1= NULL; ptr2=NULL; generic_function = (void*) fptr; }
#endif
  
  geowin_update(void (*fpt)(const S& in,R& out)) { ptr0 = fpt; ptr1= NULL; ptr2=NULL; generic_function = NULL; }
  
  geowin_update(void (*fpt)(const S& in,ResObject& obj)) 
  { ptr0 = NULL; ptr1 = fpt; ptr2 = NULL; generic_function = NULL; }

  geowin_update(ResObject (*fpt)(const S& in))           
  { ptr0 = NULL; ptr1 = NULL; ptr2 = fpt; generic_function = NULL; }
  
};

template <class S, class graph_t>
class geowin_graph_update : public geowin_update<S, list<geo_graph> > {

void (*func)(const S& in, graph_t& G);

public:

void update(const S& in, list<geo_graph>& out)
{ graph_t G;
  func(in,G);
  out.clear();
  out.append(G);
}

geowin_graph_update(void (*alg)(const S&, graph_t& G))
{ func = alg; }

};


// -------------------------------------------------------------------
// classes for object handling in scenes ...

class __exportGeoW geowin_handle {
 public:
  geowin_handle() {}
  virtual ~geowin_handle() {}
  virtual bool handle(GenPtr ptr) { return handle_object(ptr); } 
  virtual bool handle_object(GenPtr) { return false; }
};

template<class GeoObj>
class geowin_handle_object : public geowin_handle {
 bool (*fcn)(const GeoObj&);
 
 virtual bool handle_object(GenPtr ptr){
   return handle_casted_object((GeoObj*) ptr);
 }
 
 virtual bool handle_casted_object(GeoObj* ptr){
   if (fcn != NULL) return fcn(*ptr);
   return false;
 }

 public:
 geowin_handle_object() : fcn(NULL) { }
 
 geowin_handle_object(bool (*fu)(const GeoObj&)) : fcn(fu) { }
 
};


// -------------------------------------------------------------------

// class for implementing transformations ...
template<class GeoObj>
class __exportGeoW geowin_transform {
public:

 virtual ~geowin_transform() { }
 
 // pre- and post handlers ...
 virtual bool pre_transform(GeoWin& gw, const GeoObj& obj)
 { return true; }
 
 virtual void post_transform(GeoWin& gw, const GeoObj& obj)
 { } 
 
 virtual void transform(GeoObj& obj)
 { }
 
 // configure the transformation object 
 // (for instance by changing the transformation matrix)
 virtual void options(GeoWin& gw)
 { }
};

// derived default class for affine transformations 
// using LEDA TRANSFORM

template<class GeoObj,class TRANS>
class __exportGeoW geowin_affine_leda_transform : public geowin_transform<GeoObj> {
protected:
 
  // object storing the transformation matrix ...
  TRANS t;  

  
public:

 geowin_affine_leda_transform()  
 { }
 
 ~geowin_affine_leda_transform()  
 { } 

 
 // pre- and post handlers ...
 virtual bool pre_transform(GeoWin& gw, const GeoObj& obj)
 { return true; }
 
 virtual void post_transform(GeoWin& gw, const GeoObj& obj)
 { } 
 
 virtual void transform(GeoObj& obj)
 { obj = t(obj); }
 
 TRANS get_transformation() const
 { return t; } 
 
 void set_transformation(const TRANS& nt) 
 { t=nt; }
};

// -------------------------------------------------------------------
//  now the implementations for LEDA float and rational kernel :
// -------------------------------------------------------------------

// ------------------------------------------------------------------------
// (affine) transformations for float kernel ...
// ------------------------------------------------------------------------

template<class INT_MATRIX>
class matrix_editor : public panel {
protected:
    INT_MATRIX& mat;
    INT_MATRIX  backup;
        
public:
    matrix_editor(string s, INT_MATRIX& M) : panel(550,200,s), mat(M)
    {
      backup = mat;
    
      int rows = mat.dim1();    // Zeilen
      int columns = mat.dim2(); // Spalten
      
      int i,j;
      
      set_item_width(70);
      set_item_space(70);   
      
      for(i=0;i<columns;i++){
        for(j=0;j<rows;j++){
	  string st("%d/%d",i,j);	  
	  double_item(st,mat(i,j));
	}
      }
      
      text_item("");
      
      // buttons for graphical selections ...
      button("Translation",10);
      button("Rotation (ccw)",11);
      button("Rotation90",12);
      button("Reflection across point",14);
      button("Scale",16);
      
      text_item("");
      text_item("");
      
      fbutton("OK", 1);
      button("Cancel", 2); 
      button("Old Matrix", 3);              
    }    
    
    ~matrix_editor()
    {
    }
    
    void my_redraw()
    {
    }
    
    int my_open(window& W)
    { 
      int but;
    
      do {
       W.disable_panel();
       but = open();  
       
       switch(but){   
         case 3: { mat = backup; my_redraw(); break; }
	 case 10: { // translation
	   segment s;
	   W >> s; W << s;
	   W.read_mouse();	   
	   transform t = translation(s.target() - s.source());
	   mat = t.T_matrix();
	   my_redraw();	 
	   break;
	 }
	 case 11: { // rotation
	   point pt;
	   W >> pt; W << pt;
	   double x0=pt.xcoord(),y0=pt.ycoord(),x1,y1,x2,y2;
	   W.read_mouse_seg(x0,y0,x1,y1);
	   W.draw_segment(x0,y0,x1,y1);
	   W.read_mouse_seg(x0,y0,x2,y2);
	   W.draw_segment(x0,y0,x2,y2);
	   
	   // compute angle ...
	   vector v1(x1-x0,y1-y0);
	   vector v2(x2-x0,y2-y0);
	   double angle = v1.angle(v2);

	   transform t = rotation(pt,angle,0.001);
	   mat = t.T_matrix();	
	   my_redraw(); 	   
	   break;
	 }
	 case 12: { // rotation90
	   point pt;
	   W >> pt; W << pt;
	   W.read_mouse();
	   transform t = rotation90(pt);
	   mat = t.T_matrix();	
	   my_redraw();  	 
	   break;
	 }
	 case 14: { // reflection across point
	   point pt;
	   W >> pt; W << pt;
	   W.read_mouse();
	   transform t = reflection(pt);
	   mat = t.T_matrix();	  
	   
	   my_redraw();
	   break;
	 }

	 case 16 : { // scaling ...
	   double sx, sy;
	   sx = W.read_real("X - scaling factor");
	   sy = W.read_real("Y - scaling factor");
   
	   transform t; // identity ...   
	   mat = t.T_matrix();
	   mat(0,0) = sx;
	   mat(1,1) = sy;
	   my_redraw();
	   break;
	 }
       }
       
       W.enable_panel(); 
       
      }
      while (!(but==1 || but==2) );

      if (but == 2) { // CANCEL ...
        mat = backup;
      }
      
      return but;
    }    
};



// ------------------------------------------------------------------------
// (affine) transformations for rational kernel ...
// ------------------------------------------------------------------------

template<class MAT,class VT>
class matrix_window_handler : public window_handler {
public:
 MAT& m;
 int i,j;
 
 matrix_window_handler(MAT& mat,int w1,int w2) : m(mat), i(w1), j(w2) { }

 void operator()()
 { 
   // insert value in the matrix ...
   string ival(get_char_ptr());
   VT value;
   string_istream IS(ival);
   IS >> value;    
   
   // use a string stream for conversion ...  
   m(i,j) = value;
 }
};


class rat_matrix_editor : public panel {
protected:
    integer_matrix& mat;
    integer_matrix  backup;
    
    // mirror of matrix ...
    string** int_str;
    list<matrix_window_handler<integer_matrix,integer>*> hobj;
    
public:
    rat_matrix_editor(string s, integer_matrix& M) : panel(550,200,s), mat(M)
    {
      backup = mat;
    
      int rows = mat.dim1();    // Zeilen
      int columns = mat.dim2(); // Spalten
      
      int i,j;
      
      set_item_width(70);
      set_item_space(70);
      
      int_str = new string*[columns];
      for(i=0;i<columns;i++) int_str[i] = new string[rows];   
      
      for(i=0;i<columns;i++){
        for(j=0;j<rows;j++){
	  string st("%d/%d",i,j);
          matrix_window_handler<integer_matrix,integer>* mwh = new matrix_window_handler<integer_matrix,integer>(mat,i,j);
	  hobj.push_back(mwh);
          int_str[i][j] = mat(i,j).to_string();
          string_item(st,int_str[i][j],*mwh);
	}
      }
      
      text_item("");
      
      // buttons for graphical selections ...
      button("Translation",10);
      button("Rotation (ccw)",11);
      button("Rotation90",12);
      button("Reflection across point",14);
      button("Scale",16);
      
      text_item("");
      text_item("");
      
      fbutton("OK", 1);
      button("Cancel", 2); 
      button("Old Matrix", 3);              
    }    
    
    ~rat_matrix_editor()
    { 
      int columns = mat.dim2(), i;
    
      for(i=0;i<columns;i++) delete []  int_str[i];
      delete [] int_str;
      
      // clean up the handling objects ...
      matrix_window_handler<integer_matrix,integer>* mwh;
      forall(mwh,hobj){
        delete mwh;
      }  
    }
    

    void my_redraw()
    {        
      int rows = mat.dim1();    
      int columns = mat.dim2();       
      int i,j;   
      for(i=0;i<columns;i++){
        for(j=0;j<rows;j++){
          int_str[i][j] = mat(i,j).to_string();
	}
      }            
      redraw();     
    }
    
    int my_open(window& W)
    { 
      int but;
    
      do {
       W.disable_panel();
       but = open();  
       
       switch(but){   
         case 3: { mat = backup; my_redraw(); break; }
	 case 10: { // translation
	   rat_segment s;
	   W >> s; W << s;
	   W.read_mouse();	   
	   rat_transform t = translation(s.target() - s.source());
	   mat = t.T_matrix();
	   my_redraw();	 
	   break;
	 }
	 case 11: { // rotation
	   rat_point pt;
	   W >> pt; W << pt;
	   double x0=pt.xcoord().to_double(),y0=pt.ycoord().to_double(),x1,y1,x2,y2;
	   W.read_mouse_seg(x0,y0,x1,y1);
	   W.draw_segment(x0,y0,x1,y1);
	   W.read_mouse_seg(x0,y0,x2,y2);
	   W.draw_segment(x0,y0,x2,y2);
	   
	   // compute angle ...
	   vector v1(x1-x0,y1-y0);
	   vector v2(x2-x0,y2-y0);
	   double angle = v1.angle(v2);

	   rat_transform t = rotation(pt,angle,0.001);
	   mat = t.T_matrix();	
	   my_redraw(); 	   
	   break;
	 }
	 case 12: { // rotation90
	   rat_point pt;
	   W >> pt; W << pt;
	   W.read_mouse();
	   rat_transform t = rotation90(pt);
	   mat = t.T_matrix();	
	   my_redraw();  	 
	   break;
	 }
	 case 14: { // reflection across point
	   rat_point pt;
	   W >> pt; W << pt;
	   W.read_mouse();
	   rat_transform t = reflection(pt);
	   mat = t.T_matrix();	  
	   
	   my_redraw();
	   break;
	 }

	 case 16 : { // scaling ...
	   int shx, shy;
	   integer sx,sy;
	   shx = W.read_int("X - scaling factor");
	   shy = W.read_int("Y - scaling factor");
	   sx = integer(shx); sy = integer(shy);
   
	   rat_transform t; // identity ...   
	   mat = t.T_matrix();
	   mat(0,0) = sx;
	   mat(1,1) = sy;
	   my_redraw();
	   break;
	 }
       }
       
       W.enable_panel(); 
       
      }
      while (!(but==1 || but==2) );

      if (but == 2) { // CANCEL ...
        mat = backup;
      }
      
      return but;
    }    
};


// -------------------------------------------------------------------


template<class GeoObj>
class __exportGeoW GeoInputObject {
  public:
  void (*ptr)(GeoWin&, list<GeoObj>&);

  virtual void operator()(GeoWin& gw, list<GeoObj>& L)
  { 
    if (ptr != NULL) ptr(gw,L);
  }
  
  virtual void options(GeoWin& gw) { }
  
  GeoInputObject() { ptr = NULL; }
  GeoInputObject(void (*f)(GeoWin&, list<GeoObj>&)){ ptr=f; }
  virtual ~GeoInputObject() {}
};


#define GEOWIN_MARGIN  16        

#define APPLY_BUTTON    2
#define DEFAULTS_BUTTON 1
#define CANCEL_BUTTON   0

// Menuconstants
#define menu_const       400
#define io_menu          0
#define edit_menu1       io_menu + 1
#define mark_menu        edit_menu1 + 1
#define operate_menu     mark_menu + 1
#define readobject_menu  operate_menu + 1
#define clear_menu       readobject_menu + 1
#define input_menu       clear_menu + 1
#define edit_menu2       input_menu + 1
#define scene_menu       edit_menu2 + 1

#define view_menu        scene_menu + 1
#define zoom_menu        view_menu + 1
#define algo_menu        zoom_menu + 1
#define option_menu      algo_menu+ 1
#define scene_desc_menu  option_menu + 1
#define scene_opt_menu   scene_desc_menu + 1
#define run_menu         scene_opt_menu + 1

#define algo_algorithm_menu run_menu + 1
#define algo_options_menu algo_algorithm_menu + 1
#define scene_list_menu  algo_options_menu + 1
#define scene_groups_menu scene_list_menu + 1
#define scene_cont_menu  scene_groups_menu + 1
#define scene_type_menu  scene_cont_menu + 1
#define help_menu        scene_type_menu + 1
#define sub_menu         help_menu + 1

#define last_menu        sub_menu + 1
// last_menu only counter

// constants for GeoScene::id
#define geo_max_ids          4
#define geo_activ_id         0
#define geo_opt_id           1
#define geo_cont_id          2
#define geo_desc_id          3

// constants for GeoScene::labels
#define geo_max_labels       3
#define geo_col0_label       0
#define geo_col1_label       1
#define geo_col2_label       2

// disabled/enabled menu entries
#define GEO_CM_NEW           0
#define GEO_CM_WRITE         1
#define GEO_CM_WRITE_SEL     14 
#define GEO_CM_EXPORT        6
#define GEO_CM_IMPORT        13
#define GEO_CM_POST          7
#define GEO_CM_POSTVIS       8

#define GEO_CM_CLOSE         2
#define GEO_CM_CLOSE_ALL     12
#define GEO_CM_OK            3
#define GEO_CM_QUIT          4
#define GEO_CM_ACTIVE_OPT    5

#define GEO_CM_CLEAR         9
#define GEO_CM_VISIBLE       10
#define GEO_CM_Z             11
#define GEO_CM_BUTTONS       15

// as defined in GraphWin
#define A_LEFT	    (1 << 0)
#define A_MIDDLE    (1 << 1)
#define A_RIGHT	    (1 << 2)
#define A_SHIFT	    (1 << 3)
#define A_CTRL	    (1 << 4)
#define A_ALT	    (1 << 5)
#define A_DOUBLE    (1 << 6)
#define A_DRAG      (1 << 7)
#define A_OBJECT    (1 << 8)

#define A_IMMEDIATE (1 << 12)


enum PresentationType {geowin_normal, geowin_selected, geowin_focus};
// we have to draw an object normal, selected, ...

enum geowin_defining_points {geowin_show, geowin_hide, geowin_highlight};

enum geowin_font_type  { roman_font, bold_font, italic_font, fixed_font,user_font };

extern __exportGeoW void setup_font(window &w, double sz);

class __exportGeoW GeoScene;

template<class T> class GeoBaseScene;
template<class T> class GeoEditScene;
template<class S,class R> class GeoResultScene;

class __exportGeoW GeoSceneBuilder;
class __exportGeoW GeoEditor;

// animation class
// some possible actions for animations
#define GEOWIN_MOVE_SELECTED       385
#define GEOWIN_FORCE_INPUT         1
#define GEOWIN_STOP_MOVE_SELECTED  -1


class __exportGeoW geowin_animation { 
public:
 geowin_animation() {}
 virtual ~geowin_animation() {}
 virtual void init(const GeoWin&) { }
 virtual void finish(const GeoWin&) { }

 virtual bool is_running(const GeoWin&) { return true; }
 
 virtual point get_next_point(const GeoWin&) = 0;
 
 virtual long  get_next_action(const GeoWin&) { return -1; }
};



class __exportGeoW geowin_text {
  void (*position_fcn)(double x1, double x2, double y1, double y2, void* obj_ptr,bool b, double& xp, double& yp);
  typedef  void (*PFCN)(double, double, double, double, void*, bool, double&, double&);

  double x_offs;  
  double y_offs;
  
  double size;  // size
  
  string remark;// text of the remark
  
  geowin_font_type  ft_type; // font type
  
  string user_font;
  
  color  clr;   // text color

  friend class __exportGeoW GeoWin;
  
  void construct();

  public:
  
  geowin_text(string t, double ox, double oy, geowin_font_type ft, double sz, string uf, color c = black);
  geowin_text(string t, geowin_font_type ft, double sz);
  geowin_text(string t) { construct(); remark = t; }
  geowin_text() { construct(); }
  
  PFCN   get_position_fcn() const { return position_fcn; }
  PFCN   set_position_fcn(PFCN n) { 
    PFCN prev = position_fcn; position_fcn=n; return prev; 
  }
  
  void init(string t, double ox, double oy, geowin_font_type ft, double sz, string uf, color c = black);
  
  string get_text() const;
  string set_text(const string&);
  
  double get_size() const; 
  double set_size(double);
  
  geowin_font_type get_font_type() const;
  geowin_font_type set_font_type(geowin_font_type);
  
  string get_user_font() const;
  string set_user_font(const string&);
  
  color get_color() const; 
  color set_color(color);
  
  double get_x_offset() const;
  double set_x_offset(double);
  
  double get_y_offset() const;
  double set_y_offset(double);

  friend __exportGeoW ostream& operator<<(ostream& o,const geowin_text&);
  friend __exportGeoW istream& operator>>(istream& i,geowin_text&); 
};

__exportGeoW void geo_move(GeoWin& gw, const point& p);
__exportGeoW void geo_rotate(GeoWin& gw, const point& p);
__exportGeoW void geo_object_dragging(GeoWin& gw, const point& p);
__exportGeoW void geo_scroll_scene(GeoWin& gw, const point& p);


inline ostream& operator<<(ostream& o,const geowin_text& gt)
{  o << gt.get_text(); return o; }

inline istream& operator>>(istream& i,geowin_text& gt)
{ return i; }

extern __exportGeoW void geowin_draw_text_object(window&  w, double x1, double x2, double y1, double y2, void*
adr, const geowin_text& gt, bool b);
extern __exportGeoW void geowin_draw_text_object(window&  w, ps_file&  ps, double x1, double x2, double y1, double y2, void*
adr, const geowin_text& gt, bool b);


inline ps_file& operator << (ps_file& p,const geowin_text& gt)
{ return p; }

// ------------------------------------------------
//  generator objects ...
// ------------------------------------------------

template<class T>
class __exportGeoW geowin_generator {
protected:
   string name;
public:
   geowin_generator() { name = ""; } 
   geowin_generator(const string& s) { name = s; }
   
   virtual ~geowin_generator() { }
   
   string get_name() const { return name; }
   
   virtual void set_parameters(GeoWin& gw) { }
   
   virtual void operator()(T& cnt) { }
};

// ------------------------------------------------


// Class that maps all known GeoScenes
class __exportGeoW GeoScenePrototypes 
// saves all prototypes and names for known scenes 
{   
  friend class __exportGeoW GeoSceneBuilder;

  //number of known scenes
  static int       SceneCount;
  
  //array of pointers to prototypes of all known scenes
  array<GeoScene*> allscenes;
  
  // array of typenames for all known scenes
  array<string>    scenenames;  

  static int       RegisterScene(GeoScene* sc); 
  //inserts sc into allscenes and gives it a unique name,returns array position
  static bool st_alloc;   // has a call to RegisterScenes allocated memory  ?
 
  public :
  
  GeoScenePrototypes();
  ~GeoScenePrototypes();

  static void      FreeMem();
  static GeoScene* get_prototype(int id); // id-array position
  static GeoScene* get_prototype(string str);
  
  static void      set_type_name(int id, string str); 
  //sets the name of scene  allscenes[id]
  static string     get_type_name(int id);

  static list<string> get_all_editables(); 
  //returns the names of all (editable) GeoEditScenes<T>

  friend class __exportGeoW GeoScene;
};


class __exportGeoW GeoWin;

// contains functions for editing
class __exportGeoW GeoEditor
{
  protected :
  
  int id;
  
  GeoEditor(int d = 0) : id(d) { }
  
  public :
  
  virtual ~GeoEditor() {}  
  int operator() () { return id; }
  
  virtual void scene_read_object(int n) { }    // reads in a geoobject (n=0: mouse, else keyboard)
  virtual void generate() { }             // generates objects
  virtual void copy()  { }
  virtual void paste() { }
  virtual void del_sel()   { }
  virtual void obj_focus()   { }
  virtual void clear()       { }
  virtual void transform_options() { }
  virtual void transform()   { }
  virtual void move_or_rotate_sel(double, double, double, bool, bool) { }
};

class __exportGeoW geowin_import;
class __exportGeoW geowin_export;


typedef GeoEditor* geo_editor;

class __exportGeoW GeoScene
{
  
  friend class __exportGeoW GeoWin;  
  friend class __exportGeoW GeoScenePrototypes;
  friend class __exportGeoW GeoSceneBuilder;
  friend class __exportGeoW geowin_text;
  
  // Each scene has an unique |name| 
  // the name is constructed by gw

  string name; 
  string base_name;
  
  // short name for buttons ...
  string short_name;
  
  // The user can add a string desribing the scene ...
  string description;
  
  protected :
  
  GeoWin* gw;       // pointer to the scene's |GeoWin|  

  list_item  pos;   // position in the list of scenes which are managed by |gw|
  
  bool debug_mode;  // debugging mode ...
  string debug_file_name;
  
  // break incremental algorithm after first returned
  // false (from insert/del/change) or not ...
  bool br_after_false;
  
  // copy object attributes in result scenes ... 
  bool copy_attr;
  
  int fcn_state;   // number of input function (0...default input)
  
  // use scene color for 3d output ??
  bool       d3_use_scene_color;
    
  // drawing parameters :
  color      col1;  //color for normal drawing  
  color      filling_color; // color for filled objects
  color      text_color;    // color for drawing text 
  
  // drawing parameters for selected objects ...
  color      col2;  //color for selected drawing
  color      sel_filling_color;
  line_style sel_l_style;
  int        sel_line_width;

  int        back_line_width;   // for objects, if the scene is not active
  int        active_line_width; // for objects in the currently active scene
  line_style  l_style;
  point_style p_style;
  
  // cyclic colors in this scene
  bool       cyclic_colors;
  int        cyclic_fill_colors_counter;
  int        cyclic_colors_counter;
  
  // show direction/ orientation of segments/lines/polygons/...
  bool       show_orientation;
  
  // limit for edit scenes ...
  int limit;
  
  // export/ import objects
  list<geowin_export*> export_objects;
  list<geowin_import*> import_objects;

  geowin_redraw* redraw_pt;
  
  // flag used by |GeoWin| for marking the scene with input focus
  bool active;
  // flag used by |GeoWin| for marking if this scene is visible or hidden
  bool visible;
  // defines the type of the edit-menu used by |GeoWin| 
  int edit_menu_type;

  // The objects of each scene can be the input for a function whose results
  // builds a new scene. Such a result scene is stored in list |results|.
  list<GeoScene*>    owner;     //is empty if it isn't a result scene
  list<GeoScene*>    results;

  virtual GeoScene* clone() = 0; // creates an empty  clone of this scene.
  
  virtual bool IsSuperOrEqual( GeoScene* sc ) { return false; }
  // Returns true if this scene is of the same class as |sc| or if its
  // class is a super class of the class of |sc|. Returns false otherwise.
  
  // internal used integers
  // geo_activ_id ... button number in "activate" submenu of the "Scenes" menu
  // geo_opt_id ... button number in "options" submenu of the "Scenes" menu
  // geo_cont_id ... button number in "contents" submenu of the "Scenes" menu
  // geo_desc_id ... button number in "description" submenu of the "Scenes" menu
  
  int    ids[geo_max_ids];

  // internal used labels
  // geo_col0_label ... dialog label name for fill color
  // geo_col1_label ... dialog label name for col1
  // geo_col2_label ... dialog label name for col2
  string labels[geo_max_labels];

  // is set to true, while dragging 
  bool while_dragging;

  // true: we move/rotate one object with the mouse
  bool mouse_changing;
  
  // client data pointers of the scene ...
  void*  cl_data[16];
  
  // names of the generators of the scene 
  // (will be filled by generate function in edit scenes ...)
  list<string> generator_names;
  
  // activate handler (is called when scene is activated via GeoWin::activate)
  void (*activate_fcn)(GeoScene*);
  
  // limit for performing incremental update operations
  int incremental_update_limit;
  
  public :
  
  // alternative draw function
  void (*draw_fcn)(...);
  
  list<geowin_text> additional_objects;
  
  int edit_mode; // parameter for edit_obj function in edit scenes ...
  
  list<string> draw_object_parameters; // names of different drawing modes ...
  int draw_mode_number;
 
  int z;           // z - coordinate of the scene ...
  
  // removed from BaseScene/ EditScene ...
  bool     myobjs;  //true,if the memory for the data object was allocated by this Geoscene
  bool     high_light;
  bool     mouse_obj_exist;
  bool     base_mode;
  int      change_mode; // 0 ... added, 1 ... deleted, 2 ... changed; negative: call update, not 
                        // insert/del or change ...  
  int      but;
  string   nval;
  int oldwl;
  line_style oldstl;
  point_style oldpstl; 
  
  void (*init_d3_window)(GeoScene* sc, d3_window& W, GRAPH<d3_point,int>& H);  
  
  void compute_d3_output(GeoScene* sc, d3_window& W, GRAPH<d3_point,int>& H);
  
  virtual list<string> get_fcn_names(int&, bool flag) 
  { list<string> LS; return LS; }
  virtual void set_alternative_input_fcn(int i)  { }
  virtual void call_buffer_fcn(int w, bool flag, ostream& O) { }
  
  GeoScene(GeoWin* win);
  
  virtual ~GeoScene();

  static GeoScene* new_scene(string str); // parameter: prototype name or -id
  static GeoScene* new_scene(int id);
  
  void init_default_values();
  // set colors and other parameters ...
 
  void show_points(const list<point>&);

  geowin_redraw* get_redraw_pt() { return redraw_pt; } 
  GeoWin*          get_geowin()  { return gw; }
   
  virtual geo_editor type_editor()  { return 0; }
  
  bool IsMyKindOfScene( GeoScene* sc );
  virtual int GeoSceneId() const = 0;
  void split_and_append(string s, list<string>& LS, int cnt);
  
  void add_import_object(geowin_import* io) { import_objects.append(io); }
  void add_export_object(geowin_export* eo) { export_objects.append(eo); }
  
  void set_name(string nm);
  string get_name() const;
  void set_base_name(string nm);
  string get_base_name() const;
  void set_type_name(string str);
  string get_type_name() const;
  
  int generator_number;
  list<string> get_generator_names() const;
  int get_generator_number() const;
  void set_generator_number(int n);
  
  string get_short_name() { return short_name; }
  
  color get_color() const { return col1; }
  void  set_color(color c) { col1 = c ; }
  color get_color2() const { return col2; }
  void  set_color2(color c) { col2 = c; }

  int   get_back_line_width() const { return back_line_width; }
  void  set_back_line_width(int lw) { back_line_width = lw; }
  int   get_active_line_width() const { return active_line_width; }
  void  set_active_line_width(int lw) { active_line_width = lw; }
  int   get_line_width() const 
  {return active ? active_line_width : back_line_width;}
  
  virtual color get_default_color1();
  virtual color get_sel_color();
  virtual color get_default_color2();
  virtual point_style get_default_point_style();
  virtual color get_default_text_color();
  virtual line_style get_default_line_style();
  virtual int  get_default_line_width();    

  bool get_visible() const { return visible; }
  void set_visible(bool v) { visible = v; if (v) update(); }
  bool get_active() const { return active; }
  void set_active(bool ac) {  active = ac; }
  bool is_visible() const { return (gw && (visible || active)); }
  
  void add_owner(GeoScene*);
  
  // client data operations...
  void* set_client_data(void*,int i=0);
  void* get_client_data(int i=0);
  
  void delete_additional_objects() { additional_objects.clear(); }

  void add_dependence(GeoScene* sc); 
  void del_dependence(GeoScene* sc); 
  
  virtual void init_from_prototype(GeoScene* proto);

  virtual void init_data();

  virtual void call_redraw_objects(bool& b,window* w, double x1, double y1, double x2, double y2) {}
  void redraw(window* w, double x1, double y1, double x2, double y2); 
  
  virtual void mouse_at_pos(double, double, long& MASK);
  /* informs this scene about current mouse position. |MASK| will
     be checked for userdefined actions  */ 

  virtual void update();
  
  virtual void update(GeoScene* sc){}
  
  void update_and_redraw();
  
  virtual void clear_op_objs() {}
  
  virtual void copy_clip_buffer(GeoScene* sc) { }
  // sc must have same type ...
  
  void    scene_description();
  
  void    scene_options()  { options((panel*)0); }
  virtual void scene_input_options() { }
  virtual void clear() { }
  virtual void del_objref() {}

  virtual bool options(panel* p = 0);
  virtual void contents();
  
  // helper function for computing "contents"
  bool (*contents_fcn)(GeoScene*, list<string>&);

  string information();
  virtual void edit(); 
 
  virtual void read(istream& is, bool) = 0;
  virtual void write(ostream& os, string tr) = 0;
  virtual void write_postscript(ps_file& f);  
  
  virtual void* get_untyped_objptr()  { return NULL; }
  virtual void* get_untyped_mouseobjptr() { return NULL; }
  virtual void  set_mouse_object(void* adr) {}
  
  void get_export_object_names_and_descriptions(list<string>& N, list<string>& D);
  void get_import_object_names_and_descriptions(list<string>& N, list<string>& D); 
  
  // was GeoProp
  protected:

  map<void*, void*> * original; 

  // color of the boundary of the object :
  map<void*, color>* color1_map;

  // color of the interior of the object :
  map<void*, color>* color2_map;

  map<void*, line_style>* lst_map;  
  map<void*, int>* lw_map;

  // labels for the objects
  map<void*, string>* label_map;
  
  // geowin_texts for the objects
  map<void*, geowin_text>* text_map;
  
  // address list of selected objects ...  
  list<void*>             sel_objs;

public:

  // Methods on colormap1 :
  bool colors_defined1()  { return  color1_map != 0; }
  void undefine_colors1();

  color get_obj_color(void* o); 
  color set_obj_color(void* obj, color c);

  // Methods on colormap2 :
  bool colors_defined2()  { if (color2_map==NULL) return false; else return true; }
  void undefine_colors2();

  color get_obj_fill_color(void* o);
  color set_obj_fill_color(void* obj, color c);
  
  // Methods on line_style_map :
  bool line_styles_defined()   { if (lst_map==NULL) return false; else return true; }
  void undefine_line_styles();

  line_style get_obj_line_style(void* o);
  line_style set_obj_line_style(void* obj, line_style lst);

  // Methods on line_width_map :
  bool line_width_defined()   { if (lw_map==NULL) return false; else return true; }
  void undefine_line_width();

  int get_obj_line_width(void* o);
  int set_obj_line_width(void* obj, int w);

  // methods on label_map
  bool labels_defined()   { if (label_map==NULL) return false; else return true; }
  void undefine_labels();  

  bool get_obj_label(void* o,string& label);
  void set_obj_label(void* obj, string label);
  
  // methods on text_map
  bool texts_defined() { if (text_map==NULL) return false; else return true; }
  void undefine_texts(); 
  bool get_obj_text(void* o, geowin_text& t);
  void set_obj_text(void* obj, geowin_text t);  
  
  
  void setup_focus_dialog(window&,void*);

  void set_original_properties(void* other, void* orig);

  void set_def_properties(void* o);

  void original_properties(void* o);
  
  void set_default_properties(void* o);

  void undefine_all();
 
  int keyboard_panel(window& w, string& nval);
  
  int focus_dialog(string& nval, window& w);

  color oldcl;
  color oldfl;
  line_style old_ls;
  int oldw;
  color text_clr;
  point_style old_ps;
  bool  old_ori;
  
  void set_window_params(GeoWin* gw, window& w, void* adr, PresentationType pt);

  void restore_window_params(window& w);
  
  void set_ps_params(ps_file& F, void* adr); 
  
  // removed from templated class - T is a type of a geometric object

  //bool (*box_intersection_fcn)(const T& , double, double, double, double,bool);
  //void (*get_bounding_box_fcn)(const T&, double&, double&, double&, double&);
  
  bool (*box_intersection_fcn)(...);
  void (*get_bounding_box_fcn)(...);

  //void (*move_fcn)(T& obj, double, double);
  //void (*rotate_fcn)(T& obj, double, double, double);
  
  void (*move_fcn)(...);
  void (*rotate_fcn)(...);   
  
  //string (*info_fcn)(const T&);
  string (*info_fcn)(...);
  
  //void  (*objects_init_d3_window)(const T&,d3_window&, GRAPH<d3_point,int>&);
  void  (*objects_init_d3_window)(...);
  
  //void (*move_point_fcn)(GeoObj&, double, double, int);
  void (*move_point_fcn)(...);
  
  //void (*edit_obj_fcn)(GeoWin&, GeoObj&, int);
  void (*edit_obj_fcn)(...);
  
  //bool (*start_change_fcn)(GeoWin&, const GeoObj&);
  bool (*start_change_fcn)(...);
  
  //void (*end_change_fcn)(GeoWin&, const GeoObj&);
  void (*end_change_fcn)(...);
  
  //void  (*defining_points_fcn)(const GeoObj&, list<point>&);
  void (*defining_points_fcn)(...);
  geowin_defining_points     handle_defining_points;
  
  virtual void window_output(window& w, void* obj_adr) { }

  void draw_object(void* adr, window& w, PresentationType pt);
  
  bool obj_defining_points_change;
  int obj_pnr;  
  
  void move_point(void* obj_adr, double dx, double dy);
  
  bool fill_rect(double&, double&, double&, double&);
  
  void obj_edit();
  
  void ps_draw_text(ps_file& F, void* adr);
  
  // operations for iteration on the scene container ...
  virtual void init_iteration() { }  
  virtual void* get_next_obj_adr() { return NULL; }  
  virtual void iterate() { }
  virtual bool  no_end_reached() { return false; }  
  
  virtual void set_select(void* adr, bool b);
  void set_select(const list<void*>& L, bool b);
  
  virtual bool get_select(void* obj_adr);
  void select_in_rect(bool uflag, double x1, double y1, double x2, double y2,bool b); 
  
  // second select version uses a geowin_handle object to achieve information
  // what objects should be selected
  void select_callback(bool uflag, geowin_handle* callback);
   
  
  bool start_changing();
  void stop_changing();
  
  // operation for deletion of mouse object in editable scenes 
  // (+ appends copy to changed objects list, if f true ...)
  virtual bool del_mouse_object(bool f);
  
  virtual void add_mouse_object_copy(void*& new_adr);
  
  void del_focus(geo_editor ed);
  
  void setup_focus_or_raise(bool flag);  
  
  void toggle_selection(); 
  
  void draw_object_with_properties(void* obj_adr);
  
  //operation for editable scenes ...
  bool find_object(double x, double y);  
};

typedef GeoScene* geo_scene;


// import/export objects ...

class __exportGeoW geowin_import {
public:
  string name, description;
  virtual void operator()(geo_scene sc, string in_file) { }
  
  string get_name() const { return name; }
  string get_description() const { return description; }
  
  virtual ~geowin_import() { }
};

class __exportGeoW geowin_export {
public:
  string name, description;

  virtual void operator()(geo_scene sc, string out_file) { }
  
  string get_name() const { return name; }
  string get_description() const { return description; } 
  
  virtual ~geowin_export() { } 
};

// ------------------  some operators necessary for lists ----------------------------
inline int compare(const geowin_import& , const geowin_import& ){ return 0; }
inline istream& operator>>(istream& i, geowin_import&){ return i; }
inline ostream& operator<<(ostream& o, const geowin_import&){ return o; }

inline int compare(const geowin_export& , const geowin_export& ){ return 0; }
inline istream& operator>>(istream& i, geowin_export&){ return i; }
inline ostream& operator<<(ostream& o, const geowin_export&){ return o; }
// -----------------------------------------------------------------------------------



class __exportGeoW GeoSceneGroup 
{
  friend class __exportGeoW GeoScene;
  friend class __exportGeoW GeoWin;

  string name;
  int id; // menu id ...
  list<geo_scene> group_members;
  list<geo_scene> dependent_scenes;
  
  GeoSceneGroup(const string& s) : id(-1){ name = s; }
  GeoSceneGroup(const string& s,geo_scene sc) : id(-1) { name = s; group_members.append(sc); }
  GeoSceneGroup(const string& s, const list<geo_scene>& Ls) : id(-1)
  { name = s; group_members = Ls; }

};

typedef GeoSceneGroup* geo_scenegroup;

class __exportGeoW GeoSceneBuilder
// helper class for building scenes
{ 
  protected :
  
  friend class __exportGeoW GeoScene; 

  int prototypeid;

  GeoSceneBuilder() : prototypeid(-1) {}
  
  GeoSceneBuilder(geo_scene proto) 
  { 
    prototypeid = GeoScenePrototypes::RegisterScene(proto);
  }

  ~GeoSceneBuilder(){ GeoScenePrototypes::FreeMem(); }
  
  public :
  
  int get_id() { return prototypeid; }
  
  geo_scene new_scene()
  {
    geo_scene proto = GeoScenePrototypes::get_prototype(prototypeid);
    return proto->clone();
  }
};


void set_rational_precision(int prec); 

class __exportGeoW GeoFunctions
// helper class for adding functions to a menu
{ 
  protected :
  
  string label;
  
  public : 
  
  GeoFunctions(string s) : label(s) {}
  virtual ~GeoFunctions() {}
  virtual void call(GeoWin&) = 0; 
  
  LEDA_MEMORY(GeoFunctions)
};


template<class F>
class __exportGeoW GeoFunction : public GeoFunctions
{ 
  F      f;
  
  public :
  
  GeoFunction(F g, string l) : GeoFunctions(l), f(g) {}
  virtual ~GeoFunction() {}
  virtual void call(GeoWin& gw) {  geo_call(gw, f, label); }
};


typedef void (*geo_action)(GeoWin&, const point&);
typedef void (*geo_help_action)(const GeoWin&);


/*{\Manpage {GeoWin} {} {Geometry Windows}} \label{Geo Windows}*/

class __exportGeoW GeoWin 
{
  /*{\Mdefinition
    An instance of data type |\Mname| is an editor 
    for |sets of geometric objects|. It can be used for the visualization of 
    result and progression of geometric algorithms. 
    |\Mname| provides an |interactive interface| and a |programming interface|
    to visualize and manipulate geometric objects and data structures.
    
    Sets of geometric objects are maintained in so-called
    |scenes|.
    
    {\bf Scenes}    
    
    |Scenes| are instances of the various scene data types supported by |\Mname|.
    They are used to
    store collections of geometric objects and attributes of the objects and collections.
    Furthermore the scene classes have to provide functionality for |\Mname| to
    handle the geometric objects of a scene. 
    
    Each |scene| stores geometric objects in a |container| (a LEDA-list or STL-list).
    We call these geometric objects stored in a container of a |scene| the |contents|
    of a scene. The scenes and their |contents| can be manipulated by the interactive
    interface and the programming interface of |\Mname|.
    
    With every |scene| a set of attributes is associated. Most of them describe the visual 
    representation of the scene, for
    instance the boundary- and fill-color of the objects, the visibility of the scene,... .
    
    We use the type |geo_scene| as the scene item type of |\Mname|; it may 
    be helpful to view it as pointers to scenes.
    
    We distinguish the following types of scene classes:
    
    \begin{enumerate}
    
    \item {\em Edit Scenes} (type |GeoEditScene<CONTAINER>|)
    
    where |CONTAINER| is the type of the scene's container storing the contents of the scene,
    for instance |list<point>|. These scenes can be edited by the user through the interactive
    interface of |\Mname|. Note that edit scenes have some special features.
    An important feature is the possibility to |select| objects through the
    interactive interface. These selected objects have special attributes, see the table
    of scene attributes.
      
    \item {\em Result Scenes} (type |GeoResultScene<I,R>|)
    
    These scenes are not independently editable by the user. The contents of result scenes is
    computed by a user-defined |update function| or |update object| executing a geometric algorithm.
    This recomputation of the scene contents will be done every time when another scene (this other scene
    we call the input scene of the result scene) changes. 
    The contents of the result scene is stored in a container of type |R|.
    The input scene must be a |Basic Scene| with a container of type |I|.
    The update function  
      |void (*f_update)(const I& input, R& result)|
    gets the contents of this input scene and computes
    the contents |result| of the result scene.
    We say that the result scene |depends| on its input scene.   
  
    \item {\em Basic Scenes} (type |GeoBaseScene<CONTAINER>|)   

    |Edit Scenes| and |Result Scenes| are derived from |Basic Scenes|.
    The basic scene type works on 
    container types providing an interface as the list of the STL library. 
    More precisely, |CONTAINER| has to support the following type definitions 
    and STL-like operations:
    \begin{itemize}
    \item |value_type| - the type |T| of the values the container holds
    \item |iterator| 
    \item operations |begin()| and |end()| returning an iterator that can be used
          for begining (ending) the traversal of the container
    \item |void push_back(const T&)| for inserting an element at the end of the container
    \item |iterator insert(iterator it, const T&)| for inserting an element (before |it|)
    \item |void erase(iterator it)| for erasing an element at position |it|
    \item operation |bool empty()| returning |true| if the container is empty, false
          otherwise
    \end{itemize}  
    That means, that LEDA lists can be used as well as containers.
    
    \end{enumerate}
    
    The programming interface of |\Mname| provides various operations to create 
    |Edit Scenes| and |Result Scenes|. |Basic Scenes| are not created directly by
    the operations of the programming interface, but they are used for derivation
    of the other scene types, and we will find them in the programming interface, 
    when both Edit and Result Scenes are supported by an operation.

    {\bf GeoWin - class}
  
    We explain some important terms of the |\Mname| data type.
    Every instance |GW| of |\Mname| can maintain a number of |geo_scenes|. \\
    |Visible| scenes will be displayed by |GW|, non-visible scenes will not be displayed.
    Displayed means, that the contents of the scene will be displayed.
    A special case is the |active| scene of |GW|. Every |\Mname| can have at most one |active|
    scene. The active scene is an Edit Scene with input focus. That means that this scene
    is currently edited by the user through the interactive interface. Note that the
    currently active scene will be displayed. \\
    Another important topic is the display order of scenes. Every scene has an associated non-negative
    z-coordinate. When a scene is created, it gets z-coordinate |0|.
    When |GW| redraws a scene, the contents of this scene and the contents of its visible
    dependent scenes is drawn.
    In the redraw-operation of |\Mname| the scenes with higher z-coordinates will be drawn
    in the background of scenes with lower z-coordinate.The scenes with z-coordinate |0| will
    be drawn on top in the order of their creation in its instance of |\Mname| (the scene, that
    was created last and has z-coordinae |0| is the scene on top).
  
    {\bf Attributes of scenes} 
    
    The following attributes are associated with every scene.

\begin{center}
\begin{tabular} {|l|l|l|}\hline
 
\hline
{\bf Name}             & {\bf Type}      & {\bf Description} \\
\hline
\hline
|active|            & |bool|                  & activity status of a scene\\
\hline
|active_line_width| & |int|                  & line width used for drawing objects of active scenes\\
\hline
|client_data|       & |void*|                    & some |void*|-pointers that can be associated with a scene\\
\hline 
|color|             & |color|                & boundary color of non-selected objects\\
\hline
|description|       & |string|                   & a string describing the scene\\
\hline
|fill_color|        & |color|                  & fill color of objects\\
\hline
|line_style|        & |line_style|           & line style used for drawing objects\\
\hline
|line_width|        & |int|                    & line width used for drawing objects of non-active scenes\\
\hline
|name|              & |string|         & the name of the scene\\
\hline
|point_style|       & |point_style|          & point style used for drawing objects\\
\hline
|selection_color|   & |color|                  & boundary color selected objects\\
\hline
|selection_fill_color|  & |color|              & fill color of selected objects\\
\hline
|show_orientation|  & |bool|              & disables/enables the drawing of object orientations/directions\\
\hline
|text_color|        & |color|           & text label color\\
\hline
|visible|           & |bool|           & visibility of a scene in its |GeoWin|\\
\hline 
|z_order|           & |int|           & z-coordinate of a scene in its |GeoWin|\\
\hline
\end{tabular}
\end{center}

   {\bf Attributes and parameters of instances of |\Mname|}    
    
    Every instance of type |\Mname| uses the following attributes and parameters. The parameters starting with |d3_|
    are influencing the 3-d output option of |\Mname|. This 3-d output option uses the LEDA-class |d3_window| for
    displaying geometric objects. See also the |d3_window| - Manualpages for a description of the 3-d output
    parameters.

\begin{center}
\begin{tabular} {|l|l|l|}\hline
 
\hline
{\bf Name}             & {\bf Type}      & {\bf Description} \\
\hline
\hline
|active_scene|         & |geo_scene|    & the active scene\\
\hline
|bg_color|             & |color|        & window background color\\
\hline
|bg_pixmap|            & |string|       & name of the used window background pixmap\\
\hline
|d3_elimination|       & |bool|         & |true| - in the d3-output hidden lines will be eliminated\\
\hline
|d3_show_edges|        & |bool|         & enables/disables the redraw of edges in the d3-output\\
\hline
|d3_solid|             & |bool|         & |true| - in the d3-output faces will be drawn in different grey scales\\
\hline
|grid_dist|            & |double|       & width of the grid in the drawing area\\
\hline
|grid_style|           & |grid_style|   & style of the grid in the drawing area\\
\hline
|show_grid|            & |bool|         & defines if a grid should be used in the drawing area of the window\\
\hline
|show_position|        & |bool|         & |true| - the coordinates of the mouse cursor are displayed\\
\hline
\end{tabular}
\end{center}
    
    {\bf The geometric objects}

    The objects stored in the containers of the scenes have to support input and
    output operators for streams and the LEDA window and the output operator
    to the |ps_file|.

    {\bf Manual overview}
    
    The following manual pages have this structure:
\begin{itemize}
\item a) Main operations (creation of scenes)
\item b) Window operations (initialization of the drawing window)
\item c) Scenes and scene groups (get/set - operations for changing attributes)
\item d) I/O operations
\item e) View operations (zooming)
\item f) Parameter operations (get/set - operations for instances of type |\Mname|)
\item g) Event handling
\item h) Scene group operations
\item i) Further operations (changing of the user interface, 3d output, ...)
\end{itemize}    
    
}*/

  friend class __exportGeoW GeoScene;
  
  typedef void (*D3_FCN)(geo_scene, d3_window&, GRAPH<d3_point,int>&);
  
  // window member parameters :
  // **************************
  window* Wp;
  window* msg_win;
  window* quick_access;
  
  d3_window* d3_win;
  
  // we can register other windows
  // that are handled in the event loop
  // of the interactive mode ...
  
  map<window*, bool (*)(window*,int,int,double,double)>  registered_windows;
  
  // input mode ...
  int input_mode;
  
  string cur_info;
  void draw_info();
  
  // pointer to actual animation...
  geowin_animation* act_animation;

  // is |true| iff the window was opened
  bool   is_open;
  
  // if |is_locked| is true, no redraws take place  (flag to avoid multiple 
  // redraws)
  bool   is_locked;
  
  //create scenes when build in algor. are called
  bool  algo_create_scenes;
  
  // like window::grid but it is not set to 0 if the grid distance is to small
  // or if no grid display is wished
  double grid;
  bool   show_grid;    // |true| if no grid is displayed, false otherwise
  
  bool   show_coords;  // |true| if coordinates are displayed, false otherwise

  bool   time_meas;    // |true| if time measurement is on

  string bg_pixmap;    // name of the window background pixmap 
  
  string patstring;    // pattern for file operations (default *.geo)
  
  // additional text objects ...
  list<geowin_text> additional_objects;
  
  
  //show the quick access window ...
  bool show_quick_access_window;

  // parameter for 3d output
  bool   gw_elim;      // elimination in the 3d window ?
  bool   gw_solid;     // solid output ?
  int   gw_anim_speed;// animation speed for 3d
  bool   gw_d3edges;   // show or hide edges in d3 output
  bool   gw_d3_cs;     // show or hide coord. system ...
  double gw_d3_zvalue; // z -value for input of 3d objects ...
  
  // true : update result scenes while moving/rotating objects ...
  bool update_while_object_dragging;
  
  // animation steps for zooming
  int gw_anim_steps;

  // parameter for size of the isorect. around mousepointer
  // (used when finding objects)
  double mouse_fct;
  
  // call mouse_at_pos for current scene (or not)
  bool mouse_pos_check;
  
  //  should the status window be shown ?
  bool show_status;

  // menu member parameters :
  // ************************
  
  // array of all menus : 
  menu**  menus;  
  
  // array of associated menu functions :
  GeoFunctions** menu_fcn;
  int            menu_fcn_count;  // number of all menu items
  int            user_fcn_count;  // number of user defined menu items
  
  // values for the NEW, WRITE, CLOSE, DONE and QUIT buttons
  public:
  int     buttons[GEO_CM_BUTTONS];
  int     button_info[GEO_CM_BUTTONS];  // and infos about their status
                         // 0 = inactive, 1 = active, 2 = button depend

  int VISIB_CHOICE;
  private:
  // mouse handling parameter :
  // **************************
  
  // actions for certain mouse or key events
  map<long, geo_action> mouse_actions;
  
  // map for help actions
  d_array<string, geo_help_action> help_actions;
  
  list<string> help_list;
  
  string special_help_text;
  bool special_help_text_display;
  
  // scene member parameters :
  // *************************

  // list of all scenes :
  list<geo_scene> scenes;
  
  // list of all scene groups :
  list<geo_scenegroup> scene_groups;

  // currently active scene :
  geo_scene       cur;

  // if a special scene should be edited, |edit_scene| points 
  // to this scene 
  geo_scene       edit_scene;

  // list of names of scene types that can be edited by this |\Mname|
  list<string> editables;

  // maps scene names to its number to avoid duplicates
  d_array<string, int>    scene_name_map;
  
  // node colors / edge colors for d3 output window ...
  h_array<node,color> node_color_map;
  h_array<edge,color> edge_color_map; 
  
  // members of this set will be painted using arrows in d3 output
  set<edge> arrow_set;
  
  string load_fname;
  string editor_name;
  
  bool use_external_editor;
  // if external editor is used,
  // read in the new scene contents after quiting
  // the editor ...
  bool re_read;
  
  // members for the user layer 
  list<geo_paint_object> user_objects;
  
  color              user_layer_color;
  int                user_layer_line_width;
  
  void (*redraw_user_layer_fcn)(GeoWin*); 
  void (*postscript_user_layer_fcn)(GeoWin*, ps_file&); 
  
  // call back for user defined menu bars ...
  void (*user_menu_add_handler)(window&);
  
  
  // do we have to write the user layer to ps files
  bool               ps_output_user_layer;
  
  // for exporting the scene contents ...
  string replace_by_blanks;

  // geometrical parameters :
  // ************************

  // If |pin\_point| is not |0| |\Mname| is pined at this point. In this
  // case by default moves of geometrical object will be rotations around 
  // |pin\_point|
  point* pin_point;
  bool   menu_init;
  
  // handling of defaults (~/.geowinrc):
  // ***********************************
  color DEFAULT_color;
  color DEFAULT_color2;
  color DEFAULT_fill_color;
  int  DEFAULT_line_width, DEFAULT_active_line_width;
  line_style  DEFAULT_line_style;
  point_style DEFAULT_point_style;
  bool DEFAULT_show_coords;
  bool DEFAULT_time_meas;
  bool DEFAULT_gw_elim;
  bool DEFAULT_gw_solid;
  int DEFAULT_gw_anim_speed;
  int DEFAULT_anim_steps;
  
  //visible range when the GeoWin is constructed...
  double START_XMIN,START_YMIN,START_XMAX,START_YMAX;
  
  //visible ranges list (used for undo)
  list<rectangle> view_list;
  
  // test file existence when saving ...
  bool file_existence_test;
  
  bool save_defaults(string fname); //save the current default values 
  bool save_defaults();             //write them to ~/.geowinrc
  bool read_defaults(string fname); //read the current default values 
  bool read_defaults();             //read them from ~/.geowinrc 
  void reset_defaults();            //reset the default values ...

  void replace_characters(string& strh);

  // Methods :
  // *********
  // common part of all constructors
  void construct();

  // menu handling :
  // ---------------
  void call_handler(int n);  // n - position of the function to be called
  friend void geo_call_handler(int n);
  
  bool (*edit_loop_handler)(const GeoWin&);
  
  bool (*quit_handler)(const GeoWin& gw);
  bool (*done_handler)(const GeoWin& gw);

  void special_help_button_pressed();
  void done_button_pressed()     { button_info[GEO_CM_OK] = 2; }
  bool done_button_was_pressed();  

  void quit_button_pressed()     { button_info[GEO_CM_QUIT] = 2; }
  void quit_button_reset()       { button_info[GEO_CM_QUIT] = 1; }  
  bool quit_button_was_pressed();
  
  void   make_scene_menu(); // updates scene menu
  void   make_algo_menu(geo_scene sc);
  
  void   call_keyboard_read_object();
  void   call_mouse_read_object();

  // calls |activate| for the scene, for which |ids[geo_active_id] == id|
  void activate(int id);
  friend void geo_activate(int n);
  
  // calls member |options| for the scene, for which |ids[geo_opt_id] == id|
  void scene_options(int id);
  void scene_group_menu(int id);

 // calls member |contents| for the scene, for which |ids[geo_cont_id] == id|  
  void scene_contents(int id);
  
 // calls member |scene_description| for the scene with |ids[geo_desc_id] == id|  
  void scene_description(int id);  
  
  
  void active_scene_options();
  
  friend void geo_scene_option(int n);
  friend void geo_scene_description(int n);
  friend void geo_scene_algo(int n);
  friend void geo_scene_contents(int n);
  friend void geo_scene_groups(int n);

  // create a new scene of type editables[n] :
  void new_scene(int n, double dummy);
  friend void geo_new_scene_handler(int n);
  
  // close the currently active scene :
  void destroy() { if( cur ) destroy(cur); }
  
  // manipulate menu items:
  // ---------------------
  void enable_done_button();
  void disable_done_button();
  void enable_new_button();
  void disable_new_button();
  void enable_close_button();
  void disable_close_button();
  // disables all buttons useless with no scene
  void no_scene_on();
  // enables them again
  void no_scene_off();


  // options :
  // ---------

  // Panel for setting the scene defaults (stored in .geowinrc)
  void scene_defaults(); 
  // panel for the setting of grid distance and style, background color,
  // show_coordinates and time outs for double click and dragging
  void global_options();
  void advanced_global_options();
  // panel for the setting of minimal and Maximal coordinates
  void scaling_options();
  // panel for setting d3 output options
  void user_layer();
  // panel for adding/removing objects from the user layer 
  void d3_options();

  // handling :
  // ----------

  friend __exportGeoW void geo_move(GeoWin& gw, const point& p);
  friend __exportGeoW void geo_rotate(GeoWin& gw, const point& p);
  friend __exportGeoW void geo_object_dragging(GeoWin& gw, const point& p);
  friend __exportGeoW void geo_scroll_scene(GeoWin& gw, const point& p);

  // transformations
  void   transformation();

  // rotate selected object around pin point :
  void   rotate();
  void   rotate(const point& p);
  void   rotate(const point& p1, const point& p2);

  // move selected object with mouse cursor
  void   move();
  void   move(const point& p);
  void   move_animation(const point& p, geo_editor& ed);
  
  void   show_sel();
  void   paste_from();
 
  void transformation_menu() { transformation(); }
  void rotate_menu() { rotate(); }
  void move_menu()   { move();   }
  void select_menu() { select(); }
  void redraw_menu() { redraw(); }
  void destroy_menu(){ destroy();}
  void destroy_all();

  void help_about();
  void help_news ();
  
  geo_help_action get_help_action(string label);
  
  void help_user();  

  list<geo_scene> quick_access_scenes; // scenes with buttons in quick access window ...
  
  // visibility :
  // ------------
  
  void visible_scenes();
  void z_scenes();
  void scenegroup_dialog(geo_scenegroup gsg);

  void display_help_text(const list<string>& htext, const string& heading); 
  void init_help();
  
  // iniialization of the different menus
  void init_file_menu();
  void init_edit_menu();
  void init_scenes_menu();
  void init_window_menu();
  void init_options_menu();
  void init_algorithms_menu();
  void init_help_menu();
  
  bool show_menu; 
  //true ... show normal menus, false ... show only done button
  
  bool show_file_menu;
  bool show_edit_menu;
  bool show_scenes_menu;
  bool show_window_menu;
  bool show_options_menu;
  bool show_algorithms_menu;
  bool show_help_menu;
  //true ... show this menu, false ... don't show this menu
  
  bool show_input_item;
  
  window* separate_menu_win;
  bool win_created; // was main window created by GeoWin constructor ??

  public :

  void init_quick_access_window(); // initialize quick access window ...
  
  bool* visib;
  void change_visibility(bool*);
  
  // visibility/activate buttons ...
  bool vis_buttons;
  bool activate_buttons;
  
  // true ... show visibility buttons/ show activate buttons
  
  list<geo_scene> VAR_1;  // visibility of these scenes will be shown (string buttons)...
  list<geo_scene> VAR_2;  // visibility of these scenes will be shown (bitmap buttons)...
  list<string> VLABEL;    // button labels ...
  list<unsigned char*> VPIC_LIST; // bitmap pictures ....
  int VWIDTH, VHEIGHT; // width and height of the buttons ...
  unsigned char** VPICS;        // pixmaps for buttons ...
  
  list<geo_scene> ACT_SCN; 
  list<unsigned char*>    APIC_LIST;
  unsigned char**         APICS;
  
  // visibility item
  panel_item pnit1;
  panel_item pnit2;
  
  // activation item
  panel_item acit;
  
  // input objects selection
  panel_item inpit;
  
  int VIS_STATE; // state of visibility for scenes in VAR ...
  int ACT_STATE;
  int INP_STATE;
  
  //variables for saving generator numbers
  int gen_count, gen_count2;
  
  // for quick access window (scene activation)
  geo_scene get_quick_access_scene(int n);
  
  // file operations...
  void save_ps();
  void save_ps_vis(); // writes all visible scenes in ps - format...
  bool test_file_exists(string);
  void file_open();
  void write_ps(string filename);
  void write_ps_vis(string filename);
  void export_scene(string filename);
  void file_save();
  void file_save_active();
  void file_save_active_selected();
  void file_import();
  void file_export();
  void save_screen(); 
  void round_to_grid(double&,double&);
  void init_and_set_visible(geo_scene sc);
  void redraw_inp_panel();
  
  //updating active scene
  void update_active();
  
  //operations for getting scenes and GeoWin in update and result scenes ...
  static GeoWin* call_win;
  static geo_scene call_scene; 
  static geo_scene call_inp_scene;
  
  static GeoWin* get_call_geowin() { return call_win; }
  static geo_scene get_call_scene() { return call_scene; }
  static geo_scene get_call_input_scene() { return call_inp_scene; }

  bool get_copy_attr(geo_scene sc) { return sc->copy_attr; }  
  bool set_copy_attr(geo_scene sc, bool b) { bool old = sc->copy_attr; sc->copy_attr = b; return old; }
  
  //drawing mode for 3d objects 0 - xy, 1-xz, other - yz
  static int projection_mode;
  static int get_projection_mode() { return projection_mode; }

  /*{\Mcreation GW}*/

  GeoWin(const char* label = "GEOWIN"); 
  /*{\Mcreate creates a GeoWin $GW$. |\Mvar| is constructed with 
              frame label $label$}*/ 
	      
  GeoWin(int w, int h, const char* label = "GEOWIN"); 
  /*{\Mcreate creates a GeoWin $GW$ with frame label $label$ and 
              window size $w\times h$ pixels. }*/  
  
  GeoWin(window& win);
  /*{\Xcreate creates a GeoWin $GW$ using |win| as display window. |win| must be displayed before
      displaying |GW|. }*/    
  
  ~GeoWin();

  /*{\Moperations }*/
  
  /*{\Mtext
    \medskip
    {\bf a) Main Operations} }*/
#if !defined(__NO_MEMBER_TEMPLATES__)

  /*{\Mtext
    \medskip
    |In this section you find operations for creating scenes and for starting|
    |the interactive mode of GeoWin.|
    
    The |new_scene| and |get_objects| operations use member templates. If your 
    compiler does not support member templates, you should use instead
    the templated functions |geowin_new_scene| and |geowin_get_objects| with
    |\Mvar| as an additional first parameter.\\
    
    All |new_scene| operations can get as an optional last parameter a 
    pointer to a function that is used to compute the
    three-dimensional output of the scene.
    The type of such a function pointer |f| is 
    
    |void (*f)(const T&, d3_window&, GRAPH<d3_point,int>&))|
    
    where |T| is the type of the container used in the scene (for instance |list<point>|).
    The function gets a reference to the container of it's scene, a reference to the
    output |d3_window| and to the parametrized graph describing the three-dimensional
    output. The function usually adds new nodes and edges to this graph. Note that
    every edge in the graph must have a reversal edge ( and the reversal information
    has to be set). \\     
    Example:
    
\footnotesize
\begin{Mverb}
void segments_d3(const list<segment>& L,d3_window& W, 
                 GRAPH<d3_point,int>& H)
{
 GRAPH<d3_point,int> G;
 segment iter;
 forall(iter,L) {
   node v1 = G.new_node(d3_point(iter.source().xcoord(),
                                 iter.source().ycoord(),0));
   node v2 = G.new_node(d3_point(iter.target().xcoord(),
                                 iter.target().ycoord(),0));   
   edge e1 = G.new_edge(v1,v2);
   edge e2 = G.new_edge(v2,v1);
   G.set_reversal(e1,e2);
 }
 H.join(G);
}
\end{Mverb}
\normalsize    

    In this simple example the function gets a list of segments.
    For every segment in the list two new nodes and two new edges are created.
    The reversal information is set for the two edges. At the end the local graph 
    |G| is merged into |H|.

    The following templated |new_scene| operation can be used to create 
    edit scenes.
    The |CONTAINER| has to be a |list<T>| , where T is one of the following
    2d LEDA kernel type
\begin{itemize}
\item |(rat_)point|
\item |(rat_)segment|
\item |(rat_)line|
\item |(rat_)circle|
\item |(rat_)polygon|
\item |(rat_)gen_polygon|
\end{itemize}    
    or a |d3_point| or a |d3_rat_point|.
    If you want to use the other 2d LEDA kernel types, you have to include
    |geowin_init.h| and to initialize them for usage in |GeoWin| by calling
    the |geowin_init_default_type| function at the beginning of |main| (before an
    object of data type |\Mvar| is constructed).
    If you want to use the other 3d LEDA kernel types, you have to include
    |geowin_init_d3.h| and to initialize them for usage in |GeoWin| by calling
    the |geowin_init_default_type| function at the beginning of |main| (before an
    object of data type |\Mvar| is constructed).    
    }*/ 
    
  template<class CONTAINER> GeoEditScene<CONTAINER>* new_scene(CONTAINER& c) 
    /*{\Mopl creates a new edit scene and returns a pointer to the created scene.
     |c| will be the container storing the contents of the scene.}*/
  {
    return geowin_new_scene(*this, c, "", NULL);
  }       

  template<class CONTAINER> GeoEditScene<CONTAINER>* new_scene(CONTAINER& c, D3_FCN f) 
  { return geowin_new_scene(*this, c, "", f); } 
  
  template<class CONTAINER> GeoEditScene<CONTAINER>* new_scene(CONTAINER& c, string str) 
  { return geowin_new_scene(*this, c, str, NULL); }       

  template<class CONTAINER> GeoEditScene<CONTAINER>* new_scene(CONTAINER& c, string str, D3_FCN f) 
    /*{\Mopl creates a new edit scene and returns a pointer to the created scene. 
       |c| will be the container storing the contents of the scene.
        The name of the scene will be set to |str|.
    }*/
  {
    return geowin_new_scene(*this, c, str, f);
  } 
  
  /*{\Mtext
    \medskip
    The following |new_scene| operations can be used to create result scenes.
    Result scenes use the contents of another scene (the input scene) 
    as  input for a function (the update function). 
    This function computes the contents of the result scene. The update function 
    is called every time when the contents of the input scene changes.
    Instead of using an update function you can use an update object that encapsulates
    an update function.
    The type of this update object has to be |geowin_update<I,R>| 
    (|I| - type of the container in the input scene, |R| - type of the container 
    in the result scene) or a class derived from it. 
    A derived class should overwrite the virtual update function

     |void update(const I& in,R& out)| 
     
    of the base class to provide a user defined update function.
    The class |geowin_update<I,R>| has 3 constructors getting function pointers
    as arguments:
    
     |geowin_update(void (*f)(const I& in, R& res)|
     
     |geowin_update(void (*f)(const I& in, R::value_type& obj)|
     
     |geowin_update(R::value_type (*f)(const I& in)|
    
    When the update object is constructed by calling the constructor with one of these
    function pointers, the function |(*f)| will be called in the update method of the 
    update object.
    The first variant is the normal update function that gets the contents |in| of the input 
    scene and computes the contents |res| of the output scene. 
    In the second variant the contents of the result scene will first be cleared, then the
    update function will be called and |obj| will be inserted in the result scene.
    In the third variant the contents of the result scene will be cleared, and then
    the object returned by |(*f)| will be inserted in the result scene.
    The class |geowin_update| has also the following virtual functions:
    
     |bool insert(const InpObject& new)|
 
     |bool del(const InpObject& new)|
 
     |bool change(const InpObject& old_obj,const InpObject& new_obj)|
		  
    where 
    |new| is a new inserted or deleted object and |old_obj| and |new_obj| are 
    objects before and after a change.
    |InpObject| is the value type of the container of the input scene.
    With these functions it is possible to support incremental algorithms. The functions will
    be called, when in the input scene new objects are added ($insert$), deleted ($del$) or 
    changed when performing a move or rotate operation ($change$).
    In the base class |geowin_update<I,R>| these functions return |false|. 
    That means, that the standard update-function
    of the update object should be used. But in derived classes it is possible to overwrite
    these functions and provide user-defined update operations for these three incremental
    operations. Then the function has to return
    |true|. That means, that the standard update function of the update object should not be used.
    Instead the incremental operation performs the update-operation.
    
    It is also possible to provide user defined redraw for a scene.
    For this purpose we use redraw objects derived from |geowin_redraw|.
    The derived class has to overwrite the virtual redraw function

     |void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2)|
     
    of the base class to provide a user defined redraw function. The first 3 parameters
    of this function are the redraw window and the first and second drawing color (|color| 
    and |color2|) of the scene.
    The class |geowin_redraw| has also a virtual method 
    
     |bool draw_container()|
     
    that returns |false| in the base class. If you want the user defined redraw of the scene
    (provided by the redraw function |draw|) and the execution of the 'normal' redraw of the scene as well
    (output of the objects stored in the container of the scene), you have to overwrite
    |draw_container| in a derived class by a function returning |true|. 
    A virtual method
     
     |bool write_postscript(ps_file& PS,color c1,color c2)|
          
    is provided for output to a LEDA postscript file |PS|. |c1| and |c2| are the first and 
    second drawing color (|color| and |color2|) of the scene.
    Another class that can be used for user defined redraw is the templated class
    |geowin_redraw_container<CONTAINER>|. This class has as well virtual functions for redraw and postscript
    output, but provides a slighly changed interface:
    
     |bool draw(const CONTAINER& c, window& w, color c1, color c2, double,double,double,double)|
     \\
     \\
     |bool write_postscript(const CONTAINER& c, ps_file& ps, color c1, color c2)|
     
    The parameters of these two virtual functions are like the parameters of the members with the same name
    of |geowin_redraw|, but there is an additional first parameter. This parameter is a reference to the container of
    the scene that has to be redrawn.
    
    In update- and redraw- functions and objects the following static member functions 
    of the |GeoWin| class can be used:
    
    |GeoWin*    GeoWin::get_call_geowin()|
    \\
    |geo_scene  GeoWin::get_call_scene()|
    \\
    |geo_scene  GeoWin::get_call_input_scene()|   
    
    The first function returns a pointer to the |GeoWin| of the calling scene,
    the second returns the calling scene and the third (only usable in update functions/
    objects) returns the input scene of the calling scene.
    
    Note that |S| and |R| in the following operations are template parameters.
    |S| and |R| have to be a |list<T>|, where T is a 2d LEDA kernel type,
    a |d3_point| or a |d3_rat_point|.
    |S| is the type of the contents of the input scene, |R| the type of the contents
    of the created result scene.
    All operations creating result scenes return a pointer to the created result scene.
    
    This section contains three small example programs showing you the usage of the |new_scene| 
    operations for the creation of result scenes. All example programs compute the convex hull of
    a set of points stored in the container of an input scene |sc_points| and store the computed hull in a
    result scene |sc_hull|.
 }*/

  template<class S, class R> 
  GeoResultScene<S,R>* new_scene(void (*f_update)(const S&, R&), geo_scene sc, string str, D3_FCN f = NULL)
    /*{\Mopl  creates a new result scene with name |str|.
             The input scene for this new result scene will be |sc|.
             The update function will be |f_update|. }*/
  { return geowin_new_scene(*this, f_update, sc, str,f );  }

/*{\Mtext
The first example program shows the usage of the |new_scene| operation 
taking an update function pointer. The update function computes the convex hull of 
the points stored in the input scene. The result polygon will be inserted in the container |P| of
the result scene.

\footnotesize
\begin{Mverb}  
#include <LEDA/graphics/geowin.h>
#include <LEDA/geo/float_geo_alg.h>

using namespace leda;

void convex_hull(const list<point>& L, list<polygon>& P)
{ P.clear(); P.append(CONVEX_HULL_POLY(L)); }

int main()
{
 GeoWin gw;
 list<point> LP;
 
 geo_scene sc_points = gw.new_scene(LP);
 geo_scene sc_hull = gw.new_scene(convex_hull, sc_points, "Convex hull");
 gw.set_color(sc_hull, blue);
 gw.set_visible(sc_hull, true);
  
 gw.edit(sc_points);
 return 0;
}
\end{Mverb}
\normalsize

}*/

  //result scene for update objects...
  template<class S,class R>
  GeoResultScene<S,R>* new_scene(geowin_update<S,R>& up, list<geo_scene>& infl, string str, D3_FCN f = NULL)
    /*{\Mopl  creates a new result scene |scr| with name |str|.
             The input scene for this new result scene will be the first scene in |infl|.
             The update object will be |up|. |up| has to be constructed by a call |up(fu,0)|, where |fu| is a function of
	     type |void fu(const C0&, const C1&, ..., const Cn&, R&)|.
	     |infl| is a list of scenes influencing the result scene. 
	     |C0|,...,|Cn| are the types of the containers of the scenes in |infl|. When  one of the 
	     scenes in |infl| changes, |fu| will be called to update the contents of |scr|.
	     \precond |infl| must not be empty.}*/
  { return geowin_new_scene(*this,up,infl,str,f); }  
  


  template<class S, class R>
  GeoResultScene<S, list<geo_graph> >* new_scene(geowin_graph_update<S,R>& up, list<geo_scene>& infl, string str, D3_FCN f = NULL)  
  { GeoResultScene<S,list<geo_graph> >* res = geowin_new_scene(*this, up,infl,str,f); 
    res->init_d3_window = f;
    return res;
  }  
    
  
  template<class S,class R>
  GeoResultScene<S,R>* new_scene(geowin_update<S,R>& up, geo_scene sc_input, string str, D3_FCN f = NULL)
    /*{\Mopl  creates a new result scene with name |str|.
             The input scene for this new result scene will be |sc_input|.
             The update object will be |up|. }*/
  { return geowin_new_scene(*this,up,sc_input,str,f); }
  
  template<class S,class R>
  GeoResultScene<S, list<geo_graph> >* new_scene(geowin_graph_update<S,R>& up, geo_scene sc_input, string str, D3_FCN f = NULL)
  { GeoResultScene<S, list<geo_graph> >* res = geowin_new_scene(*this, up,sc_input,str,f);
    res->init_d3_window = f;
    return res;
  }  

/*{\Mtext
The second variant of the example program uses an update object |update|.

\footnotesize
\begin{Mverb}  
#include <LEDA/graphics/geowin.h>
#include <LEDA/geo/float_geo_alg.h>

using namespace leda;

int main()
{
 GeoWin gw;
 list<point> LP;
 
 geo_scene sc_points = gw.new_scene(LP);
 
 geowin_update<list<point>, list<polygon> > update(CONVEX_HULL_POLY);
 geo_scene sc_hull = gw.new_scene(update, sc_points, "Convex hull");
 gw.set_color(sc_hull, blue);
 gw.set_visible(sc_hull, true);
  
 gw.edit(sc_points);
 return 0;
}  
\end{Mverb}  
\normalsize  

}*/  
  
  template<class S,class R>
  void set_update(geo_scene res, geowin_update<S,R>& up)
  /*{\Mopl    makes |up| the update object of |res|. \precond |res| points to a scene of type
              |GeoResultScene<S,R>| .}*/
  { geowin_set_update(res,up); }
  
  template<class S,class R>
  void set_update(geo_scene res, void (*f_update)(const S&, R&))
  /*{\Mopl    makes |f_update| the update function of |res|. \precond |res| points to a scene of type
              |GeoResultScene<S,R>| .
  }*/
  { geowin_set_update(res,f_update); }  
     
  template<class S,class R>
  GeoResultScene<S,R>* new_scene(geowin_update<S,R>& up, geowin_redraw& rd, geo_scene sc_input, string str, D3_FCN f = NULL)
    /*{\Mopl  creates a new result scene with name |str|.
             The input scene for this new result scene will be |sc_input|.    
             The update object will be |ub|.  
	     The redraw object will be |rd|. }*/
  { return geowin_new_scene(*this,up,rd,sc_input,str,f); }
  

  template<class S,class R>
  GeoResultScene<S,list<geo_graph> >* new_scene(geowin_graph_update<S,R>& up, geowin_redraw& rd, geo_scene sc_input, string str, D3_FCN f = NULL)
  { GeoResultScene<S,list<geo_graph> >* res = geowin_new_scene(*this, up,rd,sc_input,str,f);
    res->init_d3_window = f;
    return res;
  }  
  
  
/*{\Mtext
The third variant of the example program uses an update and redraw object. We provide a user defined
class for update and redraw of the result scene.

\footnotesize
\begin{Mverb}
#include <LEDA/graphics/geowin.h>
#include <LEDA/geo/float_geo_alg.h>

using namespace leda;

class hull_update_redraw : public geowin_update<list<point>, list<polygon> > ,
                           public geowin_redraw
{
 list<polygon> polys;
public:
 void update(const list<point>& L, list<polygon>& P)
 { 
  polys.clear();
  polys.append(CONVEX_HULL_POLY(L));
 }
 
 void draw(window& W,color c1,color c2,double x1,double y1,double x2,double y2)
 {
  polygon piter;
  segment seg;
  forall(piter, polys){
    forall_segments(seg, piter){
      W.draw_arrow(seg, c1);
    }
  }
 }
};

int main()
{
 GeoWin gw;
 list<point> LP;
 
 geo_scene sc_points = gw.new_scene(LP);
 
 hull_update_redraw up_rd;
 geo_scene sc_hull = gw.new_scene(up_rd, up_rd, sc_points, "Convex hull");
 gw.set_color(sc_hull, blue);
 gw.set_visible(sc_hull, true);
  
 gw.edit(sc_points);
 return 0;
}  
\end{Mverb}  
\normalsize 
 
}*/  
    
  
  template<class S,class R>
  GeoResultScene<S,R>* new_scene(geowin_update<S,R>& up, geowin_redraw_container<R>& rd, geo_scene sc_input, string str, D3_FCN f = NULL)
    /*{\Mopl  creates a new result scene with name |str|.
             The input scene for this new result scene will be |sc_input|.    
             The update object will be |ub|.  
	     The redraw container object will be |rd|. }*/
  { return geowin_new_scene(*this,up,rd,sc_input,str,f); }

  template<class S,class R>
  GeoResultScene<S,list<geo_graph> >* new_scene(geowin_graph_update<S,R>& up, geowin_redraw_container<R>& rd, geo_scene sc_input, string str, D3_FCN f = NULL)
  { GeoResultScene<S,list<geo_graph> >* res = geowin_new_scene(*this, up,rd,sc_input,str,f);  
    res->init_d3_window = f; 
    return res;
  }   

  template<class CONTAINER> bool get_objects(CONTAINER& c)
    /*{\Mopl  If the container storing the contents of the current edit scene has type |CONTAINER|, 
             then the contents of this scene is copied to |c|. }*/
  {
    return get_objects(get_active_scene(), c);
  }
  
  template<class CONTAINER> bool get_objects(geo_scene sc, CONTAINER& c)
    /*{\Mopl  If the container storing the contents of scene |sc| has type |CONTAINER|, 
             then the contents of scene |sc| is copied to |c|. }*/
  {
    if( sc ) return geowin_get_objects(*this, sc, c);
    else return false;
  }

  template<class CONTAINER> bool set_objects(geo_scene sc, CONTAINER& c)
  {
    if( sc ) return geowin_set_objects(*this, sc, c);
    else return false;
  }

  
  template <class CONTAINER>
  void get_selected_objects(GeoEditScene<CONTAINER>* sc, CONTAINER& cnt)
    /*{\Mopl  returns the selected objects of scene |sc| in container |cnt|.}*/  
  {
    geowin_get_selected_objects(*this, sc, cnt);
  }
  
 
  template <class CONTAINER>
  void set_selected_objects(GeoEditScene<CONTAINER>* sc,const list<typename CONTAINER::iterator>& LIT)
    /*{\Mopl  selects the objects of scene |sc| described by the contents of container |LIT|.}*/  
  {
    geowin_set_selected_objects(*this, sc, LIT);
  }
  
  template <class CONTAINER>
  void set_selected_objects(GeoEditScene<CONTAINER>* sc)
    /*{\Mopl  selects all objects of scene |sc|.}*/  
  {
    geowin_set_selected_objects(*this, sc);
  }  
  
  template <class CONTAINER>
  void set_selected_objects(GeoEditScene<CONTAINER>* sc, const rectangle& R)
  /*{\Mopl  selects all objects of scene |sc| contained in rectangle |R|.}*/  
  {
    geowin_set_selected_objects(*this, sc, R);
  }    
  
  template<class CONTAINER>
  void set_selected_objects(GeoEditScene<CONTAINER>* sc, geowin_handle_object<typename CONTAINER::value_type>& h)
  /*{\Xopl  selects all objects of scene |sc| confirmed by |h|.}*/  
  {
     geowin_set_selected_objects(*this, sc, h);
  }  
  
  template <class CONTAINER>
  const CONTAINER& get_objects_reference(GeoBaseScene<CONTAINER>* sc)
  {
    return sc->get_objref();
  }
#endif


  void edit();
  /*{\Mop starts the interactive mode of |\Mvar|. The operation returns 
         if either the $DONE$ or $Quit$ button was pressed. }*/

  bool edit(geo_scene sc);
  /*{\Mop  edits scene $sc$. Returns |false| if the Quit-Button was pressed,
          |true| otherwise. }*/

  // --------------------------------------------------------------------------
  // handling of other windows in the edit loop ...
  bool is_registered_window(window* win);
  
  bool handle_registered_window(window* win, int event, int but, double x, double y);  
  
  void register_window(window& win, bool (*ev_fcn)(window* w, int event, int but, double x, double y) );
  /*{\Mop if you enter the interactive mode of |\Mvar| in an application, but you
  want to handle events of other windows as well, you can register  a callback function |ev_fcn|
  for your other window |win| that will be called when events associated with |win| occur.
  The parameters of |ev_fcn| are the window causing the event, the event that ocurred, 
  the button and the |x| and |y|
  coordinates of the position in |win|. The handler |ev_fcn| has to return |true| if
  the interactive mode of |GeoWin| has to be stopped, |false| otherwise.
  }*/
    
  // --------------------------------------------------------------------------
 
/*{\Mtext
\medskip
{\bf Simple Animations}

The following operation can be used to perform simple animations. One can
animate the movement of selected objects of a scene. This can be done in the
following way: select a number of objects in an edit scene; then start the
animation by calling the |animate| member function. The second parameter
of this member function is an object |anim| of type |geowin_animation|, the first parameter is the
scene that will be animated.
The object |anim| has to be derived from the abstract base class |geowin_animation|.
The derived class has to overwrite some methods of the base class:

\footnotesize
\begin{Mverb}
class geowin_animation { 
public:
 virtual void init(const GeoWin&) { }
 virtual void finish(const GeoWin&) { }
 virtual bool is_running(const GeoWin&) { return true; }
 virtual point get_next_point(const GeoWin&) = 0;
 virtual long  get_next_action(const GeoWin&) 
 { return GEOWIN_STOP_MOVE_SELECTED; }
};
\end{Mverb}
\normalsize

At the start and at the end of an animation the member functions $init$ and $finish$ are
called. The animation is stopped if |is_running| returns false. The member functions
|get_next_point| and |get_next_action| specify the animation. |get_next_point| delivers
the next point of the animation path. |get_next_action| currently can return two
values: |GEOWIN_MOVE_SELECTED| (moves the selected objects of the scene) and |GEOWIN_STOP_MOVE_SELECTED|
(stops the movement of the selected objects of the scene). 

}*/    
  
  
  bool animate(geo_scene sc, geowin_animation& anim);
  /*{\Mopl  starts animation $anim$ for edit scene $sc$.}*/

  /*{\Mtext
    \medskip
    {\bf b) Window Operations} }*/
    
  void draw_user_layer(); 
  void ps_user_layer(ps_file& F); 
  
  static void redraw_geowin(window* w, double x0, double y0, 
			    double x1, double y1);
  static void coord_handler(window*,double,double); 
  bool get_time_meas() { return time_meas; }

  void lock() { is_locked = true;}
  void unlock() { is_locked = false;}  
  
  void close() { Wp->close(); }
  /*{\Mop closes |\Mvar| .}*/
  
  double get_xmin() const;
  /*{\Mopl returns the minimal x-coordinate of the drawing area.}*/
  
  double get_ymin() const;
  /*{\Mopl returns the minimal y-coordinate of the drawing area.}*/  
  
  double get_xmax() const;
  /*{\Mopl returns the maximal x-coordinate of the drawing area.}*/
    
  double get_ymax() const;
  /*{\Mopl returns the maximal y-coordinate of the drawing area.}*/  

  void display(int x = window::center ,int y = window::center);
  /*{\Mop opens |\Mvar| at |(x,y)|. }*/
  
  window& get_window() const { return *Wp; }
  /*{\Mopl returns a reference to the drawing window. }*/
  
  void init(double xmin, double xmax, double ymin);
  /*{\Mopl     same as $window$::init($xmin$, $xmax$, $ymin$, $g$).}*/
  
  void init(double x1, double x2, double y1, double y2, int r=GEOWIN_MARGIN);
  /*{\Mopl inializes the window so that the rectangle with lower 
           left corner $(x1-r,y1-r)$ and upper right corner $(x2+r,y2+r)$
	   is visible. The window must be open. $GEOWIN\_MARGIN$ is a
	   default value provided by |\Mname|. }*/

  void redraw();
  /*{\Mopl redraws the contents of |\Mvar| (all visible scenes).}*/
  
  void redraw(double x0, double y0, double x1, double y1);
  /*{\Xopl redraws objects in the rectangle with lower 
           left corner $(x0,y0)$ and upper right corner $(x1,y1)$.}*/

  void update_status_line();
  
  void redraw_and_update_status_line();
  
  int  set_cursor(int cursor_id = -1) { return Wp->set_cursor(cursor_id); }
  /*{\Mopl sets the mouse cursor to $cursor\_id$. }*/
  
  bool get_show_status() const
   /*{\Mopl return }*/
  { return show_status; }
  
  bool set_show_status(bool b)
  /*{\Mopl  display a status window ($b$={\it true}) or not ($b$={\it false}).
  The operation should be called before the first $display$ - operation of |\Mvar|.  }*/
  { bool prev = show_status; show_status = b; return prev; }
  
/*{
\Mtext
The following operations can be used to change the menu bar of a |GeoWin|. They have to be
called before the first edit- or display-operation.
}*/  
  
  bool set_show_menu(bool v);
  /*{\Mopl sets the visibility of the menu of |\Mvar| to |v|. }*/
  
  void set_menu_add_fcn(void (*mfcn)(window& W))
  /*{\Mopl This handler function can be used to add own menus to the menu bar of a |GeoWin|.
           It is called before the menu initialization of a |GeoWin|. See the demo
	   program |geowin_gui| for an example.
  }*/
  { user_menu_add_handler = mfcn; }
  
  // set show operations for the different menus ...
  bool set_show_file_menu(bool v);
  /*{\Mopl sets the visibility of the file menu of |\Mvar| to |v|.}*/
  
  bool set_show_edit_menu(bool v);
  /*{\Mopl sets the visibility of the edit menu of |\Mvar| to |v|.}*/
  
  bool set_show_scenes_menu(bool v);
  /*{\Mopl sets the visibility of the scenes menu of |\Mvar| to |v|.}*/
  
  bool set_show_window_menu(bool v);
  /*{\Mopl sets the visibility of the window menu of |\Mvar| to |v|.}*/
  
  bool set_show_options_menu(bool v);
  /*{\Mopl sets the visibility of the options menu of |\Mvar| to |v|.}*/
  
  bool set_show_algorithms_menu(bool v);
  /*{\Mopl sets the visibility of the algorithms menu of |\Mvar| to |v|.}*/
  
  bool set_show_help_menu(bool v);  
  /*{\Mopl sets the visibility of the help menu of |\Mvar| to |v|.}*/
  
  void init_menu(window* wptr = NULL);
  /*{\Mopl initializes the menu of |\Mvar|. Normally you don't have to call this operation 
  directly, but if you want to add additional graphical elements like sliders or buttons to
  the window of |\Mvar| you have to call |init_window| (with no parameters). After that add
  the desired elements and then call |edit| or |display|. See the demo programs for
  examples.}*/
  
  void hide_buttons();
  /*{\Xopl hides the scene activation, visibility and input object buttons of |\Mvar|. }*/  

  /*{\Mtext
    \medskip
    {\bf c) Scene and scene group Operations} }*/

  void insert_scene(geo_scene sc);
  void remove(geo_scene sc);
  // removes scene |sc| from $GW$. Contents of |sc| will not be destroyed.
  
  void destroy(geo_scene sc);
  /*{\Xopl removes scene |sc| from $GW$ and destroys it. }*/

  geo_scene get_scene_with_name(string nm) const;
  /*{\Mopl returns the scene with name |nm| or nil if there is no scene
           with name |nm|.}*/
	   
  void   activate(geo_scene sc);
  /*{\Mopl  makes scene |sc| the active scene of |\Mvar|. }*/ 
  
  // operations for setting/ getting z - values for scenes 
  int get_z_order(geo_scene sc) const;
  /*{\Mopl  returns the |z|-coordinate of |sc|.}*/
  
  int set_z_order(geo_scene sc,int n);
  /*{\Mopl  sets the |z|-coordinate of |sc| to |n| and returns its previous value.}*/
  
  // user layer operations
  
  /*{\Mtext
    \medskip
    In front of the scenes of a |GeoWin| object a so-called "user layer" can store
    some geometric objects illustrating scenes. The following functions let you add
    some of these objects.
  }*/  
  
  void add_user_layer_segment(const segment& s);
  /*{\Mopl adds segment |s| to the segments of the user layer.}*/
  
  void add_user_layer_circle(const circle& c);
  /*{\Mopl adds circle |c| to the circles of the user layer.}*/  
  
  void add_user_layer_point(const point& p);
  /*{\Mopl adds point |p| to the points of the user layer.}*/    
  
  void add_user_layer_rectangle(const rectangle& r);  
  /*{\Mopl adds rectangle |r| to the rectangles of the user layer.}*/    
  
  void remove_user_layer_objects();
  /*{\Mopl removes all objects of the user layer.}*/
  
  void set_draw_user_layer_fcn(void (*fcn)(GeoWin*))
  /*{\Mopl this function can be used for additional user-defined redraw after drawing the
           objects of the user layer.
  }*/ 
  { redraw_user_layer_fcn=fcn; }
  
  void set_postscript_user_layer_fcn(void (*fcn)(GeoWin*, ps_file&))
  /*{\Mopl }*/ 
  { postscript_user_layer_fcn=fcn; }
  
  geo_scene get_active_scene() const;
  /*{\Mopl  returns the active scene of |\Mvar|. }*/ 
  
  bool is_active(geo_scene sc) const { return sc->get_active(); }
  /*{\Mopl returns true if |sc| is an active scene in a |\Mname|. }*/

  /*{\Mtext
    \medskip
    The following $get$ and $set$ operations can be used for retrieving and
    changing scene parameters. All $set$ operations return the previous 
    value. }*/
  
  bool get_algo_create_scenes() { return algo_create_scenes; }
  
  bool set_algo_create_scenes(bool b) { bool v = algo_create_scenes; algo_create_scenes = b; return v; }

  string get_name(geo_scene sc) const { return sc->get_name(); }
  /*{\Mopl returns the |name| of scene |sc|. }*/ 
  
  string get_name(geo_scenegroup gs) const { return gs->name; }
  /*{\Mopl returns the |name| of scene group |gs|. }*/   
    
  string set_name(geo_scene sc, string nm);
  /*{\Mopl  gives scene |sc| the name |nm|. If there is already
           a scene with name |nm|, another name is constructed
	   based on |nm| and is given to |sc|. The operation 
	   will return the given name.
 }*/ 
 
  string get_short_name(geo_scene sc);
 
  string set_short_name(geo_scene sc, string sn);
  
  color get_color(geo_scene sc) const;
  /*{\Mopl returns the boundary drawing color of scene |sc|. }*/

  color set_color(geo_scene sc, color c);
  /*{\Mopl sets the boundary drawing color of scene |sc| to |c|. }*/
  
  void set_color(geo_scenegroup gs, color c);
  /*{\Mopl sets the boundary drawing color of all scenes in group |gs| to |c|.}*/

  color get_selection_color(geo_scene sc) const { return get_color2(sc); }
  /*{\Mopl returns the boundary drawing color for selected objects
          of scene |sc|. }*/

  color set_selection_color(geo_scene sc, color c){ return set_color2(sc,c); }
  /*{\Mopl sets the boundary drawing color for selected objects
          of scene |sc| to |c|. }*/
	
  void set_selection_color(geo_scenegroup gs, color c);
  /*{\Mopl sets the boundary drawing color for selected objects
          of all scenes in |gs| to |c|. }*/  
	  
  color get_selection_fill_color(geo_scene sc) const; 
  /*{\Mopl returns the fill color for selected objects
          of scene |sc|. }*/   
	  
  color set_selection_fill_color(geo_scene sc, color c);  
  /*{\Mopl sets the fill color for selected objects
          of scene |sc| to |c|. }*/  
  
  line_style get_selection_line_style(geo_scene sc) const;
  /*{\Mopl returns the line style for selected objects
          of scene |sc|. }*/   
  
  line_style set_selection_line_style(geo_scene sc, line_style l) const; 
  /*{\Mopl sets the line style for selected objects
          of scene |sc| to |l|. }*/   
  
  int get_selection_line_width(geo_scene sc) const;
  /*{\Mopl returns the line width for selected objects
          of scene |sc|. }*/   
  
  int set_selection_line_width(geo_scene sc, int w) const;    
  /*{\Mopl sets the line width for selected objects
          of scene |sc| to |w|. }*/  

  color get_color2(geo_scene sc) const;
  color set_color2(geo_scene sc, color c);

  color get_fill_color(geo_scene sc) const;
  /*{\Mopl returns the fill color of |sc|.}*/  
 
  color set_fill_color(geo_scene sc,color c);
  /*{\Mopl sets the fill color of |sc| to |c|. Use color $invisible$ 
           to disable filling.}*/ 

  void set_fill_color(geo_scenegroup gs, color c);
  /*{\Mopl sets the fill color of all scenes in |gs| to |c|. 
          Use color $invisible$ to disable filling.}*/   
	  
  color get_text_color(geo_scene sc) const;
  /*{\Mopl returns the text color of |sc|.}*/  
 
  color set_text_color(geo_scene sc,color c);
  /*{\Mopl sets the text color of |sc| to |c|.}*/ 

  void set_text_color(geo_scenegroup gs, color c);
  /*{\Mopl sets the text color of all scenes in |gs| to |c|.}*/	   

  int get_line_width(geo_scene sc) const { return sc->back_line_width; }
  /*{\Mopl returns the line width of scene |sc| .}*/

  int get_active_line_width(geo_scene sc) const 
  /*{\Mopl returns the active line width of |sc| .}*/
  { return sc->active_line_width; }
  
  int set_line_width(geo_scene sc,int w) 
  /*{\Mopl sets the line width for scene |sc| to |w|.}*/
  { int val=sc->back_line_width; sc->back_line_width=w; return val;}
  
  void set_line_width(geo_scenegroup gs,int w);
  /*{\Mopl sets the line width for all scenes in |gs| to |w|.}*/
  
  int set_active_line_width(geo_scene sc,int w) 
  /*{\Mopl sets the active line width of scene |sc| to |w|.}*/
  { int val=sc->active_line_width; sc->active_line_width=w; return val; }
  
  void set_active_line_width(geo_scenegroup gs,int w);
  /*{\Mopl sets the active line width for all scenes in |gs| to |w|.}*/  

  line_style get_line_style(geo_scene sc) const { return sc->l_style; }
  /*{\Mopl returns the line style of |sc|.}*/
  
  line_style set_line_style(geo_scene sc, line_style l) 
  /*{\Mopl sets the line style of scene |sc| to |l|.}*/
  { line_style val=sc->l_style; sc->l_style=l; return val; }
  
  void set_line_style(geo_scenegroup gs, line_style l);
  /*{\Mopl sets the line style of all scenes in |gs| to |l|.}*/  
  
  bool get_visible(geo_scene sc) const;
  /*{\Mopl returns the visible flag of scene |sc|. }*/
  
  bool set_visible(geo_scene sc, bool v);
  /*{\Mopl sets the visible flag of scene |sc| to |v|. }*/
  
  void set_visible(geo_scenegroup gs, bool v);
  /*{\Mopl sets the visible flag of all scenes in |gs| to |v|. }*/  
  
  void set_all_visible(bool v);
  /*{\Mopl sets the visible flag of all scenes that are 
          currently in |\Mvar| to |v|.}*/
  
  point_style get_point_style(geo_scene sc) const { return sc->p_style; }
  /*{\Mopl returns the point style of |sc|.}*/
  
  point_style set_point_style(geo_scene sc, point_style p) 
  /*{\Mopl sets the point style of |sc| to |p|}*/ 
  { point_style pold= sc->p_style; sc->p_style=p; return pold; }
  
  void set_point_style(geo_scenegroup gs, point_style p);
  /*{\Mopl sets the point style of all scenes in |gs| to |p|}*/   
  
  bool get_cyclic_colors(geo_scene sc) const { return sc->cyclic_colors; }  
  /*{\Mopl returns the cyclic colors flag for editable scene |sc|. }*/
  
  bool set_cyclic_colors(geo_scene sc, bool b) { bool old = sc->cyclic_colors; sc->cyclic_colors = b; return old; }  
  /*{\Mopl sets the cyclic colors flag for editable scene |sc|. If the cyclic colors
  flag is set, the new inserted objects of the scene get color |counter%16|, where
  counter is the object counter of the scene.}*/
  
  void set_cyclic_colors_counter(geo_scene sc, int c) 
  {sc->cyclic_colors_counter = c; }  

  string get_description(geo_scene sc) const;
  /*{\Mopl returns the description string of scene |sc|.}*/
  
  string set_description(geo_scene sc, string desc); 
  /*{\Mopl sets the description string of scene |sc| to |desc|. The description string has the task
           to describe the scene in a more detailed way than the name of the scene does.}*/
	   
  // new: showing of orientation/direction of objects ...
  bool   get_show_orientation(geo_scene sc) const;
  /*{\Mopl   returns the show orientation/direction parameter of scene |sc|}*/
  
  bool   set_show_orientation(geo_scene sc, bool o);
  /*{\Mopl   sets the show orientation/direction parameter of scene |sc| to |o|.}*/  
  	   

  void* get_client_data(geo_scene sc, int i=0) const { return sc->get_client_data(i); }
  /*{\Mop   returns the $i$-th client data pointer of of scene |sc|. 
            \precond $i < 16$. }*/ 
  
  void* set_client_data(geo_scene sc, void* p, int i=0) { return sc->set_client_data(p,i); }
  /*{\Mopl    sets the $i$-th client data pointer of scene |sc| to $p$ and 
              returns its previous value. \precond $i < 16$. }*/  
	      
  bool get_highlight(geo_scene sc) const;
  /*{\Xopl    returns the highlight flag of scene |sc|. If it is true, the object under
              the mouse cursor will be drawn with the color of the selected objects, if |sc|
	      is active.
  }*/
  
  bool set_highlight(geo_scene sc, bool hl);
  /*{\Xopl    sets the highlight flag of scene |sc| to |hl| and returns its previous value.}*/
  

  void set_handle_defining_points(geo_scene sc, geowin_defining_points gdp);
  /*{\Mopl    sets the attribute for handling of defining points of editable scene |(*sc)| to
              |gdp|. Options for |gdp| are |geowin_show| (show the defining points of all objects of
	      the scene, |geowin_hide| (hide the defining points of all objects of the scene) and
	      |geowin_highlight| (shows only the defining points of the object under the mousepointer).
  }*/

  geowin_defining_points get_handle_defining_points(geo_scene sc);
  /*{\Mopl    returns the attribute for handling of defining points of editable scene |(*sc)|. 
  }*/
  
  /*{\Mtext
    \medskip 
     The following operations can be used for getting/setting a flag influencing the behaviour of
     incremental update operations in result scenes. If |update_state| is 
     |true| (default) : if the first incremental operation returns false , incremental update loop
     will be left \\
     |false| : the incremental update loop will be executed until the end \\
     You can also set an |update_limit| for the incremental update operations. If a number of objects
     bigger than this limit will be added/deleted/changed, the incremental update will not be executed.
     Instead the "normal" scene update operation will be used.
  }*/
  
  bool    get_incremental_update_state(geo_scene sc) const { return sc->br_after_false; }
  /*{\Mopl   returns the incremental update flag of scene |sc|.  
  }*/  
  
  bool    set_incremental_update_state(geo_scene sc, bool us);
  /*{\Mopl   sets the incremental update flag of scene |sc| to |us|. 
  }*/
  

  int     get_incremental_update_limit(geo_scene sc) const;
  /*{\Mopl   returns the incremental update limit of scene |sc|. 
  }*/  
  
  int     set_incremental_update_limit(geo_scene sc, int l);
  /*{\Mopl   sets the incremental update limit of scene |sc| to |l|. 
  }*/  
  
  
  /*{\Mtext
    \medskip
    It is not only possible to assign (graphical)  attributes to a whole scene.
    
    The following operations can be used to set/get 
    individual attributes of objects in scenes.
    All set operations return the previous value.
    The first parameter is the scene, where the object belongs to.
    The second parameter is a generic pointer to the object or an iterator
    pointing to the position of the object in the container of a scene.
    Precondition: the object belongs to the scene (is in the container
    of the scene).\\
    Note that you cannot use a pointer to a copy of the object.\\
    The following example program demonstrates the setting of individual object
    attributes in an update member function of an update class:

\footnotesize    
\begin{Mverb}
#include <LEDA/graphics/geowin.h>
#include <LEDA/geo/rat_geo_alg.h>

using namespace leda;

class attr_update : public geowin_update<list<rat_point>, list<rat_circle> >  
{
 void update(const list<rat_point>& L, list<rat_circle>& C)
 {
  GeoWin* GW_ptr = GeoWin::get_call_geowin(); 
  GeoBaseScene<list<rat_circle> >* aec = 
   (GeoBaseScene<list<rat_circle> >*) GeoWin::get_call_scene();
  
  C.clear();
  if (! L.empty()) {
    ALL_EMPTY_CIRCLES(L,C);

    // now set some attributes
    list<rat_circle>::iterator it = C.begin();
    int cw=0;
    for(;it!=C.end();it++) { 
      GW_ptr->set_obj_fill_color(aec,it,color(cw % 15)); 
      GW_ptr->set_obj_color(aec,it,color(cw % 10));     
      cw++;
    }
  } 
 }
};

int main()
{
  GeoWin GW("All empty circles - object attribute test");
  
  list<rat_point> L;
  geo_scene input =  GW.new_scene(L); 
  GW.set_point_style(input, disc_point);

  attr_update aec_help;
  geo_scene aec = GW.new_scene(aec_help, input, string("All empty circles")); 
  
  GW.set_all_visible(true);
  GW.edit(input); 
  return 0;  
}
\end{Mverb}  
\normalsize    

  }*/
  
#if !defined(__NO_MEMBER_TEMPLATES__)
  
  template<class T>
  color get_obj_color(GeoBaseScene<T>* sc, void* adr)
  /*{\Mopl returns the boundary color of the object at |(*adr)|.}*/
  { return sc->get_obj_color(adr); }
  
  //iterator variant ...
  template<class T>
  color get_obj_color(GeoBaseScene<T>* sc, typename T::iterator it)
  /*{\Mopl returns the boundary color of the object $it$ points to.}*/  
  {
    typename T::value_type & obj = *it;
    return sc->get_obj_color((void*)(&obj)); 
  }

  template<class T>    
  color set_obj_color(GeoBaseScene<T>* sc, void* adr, color c)
  /*{\Mopl sets the boundary color of the object at |(*adr)| to |c|.}*/  
  { return sc->set_obj_color(adr,c); } 
  
  //iterator variant ...
  template<class T>    
  color set_obj_color(GeoBaseScene<T>* sc, typename T::iterator it, color c)
  /*{\Mopl sets the boundary color of the object $it$ points to  to |c|.}*/  
  { 
    typename T::value_type & obj = *it;  
    return sc->set_obj_color((void*)(&obj),c); 
  }   

  // -------------- new --------------
  template<class T>
  bool get_obj_color(GeoBaseScene<T>* sc, const typename T::value_type& obj, color& c)
  /*{\Mopl if there is an object $o$ in the container of scene $sc$ with
  $o==obj$ the boundary color of $o$ is assigned to $c$ and $true$ is returned.
  Otherwise false is returned.
  }*/
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          c = sc->get_obj_color((void*)(&of));
	  return true;
       }
    }
    return false;
  }

  template<class T>    
  bool set_obj_color(GeoBaseScene<T>* sc, const typename T::value_type& obj, color c, bool all=true)
  /*{\Mopl if there is an object $o$ in the container of scene $sc$ with
  $o==obj$ the boundary color of $o$ is set to $c$ and true will be
  returned. Otherwise false will be returned.}*/
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    bool flag = false;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          sc->set_obj_color((void*)(&of),c);
	  flag=true;
	  if (flag && !all) return true;
       }
    }
    return flag;  
  }
  // ---------------------------------

  template<class T>  
  color get_obj_fill_color(GeoBaseScene<T>* sc, void* adr)
  /*{\Mopl returns the interior color of the object at |(*adr)|.}*/ 
  { return sc->get_obj_fill_color(adr); }
  
  template<class T>  
  color get_obj_fill_color(GeoBaseScene<T>* sc, typename T::iterator it)  
  {
    typename T::value_type & obj = *it;
    return sc->get_obj_fill_color((void*)(&obj)); 
  }

  template<class T>  
  color set_obj_fill_color(GeoBaseScene<T>* sc, void* adr, color c)
  /*{\Mopl sets the interior color of the object at |(*adr)| to |c|.}*/    
  { return sc->set_obj_fill_color(adr,c); }  
  

  template<class T>  
  color set_obj_fill_color(GeoBaseScene<T>* sc, typename T::iterator it, color c)    
  { 
    typename T::value_type & obj = *it;  
    return sc->set_obj_fill_color((void*)(&obj),c); 
  }

  // -------------- new --------------
  template<class T>
  bool get_obj_fill_color(GeoBaseScene<T>* sc, const typename T::value_type& obj, color& c)
  /*{\Mopl if there is an object $o$ in the container of scene $sc$ with
  $o==obj$ the interior color of $o$ is assigned to $c$ and $true$ is returned.
  Otherwise false is returned.
  }*/
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          c = sc->get_obj_fill_color((void*)(&of));
	  return true;
       }
    }
    return false;
  }

  template<class T>    
  bool set_obj_fill_color(GeoBaseScene<T>* sc, const typename T::value_type& obj, color c, bool all=true)
  /*{\Mopl if there is an object $o$ in the container of scene $sc$ with
  $o==obj$ the interior color of $o$ is set to $c$ and true will be
  returned. Otherwise false will be returned.}*/
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    bool flag = false;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          sc->set_obj_fill_color((void*)(&of),c);
	  flag=true;
	  if (flag && !all) return true;
	  return true;
       }
    }
    return flag;  
  }
  // ---------------------------------
  
  template<class T>
  line_style get_obj_line_style(GeoBaseScene<T>* sc, void* adr)
  /*{\Mopl returns the line style of the object at |(*adr)|.}*/
  { return sc->get_obj_line_style(adr); }  
  
  template<class T>  
  line_style get_obj_line_style(GeoBaseScene<T>* sc, typename T::iterator it)  
  {
    typename T::value_type & obj = *it;
    return sc->get_obj_line_style((void*)(&obj)); 
  }    

  template<class T>    
  line_style set_obj_line_style(GeoBaseScene<T>* sc, void* adr, line_style l)
  /*{\Mopl sets the line style of the object at |(*adr)| to |l|.}*/
  { return sc->set_obj_line_style(adr,l); }  
  
  template<class T>  
  line_style set_obj_line_style(GeoBaseScene<T>* sc, typename T::iterator it, line_style l)    
  { 
    typename T::value_type & obj = *it;  
    return sc->set_obj_line_style((void*)(&obj),l); 
  }  
  
  // -------------- new --------------
  template<class T>
  bool get_obj_line_style(GeoBaseScene<T>* sc, const typename T::value_type& obj, line_style& l)
  /*{\Mopl if there is an object $o$ in the container of scene $sc$ with
  $o==obj$ the line style of $o$ is assigned to $l$ and $true$ is returned.
  Otherwise false is returned.
  }*/
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          l = sc->get_obj_line_style((void*)(&of));
	  return true;
       }
    }
    return false;
  }

  template<class T>    
  bool set_obj_line_style(GeoBaseScene<T>* sc, const typename T::value_type& obj, line_style l, bool all=true)
  /*{\Mopl if there is an object $o$ in the container of scene $sc$ with
  $o==obj$ the line style of $o$ is set to $l$ and true will be
  returned. Otherwise false will be returned.}*/
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    bool flag = false;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          sc->set_obj_line_style((void*)(&of),l);
	  flag=true;
	  if (flag && !all) return true;
       }
    }
    return flag;  
  }
  // ---------------------------------

  template<class T>  
  int get_obj_line_width(GeoBaseScene<T>* sc, void* adr)
  /*{\Mopl returns the line width of the object at |(*adr)|.}*/
  { return sc->get_obj_line_width(adr); } 
  
  template<class T>  
  int get_obj_line_width(GeoBaseScene<T>* sc, typename T::iterator it)  
  {
    typename T::value_type & obj = *it;
    return sc->get_obj_line_width((void*)(&obj)); 
  }        

  template<class T>    
  int set_obj_line_width(GeoBaseScene<T>* sc, void* adr, int w)  
  /*{\Mopl sets the line width of the object at |(*adr)| to |w|.}*/
  { return sc->set_obj_line_width(adr,w); }  
  
  template<class T>  
  int set_obj_line_width(GeoBaseScene<T>* sc, typename T::iterator it, int w)    
  { 
    typename T::value_type & obj = *it;  
    return sc->set_obj_line_width((void*)(&obj),w); 
  }      

  // -------------- new --------------
  template<class T>
  bool get_obj_line_width(GeoBaseScene<T>* sc, const typename T::value_type& obj, int& l)
  /*{\Mopl if there is an object $o$ in the container of scene $sc$ with
  $o==obj$ the line width of $o$ is assigned to $l$ and $true$ is returned.
  Otherwise false is returned.
  }*/
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          l = sc->get_obj_line_width((void*)(&of));
	  return true;
       }
    }
    return false;
  }

  template<class T>    
  bool set_obj_line_width(GeoBaseScene<T>* sc, const typename T::value_type& obj, int l,bool all=true)
  /*{\Mopl if there is an object $o$ in the container of scene $sc$ with
  $o==obj$ the line width of $o$ is set to $l$ and true will be
  returned. Otherwise false will be returned.}*/
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    bool flag=false;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          sc->set_obj_line_width((void*)(&of),l);
	  flag=true;
	  if (flag && !all) return true;
       }
    }
    return flag;  
  }
  // ---------------------------------
  
  // setting labels ...
  
  template<class T>
  string get_obj_label(GeoBaseScene<T>* sc, void* adr)
  /*{\Mopl returns the label of the object at |(*adr)|.}*/
  { string lb;
    sc->get_obj_label(adr, lb); 
    return lb;
  }
  
  //iterator variant ...
  template<class T>
  string get_obj_label(GeoBaseScene<T>* sc, typename T::iterator it)
  /*{\Mopl returns the label of the object $it$ points to.}*/  
  {
    typename T::value_type & obj = *it;
    string lb;
    sc->get_obj_label((void*)(&obj), lb); 
    return lb;
  }

  template<class T>    
  string set_obj_label(GeoBaseScene<T>* sc, void* adr, string lb)
  /*{\Mopl sets the label of the object at |(*adr)| to |lb|.}*/  
  { string prev;
    sc->get_obj_label(adr, prev);
    sc->set_obj_label(adr,lb); 
    return prev;
  } 
  
  //iterator variant ...
  template<class T>    
  string set_obj_label(GeoBaseScene<T>* sc, typename T::iterator it, string lb)
  /*{\Mopl sets the label of the object $it$ points to  to |lb|.}*/  
  { 
    typename T::value_type & obj = *it;  
    string prev;
    sc->get_obj_label((void*)(&obj),prev); 
    sc->set_obj_label((void*)(&obj),lb); 
 
    return prev;
  }   
  
  template<class T>
  bool get_obj_label(GeoBaseScene<T>* sc, const typename T::value_type& obj, string& l)
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          bool b = sc->get_obj_label((void*)(&of), l);
	  return b;
       }
    }
    return false;
  }

  template<class T>    
  bool set_obj_label(GeoBaseScene<T>* sc, const typename T::value_type& obj, string l, bool all=true)
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    bool flag=false;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          sc->set_obj_label((void*)(&of),l);
	  flag=true;
	  if (flag && !all) return true;
       }
    }
    return flag;  
  }  
  
  
  // ----------------------------------------------------------------------
  // setting geowin_text objects
  // ----------------------------------------------------------------------
  
  /*{\Mtext
    \medskip
    {\bf Object texts}
    \\ 
    The following operations can be used to add/retrieve objects of type |geowin_text| to
    objects in scenes. The class |geowin_text| is used to store graphical representations
    of texts. It stores a string (the text) and the following attributes:
\begin{center}
\begin{tabular} {|l|l|l|}\hline
 
\hline
{\bf Name}             & {\bf Type}           & {\bf Description} \\   
\hline
\hline
|font_type|            & |geowin_font_type|  & font type\\
\hline
|size|                 & |double|            & font size\\
\hline
|text_color|          & |color|              & color of the text\\
\hline 
|user_font|            & |string|            & font name (if |font_type = user_font|)\\
\hline
|x_offset|             & |double|            & offset in |x|-direction to drawing position\\
\hline
|y_offset|             & |double|            & offset in |y|-direction to drawing position\\
\hline
\end{tabular}
\end{center}    
    
The enumeration type |geowin_font_type| has the following set of integral constants:
|roman_font|, |bold_font|, |italic_font|, |fixed_font| and |user_font|. \\     
The class |geowin_text| has the following constructors:   
\begin{verbatim}    
    geowin_text(string t, double ox, double oy, geowin_font_type ft, 
                double sz, string uf, color c = black);
    geowin_text(string t, geowin_font_type ft, double sz);
    geowin_text(string t);
\end{verbatim}  
    The arguments are: |t| - the text, |ox,oy| - the x/y offsets, |ft| - the font type, |sz| - the font size,
    |uf| - the user font and |c| - the text color.  If a text is associated with an object, it will be drawn
    centered at the center of the bounding box of the object translated by the |x/y| - offset parameters. Note that
    it is also possible to add texts to a whole scene and to instances of class |GeoWin|. 
    Then the |x/y| - offset parameters specify the  position (see |add_text| operation). 
  }*/  
  
  template<class T>
  bool get_obj_text(GeoBaseScene<T>* sc, void* adr, geowin_text& gt)
/*{\Mopl  Gets the text associated with the object at |adr| in the container of scene |sc| and assigns it to
          |gt|. If no text is associated with the object, |false| will be returned, otherwise |true|.  }*/  
  {
    bool b = sc->get_obj_text(adr, gt); 
    return b;
  }
  
  //iterator variant ...
  template<class T>
  bool get_obj_text(GeoBaseScene<T>* sc, typename T::iterator it, geowin_text& gt)
/*{\Mopl  Gets the text associated with the object |it| points to and assigns it to
          |gt|. If no text is associated with the object, |false| will be returned, otherwise |true|.  }*/    
  {
    typename T::value_type & obj = *it;
    bool b = sc->get_obj_text((void*)(&obj), gt); 
    return b;
  }   
  
  template<class T>    
  void set_obj_text(GeoBaseScene<T>* sc, void* adr, const geowin_text& gt)
/*{\Mopl Assigns |gt| to the object at |adr| in scene |sc|.}*/  
  { sc->set_obj_text(adr,gt); } 
  
  //iterator variant ...
  template<class T>    
  void set_obj_text(GeoBaseScene<T>* sc, typename T::iterator it, const geowin_text& gt)
/*{\Mopl Assigns |gt| to the object |it| points to in scene |sc|.}*/    
  { typename T::value_type & obj = *it;  
    sc->set_obj_text((void*)(&obj),gt); 
  }     
  

  template<class T>
  bool get_obj_text(GeoBaseScene<T>* sc, const typename T::value_type& obj, geowin_text& gt)
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          bool b = sc->get_obj_text((void*)(&of), gt);
	  return b;
       }
    }
    return false;
  }

  template<class T>    
  bool set_obj_text(GeoBaseScene<T>* sc, const typename T::value_type& obj,const geowin_text& gt, bool all=true)
  { 
    T& cnt = sc->get_objref();
    typename T::iterator it;
    bool flag=false;
    
    for(it=cnt.begin();it != cnt.end(); it++){
       if ((*it) == obj){
          typename T::value_type& of = *it;
          sc->set_obj_text((void*)(&of),gt);
	  flag=true;
	  if (flag && !all) return true;
       }
    }
    return flag;  
  }  
  

  // --------------------------------------------------------------------
  
    
  template<class T>
  void reset_obj_attributes(GeoBaseScene<T>* sc)
  /*{\Mopl deletes all individual attributes of objects in scene |(*sc)|.}*/
  { sc->undefine_all(); }
  

#endif

  void set_font(geowin_font_type font_t, double size, string fn_user);
  
  void set_active(geo_scene sc,bool ac) { sc->set_active(ac); }
  
  /*{\Mtext
    \medskip
    {\bf d) Input and Output Operations} }*/
    
  bool identify_file(ifstream& is) const;
    
  void read(geo_scene sc,istream& is) { sc->read(is,false); }
  /*{\Mopl reads the contents of |sc| from input stream |is|.
           Before the contents of |sc| is cleared.
  }*/

  void write(geo_scene sc,ostream& os) { sc->write(os,string("")); }
  /*{\Mopl writes the contents of |sc| to output stream |os|. }*/
  
  void write_active_scene(ostream& os) { if (cur) cur->write(os,string("")); }
  /*{\Mopl writes the contents of the active scene of |\Mvar|
          to output stream |os|. }*/  
  
  void read_scene(string filename);
  /*{\Xopl  reads file $filename$. The file must be written by
            write\_to\_file() and must contain objects of the
	    same type the currently edited scene has, if there
	    is one. Then the contents of file |filename| will
	    become the contents of the currently edited scene.
	    If there is no currently edited scene, a new scene
	    will be created and the contents of file |filename| will
	    become the contents of this scene.
	    
 }*/
 
 void import_scene(string filename);
 
 void write_scene_selected(string filename);
 
 
  void write_scene(string filename);
  /*{\Xopl  writes the contents of the active scene of |\Mvar| to file 
            $filename$.
	    }*/

  void write_scene(geo_scene sc, string filename, bool check_file_exists = true);
  /*{\Xopl  writes the contents of scene |sc| of |\Mvar| to file 
            $filename$.
	    }*/
  /*{\Mtext
    \medskip
    {\bf e) View Operations} }*/

  void   zoom_up();
  /*{\Mopl The visible range is reduced to the half. }*/
  void   zoom_down();
  /*{\Mopl The visible range is doubled. }*/
  void   zoom_area();

  void   adjust_zoom_rect(double& x0, double& y0, double& x1, double& y1) ;
  void   adjust_zoom_rect(double  x0, double  y0, double  x,  double  y, double& x1, double& y1, double& x2, double& y2);

  void fill_window();
  /*{\Mopl changes window coordinate system, so that the objects of the 
           currently active scene fill the window. }*/

  void zoom_undo();

  void reset_window();
  /*{\Mopl resets the visible range to the values that where current when constructing |\Mvar|.}*/
  
  /*{\Mtext
    \medskip
    {\bf f) Parameter Operations}\\
    The following operations allow the set and retrieve the various parameters of |\Mname|.
  }*/  

  string get_bg_pixmap() const;
  /*{\Mopl returns the name of the current background pixmap.}*/
  	    
  string set_bg_pixmap(string pix_name); 	  
  /*{\Mopl changes the window background pixmap to pixmap with name |pix_name|.
           Returns the name of the previous background pixmap.
  }*/
  
  color get_bg_color() const;
  /*{\Mopl returns the current background color.}*/
  
  color set_bg_color(const color& c);
  /*{\Mopl sets the background color to |c| and returns its previous value.}*/
  
  color get_user_layer_color() const;
  /*{\Mopl returns the current color of the user layer.}*/
  
  color set_user_layer_color(const color& c);
  /*{\Mopl sets the user layer color to |c| and returns its previous value.}*/  

  int get_user_layer_line_width() const;
  /*{\Mopl returns the current line width of the user layer.}*/
  
  int set_user_layer_line_width(int lw);
  /*{\Mopl sets the user layer line width to |lw| and returns its previous value.}*/  
  
  bool  get_show_grid() const;
  /*{\Mopl returns true, if the grid will be shown, false otherwise.}*/
  
  bool  set_show_grid(bool sh);
  /*{\Mopl sets the show grid flag to |sh| and returns the previous value.}*/
  
  double  get_grid_dist() const;
  /*{\Mopl returns the grid width parameter.}*/
  
  double  set_grid_dist(double g);
  /*{\Mopl sets the grid width parameter to |g| and returns the previous value.}*/

  grid_style  get_grid_style() const;
  /*{\Mopl returns the grid style parameter.}*/
  
  grid_style  set_grid_style(grid_style g);
  /*{\Mopl sets the grid style parameter to |g| and returns the previous value.}*/  
  
  bool  get_show_position() const;
  /*{\Mopl returns true, if the mouse position will be shown, false otherwise.}*/
  
  bool  set_show_position(bool sp);
  /*{\Mopl sets the show position flag to |sp| and returns the previous value.}*/  
  
  bool  get_show_quick_access_window() const;
  
  bool  set_show_quick_access_window(bool b);
  
  // update handling in result scenes ...
  bool  get_update_while_changing() const;
  
  bool  set_update_while_changing(bool um); 
  
  
  // parameters for d3 output
  /*{\Mtext  
  The following operations set or return various parameters that are used in the three-dimensional
  output of |\Mname|. The three-dimensional
  output can be started by pressing the |Show D3 Output| button in the Window menu.
  }*/
  
  bool get_d3_elimination() const;
  /*{\Mopl returns true, if elimination of hidden lines in the 3d-output mode is enabled, false otherwise.}*/
  
  bool set_d3_elimination(bool b);
  /*{\Mopl sets the |d3_elimination| flag of |\Mvar| to |b| and returns its previous value.}*/
  
  bool get_d3_solid() const;
  /*{\Mopl return true, if faces in the 3d-output mode have to be drawn in different grey scales, false
           otherwise.}*/
  
  bool set_d3_solid(bool b);  
  /*{\Mopl sets the |d3_solid| flag of |\Mvar| to |b| and returns its previous value.}*/

  int get_d3_animation_speed() const;
  int set_d3_animation_speed(int s);
  
  bool get_d3_use_scene_color(geo_scene sc) const;
  bool set_d3_use_scene_color(geo_scene sc, bool b);
  
  bool get_d3_show_edges() const;
  /*{\Mopl returns true, if the redraw of edges is enabled in the 3d-output mode, false otherwise.}*/
  
  bool set_d3_show_edges(bool b);
  /*{\Mopl sets the |d3_show_edges| flag of |\Mvar| to |b| and returns its previous value.}*/
  
  double get_d3_z_value() const { return gw_d3_zvalue; }
  
  double set_d3_z_value(double zn) { double old = gw_d3_zvalue; gw_d3_zvalue = zn; return old; }
  
  void select(const point&);
  void select_all();
  void select();
  void unselect();

  // help for object generation...
  list<string> get_generator_names(geo_scene sc) const { return sc->get_generator_names(); }
  void set_generator_number(geo_scene sc, int n) { sc->set_generator_number(n); }

   
  /*{\Mtext
    \medskip
    {\bf g) Handling of events} 

   \Mname provides operations for changing its default handling of events. 
   As in |GraphWin| (cf. Section \ref{Graph Windows}) the user can define what 
   action should follow a mouse or key event. Constants are defined as in 
   |GraphWin| : 
   \begin{itemize}
   \item  A\_LEFT  (left mouse-button)
   \item  A\_MIDDLE (middle mouse-button)
   \item  A\_RIGHT (right mouse-button)
   \item  A\_SHIFT (shift-key)
   \item  A\_CTRL (control-key)
   \item  A\_ALT (alt-key)
   \item  A\_DOUBLE (double click)
   \item  A\_DRAG (button not released)
   \item  A\_IMMEDIATE (do it immediatly without dragging or double click 
   check)
   \item  A\_OBJECT (editable object at mouse position).
   \end{itemize}
   and can be combined with OR ( \vline\ ).
   }*/
  
  
  void set_action(long mask, geo_action f=0);
  /*{\Mopl  set action on condition $mask$ to $f$. |geo_action| is a
            function of type |void (*)(GeoWin&, const point&)|. 
            For |f == 0| the corresponding action is deleted. }*/
  
  geo_action get_action(long mask);
  /*{\Mopl  get action defined for condition $mask$.  }*/
  
  void reset_actions();
  /*{\Mopl	set all actions to their default values. }*/

  
  /*{\Mtext    
    \medskip
    Default values are defined as follows :
    \begin{itemize}
    \item A\_LEFT   or   A\_LEFT \vline\ A\_OBJECT \newline
    read a new object at mouse position.
    \item A\_LEFT \vline\ A\_DRAG    \newline       scrolling the window.
    \item A\_LEFT \vline\ A\_DRAG \vline\ A\_OBJECT \newline move the object.
    \item A\_LEFT \vline\ A\_CTRL \newline 
    pin current scene at mouse position or delete the pin point if 
    it is currently there. 
    \item A\_MIDDLE \vline\  A\_OBJECT \newline
    toggle the selection state of the object at mouse position.
    \item A\_MIDDLE \vline\ A\_DRAG 
    \newline  toggle the selection state of the objects in the dragging area.
    \item A\_RIGHT \vline\  A\_IMMEDIATE \newline
    set the options of the currently active scene.
    \item A\_RIGHT \vline\  A\_IMMEDIATE \vline\  A\_OBJECT \newline
    opens a menu for the object at mouse position.
    \end{itemize}
    }*/


  void clear_actions();
  /*{\Mopl	clears all actions. }*/
  
  /*{\Mtext
    \medskip
     {\bf Scene events}
     
     The following event handling functions can be set for
     edit scenes:
     \begin{itemize}
     \item Pre add handler
     \item Pre add change handler
     \item Post add handler
     \item Pre delete handler
     \item Post delete handler
     \item Start, Pre, Post and End change handler
  
     \end{itemize}
     
     The add handlers will be called when a user tries to add an object
     to an edit scene in GeoWin, the delete handlers will be called when
     the user tries to delete an object and the change handlers will be
     called when the user tries to change an object (for instance by moving
     it).
     The templated set operations for setting handlers uses member templates. 
     If your compiler does not support member templates, you should use instead
     the templated functions |geowin_set_HANDLER|, where |HANDLER| is one the
     following handlers. 
     All handling functions get as the first parameter a reference to the 
     |\Mname|, where the scene belongs to.
         
  }*/
  
#if !defined(__NO_MEMBER_TEMPLATES__)
  
  // add handler
  
  template<class T,class F>
  bool set_pre_add_handler(GeoEditScene<T>* sc, F handler)
  /*{\Mopl sets the handler that is called before an object is added to |(*sc)|.
  |handler| must have type |bool (*handler)(GeoWin&, const T::value_type &)|.
  |handler| gets a reference to the added object. If |handler| returns false, 
  the object will not be added to the scene.}*/
  { return geowin_set_pre_add_handler(sc,handler); }
  
  template<class T,class F>
  bool set_pre_add_change_handler(GeoEditScene<T>* sc, F handler )
  // handler: bool (*handler)(GeoWin&, const T::value_type &, T::value_type &)
  { return geowin_set_pre_add_change_handler(sc,handler); }
  
  template<class T,class F>
  bool set_post_add_handler(GeoEditScene<T>* sc, F handler)
  /*{\Mopl sets the handler that is called after an object is added to |(*sc)|. 
  |handler| must have type |void (*handler)(GeoWin&, const T::value_type &)|.
  |handler| gets a reference to the added object.
  }*/
  { return geowin_set_post_add_handler(sc,handler); }

  // delete handler
    
  template<class T,class F>
  bool set_pre_del_handler(GeoEditScene<T>* sc, F handler)
  /*{\Mopl sets the handler that is called before an object is 
          deleted from |(*sc)|.
  |handler| must have type |bool (*handler)(GeoWin&, const T::value_type &)|.
  |handler| gets a reference to the added object. If |handler| returns true, the object
  will be deleted, if |handler| returns false, the object will not be deleted.}*/
  { return geowin_set_pre_del_handler(sc,handler); }
  
  template<class T,class F>
  bool set_post_del_handler(GeoEditScene<T>* sc, F handler)
  /*{\Mopl sets the handler that is called after an object is 
          deleted from |(*sc)|.
  |handler| must have type |void (*handler)(GeoWin&, const T::value_type &)|.
  }*/
  { return geowin_set_post_del_handler(sc,handler); }
  
  // change handler
  
  template<class T,class F>
  bool set_start_change_handler(GeoEditScene<T>* sc, F handler)
  /*{\Mopl sets the handler that is called when a geometric object from |(*sc)| 
  starts changing (for instance when you move it or rotate it).
  |handler| must have type |bool (*handler)(GeoWin&, const T::value_type &)|.
  The handler function gets a reference to the object.
  }*/
  { return geowin_set_start_change_handler(sc,handler); }

  template<class T,class F>
  bool set_pre_move_handler(GeoEditScene<T>* sc, F handler)
  /*{\Mopl sets the handler that is called before every move operation.
  |handler| must have type |bool (*handler)(GeoWin&, const T::value_type &, double x, double y)|. 
   The handler gets as the second parameter a reference to the object, as the third parameter
   and fourth parameter the move vector.
   If the handler returns true, the change operation will be executed, if the handler
   returns false, it will not be executed. 
 }*/
  { return geowin_set_pre_move_handler(sc,handler); }  
  
  template<class T,class F>
  bool set_post_move_handler(GeoEditScene<T>* sc, F handler)
  /*{\Mopl sets the handler that is called after every move operation.
  |handler| must have type |void (*handler)(GeoWin&, const T::value_type &, double x, double y)|. 
  The handler gets as the second parameter a reference to the object, as the third parameter
  and fourth parameter the move vector.  
  }*/
  { return geowin_set_post_move_handler(sc,handler); }
 
  template<class T,class F>
  bool set_pre_rotate_handler(GeoEditScene<T>* sc, F handler)
  /*{\Mopl sets the handler that is called before every rotate operation.
  |handler| must have type |bool (*handler)(GeoWin&, const T::value_type &, double x, double y, double a)|. 
   If the handler returns true, the rotate operation will be executed, if the handler
   returns false, it will not be executed. 
 }*/
  { return geowin_set_pre_rotate_handler(sc,handler); }  
  
  template<class T,class F>
  bool set_post_rotate_handler(GeoEditScene<T>* sc, F handler)
  /*{\Mopl sets the handler that is called after every rotate operation.
  |handler| must have type |void (*handler)(GeoWin&, const T::value_type&, double x, double x, double a)|. 
  }*/
  { return geowin_set_post_rotate_handler(sc,handler); } 
  
  template<class T,class F>
  bool set_end_change_handler(GeoEditScene<T>* sc, F handler)
  /*{\Mopl sets the handler that is called when a geometric object from |(*sc)| 
  ends changing.  |handler| gets the object as the second parameter.
  |handler| must have type |void (*handler)(GeoWin&, const T::value_type &)|.}*/
  { return geowin_set_end_change_handler(sc,handler); }

  /*{\Mtext
    \medskip
     {\bf Generator functions:}
     The following operation can be used to set a generator function for an edit scene.
     The operation uses member templates. 
     If your compiler does not support member templates, you should use instead
     the templated function |geowin_set_generate_fcn|. 
 }*/
     
  template<class T>
  bool set_generate_fcn(GeoEditScene<T>* sc, void (*f)(GeoWin& gw,T& L))
  /*{\Mopl sets the generator function for edit scene |(*sc)|. 
           The function gets the GeoWin where |(*sc)| belongs to and
	   a reference to the container |L| of |(*sc)|. The function
	   should write the generated objects to |L|.
  }*/
  { return geowin_set_generate_fcn(sc,f); }
  
  template<class T>
  bool add_generator(GeoEditScene<T>* sc, geowin_generator<T>& Gen)
  { return geowin_add_generator(sc, Gen); }
  
  /*{\Mtext
    \medskip
     {\bf Editing of objects in a scene:}
     It is possible to edit single objects in an editable scene.
     For this purpose an |edit_object| - function can be set for
     editable scenes. This function has type
     
      |void (*f)(GeoWin& gw, T& obj, int nr)|
      
     where |gw| is the $GeoWin$-object where the scene belongs to,
     |obj| is a reference to the object that will be edited
     and |nr| is the edit mode of the scene.    
  }*/
  
  template<class T,class T2>
  bool set_edit_object_fcn(GeoEditScene<T>* sc, T2 f) 
  /*{\Mopl sets the edit object - function of scene |sc| to |f|.}*/ 
  { return geowin_set_edit_object_fcn(sc,f); }  
  
  template<class T>
  void*   get_edit_object_fcn(GeoEditScene<T>* sc)
  /*{\Mopl returns the edit object - function of scene |sc| .}*/
  { return geowin_get_edit_object_fcn(sc); }
  
  // new
  template <class T,class T2>
  bool set_defining_points_fcn(GeoBaseScene<T>* sc, T2 f) 
  { return geowin_set_defining_points_fcn(sc,f); }
  
  template <class T,class T2>
  bool set_move_point_fcn(GeoEditScene<T>* sc, T2 f)
  { return geowin_set_move_point_fcn(sc,f); }
  
  
  template<class T>
  bool add_algorithm(GeoEditScene<T>* sc, void (*f)(GeoWin&, list<typename T::value_type>&), string name)  
  {
    return geowin_add_algorithm(sc,f,name);
  }
  
  // new version of add_algorithm ...
  template<class S,class R>
  GeoResultScene<S,R>* add_algorithm(geowin_update<S,R>& up, GeoEditScene<S>* sc, string scene_name, string algo_name,
                                     D3_FCN f = NULL)
  {
    return geowin_add_algorithm(*this, up, sc, scene_name, algo_name, f);
  }
  
  template<class S, class R>
  bool set_copy_attr_test_fcn(GeoResultScene<S,R>* sc, \
                              bool (*f)(color&,color&,line_style&,int&,typename S::iterator&,typename R::iterator&, int))
  {
    return geowin_set_copy_attr_test_fcn(sc,f);
  }
  
/*{\Mtext
   \medskip
   {\bf Transformation objects:}
   
|GeoWin| supports affine transformations of selected objects in editable scenes
for the LEDA rat- and float-kernel classes.
The used transformation classes are |rat_transform| and |transform| respectively.
The following class templates can be used to instantiate transformation objects.
They are derived from type |geowin_transform|.
\begin{verbatim}
 geowin_gui_rat_transform<KERNEL_CLASS> 
 geowin_gui_transform<KERNEL_CLASS>
\end{verbatim}  
where |KERNEL_CLASS| is a class of the LEDA  rat- or float-kernel.
The default is that no transformation objects are associated with editable scenes.
}*/

  template<class S,class GeoObj>
  void set_transform(GeoEditScene<S>* sc, geowin_transform<GeoObj>& trans)
  /*{\Mopl  makes |trans| the transformation object of edit scene |sc|.}*/
  { sc->transformer = &trans; }
  
  /*{\Mtext
    \medskip
     {\bf Input objects:}
     The following operation can be used to set an input object for an edit scene.
     The operation uses member templates. 
     If your compiler does not support member templates, you should use instead
     the templated functions prefixed with  $geowin$.
     A GeoInputObject\<GeoObj\> has the following virtual functions:
     
        |void operator()(GeoWin& gw, list<GeoObj>& L);|
	
     This virtual function is called for the input of objects. The new objects have to be
     returned in $L$.
    
        |void options(GeoWin& gw);|   
	
     This function is called for setting options for the input object.
     
 }*/
     
  template<class T>
  bool set_input_object(GeoEditScene<T>* sc, const GeoInputObject<typename T::value_type>& obj, string name)
  /*{\Mopl sets the input object |obj| for edit scene |(*sc)|. 
           The function gets the GeoWin where |(*sc)| belongs to and
	   a reference to a list |L|. The function must write the new objects to |L|.
  }*/
  { return geowin_set_input_object(sc,obj,name); }  
  
  template<class T>
  bool add_input_object(GeoEditScene<T>* sc, const GeoInputObject<typename T::value_type>& obj, string name)
  /*{\Mopl  adds the input object |obj| to the list of available input 
           objects of edit scene |(*sc)| without setting |obj| as input object.

  }*/
  { return geowin_add_input_object(sc,obj,name); }
  
  // draw function
  
  template<class T>
  void set_draw_object_fcn(GeoBaseScene<T>* sc, window& (*fcn)(window&, const typename T::value_type &, int w))
  /*{\Mopl  sets a function |fcn| for scene |(*sc)| that will be called for drawing the
            objects of scene |(*sc)|. If no such function is set (the default), the
	    output operator is used.
  }*/
  { geowin_set_draw_object_fcn(sc,fcn); }

#endif

  void set_draw_object_parameters(geo_scene sc,const list<string>& LS)
  { sc->draw_object_parameters = LS; }
  
  void set_activate_handler(geo_scene sc, void (*f)(geo_scene)) { sc->activate_fcn = f; }
  /*{\Mopl sets a handler function |f| that is called with |sc| as parameter when the user 
           activates |sc|. }*/  

  void set_edit_loop_handler(bool (*f)(const GeoWin& gw));
  /*{\Mopl sets a handler function |f| that is called periodically
           in the interactive mode. If this handler returns |true|, we will leave the interactive mode.}*/	   
	   
  void set_quit_handler(bool (*f)(const GeoWin& gw));
  /*{\Mopl sets a handler function |f| that is called when the user 
          clicks the
          quit menu button. |f| should return true for allowing quiting,
	  false otherwise. }*/
  
  void set_done_handler(bool (*f)(const GeoWin& gw)); 
  /*{\Mopl sets a handler function |f| that is called when the user clicks
          the
          done menu button. |f| should return true for allowing quiting,
	  false otherwise. }*/

  void set_contents_fcn(geo_scene sc, bool (*f)(geo_scene, list<string>&));

  int set_edit_mode(geo_scene sc, int emode);
  /*{\Mopl sets the edit mode of scene |sc| to |emode|.}*/
  
  int get_edit_mode(geo_scene sc) const;
  /*{\Mopl return the edit mode of scene |sc|.}*/
  
  /*{\Mtext
    \medskip
    {\bf h) Scene group Operations\\} 
    |GeoWin| can manage scenes in groups. It is possible to add and remove
    scenes to/from groups.
    Various parameters and dependences can be set for whole groups.
    Note that |geo_scenegroup| is a pointer to a scene group.
 }*/
 

  geo_scenegroup new_scenegroup(string name);
  /*{\Mopl Creates a new scene group with name |name| and returns a pointer
           to it.
  }*/

  geo_scenegroup new_scenegroup(string name, const list<geo_scene>& LS);
  /*{\Mopl Creates a new scene group |name| and adds the scenes in |LS|
           to this group.
  }*/
  
  void insert(geo_scenegroup gs, geo_scene sc);
  /*{\Mopl adds |sc| to scene group |gs| .
  }*/
  
  bool del(geo_scenegroup gs, geo_scene sc);
  /*{\Mopl removes |sc| from scene group |gs| and returns true, if the operation
           was succesful (false: |sc| was not in |gs|).
  }*/
  
  /*{\Mtext
    \medskip
    {\bf i) Further Operations} }*/
      

  double get_mfct() { return mouse_fct; }
  void init_data(geo_scene sc) { sc->init_data(); }

  bool get_debug_mode(geo_scene sc) const
  { return sc->debug_mode; }
    
  bool set_debug_mode(geo_scene sc,bool mode)
  { bool rt = sc->debug_mode;
    sc->debug_mode=mode;
    return rt; 
  }  
  
  int set_button_width(int w);
  /*{\Mopl sets the width of the scene visibility buttons in |\Mvar| and
  returns the previous value.}*/
  
  int set_button_height(int h);
  /*{\Mopl sets the height of the scene visibility buttons in |\Mvar| and
  returns the previous value.}*/  
  
  /*{\Mtext    
    \medskip
  You can associate a) buttons with labels or b) bitmap buttons with the visibility
  of a scene in GeoWin. You cannot use a) and b) at the same time.
  The following operations allow you to use add such visibility buttons to GeoWin.
  Note that before setting bitmap buttons with the |set_bitmap| operation you have to
  set the button width and height.}*/
  
  void set_label(geo_scene sc, string label);
  /*{\Mop associates a button with label |label| with the visibility of scene |sc|.}*/
  
  void set_bitmap(geo_scene sc, unsigned char* bitmap);
  /*{\Mop associates a button with bitmap |bitmap| with the visibility of scene |sc|.}*/ 
  
  void set_activate_bitmap(geo_scene sc, unsigned char* bitmap);
  
  void  add_scene_buttons(const list<geo_scene>& Ls, const list<string>& Ln);
  /*{\Mopl add a multiple choice panel for visibility of the scenes in $Ls$ to |\Mvar|.
  The button for the n-th scene in $Ls$ gets the n-th label in $Ln$.}*/

  
  void  add_scene_buttons(const list<geo_scene>& Ls, int w, int h, unsigned char** bm);
  /*{\Mopl add a multiple choice panel for visibility of the scenes in $Ls$ to |\Mvar|.
  The button for the n-th scene in $Ls$ gets the n-th bitmap in $bm$. The bitmaps have
  width $w$ and height $h$.}*/
  
  list<geo_scene> get_scenes() const { return scenes; }
  /*{\Mopl  returns the scenes of |\Mvar|.}*/
  
  list<geo_scenegroup> get_scenegroups() const { return scene_groups; }
  /*{\Mopl  returns the scene groups of |\Mvar|.}*/  
  
  list<geo_scene> get_scenes(geo_scenegroup gs) const { return gs->group_members; }
  /*{\Mopl  returns the scenes of group |gs|.}*/
  
  list<geo_scene> get_visible_scenes() const;
  /*{\Mopl  returns the visible scenes of |\Mvar|.}*/
 
  void add_dependence(geo_scene sc1, geo_scene sc2) { sc1->add_dependence(sc2);}
  /*{\Mopl makes |sc2| dependent from |sc1|. That means that |sc2|
           will be updated when the contents of |sc1| changes. }*/

  void del_dependence(geo_scene sc1, geo_scene sc2) { sc1->del_dependence(sc2);}
  /*{\Mopl deletes the dependence of scene |sc2| from |sc1|. }*/

  void set_frame_label(const char* label) { Wp->set_frame_label(string(label)); }
  /*{\Mopl   makes |label| the frame label of |\Mvar|. }*/


  int open_panel(panel& P);
  /*{\Mopl displays panel |P| centered on the drawing area of |\Mvar|,
           disabels the menu bar of |\Mvar| and returns the result of
	   |P.open()|.}*/
           
  
  int open_panel(file_panel& P);
  
  // node  and edge color maps ...
  h_array<node,color>& get_node_color_map() { return node_color_map; }
  h_array<edge,color>& get_edge_color_map() { return edge_color_map; }
  
  // arrow sets (member edges will be painted using arrow in d3 output)
  set<edge>&  get_arrow_set() { return arrow_set; }
  
  
  // adding/removing texts to/from GeoWin objects
  void add_text(const geowin_text& gt);
  /*{\Mopl   adds a text |gt| to |\Mvar|.}*/  
  
  void remove_texts();
  /*{\Mopl   removes all texts from |\Mvar| (but not from the scenes of |\Mvar|).}*/  
  
  // adding/removing/changing texts to/from scenes ...
  
  void add_text(geo_scene sc);
  
  void add_text(geo_scene sc, const geowin_text& gt);
  /*{\Mopl   adds a text |gt| to scene |sc|.}*/
  
  void remove_text(geo_scene sc, string ch);
  
  void remove_texts(geo_scene sc);
  /*{\Mopl   removes all texts from scene |sc|.}*/
  
  void change_text(geo_scene sc, string ch);
  
  void enable_menus();
  /*{\Mopl enables the menus of |\Mvar|.}*/
  
  void disable_menus();
  /*{\Mopl disables the menus of |\Mvar|, but not the |User| menu.}*/
 
  double version();
  /*{\Mopl returns the |GeoWin| version number.}*/
  
  double fileformatversion();

  void message(string msg);
  /*{\Mopl displays message |msg| on top of the drawing area.
          If |msg| is the empty string, a previously written
	  message is deleted.
  }*/

  void msg_open(string msg);
  /*{\Mopl displays message |msg| in the message window of |\Mvar| .
      If the message window is not open, it will be opened.
  }*/
  
  void   msg_close() { if(msg_win->is_open()) msg_win->close(); }
  /*{\Mopl closes the message window.}*/
  void   msg_clear() { if(msg_win->is_open()) msg_win->del_messages(); }
   /*{\Mopl clears the message window.}*/

  geowin_redraw* set_redraw_pt(geo_scene sc,geowin_redraw* rd)
  { geowin_redraw* old=sc->redraw_pt;
    sc->redraw_pt=rd; return old;
  }
  
  geowin_redraw* get_redraw_pt(geo_scene sc);

  void   show_d3();
  void   hide_d3();

  void   set_d3_fcn(geo_scene sc, void (*f)(geo_scene gs, d3_window& W, GRAPH<d3_point,int>& H)) 
  /*{\Mopl sets a function for computing 3d output. The parameters of the function are
  the |geo_scene| for that it will be set and a function pointer. The function |f| will get the scene
  for that it was set and the reference to a |d3_window| that will be the output window.
  }*/
  { sc->init_d3_window = f; }
  
  D3_FCN  get_d3_fcn(geo_scene sc)
  /*{\Mopl returns the function for computing 3d output that is set for scene |sc|.
  The returned function has pointer type |void (*)(geo_scene, d3_window&, GRAPH<d3_point,int>&)|.}*/
  { return sc->init_d3_window; }
  
  /*{\Mtext    
    \medskip
    \Mname can be pined at a point in the plane. As standard behavior it is
    defined that moves of geometric objects will be rotations around the
    pin point. }*/
    
  void draw_pin();

  bool get_pin_point( point& p ) const;
  /*{\Mopl returns the pin point in $p$ if it is set.}*/
  void set_pin_point( point  p );
  /*{\Mopl sets the pin point to $p$.}*/
  void del_pin_point( ); 
 /*{\Mopl deletes the pin point.}*/
 
  int add_call(GeoFunctions* f, string label, window* m = 0);
  /*{\Xopl adds a menu item $label$ to menu |m| of $gw$. 
           If |m| points to |0| the "User" Menu is choosen.}*/

  int add_button_call(GeoFunctions* f, string label, window* w = 0, char* m = 0);
  /*{\Xopl adds a menu button to  $gw$. 
           |m| is the pixmap of the button.}*/
	   
  void add_help_action(string name, geo_help_action act);

  void add_help_text(string name) { help_list.append(name); }
  /*{\Mop  adds the help text contained in |name.hlp| with label |name|
           to the help menu of the main window. The file |name.hlp| must
           exist either in the current working directory or in
           |\$LEDAROOT/incl/Help|. Note that this operation must be called
           before |gw.display()|. }*/
	   
  void add_special_help_text(string name, bool auto_display = false);	
  /*{\Mop  adds one help text contained in |name.hlp| 
           to the menu of the main window. The file |name.hlp| must
           exist either in the current working directory or in
           |\$LEDAROOT/incl/Help|. Note that this operation must be called
           before |gw.display()|. If |auto_display| is true, this help text will be displayed,
	   when the main window is displayed.}*/  
     

#if !defined(__NO_MEMBER_TEMPLATES__)

  // we can now set a limit for user input in edit scenes ...
  
  template<class T>
  int get_limit(GeoEditScene<T> *es) 
  /*{\Mop  returns the limit of edit scene |es| (a negative number will be returned, if there is no limit).}*/  
  { return es->limit; }
  
  template<class T>
  int set_limit(GeoEditScene<T> *es, int limit) 
  /*{\Mop  sets the limit of edit scene |es| to |limit| and returns the previous value.}*/  
  {
    int old_limit = es->limit;
    es->limit = limit;
    return old_limit;
  }

  /*{\Mtext
    \medskip
    The templated |add_user_call| operation uses member templates. If your 
    compiler does not support member templates, you should use instead
    the templated function |geowin_add_user_call| with
    |\Mvar| as an additional first parameter.
    
 }*/
  
  template<class F> void add_user_call(string label, F f)
   /*{\Mopl  adds a menu item $label$ to the "User" menu of $GW$. 
             The user defined function |void geo_call(GeoWin&, F, string)| 
	     is called whenever this menu button was pressed with parameters 
	     |\Mvar|, |f| and |label|. This menu 
	     definition has to be finished before $GW$ is opened. }*/
  {
    geowin_add_user_call(*this, label, f);
  }
#endif 

  // function for calling new update functions ...
  // void call_update_fcn(void (*fcn)(...),void* input, void* output,const list<void*>& infl);
  void call_update_fcn(void (*fcn)(...),void* input, void* output,const list<void*>& infl);
  
  /*{\Mtext    
  Import- and export objects can be used to import and export the contents of scenes in various formats.\\
  The classes |geowin_import| and |geowin_export| are used for implementing import- and export objects.
  The classes |geowin_import| and |geowin_export| have virtual |()| - operators:
  
     |virtual void operator()(geo_scene sc, string filename)|
     
  This virtual operator can be overwritten in derived classes to provide import and export functionality
  for own formats. The first parameter is the scene |sc| that will be used as source for the output or target
  for the input. The second parameter $filename$ is the name of the input (import objects) or output (export 
  objects) file.
  }*/  
  
  void add_import_object(geo_scene sc, geowin_import& io, string name, string desc);
  /*{\Mopl Adds an import object |io| to scene |sc|. The import object gets the name |name|
  and the description |desc|.}*/
  
  void add_export_object(geo_scene sc, geowin_export& eo, string name, string desc);
  /*{\Mopl Adds an export object |eo| to scene |sc|. The export object gets the name |name|
  and the description |desc|.}*/  
  
};


// moved after GeoWin for CC 

class __exportGeoW GeowinMember : public GeoFunctions
{
  void (GeoWin::*f)(void);
  
  public : 
  
  GeowinMember(void (GeoWin::*g)(), string l)  : GeoFunctions(l), f(g)   {}
  GeowinMember(void (GeoWin::*g)()) : GeoFunctions(""), f(g)  {}
  virtual ~GeowinMember() {}
  virtual inline void call(GeoWin& gw){ (gw.*f)(); }
  
  LEDA_MEMORY(GeowinMember)
};


// removed from templates ...

extern __exportGeoW bool geowin_show_options2(GeoScene* scn,panel* p,GeoWin *gw,color& filling_color,
                                     bool& high_light,bool fredraw, list<string> fnames, 
				     int& number, geowin_defining_points& hdp);
extern __exportGeoW void mouse_pos(long& MASK,GeoWin* gw, bool b);



template<class iterator>  struct __exportGeoW geo_object_properties
{
  // class defines properties of edited objects
  iterator    pos;         // position in edited container
  list_item   selected;    // nil if the object is not selected, 
                           // position in list of selected objects otherwise
  
  geo_object_properties(const geo_object_properties& gp)
    : pos(gp.pos), selected(gp.selected){}
  
  geo_object_properties(iterator it) 
    : pos(it), selected(nil) {}
  
  geo_object_properties() : pos(), selected(nil){}
  
  geo_object_properties(iterator it, list_item lit)
    : pos(it), selected(lit) {}

  LEDA_MEMORY(geo_object_properties<iterator>)
};


// **************************************************************************
// ****************     class GeoBaseScene      *********************
// every builder builds a prototype


template<class T> class GeoBaseScene;
template<class T> class GeoBaseSceneBuilder : public GeoSceneBuilder
{
  friend class GeoBaseScene<T>;

  public :
  
  GeoBaseSceneBuilder() 
    : GeoSceneBuilder(new GeoBaseScene<T>)  { 
    /*
        GeoBaseScene<T>* sc = 
      (GeoBaseScene<T>*)(GeoScenePrototypes::get_prototype(prototypeid));
    sc->id = prototypeid; */
  }
  
  // added ... 
  LEDA_MEMORY(GeoBaseSceneBuilder<T>)
};


template<class T> class GeoBaseScene : public GeoScene, public GeoEditor
{  
  protected :

  typedef typename T::iterator                iterator;
  typedef typename T::value_type              GeoObj;
  typedef geo_object_properties<iterator>       geo_props;
  
  typedef void   (*fu_help_void)(...);
  typedef bool   (*fu_help_bool)(...);
  typedef string (*fu_help_string)(...);
  
  friend class GeoBaseSceneBuilder<T>;
  friend class __exportGeoW GeoWin;
  
public:
  
  // handler functions
  bool (*pre_add_fcn)(GeoWin&, const GeoObj&);
  bool (*pre_add_changer)(GeoWin&, const GeoObj&, GeoObj&);
  void (*post_add_fcn)(GeoWin&, const GeoObj&);
  
  bool (*pre_del_fcn)(GeoWin&, const GeoObj&);
  void (*post_del_fcn)(GeoWin&, const GeoObj&);
  
  bool (*pre_move_fcn)(GeoWin&, const GeoObj&, double x, double y);
  void (*post_move_fcn)(GeoWin&, const GeoObj&, double x, double y);
  
  // we handle transformations with a transform object
  geowin_transform<GeoObj>*   transformer; 

  bool (*pre_rotate_fcn)(GeoWin&, const GeoObj&, double x, double y, double a);
  void (*post_rotate_fcn)(GeoWin&, const GeoObj&, double x, double y, double a);  


#ifdef NO_STATIC_DATA_MEMBER
  
  static GeoBaseSceneBuilder<T>& b1()
  {
    static GeoBaseSceneBuilder<T> sb1;
    return sb1;
  }

#else
  
  static GeoBaseSceneBuilder<T> sb1;
  static GeoBaseSceneBuilder<T>& b1()
  {
    return sb1;
  }
#endif

  T*                     objs;    //dataobject to display
  geowin_redraw_container<T>* gw_redraw_cnt;

  iterator mouse_obj; 
  map<void*, geo_props>  props;
 
  // for adding/deleting objects ...
  list<GeoObj>  changed;     // new inserted/deleted/changed objects
  list<GeoObj>  changed2;    // change: old objects (for move/rotate)
  
  GeoBaseScene() : GeoScene(0)
  {
    //b1(); 
    // set myobjs true in GeoScene constructor ...
    objs          = new T;
    gw_redraw_cnt = NULL;   
    pre_add_fcn = 0;
    pre_add_changer = 0;
    post_add_fcn = 0;
    pre_del_fcn = 0;
    post_del_fcn = 0;
    pre_move_fcn = 0;
    post_move_fcn = 0;
    transformer = 0;  
    pre_rotate_fcn = 0;
    post_rotate_fcn = 0;
  } 
  
  ~GeoBaseScene()
  {
    if( myobjs ) delete objs;
    undefine_all();
  }

  virtual GeoScene* clone()
  {
    GeoBaseScene<T>* sc = new GeoBaseScene<T>;
    sc->init_from_prototype(this);
    return sc;
  }   
  
  virtual bool IsSuperOrEqual( GeoScene* sc ) 
  {
    if( b1().prototypeid == sc->GeoSceneId() ) return true; 
    return false;
  }
  
  public : 
  
  // -------------------------------------------------------------
  // iteration on the scene container ...
  iterator act_iter, stop_iter;
  
  void init_iteration() { 
    act_iter = objs->begin();
    stop_iter = objs->end();  
  }
  
  void* get_next_obj_adr() { return (void*)(&(*act_iter)); }  
  void  iterate() { act_iter++; }
  bool  no_end_reached() { return (act_iter != stop_iter); }
  // -------------------------------------------------------------  
  
  //---------------------------------------------------------------------------
  
  void move(GeoObj& t,  double dx, double dy)
  { if (move_fcn) move_fcn((void*)(&t), dx, dy);  }

  void rotate(GeoObj& t,  double x, double y, double alpha)
  { if (rotate_fcn) rotate_fcn((void*)(&t), x, y, alpha); }
  
  void set_box_intersection_fcn(bool (*f)(const GeoObj& , double, double, 
					  double, double,bool))
  { box_intersection_fcn =  (fu_help_bool) f; }
  
  void set_get_bounding_box_fcn(void (*f)(const GeoObj&, double&, double&, 
					  double&, double&))
  { get_bounding_box_fcn = (fu_help_void) f; }

  void set_move_fcn(void (*f)(GeoObj&, double, double))
  { move_fcn = (fu_help_void) f; }
  
  void set_rotate_fcn(void (*f)(GeoObj&, double, double, double))
  { rotate_fcn = (fu_help_void) f;}  
   
  GeoObj read_object(istream& is)          { GeoObj t; is >> t; return t; }
  void write_object(ostream& os, GeoObj t) { os << t; }  
  //----------------------------------------------------------------------------
  
  void set_objects_init_d3_window(void  (*f)(const T&,d3_window&, GRAPH<d3_point,int>&))
  { objects_init_d3_window = (fu_help_void) f; }
  
  void set_defining_points_fcn(void (*f)(const GeoObj&, list<point>&))
  { defining_points_fcn = (fu_help_void)f; }  
  
  // operations for lists of changed/added/deleted objects ...

  virtual void clear_op_objs() { changed.clear(); changed2.clear(); change_mode=-1; }
  
  // returns a reference to the container :
  T&   get_objref()   { return *objs; }
  
  virtual void* get_untyped_objptr()  { return (void*)objs; }
  virtual void* get_untyped_mouseobjptr() {
    if (mouse_obj_exist) return (void*)(&(*mouse_obj));
    else return NULL; 
  }
  
  // sets the container reference:
  void set_objref(T& t)   
  { 
    if( myobjs )  { delete objs; }
    objs = &t;
    myobjs = false;
    init_data();
  }
  
  //deletes object reference, if not myobjs ...
  virtual void del_objref()
  {
    if (! myobjs) objs=NULL;
  }
  
  // returns a copy of the container :
  T    get_objects()     { return *objs; }
  
  // fills the container with a copy of cp
  void set_objects(T cp) { *objs = cp; init_data();  }
  
  virtual int GeoSceneId() const { 
    return b1().prototypeid; 
  }

  virtual void init_from_prototype(GeoScene* proto)
  {
    GeoScene::init_from_prototype(proto);
    GeoBaseScene<T>* pr = (GeoBaseScene<T>*)proto;
    draw_fcn = pr->draw_fcn;
    import_objects = pr->import_objects;
    export_objects = pr->export_objects;
  }
  
  void set_info_fcn( string (*f)(const T&)  ) { 
    info_fcn = (fu_help_string)f;
  }
  
  bool get_select(void* obj_adr)
  { return (props.defined(obj_adr) && props[obj_adr].selected); }

  bool get_select(const GeoObj& obj) 
  { 
    return (props.defined((void*)(&obj)) && props[(void*)(&obj)].selected);
  }
  
  virtual void window_output(window& w, void* obj_adr)
  { GeoObj* g = (GeoObj*)(obj_adr); w << *g; } 
  

  void draw_object_with_properties(const GeoObj& obj)
  { GeoScene::draw_object_with_properties((void*)(&obj)); }
  
  virtual void call_redraw_objects(bool& b,window* w, double x1, double y1, double x2, double y2)
  {
    // redraw version taking container ...
    if (gw_redraw_cnt) { b = gw_redraw_cnt->operator() (*objs,*w,col1,col2,x1,y1,x2,y2); }
    else {
    
     // redraw without container parameter ...
     if (redraw_pt) { b = redraw_pt->operator() (*w,col1,col2,x1,y1,x2,y2); }
    }
  }  
  
  // Read - Write
  void write(ostream& os, string tr){
    iterator it   = objs->begin(), stop  = objs->end();
    if( it != stop )  { os << *it++; while( it != stop ) os << ' ' << tr << *it++;   }
    os.flush();
  }

  void write_postscript(ps_file& F)  {  
   if (gw_redraw_cnt) { if (gw_redraw_cnt->write_postscript(*objs, F,col1,col2)) return; }
    else {
     if (redraw_pt){ if (redraw_pt->write_postscript(F,col1,col2)) return; }
    }

    iterator it = objs->begin(), stop = objs->end();
  
     while(it != stop)
     {
	GeoObj& obj = *it;
	void* adr = (void*)(&obj);
        set_ps_params(F,adr);
        F << obj;	
	ps_draw_text(F, adr);
	it++;
     }   
     GeoScene::write_postscript(F);
  }

  virtual void read(istream& in, bool keep){
   if (!keep) objs->clear();
   for(;;)
    { char c;
      while (in.get(c) && isspace(c));    if (!in) break;   in.putback(c);
      GeoObj p;  in >> p;  objs->push_back(p);
    }
    init_data();
  }

};

#ifndef NO_STATIC_DATA_MEMBER
template<class T> GeoBaseSceneBuilder<T> GeoBaseScene<T>::sb1;
#endif

template<class T> GeoBaseScene<T>* make_base_prototype(T* t, string str)
{
  int id =  GeoBaseScene<T>::b1().get_id();
  GeoScenePrototypes::set_type_name(id, str);
  return (GeoBaseScene<T>*)(GeoScenePrototypes::get_prototype(id));
}

template<class T> GeoBaseScene<T>* get_base_prototype(T* t)
{
  int id =  GeoBaseScene<T>::b1().get_id();
  return (GeoBaseScene<T>*)(GeoScenePrototypes::get_prototype(id));
}

// ***************************************************************************
// *****************     class GeoEditScene     ******************************


template<class T> class GeoEditScene;
template<class T> class __exportGeoW GeoEditSceneBuilder : public GeoSceneBuilder
{
  friend class GeoEditScene<T>;
  
  public :
  
  GeoEditSceneBuilder() : GeoSceneBuilder(new GeoEditScene<T>) 
  {
    GeoEditScene<T>* sc = 
      (GeoEditScene<T>*)(GeoScenePrototypes::get_prototype(prototypeid));
    sc->id = prototypeid;
  }
  
  //added ... 
  LEDA_MEMORY(GeoEditSceneBuilder<T>)
};


// input functions for Edit Scenes ...
template<class GeoObj>
class __exportGeoW GeoInputObjectWrapper {
  typedef void (*FCN)(GeoWin&, list<GeoObj>&);
  string name;
  GeoInputObject<GeoObj>* alternative_input_object;
  void (*f)(GeoWin&, list<GeoObj>&);
  geo_scene res;

  public:
   GeoInputObjectWrapper() { }
   
   GeoInputObjectWrapper(const GeoInputObject<GeoObj>& obj, string n)
   { alternative_input_object = (GeoInputObject<GeoObj>*) (&obj); f=NULL; res=NULL; name=n; }
   
   GeoInputObjectWrapper(void (*fcn)(GeoWin&, list<GeoObj>&), string n)
   { alternative_input_object = NULL; f=fcn; res=NULL; name=n; }
   
   GeoInputObjectWrapper(geo_scene sc_res, string n)
   { alternative_input_object = NULL; f=NULL; res=sc_res; name=n; }   
   
   GeoInputObject<GeoObj>* get_obj_ptr() { return alternative_input_object; }
   FCN get_fcn() { return f; }
   string  get_name(){ return name; }
   geo_scene get_scene() { return res; }
};

template<class GeoObj>
inline int compare(const GeoInputObjectWrapper<GeoObj>& f1, const GeoInputObjectWrapper<GeoObj>& f2)
{ return 0; }

template<class GeoObj>
inline istream& operator>>(istream& i, GeoInputObjectWrapper<GeoObj>&)
{ return i; }

template<class GeoObj>
inline ostream& operator<<(ostream& o, const GeoInputObjectWrapper<GeoObj>&)
{ return o; }


// window input ...
template<class GeoObj>
inline void geowin_window_object_input(window& w, GeoObj& t)
{ w >> t; }

// we cannot specialize a function template, so we must
// overload ...
inline void geowin_window_object_input(window& w, gen_polygon& t)
{
  w.set_tmp_label(">> gen_polygon: holes in clockwise order, Quit-Middle mouse button");
  w.read(t); 
  w.reset_frame_label();
}

inline void geowin_window_object_input(window& w, rat_gen_polygon& t)
{
  gen_polygon g;
  w.set_tmp_label(">> rat_gen_polygon: holes in clockwise order, Quit-Middle mouse button");
  w.read(g);
  t = rat_gen_polygon(g);
  w.reset_frame_label();
}

// corrections for g++-3.4.x
#if ((__GNUC__==3 && __GNUC_MINOR__>=4) || (__GNUC__ >3))
#define props base_class::props
#define edit_menu_type base_class::edit_menu_type
#define mouse_obj base_class::mouse_obj
#define limit base_class::limit
#define pre_add_fcn base_class::pre_add_fcn
#define pre_add_changer base_class::pre_add_changer
#define objs base_class::objs
#define set_default_properties base_class::set_default_properties
#define gw base_class::gw
#define cyclic_colors base_class::cyclic_colors
#define cyclic_colors_counter base_class::cyclic_colors_counter
#define set_obj_color base_class::set_obj_color
#define set_obj_fill_color base_class::set_obj_fill_color
#define cyclic_fill_colors_counter base_class::cyclic_fill_colors_counter
#define post_add_fcn base_class::post_add_fcn
#define pre_del_fcn base_class::pre_del_fcn
#define post_del_fcn base_class::post_del_fcn
#define mouse_obj_exist base_class::mouse_obj_exist
#define sel_objs base_class::sel_objs
#define fcn_state base_class::fcn_state
#define pre_move_fcn base_class::pre_move_fcn
#define post_move_fcn base_class::post_move_fcn
#define transformer base_class::transformer
#define pre_rotate_fcn base_class::pre_rotate_fcn
#define post_rotate_fcn base_class::post_rotate_fcn
#define undefine_all base_class::undefine_all
#define find_object base_class::find_object
#define results base_class::results
#define change_mode base_class::change_mode
#define changed base_class::changed
#define but base_class::but
#define nval base_class::nval
#define filling_color base_class::filling_color
#define update_and_redraw base_class::update_and_redraw
#define high_light base_class::high_light
#define handle_defining_points base_class::handle_defining_points
#define changed2 base_class::changed2
#define mouse_changing base_class::mouse_changing
#define obj_defining_points_change base_class::obj_defining_points_change
#define generator_names base_class::generator_names
#define set_generator_number base_class::set_generator_number
#define get_generator_number base_class::get_generator_number
#define set_original_properties base_class::set_original_properties
#define move_point base_class::move_point
#endif

template<class T> 
class  GeoEditScene : public GeoBaseScene<T>
{  
  friend class __exportGeoW GeoEditSceneBuilder<T>;
  friend class __exportGeoW GeoWin;

  public :

#ifdef NO_STATIC_DATA_MEMBER
  
  static GeoEditSceneBuilder<T>& b2()
  {
    static GeoEditSceneBuilder<T> sb2;
    return sb2;
  }
  
#else
  static GeoEditSceneBuilder<T> sb2;
  static GeoEditSceneBuilder<T>& b2()
  {
    return sb2;
  }
#endif
 
  typedef typename T::value_type                GeoObj;
  typedef typename T::iterator                  iterator; 
  typedef geo_object_properties<iterator>       geo_props;
  typedef GeoInputObjectWrapper<GeoObj>         INP_FCN;
  typedef GeoBaseScene<T> 			base_class;
  typedef list<GeoObj>                          GeoObjList;
  typedef typename GeoObjList::iterator       iterator2;   
  

  void (*generate_fcn)(GeoWin&, T&);
  void (*post_window_default_input_handler)(GeoWin&, GeoObj&);
  
  // list of additional generators (pointers to them ...)
  list<geowin_generator<T>* >  generators;

  // here are the various functions for the buffer
  list<INP_FCN>           buffer_operations;
  
  INP_FCN*                alternative_input_fcn; 
  list<INP_FCN>           input_fcns;

  protected:

  list<GeoObj>            clip_objs;
  void*                   itw_obj1;
                    

  GeoEditScene() : GeoBaseScene<T>()
  {
    //b2();
    edit_menu_type = edit_menu2;
    alternative_input_fcn = NULL;
  }  
  
  virtual geo_scene clone()
  {
    GeoEditScene<T>* sc = new GeoEditScene<T>;
    sc->init_from_prototype(this);    
    return sc;
  }  
  
  virtual bool IsSuperOrEqual( geo_scene sc ) 
  {
    if( b2().prototypeid == sc->GeoSceneId() ) return true; 
    return GeoBaseScene<T>::IsSuperOrEqual(sc);
  }
  
  // gets address of the new mouse object
  virtual void set_mouse_object(void* obj_adr)
  {
    if (props.defined(obj_adr)) { 
      mouse_obj = props[obj_adr].pos;
    }
  }
    
  bool add_object(const GeoObj& obj,iterator& back)
  {
    if ((limit < 0) || ((int) objs->size()<limit)) {
  
    if( !pre_add_fcn || pre_add_fcn(*gw, obj) )
      {
        iterator it;
	// we use this flag because of some
	// problems with SUN_PRO
	bool bvl=true;

        if (pre_add_changer) {
	
           GeoObj obj2;
           if (pre_add_changer(*gw, obj, obj2))  { it = objs->insert(objs->end(), obj2); }
           else { bvl=false; } 
      
        }
        else it = objs->insert(objs->end(), obj); 

        if (bvl){
         back=it;
	 geo_props gp(it);
         GeoObj& nwob = *it;
	 props[(void*)(&nwob)] = gp;
         set_default_properties((void*)(&nwob));
	 if (cyclic_colors){
	   set_obj_color((void*)(&nwob),color(1+cyclic_colors_counter%16));
	   set_obj_fill_color((void*)(&nwob),color(1+cyclic_colors_counter%16));
	   cyclic_colors_counter++;
	   cyclic_fill_colors_counter++;
	 }
	 
	 if(post_add_fcn) post_add_fcn(*gw, nwob);
	 return true;
	}
      }
      }
    return false;
  }

  bool del_object(iterator it)
  { 
    GeoObj& obj = *it;
  
    if( !pre_del_fcn || pre_del_fcn(*gw, obj) )
      {
	set_select((void*)(&(*it)), false);
	if( mouse_obj_exist && !(mouse_obj != it)) mouse_obj_exist = false; 
	//objs->erase(it);

        set_default_properties((void*)(&obj));
	if( post_del_fcn ) post_del_fcn(*gw, obj);
	// changed (erase operation was before set_default_properties)
        objs->erase(it);
	
	return true;
      }
    return false;
  }

public:

  void set_select(void* adr, bool b)
  {
    GeoObj& obj = *((GeoObj*)adr);
    if( b != get_select(obj) )
    { if (b) 
        props[adr].selected = sel_objs.append(adr);
      else
	{ sel_objs.del_item(props[adr].selected);
	  props[adr].selected = nil;
	 }
      }
  }  
  
  void add_algorithm(void (*f)(GeoWin&, list<GeoObj>&), string fcn_name)
  {
    buffer_operations.append(GeoInputObjectWrapper<GeoObj>(f,fcn_name));    
  }

  void add_algorithm(geo_scene sc_res, string fcn_name)
  {
    buffer_operations.append(GeoInputObjectWrapper<GeoObj>(sc_res,fcn_name));    
  }  
  
  
  void set_alternative_input_object(const GeoInputObject<GeoObj>& obj, string n)
  { 
    input_fcns.append(INP_FCN(obj,n));
    alternative_input_fcn = &(input_fcns[input_fcns.last()]); 
  }
  
  void add_alternative_input_object(const GeoInputObject<GeoObj>& obj,string n)
  { input_fcns.append(INP_FCN(obj,n)); }
  
  virtual void set_alternative_input_fcn(int i)
  {
     if (i==0)  { alternative_input_fcn=NULL; fcn_state = i; gw->INP_STATE=i; }
     else {
       if (i<=input_fcns.length()) {
        list_item it = input_fcns.get_item(i-1);
        alternative_input_fcn = &(input_fcns[it]);  
	fcn_state = i; gw->INP_STATE=i;
       }
     }  
     gw->redraw_inp_panel();
  }
  
  list<string> get_fcn_names(int& nb, bool flag)
  // flag true: return input fcn names, false: return algo names ...
  {
    nb=0;
    list<string> fcns;
    int i=1;
    INP_FCN iter;
    
    if (! flag)
      forall(iter,buffer_operations) fcns.append(iter.get_name()); 
    else
      forall(iter,input_fcns) { 
       if ((alternative_input_fcn!=NULL) && (iter.get_obj_ptr() == alternative_input_fcn->get_obj_ptr())) nb=i;
       fcns.append(iter.get_name()); i++; 
      }
      
    return fcns;
  }

  // virtual GeoScene members
  
  virtual int GeoSceneId() const { 
    return b2().prototypeid; 
  }
    
  virtual GeoEditor* type_editor()  { return this; }
  
  virtual void init_from_prototype(geo_scene proto)
  {
    GeoBaseScene<T>::init_from_prototype(proto);
    
    edit_menu_type = edit_menu2;
    
    GeoEditScene<T>* pr = (GeoEditScene<T>*)proto;    
    
    generate_fcn = pr->generate_fcn;
    post_window_default_input_handler = pr->post_window_default_input_handler;
    input_fcns = pr->input_fcns;
    buffer_operations= pr->buffer_operations;
    
    pre_add_fcn = pr->pre_add_fcn;
    pre_add_changer = pr->pre_add_changer;
    post_add_fcn = pr->post_add_fcn;
    pre_del_fcn = pr->pre_del_fcn;
    post_del_fcn = pr->post_del_fcn;
    pre_move_fcn = pr->pre_move_fcn;
    post_move_fcn = pr->post_move_fcn;
    transformer = pr->transformer; 
    pre_rotate_fcn = pr->pre_rotate_fcn;
    post_rotate_fcn = pr->post_rotate_fcn;    
    
    // new - additional generators
    generators = pr->generators;
  }

  virtual void init_data()
  { 
    undefine_all();
    mouse_obj_exist = false;

    props.clear();
    sel_objs.clear();
    clip_objs.clear();
    
    iterator it = objs->begin(), stop = objs->end();
    while( it != stop ) 
      {
	geo_props gp(it);
        GeoObj& act= *it;
	props[(void*)(&act)] = gp;
        it++;
      }

    GeoScene::init_data(); 
  }
 
  virtual void edit() { if(gw) gw->edit(this); }
  
  virtual void mouse_at_pos(double x, double y, long& MASK)
  {
    mouse_pos(MASK,gw, find_object(x,y));
  }
  
  virtual void scene_input_options()
  { if (alternative_input_fcn != NULL) alternative_input_fcn->get_obj_ptr()->options((*gw));
  }

  void results_handle(const GeoObj& t)
  {
     if( !results.empty() )
     {
      change_mode=0;
      changed.append(t);
      update_and_redraw();
     }
     else draw_object_with_properties(t); 
   } 

  // virtual GeoEditor members  
  void scene_read_object(int srk)
  {
    GeoObj t;
    iterator hl;
    
    list<GeoObj> L;
    window& w = gw->get_window();
    
    if (srk == 0){
     if (alternative_input_fcn != NULL) {
       alternative_input_fcn->get_obj_ptr()->operator()((*gw),L);
       iterator2 it2 = L.begin();
       while( it2 != L.end() ) {
        if (add_object(*it2,hl)) changed.append(*it2); it2++; 
      }
      change_mode = 0;
      update_and_redraw(); 
      return;
     }
     
     // uses >> operator, but can be overloaded ...
     geowin_window_object_input(w,t);
     
     if (post_window_default_input_handler) post_window_default_input_handler((*gw),t);
    } 
    else {
      but = keyboard_panel(gw->get_window(), nval);
      if( but == CANCEL_BUTTON ) return;
      string_istream IS(nval);
      IS >> t; 
    }
    
    if( !add_object(t,hl) )  return; 
    
    results_handle(*hl);
    gw->update_status_line();
  }
  
  virtual bool options(panel* p)
  {  
    int i;
    list<string> hlp = get_fcn_names(i, true);
    geowin_show_options2((GeoScene*)this,p,gw,filling_color,high_light,true,hlp,i,handle_defining_points);
    set_alternative_input_fcn(i);
    return true;
  }

  void get_selected_objects(list<GeoObj>& cnt)
  {
	GeoObj obj2;
	forall(itw_obj1, sel_objs) { 
	  obj2 = *((GeoObj*)itw_obj1);
          cnt.push_back(obj2); 
        }  
  }  

  virtual void copy()
  {
    if( !sel_objs.empty() )
      {
	clip_objs.clear();
	get_selected_objects(clip_objs);
	
	GeoScene::set_select(sel_objs, false);
	gw->redraw();
      }
  }

  virtual void paste()
  {

    if ( !clip_objs.empty() )
      {
	GeoObj obj;
        iterator it;
	forall(obj, clip_objs)
	  {
	    if (add_object(obj,it)) changed.append(obj);
            GeoObj& obnew= *it;
            set_select((void*)(&(*it)),true);
	    draw_object_with_properties(obnew);
	  }

	clip_objs.clear();
	change_mode=0;
	
	//update();
	update_and_redraw();
	gw->update_status_line();
      }
  }
  
  void copy_clip_buffer(geo_scene sc)
  {
    GeoEditScene<T>* esc = (GeoEditScene<T>*) sc;
    if ( ! esc->clip_objs.empty() )
      {
	GeoObj obj;
        iterator it;
	forall(obj, esc->clip_objs)
	  {
	    if (add_object(obj,it)) changed.append(obj);
            GeoObj& obnew= *it;
            set_select((void*)(&(*it)),true);
	    draw_object_with_properties(obnew);
	  }

	esc->clip_objs.clear();
	change_mode=0;
	update_and_redraw();
	gw->update_status_line();
      }
  }
  
  void call_buffer_fcn(int w, bool flag, ostream& O)
  // flag true ... call buffer fcn, false ... print buffer contents to std output
  { 
     list<GeoObj> L;
     forall(itw_obj1,sel_objs) {
       if (flag) L.append(*((GeoObj*)itw_obj1));
       else O << *((GeoObj*)itw_obj1) << "\n";
     }
     if (flag) {
       list_item it = buffer_operations.get_item(w);
       if (it != NULL) {
        geo_scene resscn = buffer_operations[it].get_scene();
        if (resscn==NULL) buffer_operations[it].get_fcn()(*gw,L);
        else resscn->update(this);
       }
     }
  }
  
#if ((__GNUC__==3 && __GNUC_MINOR__>=4)|| (__GNUC__>3))
#define update base_class::update
#endif

  // (Affine) transformation of selected objects 
  // with current transformation ...
  void transform_options()
  { if (transformer) transformer->options(*gw); }
  
  void transform()
  {
    if( sel_objs.empty() ) return;
    
    forall(itw_obj1, sel_objs)
    {
      GeoObj& ob = *((GeoObj*)itw_obj1);
      
      if (transformer && (transformer->pre_transform(*gw,ob))){	
	    changed.append(ob);  
	    transformer->transform(ob);
	    changed2.append(ob);
	    transformer->post_transform(*gw,ob);
      }
    }
     
    change_mode=2;	
    
    update(); 
    gw->redraw(); 
  }

  void move_or_rotate_sel(double dx, double dy, double alpha, bool b, bool update_flag)
  // b true: move(dx,dy), false: rotate(dx,dy,alpha)
  { 
    if( !mouse_changing && sel_objs.empty() ) return;
    
    if( mouse_changing )
      {
	GeoObj& obj1 = *mouse_obj;
	if (b) { // move
	 if ( !pre_move_fcn  || (pre_move_fcn(*gw,obj1,dx,dy))){
	  changed.append(obj1);
	  if (!obj_defining_points_change) move( obj1, dx, dy);
	  else move_point((void*)(&obj1), dx, dy);
	  changed2.append(obj1);
	  if (post_move_fcn) post_move_fcn(*gw,obj1,dx,dy);
	 }
	}
	else { // rotate
	 if (!pre_rotate_fcn  || (pre_rotate_fcn(*gw,obj1,dx,dy,alpha))){	
	  changed.append(obj1);
	  rotate( obj1, dx, dy, alpha);
	  changed2.append(obj1);
	  if (post_rotate_fcn) post_rotate_fcn(*gw,obj1,dx,dy,alpha);
	 }
	}
      }
    else
      {
 	forall(itw_obj1, sel_objs)
	  {
	   GeoObj& ob = *((GeoObj*)itw_obj1);
	   if (b) { // move
	    if (!pre_move_fcn  || (pre_move_fcn(*gw,ob,dx,dy))){	
	      changed.append(ob);  
	      move(ob, dx, dy);
	      changed2.append(ob);
	      if (post_move_fcn) post_move_fcn(*gw,ob,dx,dy);
	    }
	   }
	   else { // rotate 
	    if (!pre_rotate_fcn || (pre_rotate_fcn(*gw,ob,dx,dy,alpha))){	 
	     changed.append(ob); 
	     rotate( ob, dx, dy, alpha);
	     changed2.append(ob);
	     if (post_rotate_fcn) post_rotate_fcn(*gw,ob,dx,dy,alpha);
	    }	   
	   }
	  }
      }	
      
     change_mode=2;	
     if (update_flag) { 
       update(); 
       gw->redraw(); 
     }
  }

  virtual void del_sel()
  { 
    if ( !sel_objs.empty() )
      {
	forall(itw_obj1, sel_objs)  {
	  GeoObj ob = *((GeoObj*)itw_obj1);
	  geo_props pr = props[itw_obj1];
	  
	  if(del_object(pr.pos)) {
	     changed.append(ob);
	  }
	}
	change_mode=1;
	update_and_redraw();
      }
  }

  virtual void clear()
  {
    if (objs){
    iterator it = objs->begin();
    iterator stop = objs->end();
    
    while( it != stop ) del_object( it++ );
    if (objs->empty()){
     sel_objs.clear(); 
     props.clear();
     mouse_obj_exist = false;

     undefine_all();
    }
    update_and_redraw();
    }
  }
  
  virtual void read(istream& in, bool keep){
   if (!keep) objs->clear();
   T tmp;
   for(;;)
    { char c;
      while (in.get(c) && isspace(c));    if (!in) break;   in.putback(c);
      GeoObj p;  in >> p;  tmp.push_back(p);
    }
    iterator it = tmp.begin(),hlp;
    while( it != tmp.end() ) {
      if (add_object(*it,hlp)) changed.append(*it); it++; 
    }
    change_mode = 0;
    update_and_redraw();     
  }  

  virtual void generate()
  {
    T tmp;
    
    // get the generator names ...
    generator_names.clear();
    geowin_generator<T>* gi;   
    forall(gi, generators) generator_names.append(gi->get_name());
    set_generator_number(-1);
    
    // set calling scene ...
    GeoWin::call_scene = this;
    
    generate_fcn(*gw, tmp);
    int nb = get_generator_number();
    if ((nb >= 0) && (nb < generators.length())) { // call one of the generators ...
      list_item lit = generators.get_item(nb);
      generators[lit]->set_parameters(*gw);
      generators[lit]->operator()(tmp);
    }
    
    iterator it = tmp.begin(),hlp;
    while( it != tmp.end() ) {
      if (add_object(*it,hlp)) changed.append(*it); it++; 
    }
    change_mode = 0;
    update_and_redraw(); 
  }

  virtual void obj_focus(){
    if( mouse_obj_exist )
      {
	GeoObj& obj = *mouse_obj; 
        GeoObj obj2;
        string_ostream O;
        O <<  obj << ends;
        string nvl=O.str();

        but= focus_dialog(nvl, gw->get_window());

        if( but == CANCEL_BUTTON ) return;
        string_istream IS(nvl);
        IS >> obj2;
 
        iterator zg;
	
	//switch off limit...
	int prevl = limit;
	limit = -1;

        if (add_object(obj2,zg)){
         GeoObj& zlo = *mouse_obj;
	 GeoObj& zln = *zg;
         set_original_properties((void*)(&zln),(void*)(&zlo));
         del_object(mouse_obj);
        }
	// switch on the limit
	limit = prevl;
       
        update_and_redraw();
      }
  }
  
  // add a copy of the mouse object ...
  // (prec: mouse object exists)
  virtual void add_mouse_object_copy(void*& new_adr)
  {
      GeoObj objct       = *mouse_obj; //copy of mouse object ...
      iterator zg;
      
      if (add_object(objct,zg)) {
        GeoObj& zln = *zg;
	new_adr = (void*)(&zln);
      }
      else new_adr = NULL;
  }
  
  // delete the mouse object ...
  virtual bool del_mouse_object(bool cf)
  { 
     GeoObj gob = *mouse_obj;
     if (del_object(mouse_obj)) { if (cf) changed.append(gob); return true;}
     return false;
  }

};

// corrections for g++-3.4.x
#if ((__GNUC__==3 && __GNUC_MINOR__>=4)||(__GNUC__ >3))
#undef gw
#undef set_obj_color
#undef set_obj_fill_color
#undef update
#endif

#ifndef NO_STATIC_DATA_MEMBER
template<class T> GeoEditSceneBuilder<T> GeoEditScene<T>::sb2;
#endif

template<class T> GeoEditScene<T>* geowin_new_scene(GeoWin& gw, T& t, string str, void (*f)(geo_scene, d3_window&, GRAPH<d3_point,int>&))
{
  GeoEditScene<T>* sc = (GeoEditScene<T>*)(GeoEditScene<T>::b2().new_scene());  
  sc->set_objref(t);
  gw.insert_scene(sc);
  sc->init_default_values();
  if (f != NULL) sc->init_d3_window = f;
  if (str != "") gw.set_name(sc,str);
  return sc;
}

template<class T> GeoEditScene<T>* geowin_new_scene(GeoWin& gw, T& t)
{
  void (*f)(geo_scene, d3_window&, GRAPH<d3_point,int>&) = NULL;
  string str = "";
  
  GeoEditScene<T>* sc = (GeoEditScene<T>*)(GeoEditScene<T>::b2().new_scene());  
  sc->set_objref(t);
  gw.insert_scene(sc);
  sc->init_default_values();
  if (f != NULL) sc->init_d3_window = f;
  if (str != "") gw.set_name(sc,str);
  return sc;
}

template<class T> GeoEditScene<T>* geowin_new_scene(GeoWin& gw, T& t, void (*f)(geo_scene, d3_window&, GRAPH<d3_point,int>&))
{
  string str = "";
  GeoEditScene<T>* sc = (GeoEditScene<T>*)(GeoEditScene<T>::b2().new_scene());  
  sc->set_objref(t);
  gw.insert_scene(sc);
  sc->init_default_values();
  if (f != NULL) sc->init_d3_window = f;
  if (str != "") gw.set_name(sc,str);
  return sc;
}

template<class T> GeoEditScene<T>* geowin_new_scene(GeoWin& gw, T& t, string str)
{
  void (*f)(geo_scene, d3_window&, GRAPH<d3_point,int>&) = NULL;
  GeoEditScene<T>* sc = (GeoEditScene<T>*)(GeoEditScene<T>::b2().new_scene());  
  sc->set_objref(t);
  gw.insert_scene(sc);
  sc->init_default_values();
  if (f != NULL) sc->init_d3_window = f;
  if (str != "") gw.set_name(sc,str);
  return sc;
}



template<class T> GeoEditScene<T>* make_edit_prototype(T* t, string str)
{
  int id = GeoEditScene<T>::b2().get_id();
  GeoScenePrototypes::set_type_name(id, str);
  return (GeoEditScene<T>*)(GeoScenePrototypes::get_prototype(id));
}

template<class T> GeoEditScene<T>* get_edit_prototype(T* t)
{
  int id = GeoEditScene<T>::b2().get_id();
  return (GeoEditScene<T>*)(GeoScenePrototypes::get_prototype(id));
}


template <class T, class R, class F>
void geowin_copy_obj_attr(GeoBaseScene<T>* inp_scene, GeoBaseScene<R>* res_scene, F test_fcn)
//bool (*test_fcn)(color&,color&,line_style&,int&,typename T::iterator&,typename R::iterator&,int) = NULL)
{
  typedef typename T::iterator                iscene_iterator;         
  typedef typename R::iterator                rscene_iterator;
  typedef typename T::value_type              iscene_vtype;         
  typedef typename R::value_type              rscene_vtype;  
  
  T& cn1 = inp_scene->get_objref();
  R& cn2 = res_scene->get_objref();
  
  iscene_iterator iit = cn1.begin();
  rscene_iterator rit = cn2.begin();
  
  int counter=0;
  
  for(;iit != cn1.end() ,rit != cn2.end();iit++,rit++){
    iscene_vtype& ob1 = *iit;
    rscene_vtype& ob2 = *rit;
    void * adr_inp = (void*)(&ob1);
    void * adr_res = (void*)(&ob2);
    
    color bc = inp_scene->get_obj_color(adr_inp);
    color fc = inp_scene->get_obj_fill_color(adr_inp);
    line_style lst = inp_scene->get_obj_line_style(adr_inp);
    int w = inp_scene->get_obj_line_width(adr_inp);
    
    if ((test_fcn == NULL) || (test_fcn(bc,fc,lst,w,iit,rit,counter))) {
      res_scene->set_obj_color(adr_res,bc);
      res_scene->set_obj_fill_color(adr_res,fc);
      res_scene->set_obj_line_style(adr_res,lst);
      res_scene->set_obj_line_width(adr_res,w);
    }
  }
  
  // add here a loop for the rest of objects in the container cn2 of res_scene???
}

// ***************************************************************************
// *****************     class GeoResultScene     ****************************

template<class S, class R> class GeoResultScene;
template<class S, class R> class __exportGeoW GeoResultSceneBuilder : public GeoSceneBuilder
{
  friend class GeoResultScene<S,R>;

  public :
  
  GeoResultSceneBuilder() : GeoSceneBuilder(new GeoResultScene<S,R>) { }
  
};

template<class S, class R>
GeoResultScene<S,R>* geowin_new_scene(GeoWin& gw,
					  void (*f)(const S&, R&),
					  GeoBaseScene<S>*,
					  R&, const char* str);

template<class S, class R>
GeoResultScene<S,R>* geowin_new_scene(GeoWin& gw,
					  void (*f)(const S&, R&),
					  GeoBaseScene<S>*, 
					  R*, const char* str);


// corrections for g++-3.4.x
#if ((__GNUC__==3 && __GNUC_MINOR__>=4) || (__GNUC__>3))
#define gw base_class::gw
#define is_visible base_class::is_visible
#define debug_mode base_class::debug_mode
#define debug_file_name base_class::debug_file_name
#define incremental_update_limit base_class::incremental_update_limit
#define br_after_false base_class::br_after_false
#define get_name base_class::get_name
#define copy_attr base_class::copy_attr
#undef change_mode
#undef changed
#undef changed2
#endif

template<class S, class R> class GeoResultScene : public GeoBaseScene<R>
{
  typedef GeoBaseScene<R> base_class;
  friend class __exportGeoW GeoResultSceneBuilder<S,R>;
  friend class __exportGeoW GeoWin;

public:  
  typedef typename S::value_type  GeoObjInput;
  typedef typename R::value_type  GeoObjOutput;  
  typedef typename S::iterator    IteratorInput;
  typedef typename R::iterator    IteratorResult;  

  
#ifdef NO_STATIC_DATA_MEMBER2  
  
  static GeoResultSceneBuilder<S,R>& b3()
  {
    static GeoResultSceneBuilder<S,R> sb3;
    return sb3;
  }
  
#else
  
  static GeoResultSceneBuilder<S,R> sb3;
  static GeoResultSceneBuilder<S,R>& b3()
  {
    return sb3;
  }
#endif
   
  GeoBaseScene<S>* input_scene;
  list<geo_scene>  influencing_scenes;
  
  bool (*test_fcn)(color&,color&,line_style&,int&, IteratorInput&, IteratorResult&,int);

  // first parameter GeoWin of the result scene, second parameter input scene
  // third parameter result scene...
  void (*update_fcn)(const S&, R&);

  geowin_update<S,R>* pt_update;
  
  GeoResultScene() : GeoBaseScene<R>() 
  { 
    //b3();
    input_scene = NULL;
    update_fcn=NULL;
    test_fcn = NULL;
    pt_update=NULL;
  }
  
  virtual ~GeoResultScene() {}
  
  virtual GeoScene* clone()
  {
    GeoResultScene<S,R>* sc = new GeoResultScene<S,R>;
    sc->init_from_prototype(this);    
    return sc;
  }   
    
  virtual bool IsSuperOrEqual( geo_scene sc ) 
  {
    if( b3().prototypeid == sc->GeoSceneId() ) return true; 
    return GeoBaseScene<R>::IsSuperOrEqual(sc);
  }
   
  virtual int GeoSceneId() const //{ return b3().prototypeid; }
  { 
    return GeoBaseScene<R>::b1().get_id(); 
    //return nr;
  }
  
  
  virtual void init_from_prototype(geo_scene)
  {
    // use prototype of super class for initialization
    int id1 =  GeoBaseScene<R>::b1().get_id();
    
    if( id1 == -1 ) 
      LEDA_EXCEPTION(1, "GeoResultScene derived from unknown base scene.");
    else
      {
	geo_scene base_proto = GeoScenePrototypes::get_prototype(id1);
	GeoBaseScene<R>::init_from_prototype(base_proto);
	edit_menu_type = -1;
      }
  }
  
  void set_input_scene(GeoBaseScene<S>* input)
  {
    input_scene = input;
    input_scene->add_dependence(this);
  }

  void set_update_object(geowin_update<S,R>* up)
  {
    pt_update= up;
  }

  void set_update_fcn( void (*f)(const S&, R&) )
  { update_fcn = f; }
  
  // call update function/ object with selected 
  // objects of scene sc
  virtual void update(geo_scene scn)
  {
    GeoEditScene<S>* sc = (GeoEditScene<S>*) scn;
    list<GeoObjInput> selected_objs;
    sc->get_selected_objects(selected_objs);
    
    S input_container;
    GeoObjInput obj_iter;
    
    forall(obj_iter, selected_objs) input_container.push_back(obj_iter);
  
    //set calling GeoWin and scene 
    GeoWin::call_win = gw;
    GeoWin::call_scene = this;
    GeoWin::call_inp_scene = input_scene;   
    
    if (pt_update != NULL) { // function object   
      pt_update->update(input_container, *objs);
    }  
    else { // function
      if (update_fcn != NULL) update_fcn(input_container, *objs);
    }
    
    GeoBaseScene<R>::update();
    
    gw->redraw();
  }
  
  virtual void update()
  {
    if (input_scene == NULL) return;
  
    //set calling GeoWin and scene 
    GeoWin::call_win = gw;
    GeoWin::call_scene = this;
    GeoWin::call_inp_scene = input_scene;
    
    float timeact= used_time();

   if (is_visible()){
    if (debug_mode) gw->write_scene(input_scene, debug_file_name, false); 
   
    if (pt_update != NULL) { // function object
      int mode = input_scene->change_mode;    
      bool ret = false;
      bool inc_break = false; // exit from incr. operations
      
      typename list<GeoObjInput>::iterator it,it2;

      if ((mode >=0) && (mode <=2)) { 
       list<GeoObjInput>& L1 = input_scene->changed;
       list<GeoObjInput>& L2 = input_scene->changed2;
       
       if (L1.size() < incremental_update_limit){
       switch(mode)
       {
        case 0: // add ...
        {
	 for(it=L1.begin();it != L1.end(); it++) {
	   ret = pt_update->insert(*it);
	   if (! ret) {
	     inc_break = true;
	     if (br_after_false) break;
	   }
	 }
	 break;
        }
        case 1: // delete ...
        {
	 for(it=L1.begin();it != L1.end(); it++) {
	   ret = pt_update->del(*it); 
	   if (! ret) {
	     inc_break = true;
	     if (br_after_false) break;
	   }
	 }
	 break;
        }
        case 2: // change ...
        {
         for(it=L1.begin(),it2=L2.begin();it != L1.end(); it++,it2++) {
	   ret = pt_update->change(*it,*it2);
	   if (! ret) {
	     inc_break = true;
	     if (br_after_false) break;
	   }
	 }
         break;
        }
       }
       }
      }
      // update function of the update object ...
      if ((! ret) || inc_break) {
          if (influencing_scenes.empty()) pt_update->update(input_scene->get_objref(),*objs) ;  
	  else {  // new call ...
	    // get addresses (void*) of the containers of the scenes in influencing_scenes ...
	    list<void*> ptr_list;

	    geo_scene sc_iter;
	    
	    forall(sc_iter,influencing_scenes) ptr_list.append(sc_iter->get_untyped_objptr());

	    typedef void (*FU_HELP)(...);
	    
	    FU_HELP helper_fcn = (FU_HELP) pt_update->get_gen_fcn();
            // call of helper function ...
	    if (helper_fcn) gw->call_update_fcn(helper_fcn,input_scene->get_untyped_objptr(), (void*)objs, ptr_list);
	  }
      }
    }
    
    else { // function
      if (update_fcn != NULL) update_fcn(input_scene->get_objref(), *objs);
    }
   }

   if (gw && gw->get_time_meas() && (pt_update!=NULL || update_fcn!=NULL))
      cout << get_name() << ":" << used_time(timeact) << " s.\n";

   // set object attributes ...
   if (copy_attr) geowin_copy_obj_attr(input_scene, this, test_fcn); 
      
   GeoBaseScene<R>::update();

  } 
  

};

// corrections for g++-3.4.x
#if ((__GNUC__==3 && __GNUC_MINOR__>=4) || (__GNUC__>3))
#undef gw
#undef pre_add_fcn
#undef post_add_fcn
#undef pre_add_changer
#undef pre_del_fcn
#undef post_del_fcn
#undef pre_move_fcn
#undef post_move_fcn
#undef pre_rotate_fcn
#undef post_rotate_fcn
#endif

#ifndef NO_STATIC_DATA_MEMBER2 
template<class S, class R> GeoResultSceneBuilder<S,R> GeoResultScene<S,R>::sb3;
#endif



template<class S, class R>
GeoResultScene<S,R>* geowin_new_scene(GeoWin& gw,
					  void (*f)(const S&,R&),
					  GeoBaseScene<S>* input, 
					  const char* str, int fl)
{
  GeoResultScene<S,R>* sc = 
    (GeoResultScene<S,R>*)(GeoResultScene<S,R>::b3().new_scene());
  sc->set_base_name(str);
  sc->set_input_scene(input);
  sc->set_update_fcn(f);
 
  gw.insert_scene(sc);
  sc->init_default_values();
  sc->update();  
  return sc; 
}



template<class S, class R>
GeoResultScene<S,R>* geowin_new_scene(GeoWin& gw,
					  void (*f)(const S&,R&), geo_scene sc, 
					  const char* str, void (*d3f)(geo_scene, d3_window&, GRAPH<d3_point,int>&) = NULL)
{ GeoResultScene<S,R>* scr = geowin_new_scene(gw, f, (GeoBaseScene<S>*)sc, str, 0); 
  if (d3f != NULL) scr->init_d3_window = d3f;
  return scr;
}

template<class S, class R>
GeoResultScene<S,R>* geowin_new_scene(GeoWin& gw, geowin_update<S,R>& up, geo_scene sc, const char* name, 
                                      void (*d3f)(geo_scene, d3_window&, GRAPH<d3_point,int>&) = NULL)
{
  GeoResultScene<S,R>* res =  (GeoResultScene<S,R>*)(GeoResultScene<S,R>::b3().new_scene());
  res->set_base_name(name);
  res->set_input_scene((GeoBaseScene<S>*)sc); 
  res->set_update_object(&up);
 
  gw.insert_scene(res);
  res->init_default_values();
  res->update();  
  
  if (d3f != NULL) res->init_d3_window = d3f;
  return res;  
}


// NEW ----------------------------------------------

template<class S, class R>
GeoResultScene<S,R>* geowin_new_scene(GeoWin& gw, geowin_update<S,R>& up, const list<geo_scene>& infl, const char* name, 
                                      void (*d3f)(geo_scene, d3_window&, GRAPH<d3_point,int>&) = NULL)
{
  if (infl.empty()) return NULL; // infl must not be empty ...
  geo_scene sc = infl.head();

  GeoResultScene<S,R>* res =  (GeoResultScene<S,R>*)(GeoResultScene<S,R>::b3().new_scene());
  res->set_base_name(name);
  res->set_input_scene((GeoBaseScene<S>*)sc); 
  res->set_update_object(&up);
  
  list<geo_scene> infl2 = infl;
  infl2.pop();
  res->influencing_scenes = infl2;
 
  gw.insert_scene(res);
  res->init_default_values();
  res->update();  
  
  // add dependences ...
  geo_scene sc_iter;
  forall(sc_iter, infl) gw.add_dependence(sc_iter, res);
  
  if (d3f != NULL) res->init_d3_window = d3f;
  return res;  
}

// --------------------------------------------------





template<class S, class R>
GeoResultScene<S,R>* geowin_new_scene(GeoWin& gw, geowin_update<S,R>& up,geowin_redraw& rb, geo_scene sc,const char* name,
                                      void (*d3f)(geo_scene, d3_window&, GRAPH<d3_point,int>&) = NULL)
{
  GeoResultScene<S,R>* res =  (GeoResultScene<S,R>*)(GeoResultScene<S,R>::b3().new_scene());
  res->set_base_name(name);
  res->set_input_scene((GeoBaseScene<S>*)sc); 
  res->set_update_object(&up);

  gw.insert_scene(res);
  res->init_default_values();
  gw.set_redraw_pt(res, &rb); 
  res->update();  
  
  if (d3f != NULL) res->init_d3_window = d3f;  
  return res;  
}

template<class S, class R>
GeoResultScene<S,R>* geowin_new_scene(GeoWin& gw, geowin_update<S,R>& up,geowin_redraw_container<R>& rd, geo_scene sc,const char* name,
                                      void (*d3f)(geo_scene, d3_window&, GRAPH<d3_point,int>&) = NULL)
{
  GeoResultScene<S,R>* res =  (GeoResultScene<S,R>*)(GeoResultScene<S,R>::b3().new_scene());
  res->set_base_name(name);
  res->set_input_scene((GeoBaseScene<S>*)sc); 
  res->set_update_object(&up);
  
  // redraw container object ...
  res->gw_redraw_cnt = &rd;

  gw.insert_scene(res);
  res->init_default_values();
  res->update();  
  
  if (d3f != NULL) res->init_d3_window = d3f;  
  return res;  
}

// setting update objects
template<class S,class R>
void geowin_set_update(geo_scene res, geowin_update<S,R>& up)
{ 
 GeoResultScene<S,R>* result = (GeoResultScene<S,R>*)res;
 result->set_update_fcn(NULL);
 result->set_update_object(&up);
}

// setting update functions
template<class S,class R>
void geowin_set_update(geo_scene res, void (*f_update)(const S&, R&))
{
 GeoResultScene<S,R>* result = (GeoResultScene<S,R>*)res;
 result->set_update_fcn(f_update);
 result->set_update_object(NULL); 
}

/*{\Mtext \bigskip {\bf 4. Non-Member Functions} }*/

class __exportGeoW GeoEditorMember : public GeoFunctions
{
  void (GeoEditor::*f)(void);
  
  public : 
  
  GeoEditorMember(void (GeoEditor::*g)()) : GeoFunctions(""), f(g) {}
 
  virtual ~GeoEditorMember() {}
  virtual void call(GeoWin& gw) 
  {  
    geo_scene  sc = gw.get_active_scene();
    geo_editor ed = sc ? sc->type_editor() : 0;
    if( ed ) (ed->*f)();
  }
  
  LEDA_MEMORY(GeoEditorMember)
};

class __exportGeoW GeoSceneMember : public GeoFunctions
{
  void (GeoScene::*f)(void);
  
  public : 
  
  GeoSceneMember(void (GeoScene::*g)()) : GeoFunctions(""), f(g) {}
  virtual ~GeoSceneMember() {}
  virtual void call(GeoWin& gw) 
  {  
    geo_scene  sc = gw.get_active_scene();
    if( sc ) (sc->*f)();
  }
  
  LEDA_MEMORY(GeoSceneMember)
};


template<class F> 
void geowin_add_user_call(GeoWin& gw, string label, F f)
{
  if( f)
    {  
      GeoFunctions* fp = new GeoFunction<F>(f, label);
      gw.add_call(fp, label);
    }
}

inline GeoWin* get_geowin(geo_scene sc)
/*{\Mfuncl
  returns a pointer to the |GeoWin| of |sc|.}*/
{ return sc->get_geowin(); }

template<class CONTAINER>
inline bool get_objects(geo_scene sc, CONTAINER& c)
/*{\Mfuncl
 If the contents of scene |sc| matches type |CONTAINER|, then the contents of scene |sc| is copied to |c|.}*/
{ GeoWin* gw = sc->get_geowin(); 
  return gw->get_objects(sc,c);
}


void __exportGeoW geo_call(GeoWin& gw, void (*fkt)(GeoWin&), string);
void __exportGeoW geo_call(GeoWin& gw, void (*fkt)(), string);

bool __exportGeoW geowin_identify_file(ifstream& is, string& type_name, string& sc_name);


template<class T> bool geowin_get_objects(GeoWin& gw, geo_scene sc, T& t)
{
    GeoBaseScene<T>* pr=get_base_prototype GEOWIN_FU_TEMPL(&t);
    if( pr->IsMyKindOfScene(sc) )
      {
	pr = (GeoBaseScene<T>*)sc;
	t = pr->get_objects();
	return true;
      }
    return false;
}

template<class T> bool geowin_set_objects(GeoWin& gw, geo_scene sc, T& t)
{
    GeoBaseScene<T>* pr=get_base_prototype GEOWIN_FU_TEMPL(&t);
    if( pr->IsMyKindOfScene(sc) )
      {
	pr = (GeoBaseScene<T>*)sc;
	pr->set_objects(t);
	return true;
      }
    return false;
}

  
template <class T>
void geowin_get_selected_objects(GeoWin& gw,GeoEditScene<T>* sc, T& cnt)
{
   typedef typename T::value_type value_type;
   list<value_type> HL;
   
   sc->get_selected_objects(HL);
   value_type iter;
   forall(iter,HL) cnt.push_back(iter);
}

template <class T>
void geowin_set_selected_objects(GeoWin& gw,GeoEditScene<T>* sc,const list<typename T::iterator>& LIT)
{
   typedef typename T::iterator iterator;
   iterator it;
   forall(it,LIT) sc->set_select((void*)(&(*it)),true);
}

template <class T>
void geowin_set_selected_objects(GeoWin& gw,GeoEditScene<T>* sc)
{
   sc->select_in_rect(false,0,0,0,0,true);
}

template <class T>
void geowin_set_selected_objects(GeoWin& gw,GeoEditScene<T>* sc,const rectangle& R)
{
   sc->select_in_rect(false,R.xmin(),R.ymin(),R.xmax(),R.ymax(),false);
}

template <class T>
void geowin_set_selected_objects(GeoWin& gw,GeoEditScene<T>* sc,geowin_handle_object<typename T::value_type>& h)
{
   sc->select_callback(false,&h);
}

template <class T>
const T& geowin_get_objects_reference(GeoWin& gw,GeoBaseScene<T>* sc)
{
   return sc->get_objref();
}

//  template functions for setting handlers ...
// add handler
  
template<class T,class F>
bool geowin_set_pre_add_handler(GeoEditScene<T>* sc, F handler )
{ 
 if (sc == NULL) return false;
 sc->pre_add_fcn = handler; 
 return true;
}
  
template<class T,class F>
bool geowin_set_pre_add_change_handler(GeoEditScene<T>* sc, F handler )
{ 
 if (sc == NULL) return false;
 sc->pre_add_changer = handler; 
 return true;
}
  
template<class T,class F>
bool geowin_set_post_add_handler(GeoEditScene<T>* sc, F handler )
{ 
 if (sc == NULL) return false;
 sc->post_add_fcn = handler; 
 return true;
}

// delete handler
    
template<class T,class F>
bool geowin_set_pre_del_handler(GeoEditScene<T>* sc, F handler )
{ 
 if (sc == NULL) return false;
 sc->pre_del_fcn = handler; 
 return true;
}
  
template<class T,class F>
bool geowin_set_post_del_handler(GeoEditScene<T>* sc, F handler )
{ 
 if (sc == NULL) return false;
 sc->post_del_fcn = handler; 
 return true;
}
  
// change handler
  
template<class T,class F>
bool geowin_set_start_change_handler(GeoEditScene<T>* sc, F handler )
{
 typedef bool (*FU)(...);
 if (sc == NULL) return false;
 sc->start_change_fcn = (FU) handler; 
 return true;
}

template<class T,class F>
bool geowin_set_pre_move_handler(GeoEditScene<T>* sc, F handler )
{ 
 if (sc == NULL) return false;
 sc->pre_move_fcn = handler; 
 return true;
}
  
template<class T,class F>
bool geowin_set_post_move_handler(GeoEditScene<T>* sc, F handler )
{ 
 if (sc == NULL) return false;
 sc->post_move_fcn = handler; 
 return true;
}
  
template<class T,class F>
bool geowin_set_pre_rotate_handler(GeoEditScene<T>* sc, F handler )
{ 
 if (sc == NULL) return false;
 sc->pre_rotate_fcn = handler; 
 return true;
}
  
template<class T,class F>
bool geowin_set_post_rotate_handler(GeoEditScene<T>* sc, F handler )
{ 
 if (sc == NULL) return false;
 sc->post_rotate_fcn = handler; 
 return true;
}

template<class T,class F>
bool geowin_set_end_change_handler(GeoEditScene<T>* sc, F handler )
{
 typedef void (*FU)(...);
 if (sc == NULL) return false;
 sc->end_change_fcn = (FU) handler; 
 return true; 
}

template<class T>
bool geowin_set_generate_fcn(GeoEditScene<T>* sc, void (*f)(GeoWin& gw,T& L))
{
 if (sc == NULL) return false;
 sc->generate_fcn = f;
 return true;
}

template<class T>
bool geowin_add_generator(GeoEditScene<T>* sc, geowin_generator<T>& Gen)
{ 
 if (sc == NULL) return false;
 sc->generators.append(&Gen);
 return true;
}

template<class T, class T2>
bool geowin_set_edit_object_fcn(GeoEditScene<T>* sc, T2 f)
{
 typedef void (*FU)(...);
 if (sc == NULL) return false;
 sc->edit_obj_fcn =  (FU) f;
 return true;
}

template<class T>
void*  geowin_get_edit_object_fcn(GeoEditScene<T>* sc)
{
 return (void*) (sc->edit_obj_fcn);
}

template <class T,class T2>
bool geowin_set_defining_points_fcn(GeoBaseScene<T>* sc, T2 f) 
{ 
 if (sc == NULL) return false;
 sc->set_defining_points_fcn(f);
 return true;
}
  
template <class T,class T2>
bool geowin_set_move_point_fcn(GeoEditScene<T>* sc, T2 f)
{
 typedef void (*FU)(...);
 if (sc == NULL) return false;
 sc->move_point_fcn = (FU) f;
 return true;
}

template<class T,class F>
bool geowin_add_algorithm(GeoEditScene<T>* sc, F f, string name)
{
 if (sc == NULL) return false;
 sc->add_algorithm(f,name);
 return true;
}

template<class S,class R>
GeoResultScene<S,R>* geowin_add_algorithm(GeoWin& gw, geowin_update<S,R>& up, GeoEditScene<S>* sc, string scene_name,
                     string algo_name,
                     void (*d3f)(geo_scene, d3_window&, GRAPH<d3_point,int>&) = NULL)
{
  GeoResultScene<S,R>* res =  (GeoResultScene<S,R>*)(GeoResultScene<S,R>::b3().new_scene());
  res->set_base_name(scene_name);

  res->set_update_object(&up);
  res->add_owner(sc); // new !!!

  gw.insert_scene(res);
  res->init_default_values();
  
  if (d3f != NULL) res->init_d3_window = d3f;  
  // add algorithm to sc...
  sc->add_algorithm((geo_scene)res, algo_name);  
  
  return res;  
}

// third template argument because of SUN CC ...
template<class S, class R, class T>
bool geowin_set_copy_attr_test_fcn(GeoResultScene<S,R>* sc, T f)
{
 if (sc==NULL) return false;
 sc->test_fcn = f; 
 return true;
}


// here a second template argument because of SUN CC ...
template<class T, class T2>
bool geowin_set_input_object(GeoEditScene<T>* sc, const GeoInputObject<T2>& obj, string fname)
{
 if (sc == NULL) return false;
 sc->set_alternative_input_object(obj,fname);
 return true;
}

template<class T, class T2>
bool geowin_add_input_object(GeoEditScene<T>* sc, const GeoInputObject<T2>& obj, string fname)
{
 if (sc == NULL) return false;
 sc->add_alternative_input_object(obj,fname);
 return true;
}

template<class T, class T2>
void geowin_set_draw_object_fcn(GeoBaseScene<T>* sc, T2 fcn)
{
 typedef void (*FU)(...);  
 sc->draw_fcn = (FU) fcn; 
}


// set object properties in scenes
template<class T> color geowin_get_obj_color(GeoBaseScene<T>* sc, void* adr)
{ return sc->get_obj_color(adr); }

template<class T> color geowin_set_obj_color(GeoBaseScene<T>* sc, void* adr, color c)
{ return sc->set_obj_color(adr,c); } 

template<class T> color geowin_get_obj_fill_color(GeoBaseScene<T>* sc, void* adr)
{ return sc->get_obj_fill_color(adr); }

template<class T> color geowin_set_obj_fill_color(GeoBaseScene<T>* sc, void* adr, color c)
{ return sc->set_obj_fill_color(adr,c); }  

template<class T> line_style geowin_get_obj_line_style(GeoBaseScene<T>* sc, void* adr)
{ return sc->get_obj_line_style(adr); }    

template<class T> line_style geowin_set_obj_line_style(GeoBaseScene<T>* sc, void* adr, line_style l)
{ return sc->set_obj_line_style(adr,l); }  

template<class T> int geowin_get_obj_line_width(GeoBaseScene<T>* sc, void* adr)
{ return sc->get_obj_line_width(adr); }   

template<class T> int geowin_set_obj_line_width(GeoBaseScene<T>* sc, void* adr, int w)  
{ return sc->set_obj_line_width(adr,w); }    


// ----------------------------------------------------------------------------


// corrections for g++-3.4.x
#if ((__GNUC__==3 && __GNUC_MINOR__>=4) || (__GNUC__>3))
#define get_transformation base_class::get_transformation
#endif

template<class GeoObj,class TRANS = transform>
class geowin_gui_transform : public geowin_affine_leda_transform<GeoObj,TRANS> 
{

typedef geowin_affine_leda_transform<GeoObj,TRANS> base_class;
public:
 geowin_gui_transform() : 
        geowin_affine_leda_transform<GeoObj,TRANS>()
 { }
 
 virtual bool pre_transform(GeoWin& gw, const GeoObj& obj)
 { return true; }
 
 void options(GeoWin& gw)
 {
   matrix mat = get_transformation().T_matrix();
   
   matrix_editor<matrix> med("Edit Transformation matrix", mat);
   
   med.my_open(gw.get_window());   
   
   TRANS nt(mat);
   
   set_transformation(nt);                     
 }
};

// ----------------------------------------------------------------------------


template<class GeoObj,class TRANS = rat_transform >
class geowin_gui_rat_transform : public geowin_affine_leda_transform<GeoObj,TRANS> 
{
typedef geowin_affine_leda_transform<GeoObj,TRANS> base_class;
public:
 geowin_gui_rat_transform() : 
        geowin_affine_leda_transform<GeoObj,TRANS>()
 { }
 
 virtual bool pre_transform(GeoWin& gw, const GeoObj& obj)
 { return true; }
 
 void options(GeoWin& gw)
 {
   integer_matrix mat = get_transformation().T_matrix();
   
   rat_matrix_editor med("Edit Transformation matrix", mat);
   
   med.my_open(gw.get_window());   
   
   TRANS nt(mat);
   
   set_transformation(nt);                        
 }
};



GEOWIN_END_NAMESPACE

// corrections for g++-3.4.x
#if ((__GNUC__==3 && __GNUC_MINOR__>=4) || (__GNUC__> 3))
#undef props
#undef edit_menu_type
#undef mouse_obj
#undef limit
#undef pre_add_fcn
#undef pre_add_changer
#undef objs
#undef set_default_properties
#undef gw
#undef cyclic_colors
#undef cyclic_colors_counter
#undef set_obj_color
#undef set_obj_fill_color
#undef cyclic_fill_colors_counter
#undef post_add_fcn
#undef pre_del_fcn
#undef post_del_fcn
#undef mouse_obj_exist
#undef sel_objs
#undef fcn_state
#undef pre_move_fcn
#undef post_move_fcn
#undef transformer
#undef pre_rotate_fcn
#undef post_rotate_fcn
#undef undefine_all
#undef find_object
#undef results
#undef change_mode
#undef changed
#undef but
#undef nval
#undef filling_color
#undef update_and_redraw
#undef high_light
#undef handle_defining_points
#undef changed2
#undef update
#undef mouse_changing
#undef obj_defining_points_change
#undef generator_names
#undef set_generator_number
#undef get_generator_number
#undef set_original_properties
#undef move_point
#undef is_visible
#undef debug_mode
#undef debug_file_name
#undef incremental_update_limit
#undef br_after_false
#undef get_name
#undef copy_attr
#undef get_transformation
#endif


#if LEDA_ROOT_INCL_ID == 600151
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif



