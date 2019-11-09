#ifndef __util_H_INCLUDED__
#define __util_H_INCLUDED__

#include <iostream>
#include <vector>
#include <string>
#include <openssl/sha.h>
#include <math.h>
#include <sstream>

using namespace std;

long long int gethash(string key);
void printprompt();
vector<string> splitcommand(string input);

#endif