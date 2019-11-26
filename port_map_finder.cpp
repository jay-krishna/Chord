#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include<math.h>
#include <fstream>
#include <openssl/sha.h>
#include<bits/stdc++.h>

using namespace std;

long long int gethash(string key){
    unsigned char obuf[41];
    char finalHash[41];
    string keyHash = "";
    int i;
    long long int mod = pow(2,4); /// here m value is equal to 11 so total 0 to 2^11 -1 position available for nodes
 
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

void prepare(){
	for(int i=0;i<16;++i){
		string name=to_string(i);
		name="files/"+name+".txt";
		ofstream f;
		f.open(name,ofstream::out);
		f.close();
	}
}

void driver(int i){
	string ip="127.0.0.1:"+to_string(i);
	string value=to_string(gethash(ip));
	string name="files/"+value+".txt";
	// cout<<name<<endl;
	// cout<<value<<endl;

	ofstream f;
	f.open(name,ofstream::app);
	f<<to_string(i)<<endl;
	f.close();
}

int main(){
	// string ip="127.0.0.1:1000";
	prepare();

	// driver(1000);
	for(int i=10000;i<10500;++i){
		driver(i);
	}
	// cout<<gethash(ip)<<endl;
	return 0;
}