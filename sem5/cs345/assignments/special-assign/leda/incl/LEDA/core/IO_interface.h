/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  IO_interface.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_IO_INTERFACE_H
#define LEDA_IO_INTERFACE_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600014
#include <LEDA/internal/PREAMBLE.h>
#endif

#include <LEDA/system/basic.h>
#include <LEDA/system/stream.h>

LEDA_BEGIN_NAMESPACE


class IO_interface_book{

int p;

public:

IO_interface_book(const string& header = ""):p(4)
{ cout.precision(p);}

~IO_interface_book(){ } 

void set_precision(int prec){ 
p = prec; 
cout.precision(p);
}

int read_int(const string& mes, const int n0 = 0) 
{ return n0;
}

double read_double(const string& mes, const double n0 = 0.0) 
{ return n0;
}

char read_char(const string& mes, const int c0 = ' ')
{ return c0;
}

void stop(){}

// WRITE_TABLE

void write_table(const string& mes, double T, string mes2 = "") 
{ cout << mes << T << mes2; cout.flush(); }

void write_table(const string& mes, int T,string mes2 = "") 
{ cout << mes << T << mes2; cout.flush(); }

void write_table(const string& mes) 
{ cout << mes; cout.flush(); }

void write_table(double T) 
{ cout << T; cout.flush(); }

void write_table(int  n) 
{ cout << n; cout.flush(); }

// WRITE DEMO has empty bodies

void write_demo(int k, const string& mes, double T, string mes2 = "")
{ }

void write_demo(int k, const string& mes, int T, string mes2 = "")
{ }

void write_demo(int k, const string& mes)
{ }

// default value of k is 2

void write_demo(const string& mes, double T, 
                        string mes2 = "")
{ write_demo(2,mes,T,mes2); }

void write_demo(const string& mes, int T, 
                               string mes2 = "")
{ write_demo(2,mes,T,mes2); }

void write_demo(const string& mes)
{ write_demo(2,mes); } 

};


class IO_interface_demo{

int p;

public:

IO_interface_demo(const string& header = ""):p(4)
{ cout.precision(p); }

~IO_interface_demo(){ cout <<"\n\n"; } 
    // to produce two empty lines at end of demo

void set_precision(int prec){ 
p = prec; 
cout.precision(p);
}

string break_lines(string mes)
{ string result;
  string blank = " ";
  
  while ( mes.length() > 0 )
  { string current_line;
    while ( true )
    { int k1 = mes.pos(blank);
      int k2 = mes.pos("\n");
      int k;
      if ( k1 == -1 || k2 == -1 ) k = leda_max(k1,k2);
      else k = leda_min(k1,k2);
      if ( k == -1 ) { current_line += mes; mes = ""; break; }
      if ( current_line.length() + k + 1 < 80 )
      { current_line += mes.head(k + 1); // first k+1 characters
        mes = mes.del(0,k);
        if ( k == k2 ) break;
        continue;
      }
      break;
    }
    if ( result.length() > 0 ) result += "\n";
    result += current_line;
  }
  return result;
}

int read_int(const string& mes, const int n0 = 0) 
{ cout << "\n\n" << break_lines(mes);
  int i; cin >> i;
  return i;
}

double read_double(const string& mes, const double n0 = 0.0) 
{ cout << "\n\n" << break_lines(mes);
  double i; cin >> i;
  return i;
}

char read_char(const string& mes, const char c0 = ' ') 
{ cout << "\n\n" << break_lines(mes);
  char i; cin >> i;
  return i;
}


void stop(){ cout <<"\n\nPress any key to proceed.\n";
             string s; cin >> s;
}


// WRITE_TABLE is empty

void write_table(const string& mes, double T, string mes2 = "")  {}
void write_table(const string& mes, int T, string mes2 = "")     {}
void write_table(const string& mes) {}
void write_table(double T) {}
void write_table(int  n) {}


// WRITE_DEMO

void write_demo(int k, const string& mes, double T, string mes2 = "")
{ while (k-- > 0) cout << "\n";
 cout << break_lines(mes) << " " << string("%.2f",T) << " " << break_lines(mes2); 
 cout.flush();
}

void write_demo(int k, const string& mes, int T, string mes2 = "")
{ while (k-- > 0) cout << "\n";
 cout << break_lines(mes) << " " << T << " " << break_lines(mes2); 
 cout.flush();
}

void write_demo(int k, const string& mes)
{ while (k-- > 0) cout << "\n";
  cout << break_lines(mes); 
  cout.flush();
}

// default value of k is two

void write_demo(const string& mes, double T, string mes2 = "")
{ write_demo(2,mes,T,mes2); }

void write_demo(const string& mes, int T, string mes2 = "")
{ write_demo(2,mes,T,mes2); }

void write_demo(const string& mes)
{ write_demo(2,mes); }

};


#ifdef BOOK
#define IO_interface IO_interface_book
#else
#define IO_interface IO_interface_demo
#endif



#if LEDA_ROOT_INCL_ID == 600014
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif

LEDA_END_NAMESPACE

#endif

