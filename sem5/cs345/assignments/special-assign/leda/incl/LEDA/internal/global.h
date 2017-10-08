/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  global.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_GLOBAL_H
#define LEDA_GLOBAL_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600267
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/internal/system.h>
#include <LEDA/internal/std/limits.h>
#include <LEDA/internal/std/float.h>

#include <limits.h>



//------------------------------------------------------------------------------
// values and limits
//------------------------------------------------------------------------------


#if !defined(MAXINT)
//#define MAXINT std::numeric_limits<int>::max();
#define MAXINT INT_MAX
#endif

#if !defined(MAXDOUBLE)
//#define MAXDOUBLE std::numeric_limits<double>::max();
#define MAXDOUBLE DBL_MAX
#endif

#if LONG_BIT == 64 || defined(__ia64__) || defined(_LONG64) || defined(__amd64) || defined(__WIN64__)
#define WORD_LENGTH_64
#define WORD_LENGTH_LOG 6
#else
#define WORD_LENGTH_32
#define WORD_LENGTH_LOG 5
#endif


#define	LEDA_PI   3.14159265358979323846
#define	LEDA_PI_2 1.57079632679489661923



//------------------------------------------------------------------------------
// global types, constants, functions, and macros
//------------------------------------------------------------------------------

LEDA_BEGIN_NAMESPACE


#if defined(LEDA_NAMESPACE)

// internal template namespace (for swap,min,max ...) (s.n. 01/2006)

namespace leda_templates {}

using namespace leda_templates;

#endif




typedef void* GenPtr;    // generic pointer type

/*
typedef void* volatile GenPtr; 
*/


#ifndef nil
#define nil 0
#endif

#define leda_nil 0

#if !defined(LEDA_NAMESPACE)

namespace leda {
static const int behind = 0;
static const int before = 1;
static const int after = behind;
};

#else

const int behind = 0;
const int before = 1;
const int after = behind;

#endif


// deutsche Umlaute

const char uml_a = '\344';
const char uml_o = '\366';
const char uml_u = '\374';
const char uml_A = '\304';
const char uml_O = '\326';
const char uml_U = '\334';

/*
enum { uml_a = '\344', uml_o = '\366', uml_u = '\374',
       uml_A = '\304', uml_O = '\326', uml_U = '\334' };
*/



// global flags and names

extern __exportD int   node_data_slots;
extern __exportD int   edge_data_slots;
extern __exportD int   face_data_slots;

extern __exportD char*       option_list;
extern __exportD const char* version_string;
extern __exportD const char* copyright_string;
extern __exportD const char* copyright_window_string;
extern __exportD const char* compile_date;
extern __exportD const char* rootdir_name;
extern __exportD bool        use_karatsuba;

// global functions

extern __exportF void  LEDA_CHECK_VERSION();
extern __exportF void  compiler_string(char*);
extern __exportF void  write_log(const char*);
extern __exportF void  check_license();
extern __exportF void  user_log();
extern __exportF void  read_options();
extern __exportF void  sleep(double);
extern __exportF float used_time();
extern __exportF float used_time(float&);
extern __exportF void  trace_stack(ostream&);
extern __exportF int   trace_stack(char**);

LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600267
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

#endif
