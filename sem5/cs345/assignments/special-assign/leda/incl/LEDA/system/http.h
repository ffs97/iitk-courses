/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  http.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_HTTP_H
#define LEDA_HTTP_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600237
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/core/string.h>
#include <LEDA/core/list.h>
#include <LEDA/system/socket.h>

LEDA_BEGIN_NAMESPACE


class __exportC http  {

  enum { buf_len = 2048 }; 

  string host;
  string fname;
  int port;
  int time_out;

  string error_msg;

  char buf[buf_len+1]; // buf[buf_len] sentinel
  char* first;
  char* last;

  int delimiter;

  int content_length;
  string content_type;

  int current_length;

  list<string> header;

  leda_socket sock;

  bool get(string& str);
  bool is_delimiter(char c) const;

public:

http(int timeout=10);

bool connect(string url);
void disconnect() { sock.disconnect(); }

void set_timeout(int t) { time_out = t; }

int    get_content_length() const { return content_length; }
string get_content_type() const { return content_type; }

list<string> get_header()  const { return header; }
int          get_timeout() const { return time_out; }
string       get_error()   const { return error_msg; }


int get(char* p, int sz);

int get(ostream& out, void (*progress_f)(int,int)=0);

bool get_line(string& line); 
bool get_string(string& s);
bool get_int(int& x);
bool get_float(double& x);

};


LEDA_END_NAMESPACE

#if LEDA_ROOT_INCL_ID == 600237
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


#endif
