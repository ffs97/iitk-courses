/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  PREAMBLE.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <LEDA/internal/system.h>

#include <LEDA/internal/std/iostream.h>
#include <LEDA/internal/std/fstream.h>
#include <LEDA/internal/std/sstream.h>
#include <LEDA/internal/std/stddef.h>
#include <LEDA/internal/std/stdlib.h>
#include <LEDA/internal/std/math.h>

#if defined(LEDA_STD_IO_HEADERS)
#define ostream        std::ostream 
#define istream        std::istream
#define cout           std::cout
#define cin            std::cin
#define cerr           std::cerr
#define endl           std::endl
#define ends           std::ends
#define hex            std::hex
#define dec            std::dec
#define setw           std::setw
#define setprecision   std::setprecision
#define ios            std::ios
#define ofstream       std::ofstream
#define ifstream       std::ifstream
#define istringstream  std::istringstream
#define ostringstream  std::ostringstream
#define ios_base       std::ios_base
#define streambuf      std::streambuf
#define streampos      std::streampos
#define streamoff      std::streamoff
#undef streamsize
#define streamsize     std::streamsize
#undef filebuf
#define filebuf        std::filebuf

#ifndef STREAM_DUMMY
#define STREAM_DUMMY
inline void stream_dummy(char* buf, int sz) 
{ ifstream is;
  is.open("dummy");
  is.get(buf,sz);
  is >> buf;
}
#endif

#endif



// work-around for MSVC 6.0, CXX5.1 (alpha), aCC
#if !defined(LEDA_STD_HEADERS) && ((defined(_MSC_VER) && _MSC_VER < 1300) || defined(__DECCXX) || defined (__HP_aCC))
#define LEDA_COMPRESSION_WA1
#define openmode open_mode
#define seekdir seek_dir
#undef streamsize
#define streamsize int
#define pubseekpos seekpos
#define pubseekoff seekoff
#define pubsync sync
#endif

// work-around for SGI CC
#ifdef __mipspro__
#define LEDA_COMPRESSION_WA2
#include <LEDA/internal/std/sgi_filebuf.h>
#endif
