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

#include "clientutil.h"

using namespace std;

int main(){

	string msg;
	string ip;
	string port;
	cout << "Enter IP to connect" << endl;
	getline(cin,ip);

	cout << "Enter Port no" << endl;
	getline(cin,port);

	while(true){

		prompt();
		getline(cin,msg);
		if(msg == ""){
			continue;
		}
		if(msg == "exit"){
			return 0;
		}
		int sockfd = newconnection(ip,port);
		send(sockfd,msg.c_str(),msg.size(),0);
		char buffer[200];
		memset(buffer,'\0',sizeof(buffer));

		recv(sockfd,buffer,sizeof(buffer), 0);
		cout << buffer << endl;
		close(sockfd);

	}
	return 0;	
}