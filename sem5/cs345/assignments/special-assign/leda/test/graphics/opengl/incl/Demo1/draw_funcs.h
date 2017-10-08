// some drawing functions 

// 0 = zpos   ; 1 = zmin ; 2 = zmax ; 3 = z_grid_dist ;
// 4 = height ; 5 = xmin ; 6 = xmax

// 0 = ypos   ; 1 = ymin ; 2 = ymax ; 3 = y_grid_dist ;
// 4 = depth ; 5 = xmin ; 6 = xmax

void draw_x_parallels(double params[7], const int& index1, const int& index2)					  
{
	double* p1;
	double* p2;

	if(index1 > index2){
		p1 = params+4;
		p2 = params+2;		
	}
	else{
		p1 = params+2;
		p2 = params+4;		
	}	
         
	while(params[0] < params[2]){
		glVertex3d(params[5],params[index1],params[index2]);
		glVertex3d(params[6],params[index1],params[index2]);			
		params[0] += params[3];
	}
	
	glVertex3d(params[5],*p1,*p2);
	glVertex3d(params[6],*p1,*p2);

	params[0] = 0.0;

	while(params[0] > params[1]){
		glVertex3d(params[5],params[index1],params[index2]);
		glVertex3d(params[6],params[index1],params[index2]);			
		params[0] -= params[3];
	}

	if(index1 > index2)
		--p2;
	else
		--p1;
	
	glVertex3d(params[5],*p1,*p2);
	glVertex3d(params[6],*p1,*p2);	
}


// 0 = xpos   ; 1 = xmin ; 2 = xmax ; 3 = x_grid_dist ;
// 4 = depth  ; 5 = ymin ; 6 = ymax

// 0 = zpos   ; 1 = zmin ; 2 = zmax ; 3 = z_grid_dist ;
// 4 = width  ; 5 = ymin ; 6 = ymax 

void draw_y_parallels(double params[7], const int& index1, const int& index2)
{
	double* p1;
	double* p2;

	if(index1 > index2){
		p1 = params+4;
		p2 = params+2;
	}
	else{
		p1 = params+2;
		p2 = params+4;
	}	
         
	while(params[0] < params[2]){
		glVertex3d(params[index2],params[5],params[index1]);
		glVertex3d(params[index2],params[6],params[index1]);			
		params[0] += params[3];
	}
	glVertex3d(*p2,params[5],*p1);
	glVertex3d(*p2,params[6],*p1);

	params[0] = 0.0;

	while(params[0] > params[1]){
		glVertex3d(params[index2],params[5],params[index1]);
		glVertex3d(params[index2],params[6],params[index1]);			
		params[0] -= params[3];
	}

	if(index1 > index2)
		--p2;
	else
		--p1;

	glVertex3d(*p2,params[5],*p1);
	glVertex3d(*p2,params[6],*p1);	
}


// 0 = xpos   ; 1 = xmin ; 2 = xmax ; 3 = x_grid_dist ;
// 4 = height ; 5 = zmin ; 6 = zmax

// 0 = ypos   ; 1 = ymin ; 2 = ymax ; 3 = y_grid_dist ;
// 4 = width  ; 5 = zmin ; 6 = zmax

void draw_z_parallels(double params[7], const int& index1, const int& index2)
{
	double* p1;
	double* p2;

	if(index1 > index2){
		p1 = params+4;
		p2 = params+2;
	}
	else{
		p1 = params+2;
		p2 = params+4;
	}	
         
	while(params[0] < params[2]){
		glVertex3d(params[index2],params[index1],params[5]);
		glVertex3d(params[index2],params[index1],params[6]);			
		params[0] += params[3];
	}
	glVertex3d(*p2,*p1,params[5]);
	glVertex3d(*p2,*p1,params[6]);

	params[0] = 0.0;

	while(params[0] > params[1]){
		glVertex3d(params[index2],params[index1],params[5]);
		glVertex3d(params[index2],params[index1],params[6]);			
		params[0] -= params[3];
	}

	if(index1 > index2)
		--p2;
	else
		--p1;

	glVertex3d(*p2,*p1,params[5]);
	glVertex3d(*p2,*p1,params[6]);	
}


void draw_front_back(const unsigned int& tex_num, const bool& front)
{		 
		 float y_step = tex_height[tex_num];
		 float x_step = tex_width[tex_num];
		 
		 if(y_step > (box_ymax - box_ymin)) y_step = box_ymax - box_ymin;
		 if(x_step > (box_xmax - box_xmin)) x_step = box_xmax - box_xmin;

		 float xpos  = box_xmin;
		 float xpos2 = box_xmin + x_step;
		 float ypos  = box_ymin;
		 float ypos2 = box_ymin + y_step;
		 bool  flag  = true;		 

		 float map_x1 = 0.0f;
		 float map_x2 = 1.0f;
		 float z = box_zmin;
		 if(front){
			 map_x1 = 1.0f;
			 map_x2 = 0.0f;
			 z = box_zmax;
		 }		 		 

		 if(T[tex_num] != NULL){				
			 glBindTexture(GL_TEXTURE_2D, tex_name[tex_num]);
			 glBegin(GL_QUADS);	
		 }
		 else return;		 		 		 

		 while(ypos<box_ymax)
		 {
			 if(flag)
			 {
				 while(xpos2 < box_xmax)
				 {						 
					 glTexCoord2f(map_x1, 0.0f); glVertex3f(xpos  ,ypos , z);
				     glTexCoord2f(map_x1, 1.0f); glVertex3f(xpos  ,ypos2, z);					 						 
					 glTexCoord2f(map_x2, 1.0f); glVertex3f(xpos2 ,ypos2, z);
				 	 glTexCoord2f(map_x2, 0.0f); glVertex3f(xpos2 ,ypos , z);					 
					 					 
					 xpos  += x_step;
					 xpos2 += x_step;
				 }				 

				 xpos2 = box_xmax;				 
				 
				 glTexCoord2f(map_x1, 0.0f); glVertex3f(xpos  ,ypos , z );
				 glTexCoord2f(map_x1, 1.0f); glVertex3f(xpos  ,ypos2, z );				 
                 glTexCoord2f(map_x2, 1.0f); glVertex3f(xpos2 ,ypos2, z );
				 glTexCoord2f(map_x2, 0.0f); glVertex3f(xpos2 ,ypos , z );				 				 				 

				 ypos  += y_step;
				 ypos2 += y_step;
				 flag = false;
			 }
			 else
			 {					 
				 glTexCoord2f(map_x2, 0.0f); glVertex3f(xpos2 ,ypos,  z ); 
				 glTexCoord2f(map_x2, 1.0f); glVertex3f(xpos2 ,ypos2, z );				 					 
				 glTexCoord2f(map_x1, 1.0f); glVertex3f(xpos  ,ypos2, z );
				 glTexCoord2f(map_x1, 0.0f); glVertex3f(xpos  ,ypos,  z );				
				 				 
				 xpos2 = xpos + x_step;

				 do{
					 xpos  -= x_step;
					 xpos2 -= x_step;
					 
					 glTexCoord2f(map_x2, 0.0f); glVertex3f(xpos2 ,ypos,  z ); 
					 glTexCoord2f(map_x2, 1.0f); glVertex3f(xpos2 ,ypos2, z );					 
					 glTexCoord2f(map_x1, 1.0f); glVertex3f(xpos  ,ypos2, z );
					 glTexCoord2f(map_x1, 0.0f); glVertex3f(xpos  ,ypos,  z );											 
					 					 
				 }while(xpos > box_xmin);							 

				 ypos  += y_step;
				 ypos2 += y_step;
				 flag = true;
			 }
			 
			 if(ypos2 > box_ymax)
				 ypos2 = box_ymax;
		 }	
		 
		 glEnd();		 			 			
}


void draw_left_right(const unsigned int& tex_num, const bool& left)
{			 
		 float y_step = tex_height[tex_num];
		 float z_step = tex_width[tex_num];
		 
		 if(y_step > (box_ymax - box_ymin)) y_step = box_ymax - box_ymin;
		 if(z_step > (box_zmax - box_zmin)) z_step = box_zmax - box_zmin;

		 float zpos  = box_zmin;
		 float zpos2 = box_zmin + z_step;
		 float ypos  = box_ymin;
		 float ypos2 = box_ymin + y_step;
		 bool  flag  = true;
		 
		 float map_x1 = 0.0f;
		 float map_x2 = 1.0f;
		 float x = box_xmax;

		 if(left){
			 map_x1 = 1.0f;
			 map_x2 = 0.0f;
			 x = box_xmin;
		 }		 		 

		 if(T[tex_num] != NULL){			 
			glBindTexture(GL_TEXTURE_2D, tex_name[tex_num]);
			glBegin(GL_QUADS);	
		 }
		 else return;		

		 while(zpos<box_zmax)
		 {
			 if(flag)
			 {
				 while(ypos2 < box_ymax)
				 {						 
					 glTexCoord2f(map_x1, 0.0f); glVertex3f(x ,ypos,  zpos );
				     glTexCoord2f(map_x1, 1.0f); glVertex3f(x ,ypos2, zpos );					 
                     glTexCoord2f(map_x2, 1.0f); glVertex3f(x ,ypos2, zpos2);
					 glTexCoord2f(map_x2, 0.0f); glVertex3f(x ,ypos , zpos2);											 
					 					 
					 ypos  += y_step;
					 ypos2 += y_step;
				 }				 

				 ypos2 = box_ymax;				 
				 glTexCoord2f(map_x1, 0.0f); glVertex3f(x ,ypos , zpos );
				 glTexCoord2f(map_x1, 1.0f); glVertex3f(x ,ypos2, zpos );				 
				 glTexCoord2f(map_x2, 1.0f); glVertex3f(x ,ypos2, zpos2);
				 glTexCoord2f(map_x2, 0.0f); glVertex3f(x ,ypos , zpos2);									 				 				 

				 zpos  += z_step;
				 zpos2 += z_step;
				 flag = false;
			 }
			 else
			 {					 
				 glTexCoord2f(map_x2, 0.0f); glVertex3f(x ,ypos , zpos2); 
				 glTexCoord2f(map_x2, 1.0f); glVertex3f(x ,ypos2, zpos2);				 				 
                 glTexCoord2f(map_x1, 1.0f); glVertex3f(x ,ypos2, zpos );
				 glTexCoord2f(map_x1, 0.0f); glVertex3f(x ,ypos , zpos );									 
				 				 
				 ypos2 = ypos + y_step;

				 do{
					 ypos  -= y_step;
					 ypos2 -= y_step;					 
					 glTexCoord2f(map_x2, 0.0f); glVertex3f(x ,ypos , zpos2); 
					 glTexCoord2f(map_x2, 1.0f); glVertex3f(x ,ypos2, zpos2);					 
					 glTexCoord2f(map_x1, 1.0f); glVertex3f(x ,ypos2, zpos );
					 glTexCoord2f(map_x1, 0.0f); glVertex3f(x ,ypos , zpos );											 
										 
				 }while(ypos > box_ymin);							 

				 zpos  += z_step;
				 zpos2 += z_step;
				 flag = true;
			 }
			 
			 if(zpos2 > box_zmax)
				 zpos2 = box_zmax;
		 }	
		 
		 glEnd();		 	 			
}


void draw_bottom_top(const unsigned int& tex_num, const bool& top)
{			 
		 float x_step = tex_width[tex_num];
		 float z_step = tex_height[tex_num];
		 
		 if(x_step > (box_xmax - box_xmin)) x_step = box_xmax - box_xmin;
		 if(z_step > (box_zmax - box_zmin)) z_step = box_zmax - box_zmin;

		 float zpos  = box_zmin;
		 float zpos2 = box_zmin + z_step;
		 float xpos  = box_xmin;
		 float xpos2 = box_xmin + x_step;
		 bool  flag  = true;
		 
		 float map_y1 = 0.0f;
		 float map_y2 = 1.0f;
		 float y = box_ymin;
		 if(top){
			 map_y1 = 1.0f;
			 map_y2 = 0.0f;
			 y = box_ymax;
		 }		 		 

		 if(T[tex_num] != NULL){			  
			glBindTexture(GL_TEXTURE_2D, tex_name[tex_num]);
			glBegin(GL_QUADS);	
		 }
		 else return;		 

		 while(zpos<box_zmax)
		 {
			 if(flag)
			 {
				 while(xpos2 < box_xmax)
				 {					 
					 glTexCoord2f(1.0f, map_y1); glVertex3f(xpos  ,y, zpos );
				     glTexCoord2f(1.0f, map_y2); glVertex3f(xpos  ,y, zpos2);					 
					 glTexCoord2f(0.0f, map_y2); glVertex3f(xpos2 ,y, zpos2);
					 glTexCoord2f(0.0f, map_y1); glVertex3f(xpos2 ,y, zpos );											 
					 					 
					 xpos  += x_step;
					 xpos2 += x_step;
				 }				 

				 xpos2 = box_xmax;					 
				 glTexCoord2f(1.0f, map_y1); glVertex3f(xpos  ,y, zpos );
				 glTexCoord2f(1.0f, map_y2); glVertex3f(xpos  ,y, zpos2);				 
                 glTexCoord2f(0.0f, map_y2); glVertex3f(xpos2 ,y, zpos2);
				 glTexCoord2f(0.0f, map_y1); glVertex3f(xpos2 ,y, zpos );					 

				 zpos  += z_step;
				 zpos2 += z_step;
				 flag = false;
			 }
			 else
			 {					 
				 glTexCoord2f(0.0f, map_y1); glVertex3f(xpos2 ,y, zpos ); 
				 glTexCoord2f(0.0f, map_y2); glVertex3f(xpos2 ,y, zpos2);				 					
				 glTexCoord2f(1.0f, map_y2); glVertex3f(xpos  ,y, zpos2);
				 glTexCoord2f(1.0f, map_y1); glVertex3f(xpos  ,y, zpos );				 
				 				 
				 xpos2 = xpos + x_step;

				 do{
					 xpos  -= x_step;
					 xpos2 -= x_step;
					 
					 glTexCoord2f(0.0f, map_y1); glVertex3f(xpos2 ,y, zpos ); 
					 glTexCoord2f(0.0f, map_y2); glVertex3f(xpos2 ,y, zpos2); 					 						
					 glTexCoord2f(1.0f, map_y2); glVertex3f(xpos  ,y, zpos2);
					 glTexCoord2f(1.0f, map_y1); glVertex3f(xpos  ,y, zpos );					 
					 					 
				 }while(xpos > box_xmin);							 

				 zpos  += z_step;
				 zpos2 += z_step;
				 flag = true;
			 }
			 
			 if(zpos2 > box_zmax)
				 zpos2 = box_zmax;
		 }
		 
		 glEnd();		 	 	
}


void draw_box(d3_gl_window* d3_gl_win, gl_window* gl_win)
{
  if(texturing_box){
     glEnable(GL_TEXTURE_2D);      

		draw_front_back(FRONT  , true);   
		draw_front_back(BACK   , false);   
  
		draw_left_right(LEFT   , true);   
		draw_left_right(RIGHT  , false);        

		draw_bottom_top(TOP    , true);  	  
		draw_bottom_top(BOTTOM , false);
	
	glDisable(GL_TEXTURE_2D);	
  }

  if(box_filling){	 
	 box_fill_color();
	 glBegin(GL_QUADS);

	    // left
		glVertex3f(box_xmin,box_ymin,box_zmin);
		glVertex3f(box_xmin,box_ymin,box_zmax);
		glVertex3f(box_xmin,box_ymax,box_zmax);
		glVertex3f(box_xmin,box_ymax,box_zmin);		

		// right
		glVertex3f(box_xmax,box_ymin,box_zmin);
		glVertex3f(box_xmax,box_ymin,box_zmax);
		glVertex3f(box_xmax,box_ymax,box_zmax);
		glVertex3f(box_xmax,box_ymax,box_zmin);

		// bottom
		glVertex3f(box_xmin,box_ymin,box_zmin);
		glVertex3f(box_xmin,box_ymin,box_zmax);
		glVertex3f(box_xmax,box_ymin,box_zmax);
		glVertex3f(box_xmax,box_ymin,box_zmin);

		// top
		glVertex3f(box_xmin,box_ymax,box_zmin);
		glVertex3f(box_xmin,box_ymax,box_zmax);
		glVertex3f(box_xmax,box_ymax,box_zmax);
		glVertex3f(box_xmax,box_ymax,box_zmin);

		// front
		glVertex3f(box_xmin,box_ymin,box_zmax);
		glVertex3f(box_xmin,box_ymax,box_zmax);
		glVertex3f(box_xmax,box_ymax,box_zmax);
		glVertex3f(box_xmax,box_ymin,box_zmax);

		// back
		glVertex3f(box_xmin,box_ymin,box_zmin);
		glVertex3f(box_xmin,box_ymax,box_zmin);
		glVertex3f(box_xmax,box_ymax,box_zmin);
		glVertex3f(box_xmax,box_ymin,box_zmin);

	 glEnd();
  }	

  if(box_inlines){ 		
		box_line_color();
	    glLineWidth(box_line_width);
		
		glBegin(GL_LINES);
		
		//---------------------------------------
		
		double params[7];		
		params[0] = 0.0;		
		params[1] = box_zmin;
		params[2] = box_zmax;
		params[3] = d3gl_win->get_z_grid_dist();
		params[4] = box_ymin;
		params[5] = box_xmin;
		params[6] = box_xmax;		
		draw_x_parallels(params,4,0);

		params[0] = 0.0;
		params[4] = box_ymax;
		draw_x_parallels(params,4,0);				   		
		
		params[0] = 0.0;		
		params[1] = box_ymin;
		params[2] = box_ymax;
		params[3] = d3gl_win->get_y_grid_dist();
		params[4] = box_zmin;				
		draw_x_parallels(params,0,4);

		params[0] = 0.0;
		params[4] = box_zmax;		
		draw_x_parallels(params,0,4);		
		
		//----------------------------------------		
		params[0] = 0.0;		
		params[1] = box_xmin;
		params[2] = box_xmax;
		params[3] = d3gl_win->get_x_grid_dist();
		params[4] = box_zmin;
		params[5] = box_ymin;
		params[6] = box_ymax;		
		draw_y_parallels(params,4,0);

		params[0] = 0.0;	
		params[4] = box_zmax;
		draw_y_parallels(params,4,0);
		
		params[0] = 0.0;		
		params[1] = box_zmin;
		params[2] = box_zmax;
		params[3] = d3gl_win->get_z_grid_dist();
		params[4] = box_xmin;
		params[5] = box_ymin;
		params[6] = box_ymax;		
		draw_y_parallels(params,0,4);

		params[0] = 0.0;	
		params[4] = box_xmax;
		draw_y_parallels(params,0,4);		
		//----------------------------------------
		
		params[0] = 0.0;		
		params[1] = box_xmin;
		params[2] = box_xmax;
		params[3] = d3gl_win->get_x_grid_dist();	
		params[4] = box_ymin;
		params[5] = box_zmin;
	 	params[6] = box_zmax;		  
		draw_z_parallels(params,4,0);
		
		params[0] = 0.0;
		params[4] = box_ymax;
		draw_z_parallels(params,4,0);
		
		
		params[0] = 0.0;		
		params[1] = box_ymin;
		params[2] = box_ymax;
		params[3] = d3gl_win->get_y_grid_dist();	
		params[4] = box_xmin;
		params[5] = box_zmin;
	 	params[6] = box_zmax;		  
		draw_z_parallels(params,0,4);

		params[0] = 0.0;
		params[4] = box_xmax;
		draw_z_parallels(params,0,4);
		
		glEnd();
	}
  
  if(box_outlines){ 	    
		box_line_color();
	    glLineWidth(box_line_width);	    		  
	  
		glBegin(GL_LINE_LOOP); 
			glVertex3f(box_xmin,box_ymin,box_zmin);
			glVertex3f(box_xmin,box_ymin,box_zmax);
			glVertex3f(box_xmin,box_ymax,box_zmax);
			glVertex3f(box_xmin,box_ymax,box_zmin);
		glEnd();

		glBegin(GL_LINE_LOOP); 
			glVertex3f(box_xmax,box_ymin,box_zmin);
			glVertex3f(box_xmax,box_ymin,box_zmax);
			glVertex3f(box_xmax,box_ymax,box_zmax);
			glVertex3f(box_xmax,box_ymax,box_zmin);
		glEnd();

		glBegin(GL_LINES);     
			glVertex3f(box_xmin,box_ymax,box_zmin); glVertex3f(box_xmax,box_ymax,box_zmin);
			glVertex3f(box_xmin,box_ymax,box_zmax); glVertex3f(box_xmax,box_ymax,box_zmax);

			glVertex3f(box_xmin,box_ymin,box_zmin); glVertex3f(box_xmax,box_ymin,box_zmin);
			glVertex3f(box_xmin,box_ymin,box_zmax); glVertex3f(box_xmax,box_ymin,box_zmax);
		glEnd();			        
  }    
}

void texture_plane(const unsigned int& tex_num)
{	
	float x_step = tex_width[tex_num];
	float z_step = tex_height[tex_num];
		 
	if(x_step >   plane_xmax ) x_step =   plane_xmax;
	if(z_step > - plane_zmin ) z_step = - plane_zmin;

	float zpos  = 0.0f;
	float zpos2 = - z_step;
	float xpos  = 0.0f;
	float xpos2 = x_step;	
	float y = plane_height;		
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_name[tex_num]);

	glBegin(GL_QUADS);	

	// right-top	
	while(zpos > plane_zmin){		 
		while(xpos2 < plane_xmax) {		 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos  ,y, zpos );
			glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos  ,y, zpos2);					 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(xpos2 ,y, zpos2);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(xpos2 ,y, zpos );								 
					 					 
			xpos  += x_step;
			xpos2 += x_step;
		}
		xpos2 = plane_xmax;		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos  ,y, zpos );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos  ,y, zpos2);				 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos2 ,y, zpos2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos2 ,y, zpos );
		
		xpos  = 0.0f;
	    xpos2 = x_step;

		zpos  -= z_step;
		zpos2 -= z_step;

		if(zpos2 < plane_zmin)
			 zpos2 = plane_zmin;
	}

	zpos  = 0.0f;
	zpos2 = z_step;

	// right-down
	while(zpos < plane_zmax){		 
		while(xpos2 < plane_xmax) {		 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos  ,y, zpos );
			glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos  ,y, zpos2);					 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(xpos2 ,y, zpos2);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(xpos2 ,y, zpos );								 
					 					 
			xpos  += x_step;
			xpos2 += x_step;
		}
		xpos2 = plane_xmax;		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos  ,y, zpos );
		glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos  ,y, zpos2);				 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos2 ,y, zpos2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos2 ,y, zpos );
		
		xpos  = 0.0f;
	    xpos2 = x_step;

		zpos  += z_step;
		zpos2 += z_step;

		if(zpos2 > plane_zmax)
			 zpos2 = plane_zmax;
	}

	zpos  = 0.0f;
	zpos2 = - z_step;
	xpos  = 0.0f;
	xpos2 = - x_step;

	// left-top	
	while(zpos > plane_zmin){		 
		while(xpos2 > plane_xmin) {		 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos2 ,y, zpos );
			glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos2 ,y, zpos2);					 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(xpos  ,y, zpos2);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(xpos  ,y, zpos );								 
					 					 
			xpos  -= x_step;
			xpos2 -= x_step;
		}
		xpos2 = plane_xmin;		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos2 ,y, zpos );
		glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos2 ,y, zpos2);				 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos  ,y, zpos2);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos  ,y, zpos );
		
		xpos  = 0.0f;
	    xpos2 = - x_step;

		zpos  -= z_step;
		zpos2 -= z_step;

		if(zpos2 < plane_zmin)
			 zpos2 = plane_zmin;
	}

	zpos  = 0.0f;
	zpos2 = z_step;

	// left-down
	while(zpos < plane_zmax){		 
		while(xpos2 > plane_xmin) {		 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos2 ,y, zpos );
			glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos2 ,y, zpos2);					 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(xpos  ,y, zpos2);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(xpos  ,y, zpos );								 
					 					 
			xpos  -= x_step;
			xpos2 -= x_step;
		}
		xpos2 = plane_xmin;		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos2 ,y, zpos );
		glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos2 ,y, zpos2);				 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(xpos  ,y, zpos2);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(xpos  ,y, zpos );
		
		xpos  = 0.0f;
	    xpos2 = - x_step;

		zpos  += z_step;
		zpos2 += z_step;

		if(zpos2 > plane_zmax)
			 zpos2 = plane_zmax;
	}
	
	glEnd();

	glDisable(GL_TEXTURE_2D);
	
}

void draw_plane(d3_gl_window* d3_gl_win, gl_window* gl_win)
{
	if(texturing_plane && T[6] != NULL){
		texture_plane(6);		
	}

	if(plane_filling){			    
		plane_fill_color();

		glBegin(GL_POLYGON);
			glVertex3d(plane_xmin,plane_height,plane_zmin);
			glVertex3d(plane_xmax,plane_height,plane_zmin);
			glVertex3d(plane_xmax,plane_height,plane_zmax);
			glVertex3d(plane_xmin,plane_height,plane_zmax);			
		glEnd();
	}

	if(plane_x_parallels){ 		  
		  plane_line_color();
	      glLineWidth(plane_line_width);		        
		
		  double params[7];
		  params[0] = 0.0;		
		  params[1] = plane_zmin;
		  params[2] = plane_zmax;
		  params[3] = d3gl_win->get_z_grid_dist();
		  params[4] = plane_height;
		  params[5] = plane_xmin;
		  params[6] = plane_xmax;
		
		  glBegin(GL_LINES);
		  draw_x_parallels(params,4,0);
		  glEnd();
	}

	if(plane_z_parallels){			  
		  plane_line_color();
	      glLineWidth(plane_line_width);		   
		  
		  double params[7];
		  params[0] = 0.0;		
		  params[1] = plane_xmin;
		  params[2] = plane_xmax;
		  params[3] = d3gl_win->get_x_grid_dist();	
		  params[4] = plane_height;
		  params[5] = plane_zmin;
	 	  params[6] = plane_zmax;
		  
		  glBegin(GL_LINES);
		  draw_z_parallels(params,4,0);
		  glEnd();
	}

	if(plane_outlines){		
		plane_line_color();
	    glLineWidth(plane_line_width);

		glBegin(GL_LINE_LOOP);
			glVertex3d(plane_xmin,plane_height,plane_zmin);
			glVertex3d(plane_xmax,plane_height,plane_zmin);
			glVertex3d(plane_xmax,plane_height,plane_zmax);
			glVertex3d(plane_xmin,plane_height,plane_zmax);			
		glEnd();		
	}
}
