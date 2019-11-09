#ifndef __util_H_INCLUDED__
#define __util_H_INCLUDED__

#include <iostream>
#include <vector>
#include <string>
#include <openssl/sha.h>
#include <math.h>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

using namespace std;

long long int gethash(string key);
void printprompt();
vector<string> splitcommand(string input);
int newconnection(string ip, string portno);
#endif