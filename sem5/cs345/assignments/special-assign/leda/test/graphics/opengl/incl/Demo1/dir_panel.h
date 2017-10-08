#ifndef LEDA_DIR_PANEL_H
#define LEDA_DIR_PANEL_H

#include <LEDA/file_panel.h>

LEDA_BEGIN_NAMESPACE

const int file_load   = 0; // see file_panel.c  

class dir_panel : public file_panel
{
protected :
	window* wp;
	string* dir_ptr;

public :
	dir_panel(string& fname, string& dname) : file_panel(fname,dname)
	{ wp = NULL; dir_ptr = &dname; }
	dir_panel(window& W, string& fname, string& dname) : file_panel(W,fname,dname)
	{ wp = &W; dir_ptr = &dname; }
	
	bool open();
	bool open(int xpos, int ypos);
};


bool dir_panel::open()
{ 
  int result;
  init_panel();
  panel& P = get_panel();    
  P.set_text(P.first_item(),"\\bf\\blue Select Directory");

  if(wp)
 	 result = P.open(*wp,window::center,window::center);	    
  else
	 result = P.open(window::center,window::center);	  

  if(!result){
	  if (is_directory(*dir_ptr)){	  
		set_directory(*dir_ptr);
		*dir_ptr = get_directory();
		return true;
	  }
	  else
		return false;
  }
  else
	  return false;
}

bool dir_panel::open(int xpos, int ypos)
{ 
  int result;
  init_panel();
  panel& P = get_panel();    
  P.set_text(P.first_item(),"\\bf\\blue Select Directory");

  if (wp)
 	 result = P.open(*wp,xpos,ypos);	    
  else
	 result = P.open(xpos,ypos);	      	  

  if(!result){
	  if (is_directory(*dir_ptr)){	  
		set_directory(*dir_ptr);
		*dir_ptr = get_directory();
		return true;
	  }
	  else
		return false;
  }
  else
	  return false;
}


// for Demo1.cpp

typedef double* D_PTR;

int compare(const D_PTR& d1, const D_PTR& d2)
{ 	if(d1 == d2)
		return 0;
	else
		return -1;
}


typedef int* I_PTR;

int compare(const I_PTR& i1, const I_PTR& i2)
{ 	if(i1 == i2)
		return 0;
	else
		return -1;
}


typedef string* STR_PTR;

int compare(const STR_PTR& s1, const STR_PTR& s2)
{ 	if(s1 == s2)
		return 0;
	else
		return -1;
}


LEDA_END_NAMESPACE

#endif
