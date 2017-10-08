/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  polygis.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/geo/rat_kernel.h>

LEDA_BEGIN_NAMESPACE

typedef rat_point POINT;
typedef rat_segment SEGMENT;
typedef rat_polygon POLYGON;
typedef rat_gen_polygon GPOLYGON;


class __exportC polygis_polygon  
{

 enum { UNKNOWN_OBJ = 0, POLYGON_OBJ = 1, LINE_OBJ = 2 };  


 list<POINT> plist;
 GPOLYGON poly;

 int constructed;

 int id_num;

public:

 polygis_polygon() { constructed = UNKNOWN_OBJ; }
 polygis_polygon(const GPOLYGON&);
 polygis_polygon(const list<POINT>&);
 
 void set_id(int i) { id_num = i; }
 int  get_id()      { return id_num; }

 void clear() { plist.clear(); }
 int  size()  { return plist.size(); }

 const GPOLYGON&    get_poly();
 const list<POINT>& get_polyline();

 void add_point(double x, double y) { plist.append(POINT(point(x,y))); }

 void construct_polygon();
 void construct_polyline();
 
 void write_points(double*,double*);

};

LEDA_END_NAMESPACE


