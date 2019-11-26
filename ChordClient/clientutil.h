#ifndef __clientutil_H_INCLUDED__
#define __clientutil_H_INCLUDED__

#include <iostream>
#include <vector>
#include <math.h>
#include <openssl/sha.h>
#include <pthread.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

int newconnection(string ip,string portno);

vector<string> splitcommand(string input);

void prompt();

#endif