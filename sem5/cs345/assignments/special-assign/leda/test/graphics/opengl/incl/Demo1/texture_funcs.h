// methods for loading, registering and managing textures

bool reg_tex(const unsigned int& i,const GLint& filter)
{
	if(T[i]){
		d3gl_win->reg_tex(&tex_name[i],T[i]->get_data(),
			T[i]->get_width(),T[i]->get_height(),filter);
		return true;
	}
	return false;
}

bool load_tex(const unsigned int& i,const string& s,
			  const float& wished_width, const float& wished_height)
{
	gl_win->gl_start();
	
	// first delete old object if it exists
	if(T[i] != NULL){
		const GLuint* ptr = &tex_name[i];
		glDeleteTextures(1,ptr);
		delete T[i];
		T[i] = NULL;		
		tex_name[i] = 0;
		tex_width[i] = 0;
		tex_height[i] = 0;
	}	

	// then create new object
	T[i] = new tc_texture;
	tex_name[i] = 0;	
	
	if(!handle_bmp(s,*T[i])){				
		delete T[i];
		T[i] = NULL;
		tex_name[i] = 0;
		tex_width[i] = 0;
		tex_height[i] = 0;
		return false;
	}
	
	if(wished_width < 0)
		tex_width[i] = T[i]->get_width();
	else
		tex_width[i] = wished_width;

	if(wished_height < 0)
		tex_height[i] = T[i]->get_height();
	else
		tex_height[i] = wished_height;			

	return true;
}

bool load_and_reg_scene_tex(const string& s, const GLint& filter,d3_gl_scene* sc,
						 const list<face>& faces)
{
	// then create new object
	tc_texture* T = new tc_texture;		
	
	if(!handle_bmp(s,*T)){				
		delete T;
		return false;
	}

	int tex_num = sc->reg_tex(T->get_data(),T->get_width(),T->get_height(),filter,faces);	

	scene_textures.append(texture_int(T,tex_num));
	return true;
}

void delete_and_unreg_scene_tex(const int& tex_num, d3_gl_scene* scene)
{
	list_item it = scene_textures.first();
	while(it!=nil){
		texture_int& T = scene_textures[it];		

		if(T.second() == tex_num){
			scene->unreg_tex(tex_num);
			delete T.first();			

			list_item now = it;
			it = scene_textures.succ(it);
			scene_textures.del(now);
		}
		else
			it = scene_textures.succ(it);
	}
}

/*
bool load_tex(const unsigned int& i,const string& s,  
			  const float& width, const float& height);
bool reg_tex(const unsigned int& i,const GLint& filter);
*/

bool set_bg_tex(int& surface, string& tex_width, string& tex_height) 
{   	
	int detail;
	if(d3gl_win->get_tex_filter(&tex_name[surface]) == GL_NEAREST)
		detail = 0;
	else
		detail = 1;

	string surface_str;
	switch(surface)
	{
	case 0 : surface_str = "Front"; break;
	case 1 : surface_str = "Back"; break;
	case 2 : surface_str = "Left"; break;
	case 3 : surface_str = "Right"; break;
	case 4 : surface_str = "Top"; break;
	case 5 : surface_str = "Bottom"; break;
	}

	panel P(surface_str+" Texture");
	P1 = &P;     //P1 musst be free !
	P.set_panel_bg_color(ivory);
	P.text_item("\\bf\\blue "+surface_str+" Texture:");
	P.text_item("\\n \\n ");
	
	list<string> texture_files; 
	string bg_tex;
	load_files(texture_files,bg_path,bg_tex);		
	P.string_item("select texture",bg_tex,texture_files,5,update_bg_tex_file);

	P.text_item("");		
	P.string_item("Texture Width" ,tex_width  ,update_max);
	P.string_item("Texture Height",tex_height ,update_max);

	P.text_item("");
	list<string> details;
	details.append("low (GL_NEAREST)");
	details.append("high (GL_LINEAR)");	
	P.choice_item("Texture Detail",detail,details);

	P.button("ok",25);
	P.button("cancel",26);	

	int val = P.open();

	if(val==25){
		string s = bg_path+sep+bg_tex;
		if(!is_file(s)){
			P.acknowledge("The texture-file you entered ("+bg_tex+") doesnt exist !!!");
			if(detail) d3gl_win->set_tex_filter(&tex_name[surface],GL_LINEAR);
			else	   d3gl_win->set_tex_filter(&tex_name[surface],GL_NEAREST);
			return false;
		}
		else{			
			if(tex_width.length()==0 || tex_height.length()==0){
				P.acknowledge("Width or height or both was entered incorrectly !!!");
				if(detail) d3gl_win->set_tex_filter(&tex_name[surface],GL_LINEAR);
				else	   d3gl_win->set_tex_filter(&tex_name[surface],GL_NEAREST);
				return false;
			}
			else{
				float w = (float) string_to_int(tex_width);
				float h = (float) string_to_int(tex_height);

				if(w == 0 || h == 0){
					P.acknowledge("Width or height or both was entered incorrectly !!!");
					return false;
				}
									
				if(!load_tex(surface,s,w,h)){	
					P.acknowledge(ERR_MSG1);
					if(detail) d3gl_win->set_tex_filter(&tex_name[surface],GL_LINEAR);
					else	   d3gl_win->set_tex_filter(&tex_name[surface],GL_NEAREST);
					return false;
				}
				else reg_tex(surface,detail?GL_LINEAR:GL_NEAREST);
			}
		}
	}

	P.close();
	P1 = NULL;
	return true;
}
