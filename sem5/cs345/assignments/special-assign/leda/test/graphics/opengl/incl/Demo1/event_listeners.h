// some event-listeners ; they are called when panel_items are pressed/used/changed

void update_to_int(char* str_ptr, const int& min, const int& max, string& target)
{
	if(str_ptr[0]=='\0'){
		target = "";
		return;
	}

	string str(str_ptr);	
	clean_string(str,false);		

	if(str.length()>0){
		if( !(str.length()==1 && str[0]=='-') ){			
			int pos_now = string_to_int(str);
			if(pos_now>max){
				target = int_to_string(max);			
			}		
			else if(pos_now<min){
				target = int_to_string(min);
			}
			else
				target = str;			
		}
		else
			target = str;
	}	
}


void update_helper(char* str_ptr, const int& min, const int& max)
{
	string original(str_ptr);
	string changed;
	update_to_int(str_ptr,min,max,changed);

	if(original != changed){		
		int i;
		for(i=0; i<changed.length(); i++)
			str_ptr[i] = changed[i];
	    str_ptr[i] = '\0';		

		if(P3) P3->redraw_panel();
	}
}


void update_min(char* str_ptr)
{ update_helper(str_ptr,MIN_COORD_SIZE,MAX_COORD_SIZE); }

void update_x(char* str_ptr)
{  
  update_helper(str_ptr,((int)d3gl_win->get_xmin()),((int)d3gl_win->get_xmax()));  
  if(P3 == NULL){
	d1_str[0] = str_ptr;
	P1->redraw_panel();
  }
 }

void update_y(char* str_ptr)
{   
  update_helper(str_ptr,((int)d3gl_win->get_ymin()),((int)d3gl_win->get_ymax()));
  if(P3 == NULL){
	d1_str[1] = str_ptr;
	P1->redraw_panel();
  }
 }

void update_z(char* str_ptr)
{
  update_helper(str_ptr,((int)d3gl_win->get_zmin()),((int)d3gl_win->get_zmax()));
  if(P3 == NULL){
	d1_str[2] = str_ptr;
	P1->redraw_panel();
  }
 }

void update_x_rot_pt(char* str_ptr)
{ update_helper(str_ptr,((int)d3gl_win->get_xmin()),((int)d3gl_win->get_xmax()));
  d2_str[3] = str_ptr;
  P1->redraw_panel();
 }

void update_y_rot_pt(char* str_ptr)
{ update_helper(str_ptr,((int)d3gl_win->get_ymin()),((int)d3gl_win->get_ymax()));
  d2_str[4] = str_ptr;
  P1->redraw_panel();
}

void update_z_rot_pt(char* str_ptr)
{ update_helper(str_ptr,((int)d3gl_win->get_zmin()),((int)d3gl_win->get_zmax()));
  d2_str[5] = str_ptr;
  P1->redraw_panel();
 }

void update_x_rot(char* str_ptr)
{ update_helper(str_ptr,((int)d3gl_win->get_xmin()),((int)d3gl_win->get_xmax()));
  d2_str[0] = str_ptr;
  P1->redraw_panel();
 }

void update_y_rot(char* str_ptr)
{ update_helper(str_ptr,((int)d3gl_win->get_ymin()),((int)d3gl_win->get_ymax()));
  d2_str[1] = str_ptr;
  P1->redraw_panel();
 }

void update_z_rot(char* str_ptr)
{ update_helper(str_ptr,((int)d3gl_win->get_zmin()),((int)d3gl_win->get_zmax()));
  d2_str[2] = str_ptr;
  P1->redraw_panel();
 }



void update_to_unsigned_int(char* str_ptr, const int& limit, string& target)
{
	if(str_ptr[0] == '\0'){
		target = "";
		return;
	}

	string str(str_ptr);	
	clean_string(str,true);		

	if(str.length()>0){					
		int now = string_to_int(str);
		if(now>limit)			
			target = int_to_string(limit);							
		else
			target = str;						
	}    	
}


void update_helper(char* str_ptr, const int& limit)
{
  string original(str_ptr);
  string changed;
  update_to_unsigned_int(str_ptr,limit,changed);

  if(original != changed){ 	
    int i;
	for(i=0; i<changed.length(); i++)
		 str_ptr[i] = changed[i];
	str_ptr[i] = '\0';
	
	if(P3) P3->redraw_panel();	
  }
}


void update_max(char* str_ptr)
{ update_helper(str_ptr,MAX_COORD_SIZE); }

void update_width(char* str_ptr)
{ update_helper(str_ptr,(int)(d3gl_win->get_xmax()-d3gl_win->get_xmin()));
  d1_str[3] = str_ptr;
  P1->redraw_panel();
 }

void update_height(char* str_ptr)
{ update_helper(str_ptr,(int)(d3gl_win->get_ymax()-d3gl_win->get_ymin()));
  d1_str[4] = str_ptr;
  P1->redraw_panel();
 }

void update_depth(char* str_ptr)
{ update_helper(str_ptr,(int)(d3gl_win->get_zmax()-d3gl_win->get_zmin()));
  d1_str[5] = str_ptr;
  P1->redraw_panel();
 }

void update_num_pts(char* str_ptr)
{ update_helper(str_ptr,100000);
  d1_str[6] = str_ptr;
  P1->redraw_panel();
 }

void update_rot_angle(char* str_ptr)
{ update_helper(str_ptr,360);
  d2_str[6] = str_ptr; 
  P1->redraw_panel();
 }



void update_type(int i)
{	
	if(i==2){ // square
		panel_item it = P1->get_item("depth");
		d1_str[5] = "0";		
		P1->disable_item(it);
	}
	else{
		panel_item it = P1->get_item("depth");
		P1->enable_item(it);
	}
	P1->redraw();
}

void update_rotation(int j)
{
	if(!j){
		for(int i=0 ; i<3 ; i++){
			P1->disable_item(rot_items[i]);
			P1->disable_item(rot_sub_items[i]);
		}		
		P1->disable_item(rot_items[3]);
		P1->disable_item(rot_items[4]);
	}
	else{
		for(int i=0; i<5 ; i++)
			P1->enable_item(rot_items[i]);
		if(!middle_point){
			for(int k=0; k<3; k++)
				P1->enable_item(rot_sub_items[k]);
		}
	}
}

void update_mp(int i)
{
	if(!i){
		for(int k=0; k<3; k++)
			P1->enable_item(rot_sub_items[k]);
	}
	else{
		for(int k=0; k<3; k++)
			P1->disable_item(rot_sub_items[k]);
	}
	middle_point = i;
}


void update_files(char* str_ptr, const bool& bg_tex)
{
	string s;
	if(bg_tex)	s = bg_path+sep+str_ptr;
	else		s = path+sep+str_ptr;

	if(is_file(s)){
		tc_texture T;
		if(!handle_bmp(s,T)){						
			P1->acknowledge(ERR_MSG1);			
		}
	}
}

void update_obj_tex_file(char* str_ptr)
{ update_files(str_ptr, false); }

void update_bg_tex_file(char* str_ptr)
{ update_files(str_ptr, true); }


