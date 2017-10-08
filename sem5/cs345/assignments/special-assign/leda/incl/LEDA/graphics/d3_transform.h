/* 
  last modified: 26.06.2003   
*/

#ifndef VGL_D3_TRANSFORM_H
#define VGL_D3_TRANSFORM_H

//#include <algorithm>

#include <LEDA/graphics/gl_basic.h>
#include <LEDA/graphics/gl_smatrix.h>

#ifdef LEDA_D3_TRANSFORM_SUPPORT
#include <LEDA/geo/d3_point.h>
#include <LEDA/geo/d3_ray.h>
#include <LEDA/geo/d3_segment.h>
#include <LEDA/geo/d3_line.h>
#include <LEDA/geo/d3_plane.h>
#include <LEDA/geo/d3_triangle.h>
#include <LEDA/geo/d3_sphere.h>
#include <LEDA/geo/d3_simplex.h>
#endif

VGL_BEGIN_NAMESPACE

//---------------------------------------------------------------------------
// workaround 
//---------------------------------------------------------------------------

namespace vgl_detail {

// #ifdef MSC_VER_6

inline double sin(double x) { return ::sin(x); }
inline double cos(double x) { return ::cos(x); }


// #else

// inline double sin(double x) { return /*std::*/sin(x); }
// inline double cos(double x) { return /*std::*/cos(x); }

// #endif

}

// ------------------------------------------------------------------------------------
// helper function
// ------------------------------------------------------------------------------------

inline double deg_to_rad(double angle) { 
  return angle * 3.14159265358979323846 / 180.0; 
}

inline double rad_to_deg(double angle) { 
  return angle * 180.0 / 3.14159265358979323846; 
}

// ------------------------------------------------------------------------------------
// d3_base_transform
// ------------------------------------------------------------------------------------

template <typename T>
class d3_base_transform {

public:

  typedef smatrix<4,4,T> matrix; 
  typedef double         value_type;

  explicit d3_base_transform() { identity(); }

  explicit d3_base_transform(const matrix& m) : M(m) {}
   
  void identity()
  { M = 1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;  
  }

  void translate(const T& x, const T& y, const T& z)
  { M = 1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1;      
  }
   
  void scale(const T& x, const T& y, const T& z)
  { M = x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1;         
  }
   
  void reflect(const T& x0, const T& y0, const T& z0,
               const T& x1, const T& y1, const T& z1,
               const T& x2, const T& y2, const T& z2)
  { svector<3,T> v(x1 - x0, y1 - y0, z1 - z0);
    svector<3,T> w(x2 - x0, y2 - y0, z2 - z0);
    svector<3,T> n = norm(cross_product(v,w));
    
    T A = n[0], B = n[1], C = n[2], D = -A * x0 - B * y0 - C * z0;
 
    M = 1 - 2 * A * A,    -2 * A * B,   - 2 * A * C, - 2 * A * D,
          - 2 * A * B, 1 - 2 * B * B,   - 2 * B * C, - 2 * B * D,
          - 2 * A * C,   - 2 * C * B, 1 - 2 * C * C, - 2 * C * D,
                    0,             0,             0,           1;    
  }         

  void project(const T& x0, const T& y0, const T& z0,
               const T& x1, const T& y1, const T& z1,
               const T& x2, const T& y2, const T& z2,
               const T& sx, const T& sy, const T& sz, const T& sw = 1.0)
  { svector<3,T> v(x1 - x0, y1 - y0, z1 - z0);
    svector<3,T> w(x2 - x0, y2 - y0, z2 - z0);
    svector<3,T> n = norm(cross_product(v,w));

    T A = n[0], B = n[1], C = n[2], D = -A * x0 - B * y0 - C * z0;
    T d = A * sx + B * sy + C * sz + D * sw;
    
    M = d - A * sx,   - B * sx,   - C * sx,    -D * sx,
            A * sy, d - B * sy,   - C * sy,    -D * sy,
            A * sz,   - B * sz, d - C * sz,    -D * sz,
            A * sw,   - B * sw,   - C * sw, d - D * sw;
  }

  matrix set_matrix(matrix m) 
  { std::swap(m,M); 
    return M; 
  }

  matrix get_matrix() const { return M; }

  operator matrix() { return M; }
  
protected:

  matrix M;              
};

//-------------------------------------------------------------------------------------------------
// d3 double transform
//-------------------------------------------------------------------------------------------------

struct d3_transform : public d3_base_transform<double> {

  void rotate(double angle, double px, double py, double pz, 
                            double x,  double y,  double z)
  { double c = vgl_detail::cos(angle), c1 = 1 - c;
    double s = vgl_detail::sin(angle);    
   
    M = c + c1 * x * x,     c1 * y * x - s * z, c1 * z * x + s * y, 0,
        c1 * x * y + s * z, c + c1 * y * y,     c1 * z * y - s * x, 0,
        c1 * x * z - s * y, c1 * y * z + s * x, c + c1 * z * z,     0,
        0,                  0,                  0,                  1;
  
    M(0,3) = px - px * M(0,0) - py * M(0,1) - pz * M(0,2);     
    M(1,3) = py - px * M(1,0) - py * M(1,1) - pz * M(1,2);
    M(2,3) = pz - px * M(2,0) - py * M(2,1) - pz * M(2,2);
  }
    
  void rotate(double angle, double x, double y, double z)
  { double c = vgl_detail::cos(angle), c1 = 1 - c;
    double s = vgl_detail::sin(angle);    
    M = c + c1 * x * x,     c1 * y * x - s * z, c1 * z * x + s * y, 0,
        c1 * x * y + s * z, c + c1 * y * y,     c1 * z * y - s * x, 0,
        c1 * x * z - s * y, c1 * y * z + s * x, c + c1 * z * z,     0,
        0,                  0,                  0,                  1;    
  }

  void rotate_x(double angle)
  { double c = vgl_detail::cos(angle);
    double s = vgl_detail::sin(angle);
    M = 1, 0, 0, 0,
        0, c,-s, 0,
        0, s, c, 0,
        0, 0, 0, 1;
  }
      
  void rotate_y(double angle)
  { double c = vgl_detail::cos(angle);
    double s = vgl_detail::sin(angle);

    M = c, 0, s, 0,
        0, 1, 0, 0,
       -s, 0, c, 0,
        0, 0, 0, 1;
  }
   
  void rotate_z(double angle)
  { double c = vgl_detail::cos(angle);
    double s = vgl_detail::sin(angle);
    M = c,-s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;
  }  
  
  vector3d operator*(const vector3d& V) 
  { vector4d r = M * vector4d(V[0],V[1],V[2],1);
    return vector3d(r[0],r[1],r[2]);
  }  
  
  vector3d operator()(const vector3d& V) { return *this * V; }

#ifdef LEDA_D3_TRANSFORM_SUPPORT
  leda::vector operator()(const leda::vector& V)
  { vector4d r = M * vector4d(V[0],V[1],V[2],1);
    return leda::vector(r[0],r[1],r[2]);
  }
 
  leda::d3_point operator()(const leda::d3_point& p)
  { vector4d r = M * vector4d(p.xcoord(),p.ycoord(),p.zcoord(),1);    
    return leda::d3_point(r[0],r[1],r[2]);
  }

  leda::d3_segment operator()(const leda::d3_segment& s) 
  { d3_transform& T = *this;   
    return leda::d3_segment(T(s.source()),T(s.target())); 
  }

  leda::d3_line operator()(const leda::d3_line& l) 
  { d3_transform& T = *this;   
    return leda::d3_line(T(l.point1()),T(l.point2())); 
  }

  leda::d3_ray operator()(const leda::d3_ray& r) 
  { d3_transform& T = *this;
    return leda::d3_ray(T(r.point1()),T(r.point2()));    
  }

  leda::d3_plane operator()(const leda::d3_plane& p) 
  { d3_transform& T = *this;
    return leda::d3_plane(T(p.point1()),T(p.point2()),T(p.point3())); 
  }

  leda::d3_triangle operator()(const leda::d3_triangle& t) 
  { d3_transform& T = *this;  
    return leda::d3_triangle(T(t.point1()),T(t.point2()),T(t.point3())); 
  }

  leda::d3_sphere apply(const leda::d3_sphere& s) 
  { d3_transform& T = *this;  
    return leda::d3_sphere(T(s.point1()),T(s.point2()),T(s.point3()),T(s.point4())); 
  }

  leda::d3_simplex apply(const leda::d3_simplex& s) 
  { d3_transform& T = *this;
    return leda::d3_simplex(T(s.point1()),T(s.point2()),T(s.point3()),T(s.point4())); 
  }
#endif

};


//-------------------------------------------------------------------------------------------------
// d3 float transform
//-------------------------------------------------------------------------------------------------

struct d3_transform_f : public d3_base_transform<float> {
      
  void rotate(float angle, float px, float py, float pz, 
                           float x,  float y,  float z)
  { float c = float(vgl_detail::cos(angle)), c1 = 1 - c;
    float s = float(vgl_detail::sin(angle));    
   
    M = c + c1 * x * x,     c1 * y * x - s * z, c1 * z * x + s * y, 0,
        c1 * x * y + s * z, c + c1 * y * y,     c1 * z * y - s * x, 0,
        c1 * x * z - s * y, c1 * y * z + s * x, c + c1 * z * z,     0,
        0,                  0,                  0,                  1;
  
    M(0,3) = px - px * M(0,0) - py * M(0,1) - pz * M(0,2);     
    M(1,3) = py - px * M(1,0) - py * M(1,1) - pz * M(1,2);
    M(2,3) = pz - px * M(2,0) - py * M(2,1) - pz * M(2,2);
  }
    
  void rotate(float angle, float x, float y, float z)
  { float c = float(vgl_detail::cos(angle)), c1 = 1 - c;
    float s = float(vgl_detail::sin(angle));    
    M = c + c1 * x * x,     c1 * y * x - s * z, c1 * z * x + s * y, 0,
        c1 * x * y + s * z, c + c1 * y * y,     c1 * z * y - s * x, 0,
        c1 * x * z - s * y, c1 * y * z + s * x, c + c1 * z * z,     0,
        0,                  0,                  0,                  1;    
  }

  void rotate_x(float angle)
  { float c = float(vgl_detail::cos(angle));
    float s = float(vgl_detail::sin(angle));
    M = 1, 0, 0, 0,
        0, c,-s, 0,
        0, s, c, 0,
        0, 0, 0, 1;
  }
      
  void rotate_y(float angle)
  { float c = float(vgl_detail::cos(angle));
    float s = float(vgl_detail::sin(angle));
    M = c, 0, s, 0,
        0, 1, 0, 0,
       -s, 0, c, 0,
        0, 0, 0, 1;
  }
   
  void rotate_z(float angle)
  { float c = float(vgl_detail::cos(angle));
    float s = float(vgl_detail::sin(angle));
    M = c,-s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1;
  } 
  
  vector3f operator*(const vector3f& V) 
  { vector4f r = M * vector4f(V[0],V[1],V[2],1);
    return vector3f(r[0],r[1],r[2]);
  }  
  
  vector3f operator()(const vector3f& V) { return *this * V; }   
};

VGL_END_NAMESPACE

#endif
