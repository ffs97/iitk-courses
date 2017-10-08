// function for reading geometric objects from a file in standard format

bool read_file_standard_format(ifstream& input_stream, PARA_GRAPH* H, list<d3_rat_point>& PTS)
{	
	string line;	
	double d;	

	list<node> nodes;		

    while(input_stream) 	
	{ 		
		line = read_line(input_stream);			

		switch(line[0])	
		{
			case 'v' :{	// vertex
				if(line.length() > 0 && line[1] == 'n') continue; // exclude "vn"				

				line = line(1,line.length());								
				string_istream I(line);													
				
				I >> d;
				rational x(d);							
				
				I >> d;
				rational y(d);								
				
				I >> d;
				rational z(d);								 																
						
				nodes.append(H->new_node(d3_rat_point(x,y,z)));
				PTS.append(d3_rat_point(x,y,z));				

				break;
					  }
             
			case 'f' :{ // face		
				
				int i1,i2,i3;
				line = line(1,line.length());

				i1 = line.pos("//");				
				while(i1 != -1){		
					i2 = line.pos(" ",i1);
					if(i2 == -1) i2 = line.length();
					line = line(0,i1-1)+line(i2,line.length());

					i1 = line.pos("//");
				}			

				string_istream I(line);

				I >> i1;
				I >> i2;
				I >> i3;				

				node n1 = nodes[nodes.get_item(i1-1)];					
				node n2 = nodes[nodes.get_item(i2-1)];
				node n3 = nodes[nodes.get_item(i3-1)];				

				H->set_reversal(H->new_edge(n1,n2),H->new_edge(n2,n1));
				H->set_reversal(H->new_edge(n1,n3),H->new_edge(n3,n1));
				H->set_reversal(H->new_edge(n2,n3),H->new_edge(n3,n2));	

				i2 = i3;
				I >> i3;
				
				while(I){
					n1 = nodes[nodes.get_item(i1-1)];					
					n2 = nodes[nodes.get_item(i2-1)];
					n3 = nodes[nodes.get_item(i3-1)];					

					H->set_reversal(H->new_edge(n1,n2),H->new_edge(n2,n1));
					H->set_reversal(H->new_edge(n1,n3),H->new_edge(n3,n1));
					H->set_reversal(H->new_edge(n2,n3),H->new_edge(n3,n2));
					
					i2 = i3;
					I >> i3;
				}											

				break;
					  }
		}
    }	
	
	return true;
}


bool read_file_off_format(ifstream& input_stream, PARA_GRAPH* H, list<d3_rat_point>& PTS2)
{
	double x0,x1,y0,y1,z0,z1;
	array<d3_point> PTS;	

	typedef list<int> int_list;
	list<int_list> face_list;

	if(read_off_file(input_stream,x0,x1,y0,y1,z0,z1,PTS,face_list)){
		int s = PTS.size();
		array<node> NODES(s);

		for(int i=0; i<s; i++){
			d3_point& p = PTS[i];
			rational x(p.xcoord());
			rational y(p.ycoord());
			rational z(p.zcoord());
			NODES[i] = H->new_node(d3_rat_point(x,y,z));
			PTS2.append(d3_rat_point(x,y,z));
		}

		list_item it = face_list.first();
		while(it != nil){
			int_list& L = face_list[it];

			if(L.length()<3) continue;

			list_item it2 = L.first();
			node n1 = NODES[L[it2]];
			it2 = L.succ(it2);
			node n2 = NODES[L[it2]];
			it2 = L.succ(it2);
			node n3 = NODES[L[it2]];
			it2 = L.succ(it2);

			H->set_reversal(H->new_edge(n1,n2),H->new_edge(n2,n1));
			H->set_reversal(H->new_edge(n1,n3),H->new_edge(n3,n1));
			H->set_reversal(H->new_edge(n2,n3),H->new_edge(n3,n2));	

			while(it2 != nil){
                n2 = n3;
				n3 = NODES[L[it2]];

				H->set_reversal(H->new_edge(n1,n2),H->new_edge(n2,n1));
				H->set_reversal(H->new_edge(n1,n3),H->new_edge(n3,n1));
				H->set_reversal(H->new_edge(n2,n3),H->new_edge(n3,n2));	

				it2 = L.succ(it2);
			}

			it = face_list.succ(it);			
		}

		return true;
	}

	return false;
}


// function that chooses between generell- and standard-format

// return values :
// 0 -> real error
// 1 -> 'unreal' error -> new try
// 2 -> success 
int load_scene_from_file(d3_gl_scene*& sc, const int& conv_hull, const int& triang)
{
	ifstream input_stream(filename); 	

    if (!input_stream.good() || !input_stream)
	{  show_short_message(ERR_MSG5,"Warning");       
       return 1;
    }

	list<d3_rat_point> PTS;
	PARA_GRAPH* H = new PARA_GRAPH;	

	if(off_format){
		if(!read_file_off_format(input_stream, H, PTS)){
			input_stream.close();
			show_short_message(ERR_MSG6,"Error");
			return 0;
		}
		else if(H->number_of_nodes() == 0){
			input_stream.close();
			show_short_message(ERR_MSG6,"Error");
			return 0;
		}
		else{
			H->make_planar_map();
		}
	}
	else{
		if(!read_file_standard_format(input_stream, H, PTS)){
			input_stream.close();
			show_short_message(ERR_MSG6,"Error");
			return 0;
		}
		else if(H->number_of_nodes() == 0){
			input_stream.close();
			show_short_message(ERR_MSG6,"Error");
			return 0;
		}
		else{			
			H->make_planar_map();			
		}
	}
	
	if(conv_hull){
		D3_HULL(PTS,*H);
		H->compute_faces();
	}
	else if(triang){
		H->triangulate_map();
	}	
	
	input_stream.close();

	node_array<rat_vector> pos_nodes(*H);
	node v;
	forall_nodes(v,*H) pos_nodes[v] = (*H)[v].to_vector();	
	
	sc = new d3_gl_scene(*H,pos_nodes);    

	d3gl_win->append_scene(sc);		
	
	return 2;
}


// if path2 contains valid bmp-files then a valid file is assigned to tex_file
// (maybe tex_file itself) ; otherwise an error-message is created

void load_files(list<string>& texture_files, string& path2, string& tex_file)
{
    texture_files = get_files(path2,"*.bmp");		
	if(texture_files.empty())
		tex_file = "no bmp-files in current directory";
	else{		
		tc_texture T;				

		if(texture_files.search(tex_file) == nil)
			tex_file = texture_files.front();				
			
		if(!handle_bmp(path2+sep+tex_file,T)){
			panel P2("Error");
			string s2("\\bf\\blue ");
			s2 += ERR_MSG1;
			P2.text_item(s2);
			P2.button("ok",0);
			P2.open(); // GUI error-message
			cout<<"\nload_files : error on loading texture !!!\n"; // shell error-message
		}
	}	
}
