#include <LEDA/d3_hull.h>
#include <LEDA/d3_rat_point.h>
#include <LEDA/string.h>
#include <LEDA/file.h>
#include <LEDA/file_panel.h> 

#include "incl/texturing.h"
#include "incl/off_import.h"
#include "incl/Demo1/dir_panel.h" 

#include <LEDA/d3_gl_scene.h>


using namespace leda;
 

#if defined(LEDA_STD_IO_HEADERS)
using std::ifstream;
#endif


#define ERR_MSG1 "Error on loading the texture-file ("+d1_str[7]+") !! Probably the graphics-format of the file is not supported by the programm !!" 
#define ERR_MSG2 "The texture-file you entered ("+d1_str[7]+") doesnt exist !!!"
#define ERR_MSG3 "One or more input-fields are not valued !!! Please try again !!!"

#define ERR_MSG4 "The format you entered is invalid !!!"
#define ERR_MSG5 "The text-file you selected can't be opened."
#define ERR_MSG6 "The text-file you selected contains invalid data."
#define ERR_MSG7 "The directory you entered doesn't exist."
#define ERR_MSG8 "The file you entered doesn't exist."
#define ERR_MSG9 "Triangulation impossible. You have to select \"convex hull\" firstly."

#define MIN -20000
#define MAX  20000
#define BG_SIZE  4096
#define INF_SIZE 50000
#define INF_GRID_DIST 1000
#define FRAME_RATE 30.0

#define MAX_COORD_SIZE  2000000000
#define MIN_COORD_SIZE -2000000000
#define MAX_COORD_STR " 2 billion"
#define MIN_COORD_STR "-2 billion"


enum { FRONT=0, BACK=1, LEFT=2, RIGHT=3, TOP=4, BOTTOM=5 };


d3_gl_window*   d3gl_win = NULL;
gl_window*      gl_win   = NULL;
window*         w2       = NULL;
	      
panel*          P1 = NULL; // gen-dialog1 , gen-dialog2
window*         P3 = NULL; // options-dialogs (op-dialog1, op-dialog2 and op-dialog3)

string sep; // os-dependent separator 

int mode = 10;     // camera, moving, rotating, scaling

int vis_count = 1;

string* d1_str;	
string* d2_str; 

//-----------------------------------------------------
// variables and some special items of both gen dialogs
string*  x_str = new string("0");
string*  y_str = new string("0");
string*  z_str = new string("0");
string*  width_str  = new string("2000");
string*  height_str = new string("2000");
string*  depth_str  = new string("2000");
string*  num_of_pts_str = new string("200");
int      types = 0; // ball,cube,para,mesh,line,sphere 

string* tex_file = new string("");

int convex_hull    = 1;
int triangulate    = 1;
int draw_nodes     = 1; 
int draw_edges     = 1;
int draw_faces     = 1;
int node_width     = 2;
int edge_width     = 2; 
int texturing      = 1;
int middle_point   = 1;
int tex_detail     = 1;	

color nodes_color = red;
color edges_color = green;
color faces_color = blue;

int rotation = 1;
string* x_rot_axis_str = new string("0");
string* y_rot_axis_str = new string("1");
string* z_rot_axis_str = new string("0");
string* x_rot_pt_str   = new string("0");
string* y_rot_pt_str   = new string("0");
string* z_rot_pt_str   = new string("0");
string* rot_angle_str  = new string("15");

// some special panel_items, which are often used by event methods
panel_item* rot_items;
panel_item* rot_sub_items;

// variables and special items of options-dialogs

string path;           // path for object textures
string bg_path;        // path for background textures

//---------------------------------
// load_scene_from_file-dialog
string txt_files_path;      // path for text-files containing points
string file_pattern("*.obj");
string filename("dolphins.obj");
int    off_format = 0;   

//---------------------------------
// box variables
double box_xmin, box_xmax;
double box_ymin, box_ymax;
double box_zmin, box_zmax;

int texturing_box = 0;
int box_outlines  = 0; 
int box_inlines   = 0;
int box_filling   = 0;

int box_line_width; 

d3_gl_col box_line_color;
d3_gl_col box_fill_color;

//---------------------------------

bool   infinity = false;
int*   inf_backup = NULL;
double height_dist;
menu*  temp = NULL;

gl_camera cam;       // camera
vector3d  eye_pos;   // eye-pos

//---------------------------------

// plane variables
double plane_xmin, plane_xmax;
double plane_zmin, plane_zmax;
double plane_height;

int texturing_plane = 0;
int plane_outlines  = 0; 
int plane_x_parallels = 0;
int plane_z_parallels = 0;
int plane_filling   = 0;

int plane_line_width; 

d3_gl_col plane_line_color;
d3_gl_col plane_fill_color;

//---------------------------------
// texture variables
tc_texture* T[7];
GLuint      tex_name[7];
float       tex_width[7];   
float       tex_height[7];
//---------------------------------


bool   camera_on  = true;
bool   with_status_win = true;


typedef two_tuple<tc_texture*, int> texture_int;
list<texture_int> scene_textures;


typedef four_tuple<d3_gl_scene*, int*, D_PTR, string*> sc_model;
					 // scene,type and texture name
list<sc_model> models;


typedef GRAPH<d3_rat_point,int> PARA_GRAPH;


void show_short_message(const string& message, const string& headline);
void dummy_handler(string s);
void overtake_params(int* dia1_int, color* dia1_col[3],int* dia2_int);
bool test_input(string str[], const unsigned int& l); 


#include "incl/Demo1/string_funcs.h"
#include "incl/Demo1/file_funcs.h"
#include "incl/Demo1/event_listeners.h"
#include "incl/Demo1/texture_funcs.h"
#include "incl/Demo1/scene_funcs.h"
#include "incl/Demo1/dialogs.h"
#include "incl/Demo1/draw_funcs.h"


void show_short_message(const string& message, const string& headline)
{
	panel P;
	P.set_panel_bg_color(ivory);
	P.text_item("\\bf \\blue "+headline);
	P.text_item("");
	P.text_item("\\n ");
	P.text_item(message);
	P.button("ok",100);
	P.open();
}

void dummy_handler(string s)
{ }

void overtake_params(int* dia1_int, color* dia1_col[3],int* dia2_int)
{
	*x_str = d1_str[0];
	*y_str = d1_str[1];		
	*z_str = d1_str[2];
	*width_str  = d1_str[3];
	*height_str = d1_str[4];
	*depth_str  = d1_str[5];
	*num_of_pts_str = d1_str[6];
	*tex_file = d1_str[7];

	types       = dia1_int[0];
	texturing   = dia1_int[1];
	tex_detail  = dia1_int[2];
	draw_nodes  = dia1_int[3];
	draw_edges  = dia1_int[4];
	draw_faces  = dia1_int[5];
	node_width  = dia1_int[6];
	edge_width  = dia1_int[7];
	convex_hull = dia1_int[8];
	triangulate = dia1_int[9];

	nodes_color = *dia1_col[0];
	edges_color = *dia1_col[1];
	faces_color = *dia1_col[2];

	*x_rot_axis_str = d2_str[0];
	*y_rot_axis_str = d2_str[1];
	*z_rot_axis_str = d2_str[2];
	*x_rot_pt_str = d2_str[3];
	*y_rot_pt_str = d2_str[4];
	*z_rot_pt_str = d2_str[5];
	*rot_angle_str = d2_str[6];

	rotation = dia2_int[0];
	middle_point =dia2_int[1];
}


bool test_input(string str[], const unsigned int& l) 
{
	for(unsigned int i=0; i<l; i++){
		if(str[i].length()==0 || (str[i].length()==1 && str[i][0]=='-' )){
			P1->acknowledge(ERR_MSG3);		
			return false;
		}
	}
	return true;
}


void do_gen_dialog(d3_gl_scene* sc, const bool& randomizer)
{ 
	bool dialog_one = true;		
	int x = -1, y = -1, w = -1, h = -1;	

	int    dialog1_int[10];
	color* dialog1_col[3];		
	int    dialog2_int[2];  	
	
	d1_str = new string[8];
	d2_str = new string[7];
	rot_items = new panel_item[5];
	rot_sub_items = new panel_item[3];	

	if(sc == NULL){			
		d1_str[0] = *x_str;
		d1_str[1] = *y_str;		
		d1_str[2] = *z_str;
		d1_str[3] = *width_str;
		d1_str[4] = *height_str;
		d1_str[5] = *depth_str;
		d1_str[6] = *num_of_pts_str;
		d1_str[7] = *tex_file;

		dialog1_int[0] = types;
		dialog1_int[1] = texturing;
		dialog1_int[2] = tex_detail;
		dialog1_int[3] = draw_nodes;
		dialog1_int[4] = draw_edges;
		dialog1_int[5] = draw_faces;
		dialog1_int[6] = node_width;
		dialog1_int[7] = edge_width;
		if(randomizer){
			dialog1_int[8] = convex_hull;
			dialog1_int[9] = triangulate;
		}
		else{
			dialog1_int[8] = 0;
			dialog1_int[9] = 0;
		}

		int r,g,b;
		nodes_color.get_rgb(r,g,b);
		dialog1_col[0] = new color(r,g,b);
		edges_color.get_rgb(r,g,b);
		dialog1_col[1] = new color(r,g,b);
		faces_color.get_rgb(r,g,b);
		dialog1_col[2] = new color(r,g,b);        

		d2_str[0] = *x_rot_axis_str;
		d2_str[1] = *y_rot_axis_str;
		d2_str[2] = *z_rot_axis_str;
		d2_str[3] = *x_rot_pt_str;
		d2_str[4] = *y_rot_pt_str;
		d2_str[5] = *z_rot_pt_str;
		d2_str[6] = *rot_angle_str;

		dialog2_int[0] = rotation;
		dialog2_int[1] = middle_point;		 
	}
	else{		
		double* rot_info = NULL;
		sc_model m;
		forall(m,models){
		  if(m.first() == sc)
			  rot_info = m.third();
		}

		double* first_trans  = sc->get_seq_trans_a(0,0);
		double* rotation     = sc->get_seq_trans_a(0,1);		
		vector v = sc->get_center();

		d1_str[0] = int_to_string( (int)(v[0]+first_trans[0]-rot_info[0]) );
		d1_str[1] = int_to_string( (int)(v[1]+first_trans[1]-rot_info[1]) );
		d1_str[2] = int_to_string( (int)(v[2]+first_trans[2]-rot_info[2]) );								

		d1_str[3] = int_to_string( ((int)(sc->get_width()))  );
		d1_str[4] = int_to_string( ((int)(sc->get_height())) );
		d1_str[5] = int_to_string( ((int)(sc->get_depth()))  );
		d1_str[6] = int_to_string( ((int)((sc->get_graph())->number_of_nodes())) );
		d1_str[7] = *(m.fourth()); 
		
		// dialog1_int[0] = model_info[0];
		dialog1_int[1] = ((int)(sc->get_texturing()));

		int detail = 0;	
		if(dialog1_int[1]){ // texturing ??
			GLuint tex_num = sc->get_texture((sc->get_graph())->first_face());
			GLint filter = sc->get_tex_filter(tex_num);				
			if(filter == GL_LINEAR)
				detail = 1;
			else
				detail = 0;
		}
		dialog1_int[2] = detail;

		int* model_info = m.second();		

		dialog1_int[3] = ((int)(sc->get_draw_nodes()));
		dialog1_int[4] = ((int)(sc->get_draw_edges()));
		dialog1_int[5] = ((int)(sc->get_draw_faces()));		 
		dialog1_int[6] = ((int)(sc->get_node_width()));
		dialog1_int[7] = ((int)(sc->get_edge_width()));
		dialog1_int[8] = model_info[2];
		dialog1_int[9] = model_info[3];
		
		d3_gl_col c1,c2,c3;				
		
		if(!sc->get_color(sc->get_graph()->first_node(),c1)){
			int r,g,b;
			nodes_color.get_rgb(r,g,b);
			dialog1_col[0] = new color(r,g,b);
		}
		else dialog1_col[0] = new color(c1.get_color());

		if(!sc->get_color(sc->get_graph()->first_edge(),c2)){
			int r,g,b;
			edges_color.get_rgb(r,g,b);
			dialog1_col[1] = new color(r,g,b);
		}
		else dialog1_col[1] = new color(c2.get_color());

		if(!sc->get_color(sc->get_graph()->first_face(),c3)){
			int r,g,b;
			faces_color.get_rgb(r,g,b);
			dialog1_col[2] = new color(r,g,b);
		}
		else dialog1_col[2] = new color(c3.get_color());
		
		d2_str[0] = int_to_string( ((int)(rotation[2])) );
		d2_str[1] = int_to_string( ((int)(rotation[3])) );
		d2_str[2] = int_to_string( ((int)(rotation[4])) );		
		d2_str[3] = int_to_string( ((int)(first_trans[0])) ); // x of rot-pt
		d2_str[4] = int_to_string( ((int)(first_trans[1])) ); // y of rot-pt
		d2_str[5] = int_to_string( ((int)(first_trans[2])) ); // z of rot-pt
		d2_str[6] = int_to_string( ((int)(rot_info[3])) );    // rot-angle

		dialog2_int[0] = model_info[0];		
		dialog2_int[1] = model_info[1];				
	}	 

	do { gen_dialog(dialog_one,sc,dialog1_int,dialog1_col,
					dialog2_int,x,y,w,h,randomizer); } while(P1!=NULL);
	
	delete[] d1_str;
	delete[] d2_str;
	delete[] rot_items;
	delete[] rot_sub_items;
	
	delete dialog1_col[0];
	delete dialog1_col[1];
	delete dialog1_col[2];
}


// ---------------- start help-dialog -----------------

#define NUM_LINES 53

void draw_text() {
	
    double txt_y  = P3->ymax() - P3->pix_to_real(8);
	double txt_h  = P3->text_height("H") + P3->pix_to_real(4);
	double win_w  = P3->xmax()-P3->xmin();			
	
	double max_right_w = P3->text_width("ddddeincrease speed and sensitivity");	
	double max_left_w  = P3->text_width("ctrl + left mouse");

	P3->start_buffering();
	P3->clear();
	
	int index = *((int*)P3->get_inf(0));
	int until = *((int*)P3->get_inf(1));	

	while(index<until){
		// one word 
		if(d2_str[index].length() == 0){ 
			if( string(d1_str[index](0,7)) == string("\\bf\\blue") ){
				double txt_w = P3->text_width(d1_str[index]);
				P3->text_box((win_w-txt_w)/2,P3->xmax(),txt_y,d1_str[index]);
			}			
			else if(d1_str[index].pos("mouse and key") > 0 ||
				    d1_str[index].pos("infinity mode") > 0)
				P3->draw_text( win_w/2+(win_w/2-max_right_w)/2,txt_y,d1_str[index]);			
			else{
				double txt_w = P3->text_width(d1_str[index]);
				P3->text_box((win_w-txt_w)/2,P3->xmax(),txt_y,d1_str[index]);				
			}
		}
		// two words
		else{
			if(d2_str[index] == "format1"){
				double txt_w = P3->text_width("- This  programm  currently  only  supports  bitmap-files.");
				P3->text_box((win_w-txt_w)/2,P3->xmax(),txt_y,d1_str[index]);
			}				
			else{
				// left
				P3->draw_text( (win_w/2-max_left_w)/2,txt_y,d2_str[index]);			

				// right
				P3->draw_text( win_w/2+(win_w/2-max_right_w)/2,txt_y,d1_str[index]);
			}
		}				

		txt_y -= txt_h;
		++index;			
	}	

	P3->flush_buffer();
	P3->stop_buffering();
}

void redraw(window* W)
{ draw_text(); }

void scroll_down(int i)
{
	int* pos0 = (int*)P3->get_inf(0);
	int* pos1 = (int*)P3->get_inf(1);
	int* scroll = (int*) P3->get_inf(3);
	if(*scroll){
		if(*pos1<NUM_LINES){
			*pos1 += 1;
			*pos0 += 1;		
			draw_text();			
			int* win_lines = (int*)P3->get_inf(2);
			double dy = (double(*pos0))/(double(NUM_LINES - (*win_lines)) );
			P3->set_scrollbar_pos(dy);
		}
		*scroll = 0;
	}
	else
		*scroll = 1;
}

void scroll_up(int i)
{
	int* pos0   = (int*)P3->get_inf(0);
	int* pos1   = (int*)P3->get_inf(1);
	int* scroll = (int*) P3->get_inf(3);
	if(*scroll){
		if(*pos0>0){
			*pos0 -= 1;
			*pos1 -= 1;
			draw_text();			
			int* win_lines = (int*)P3->get_inf(2);
			double dy = (double(*pos0))/(double(NUM_LINES - *(win_lines)) );
			P3->set_scrollbar_pos(dy);
		}
		*scroll = 0;
	}
	else
		*scroll = 1;
}

void scroll_drag(int i)
{
  if (i < 0) return;

  int* pos0 = (int*)P3->get_inf(0);
  int* pos1 = (int*)P3->get_inf(1);
  int* win_lines = (int*)P3->get_inf(2);

  int pos = int((i/1000.0)*(NUM_LINES-(*win_lines)));
  if (pos != *pos0)
  { *pos0 = pos;
    *pos1 = pos+(*win_lines);    
    draw_text();
  }
}

void help_dialog()
{
	d1_str = new string[NUM_LINES];
	d2_str = new string[NUM_LINES];

	d1_str[0]  = "\\bf\\blue Select, Deselect And Change Objects"; d2_str[0] = "";
	d1_str[1]  = " ";                d2_str[1]  = "";
	d1_str[2]  = "select object";    d2_str[2]  = "ctrl + left mouse";
	d1_str[3]  = "deselect object "; d2_str[3]  = "ctrl + left mouse";
	d1_str[4]  = "change object";    d2_str[4]  = "ctrl + right mouse";
	d1_str[5] = " ";                 d2_str[5] = "";

	d1_str[6]  = "\\bf\\blue How To Control The Camera"; d2_str[6] = "";
	d1_str[7]  = " ";             d2_str[7]  = "";
	d1_str[8]  = "go up";         d2_str[8]  = "7";
	d1_str[9]  = "go down ";      d2_str[9]  = "1";
	d1_str[10]  = "go ahead";      d2_str[10]  = "8";
	d1_str[11]  = "go back";       d2_str[11]  = "2";
	d1_str[12]  = "turn left";     d2_str[12]  = "4";
	d1_str[13]  = "turn right";    d2_str[13]  = "6";
	d1_str[14]  = "turn up";       d2_str[14]  = "3";
	d1_str[15]  = "turn down";     d2_str[15]  = "9";
	d1_str[16] = "rotate left";   d2_str[16] = "<-";
	d1_str[17] = "rotate right";  d2_str[17] = "->";
	d1_str[18] = "\\bf Note : \\rm Numlock has to be on !"; d2_str[18] = "";

	d1_str[19] = " ";             d2_str[19] = "";
	d1_str[20] = "\\bf\\blue How To Move Selected Objects"; d2_str[20] = "";
	d1_str[21] = " ";             d2_str[21] = "";
	d1_str[22] = "move up";       d2_str[22] = "7";
	d1_str[23] = "move down";     d2_str[23] = "1";
	d1_str[24] = "move left";     d2_str[24] = "4";
	d1_str[25] = "move right";    d2_str[25] = "6";
	d1_str[26] = "move ahead";    d2_str[26] = "8";
	d1_str[27] = "move back";     d2_str[27] = "2";
	d1_str[28] = "\\bf Note : \\rm Numlock has to be on !"; d2_str[28] = "";

	d1_str[29] = " ";                                  d2_str[29] = "";
	d1_str[30] = "\\bf\\blue Further Functional Keys"; d2_str[30] = "";
	d1_str[31] = " ";                                  d2_str[31] = "";
	d1_str[32] = "open / close status-line";           d2_str[32] = "F1";
	d1_str[33] = "turn camera on / off";               d2_str[33] = "F2";
	d1_str[34] = "orthogonalize camera";               d2_str[34] = "F3";
	d1_str[35] = "reset camera to start-pos";          d2_str[35] = "F4";
	d1_str[36] = "change between \"normal mode\"";     d2_str[36] = "F5";
	d1_str[37] = " and \"infinity mode\"";             d2_str[37] = ""; 
	d1_str[38] = " ";                                  d2_str[38] = "";   

	d1_str[39] = "delete selected object";             d2_str[39] = "d";
	d1_str[40] = "increase speed and sensitivity";     d2_str[40] = "+";
	d1_str[41] = "of mouse and keyboard";              d2_str[41] = "";
	d1_str[42] = "decrease speed and sensitivity";     d2_str[42] = "-";
	d1_str[43] = "of mouse and keyboard";              d2_str[43] = "";
	d1_str[44] = "quit programm";                      d2_str[44] = "esc"; 

	d1_str[45] = " ";             d2_str[45] = "";
	d1_str[46] = "\\bf\\blue Supported Formats Of Graphic-Files"; d2_str[46] = "";
	d1_str[47] = " ";             d2_str[47] = "";
	d1_str[48] = "- This  programm  currently  only  supports  bitmap-files."; d2_str[48] = "format1";
	d1_str[49] = "- Width and height of the bitmap-file have to be equal."; d2_str[49] = "format1";
	d1_str[50] = "- Width and height have to be a power of two (2,4,8,16,32,64,...)"; d2_str[50] = "format1";
	d1_str[51] = " "; d2_str[51] = "";
	d1_str[52] = " "; d2_str[52] = "";
	
	window W(550,460,"Help Panel");
	P3 = &W;
	W.set_bg_color(ivory);
	W.set_redraw(redraw);
	W.display(window::center,window::center);		

	int width    = W.width();
	int height   = W.height();		
	
	panel P;
	P.set_panel_bg_color(ivory);		
	P.set_bg_color(ivory);	
	P.set_border_color(ivory);
	P.set_border_width(0);
	P.button("close",1);
	P.display(0,0,W);
	P.resize(0,height-P.get_panel_height(),width,P.get_panel_height());
	
	double txt_h = W.text_height("H") + W.pix_to_real(4);
	double wh =  W.ymax() - W.ymin() - W.pix_to_real(8) - W.pix_to_real(P.get_panel_height());
	int win_lines = int(wh/txt_h);

	W.set_inf(new int(0),0);	
	int txt_end;
    (win_lines<NUM_LINES) ? txt_end=win_lines : txt_end=NUM_LINES;
	W.set_inf(new int(txt_end),1);
	W.set_inf(new int(win_lines),2);
	W.set_inf(new int(1),3);
	W.set_inf(new int(P.get_panel_height()),4);

	double f = double(win_lines)/((double)NUM_LINES);
    if (f < 1)
       W.open_scrollbar(scroll_up,scroll_down,scroll_drag,f);
    else
       W.close_scrollbar();	

	W.redraw();
	
	int e,val;	
	double x,y;

	do{		
		if(width != W.width() || height != W.height()){
			width  = W.width();
			height = W.height();			

			txt_h = W.text_height("H") + W.pix_to_real(4);
			wh =  W.ymax() - W.ymin() - W.pix_to_real(8) - W.pix_to_real(P.get_panel_height());
			win_lines = int(wh/txt_h);

			delete (int*)W.get_inf(0);
			delete (int*)W.get_inf(1);
			delete (int*)W.get_inf(2);
			delete (int*)W.get_inf(4);

			W.set_inf(new int(0),0);				
			(win_lines<NUM_LINES) ? txt_end=win_lines : txt_end=NUM_LINES;
			W.set_inf(new int(txt_end),1);
			W.set_inf(new int(win_lines),2);
			W.set_inf(new int(P.get_panel_height()),4);			

			double f = double(win_lines)/((double)NUM_LINES); 
			if (f < 1){
				P.resize(0,W.height()-P.get_panel_height(),W.width()-20,P.get_panel_height());
				W.open_scrollbar(scroll_up,scroll_down,scroll_drag,f);
			}
			else{
				W.close_scrollbar();
				P.resize(0,W.height()-P.get_panel_height(),W.width(),P.get_panel_height());
			}
			W.redraw();
		}
		e = P.get_event(val,x,y);
	}while(e != button_press_event && val != 1);		
	

	delete (int*)W.get_inf(0);
	delete (int*)W.get_inf(1);
	delete (int*)W.get_inf(2);
	delete (int*)W.get_inf(3);
	delete (int*)W.get_inf(4);

	delete[] d1_str;
	delete[] d2_str;

	P3 = NULL;
} 

// ---------------- end   help-dialog -----------------


void delete_heap()
{
	sc_model m;
	forall(m,models){
	      d3gl_win->remove_scene(m.first());
		  delete   m.first();
		  delete[] m.second();
		  delete[] m.third();
		  delete   m.fourth();
		  models.remove(m);	 
	}	
	
	delete d3gl_win; 

	if(w2){
		if(!w2->is_closed())
			w2->close();
		if(temp){
			if(!temp->is_closed())
				temp->close();
			delete temp;
		}
		delete w2;
	}

	for(int i=0; i<7; i++)
		delete T[i];

	if(inf_backup)
		delete[] inf_backup;

	delete  x_str;
    delete  y_str;
    delete  z_str;
    delete  width_str;
    delete  height_str;
    delete  depth_str;
    delete  num_of_pts_str;

	delete tex_file;

	delete x_rot_axis_str;
    delete y_rot_axis_str;
    delete z_rot_axis_str;
    delete x_rot_pt_str;
    delete y_rot_pt_str; 
    delete z_rot_pt_str;
    delete rot_angle_str;	
	
	texture_int scene_tex;
	forall(scene_tex,scene_textures)
		delete scene_tex.first();
}

void set_status_string1(const bool& camera)
{
  string s1,s2;

  if(camera) s1 = "ON";
  else       s1 = "OFF";

  if(infinity) s2 = "ON";
  else         s2 = "OFF";

  d3gl_win->set_status_string("\\bf CAMERA : \\blue "+s1+" \\black  INFINITY : \\blue "+s2+" \\c ");
}


void set_status_string2(const int& mode, const bool& mouse)
{
	switch(mode){
	case 10 : {
		if(mouse)
			d3gl_win->set_status_string("\\bf MODE : \\blue camera   \\black  LEFT : \\blue go ahead   \\black  RIGHT : \\blue go back \\c ");
		else
			d3gl_win->set_status_string("\\bf MODE : \\blue camera   \\black  CONTROL : \\blue see Help-Menu \\c ");
		break;
			  }

	case 11 : {
		if(mouse)
			d3gl_win->set_status_string("\\bf MODE : \\blue moving   \\black  LEFT : \\blue go ahead   \\black  RIGHT : \\blue go back \\c ");
	    else
	 	    d3gl_win->set_status_string("\\bf MODE : \\blue moving   \\black  CONTROL : \\blue 8,2,4,6,7,1 \\c ");
		break;
			  }

	case 12 : {
		if(mouse)
			d3gl_win->set_status_string("\\bf MODE : \\blue rotating   \\black  LEFT : \\blue start rotation   \\black  RIGHT : \\blue stop rotation \\c ");
	    else
		    d3gl_win->set_status_string("\\bf MODE : \\blue rotating   \\black  ROT-AXIS : \\blue 4,6,7,1  \\black  ROT-SPEED : \\blue +, - \\c ");
		break;
			  }

	case 13 : {
		if(mouse)
		    d3gl_win->set_status_string("\\bf MODE : \\blue scaling   \\black  LEFT : \\blue stretching   \\black  RIGHT : \\blue shrunking \\c ");
		else
		    d3gl_win->set_status_string("\\bf MODE : \\blue scaling   \\black  CONTROL : \\blue +, - \\c ");
		break;
			  }
	}
}

void reset_camera()
{
	double range;
	double x_mp;
	double z_mp;
	double height;

	if(texturing_plane || plane_x_parallels || plane_z_parallels || plane_outlines || plane_filling){ // plane
		range  = ((plane_xmax-plane_xmin)+(plane_zmax-plane_zmin))/2*0.625;
		x_mp   = plane_xmin+(plane_xmax-plane_xmin)/2;
		z_mp   = plane_zmin+(plane_zmax-plane_zmin)/2;
		height = range/4.1666;		
	}
	else if(texturing_box || box_outlines || box_inlines || box_filling){
		range  = ((box_xmax-box_xmin)+(box_zmax-box_zmin))/2*0.625;
		x_mp   = box_xmin+(box_xmax-box_xmin)/2;
		z_mp   = box_zmin+(box_zmax-box_zmin)/2;
		height = range/4.1666;
	}
	else if(d3gl_win->get_draw_axis() || d3gl_win->get_draw_coord() || d3gl_win->get_draw_pts()){		
		double xmin, xmax;
		double ymin, ymax;
		double zmin, zmax;
		d3gl_win->get_coord(xmin,xmax,ymin,ymax,zmin,zmax);

		range  = ((xmax-xmin)+(zmax-zmin))/2*0.625;
		x_mp   = xmin+(xmax-xmin)/2;
		z_mp   = zmin+(zmax-zmin)/2;
		height = range/4.1666;		
	}	
	else{
		x_mp = 0.0;
		z_mp = 0.0;
		height = 1.0;
		range  = 1.0;
	}	

	cam.set( x_mp+range , height , z_mp+range,
			 x_mp       , height , z_mp,
			 0.0        , 1.0    , 0.0 );
}


inline void update_camera_action()
{	
	vector3d dist = cam.get_eye() - eye_pos;
	eye_pos = cam.get_eye();
	double new_height_dist = eye_pos[1] - plane_height;					  
	
	if(new_height_dist<0) new_height_dist *= -1;

	if(new_height_dist > height_dist){
	  plane_xmin -= new_height_dist;
	  d3gl_win->set_xmin(plane_xmin);
	  plane_xmax += new_height_dist;
	  d3gl_win->set_xmax(plane_xmax);

	  plane_zmin -= new_height_dist;
	  d3gl_win->set_zmin(plane_zmin);
	  plane_zmax += new_height_dist;
	  d3gl_win->set_zmax(plane_zmax);
	  height_dist = new_height_dist;
	}	

	if(dist[0]<0){
	  plane_xmin += dist[0];
	  d3gl_win->set_xmin(plane_xmin);						  
	}
	else{
	  plane_xmax += dist[0];
	  d3gl_win->set_xmax(plane_xmax);						  
	}					  

	if(dist[2]<0){
	  plane_zmin += dist[2];
	  d3gl_win->set_zmin(plane_zmin);						  
	}
	else{
	  plane_zmax += dist[2];
	  d3gl_win->set_zmax(plane_zmax);			  
	}	
}

inline void update_mouse_mode(int button)
{
	mode = button;
	set_status_string2(mode,true);	
}

void swap_infinity()
{
	infinity = !infinity;
	if(infinity){
		// store some information
		inf_backup = new int[24];
		inf_backup[0]  = texturing_box;
		inf_backup[1]  = box_outlines; 
		inf_backup[2]  = box_inlines;
		inf_backup[3]  = box_filling;

		inf_backup[4]  = (int)d3gl_win->get_xmin();
		inf_backup[5]  = (int)d3gl_win->get_xmax();
		inf_backup[6]  = (int)d3gl_win->get_ymin();
		inf_backup[7]  = (int)d3gl_win->get_ymax();
		inf_backup[8]  = (int)d3gl_win->get_zmin();
		inf_backup[9]  = (int)d3gl_win->get_zmax();

		inf_backup[10] = (int)plane_xmin;
		inf_backup[11] = (int)plane_xmax;
		inf_backup[12] = (int)plane_zmin;
		inf_backup[13] = (int)plane_zmax;
		inf_backup[14] = (int)plane_height;

		inf_backup[15] = (int)d3gl_win->get_x_grid_dist();
		inf_backup[16] = (int)d3gl_win->get_y_grid_dist();
		inf_backup[17] = (int)d3gl_win->get_z_grid_dist();

		inf_backup[18]  = (int)box_xmin;
		inf_backup[19]  = (int)box_xmax;
		inf_backup[20]  = (int)box_ymin;
		inf_backup[21]  = (int)box_ymax;
		inf_backup[22]  = (int)box_zmin;
		inf_backup[23]  = (int)box_zmax;				

		// make box invisible
		texturing_box = 0;
		box_outlines = 0; 
		box_inlines = 0;
		box_filling = 0;

		// reset plane 	
		eye_pos = cam.get_eye();
		height_dist = eye_pos[1] - plane_height;
		if(height_dist < 0) height_dist *= -1;

		double d = plane_xmax - plane_xmin;
		plane_xmax += d + height_dist;
		plane_xmin -= d - height_dist;

        d = plane_zmax - plane_zmin;
		plane_zmax += d + height_dist;
		plane_zmin -= d - height_dist;				
				
		if(plane_xmax-plane_xmin<INF_SIZE){
			double d = INF_SIZE-(plane_xmax-plane_xmin);
			plane_xmax += d/2;
			plane_xmin -= d/2;
		}				

		if(plane_zmax-plane_zmin<INF_SIZE){
			double d = INF_SIZE-(plane_zmax-plane_zmin);
			plane_zmax += d/2;
			plane_zmin -= d/2;
		}				
				
		// reset coord-system
		double c_ymin = d3gl_win->get_ymin();
		double c_ymax = d3gl_win->get_ymax();
		if(c_ymax-c_ymin<INF_SIZE){
			double d = INF_SIZE-(c_ymax-c_ymin);
			c_ymax += d/2;
			c_ymin -= d/2;
		}
		d3gl_win->set_coord(plane_xmin,plane_xmax,c_ymin,c_ymax,plane_zmin,plane_zmax,false);

		// reset grid-distances
		/*
		if(d3gl_win->get_x_grid_dist()>INF_GRID_DIST)
			d3gl_win->set_x_grid_dist(INF_GRID_DIST);
			
		if(d3gl_win->get_y_grid_dist()>INF_GRID_DIST)
			d3gl_win->set_y_grid_dist(INF_GRID_DIST);

		if(d3gl_win->get_z_grid_dist()>INF_GRID_DIST)
			d3gl_win->set_z_grid_dist(INF_GRID_DIST);
		*/

		// disable box-button
		window* w = d3gl_win->get_menu_win();
		w->disable_button(5);
		if(temp){
			temp->disable_button(5);
			w2->redraw();
		}
		else{
			w->redraw();
		}
	}
	else{
		int result = quit_infinity_dialog();
				
		// restore box params				
		texturing_box = inf_backup[0];
		box_outlines  = inf_backup[1]; 
		box_inlines   = inf_backup[2];
		box_filling   = inf_backup[3];			

		if(result){
			d3gl_win->set_xmin(inf_backup[4]);
		    d3gl_win->set_xmax(inf_backup[5]);
		    d3gl_win->set_ymin(inf_backup[6]);
		    d3gl_win->set_ymax(inf_backup[7]);
		    d3gl_win->set_zmin(inf_backup[8]);
		    d3gl_win->set_zmax(inf_backup[9]);

			plane_xmin   = inf_backup[10];
		    plane_xmax   = inf_backup[11];
		    plane_zmin   = inf_backup[12];
		    plane_zmax   = inf_backup[13];
		    plane_height = inf_backup[14];

			d3gl_win->set_x_grid_dist(inf_backup[15]);
		    d3gl_win->set_y_grid_dist(inf_backup[16]);
		    d3gl_win->set_z_grid_dist(inf_backup[17]);

			box_xmin = inf_backup[18];
		    box_xmax = inf_backup[19];
		    box_ymin = inf_backup[20];
		    box_ymax = inf_backup[21];
		    box_zmin = inf_backup[22];
		    box_zmax = inf_backup[23];

			eye_pos = cam.get_eye();
			double x,y,z;

			if(eye_pos[0] > inf_backup[5] || eye_pos[0] < inf_backup[4])
				x = inf_backup[4] + (inf_backup[5] - inf_backup[4])/2;
			else
				x = eye_pos[0];

			if(eye_pos[1] > inf_backup[7] || eye_pos[1] < inf_backup[6])
				y = inf_backup[6] + (inf_backup[7] - inf_backup[6])/2;
			else
				y = eye_pos[1];

			if(eye_pos[2] > inf_backup[9] || eye_pos[2] < inf_backup[8])
				z = inf_backup[8] + (inf_backup[9] - inf_backup[8])/2;
			else
				z = eye_pos[2];

			cam.set_eye(x,y,z);					
					
		}else{
			box_xmin = d3gl_win->get_xmin();
		    box_xmax = d3gl_win->get_xmax();
		    box_ymin = d3gl_win->get_ymin();
		    box_ymax = d3gl_win->get_ymax();
		    box_zmin = d3gl_win->get_zmin();
		    box_zmax = d3gl_win->get_zmax();
		}

		delete[] inf_backup;
		inf_backup = NULL;

		// enable box-button
		window* w = d3gl_win->get_menu_win();
		w->enable_button(5);
		if(temp){
			temp->enable_button(5);
			w2->redraw();
		}
		else{
			w->redraw();
		}				
	}

	if(with_status_win && w2 == NULL)
		set_status_string1(camera_on);
}


void setup_box_or_plane(const int& val)
{
	string* strs;
	int*    ints;
	color*  cols;

	if(val == 5){
		strs = new string[6];
		strs[0] = int_to_string(((int)box_xmin));
		strs[1] = int_to_string(((int)box_xmax));
		strs[2] = int_to_string(((int)box_ymin));
		strs[3] = int_to_string(((int)box_ymax));
		strs[4] = int_to_string(((int)box_zmin));
		strs[5] = int_to_string(((int)box_zmax));					
					
		ints = new int[4];
		ints[0] = texturing_box;
		ints[1] = box_outlines;
		ints[2] = box_inlines;
		ints[3] = box_filling;
					
		cols = new color[2];
		cols[0] = box_line_color.get_color();
		cols[1] = box_fill_color.get_color();
	}
	else{
		strs = new string[5];
		strs[0] = int_to_string(((int)plane_xmin));
		strs[1] = int_to_string(((int)plane_xmax));					
		strs[2] = int_to_string(((int)plane_zmin));
		strs[3] = int_to_string(((int)plane_zmax));
		strs[4] = int_to_string(((int)plane_height));

		ints = new int[5];
		ints[0] = texturing_plane;
		ints[1] = plane_outlines;
		ints[2] = plane_x_parallels;
		ints[3] = plane_z_parallels;
		ints[4] = plane_filling;

		cols = new color[2];
		cols[0] = plane_line_color.get_color();
		cols[1] = plane_line_color.get_color();
	}				

	do{ box_or_plane_dialog(strs,ints,cols,val); }
	while(P3 != NULL);
				
	delete[] strs;
	delete[] ints;
	delete[] cols;								
}


int main() 
{
	#if defined(__win32__)
		sep = "\\";	
	#elif defined(__unix__)
		sep = "/";
    #else
		sep = "/";
	#endif	

	path = get_directory();   // path for objext textures
	txt_files_path = path+sep+"obj";
	path += sep+"bmp_pics";	 
	bg_path = path;           // path for background textures	

	// init variables for creating the background-box
	for(int i=0; i<6; i++){
		T[i] = NULL;		
	}        
    box_line_width = plane_line_width = 3;
    box_line_color = plane_line_color = d3_gl_col(0.0,0.0,0.0,1.0);
	box_fill_color = plane_fill_color = d3_gl_col(1.0,1.0,1.0,1.0);					
	
	menu options;		
	options.button("directories",3);	
	options.button("control",4);
	
	menu generators;
	generators.button("randomizer",0);
	generators.button("text-file",9);

	// create window	
	window w(BASE_WINDOW::screen_width(),BASE_WINDOW::screen_height()-80);
	w.button("options",options);
	w.button("help"        , 8);
	w.button("coord"       , 1);
	w.button("plane"       , 2);
	w.button("box"         , 5);
	w.button("gen", generators);		
	w.button("exit"        , 7);

	gl_win = new gl_window(BASE_WINDOW::screen_width(),BASE_WINDOW::screen_height()-80);
	gl_win->display(window::center,0);	
	
	w.display(0,0,*gl_win);
	w.resize(0,0,gl_win->width(),w.get_panel_height());

	d3gl_win = new d3_gl_window(*gl_win,cam,&w);
	d3gl_win->set_coord(MIN,MAX,MIN,MAX,MIN,MAX,false);	

	box_xmin = MIN; box_xmax = MAX;
	box_ymin = MIN; box_ymax = MAX;	
	box_zmin = MIN; box_zmax = MAX;

	d3gl_win->set_x_grid_dist(MAX/4);
	d3gl_win->set_y_grid_dist(MAX/4);
	d3gl_win->set_z_grid_dist(MAX/4);

	plane_xmin = MIN; plane_xmax = MAX;
	plane_zmin = MIN; plane_zmax = MAX;
	plane_height = 0;

	d3gl_win->set_draw_axis(true);

	gl_win  = d3gl_win->get_gl_win();	
	
	d3gl_win->append_draw(draw_box);	
	d3gl_win->append_draw(draw_plane);

	// front
	if(load_tex(0,"bmp_pics/try1.bmp",BG_SIZE,BG_SIZE))	
		reg_tex(0,GL_LINEAR);
			
	// back
	if(load_tex(1,"bmp_pics/try1.bmp",BG_SIZE,BG_SIZE))			
		reg_tex(1,GL_LINEAR);

	// left
	if(load_tex(2,"bmp_pics/try1.bmp",BG_SIZE,BG_SIZE))	
		reg_tex(2,GL_LINEAR);

	// right
	if(load_tex(3,"bmp_pics/try1.bmp",BG_SIZE,BG_SIZE))		
		reg_tex(3,GL_LINEAR);

	// top
	if(load_tex(4,"bmp_pics/try1.bmp",BG_SIZE,BG_SIZE))	
		reg_tex(4,GL_LINEAR);

	// bottom
	if(load_tex(5,"bmp_pics/try1.bmp",BG_SIZE,BG_SIZE))		
		reg_tex(5,GL_LINEAR);

	// plane texture
	if(load_tex(6,"bmp_pics/fracblue.bmp",(MAX-MIN)/4,(MAX-MIN)/4))		
		reg_tex(6,GL_LINEAR);

	int val = 0;
	double x_new = 0.0, y_new = 0.0;
	double x_old = 0.0, y_old = 0.0;		

	double* first_trans  = NULL;
	double* rotation     = NULL;
	double* second_trans = NULL;
	sc_model m;

	double m_sens  = deg_to_rad(0.25);   // mouse sensitivity
	double m_speed = MAX/200;            // mouse speed 
	double k_sens  = 4*m_sens;           // key sensitivity
	double k_speed = MAX/200;            // key speed 				

	bool   left_button   = false;
	bool   right_button  = false;	
	bool   rotating   = true;	 	

	d3gl_win->open_status_window(ivory,20);																												
	d3gl_win->set_status_string("\\bf CAMERA : \\blue ON \\black  INFINITY : \\blue OFF \\c ");	
    
	d3_gl_scene* sc_ptr = NULL;	

	cam.set(25000.0,6000.0,25000.0, 0.0,6000.0,0.0, 0.0,1.0,0.0);	

	d3gl_win->draw(); 
	
	while(true)
	{ 
	  d3_transform T; 	  
      int e = d3gl_win->get_event(val,x_new,y_new);	  

	  if(e == key_press_event)
	  { 
	    if(val == 2) {       // ESC
			delete_heap();			
			return 0;
		} 
		else if(val == 12){ // F1
			with_status_win = ! with_status_win;
			if(with_status_win){
				d3gl_win->open_status_window(ivory,20);	
				if(w2)
					set_status_string2(mode,true);				
				else 
					set_status_string1(camera_on);				
			}
			else{				
				d3gl_win->close_status_window();
			}
		}
		else if(val == 13){ // F2
			camera_on = !camera_on;
			if(with_status_win && w2 == NULL)
				set_status_string1(camera_on);
		}
		else if(val == 14){ // F3			
			vector3d u = cam.get_u();
			vector3d v = cam.get_v();			
			if(v[1]<0)
				cam.set(vector3d(u[0],0,u[2]),vector3d(0,-1,0),vector3d(u[2],0,-u[0]));
			else
				cam.set(vector3d(u[0],0,u[2]),vector3d(0,1,0),vector3d(-u[2],0,u[0]));
		}
		else if(val == 15){ // F4			
			reset_camera();
		}
		else if(val == 16){ // F5
			swap_infinity();
		}		

		else if(val == 100)
		{			
		  if(sc_ptr){
			  delete[] m.second();
			  delete[] m.third();
			  delete   m.fourth();
			  models.remove(m);

			  d3gl_win->remove_scene(sc_ptr);
			  delete sc_ptr;

			  sc_ptr = NULL;
			  first_trans = NULL;
			  rotation = NULL;
			  second_trans = NULL;							
			  
			  d3gl_win->set_menu_win(&w); 
			  
			  delete temp;
			  delete w2;
			  
			  temp = NULL;
			  w2 = NULL;			  

			  mode = 10;			  
		  }		  
		}		
		
		else if(val == 99){ // C		  
		  if(w2){
			  if(gl_win->alt_key_down()){				  
				  mode = 10;
				  set_status_string2(mode,true);
			  }
		  }
		}

		else if(val == 109){ // M		  
		  if(w2){			  
			  if(gl_win->alt_key_down()){				  
				  mode = 11;
				  set_status_string2(mode,true);				  
			  }
		  }			  
		}

		else if(val == 114){ // R		
		  if(w2){
			  if(gl_win->alt_key_down()){					  
				  mode = 12;	
				  set_status_string2(mode,true);
			  }
		  }			  
		}

		else if(val == 115){ // S
		  if(w2){
			  if(gl_win->alt_key_down()){				  
				  mode = 13;
				  set_status_string2(mode,true);
			  }
		  }		
		}


		switch (val) 
		{ 		  
		 case 43 : { // +
			 if(mode == 10){					
				m_speed = m_speed * 1.2;
				m_sens  = m_sens  * 1.2;
					
				k_speed = k_speed * 1.2;
				k_sens  = k_sens  * 1.2; 					
			 }			 

             break;
				  }

		 case 45 : { // -
			 if(mode == 10){					
				m_speed = m_speed * 0.8;
				m_sens  = m_sens  * 0.8;
					
				k_speed = k_speed * 0.8;
				k_sens  = k_sens  * 0.8; 					
			 }			 

             break;
				  }

		 case 50 :{ // 2
			 if(mode == 10){
				  cam.slide(0,0,k_speed);				  
				  if(infinity){
					  update_camera_action();
				  }					  
			 }
              
			  break;
				   }

		 case 56 :{ // 8			 
			 if(mode == 10){
				  cam.slide(0,0,-k_speed);
				  if(infinity){
					  update_camera_action();
				  }
			 }              

			  break;               
				   }

		 case 55 :{ // 7
			  if(mode == 10){
				  cam.slide(0,k_speed,0);
				  if(infinity){
					  update_camera_action();
				  }
			  }
			  break;
				   }

		 case 49 :{ // 1
			  if(mode == 10){
				  cam.slide(0,-k_speed,0);
				  if(infinity){
					  update_camera_action();
				  }
			  }
			  break;        
				   }

          case 48 : { cam.slide(-k_speed,0,0); break; }
          case 46 : { cam.slide(k_speed,0,0);  break; }
                
          case 54 :{ // 6
			  if(mode == 10)
				  cam.yaw(-k_sens);            

			  break;
				   }

          case 52 : { // 4
			  if(mode == 10)
				  cam.yaw(k_sens);              

			  break;
					}

          case 57 : { cam.pitch(-k_sens); break;  }
          case 51 : { cam.pitch(k_sens);  break;  }
          case 47 : { cam.roll(k_sens);   break;  }
          case 42 : { cam.roll(-k_sens);  break;  }

          case 3  :{ // left arrow key
					T.rotate(k_sens,0,1,0);
                    cam.set_eye(T(cam.get_eye()));                                    
                    cam.rotate(k_sens,0,1,0); 

					if(infinity){
						update_camera_action();
					}
					
                    break;
				   }
                  
          case 4  :{ // right arrow key
					T.rotate(-k_sens,0,1,0);
                    cam.set_eye(T(cam.get_eye()));
                    cam.rotate(-k_sens,0,1,0); 

					if(infinity){
						update_camera_action();
					}

                    break;        
				   }          		  
		}			
	  }
	  else if(e == button_press_event)
	  {		  
		  switch(val)
		  { 
		    case MOUSE_BUTTON(1) :{ 									
				if(gl_win->ctrl_key_down()){
					d3_gl_scene* picked_sc = d3gl_win->pick_object(x_new,y_new);
					
					if(picked_sc == sc_ptr){
						if(picked_sc != NULL){
							sc_ptr->set_draw_obj_border(false);							
							sc_ptr = NULL;

							first_trans = NULL;
							rotation = NULL;
							second_trans = NULL;							
							
							d3gl_win->set_menu_win(&w);
							delete temp;
							delete w2;
							w2 = NULL;
							temp = NULL;

							mode = 10;
							
							if(with_status_win){
								d3gl_win->open_status_window(ivory,20);																												
								set_status_string1(camera_on);
							}
						}
					}
					else{					
						if( (sc_ptr) != NULL) (sc_ptr)->set_draw_obj_border(false);
						sc_ptr = picked_sc;
						if(sc_ptr != NULL){
							sc_ptr->set_draw_obj_border(true); 							

							first_trans  = sc_ptr->get_seq_trans_a(0,0);
							rotation     = sc_ptr->get_seq_trans_a(0,1);
							second_trans = sc_ptr->get_seq_trans_a(0,2);														
							if(rotation[1] == 0) rotating = false;
							
							forall(m,models){
								if(m.first() == sc_ptr)
									break;
							}

							temp = new menu;
							temp->button("help"  ,8);
							temp->button("gen"   ,0);
							temp->button("plane" ,2);
							temp->button("box"   ,5);
							temp->button("exit"  ,7);

							if(infinity)
								temp->disable_button(5);							
							
							w2 = new window(d3gl_win->get_width(),d3gl_win->get_panel_height(),
								"test window");							

							w2->button("&camera",10,update_mouse_mode);
							w2->button("&moving",11,update_mouse_mode);
							w2->button("&rotating",12,update_mouse_mode);
							w2->button("&scaling",13,update_mouse_mode);
							w2->button("gen",generators);
							w2->button("options",options);
							w2->button("menu",*temp);																					
													
							d3gl_win->set_menu_win(w2);
							
							mode = 10;

							if(with_status_win){
								d3gl_win->open_status_window(ivory,20);																												
								set_status_string2(mode,true);
							}																			
						}
					}
				}
				else{				
					if(mode == 10 || mode == 11 || mode == 13)  // camera, moving, scaling ??
						left_button = true;
					else if(mode == 12){     // rotating ??							
						if(!rotating)
							rotation[1] = m.third()[3] / FRAME_RATE;
						m.second()[0] = 1;
						rotating = true;																	
					}										
				}

				break;
								  }
            
			case MOUSE_BUTTON(3) :{									
					if(mode == 10 || mode == 11 || mode == 12 || mode == 13){ 
						if(gl_win->ctrl_key_down()){
							if(sc_ptr == NULL){
								d3_gl_scene* picked_sc = d3gl_win->pick_object(x_new,y_new);
								if(picked_sc)
									do_gen_dialog(picked_sc,true);
							}
							else{
								do_gen_dialog(sc_ptr,true); // rotation changes  
								
								// take new rotation parameters
								first_trans  = sc_ptr->get_seq_trans_a(0,0);
								rotation     = sc_ptr->get_seq_trans_a(0,1);
								second_trans = sc_ptr->get_seq_trans_a(0,2);														
								if(rotation[1] == 0) rotating = false;							
							}
						}
						else{
							if(mode == 12){								
								m.third()[3] = rotation[1] * FRAME_RATE;
								rotation[1] = 0;
								m.second()[0] = 0;
								rotating = false;												
							}
							else
								right_button = true;
						}
					}												
					
				    break;
								  }

			case 9 :{
				int result;
				do{ result = load_scene_from_file_dialog(); }
				while(result == 1);

				if(result == 2)
					do_gen_dialog(NULL,false);

				break;
					}
				
		    case 0 :{
				do_gen_dialog(NULL,true); 
				break;
					}

			case 1 :
			case 3 :{
				double xmin, xmax;
				double ymin, ymax;
				double zmin, zmax;
				d3gl_win->get_coord(xmin,xmax,ymin,ymax,zmin,zmax);

				string coord_str[6]; 
				coord_str[0] = int_to_string(((int)xmin));
				coord_str[1] = int_to_string(((int)xmax));
				coord_str[2] = int_to_string(((int)ymin));
				coord_str[3] = int_to_string(((int)ymax));
				coord_str[4] = int_to_string(((int)zmin));
				coord_str[5] = int_to_string(((int)zmax));				
				
				int i = -1, j = -1;
				
				do{ options_dialog(coord_str,i,j,val); }
				while(P3 != NULL);				

				break;
					}

            case 5 :
			case 2 : {				
				setup_box_or_plane(val);
				break;
					 }
				
			case 4 : {
				control_options(m_sens,m_speed,k_sens,k_speed);
				break;
					 }					

			case 8 : {
				help_dialog();
				break;
					 }		

			case 7 : { 
				delete_heap();				
				return 0;
					 }

			// second panel-bar :

			case 10 : {				
				w2->set_status_string("\\bf MODE : \\blue camera   \\black  LEFT : \\blue go ahead   \\black  RIGHT : \\blue go back \\c ");				
				mode = 10;
				break;
					  }

			case 11 : {				
				w2->set_status_string("\\bf MODE : \\blue moving   \\black  LEFT : \\blue go ahead   \\black  RIGHT : \\blue go back \\c ");				
				mode = 11;
				break;
					  }

			case 12 : {				
				w2->set_status_string("\\bf MODE : \\blue rotating   \\black  LEFT : \\blue start rotation   \\black  RIGHT : \\blue stop rotation \\c ");				
				mode = 12;
				break;
					  }

			case 13 : {				
				w2->set_status_string("\\bf MODE : \\blue scaling   \\black  LEFT : \\blue stretching   \\black  RIGHT : \\blue shrunking \\c ");				
				mode = 13;
				break;
					  }
				
		  }
	  }
      
	  else if(e == button_release_event){
		  left_button   = false;
		  right_button  = false;
	  }

	  
	  else{ 		  
		if(camera_on && mode == 10){
			if(y_new > 0.0 && y_old > 0.0){				
				if(y_new == y_old){
                   if(y_new < 3.0)
				  	    cam.pitch(-2*m_sens);
				   else if(y_new > gl_win->ymax()-gl_win->pix_to_real(w2?w2->get_panel_height():w.get_panel_height())-2.0)
					    cam.pitch(2*m_sens);				   
				}
				else
					cam.pitch((y_new-y_old)*m_sens);				
			}			 

			if(x_new > 0.0 && x_old > 0.0){				  
				if(x_new == x_old){
					if(x_new < 3.0)
		  				cam.yaw(2*m_sens);
					else if(x_new > gl_win->xmax()-3.0)
						cam.yaw(-2*m_sens);
				}
				else
					cam.yaw((x_old-x_new)*m_sens);
			}	
		}	  
		  
		else if(first_trans && mode == 11){

			if(x_new != 0.0 && x_old != 0.0){
				vector3d u = cam.get_u();				

				u[0] *= m_speed * (x_new - x_old);
				u[1] *= m_speed * (x_new - x_old);
				u[2] *= m_speed * (x_new - x_old);

				sc_ptr->translate(u[0],u[1],u[2],true,true);
				first_trans[0]  += u[0];
				first_trans[1]  += u[1];
				first_trans[2]  += u[2];
				second_trans[0] -= u[0];
				second_trans[1] -= u[1];
				second_trans[2] -= u[2];			  
			 }
			  
			if(y_new != 0.0 && y_old != 0.0){
				vector3d v = cam.get_v();				

				v[0] *= m_speed * (y_new - y_old);
				v[1] *= m_speed * (y_new - y_old);
				v[2] *= m_speed * (y_new - y_old);				

				sc_ptr->translate(v[0],v[1],v[2],true,true);
				first_trans[0]  += v[0];
				first_trans[1]  += v[1];
				first_trans[2]  += v[2];
				second_trans[0] -= v[0];
				second_trans[1] -= v[1];
				second_trans[2] -= v[2];			  
			}				  
			  
		}
		else if(rotating && mode == 12){                
				GLint viewport[4];
				GLdouble mvmatrix[16], projmatrix[16];

				gl_win->gl_start();
				glGetIntegerv(GL_VIEWPORT, viewport);				
				glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);
				sc_ptr->get_modelview_matrix(mvmatrix);
				
				GLdouble winx = 0, winy = 0, winz = 0;
				vector center = sc_ptr->get_center();				
				
				if( gluProject(center[0],center[1],center[2],
					mvmatrix,projmatrix,viewport,
					&winx,&winy,&winz) == GL_TRUE) { }								
				
				if(winx > 0 && winx < viewport[2] && winy > 0 && winy < viewport[3]){					
					winx = gl_win->pix_to_real((int)winx);
					winy = gl_win->pix_to_real((int)winy);

					double xmin = d3gl_win->get_xmin();
					double xmax = d3gl_win->get_xmax();
					double ymin = d3gl_win->get_ymin();
					double ymax = d3gl_win->get_ymax();					

					// MOUSE
					double x = (x_new - gl_win->xmin()) / (gl_win->xmax() - gl_win->xmin());
					double y = (y_new - gl_win->ymin()) / (gl_win->ymax() - gl_win->ymin());
					x = x*(xmax-xmin)+xmin;
					y = y*(ymax-ymin)+ymin;

					// CENTER
					winx = (winx - gl_win->xmin()) / (gl_win->xmax() - gl_win->xmin());
					winy = (winy - gl_win->ymin()) / (gl_win->ymax() - gl_win->ymin());
					winx = winx*(xmax-xmin)+xmin;
					winy = winy*(ymax-ymin)+ymin;
					
					x -= winx;
					y -= winy;				
														
					if(rotation){						
						rotation[2] += x;
						rotation[3] += y;
						rotation[2] /= 2;
						rotation[3] /= 2;						

						vector vec(x-winx,y-winy);
						double l = vec.length();
						double d = ((xmax - xmin) + (ymax - ymin)) / 2 ;						

						rotation[1] = (l/d)*(l/d)*12;						
						
						m.third()[3] = rotation[1] * FRAME_RATE;							
					}									  
			}
		}
	 }
	  

	  if(left_button){
		  if(mode == 10){
			  cam.slide(0.0,0.0,-m_speed);

			  if(infinity){
				  update_camera_action();
			  }
		  }
		  else if(mode == 11){
			  if(first_trans){					  
				  vector3d n = cam.get_n();

				  n[0] *= m_speed;
				  n[1] *= m_speed;
				  n[2] *= m_speed;

				  sc_ptr->translate(-n[0],-n[1],-n[2],true,true);
				  first_trans[0]  -= n[0];
				  first_trans[1]  -= n[1];
				  first_trans[2]  -= n[2];
				  second_trans[0] += n[0];
				  second_trans[1] += n[1];
				  second_trans[2] += n[2];					  			  
			  }
		  }
		  else if(mode == 13){
			  sc_ptr->direct_scale( 1.04,1.04,1.04, true );
		  }
	  }
	  else if(right_button){
		  if(mode == 10){
			  cam.slide(0.0,0.0, m_speed);

			  if(infinity){
				  update_camera_action();
			  }
		  }
		  else if(mode == 11){
			  if(first_trans){					  
				  vector3d n = cam.get_n();

				  n[0] *= m_speed;
				  n[1] *= m_speed;
				  n[2] *= m_speed;

				  sc_ptr->translate(n[0],n[1],n[2],true,true);
				  first_trans[0]  += n[0];
				  first_trans[1]  += n[1];
				  first_trans[2]  += n[2];
				  second_trans[0] -= n[0];
				  second_trans[1] -= n[1];
				  second_trans[2] -= n[2];					  			  
			  }
		  }
		  else if(mode == 13){
			  sc_ptr->direct_scale( 0.95,0.95,0.95, true );
		  }
	  }	  

	  y_old = y_new;
	  x_old = x_new;	  
		  
	  d3gl_win->draw(); 
	}		

	delete_heap();	
	return 0;
}
