// some dialogs und sub-dialogs

void explain_standard_format()
{
	panel P("Standard Format");	
	P.set_panel_bg_color(ivory);	
	
	P.text_item("\\bf\\blue Standard Format");
	P.text_item("");
	P.text_item("\\n ");
	P.text_item("Text-files using the standard-format consist of two parts.");	
	P.text_item("The first part contains the vertices, whereby you have to note");
	P.text_item(" two things : \\n a) a line must contain only one vertex");
	P.text_item("\\n b) a line must start with \"v\"");
	P.text_item("");
	P.text_item("\\n ");
	P.text_item("The second part contains the faces, that are defined by three");
	P.text_item(" or more vertices. You have to note two things as well :");
	P.text_item("\\n a) a line must start with \"f\"");
	P.text_item("\\n b) a line must contain at least three numbers of vertices");
	P.text_item("");
	P.text_item("\\n ");
	P.text_item("A text-file describing a cube in the standard-format might");
	P.text_item("look as follows :");
	P.text_item("");	
	P.text_item("\\n v -1.0 -1.0  1.0 ");
	P.text_item("\\n v -1.0  1.0  1.0 ");
	P.text_item("\\n v  1.0  1.0  1.0 ");
	P.text_item("\\n v  1.0 -1.0  1.0 ");
	P.text_item("\\n v -1.0 -1.0 -1.0 ");
	P.text_item("\\n v -1.0  1.0 -1.0 ");
	P.text_item("\\n v  1.0  1.0 -1.0 ");
	P.text_item("\\n v  1.0 -1.0 -1.0 ");
	P.text_item("\\n f  1 2 3 4 ");
	P.text_item("\\n f  5 6 7 8 ");
	P.text_item("\\n f  1 2 5 6 ");
	P.text_item("\\n f  3 4 7 8 ");
	P.text_item("\\n f  2 3 6 7 ");
	P.text_item("\\n f  1 4 5 8 ");
	P.text_item("");
	P.text_item("\\n ");

	P.button("ok",7);
	P.open();
	P.close();
}

void explain_off_format()
{
	panel P("OFF Format");	
	P.set_panel_bg_color(ivory);	
	
	P.text_item("\\bf\\blue OFF Format");
	P.text_item("");
	P.text_item("\\n ");
	P.text_item("The OFF-format is a quite common format for encoding geometric objects ");
	P.text_item("in computer graphics. If you want to get to know more about it you can ");
	P.text_item("use the internet.");
	P.text_item("");
	P.text_item("\\n ");
	P.text_item("Nevertheless here is a little example of an OFF-file that encodes a cube : ");
	P.text_item("");
	P.text_item("\\n OFF");
	P.text_item("\\n 8 6 0");
	P.text_item("\\n -1.0 -1.0  1.0 ");
	P.text_item("\\n -1.0  1.0  1.0 ");
	P.text_item("\\n  1.0  1.0  1.0 ");
	P.text_item("\\n  1.0 -1.0  1.0 ");
	P.text_item("\\n -1.0 -1.0 -1.0 ");
	P.text_item("\\n -1.0  1.0 -1.0 ");
	P.text_item("\\n  1.0  1.0 -1.0 ");
	P.text_item("\\n  1.0 -1.0 -1.0 ");
	P.text_item("\\n  4 1 2 3 4 ");
	P.text_item("\\n  4 5 6 7 8 ");
	P.text_item("\\n  4 1 2 5 6 ");
	P.text_item("\\n  4 3 4 7 8 ");
	P.text_item("\\n  4 2 3 6 7 ");
	P.text_item("\\n  4 1 4 5 8 ");
	P.text_item("");
	P.text_item("\\n ");

	P.button("ok",7);
	P.open();
	P.close();
}

// return values :
// 0 -> real error
// 1 -> 'unreal' error -> new try
// 2 -> success 
int load_scene_from_file_dialog()
{
	int clicked = -1;			

	panel P("Load Options");	
	P.set_panel_bg_color(ivory);	
	
	P.text_item("\\bf\\blue Load Options ");
	P.text_item("");
	P.text_item("\\n ");
	P.text_item("1.) Firstly you have to select a text-file, that contains a geometric object in a special format.");	
	P.choice_item("",clicked,list<string>("select"));
	
	string path2 = txt_files_path;				
	for(int i=0; i<path2.length() ; i++)
	{	if( string(path2[i]) == "\\" ) 
			path2[i] = '/';
	}	
	 
	if(!is_file(path2+sep+filename))
		filename = "???";

	P.text_item("\\bf currently selected file : \\rm "+path2+"/"+filename);
	
	P.text_item("");
	P.text_item("\\n ");
	P.text_item("2.) Secondly you have to choose the file-format ");
	P.text_item(" depending on what format the file you selected has. You can either select ");
	P.text_item(" the standard-format or the OFF-format.");
	P.text_item("");
	P.text_item("\\n ");
	P.text_item("If you want to know more about one or both formats, click on ");
	P.text_item("the corresponding button.");		

	P.text_item("");
	P.text_item("\\n ");

	int clicked3 = -1;
	list<string> format_infos;
	format_infos.append("standard-format-info");
	format_infos.append("OFF-format-info");
	P.choice_item("format info",clicked3,format_infos);

	list<string> formats;
	formats.append("standard-format");
	formats.append("OFF-format");

	P.choice_item("select format",off_format,formats);	
	int clicked2 = off_format;	

	P.text_item("");
	P.text_item("\\n ");	

	P.button("ok",1);
	P.button("cancel",2);
	P.display(window::center,window::center);

	int val;
	double x,y;

	do{
		if(clicked != -1){			
			string fname(filename);
			string path2 = txt_files_path;

			dir_panel DP(fname,path2);
			DP.set_load_handler(dummy_handler);
			DP.set_pattern(file_pattern);
			if(DP.open()){
				file_pattern = DP.get_pattern();

				if(is_directory(path2))
					txt_files_path = path2;
				else{
					P.acknowledge(ERR_MSG7);
					return 1;
				}

				if(is_file(fname))
					filename = fname;
				else{
					P.acknowledge(ERR_MSG8);
					return 1;
				}
			}
			return 1;
		}
		else if(clicked2 != off_format)
			return 1;
		else if(clicked3 == 0){
			explain_standard_format();
			return 1;
		}
		else if(clicked3 == 1){
			explain_off_format();
			return 1;
		}

	}while(P.get_event(val,x,y) != button_press_event);

	if(val == 2) return 0;		

	if(!is_file(path2+sep+filename)){
		d1_str = new string[8];
		d1_str[7] = filename;
		P.acknowledge(ERR_MSG2);
		delete[] d1_str;		
		return 1;
	}	
	
	return 2;
}


void gen_dialog(bool& dialog_one, d3_gl_scene* sc, int dia1_int[],color* dia1_col[3],
				int dia2_int[], int& xpos, int& ypos, int& w, int& h,
				const bool& randomizer )
{ 
	panel P("Generate-Options");
	if(sc) P.set_frame_label("Setup Object");
	P1 = &P;
	P.set_panel_bg_color(ivory);
	P.text_item("\\n \\n ");
	P.button("1/2",   6);	
	P.button("2/2",   7);
	P.button("ok",    8);
	P.button("cancel",9);

	string min_str(MIN_COORD_STR);
	string max_str(MAX_COORD_STR);	

	list<string> no_yes;
	no_yes.append("no");
	no_yes.append("yes");	

	if(dialog_one){			

		if(randomizer && sc == NULL){
			P.text_item("\\bf\\blue Select Object Type :");
			P.text_item("\\n \\n ");
			list<string> type_list;
			type_list.append("cube");
			type_list.append("ball");
			type_list.append("square");
			type_list.append("para");
			type_list.append("mesh");
			type_list.append("sphere");
			type_list.append("line");
			P.choice_item("",dia1_int[0], type_list,update_type);
		}
		
		P.text_item("\\bf\\blue Select Object Pos And Size :");
		P.text_item("\\n \\n ");			
	
		P.string_item("x value of middle point ("+min_str+" to "+max_str+")", d1_str[0] ,update_x);
		P.string_item("y value of middle point ("+min_str+" to "+max_str+")", d1_str[1] ,update_y);
		P.string_item("z value of middle point ("+min_str+" to "+max_str+")", d1_str[2] ,update_z);	
	
		P.string_item("width" , d1_str[3], update_width);
		P.string_item("height", d1_str[4], update_height);
		P.string_item("depth" , d1_str[5], update_depth);			
		
		if(sc){
			if(dia1_int[8])					
				 P.choice_item("convex hull (already done)",dia1_int[8],no_yes);								
			else P.choice_item("convex hull",dia1_int[8],no_yes);

			if(dia1_int[9])
				 P.choice_item("triangulate (already done)",dia1_int[9],no_yes);								
			else P.choice_item("triangulate",dia1_int[9],no_yes);
		}
		else{
			P.choice_item("convex hull",dia1_int[8],no_yes);
			P.choice_item("triangulate",dia1_int[9],no_yes);
		}
	
		if(randomizer){
			if(sc == NULL){
				P.string_item("number of points", d1_str[6],update_num_pts);					
			}
		}	

		P.text_item("\\bf\\blue Select Object View : ");
		P.text_item("\\n \\n ");				
	
		P.choice_item("texturing", dia1_int[1] ,no_yes);

		list<string> texture_files;	
		load_files(texture_files,path,d1_str[7]);			
		P.string_item("select texture", d1_str[7],texture_files,5,update_obj_tex_file);				
		
		P.text_item("");
		list<string> details;
		details.append("low");  // (GL_NEAREST)
		details.append("high");	// (GL_LINEAR) 
		P.choice_item("Texture Detail", dia1_int[2],details);						

		P.text_item("\\n ");		
		P.choice_item("draw_nodes", dia1_int[3], no_yes );	
		P.choice_item("draw_edges", dia1_int[4], no_yes );	
		P.choice_item("draw_faces", dia1_int[5], no_yes );							
	
		P.color_item("nodes-color", *dia1_col[0]);	
		P.color_item("edges-color", *dia1_col[1]);	
		P.color_item("faces-color", *dia1_col[2]);			

		P.text_item("\\n ");  
		P.int_item("node_width (0 to 10)", dia1_int[6],0,10);
		P.int_item("edge_width (0 to 10)", dia1_int[7],0,10);			

		
		if(dia1_int[0] == 2){ // depth should not be adjustable if a square is selected
			d1_str[5] = "0";
			P.disable_item(P.get_item("depth"));
		}
		 					
		P.disable_button(6);
	}
	else{				
		P.text_item("\\bf\\blue Rotation :");
		P.text_item("\\n \\n ");	
	
		P.choice_item("", dia2_int[0], no_yes, update_rotation);
		P.text_item("\\bf Rotation axis : \\rm ");
		rot_items[0] = P.string_item("x (rotation-axis)",d2_str[0], update_x_rot);	
		rot_items[1] = P.string_item("y (rotation-axis)",d2_str[1], update_y_rot);	
		rot_items[2] = P.string_item("z (rotation-axis)",d2_str[2], update_z_rot);

		P.text_item("\\bf Rotation angle : ");
		rot_items[4] = P.string_item("",d2_str[6], update_rot_angle);		
		
		P.text_item("\\bf Rotation point : ");
		rot_items[3] = P.choice_item("rot-point == middle-point ??", dia2_int[1],no_yes,update_mp);
	
		rot_sub_items[0] = P.string_item("x value of rot point ("+min_str+" to "+max_str+")",
			d2_str[3], update_x_rot_pt);	

		rot_sub_items[1] = P.string_item("y value of rot point ("+min_str+" to "+max_str+")",
			d2_str[4], update_y_rot_pt);	

		rot_sub_items[2] = P.string_item("z value of rot point ("+min_str+" to "+max_str+")",
			d2_str[5], update_z_rot_pt);					
	
		if(!dia2_int[0]){
			for(int k=0; k<5 ; k++)
				P.disable_item(rot_items[k]);
		}
		if(!dia2_int[0] || dia2_int[1]){
			for(int j=0; j<3; j++)
				P.disable_item(rot_sub_items[j]);
		}
		
		P.disable_button(7);
	}
	
	if(xpos == -1)
		P.display(window::center,window::center);	
	else{
		P.display(xpos,ypos);
		ypos = ypos + h - P.get_panel_height();
		P.resize(xpos,ypos,w,P.get_panel_height());		
	}			

	int val = P.read_mouse();

	switch(val)
	{		 
		case 6 :
		case 7 :{			
			dialog_one = !dialog_one; 
			xpos = P.xpos(); ypos = P.ypos();
			w = P.width(); h = P.height(); break;
				}
		case 8 : {
			if(test_input(d1_str,8) && test_input(d2_str,7)){
				if(sc){
					int model_type = -1;
					sc_model m;
					forall(m,models){
						if(m.first() == sc)
							model_type = (int) m.second()[0];
					}
					
					if(change_model(sc,dia1_int,dia1_col,dia2_int)){
						P.close(); P1 = NULL; break;
					}					
					// P.close(); P1 = NULL; break;
				}
				else{
					if(generate_model(dia1_int,dia1_col,dia2_int,randomizer)){
						overtake_params(dia1_int,dia1_col,dia2_int);
						P.close(); P1 = NULL; break;						
					}					
				}
			}
			xpos = P.xpos(); ypos = P.ypos();
			w = P.width(); h = P.height(); break;
				 }
			
		case 9 : { P.close(); P1 = NULL;
			       if(sc == NULL) 
					  overtake_params(dia1_int,dia1_col,dia2_int); 			       
				   break;
				 }	
	}
}


void options_dialog(string coord_str[], int& xpos, int& ypos, const int& dialog_type)
{   
	panel P("Options");
	P3 = &P;
	P.set_panel_bg_color(ivory);

	double path_w = 0.0;
		
	int clicked  = 1; // path for background textures
	int clicked2 = 1; // path for object textures		

	int   draw_axis  = (int) d3gl_win->get_draw_axis();
	int   a_line_w   = (int) d3gl_win->get_axis_line_w();
	int   draw_coord = (int) d3gl_win->get_draw_coord();
	int   c_line_w   = (int) d3gl_win->get_coord_line_w();
	int   pt_size    = (int) d3gl_win->get_pt_size();
	int   draw_pts   = (int) d3gl_win->get_draw_pts();	

	d3_gl_col line_col = d3gl_win->get_axis_line_c();	
	color a_line_c = line_col.get_color();		
	double a_alpha = line_col.a();	

	line_col = d3gl_win->get_coord_line_c();	
	color  c_line_c = line_col.get_color();
	double c_alpha = line_col.a();	

	line_col = d3gl_win->get_pt_col();
	color pt_col = line_col.get_color();
	double p_alpha = line_col.a();
	
	line_col = d3gl_win->get_bg_color();
	color bg_col = line_col.get_color(); 

	int x_grid_dist = (int) d3gl_win->get_x_grid_dist();
	int y_grid_dist = (int) d3gl_win->get_y_grid_dist();
	int z_grid_dist = (int) d3gl_win->get_z_grid_dist();
	string grid_strs[3];
	grid_strs[0] = int_to_string(x_grid_dist);
	grid_strs[1] = int_to_string(y_grid_dist);
	grid_strs[2] = int_to_string(z_grid_dist);

	if(dialog_type == 1){		
		P.text_item("\\bf\\blue Set Coordinate-System :");
		P.text_item("");
		P.text_item("\\n ");

		if(infinity){
			coord_str[0] = "infinity";
			coord_str[1] = "infinity";
			coord_str[2] = "infinity";
			coord_str[3] = "infinity";
			coord_str[4] = "infinity";
			coord_str[5] = "infinity";

			panel_item items[6];
			items[0] = P.string_item("xmin",coord_str[0],update_min);
			items[1] = P.string_item("xmax",coord_str[1],update_max);
			items[2] = P.string_item("ymin",coord_str[2],update_min);
			items[3] = P.string_item("ymax",coord_str[3],update_max);
			items[4] = P.string_item("zmin",coord_str[4],update_min);
			items[5] = P.string_item("zmax",coord_str[5],update_max);

			for(int i=0; i<6; i++)
				P.disable_item(items[i]);
		}else{
			string min_str(MIN_COORD_STR);
			string max_str(MAX_COORD_STR);	

			P.string_item("xmin ("+min_str+" to "+max_str+")",coord_str[0],update_min);
			P.string_item("xmax ("+min_str+" to "+max_str+")",coord_str[1],update_max);
			P.string_item("ymin ("+min_str+" to "+max_str+")",coord_str[2],update_min);
			P.string_item("ymax ("+min_str+" to "+max_str+")",coord_str[3],update_max);
			P.string_item("zmin ("+min_str+" to "+max_str+")",coord_str[4],update_min);
			P.string_item("zmax ("+min_str+" to "+max_str+")",coord_str[5],update_max);
		}

		P.text_item("\\n ");
		
		P.string_item("x-grid-dist",grid_strs[0],update_max);
		P.string_item("y-grid-dist",grid_strs[1],update_max);
		P.string_item("z-grid-dist",grid_strs[2],update_max);			

		list<string> no_yes;
		no_yes.append("no");
		no_yes.append("yes");

		P.text_item("\\n ");
		
		P.choice_item("draw axis",draw_axis,no_yes);
		P.int_item("axis line width (0 to 10)",a_line_w,0,10);
		P.color_item("axis line color", a_line_c);
		// P.double_item("axis line alpha",a_alpha);

		P.text_item("\\n ");

		P.choice_item("draw coord",draw_coord,no_yes);
		P.int_item("coord line width (0 to 10)",c_line_w,0,10);
		P.color_item("coord line color", c_line_c);
		// P.double_item("coord line alpha",c_alpha);
		
		P.text_item("\\n ");

		P.choice_item("draw points",draw_pts,no_yes);
		P.int_item("point size (0 to 10)",pt_size,0,10);
		P.color_item("point color", pt_col);
		// P.double_item("point alpha",p_alpha);

		P.text_item("\\n ");		
		P.color_item("space color",bg_col);
		P.text_item("\\n ");

		P.button("cancel",24);
	}	
	else if(dialog_type == 3){
		P.text_item("\\bf\\blue Set Object Textures Directory :");
		P.text_item("\\n \\n ");			
		list<string> pathes;
		pathes.append("click");		
		P.choice_item("",clicked2,pathes);

		string path2 = path;				
		for(int i=0; i<path2.length() ; i++)
		{	if( string(path2[i]) == "\\" ) 
				path2[i] = '/';
		}		
		P.text_item("\\bf current path : \\rm "+path2);

		P.text_item("");
		P.text_item("\\n ");
		P.text_item("");

		P.text_item("\\bf\\blue Set Background Textures Directory :");
		P.text_item("\\n \\n ");				
		P.choice_item("",clicked,pathes);

		string path3 = bg_path;			
		for(int j=0; j<path3.length() ; j++)
		{	if( string(path3[j]) == "\\") 
		path3[j] = '/';
		}
		
		P.text_item("\\bf current path : \\rm "+path3);

		string temp = path2.length() < path3.length() ? path3 : path2;
		temp += "( \\bf current path : \\rm ";
		
		path_w = P.text_width(temp);
		path_w += 10;
	}
	
	P.button("ok",23);
	
	if(xpos!=-1)
		P.display(xpos,ypos);	
	else
		P.display(window::center,window::center);

	double win_w = (path_w > 0.0 && path_w > P.get_panel_width()) ? path_w : P.get_panel_width();	
	
	P.resize(P.xpos(),P.ypos(),(int)win_w,P.get_panel_height());

	int t2_old = texturing_box;
	int e,val;	 
	double xe,ye;

	do{		    
		if(t2_old != texturing_box){ 
			val = 0; break;
		}

		if(!clicked2){
			string fname("none");
			string path2 = path;

			dir_panel DP(fname,path2);
			DP.set_load_handler(dummy_handler);
			DP.set_pattern("*.bmp");
			if(DP.open())
				path = path2;	

			val = 0; break;
		}

		if(!clicked){			
			string fname("none");
			string path2 = bg_path;

			dir_panel DP(fname,path2);
			DP.set_load_handler(dummy_handler);
			DP.set_pattern("*.bmp");
			if(DP.open())
				bg_path = path2;
			
			val = 0; break;
		}			

		e = P.get_event(val,xe,ye);
	}while(e != button_press_event);	

	switch(val)
	{	
		case 0  :
		case 23 : {			
			double x0, x1, y0, y1, z0, z1;							

			if(val && (!test_input(coord_str,6) || !(test_input(grid_strs,3))) ){ 			
				val = 0; // wrong input -> do it again	
			}
			else{
				if(!infinity){
					x0 = (double) string_to_int(coord_str[0]);
					x1 = (double) string_to_int(coord_str[1]);
					y0 = (double) string_to_int(coord_str[2]);
					y1 = (double) string_to_int(coord_str[3]);
					z0 = (double) string_to_int(coord_str[4]);
					z1 = (double) string_to_int(coord_str[5]);
				}
			}			
			
			if(val){
				if(dialog_type == 1){
					if(!infinity){
						if(x0 > x1) { swap(x0,x1); }
						if(y0 > y1) { swap(y0,y1); }
						if(z0 > z1) { swap(z0,z1); }						
					}
					
					if(texturing_plane || plane_x_parallels || plane_z_parallels || plane_outlines || plane_filling)
						d3gl_win->set_coord(x0,x1,y0,y1,z0,z1,false);
					else
						d3gl_win->set_coord(x0,x1,y0,y1,z0,z1,true);
					d3gl_win->set_bg_color(bg_col);				
					
					if(a_alpha < 0.0) a_alpha = 0.0;
					if(a_alpha > 1.0) a_alpha = 1.0;
					if(c_alpha < 0.0) c_alpha = 0.0;
					if(c_alpha > 1.0) c_alpha = 1.0;					
					
					d3gl_win->set_draw_axis( (bool)(draw_axis) );
	                d3gl_win->set_axis_line_w( (float)(a_line_w) );
				    d3gl_win->set_axis_line_c(a_line_c,a_alpha);					
					
					d3gl_win->set_draw_coord( (bool)(draw_coord) );
					d3gl_win->set_coord_line_w( (float)(c_line_w) );
					d3gl_win->set_coord_line_c(c_line_c,c_alpha);

					d3gl_win->set_draw_pts( (bool)(draw_pts) );
					d3gl_win->set_pt_size( (float)(pt_size) );
					d3gl_win->set_pt_col(pt_col,p_alpha);
					
					x_grid_dist = string_to_int(grid_strs[0]);
					y_grid_dist = string_to_int(grid_strs[1]);
					z_grid_dist = string_to_int(grid_strs[2]);						

					if(x_grid_dist < x1)
						d3gl_win->set_x_grid_dist((double) x_grid_dist);
					if(y_grid_dist < y1)
						d3gl_win->set_y_grid_dist((double) y_grid_dist);
					if(z_grid_dist < z1)
						d3gl_win->set_z_grid_dist((double) z_grid_dist);					
				}				
			}
			else{				
				xpos = P.xpos();
				ypos = P.ypos();
				P.close();
				break;
			}			
				  }
		case 24 : { P.close(); P3 = NULL; P1 = NULL; break; }
	}	
}

void control_options(double& m_sens, double& m_speed,double& k_sens, double& k_speed)
{
	panel P("Control Options");
	P.set_panel_bg_color(ivory);

	P.text_item("\\bf\\blue Setup Mouse And Keyboard ");
	P.text_item("");
	P.text_item("\\n ");				

	P.double_item("mouse sensitivity",m_sens);
	P.double_item("mouse speed",m_speed);
	P.double_item("key sensitivity",k_sens);
	P.double_item("key speed",k_speed);

	P.button("ok",10);	
	
	P.open();
}

int quit_infinity_dialog()
{
	panel temp_panel;
	temp_panel.set_panel_bg_color(ivory);
	temp_panel.buttons_per_line(1);

	temp_panel.text_item("\\bf\\blue Quiting \"infinity-mode\"");				 
	temp_panel.text_item("");
	temp_panel.text_item("\\n ");		
	temp_panel.text_item("As you are quiting \"infinity-mode\", you have to decide how ");
	temp_panel.text_item("this programm reconverts to \"normal mode \".");
	temp_panel.text_item("\\n ");
	temp_panel.text_item("");
	temp_panel.text_item("\\n ");
	temp_panel.text_item("1.) Either you can overtake the current size of plane,");
	temp_panel.text_item("box and coordinate-system.");
	temp_panel.text_item("\\n ");
	temp_panel.text_item("");
	temp_panel.text_item("\\n ");
	temp_panel.text_item("2.) Or you can restore the size of plane, box and coordinate-system to");
	temp_panel.text_item(" the sizes they had before entering \"infinity-mode\", whereby ");
	temp_panel.text_item("objects you generated in \"infinity-mode\" could become out of view.");
	temp_panel.text_item("\\n ");
	temp_panel.text_item("");
	temp_panel.text_item("\\n ");
	temp_panel.text_item("If you are not sure what to do, it is recommended to ");
	temp_panel.text_item("select \"overtake current size\".");
	temp_panel.text_item("");
	temp_panel.text_item("\\n ");

	temp_panel.button("overtake current size",0);
	temp_panel.button("restore size"         ,1);

	return temp_panel.open();
}

void box_or_plane_dialog(string strs[], int ints[], color cols[], const int& what)
{
	int tex1 = -1;
	int tex2 = -1;
	int tex3 = -1;
	
	list<string> no_yes;
	no_yes.append("no");
	no_yes.append("yes");

	panel P("Options");
	P3 = &P;
	P1 = &P;
	P.set_panel_bg_color(ivory);

	string min_str(MIN_COORD_STR);
	string max_str(MAX_COORD_STR);	

	if(what == 5){
		P.text_item("\\bf\\blue Setup Box ");
		P.text_item("");
		P.text_item("\\n ");

		if(infinity){
			strs[0] = strs[1] = strs[2] = strs[3] = strs[4] =  strs[5] = "infinity";

			panel_item items[6];	
			items[0] = P.string_item("box-xmin ("+min_str+" to "+max_str+")",strs[0],update_min);
			items[1] = P.string_item("box-xmax ("+min_str+" to "+max_str+")",strs[1],update_max);
			items[2] = P.string_item("box-ymin ("+min_str+" to "+max_str+")",strs[2],update_min);
			items[3] = P.string_item("box-ymax ("+min_str+" to "+max_str+")",strs[3],update_max);
			items[4] = P.string_item("box-zmin ("+min_str+" to "+max_str+")",strs[4],update_min);
			items[5] = P.string_item("box-zmax ("+min_str+" to "+max_str+")",strs[5],update_max);

			for(int i=0; i<6; i++)
				P.disable_item(items[i]);
		}else{
			P.string_item("box-xmin ("+min_str+" to "+max_str+")",strs[0],update_min);
			P.string_item("box-xmax ("+min_str+" to "+max_str+")",strs[1],update_max);
			P.string_item("box-ymin ("+min_str+" to "+max_str+")",strs[2],update_min);
			P.string_item("box-ymax ("+min_str+" to "+max_str+")",strs[3],update_max);
			P.string_item("box-zmin ("+min_str+" to "+max_str+")",strs[4],update_min);
			P.string_item("box-zmax ("+min_str+" to "+max_str+")",strs[5],update_max);
		}

		P.text_item("\\n ");						
		P.choice_item("texturing",ints[0],no_yes);												
		P.text_item("\\n ");				
        
		list<string> surfaces1;
		surfaces1.append("front tex");
		surfaces1.append("back tex");
		list<string> surfaces2;
		surfaces2.append("left tex");
		surfaces2.append("right tex");
		list<string> surfaces3;
		surfaces3.append("top tex");
		surfaces3.append("bottom tex");
		
		P.choice_item("set textures",tex1,surfaces1);
		P.choice_item("",tex2,surfaces2);
		P.choice_item("",tex3,surfaces3);
		
		P.text_item("\\n ");
		P.choice_item("filling",ints[3],no_yes);
		P.color_item("fill color",cols[1]);

		P.text_item("\\n \\n ");		
		P.choice_item("draw box lines",ints[1],no_yes);		
		P.choice_item("draw coord lines",ints[2],no_yes);
		P.int_item("line width",box_line_width,0,10);
		P.color_item("line color",cols[0]);

		P.text_item("\\n ");
	}
	else if(what == 2){
		P.text_item("\\bf\\blue Setup Plane ");
		P.text_item("");
		P.text_item("\\n ");				

		if(infinity){
			strs[0] = strs[1] = strs[2] = strs[3] = strs[4] = "infinity";

			panel_item items[5];		
			items[0] = P.string_item("plane-xmin",strs[0],update_min);
			items[1] = P.string_item("plane-xmax",strs[1],update_max);
			items[2] = P.string_item("plane-zmin",strs[2],update_min);
			items[3] = P.string_item("plane-zmax",strs[3],update_max);
			items[4] = P.string_item("plane height",strs[4],update_min);

			for(int i=0; i<5; i++)
				P.disable_item(items[i]);
		}else{
			P.string_item("plane-xmin ("+min_str+" to "+max_str+")",strs[0],update_min);
			P.string_item("plane-xmax ("+min_str+" to "+max_str+")",strs[1],update_max);
			P.string_item("plane-zmin ("+min_str+" to "+max_str+")",strs[2],update_min);
			P.string_item("plane-zmax ("+min_str+" to "+max_str+")",strs[3],update_max);
			P.string_item("plane height ("+min_str+" to "+max_str+")",strs[4],update_min);
		}		
		
		P.text_item("\\n ");						
		P.choice_item("texturing",ints[0],no_yes);												
		P.text_item("\\n ");

		list<string> setup_texture;
		setup_texture.append("setup texture");
		P.choice_item("",tex3,setup_texture);
		
		P.text_item("\\n ");
		P.choice_item("filling",ints[4],no_yes);
		P.color_item("fill color",cols[1]);

		P.text_item("\\n \\n ");		
		P.choice_item("draw plane lines",ints[1],no_yes);		
		P.choice_item("draw x parallels",ints[2],no_yes);
		P.choice_item("draw z parallels",ints[3],no_yes);
		P.int_item("line width",plane_line_width,0,10);
		P.color_item("line color",cols[0]);		

		P.text_item("\\n ");
	}
	
	P.button("ok",23);
	P.button("cancel",24);

	P.display(window::center,window::center);

	int e,val;	 
	double xe,ye;

	do{	
		if(tex1 != -1 || tex2 != -1 || tex3 != -1){							
			if(tex2 != -1) tex1 = tex2+2;
			else if(tex3 != -1) tex1 = tex3+4;
			if(what == 2) tex1 += 2;
			
			string tex_height_str = int_to_string( ((int)tex_height[tex1]) );
			string tex_width_str  = int_to_string( ((int)tex_width[tex1])  );

			while(!set_bg_tex(tex1,tex_width_str,tex_height_str));		
			val = 0; break;		
		}		

		e = P.get_event(val,xe,ye);
	}while(e != button_press_event);
	

	if(val == 23 || val == 0){				
		if(val){						
			if(what == 5){
				if(!test_input(strs,6)) return;  // wrong input -> do it again							

				if(!infinity){
					box_xmin = (double) string_to_int(strs[0]);
					box_xmax = (double) string_to_int(strs[1]);
					box_ymin = (double) string_to_int(strs[2]);
					box_ymax = (double) string_to_int(strs[3]);
					box_zmin = (double) string_to_int(strs[4]);
					box_zmax = (double) string_to_int(strs[5]);

					if(box_xmin > box_xmax) { swap(box_xmin,box_xmax); }
					if(box_ymin > box_ymax) { swap(box_ymin,box_ymax); }
					if(box_zmin > box_zmax) { swap(box_zmin,box_zmax); }
				}

				texturing_box = ints[0];
				box_outlines  = ints[1];
				box_inlines   = ints[2];
				box_filling   = ints[3];

				box_line_color = d3_gl_col(cols[0]);
				box_fill_color = d3_gl_col(cols[1]);
			}
			else if(what == 2){
				if(!test_input(strs,5)) return;  // wrong input -> do it again

				if(!infinity){
					plane_xmin   = (double) string_to_int(strs[0]);
					plane_xmax   = (double) string_to_int(strs[1]);				
					plane_zmin   = (double) string_to_int(strs[2]);
					plane_zmax   = (double) string_to_int(strs[3]);
					plane_height = (double) string_to_int(strs[4]);

					if(plane_xmin > plane_xmax) { swap(plane_xmin,plane_xmax); }				
					if(plane_zmin > plane_zmax) { swap(plane_zmin,plane_zmax); }
				}

				texturing_plane   = ints[0];
				plane_outlines    = ints[1];
				plane_x_parallels = ints[2];
				plane_z_parallels = ints[3];
				plane_filling     = ints[4];

				plane_line_color = d3_gl_col(cols[0]);
				plane_fill_color = d3_gl_col(cols[1]);
			}			 										

			P.close();
			P1 = NULL; P3 = NULL;			
		}		
	}	
	else if(val == 24){ // cancel	
		P.close();
		P1 = NULL; P3 = NULL;
	}
}
