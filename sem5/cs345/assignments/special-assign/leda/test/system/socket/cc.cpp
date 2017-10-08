/*******************************************************************************
+
+  LEDA 6.3  
+
+
+  cc.c
+
+
+  Copyright (c) 1995-2010
+  by Algorithmic Solutions Software GmbH
+  All rights reserved.
+ 
*******************************************************************************/



#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>


#if defined(LEDA_STD_IO_HEADERS)
using std::cout;
using std::cin;
using std::endl;
#endif



#define AGD_PORT 9660



// send-functions

void send_size(int sock_fd, int sz)
{ unsigned char sz_bytes[4];
  int i=0;
  while (i < 4)
  { sz_bytes[i++] = sz % 256;
    sz /= 256;
   }
  send(sock_fd,sz_bytes,4,0); // send 4 bytes
}

void send_string(int sock_fd, const char* str)
{ unsigned int len = strlen(str);
  send_size(sock_fd,len);
  send(sock_fd,str,len,0);
}

void send_file(int sock_fd, const char* fname) 
{ char buf[1024];
  int file_fd,n;

  struct stat stat_buf;
  if (stat(fname,&stat_buf) != 0)
  { fprintf(stderr, "input file does not exist.\n");
    exit(1);
   }

  send_size(sock_fd,(int)stat_buf.st_size);

  file_fd = open(fname,O_RDONLY);
  while ((n = read(file_fd,buf,1024)) > 0) send(sock_fd,buf,n,0);
  close(file_fd);
}



// receive-functions

int receive_size(int sock_fd)
{ unsigned char sz_bytes[4];
  int sz=0, f=1, i=0;
  recv(sock_fd,sz_bytes,4,0); // get 4 bytes
  while (i < 4)
  { sz += f * sz_bytes[i++];
    f *= 256;
   }
  return sz;
}


void receive_string(int sock_fd, char* buf, int buf_len)
{ int len = receive_size(sock_fd);
  assert(len < buf_len);
  recv(sock_fd,buf,len,0);
  buf[len] = '\0';
}


void receive_file(int sock_fd, const char* fname) 
{ int buf[1024];
  int sz = receive_size(sock_fd);
  int file_fd = open(fname,O_WRONLY|O_CREAT,S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR);
  while (sz > 0)
  { int n = recv(sock_fd,buf,1024,0);
    write(file_fd,buf,n);
    sz -= n;
   }
}



void wait_string(int sock_fd, const char* str) 
{ char buf[512];
  receive_string(sock_fd,buf,512);

  if (strcmp(buf,str) == 0) return;

  if (strcmp(buf,"error") == 0)  // "error" instead of str
  { // retrieve error message
    send_string(sock_fd,"result");
    receive_string(sock_fd,buf,512);
    fprintf(stderr,"AGD Error:\n");
    fprintf(stderr,"%s\n",buf);
   }
  else
  { // something else instead of str
    fprintf(stderr,"expected: %s\n",str);
    fprintf(stderr,"received: %s\n",buf);
    fprintf(stderr,"\n");
   }

  exit(1);
}






int main(int argc, char *argv[])
{
  struct sockaddr_in serv_addr;
  struct hostent* hp;
  int sock_fd;


  if (argc != 2 && argc != 4 && argc != 5) {
    fprintf(stderr,"\n");
    fprintf(stderr,"usage:\n");
    fprintf(stderr,"\n");
    fprintf(stderr,"%s host\n",argv[0]);
    fprintf(stderr,"\t prints list of available algorithms.\n");
    fprintf(stderr,"\n");
    fprintf(stderr,"%s host alg infile [outfile]\n",argv[0]);
    fprintf(stderr,"\t runs algorithm alg for graph in infile\n");
    fprintf(stderr,"\t writes output to stdout or outfile.\n");
    fprintf(stderr,"\n");
    exit(1);
  }

  

  /* create endpoint */
  if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror(NULL);
    exit(1);
  }

  /* connect to server */
  serv_addr.sin_family = AF_INET;
  
printf("get hp\n");

  if(!(hp = gethostbyname(argv[1])))
  { perror(NULL);
    exit(1);
  }

  bcopy(hp->h_addr,(char *)&serv_addr.sin_addr,hp->h_length);
  
  serv_addr.sin_port = htons(AGD_PORT);


printf("before connect\n");

  int res = connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

printf("connect = %d\n",res);

  if (res < 0) 
  { perror(NULL);
    exit(1);
   }

printf("after connect\n");



  if (argc == 2) 
  { 
    // get list of algorithms

    send_string(sock_fd,"c_client");        // send name of client
    wait_string(sock_fd,"ok");
    send_string(sock_fd,"list");            // send cmd "list"
    wait_string(sock_fd,"ok");
    send_string(sock_fd,"result");          // send cmd "result"
    receive_file(sock_fd,"/dev/tty");       // receive and show list
    return 0;
   }




// argc > 2   
//
// argv[2]: algorithm 
// argv[3]: gml input file
// argv[4]: gml output file (optional)


  send_string(sock_fd,"c_client");    // send name of client
  wait_string(sock_fd,"ok");

  send_string(sock_fd,"layout");      // send cmd "layout"
  wait_string(sock_fd,"ok");

  send_string(sock_fd,argv[2]);       // send name of layout algorithm 
  wait_string(sock_fd,"ok");


  // send options
  send_string(sock_fd,"options"); // send cmd "options"
  wait_string(sock_fd,"ok");

  send_string(sock_fd,"option1 value1  option2 value2  option3 value3");
  wait_string(sock_fd,"ok");

  send_string(sock_fd,"gml");     // send cmd "gml"
  wait_string(sock_fd,"ok");

  send_file(sock_fd,argv[3]);     // send input graph
  wait_string(sock_fd,"ok");

  send_string(sock_fd,"result"); // get new layout and write it to outfile
  if (argc > 4)
     receive_file(sock_fd,argv[4]);
  else
     receive_file(sock_fd,"/dev/tty");

  close(sock_fd);

  return 0;
}
