/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  socket.h
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef LEDA_SOCKET_H
#define LEDA_SOCKET_H

#if !defined(LEDA_ROOT_INCL_ID)
#define LEDA_ROOT_INCL_ID 600230
#include <LEDA/internal/PREAMBLE.h>
#endif

/*{\Manpage {leda_socket} {} {Sockets} {S}}*/


#include <LEDA/system/basic.h>
#include <LEDA/system/stream.h>

LEDA_BEGIN_NAMESPACE


class __exportC leda_socket {

/*{\Mdefinition
A data {\bf packet} consists of a sequence of bytes (in {\tt C} of type
{\tt unsigned char}) $c_0,c_1,c_2,c_3,x_1,\ldots,x_n$. The first four 
bytes encode the number $n$ of the following bytes such that
$n = c_0 + c_1\cdot 2^8 + c_2\cdot 2^{16} + c_3\cdot 2^{24}$.
The LEDA data type {\tt leda\_socket} offers, in addition to the
operations for establishing a socket connection, functions for
sending and receiving packets across such a connection. 
It is also possible to set a receive limit; if such a receive limit is set,
messages longer than the limit will be refused. If the limit is negative (default),
no messages will be refused. \\
In particular, the following operations are available:
}*/



static int leda_socket_init;

string hostname;
string ipaddr;
int    portnum;
int    sockfd0;
int    sockfd;
int    qlength;
int    timeout;

int    receive_limit;

string error_str;
string infile_name;
string outfile_name;

ofstream* out_stream;
ifstream* in_stream;

void (*wait_error_handler)(leda_socket&,string);

void (*receive_handler)(leda_socket&,int,int);
void (*send_handler)(leda_socket&,int,int);

int  receive_size();
int  receive_raw(char*,int);

void send_size(int sz);
void error(string msg);


public:

/*{\Mcreation}*/

leda_socket(const char* host, int port);
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| associated with host name |host| and port number |port|.}*/ 
 
leda_socket(const char* host); 
/*{\Mcreate creates an instance |\Mvar| of type |\Mname| associated with host name |host|.}*/
 
leda_socket(); 
/*{\Mcreate creates an instance |\Mvar| of type |\Mname|.}*/ 
 
 
~leda_socket();

/*{\Moperations}*/

void init();

ofstream& ostr() { return *out_stream; }
ifstream& istr() { return *in_stream;  }

void set_host(const char* host) { hostname = host; }
/*{\Mop  sets the host name to |host|. }*/

void set_port(int port)         { portnum = port;  }
/*{\Mop  sets the port number to |port|. }*/

int get_limit() const { return receive_limit; }
/*{\Mop  returns the receive limit parameter.}*/

void set_limit(int limit)       { receive_limit = limit; }
/*{\Mop  sets the receive limit parameter to |limit|. If a negative limit is set,
the limit parameter will be ignored. }*/


void set_qlength(int len)       { qlength = len;   }
/*{\Mop  sets the queue length to |len|. }*/

void set_timeout(int sec)       { timeout = sec;   }
/*{\Mop  sets the timeout interval to |sec| seconds. }*/

void set_error_handler(void (*f)(leda_socket&,string)) 
{ wait_error_handler = f; }
/*{\Mop  sets the error handler to function |f|. }*/

void set_receive_handler(void (*f)(leda_socket&,int,int)) 
{ receive_handler = f; }
/*{\Mop  sets the receive handler to function |f|. }*/

void set_send_handler(void (*f)(leda_socket&,int,int)) 
{ send_handler = f; }
/*{\Mop  sets the send handler to function |f|. }*/


string get_host()  const  { return hostname; }
/*{\Mop  returns the host name. }*/

int    get_port()  const  { return portnum;  }
/*{\Mop  returns the port number. }*/

int    get_timeout() const { return timeout;  }
/*{\Mop  returns the timeout interval length in seconds. }*/

int    get_qlength() const { return qlength;  }
/*{\Mop  returns the queue length. }*/

string get_ip()  const   { return ipaddr;    }
/*{\Mop  returns the ip address. }*/

string get_error() const { return error_str; }


// client side
bool connect1(int sec);

bool connect(int sec = 10) { return connect1(sec); }
/*{\Mop  tries to establish a connection from a client to a server. If the 
         connection can be established within $sec$ seconds, the operation 
         returns |true| and  |false| otherwise.}*/

// server side
bool listen();
/*{\Mop  creates a socket endpoint on the server, performs address binding and signals readiness of a
server to receive data.}*/

bool accept();
/*{\Mop  the server takes a request from the queue.}*/

void disconnect();
/*{\Mop  ends a connection.}*/

void send();

/*{\Mtext {\bf Sending and receiving packets}}*/

void send_file(string fname);
/*{\Mop  sends the contents of file |fname|.}*/

void send_bytes(char* buf, int numb);
/*{\Mop  sends |numb| bytes starting at address |buf|.}*/

void send_string(string msg);
/*{\Mop  sends string |msg|.}*/

void send_int(int x);
/*{\Mop  sends (a text representation of) integer $x$.}*/


bool receive();

bool receive_file(string fname);
/*{\Mopl receives data and writes it to file |fname|.}*/

bool receive(ostream& out);

char* receive_bytes(int& numb);
/*{\Mopl receives |numb| bytes. The function allocates memory and returns the first address
of the allocated memory. |numb| is used as the return parameter for the number of received bytes.}*/

int receive_bytes(char* buf, int buf_sz);
/*{\Mopl receives at most |buf_sz| bytes and writes them into the buffer |buf|.
         It returns the number of bytes supplied by the sender (maybe more 
		 than |buf_sz|), or -1 in case of an error.}*/

bool receive_string(string& s);
/*{\Mopl receives string |s|.}*/

bool receive_int(int& x);
/*{\Mop  receives (a text representation of) an integer and stores its
         value in $x$.}*/


bool wait(string s);
/*{\Mopl returns |true|, if |s| is received, |false| otherwise.}*/


int    read_string(char* buf, int sz);
int    read_string(char* buf, int sz, int timeout);
string read_string();
void   write_string(string s);

};


/*{\Mtext
\bigskip
The following template functions can be used to send/receive objects supporting 
input and output operators for iostreams.
}*/

// templates for sending/ receiving objects ...
template<class T>
void socket_send_object(const T& obj, leda_socket& sock)
/*{\Mfuncl
sends |obj| to the connection partner of |sock|. 
}*/ 
{
 string_ostream OS;
 OS << obj;
 OS << ends;
 sock.send_string(OS.str());
}


template<class T>
void socket_receive_object(T& obj, leda_socket& sock)
/*{\Mfuncl
receives |obj| from the connection partner of |sock|.
}*/ 
{
 string St;
 sock.receive_string(St);
 string_istream IS(St.cstring());
 IS >> obj;
}


#if LEDA_ROOT_INCL_ID == 600230
#undef LEDA_ROOT_INCL_ID
#include <LEDA/internal/POSTAMBLE.h>
#endif


LEDA_END_NAMESPACE

#endif
