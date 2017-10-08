      #ifndef LEDA_D3_GL_SCENE_H
      #define LEDA_D3_GL_SCENE_H

      #if !defined(LEDA_ROOT_INCL_ID)
      #define LEDA_ROOT_INCL_ID 600144
      #include <LEDA/internal/PREAMBLE.h>
      #endif

      #include <LEDA/graph/graph.h>
      #include <LEDA/numbers/rat_vector.h>
      #include <LEDA/core/tuple.h>
      #include <LEDA/graphics/gl_window.h>
      #include <LEDA/graphics/color.h>
      #include <LEDA/core/sortseq.h>
      #include <LEDA/core/set.h>
      #include <LEDA/graph/face_array.h>
      #include <cmath> 

      #include <LEDA/graphics/gl_window.h>   
      #include <LEDA/graphics/d3_gl_col.h>


      LEDA_BEGIN_NAMESPACE

      /*{\Moptions usesubscripts=yes}*/


      // for textures
      struct tex_info
      {
	      char*  data;
	      int    w,h;
	      GLint  filter;
	      GLuint gl_num;

	      tex_info();

	      tex_info(char* d, const int& w2, const int& h2,
		      const GLint& f, const GLuint& n);
      };


      enum { D3_GL_TRANS=0, D3_GL_ROT=1, D3_GL_SCALE=2 };

      typedef set<node> node_set;

      typedef vector* point_pointer; 
      typedef set<point_pointer> point_pointer_set;

      typedef two_tuple<point_pointer,point_pointer> pp_tuple;
      typedef set<pp_tuple> pp_tuple_set;

      typedef two_tuple<seq_item,list_item> item_tuple;
      typedef list<point_pointer_set> pps_list;

      // for textures
      typedef two_tuple<pps_list,tex_info> tex_points_tuple;

      // for transformation sequences
      typedef two_tuple<int,GLdouble*> transformation;
      typedef list<transformation> transformation_list;


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


      /*{\Manpage {d3_gl_scene} {} {D3-GL-Scene} {S}}*/

      class __exportC d3_gl_window;

      class __exportC d3_gl_scene
      {

      friend class __exportC d3_gl_window;

      /*{\Mdefinition
      An instance |\Mvar| of the data type |\Mname| draws a graph with OpenGL in
      three-dimensional space. Thereto every node of the graph is assigned to
      a point in 3d space. Nodes are drawn as points, edges as lines and faces
      as polygons. You can setup the color of nodes, edges and faces
      as you like it. Moreover faces can be textured. 

      For drawing you have to overgive |\Mvar| to an instance of the
      class $d3\_gl\_window$, that finally draws |\Mvar| in a $LEDA-gl\_window$.
      }*/ 


      protected : 

      static int tex_counter;

      d3_gl_window* d3_gl_win;

      bool   draw_nodes;
      bool   draw_edges;
      bool   draw_faces;
      bool   visible;
      bool   texturing;

      // transformation sequences     
      list<transformation_list> sequences;
      list<GLdouble*> seq_mat;

      GLdouble trans_mat[16];  // 4*4 transformation-matrix

      double  xmin,xmax;
      double  ymin,ymax;
      double  zmin,zmax;

      vector  g_center;

      GLfloat node_width;
      GLfloat edge_width;

      bool	draw_bounding_box;
      GLfloat bb_line_width;
      d3_gl_col<GLfloat>  bb_line_color;

      sortseq<d3_gl_colF,point_pointer_set>  col_node_seq;
      node_array<seq_item>* col_node_item_array;

      sortseq<d3_gl_colF,pp_tuple_set> col_edge_seq;
      edge_array<seq_item>* col_edge_item_array;

      sortseq<d3_gl_colF,pps_list> col_face_seq;
      face_array<item_tuple>* col_face_item_array;

      // texture support
      sortseq<int,tex_points_tuple> tex_face_seq;
      face_array<item_tuple>* tex_face_item_array; 

      // node_array<vector> pos1;
      // we use a pointer to work around a bug in g++-2.8 
      node_array<vector>* pos1;

      graph* Hp; 


      public :

      /*{\Mcreation S}*/

      d3_gl_scene(graph& G, const node_array<vector>& pos) :
          col_node_seq(compare), col_edge_seq(compare), col_face_seq(compare)
      { init(); exchange_graph(G,pos); }
      /*{\Mcreate creates an instance |\Mvar| of the data type |\Mname|.
The visualized graph is |G|.
The positions of the nodes are given in |pos|.
\precond |pos| is a $node\_array$ of |G|.}*/

d3_gl_scene(graph& G, const node_array<rat_vector>& pos) :
      col_node_seq(compare), col_edge_seq(compare), col_face_seq(compare)
{ init(); exchange_graph(G,pos); }
/*{\Mcreate creates an instance |\Mvar| of the data type |\Mname|.
The visualized graph is |G|.
The positions of the nodes are given in |pos|.
\precond |pos| is a $node\_array$ of |G|.}*/

~d3_gl_scene();


protected :
	
void init(); 
void init_col_tex_seq();


public :

/*{\Moperations 3.1 4.4}*/

/*{\Mtext
{\bf 3.1 Color Information} }*/

void set_color(const d3_gl_colF& c, const list<node>& node_list);
/*{\Mop  sets the color of all nodes in $node\_list$ to $c$.
         \precond all nodes in $node\_list$ have to be nodes of |G|.}*/

void set_color(const d3_gl_colF& c, const list<edge>& edge_list);
/*{\Mop  sets the color of all edges in $edge\_list$ to $c$.
         \precond all edges in $edge\_list$ have to be edges of |G|.}*/

void set_color(const d3_gl_colF& c, const list<face>& face_list);
/*{\Mop  sets the color of all faces in $face\_list$ to $c$.
         \precond all faces in $face\_list$ have to be faces of |G|.}*/
			   

void set_color(const color& c, const list<node>& node_list, const float& a = 1.0f);
/*{\Mop  sets the color of all nodes in $node\_list$ to $c$ with alpha value $a$.
         \precond all nodes in $node\_list$ have to be nodes of |G|
          and $0.0\le a \le 1.0$.}*/

void set_color(const color& c, const list<edge>& edge_list, const float& a = 1.0f);
/*{\Mop  sets the color of all edges in $edge\_list$ to $c$ with alpha value $a$.
         \precond all edges in $edge\_list$ have to be edges of |G|
         and $0.0\le a \le 1.0$.}*/

void set_color(const color& c, const list<face>& face_list, const float& a = 1.0f);
/*{\Mop  sets the color of all faces in $face\_list$ to $c$ with alpha value $a$.
         \precond all faces in $face\_list$ have to be faces of |G|
         and $0.0\le a \le 1.0$.}*/


d3_gl_colF get_d3gl_color(const node& n) const;
/*{\Mop  returns the color of node $n$.
         \precond $n$ is a node of |G|.}*/

d3_gl_colF get_d3gl_color(const edge& e) const;
/*{\Mop  returns the color of edge $e$.
         \precond $e$ is an edge of |G|.}*/

d3_gl_colF get_d3gl_color(const face& f) const;
/*{\Mop  returns the color of face $f$.
         \precond $f$ is a face of |G|.}*/


color get_color(const node& n) const;
/*{\Mop  returns the color of node $n$.
         \precond $n$ is a node of |G|.}*/

color get_color(const edge& e) const;
/*{\Mop  returns the color of edge $e$.
         \precond $e$ is an edge of |G|.}*/

color get_color(const face& f) const;
/*{\Mop  returns the color of face $f$.
         \precond $f$ is a face of |G|.}*/


/*{\Mtext
{\bf 3.2 Texturing} }*/

int   reg_tex(char* t_data, const int& w, const int& h, const GLint& filter);
/*{\Mop  registers the texture defined by $t\_data$, $w$, $h$ and $filter$
         and returns a texture number.}*/

int   reg_tex(char* t_data, const int& w, const int& h,
		    const GLint& filter, const list<face>& face_list);  
/*{\Mop  registers the texture defined by $t\_data$, $w$, $h$ and $filter$ and
		 attaches it with every face of $face\_list$. A texture number is returned.
		 \precond all faces in $face_list$ have to be faces of |G| and have
		 to be triangles or quadrangles.}*/	

bool  unreg_tex(int tex_num);
/*{\Mop  unregisters the texture identified by $tex\_num$.
         If $tex\_num$ is an invalid texture number, false is returned and true
		 otherwise.}*/

int   get_texture(const face& f) const;
/*{\Mop  returns the texture num of the texture attached to $f$. -1 is
		 returned if $f$ isn't attached with any texture.			 
		 \precond $f$ is a face of |G|.}*/

bool  set_texture(int tex_num, const list<face>& face_list);
/*{\Mop  attaches all faces in $face\_list$ with the texture identified by $tex\_num$.
         If $tex\_num$ is an invalid texture number, false is returned and true
		 otherwise.
		 \precond all faces in $face\_list$ have to be faces of |G|.}*/

GLint get_tex_filter(int tex_num) const;
/*{\Mop  returns the current filter of the registered texture $tex\_num$.			 
         If $tex\_num$ is an invalid texture number -1 is returned.}*/

bool  set_tex_filter(int tex_num, const GLint& filter);
/*{\Mop  sets the current filter of the registered texture $tex\_num$
		 to $filter$. If $tex\_num$ is an invalid texture number false
		 is returned and true otherwise.}*/

char* get_tex_data(int tex_num) const; 
/*{\Mop  returns the texture data of the registered texture $tex\_num$.
         If $tex\_num$ is an invalid texture number NULL is returned.}*/
 
int   number_of_textures() const;
/*{\Mop  returns the current number of correctly registered textures.}*/


/*{\Mtext
{\bf 3.3 Exchange Graph} }*/

void exchange_graph(graph& G, const node_array<vector>& pos);
/*{\Mop  makes $G$ the current graph, resets all color and texture information
         to default values and assigns 3d-points in $pos$ to the nodes of $G$.
		 \precond |pos| is a $node\_array$ of |G|.}*/
void exchange_graph(graph& G, const node_array<rat_vector>& pos);
/*{\Mop  makes $G$ the current graph, resets all color and texture information
         to default values and assigns 3d-points in $pos$ to the nodes of $G$.
		 \precond |pos| is a $node\_array$ of |G|.}*/


protected :

void   update_textures();


public :

/*{\Mtext
{\bf 3.4 Modelview Matrix}

As it is quite common in OpenGL you can define a modelview matrix
for the graph that is visualized. When the graph is drawn,
this self defined matrix is multiplied with the current
OpenGL modelview matrix and thereby the final modelview matrix, that
projects the graph in 3d space, is created. The default modelview
matrix is the identity matrix. In OpenGL an array with length 16
represents a 4*4 matrix, whereby the first four elements of the
array are the first column, the second four elements are the
second column and so on.

}*/

void  mult_matrix(const GLdouble* m);
/*{\Mop  multiplies the current modelview matrix of |\Mvar| with $m$.
         \precond $m$ is a correct 4*4 OpenGL matrix.}*/

void  clear_matrix();
/*{\Mop  sets the modelview matrix to the identity matrix.}*/

void  set_matrix(const GLdouble* m);
/*{\Mop  sets the modelview matrix of |\Mvar| to $m$.
         \precond $m$ is a correct 4*4 OpenGL matrix.}*/

void  print_matrix() const;
/*{\Mop  prints the current modelview matrix of |\Mvar| to cout.}*/

const GLdouble* get_matrix() const;
/*{\Mop  returns a pointer to the current modelview matrix of |\Mvar|.}*/

void  get_modelview_matrix(GLdouble* matrix);
/*{\Mop  $matrix$ has to be a pointer to an 4*4 OpenGL matrix, i.e a
         pointer to an array with length 16.
         The current OpenGL modelview matrix is multiplied with
         the current modelview matrix of |\Mvar| and the result is
		 assigned componentwise to $matrix$. So $matrix$ contains
         the final modelview matrix, that projects |\Mvar| into space.}*/

void translate(const double& x, const double& y, const double& z, const bool& first = false);
/*{\Mop  multilpies the current modelview matrix of |\Mvar|, which is M,
         with a translation matrix T definded by $x$, $y$ and $z$. If first
	 is true, T*M is the new modelview matrix of |\Mvar|, otherwise
	 M*T is the new one. The order of multiplication is relevant as it is in OpenGL.}*/

void scale(const double& x, const double& y, const double& z, const bool& first = false);
/*{\Mop  multilpies the current modelview matrix of |\Mvar|, which is M,
         with a scale matrix SC definded by $x$, $y$ and $z$. If first
	 is true, SC*M is the new modelview matrix of |\Mvar|, otherwise
	 M*SC is the new one. The order of multiplication is relevant as it is in OpenGL.}*/

void rotate(const double& alpha, double x, double y, double z, const bool& first = false);
/*{\Mop  multilpies the current modelview matrix of |\Mvar|, which is M,
         with a rotation matrix R definded by $x$, $y$ and $z$. If first
	 is true, R*M is the new modelview matrix of |\Mvar|, otherwise
	 M*R is the new one. The order of multiplication is relevant as it is in OpenGL.}*/



/*{\Mtext
{\bf 3.5 Transformation Sequences}

A transformation sequence is a free defineable series of affine
transformations, that are permanently executed in order to realize
an animation. Within a sequence you can perform translations,
rotations and scalations by calling apropriate methods, that are described
in the following section. The order of transformations within the
sequence is relevant as it is in OpenGL.

If you are using any methods described in section 3.4, in particular
affine transformations, before you start a sequence, those operations
are performed before the operations of a sequence and the other way round.
So the affine transformations affecting the modelview matrix are performed
in the order they occur as it is in OpenGL.

}*/

int  begin_sequence();
/*{\Mop  marks the beginning of a new sequence and returns a sequence number.
         See the example at the end of this chapter for better understanding.}*/

int  end_sequence();
/*{\Mop  marks the end of a sequence and returns a sequence number.
         The number is equal to the number returned by begin\_sequence().
         See the example at the end of this chapter for better understanding.}*/

bool remove_sequence(const int& seq_num);
/*{\Mop  removes the sequence with the number $seq_num$.
         \precond $seq\_num$ is a correct sequence number.
         The function returns true, if the precondition is fulfilled and false otherwise.}*/ 

int  seq_translate(const double& x , const double& y , const double& z,
	   			   const double& x2, const double& y2, const double& z2,
				   int seq_num = -1);
/*{\Mop  adds a translation to the sequence identified by $seq\_num$ and returns
         a transformation number. By default $seq\_num$ is -1, which means,
		 that the current sequence is selected.
		 First performs a normal translation defined by $x$, $y$ and $z$.
         Then during every execution of the draw() method $x2$, $y2$ and $z2$
         are added to $x$, $y$ and $z$.
         \precond $seq\_num$ is a correct sequence number.
         If the precondition isn't fulfilled -1 is returned.}*/ 

int  seq_translate(const double* params, int seq_num = -1);
/*{\Mop  does the same thing as the previous method. The parameters
         are stored in $params$, that is a pointer to an array of length {\bf 6}.
         \precond $seq\_num$ is a correct sequence number.
         If the precondition isn't fulfilled -1 is returned.}*/

int  seq_scale(const double& x , const double& y , const double& z,
			   const double& x2, const double& y2, const double& z2,
			   int seq_num = -1);
/*{\Mop  adds a scalation to the sequence and returns a transformation
         number.By default $seq\_num$ is -1, which means,
		 that the current sequence is selected.
		 First performs a normal scalation defined by $x$, $y$ and $z$.
         Then during every execution of the draw() method $x2$, $y2$ and $z2$
         are added to $x$, $y$ and $z$.
         \precond $seq\_num$ is a correct sequence number.
         If the precondition isn't fulfilled -1 is returned.}*/ 

int  seq_scale(const double* params, int seq_num = -1);
/*{\Mop  does the same thing as the previous method. The parameters
         are stored in $params$, that is a pointer to an array of length {\bf 6}.
         \precond $seq\_num$ is a correct sequence number.
         If the precondition isn't fulfilled -1 is returned.}*/

int  seq_rotate(const double& start_angle, const double& alpha,
				const double& x, const double& y, const double& z,
				int seq_num = -1);   
/*{\Mop  adds a rotation to the sequence and returns a transformation
         number. By default $seq\_num$ is -1, which means,
		 that the current sequence is selected.
		 First performs a normal rotation along the axis defined
		 by ($x$,$y$,$z$) and (0,0,0) about the angle $start\_angle$.
         Then during every execution of the draw() method $alpha$
         is added to $start\_angle$.
         \precond $seq\_num$ is a correct sequence number.
         If the precondition isn't fulfilled -1 is returned.}*/       

int  seq_rotate(const double* params, int seq_num = -1);
/*{\Mop  does the same thing as the previous method. The parameters
         are stored in $params$, that is a pointer to an array of length {\bf 5}.
         \precond $seq_num$ is a correct sequence number.
         If the precondition isn't fulfilled -1 is returned.}*/

bool    set_seq_transform(const int& seq_num, const int& tramsform_num, const double* params);
/*{\Mop  sets the parameters of the transformation identified by $transform\_num$
         of the sequence represented by $seq\_num$ to the array-elements where $params$ points to.
		 \precond $seq\_num$ is a correct sequence number. $transform\_num$ is a correct
         transformation number. $params$ is a pointer to an array with correct length.
         The correct length is 5 or 6 depending on what transformation is changed.
         The function returns true, if all preconditions are fulfilled and false otherwise.}*/

double* get_seq_transform(const int& seq_num, const int& transform_num, int& l) const;
/*{\Mop  returns a pointer to an array, that contains the parameters of the transformation
         identified by $transform\_num$ of the sequence represented by $seq\_num$.
		 The length of the array is assigned to $l$.
		 \precond $seq\_num$ is a correct sequence number and $transform\_num$ is a correct
         transformation number. The function returns a valid pointer, if all preconditions are 
		 fulfilled and NULL otherwise.}*/

bool    remove_seq_transform(const int& seq_num, const int& transform_num);
/*{\Mop  removes the transformation identified by $transform\_num$ of the sequence
         represented by $seq\_num$. \precond $seq\_num$ is a correct sequence number
         and $transform\_num$ is a correct transformation number. The function
         returns true, if all preconditions are fulfilled and false otherwise.}*/
		 
/*{\Mtext
{\bf Example}

Here is an easy example of an transformation sequence for better
understanding :\\ \\
|\Mvar|.begin\_sequence(); \\
|\Mvar|.seq\_translate( x, y, z, 0, 0 ,0); \\
|\Mvar|.seq\_rotate(0,angle,$x\_rot$,$y\_rot$,$z\_rot$); \\
|\Mvar|.seq\_translate(-x,-y,-z, 0 ,0 ,0); \\
|\Mvar|.end\_sequence(); \\ \\
This code fragment generates a sequence, that rotates
|\Mvar| permanently about the axis defined by the points
$(x\_rot,y\_rot,z\_rot)$ and (0,0,0). The angle parameter
specifies the angle of rotation in degrees.

}*/


protected :

void draw();

void comp_optimal_coord(double& x0, double& x1, double& y0,
						double& y1, double& z0, double& z1) const;
void comp_coord_extrema(double& x0, double& x1, double& y0,
						double& y1, double& z0, double& z1) const; 

void nodes_to_points(const list<node>& n_list, point_pointer_set& S) const;
void edges_to_points(const list<edge>& e_list, pp_tuple_set& S) const;
void faces_to_points(const list<face>& f_list, pps_list& L) const;


public :

/*{\Mtext
{\bf 3.6 Getting/Setting Parameters} }*/

const graph* get_graph() const { return Hp; }
/*{\Mop  returns a pointer to |G|.}*/

vector get_position(const node& n) const
{ return (*pos1)[n];  }
/*{\Mop  returns the vector of $n$.
         \precond $n$ is a node of |G|.}*/

void set_position(const node& n, const vector& v)
{ set_position(n,v[0],v[1],v[2]); }
/*{\Mop  sets the position of $n$ to $v$.
         \precond $n$ is a node of |G|.}*/

void get_position(const node& n, double& x, double& y, double& z) const
{ vector v = (*pos1)[n];
  x = v[0]; y = v[1]; z = v[2];
}
/*{\Mop  assigns the position of $n$ to $x$, $y$ and $z$.
         \precond $n$ is a node of |G|.}*/

void set_position(const node& n, const double& x, const double& y, const double& z);
/*{\Mop  sets the position of $n$ to ($x$,$y$,$z$).
         \precond $n$ is a node of |G|.}*/

const node_array<vector>* get_position_array() const { return pos1;  }
/*{\Mop  returns a pointer the node\_array of |\Mvar|.}*/

void set_position_array(const node_array<vector>& pos);
/*{\Mop  sets the position of all nodes of |G| to the vectors in $pos$.
         \precond $pos$ is a node\_array of |G|.}*/


bool get_draw_nodes() const
{ return draw_nodes; }
/*{\Mop  returns the draw nodes parameter.}*/

void set_draw_nodes(const bool& b)
{ draw_nodes = b; }
/*{\Mop  sets the draw nodes parameter to $b$.}*/

bool get_draw_edges() const
{ return draw_edges; }
/*{\Mop  returns the draw edges parameter.}*/

void set_draw_edges(const bool& b)
{ draw_edges = b; }
/*{\Mop  sets the draw edges parameter to $b$.}*/

bool get_draw_faces() const
{ return draw_faces; }
/*{\Mop  returns the draw faces parameter.}*/

void set_draw_faces(const bool& b)
{ draw_faces = b; }
/*{\Mop  sets the draw faces parameter to $b$.}*/

bool get_texturing() const
{ return texturing; }
/*{\Mop  returns the texturing parameter.}*/

void set_texturing(const bool& b)
{ texturing = b; }
/*{\Mop  sets the texturing parameter to $b$.}*/

bool get_visible() const
{ return visible; }
/*{\Mop  returns the visible parameter.}*/

void set_visible(const bool& b)  
{ visible = b; }
/*{\Mop  sets the visible parameter to $b$.}*/


GLfloat get_node_width() const
{ return node_width; }
/*{\Mop  returns the node with.}*/

void set_node_width(const GLfloat& w)   
{ node_width = w; }
/*{\Mop  sets the node width to $w$.}*/ 

GLfloat get_edge_width() const
{ return edge_width; }
/*{\Mop  returns the edge with.}*/

void set_edge_width(const GLfloat& w)   
{ edge_width = w; } 
/*{\Mop  sets the edge width to $w$.}*/


bool get_draw_bounding_box() const
{ return draw_bounding_box; }
/*{\Mop  returns the draw bounding box parameter.}*/

void set_draw_bounding_box(const bool& b)  
{ draw_bounding_box = b; }
/*{\Mop  sets the draw bounding box parameter to $b$.}*/

GLfloat get_bb_line_width() const
{ return bb_line_width; }
/*{\Mop  returns the bounding box line with.}*/

void set_bb_line_width(const GLfloat& w)
{ bb_line_width = w; }
/*{\Mop  sets the bounding box line with to $w$.}*/

d3_gl_colF get_bb_line_color() const
{ return bb_line_color; }
/*{\Mop  returns the bounding box line color.}*/

void set_bb_line_color(const d3_gl_colF& c)
{ bb_line_color = c; }
/*{\Mop  sets the bounding box line color to $c$.}*/


/*{\Mtext
{\bf Notice}

The following methods don't regard the actual
modelview matrix of |\Mvar|. As the modelview
matrix of |\Mvar| affects its view, the following
methods can be described as static and view-independent.

}*/


vector get_center() const
{ return g_center;  }
/*{\Mop  returns the center of |G|.}*/

double get_xmin() const
{ return xmin; }
/*{\Mop  returns the smallest x coordinate.}*/

double get_xmax() const
{ return xmax; }
/*{\Mop  returns the biggest x coordinate.}*/

double get_ymin() const
{ return ymin; }
/*{\Mop  returns the smallest y coordinate.}*/

double get_ymax() const
{ return ymax; }
/*{\Mop  returns the biggest y coordinate.}*/

double get_zmin() const
{ return zmin; }
/*{\Mop  returns the smallest z coordinate.}*/

double get_zmax() const
{ return zmax; }
/*{\Mop  returns the biggest z coordinate.}*/

double get_width() const
{ return xmax-xmin; }
/*{\Mop  returns the width of the points of |G|.}*/

double get_height() const
{ return ymax-ymin; }
/*{\Mop  returns the height of the points of |G|.}*/

double get_depth() const
{ return zmax-zmin; }
/*{\Mop  returns the depth of the points of |G|.}*/

};


int compare(const d3_gl_scene& scene1, const d3_gl_scene& scene2);

typedef d3_gl_scene* SC_PTR;
int compare(const SC_PTR& scene1, const SC_PTR& scene2); 

int compare(const point_pointer& p1, const point_pointer& p2);

int compare(const seq_item& it1, const seq_item& it2);


#if LEDA_ROOT_INCL_ID == 600144
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif

