/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  ps_file.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#include <LEDA/graphics/ps_file.h>

#include <LEDA/geo/point.h>
#include <LEDA/geo/segment.h>
#include <LEDA/geo/line.h>
#include <LEDA/geo/circle.h>
#include <LEDA/geo/polygon.h>
#include <LEDA/core/list.h>
#include <LEDA/graphics/color.h>
#include <LEDA/system/stream.h>

using namespace leda;

int main() {

  // file 'demo1.ps': 10cm x 10cm
  ps_file file(10.0,10.0,"demo1.ps");
  file.init(1000,2000,1000);

  // Page 1: points, lines, v_lines, h_lines
  file.draw_text(1100,1900,"1. POINT, LINE,");
  file.draw_text(1165,1840,"VLINE, HLINE");
  point p1(1250,1600);
  file << p1;
  file.draw_line(1300,1300,1850,1950);
  file.draw_hline(1300);
  file.draw_vline(1800);

  // Page 2: segments
  file.clear();
  file.draw_text(1100,1900,"2. SEGMENTS");
  segment s1(1150,1350,1850,1700);
  segment s2(1100,1100,1800,1300);
  file << s1 << s2;
  file.draw_segment(1250,1700,1950,1400);

  // Page 3: circles, ellipses
  file.clear();
  file.draw_text(1100,1900,"3. CIRCLES, ELLIPSES");
  circle C1(1300,1500,50);
  file << C1;
  file.draw_ellipse(1300,1200,70,30,violet);
  file.draw_disc(1700,1400,100,brown);
  file.draw_filled_ellipse(1700,1700,70,110,cyan);

  // Page 4: triangles, rectangles, polygons
  file.clear();
  file.draw_text(1100,1900,"4. TRIANGLES, RECTANGLES,");
  file.draw_text(1510,1840,"POLYGONS");
  point p2(1100,1700), p3(1200,1800), p4(1400,1820),
        p5(1350,1600), p6(1200,1450);
  list<point> lp;
  lp.append(p2);lp.append(p3);lp.append(p4);
  lp.append(p5);lp.append(p6);
  polygon P1(lp);
  file << P1;
  point a1(1100,1200), b1(1250,1400), c1(1400,1050);
  file.draw_triangle(a1,b1,c1,blue);
  file.draw_rectangle(1650,1600,1950,1350,blue);
  point a2(1600,1050), b2(1750,1300), c2(1900,1050);
  file.draw_filled_triangle(a2,b2,c2,blue);

  // Page 5: arrows, edges, illegal functions
  file.clear();
  file.set_draw_bb(false);
  file.draw_text(1100,1900,"5. ARROWS, EDGES");
  file.plot_xy(100,100,win_draw_func(0));
  file.draw_arrow(1100,1600,1750,1700);
  file.draw_disc(1200,1200,3);
  file.draw_edge(1200,1200,1600,1500);
  file.draw_edge_arrow(1900,1100,1600,1500);

  // Page 6: nodes
  file.clear();
  file.draw_text(1100,1900,"6. NODES");
  file.draw_node(1300,1600);
  file.draw_text_node(1300,1300,"N",white);
  file.draw_int_node(1700,1600,3,green);
  file.draw_filled_node(1700,1300);

  // Calling file.close() is optional:
// file.close();

  // Start a new file:
  // file 'demo2.ps': 15cm x 15cm
  file.newfile(15.0,15.0,"demo2.ps");
  file.init(1000,2000,1000);

  // Page 1: texts and fonts, illegal functions
  file.draw_text(1100,1900,"1. TEXTS & FONTS");
  file.draw_arc(1000,1000,2000,2000,100);
  file.set_text_font("Helvetica-Bold");
  file.set_cross_width(1);
  file.draw_point(1500,1800);
  file.draw_ctext(1500,1800,"centered");
  file.draw_point(1500,1700);
  file.draw_text(1500,1700,"left-justified");
  file.set_text_font("Helvetica-BoldOblique");
  file.set_font_size(0.5);
  file.draw_ctext(1500,1600,"Helvetica-BoldOblique");
  file.set_text_font("Times-Bold");
  file.set_font_size(1);
  file.draw_ctext(1500,1500,"Times-Bold");
  file.set_text_font("Courier");
  file.set_font_size(1.5);
  file.draw_ctext(1500,1350,"Courier");
  file.set_text_font("Symbol");
  file.set_font_size(2);
  file.draw_ctext(1500,1150,"Symbol");
  file.set_text_font("No-Font");
  file.set_cross_width(0.2);
  file.set_font_size(0.4);

  // Page 2: linewidth, unitlinewidth, linestyle, crosswidth
  file.clear();
  file.draw_text(1100,1900,"2. LINEWIDTH, LINESTYLE, CROSSWIDTH");
  file.draw_hline(1800);
  file.set_line_width(10);
  file.draw_hline(1730);
  file.set_line_style(dashed);
  file.set_unit_line_width(0.5); // =0.5cm
  file.set_line_width(2);        // =1cm
  file.draw_hline(1600);
  file.set_line_style(dotted);
  file.draw_hline(1450);
  file.set_line_width(1);
  file.set_unit_line_width(PIXEL);
  file.set_line_style(solid);
  file.draw_point(1250,1100);
  file.set_cross_width(1);
  file.draw_point(1500,1100);
  file.set_cross_width(2);
  file.draw_point(1750,1100);
  file.set_cross_width(0.2);

  // Page 3: foreground color, optional color argument
  file.clear();
  file.draw_text(1100,1900,"3. FOREGROUND COLOR");
  file.set_color(grey2);
  file.draw_box(1300,1800,1700,1700);
  file.draw_disc(1250,1450,75,red);
  point a(1450,1400), b(1500,1550), c(1550,1400);
  file.draw_filled_triangle(a,b,c,green);
  file.draw_box(1700,1550,1800,1400,blue);
  file.draw_box(1300,1200,1700,1100);
  file.set_color(black);

  // Page 4: output modes
  file.clear();
  file.draw_text(1100,1900,"4. OUTPUT MODES");
  file.draw_disc(1150,1500,100,green);
  file.draw_ctext(1150,1500,"GREEN",white);
  file.set_output_mode(gray_mode);
  file.draw_disc(1350,1500,100,green);
  file.draw_ctext(1350,1500,"GREEN",white);
  file.set_output_mode(colored_mode);
  file.draw_disc(1650,1500,100,red);
  file.draw_ctext(1650,1500,"RED",white);
  file.set_output_mode(gray_mode);
  file.draw_disc(1850,1500,100,red);
  file.draw_ctext(1850,1500,"RED",white);

  // Page 5: gray values
  file.clear();
  file.draw_text(1500,1900,"5. GRAY VALUES");
  file.set_output_mode(gray_mode);
  file.draw_box(1100,1800,1200,1700,blue);
  file.set_gray_value(blue,0.3);
  file.draw_box(1300,1600,1400,1500,blue);
  file.set_gray_value(blue,0.6);
  file.draw_box(1500,1400,1600,1300,blue);
  file.set_gray_value(blue,0.9);
  file.draw_box(1700,1200,1800,1100,blue);
  file.set_output_mode(colored_mode);

  // Page 6: init
  file.clear();
  file.set_draw_bb(false);
  file.draw_text(1100,1900,"6. INIT OPERATION");
  file.draw_vline(1500);
  file.set_line_style(dashed);
  file.draw_segment(1100,1100,1400,1800);
  file.draw_segment(1100,1800,1400,1100);
  file.init(100,200,100);
  file.set_line_style(solid);
  file.draw_segment(160,110,190,180);
  file.draw_segment(160,180,190,110);

  // Calling file.close() is optional:
  file.close(); 

  return 0;
}
