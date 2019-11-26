#ifndef __nodeserver_H_INCLUDED__
#define __nodeserver_H_INCLUDED__

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <utility>

#include "nodeclass.h"
#include "util.h"

using namespace std;

struct threaddetails{
	Node *currentnode;
	int socketfd;
};

void senddata(Node *args);
void changesuccpred(Node *args);
void changepredsucc(Node *args);
// int newconnection(string ip, string portno);
void *event(void *fd);
void* NodeServer(void* pointer);

#endif