/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  motion.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_MOTION_H
#define LEDA_MOTION_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 430256
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/list.h>
#include <LEDA/geo/line.h>

LEDA_BEGIN_NAMESPACE

//---------------------------------------------------------------------------
// base class for all kind of motions
//---------------------------------------------------------------------------

struct motion_base
{ virtual void   get_path(point,point,int,list<point>&) = 0;
  virtual double get_path_length(point p0, point p1) = 0;
  virtual vector get_step(vector,int,int) = 0;
  virtual ~motion_base() {}
};


//---------------------------------------------------------------------------
// simple linear motion
//---------------------------------------------------------------------------

struct __exportC linear_motion : public motion_base
{ void   get_path(point p0, point p1, int steps, list<point>& L);
  double get_path_length(point p0, point p1);
  vector get_step(vector v, int steps, int i);  
};


//---------------------------------------------------------------------------
// circular motion
//---------------------------------------------------------------------------

class __exportC circular_motion : public motion_base
{
  double angle;
  
  point center(point p0, point p1);
  
  public :
  
  circular_motion() : angle(0) {}
  circular_motion(double x) : angle(x) {}  
  
  double set_angle(double x) { leda_swap(angle,x); return x; }
  double get_angle() const   { return angle; }
    
  void   get_path(point p0, point p1, int steps, list<point>& L);
  double get_path_length(point p0, point p1);
  vector get_step(vector v, int steps, int i);  
};


//---------------------------------------------------------------------------
// poly motion
//---------------------------------------------------------------------------

class __exportC poly_motion : public motion_base
{
  public:
  
  enum { ABSOLUTE, RELATIVE, UNDEFINED};

  class base_coord
  {   
    protected:  

    int    Type;    
    double Coord;   
    
    public:  
    
    base_coord() : Type(UNDEFINED), Coord(0) {}  
    
    double coord() const { return Coord; }   
    int    type()  const { return Type; }
  };

  class poly_point 
  { 
    base_coord coord[2];
   
    public:

    poly_point() {}    
    poly_point(base_coord x, base_coord y) 
    { coord[0] = x; coord[1] = y; }

    double xcoord() const { return coord[0].coord(); }
    double ycoord() const { return coord[1].coord(); }
    
    int xcoord_type() const { return coord[0].type(); }
    int ycoord_type() const { return coord[1].type(); }
  };

  protected:

  list<poly_point> Path;

  void translate(point, point, list<point>&);
  
  public:
  
  void append(base_coord x, base_coord y);
  
  void clear();    
  bool empty() const;
  int  size() const;
  
  virtual void get_path(point p0, point p1, int steps, list<point>& L);
  virtual double get_path_length(point p0, point p1);
  virtual vector get_step(vector v, int steps, int i);   
  virtual ~poly_motion() {}
};

struct abs_coord : public poly_motion::base_coord {   
  abs_coord(double x) { Type = poly_motion::ABSOLUTE; Coord = x; }
};

struct rel_coord : public poly_motion::base_coord { 
  rel_coord(double x) { Type = poly_motion::RELATIVE; Coord = x; }
};


//---------------------------------------------------------------------------
// bezier motion
//---------------------------------------------------------------------------

class __exportC bezier_motion : public poly_motion
{
  point casteljau(double t, int n, double* X, double* Y); 
  
  public:
  
  void   get_path(point p0, point p1, int steps, list<point>& L);
  double get_path_length(point p0, point p1);
  vector get_step(vector v, int steps, int i);     
};


LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 430256
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif

