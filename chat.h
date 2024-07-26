#ifndef CHAT_H
#define CHAT_H

#include <arpa/inet.h>

extern char nickname[50];
extern int port;
extern char ip[16];
extern int sock;
extern sockaddr_in broadcast_addr;

void* receive_messages(void* arg);
void* send_messages(void* arg);
int setup_socket();
void get_user_input();

#endif // CHAT_H
