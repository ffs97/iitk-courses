#ifndef LEDA_D3_GL_WINDOW_H
#define LEDA_D3_GL_WINDOW_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600146
#include <LEDA/internal/PREAMBLE.h>
#endif


#include <LEDA/graphics/gl_window.h>

#include <LEDA/graphics/gl_camera.h>
using namespace vgl;

#include <LEDA/graphics/d3_gl_scene.h>
#include <LEDA/graphics/d3_gl_col.h>


#define ARROW_DEPTH_QUOTIENT 40
#define ARROW_QUAD_QUOTIENT  80 


LEDA_BEGIN_NAMESPACE 

/*{\Moptions usesubscripts=yes}*/


void d3_gl_resize(int w, int h);

typedef four_tuple<char*,int,int,GLint> tex_params; // data, w, h, filter


/*{\Manpage {d3_gl_window} {} {D3-GL-WINDOW} {W}}*/

class __exportC d3_gl_window 
{

typedef void (*gl_func) (d3_gl_window*, gl_window*);
typedef void (*resize_func) (int,int);

friend class __exportC d3_gl_scene;

/*{\Mdefinition
An instance |\Mvar| of the data type |\Mname| draws a set of $d3\_gl\_scene$-objects
three-dimensionally with OpenGL in a LEDA-$gl\_window$. If you want to use a
panel-section you have to overgive |\Mvar| such a one explicitly because a
LEDA-gl\_window currently doesn't support a panel win. For navigating through 3d-space
you can use an instance of the camera-class $gl\_camera$. }*/


protected :


gl_window*   gl_win;
window*      second_win; // panel_win or main win : depends on constructor and its parameters                          
bool         real_gl;

int width, height;


bool        draw_axis;
float	    axis_line_w;
d3_gl_colF  axis_line_c;

bool	    draw_coord_with_ln;
float	    coord_line_w;
d3_gl_colF  coord_line_c;

bool	    draw_coord_with_pts;
float       pt_size;
d3_gl_colF  pt_col;

d3_gl_colF  bg_color;


double xmin , xmax;
double ymin , ymax;
double zmin , zmax;

double x_grid_dist;
double y_grid_dist;
double z_grid_dist;

sortseq<GLuint*,tex_params> textures;

set<d3_gl_scene*> scenes;

gl_func prae_gl_func;
gl_func post_gl_func;

resize_func rsz_func;

gl_camera* cam;


void base_init();


public :


/*{\Mcreation W}*/


 d3_gl_window(window& P, const gl_camera& gl_cam,
 	      const set<d3_gl_scene*>& S = set<d3_gl_scene*>(compare));
 /*{\Mcreate
 creates an instance |\Mvar| of type |\Mname|, whereby |P| is used as a
 panel-win containing the LEDA-$gl\_window$. $gl\_cam$ is the camera-object.
 $S$ is an optional parameter pointing to a set of pointers to instances of the
 class $d3\_gl\_scene$. }*/ 
 
 d3_gl_window(const gl_window& glW, const gl_camera& gl_cam,
			  window* M = NULL, const set<d3_gl_scene*>& S = set<d3_gl_scene*>(compare));
 /*{\Mcreate
     creates an instance |\Mvar| of type |\Mname|. $glW$ is the
     LEDA-$gl\_window$ and $gl\_cam$ is used as a virtual camera. $M$ points to
	 an panel-window, which is optional. $S$ points to a set of pointers to
	 instances of the class $d3\_gl\_scene$ and is optional as well. }*/

 ~d3_gl_window(); 


/*{\Moperations 2 4}*/

/*{\Mtext
{\bf 3.1 Scenes} }*/

void  add_scene(d3_gl_scene* sc);
/*{\Mop adds $sc$ to |\Mvar|. }*/

void  del_scene(d3_gl_scene* sc);
/*{\Mop removes $sc$ from |\Mvar|. }*/ 

void  join_scenes(const set<d3_gl_scene*>& S);
/*{\Mop adds all scenes in $S$ to |\Mvar|. }*/

void  disjoin_scenes(const set<d3_gl_scene*>& S);
/*{\Mop removes all scenes in $S$ from |\Mvar|. }*/

void  clear_scenes();
/*{\Mop removes all scenes from |\Mvar|. }*/


/*{\Mtext
{\bf 3.2 Functions} }*/

gl_func get_prae_gl_func() const { return prae_gl_func; }
/*{\Mop returns a pointer to the prae\_gl-function. This
        function is executed before the scenes are drawn
        and might contain extra OpenGL-code.}*/

void set_prae_gl_func(gl_func f) { prae_gl_func = f; }
/*{\Mop sets the prae\_gl-function to f. The prae\_gl-function
        is executed before the scenes are drawn
        and might contain extra OpenGL-code.}*/

gl_func get_post_gl_func() const { return post_gl_func; }
/*{\Mop returns a pointer to the post\_gl-function. This
        function is executed after the scenes are drawn
        and might contain extra OpenGL-code.}*/

void set_post_gl_func(gl_func f) { post_gl_func = f; }
/*{\Mop sets the post\_gl-function to f. The post\_gl-function
        is executed after the scenes are drawn
        and might contain extra OpenGL-code.}*/

resize_func get_resize_func() const { return rsz_func; }
/*{\Mop returns a pointer to the current resize-function. This
        function is called everytime when the window-size changes.
        It adjusts the viewport and sets the projection-matrix as
        practised in OpenGL.}*/

void set_resize_func(resize_func f) { rsz_func = f; } 
/*{\Mop sets the current resize-function to f. This
        function is called everytime when the window-size changes.
        It adjusts the viewport and sets the projection-matrix as
        practised in OpenGL. You can see this method as equivalent
        to the GLUT-method glutReshapeFunc().}*/


protected : 

void draw_scenes(); 
void draw_pointed_plane(const double& mp_x, const double& mp_y,
						const double& mp_z, const double& ypos) const;
void draw_pointed_coord(const double& mp_x, const double& mp_y, const double& mp_z) const;
void draw_coord_with_lines(const double& mp_x, const double& mp_y, const double& mp_z) const;
void draw_coord_arrow(vector* v) const;

void gl_resize() const;
void gl_init() const;


public :


/*{\Mtext
{\bf 3.3 Texturing} }*/

GLuint*  reg_tex(char* t_data, int width, int height, const GLint& filter);
/*{\Mop  registers the texture defined by $t\_data$, $width$, $height$ and $filter$.
         A pointer to an OpenGL texture number, that was created during the
		 registration, is returned. In particular every texture used in an extra
		 drawing-function has to be registered by this method. Before using the
         registered texture in an extra drawing-function you have to bind it
		 with OpenGL by taking the created texture number.}*/

void  unreg_tex(GLuint* t_num);
/*{\Mop  unregisters the texture with the number identified by $t\_num$.		 
		 \precond $t\_num$ has to point to a valid texture number.}*/

void  set_tex_filter(GLuint* t_num, const GLint& filter);
/*{\Mop  sets the current filter of the texture identified by $t\_num$ to $filter$.			 
		 \precond $t\_num$ has to point to a valid texture number.}*/

GLint get_tex_filter(GLuint* t_num);
/*{\Mop  returns the current filter of the texture identified by $t\_num$.			 
		 \precond $t\_num$ has to point to a valid texture number.}*/


/*{\Mtext
{\bf 3.4 Drawing} }*/

void draw();
/*{\Mop  draws the whole scene with OpenGL in an LEDA-$gl\_window$.}*/


protected :

void  reg_scene_tex(GLuint* t_num, char* t_data, int width, int height,const GLint& filter);
void  update_textures();
void  print_tex_nums();


public : 
	

/*{\Mtext
{\bf 3.5 Events} }*/

int  get_event(int& val, double& x, double& y) const;
/*{\Mop  if there is an event for |\Mvar| in the event queue it is
         returned. In this case the button or key is assigned to $val$
         and (if button) the mouse-position to $(x,y)$. Otherwise
         $no\_event$ is returned. This method is equivalent to the method
         with the same name of the class $window$. }*/

int  read_event(int& val, double& x, double& y) const;
/*{\Mop  waits for the next event and returns it. $val$, $x$ and $y$
         are used as in the previous method. There is an equivalent
         method in the class $window$, too.}*/

int  get_mouse(double& x, double& y) const;
/*{\Mop  non-blocking event operation, i.e., if a button was pressed,
         its number is returned and the mouse position is assigned to
         $(x,y)$. otherwise the constant $NO\_BUTTON$ is returned.}*/

int  read_mouse(double& x, double& y) const;
/*{\Mop  waits until a mouse button is pressed and assigns the
         mouse position to $(x,y)$.}*/


/*{\Mtext
{\bf 3.6 Status Window} }*/

BASE_WINDOW* create_status_window(color bc=ivory, int h=17) const;
/*{\Mop  creates a status window with background color $bc$ and height $h$.}*/

BASE_WINDOW* open_status_window(color bc=ivory, int h=17) const;
/*{\Mop  opens a status window with background color $bc$ and height $h$.}*/

void         close_status_window() const;
/*{\Mop  closes the status window.}*/

void         destroy_status_window() const;
/*{\Mop  destroys the status window.}*/

void         set_status_string(const char* c) const;
/*{\Mop  sets the status string to $c$.}*/

const char*  get_status_string() const;
/*{\Mop  returns the status string.}*/
 
BASE_WINDOW* get_status_window() const; 
/*{\Mop  returns the status window.}*/


/*{\Mtext
{\bf 3.7 Coordinate System} }*/

double get_xmin() const { return xmin; }
/*{\Mop  returns xmin of the coordinate system.}*/

double get_xmax() const { return xmax; }
/*{\Mop  returns xmax of the coordinate system.}*/

double get_ymin() const { return ymin; }
/*{\Mop  returns ymin of the coordinate system.}*/

double get_ymax() const { return ymax; }
/*{\Mop  returns ymax of the coordinate system.}*/

double get_zmin() const { return zmin; }
/*{\Mop  returns zmin of the coordinate system.}*/

double get_zmax() const { return zmax; }
/*{\Mop  returns zmax of the coordinate system.}*/


void set_xmin(const double& d) { xmin = d; }
/*{\Mop  sets xmin of the coordinate system to $d$.}*/

void set_xmax(const double& d) { xmax = d; }
/*{\Mop  sets xmax of the coordinate system to $d$.}*/

void set_ymin(const double& d) { ymin = d; }
/*{\Mop  sets ymin of the coordinate system to $d$.}*/

void set_ymax(const double& d) { ymax = d; }
/*{\Mop  sets ymax of the coordinate system to $d$.}*/

void set_zmin(const double& d) { zmin = d; }
/*{\Mop  sets zmin of the coordinate system to $d$.}*/

void set_zmax(const double& d) { zmax = d; }
/*{\Mop  sets zmax of the coordinate system to $d$.}*/


void get_coord(double& x0, double& x1, double& y0, double& y1, double& z0, double& z1) const;
/*{\Mop  assigns xmin to $x0$ and xmax to $x1$. same with $y0$, $y1$, $z0$ and $z1$.}*/

void set_coord(const double& x0, const double& x1, const double& y0,
		  const double& y1, const double& z0, const double& z1, const bool& update_cam = true);
/*{\Mop  the coordinate system is set to the new system defined by $x0$, $x1$,
         $y0$, $y1$, $z0$ and $z1$. If $update\_cam$ is true, the camera position
         is adapted so that it keeps its relative position in space. Otherwise
         the camera-position doesn't change, i.e. keeps its absolute position.}*/

void set_coord_by_scenes(const bool& update_cam = true);
/*{\Mop  computes a coordinate system, that includes all scenes.
         the parameter $update\_cam$ behaves as in the previous method. 
         \precond |\Mvar| has to contain at least one scene.}*/


/*{\Mtext
{\bf 3.8 Getting/Setting Parameters} }*/

d3_gl_colF get_bg_color() const
{ return bg_color; }
/*{\Mop  returns the background color.}*/

void set_bg_color(const d3_gl_colF& c);
/*{\Mop  sets the background color to $c$.}*/


d3_gl_colF get_axis_line_c() const
{ return axis_line_c; }
/*{\Mop  returns the axis line color.}*/

void set_axis_line_c(const d3_gl_colF& c)
{ axis_line_c = c; }
/*{\Mop  sets the axis line color to $c$.}*/


d3_gl_colF get_coord_line_c() const
{ return coord_line_c; }
/*{\Mop  returns the coord line color.}*/

void set_coord_line_c(const d3_gl_colF& c)
{ coord_line_c = c; }
/*{\Mop  sets the coord line color to $c$.}*/


d3_gl_colF get_pt_col() const
{ return pt_col; }
/*{\Mop  returns the point color.}*/

void set_pt_col(const d3_gl_colF& c)
{ pt_col = c; }
/*{\Mop  sets the point color to $c$.}*/



void set_pt_size(const float& s)   
{ if(s > 0.0) pt_size = s; }
/*{\Mop  sets the point size to $s$.
         \precond $0\le s$.}*/

float get_pt_size() const
{ return pt_size; }
/*{\Mop  returns the point size.}*/


void set_axis_line_w(const float& w)   
{ if(w > 0.0) axis_line_w = w; }
/*{\Mop  sets the width of the axis line to $w$.
         \precond $0\le w$.}*/

float get_axis_line_w() const
{ return axis_line_w; }
/*{\Mop  returns the axis line width.}*/


void set_coord_line_w(const float& w)   
{ if(w > 0.0) coord_line_w = w; }
/*{\Mop  sets the width of the coord lines to $w$.
         \precond $0\le w$.}*/

float get_coord_line_w() const
{ return coord_line_w; }
/*{\Mop  returns the coord line width.}*/



void set_draw_axis(const bool& b)
{ draw_axis = b; }
/*{\Mop  sets the draw axis parameter to $b$. if this parameter is
         true, the axis is drawn, otherwise not.}*/

bool get_draw_axis() const
{ return draw_axis; }
/*{\Mop  returns the draw axis parameter.}*/


void set_draw_coord_with_line(const bool& b)
{ draw_coord_with_ln = b; }
/*{\Mop  sets the draw-coord-with-line parameter to $b$. if this parameter is
         true, the coordinate system is drawn with lines, otherwise not.}*/

bool get_draw_coord_with_line() const
{ return draw_coord_with_ln; }
/*{\Mop  returns the draw-coord-with-line parameter.}*/


void set_draw_coord_with_pts(const bool& b)
{ draw_coord_with_pts = b; }
/*{\Mop  sets the draw-coord-with-points parameter to $b$. if this parameter is
         true, the coordinate system is drawn with points, otherwise not.}*/

bool get_draw_coord_with_pts() const
{ return draw_coord_with_pts; }
/*{\Mop  returns the draw-coord-with-points parameter.}*/



void set_x_grid_dist(const double& d)
{ if(d<0) x_grid_dist = -d;
  else    x_grid_dist = d;
 }
/*{\Mop  sets the x-grid-dist parameter to $d$. This parameter defines
         the x-grid distance of the coordinate system.}*/

double get_x_grid_dist() const
{ return x_grid_dist; }
/*{\Mop  returns x-grid distance.}*/


void set_y_grid_dist(const double& d)
{ if(d<0) y_grid_dist = -d;
  else    y_grid_dist = d; 
 }
/*{\Mop  sets the y-grid-dist parameter to $d$. This parameter defines
         the y-grid distance of the coordinate system.}*/

double get_y_grid_dist() const
{ return y_grid_dist; }
/*{\Mop  returns y-grid distance.}*/


void set_z_grid_dist(const double& d)
{ if(d<0) z_grid_dist = -d;
  else    z_grid_dist = d; 
 }
/*{\Mop  sets the z-grid-dist parameter to $d$. This parameter defines
         the z-grid distance of the coordinate system.}*/

double get_z_grid_dist() const
{ return z_grid_dist; }
/*{\Mop  returns z-grid distance.}*/


int get_width () const { return width; }
/*{\Mop  returns the window width.}*/

int get_height() const { return height; }
/*{\Mop  returns the window height.}*/


int get_panel_height() const;
/*{\Mop  returns the panel height if a panel window exists and -1 otherwise.}*/


gl_window* set_panel_win(window* M);
/*{\Mop  sets the panel window to $M$. if a new $gl\_window$ is created
         it is returned. if not, NULL is returned.
         \precond $M$ is not NULL.}*/

window* get_panel_win() const
{ return second_win; }
/*{\Mop  returns a pointer to the panel window.}*/


gl_window* get_gl_win() const
{ return gl_win; }
/*{\Mop  returns a pointer to the gl\_window.}*/

gl_camera* get_cam() const
{ return cam; }
/*{\Mop  returns a pointer to the camera-object.}*/


/*{\Mtext
{\bf 3.9 Miscellaneous} }*/

void center_camera() const;
/*{\Mop  places the camera in the middle of the coordinate system
         looking along the negative z-axis.}*/
         
d3_gl_scene* pick_object(const double& x, const double& y) const;
/*{\Mop  if there is an instance of the class $d3\_gl\_scene$, i.e., a
         graph drawn with OpenGL, at the position $(x,y)$ a pointer
		 to this instance is returned. otherwise the NULL pointer
         is returned. You can use this feature to select a scene via
         mouse-click.}*/
         
};


typedef GLuint* gl_uint_ptr;
int compare(const gl_uint_ptr& p1, const gl_uint_ptr& p2);


#if LEDA_ROOT_INCL_ID == 600146
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

