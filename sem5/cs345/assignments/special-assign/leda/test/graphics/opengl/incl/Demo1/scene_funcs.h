// scenes, i.e. models, i.e. geometric objects, i.e. graphs

d3_gl_scene* create_scene(int* dia1_int , int& range , int& width, int& height, int& depth)
{		
	if(width<height){
		if(depth<height)
			range = height;
		else
			range = depth;
	}
	else{
		if(depth<width)
			range = width;
		else
			range = depth;
	}	

	list<d3_rat_point> PTS;
	int num_of_pts = string_to_int(d1_str[6]);

	switch(dia1_int[0]) // types
	{
	case 0 :{ 
		random_points_in_cube(num_of_pts,range/2,PTS);						
		break;
			}
	case 1 :{ 
		random_points_in_ball(num_of_pts,range/2,PTS);
		break;
			}
	case 2 :{ 
		random_points_in_square(num_of_pts,range/2,PTS);		
		break;
			}
	case 3 :{
		random_points_on_paraboloid(num_of_pts,range/2,PTS);
        break;
			}
	case 4 :{
		lattice_points(num_of_pts,range/2,PTS);
		break;
			}
	case 5 :{
		random_points_on_sphere(num_of_pts,range/2,PTS);
        break;
			}
	case 6 :{
		random_points_on_segment(num_of_pts,range/2,PTS);
        break;
			}
	}

	PARA_GRAPH* H = new PARA_GRAPH;
	
	if(dia1_int[8])
		D3_HULL(PTS,*H);
	else{
		d3_rat_point pt;
		forall(pt,PTS)
			H->new_node(pt);

		if(dia1_int[9]){		
			H->triangulate_map();			
		}
	}	

	H->compute_faces();			

	node_array<rat_vector> pos(*H);
	node v;
	forall_nodes(v,*H) pos[v] = (*H)[v].to_vector();				
	
	d3_gl_scene* scene = new d3_gl_scene(*H,pos);
	d3gl_win->append_scene(scene);	
	
	return scene;
}


void setup_scene(d3_gl_scene* scene, const int& width, const int& height, const int& depth,
				 int* dia1_int, int* dia2_int, color* dia1_col[3], const bool& do_trans = true)
{
	const graph* H = scene->get_graph();
	vector center  = scene->get_center();	
	
	double x_move,y_move,z_move;
	x_move = ((double)string_to_int(d1_str[0])) - center[0];
	y_move = ((double)string_to_int(d1_str[1])) - center[1];
	z_move = ((double)string_to_int(d1_str[2])) - center[2];		

	scene->direct_scale( ((double)width)/scene->get_width(),
		((double)height)/scene->get_height(), 
		scene->get_depth()==0?0:((double)depth)/scene->get_depth(), true );		

	if(do_trans)
		scene->direct_translate(x_move,y_move,z_move);	

	scene->set_draw_nodes( bool(dia1_int[3]) );
	scene->set_draw_edges( bool(dia1_int[4]) );
	scene->set_draw_faces( bool(dia1_int[5]) );
	scene->set_node_width( float(dia1_int[6]) );
	scene->set_edge_width( float(dia1_int[7]) );		
	
	scene->change_color(*dia1_col[0],H->first_node());	
	scene->change_color(*dia1_col[1],H->first_edge());	
	scene->change_color(*dia1_col[2],H->first_face());

	double xrot_axis,yrot_axis,zrot_axis;
	xrot_axis = (double) string_to_int(d2_str[0]);
	yrot_axis = (double) string_to_int(d2_str[1]);
	zrot_axis = (double) string_to_int(d2_str[2]);

	double xrot_pt,yrot_pt,zrot_pt;
	xrot_pt = (double) string_to_int(d2_str[3]);
	yrot_pt = (double) string_to_int(d2_str[4]);
	zrot_pt = (double) string_to_int(d2_str[5]);

	double rot_angle = (double) string_to_int(d2_str[6]);
	
	if(dia2_int[1]){ // middle point as rotation point ??
		vector v = scene->get_center(); 
		xrot_pt = v[0];
		yrot_pt = v[1];
		zrot_pt = v[2];
	}
				
	scene->begin_sequence();

	scene->seq_translate(xrot_pt,yrot_pt,zrot_pt,0.0,0.0,0.0);			
	scene->seq_rotate(0.0,dia2_int[0]?rot_angle/FRAME_RATE:0.0,
		xrot_axis,yrot_axis,zrot_axis);
	scene->seq_translate(-xrot_pt,-yrot_pt,-zrot_pt,0.0,0.0,0.0);

	scene->end_sequence();										

	sc_model m;
	forall(m,models){
		if(m.first() == scene){

			int*& model_info = m.second();
			model_info[0] = dia2_int[0]; // rotation
			model_info[1] = dia2_int[1]; // middle point
			if(!model_info[2])
				model_info[2] = dia1_int[8]; // convex hull
			if(!model_info[3])
				model_info[3] = dia1_int[9]; // triangulated 

			double*& rot_info = m.third();
			rot_info[0] = xrot_pt;
			rot_info[1] = yrot_pt;
			rot_info[2] = zrot_pt;
			rot_info[3] = rot_angle;
			
			*(m.fourth()) = d1_str[7];

			return;
		}				
	}
	
	int* model_info = new int[4];		
	model_info[0] = dia2_int[0]; // rotation
	model_info[1] = dia2_int[1]; // middle point		
	model_info[2] = dia1_int[8]; // convex hull
	model_info[3] = dia1_int[9]; // triangulated    

	double* rot_info = new double[4];
	rot_info[0] = xrot_pt;
	rot_info[1] = yrot_pt;
	rot_info[2] = zrot_pt;
	rot_info[3] = rot_angle;
	models.append(sc_model(scene, model_info, rot_info, new string(d1_str[7])));
}

bool generate_model(int* dia1_int, color* dia1_col[3], int* dia2_int, const bool& randomizer)
{		
	int range, width, height, depth; 

	width  = string_to_int(d1_str[3]);
	height = string_to_int(d1_str[4]);
	depth  = string_to_int(d1_str[5]);
	
	if(width == 0 || height == 0 || (depth == 0 && dia1_int[0] != 2) ){
		show_short_message("Width, height or depth or all are 0. This might cause unexpected results.",
			"WARNING");
	}
	
	d3_gl_scene* scene;	

	if(randomizer){
		scene = create_scene(dia1_int, range ,width, height, depth);
		if(scene == NULL) return false;
	}
	else{
		int result;
		do{ result = load_scene_from_file(scene,dia1_int[8],dia1_int[9]); }
		while(result == 1);
		if(result == 0)	return false;		
	}	
	
	scene->clear_lists();
	scene->set_color(*dia1_col[0],scene->get_graph()->all_nodes(),false);	
	scene->set_color(*dia1_col[1],scene->get_graph()->all_edges(),false);	
	scene->set_color(*dia1_col[2],scene->get_graph()->all_faces(),false);	

	scene->clear_matrix();	

	setup_scene(scene, width, height, depth, dia1_int, dia2_int, dia1_col);		
	
	scene->set_texturing(false);
	if(dia1_int[1]){ // texturing ??
		string s = path+sep+d1_str[7]; //dia1_str[7] == tex_file
		if(!is_file(s)){
			P1->acknowledge(ERR_MSG2);
			d3gl_win->remove_scene(scene);
			delete scene;
			return false;
		}
		else{													
			// GLuint tex_num;
			int filter;
			if(dia1_int[2]) filter = GL_LINEAR;
			else            filter = GL_NEAREST; 			
							
			if(!load_and_reg_scene_tex(s,filter,scene,(scene->get_graph())->all_faces())){
				P1->acknowledge(ERR_MSG1);
				d3gl_win->remove_scene(scene);
				delete scene;
				return false;
			}			

			scene->set_texturing(true);			
		}		
	}	
	
	return true;
}


bool change_model(d3_gl_scene* sc,int* dia1_int, color* dia1_col[3],int* dia2_int)
{	
	sc->remove_sequence(0);

	// sc->clear_matrix();

	// vector v = sc->get_center();
	// sc->direct_translate(-v[0],-v[1],-v[2]);

	int width  = string_to_int(d1_str[3]);
	int height = string_to_int(d1_str[4]);
	int depth  = string_to_int(d1_str[5]);	

	setup_scene(sc, width, height, depth, dia1_int, dia2_int, dia1_col, false);		

	if(dia1_int[8]){ // convex hull
		
		node_array<vector>* pos = sc->get_node_array();
		graph* G = sc->get_graph();
				
		int tex_num  = sc->get_texture(G->first_face());
		
		list<d3_rat_point> PTS;				
	    node n;
		forall_nodes(n,*G){
			vector v = (*pos)[n];
			rational x(v[0]);
			rational y(v[1]);
			rational z(v[2]);
			PTS.append(d3_rat_point(x,y,z));	
		}
		
		PARA_GRAPH* G2 = new PARA_GRAPH;	
		D3_HULL(PTS,*G2);
		G2->compute_faces();			
		
		node_array<rat_vector> pos2(*G2);		
		forall_nodes(n,*G2)
			pos2[n] = (*G2)[n].to_vector();
			
		sc->redefine_scene(*G2,pos2);
		
		sc->set_color(*dia1_col[0],G2->all_nodes(),false);
		sc->set_color(*dia1_col[1],G2->all_edges(),false);
		sc->set_color(*dia1_col[2],G2->all_faces(),false);
		
		if(tex_num != -1)
			sc->set_texture(tex_num,G2->all_faces());				

		delete (PARA_GRAPH*)G;		
	}
	else if(dia1_int[9]){ // triangulation						
		graph* G = sc->get_graph();
		int tex_num  = sc->get_texture(G->first_face());
		G->triangulate_map();
		G->compute_faces();	
		
		sc->clear_lists();
		sc->set_color(*dia1_col[0],G->all_nodes(),false);	
		sc->set_color(*dia1_col[1],G->all_edges(),false);	
		sc->set_color(*dia1_col[2],G->all_faces(),false);
		
		if(tex_num != -1)
			sc->set_texture(tex_num,G->all_faces());
	}
	
	// texturing ??
	if(dia1_int[1]){
		string s = path+sep+d1_str[7]; //dia1_str[7] == tex_file
		if(!is_file(s)){
			P1->acknowledge(ERR_MSG2);			
			return false;
		}
		else{																
			int filter;
			if(dia1_int[2]) filter = GL_LINEAR;
			else            filter = GL_NEAREST;			

			int tex_name = sc->get_texture((sc->get_graph())->first_face());
			if(tex_name != -1)
				delete_and_unreg_scene_tex(tex_name,sc);				
							
			if(!load_and_reg_scene_tex(s,filter,sc,(sc->get_graph())->all_faces())){
				P1->acknowledge(ERR_MSG1);				
				return false;
			}			
			  
			sc->set_texturing(true);
		}		
	}
	else
		sc->set_texturing(false);

	return true;
}
