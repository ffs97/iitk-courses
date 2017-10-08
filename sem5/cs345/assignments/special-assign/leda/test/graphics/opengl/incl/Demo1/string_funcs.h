// some useful functions for string-manipulation and string-processing

inline int string_to_int(const string& s)
{ 
	integer i(s);
	return ((int)i.to_double());
}

inline string int_to_string(const int& i)
{
	integer i_obj(i);
	return i_obj.to_string();
}

bool is_digit(const char& c){
	switch(c){
			case '0' :
			case '1' :
			case '2' :
			case '3' :
			case '4' :
			case '5' :
			case '6' :
			case '7' :
			case '8' :
			case '9' : return true;		
	}
	return false;
}


bool clean_string(string& s, const bool& only_positive)
{	
	bool changed = false;
	char c = s[0];	
	int index = 0;

	if(c=='-'){
		if(only_positive){
			changed = true;
			s = s(1,s.length());		
		}
		else
			index = 1;
	}	

	int l = s.length();	

	while(index < l)
	{
		c = s[index];
		if(!is_digit(c)){
			changed = true;
			s = s(0,index-1) + s(index+1,l);
			--l;			
			continue;
		}
		++index;
	}
	return changed;
}
  
