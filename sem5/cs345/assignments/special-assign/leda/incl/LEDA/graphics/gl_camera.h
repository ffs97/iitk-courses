#ifndef VGL_GL_CAMERA_H
#define VGL_GL_CAMERA_H

#include <LEDA/graphics/gl_basic.h>
#include <LEDA/graphics/d3_transform.h>

VGL_BEGIN_NAMESPACE

struct gl_viewpoint { 
  virtual void operator()() = 0;
  virtual ~gl_viewpoint() {}
};

class gl_camera : public gl_viewpoint {

public:
    
  gl_camera();   

  void operator()();   

  void set(double ex, double ey, double ez,
           double lx, double ly, double lz,
           double ux, double uy, double uz);  
        
  void set(const vector3d& u0, const vector3d& v0, const vector3d& n0);         

  void set_eye(double x, double y, double z);  

  template <class point>
  void set_eye(const point& e);  

  void set_eye(const vector3d& v);  
          
  void get_eye(double& x, double& y, double& z) const;  

  template <class point>
  void get_eye(point& p) const; 
  
  vector3d get_eye() const;
  vector3d get_n()   const;
  vector3d get_u()   const;
  vector3d get_v()   const; 

  void rotate(double angle, double x, double y, double z);
  void slide(double dx, double dy, double dz);    
  void pitch(double angle);
  void roll(double angle);  
  void yaw(double angle);

protected:
  
  void initialize();  

  void set_model_matrix();  
  
  vector3d eye;
  vector3d look;
  vector3d up;
  
  vector3d u;
  vector3d v;
  vector3d n;
  
  matrix44d M;
};


VGL_END_NAMESPACE

#endif
