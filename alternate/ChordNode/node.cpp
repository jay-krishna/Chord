#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include<math.h>
#include <openssl/sha.h>
#include<bits/stdc++.h>

#include "nodeclass.h"

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

void printprompt(){
	cout << "$ ";
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

int main()
{
	string inputcommand;
	string myip;
	int portno;
	cout << "Enter Node IP" << endl;
	cin >> myip;
	cout << "Enter Node Portno" << endl;
	cin >> portno;

	string temp;// junk enter char
	getline(cin,temp);//ignore

	Node currentnode = Node(myip,portno);
	while(true){
		printprompt();
		getline(cin,inputcommand);

		vector<string> command;
		command = splitcommand(inputcommand);

		if(command[0] == "create_ring"){ // createring
			
			if(currentnode.ringstatus() == false){
				
				string key = myip +":"+ to_string(portno);
				long long int id = gethash(key);			// currentnode nodeid, given by sha1 (key = ip+port)
				currentnode.setid(id);						// set all node values
				currentnode.setringstatus();
				currentnode.successor(myip,portno,id);
				currentnode.predecessor("",-1,-1);

				// launch thread to start listening for other nodes to join chord ring
				


				// launch thread for stabalization 
			}
			else{
				cout << "Node is already part of ring" << endl;
				}
		}

		else if(command[0] == "join_ring"){
			// if(currentnode.ringstatus() == false){

			// }
		}


		else{
			cout << "Wrong Command input" << endl;
		}
	}


	return 0;


}