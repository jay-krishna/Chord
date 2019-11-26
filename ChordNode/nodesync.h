#ifndef __nodesync_H_INCLUDED__
#define __nodesync_H_INCLUDED__

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

extern bool closeall;

void fixfinger(void *fd);
// void senddata(Node *args);
// void changesuccpred(Node *args);
// void changepredsucc(Node *args);
void *stable(void *fd);

#endif