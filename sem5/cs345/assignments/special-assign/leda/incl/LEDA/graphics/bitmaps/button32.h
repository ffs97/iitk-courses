/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  button32.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/graphics/bitmaps/button32/a_annulus.xbm>
#include <LEDA/graphics/bitmaps/button32/w_annulus.xbm>
#include <LEDA/graphics/bitmaps/button32/circle.xbm>
#include <LEDA/graphics/bitmaps/button32/dice.xbm>
#include <LEDA/graphics/bitmaps/button32/empty_circle.xbm>
#include <LEDA/graphics/bitmaps/button32/encl_circle.xbm>
#include <LEDA/graphics/bitmaps/button32/exit.xbm>
#include <LEDA/graphics/bitmaps/button32/f_triang.xbm>
#include <LEDA/graphics/bitmaps/button32/f_voro.xbm>
#include <LEDA/graphics/bitmaps/button32/grid.xbm>
#include <LEDA/graphics/bitmaps/button32/help.xbm>
#include <LEDA/graphics/bitmaps/button32/hull.xbm>
#include <LEDA/graphics/bitmaps/button32/inside.xbm>
#include <LEDA/graphics/bitmaps/button32/intersect.xbm>
#include <LEDA/graphics/bitmaps/button32/line.xbm>
#include <LEDA/graphics/bitmaps/button32/point.xbm>
#include <LEDA/graphics/bitmaps/button32/poly.xbm>
#include <LEDA/graphics/bitmaps/button32/rect.xbm>
#include <LEDA/graphics/bitmaps/button32/tree.xbm>
#include <LEDA/graphics/bitmaps/button32/triang.xbm>
#include <LEDA/graphics/bitmaps/button32/triangle.xbm>
#include <LEDA/graphics/bitmaps/button32/union.xbm>
#include <LEDA/graphics/bitmaps/button32/voro.xbm>

#define num_xbm_button32 23

static unsigned char* xbm_button32[] = {
a_annulus_bits,
w_annulus_bits,
circle_bits,
dice_bits,
empty_circle_bits,
encl_circle_bits,
exit_bits,
f_triang_bits,
f_voro_bits,
grid_bits,
help_bits,
hull_bits,
inside_bits,
intersect_bits,
line_bits,
point_bits,
poly_bits,
rect_bits,
tree_bits,
triang_bits,
triangle_bits,
union_bits,
voro_bits
};


static const char* name_xbm_button32[] = {
"a_annulus",
"w_annulus",
"circle",
"dice",
"empty_circle",
"encl_circle",
"exit",
"f_triang",
"f_voro",
"grid",
"help",
"hull",
"inside",
"intersect",
"line",
"point",
"poly",
"rect",
"tree",
"triang",
"triangle",
"union",
"voro"
};


#if defined(__GNUC__)
inline char xbm_button32_unused_warning()
{ return xbm_button32[0][0] + name_xbm_button32[0][0]; }
#endif
