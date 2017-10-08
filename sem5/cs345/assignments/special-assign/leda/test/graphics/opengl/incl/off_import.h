#include <LEDA/string.h>
#include <LEDA/array.h>
#include <LEDA/list.h>
#include <LEDA/d3_point.h>
#include <LEDA/stream.h>
#include <fstream>


// -----------------------------------------------------------
// we return the bounding box, the vertices and 
// the face description in the parameters ...
// -----------------------------------------------------------

LEDA_BEGIN_NAMESPACE

#if defined(LEDA_STD_IO_HEADERS)
using std::ifstream;
#endif


bool read_off_file(ifstream& I, 
                   double& xmin, double& xmax,
		   double& ymin, double& ymax,
		   double& zmin, double& zmax,
		   leda::array<leda::d3_point>& vertices,
		   leda::list<leda::list<int> >& face_list)
{         
 char* line = new char[1000];
 leda::string act;
 
 int mode = 1;
 
 // init values ...
 face_list.clear();
 
 const int off_keyword_mode = 1;
 const int off_numbers_mode = 2;
 const int off_vertex_mode  = 3;
 const int off_face_mode    = 4;
 
 int number_of_faces, number_of_vertices, number_of_edges;
 int v_count =0, f_count=0;
 
 leda::string offst("OFF");
 
 while (! I.eof()){
   // get a line ...
   I.getline(line,1000); 
   
   if (line[0] != '#'){
     act = leda::string(line);
     
     if (act.length() > 0){
       switch (mode){
         case off_keyword_mode: {  // read file header OFF
	   if (act == offst) mode++;
	   break;
	 }
	 case off_numbers_mode: { // read number of faces/vertices/edges
	   leda::string_istream IS(act);
	 
	   //# vertices ...
	   IS >> number_of_vertices;
	   
	   //# faces ...
	   IS >> number_of_faces;
	  
	   //# edges ...
	   IS >> number_of_edges;
	   
	   vertices.resize(number_of_vertices);  
	   
	   mode++;
	   break;
	 }
	 case off_vertex_mode: { // read x/y/z double values ...
	   leda::string_istream IS(act);
	   double xv, yv, zv;
	   
	   IS >> xv;
	   IS >> yv;
	   IS >> zv;
	   
	   vertices[v_count] = leda::d3_point(xv,yv,zv);
	   
	   // init bbox ...
	   if (v_count == 0) { 
	     xmin = xmax = xv;
	     ymin = ymax = yv;
	     zmin = zmax = zv;
	   }
	   else { // ... check old bbox values 
	     if (xv < xmin) xmin = xv;
	     if (xv > xmax) xmax = xv;
	     if (yv < ymin) ymin = yv;
	     if (yv > ymax) ymax = yv;
	     if (zv < zmin) zmin = zv;
	     if (zv > zmax) zmax = zv;	     	     
	   }
	   
	   v_count++;
	   
	   if (v_count == number_of_vertices) mode++; // go into face mode ...
	   
	   break;
	 }
	 case off_face_mode: { 
	   // read faces (first comes #vertices for a face, then the list of vertex indices
	   leda::string_istream IS(act);
	   
	   int vertex_per_face, i, index;
	   
	   IS >> vertex_per_face;
	   
	   leda::list<int> act_face;
	   
	   for(i=0;i<vertex_per_face;i++){
	     IS >> index;
	     act_face.push_back(index);
	   }
	   
	   // append new face ...
	   face_list.push_back(act_face);
	   
	   f_count++;
	   
	   break;
	 }
       }
     }
   }
   else {
   }
 }

 delete [] line;

 I.close();

 return true;
}

LEDA_END_NAMESPACE

// -----------------------------------------------------------
