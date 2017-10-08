/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  sgi_filebuf.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef _LEDA_SGI_FILEBUF_H_
#define _LEDA_SGI_FILEBUF_H_

#include <LEDA/internal/std/string.h>

/*
	There is a bug in the filebuf implementation of SGI CC:
	  When we read until we reach an EOF, the class goes to an error state and
	  we cannot do seekoff/seekpos anymore (to rewind to the beginning).

	Work-around:
	  We derive a class that stores the filename and reopens the file if a seek
	  operation fails.

	Known problems:
	- Since the open-method is not virtual, our method is bypassed if a 
	  leda_filebuf* is converted to filebuf*.
    - We cannot work-around a relative seekoff (i.e. way == ios_base::cur).
*/

class leda_filebuf : public filebuf {
public:
	typedef filebuf base;

public:
	leda_filebuf() : FileName(0) {}
	virtual ~leda_filebuf() { assign_filename(0); }

	leda_filebuf* open(const char* name, ios_base::openmode mode)
	{ assign_filename(name); Mode = mode; base::open(name, mode); return this; }

protected:
	virtual streampos seekoff(streamoff off, ios::seekdir way, ios_base::openmode which)
	{
		streampos res = base::seekoff(off, way, which);
		if (res >= 0 || way == ios_base::cur) return res;
		base::close(); base::open(FileName, Mode);
		return base::seekoff(off, way, which);
	}

	virtual streampos seekpos(streampos sp, ios_base::openmode which)
	{
		streampos res = base::seekpos(sp, which);
		if (res >= 0) return res;
		base::close(); base::open(FileName, Mode);
		return base::seekpos(sp, which);
	}

private:
	void assign_filename(const char* name)
	{ 
		delete FileName; FileName = 0; 
		if (name) { int n = strlen(name); FileName = new char[n]; strcpy(FileName, name); }
	}

private:
	char*              FileName;
	ios_base::openmode Mode;
};

#endif

// The following must be OUTSIDE the #ifndef / #endif
#undef filebuf
#define filebuf leda_filebuf
