#ifndef LEDA_D3_GL_SUPPORT_H
#define LEDA_D3_GL_SUPPORT_H

#include <LEDA/numbers/vector.h>
#include <LEDA/core/tuple.h>


LEDA_BEGIN_NAMESPACE

// note : it is not tested if the vector has a length/dimension/size of 3

inline void gl_vertex(const vector& v)
{ glVertex3d(v[0],v[1],v[2]); }

inline void gl_vertex(const vector& v1, const vector& v2)
{ glVertex3d(v1[0],v1[1],v1[2]);
  glVertex3d(v2[0],v2[1],v2[2]);
}

inline void gl_vertex(const vector& v1, const vector& v2, const vector& v3)
{ glVertex3d(v1[0],v1[1],v1[2]);
  glVertex3d(v2[0],v2[1],v2[2]);
  glVertex3d(v3[0],v3[1],v3[2]);
}


typedef four_tuple<char*,int,int,int> tex_info;


LEDA_END_NAMESPACE

#endif
