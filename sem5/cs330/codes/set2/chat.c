#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <assert.h>
#include <stdio.h>
#include <sys/time.h>

#define MSGKEY 19

typedef struct {
        long type;
        char buf[2048];
} msg_t;

void mygetline (char *buf)
{
   int pointer=0;

   while (1) {
      scanf("%c", &buf[pointer]);
      if (buf[pointer] == '\n') return;
      pointer++;
   }
}

void printline(char *buf)
{
   int pointer=0;

   while (1) {
      printf("%c", buf[pointer]);
      if (buf[pointer] == '\n') return;
      pointer++;
   }
}

int main(int argc, char *argv[])
{
    char *p=getenv("USER"), buf[1024];
    msg_t msg;
    int msgid;
    long send_msg_type = 0, recv_msg_type = 0;
    int i;
    struct timeval tp;
    unsigned long long mytime, othertime;

    if (p == NULL) {
       printf("Cannot get screenname. Aborting...\n");
       exit(0);
    }

    if (argc != 2) {
       printf("Usage: chat <screenname of chat partner>\n");
       exit(0);
    }

    // Works only if the first four characters of all screennames are distinct
    // Prepare message types for communication
    i = 0;
    while ((argv[1] != NULL) && (i < 4)) {
       send_msg_type = send_msg_type*128 + toascii(argv[1][0]);
       argv[1]++; i++;
    }

    i = 0;
    char *q=p;
    while ((q != NULL) && (i < 4)) {
       recv_msg_type = recv_msg_type*128 + toascii(q[0]);
       q++; i++;
    }
    msgid = msgget (MSGKEY, 0777 | IPC_CREAT);

    // Clean up any message of the type I will send from the message queue
    // send_msg_type is used for initial handshake and send_msg_type+1 is used for chat messages
    // Effectively flushes the entire message queue
    while (msgrcv(msgid, &msg, 2048, send_msg_type, IPC_NOWAIT) != -1);
    while (msgrcv(msgid, &msg, 2048, send_msg_type+1, IPC_NOWAIT) != -1);

    // Establish who gets to talk first (alternating chat session)

    // PROTOCOL: Send a dummy message. The first sender gets to talkfirst.

    // Post a dummy message carrying send time
    gettimeofday(&tp, NULL);
    msg.type = send_msg_type;
    *((unsigned long long*)msg.buf) = tp.tv_sec*1000000+tp.tv_usec;
    msgsnd(msgid, &msg, 2048, 0);
    msgrcv(msgid, &msg, 2048, recv_msg_type, 0);
    mytime = tp.tv_sec*1000000+tp.tv_usec;
    othertime = *((unsigned long long*)msg.buf);
    
    if (othertime > mytime) {
       // I am not the last sender; so I get to talk first.
       
       // We are ready to start the chat session
       // I talk first.

       // Make sure the chat message types are different from the initial message types
       while(1) {
          printf("%s>", p);
          mygetline(buf);
          msg.type = send_msg_type+1;
          sprintf(msg.buf, "%s> %s", p, buf);
          msgsnd(msgid, &msg, 2048, 0);
          msgrcv(msgid, &msg, 2048, recv_msg_type+1, 0);
          printline(msg.buf);
       }
    }
    else {
       // I am the last sender. The other guy gets to talk first.
       
       // We are ready to start the chat session
       while(1) {
          msgrcv(msgid, &msg, 2048, recv_msg_type+1, 0);
          printline(msg.buf);
          printf("%s>", p);
          mygetline(buf);
          msg.type = send_msg_type+1;
          sprintf(msg.buf, "%s> %s", p, buf);
          msgsnd(msgid, &msg, 2048, 0);
       }
    }
    return 0;
}
