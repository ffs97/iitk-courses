/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  gl_hull.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


// 3d convex hull demo using OpenGL ...

#include <LEDA/graphics/gl_window.h>
#include <LEDA/geo/d3_hull.h>

#include "gl_support.h"

#if defined(LEDA_NAMESPACE)
using namespace leda;
#endif

// camera position ...
double  xpos = 0.0;
double  ypos = 0.0;
double  zpos = -900.0;

// scaling factor ...
double  scale = 1.0;

// what kind of projection ??
bool    ortho = true;
double  aspect=1.0, angle=90.0;

// we can switch on/off two lights in this application ...
bool    use_light0 = true;
bool    use_light1 = true;
// are the two lights directional or positional ??
bool    positional0 = true;
bool    positional1 = true;
// ambient part in the lights ??
bool    ambient0 = true;
bool    ambient1 = true;

// we need a global pointer to force redraw ...
gl_window* wptr;

int input_kind = 1;

bool hull_mode = false;    // false: show points ; true: show computed hull

list<d3_rat_point>         input;
GRAPH<d3_rat_point,int>    hull;

d3_point                   inside_hull;                // a point in the convex hull polyhedron
bool                       exists_inside_hull = false; // does a point inside the hull polyhedron exist
bool                       draw_solid = true;          // solid or wireframe drawing
bool                       light_usage = true;         // light usage or not 
list<list<d3_point> >      polys;                      // list of polygons in 3d (describes surface of 3d hull polyhedron)


// callback ...
void callback(int)
{ wptr->redraw(); }

// compute a point inside a convex polyhedron ...

template<class POINT>
bool compute_point_in_convex_polyhedron(const GRAPH<POINT,int>& G, POINT& ins)
{
  // find four non-coplanar vertices ...
  // then compute a point inside the tetrahedron 
  // defined by these points ...
  
  POINT a,b,c,d;
  
  if (G.number_of_nodes() < 4) return false;
  
  const list<node>& vert = G.all_nodes();
  node v;
  int state = 0;
  
  forall(v,vert){
    switch(state){
      case 0: {
        a = G[v];
	state++;
        break;
      }
      case 1: {
        if (a != G[v]){
	  b = G[v];
	  state++;
	}
        break;
      }
      case 2: {
        // 3 points must be non-collinear ...
	if (! collinear(a,b,G[v])){
	  c = G[v];
	  state++;
	}
	break;
      }
      case 3: {
        // 4 points must be non-coplanar ...
	if (! coplanar(a,b,c,G[v])){
	  d = G[v];
	  state++;
	}	
	break;
      }
    }
    
    if (state == 4) break;
  }
  
  if (state == 4) { // we found 4 non-coplanar vertices ...
    ins = midpoint(d, midpoint(c,midpoint(a,b)));
  
    return true;
  }

  return false;
}

// -------------------------------------------------------------------
// drawing functions for hull and input points ...
// -------------------------------------------------------------------

void draw_points(const list<d3_rat_point>& L)
{
  d3_rat_point pi;

  glPointSize(3.0f);
  glBegin(GL_POINTS);
     forall(pi,L) leda::gl_vertex(pi.to_float());
  glEnd();
}

// G         : surface graph
// use_light : use light (provide normals) or not (provide color information) 

void draw_hull(const GRAPH<d3_rat_point,int>& G, bool use_light = true, bool solid_redraw = true)
{
  d3_rat_point ps,pt;
  edge e;
  
  if (! exists_inside_hull || ! solid_redraw){ // render wire frame ...

    glLineWidth(3.0f);
    glBegin(GL_LINES);
  
    forall_edges(e,G) {
     ps = G[source(e)];
     pt = G[target(e)];
     leda::gl_vertex(ps.to_float());
     leda::gl_vertex(pt.to_float());
    } 
    glEnd();
  }
  else { // render solid ...
    list_item it;
    
    forall_items(it, polys){
      const list<d3_point>& Li = polys[it];
      d3_point pi;
      d3_point a,b,c;
      
      if (Li.size() > 2) {
        a = Li[Li.get_item(0)];
	b = Li[Li.get_item(1)];
	c = Li[Li.get_item(2)];

        GLfloat rc=(GLfloat)0.2, gc=(GLfloat)0.0, bc=(GLfloat)0.3;
      
        glBegin(GL_POLYGON);
         // compute normal vector ...
         if (use_light) gl_normal(a,b,c, inside_hull);
      
         forall(pi,Li){
           if (! use_light){ // provide color information
             glColor3f(rc,gc,bc);
             if (gc <= (GLfloat)0.7)
               gc = gc + (GLfloat)0.3;
           }
	   gl_vertex(pi);
         }
        glEnd();
      }
    }
  }
}


void redraw(gl_window* win)
{
  win->gl_start();
  
  // show computed convex hull ...
  if (hull_mode) {
  
    if (light_usage) {
     glEnable(GL_LIGHTING);
     if (use_light0) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
     if (use_light1) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);
    }
    else {
     glDisable(GL_LIGHTING);
     glDisable(GL_LIGHT0);
     glDisable(GL_LIGHT1);
    }
	
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);	
	
    glClearColor(0.0f, 0.0f, 0.0f,0.0f);
    glViewport(0, 0, 750, 550);

    GLfloat mat_ambient[] = { 0.0f , 0.9f, 0.7f ,1.0f};
    GLfloat mat_diffuse[] = { 0.4f , 0.9f, 0.6f ,1.0f};
    GLfloat mat_specular[] = { 0.5f , 0.9f, 0.9f ,1.0f};
    GLfloat mat_shin[] = { 100.0f };	
	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shin);

    GLfloat pos1, pos2;

    if (positional0) pos1 = 1.0f; else pos1 = 0.0f;
    if (positional1) pos2 = 1.0f; else pos2 = 0.0f;
		
    GLfloat light_intensity1[] = { 0.9f , 0.9f, 0.9f ,1.0f};
    GLfloat light_intensity2[] = { 0.5f , 0.5f, 0.9f ,1.0f}; 
    GLfloat light_amb_intensity1[] = { 0.2f , 0.1f, 0.1f ,1.0f};
    GLfloat light_amb_intensity2[] = { 0.2f , 0.2f, 0.1f ,1.0f};       
    GLfloat light_position1[] = { 0.0f , 0.0f, -2200.0f , pos1};
    GLfloat light_position2[] = { 0.0f , 0.0f, 2200.0f , pos2};
    GLfloat no_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f };
	
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_intensity1);	

    if (ambient0) glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb_intensity1);
    else glLightfv(GL_LIGHT0, GL_AMBIENT, no_ambient );	
 
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_intensity2); 

    if (ambient1) glLightfv(GL_LIGHT1, GL_AMBIENT, light_amb_intensity2);
    else glLightfv(GL_LIGHT1, GL_AMBIENT, no_ambient );	   						

    glMatrixMode(GL_PROJECTION); // set the view volume shape
    glLoadIdentity();

    // init projection matrix ...

    if (ortho) glOrtho(-1500.0, 1500.0, -1500.0, 1500.0, 1, 8000);
    else {  // perspective projection ...
      gluPerspective((GLdouble) angle, (GLdouble) aspect, 1, 8000);
    }

    glMatrixMode(GL_MODELVIEW); // position and aim the camera
    glLoadIdentity();
	
    // set camera		
    gluLookAt(xpos, ypos, zpos, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen

    glScaled(scale,scale,scale);

    // output convex hull ...
    draw_hull(hull, light_usage, draw_solid);

    glPopMatrix();
    glFlush();
  }
  else { // draw only the points
    glShadeModel(GL_FLAT);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
	
    glClearColor(0.0f, 0.0f, 0.0f,0.0f);
    glViewport(0, 0, 750, 550);						

    glMatrixMode(GL_PROJECTION); // set the view volume shape
    glLoadIdentity();
    glOrtho(-1500.0, 1500.0, -1500.0, 1500.0, 1, 8000);
    glMatrixMode(GL_MODELVIEW); // position and aim the camera
    glLoadIdentity();
	
    // set camera		
    gluLookAt(xpos, ypos, zpos, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen

    glColor3f(1.0f, 0.0f, 0.0f);
    
    glScaled(scale,scale,scale);
    
    draw_points(input);

    glPopMatrix();
    glFlush();  
  }  	
  win->gl_finish();	
}

int main()
{
  gl_window W(750,550,"OpenGL 3d Convex hull demo");
  wptr = &W;

  W.init(0,750,0);
  window ADD(300,200);  

  int number = 500;

  list<string> choices;
  choices.append("cube");
  choices.append("ball");
  choices.append("square");
  choices.append("para");
  choices.append("mesh");
  choices.append("sphere");
  choices.append("line");

  window M(W.width(),28);  
  M.button("Hull", 10, "compute convex hull of current points");
  M.button("Gen",11, "generate points");  

  int b_left   = M.button("Left",2);
  M.set_button_repeat(b_left,150);

  int b_right  = M.button("Right",3);
  M.set_button_repeat(b_right,150);

  int b_up     = M.button("Up",4);
  M.set_button_repeat(b_up,150);

  int b_down   = M.button("Down",5);  
  M.set_button_repeat(b_down,150);

  int b_zplus  = M.button("Z +",6);
  M.set_button_repeat(b_zplus,150);

  int b_zminus = M.button("Z -",7);   
  M.set_button_repeat(b_zminus,150);

  int b_in     = M.button("In",8);
  M.set_button_repeat(b_in,150);

  int b_out    = M.button("Out",9);      
  M.set_button_repeat(b_out,150);

  M.choice_item("input",input_kind,choices);
  M.bool_item("Draw solid", draw_solid);
  M.bool_item("Light", light_usage);
  M.int_item("number ",number,20,2000);

  M.button("Exit",1); 

  // add some control elements to add ...
  ADD.bool_item("Ortho Projection", ortho, callback);
  ADD.text_item("Perspective projection:");
  ADD.double_item("Angle:",angle);
  ADD.double_item("Aspect:",aspect);
  ADD.text_item("Light 0:");
  ADD.bool_item("Use it", use_light0, callback);
  ADD.bool_item("Positional", positional0, callback);
  ADD.bool_item("Ambient part", ambient0, callback);
  ADD.text_item("Light 1:");  
  ADD.bool_item("Use it", use_light1, callback);
  ADD.bool_item("Positional", positional1, callback);
  ADD.bool_item("Ambient part", ambient1, callback);
  
  W.set_redraw(redraw);  
  W.display(0,window::center);
  M.display(0,0,W);
  ADD.display();
 
  int maxc   = 900;
		
  W.redraw();
  while (1)
  { switch (M.read_mouse())
    { case 1 : return 0;
      case 2 : xpos += 80; break;
      case 3 : xpos -= 80; break;
      case 4 : ypos += 80; break;
      case 5 : ypos -= 80; break; 
      case 6 : zpos += 80; break;
      case 7 : zpos -= 80; break;
      case 8 : scale += 0.1; break;
      case 9 : scale -= 0.1; break;                   
      case 10 : {  // compute convex hull ...
       hull.clear();
       D3_HULL(input, hull);
       hull.compute_faces();
       
       // try to compute a point inside the
       // hull ...
       
       d3_rat_point ins;
       exists_inside_hull = compute_point_in_convex_polyhedron(hull, ins);
       inside_hull = ins.to_float();
       
       face f;
       polys.clear();
       list<d3_point> Lp;
       edge eiter;

       forall_faces(f,hull) {
         list<edge> LE = hull.adj_edges(f);
	 forall(eiter,LE) Lp.append(hull[source(eiter)].to_float());
	
         // create a 3d polygon from the points in Lp ...
	 polys.push_back(Lp);
   
	 Lp.clear();
	}
       hull_mode = true;
       break;         
      }
      case 11 : { // generate input ...
       input.clear();

       // switch ...
       switch (input_kind) {

          case 0: random_points_in_cube(number,3*maxc/4,input);
                  break;

          case 1: random_points_in_ball(number,maxc,input);
                  break;

          case 2: random_points_in_square(number,maxc,input);
                  break;

          case 3: random_points_on_paraboloid(number,maxc,input);
                  break;

          case 4: lattice_points(number,3*maxc/4,input);
                  break;

          case 5: random_points_on_sphere(number,maxc,input);
                  break;

          case 6: random_points_on_segment(number,maxc,input);
                  break;

        }

       hull_mode = false;
       break;
      }
    };
    W.redraw();
  }
  
  return 0;
}
