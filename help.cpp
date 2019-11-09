#include <iostream>
#include <vector>
#include<math.h>
#include <openssl/sha.h>
#include<pthread.h>
#include<bits/stdc++.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<netinet/in.h>
#include<netdb.h>

using namespace std;


void printprompt(){
	cout << "$ ";
	}


long long int gethash(string key){
    unsigned char obuf[41];
    char finalHash[41];
    string keyHash = "";
    int i;
    long long int mod = pow(2,11); /// here m value is equal to 11 so total 0 to 2^11 -1 position available for nodes
 
    unsigned char unsigned_key[key.length()+1];
    for(i=0;i<key.length();i++){
        unsigned_key[i] = key[i];
    }
    unsigned_key[i] = '\0';


    SHA1(unsigned_key,sizeof(unsigned_key),obuf);
    for (i = 0; i < 11/8; i++) {
        sprintf(finalHash,"%d",obuf[i]);
        keyHash += finalHash;
    }

    long long int hash = stoll(keyHash) % mod;

    return hash;
}




vector<string> splitcommand(string input){ /// splits command via space and returns vector of string
	stringstream ss(input);
	vector<string> result;
	string temp;
	while(ss >> temp){
		result.push_back(temp);
	}
	return result;
}
