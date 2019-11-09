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

int newconnection(string ip,string portno){
	long long int port = atoi(portno.c_str());
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0){cout << "Error Opening Socket";}
	server = gethostbyname(ip.c_str());
	bzero((char *)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);
	if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
		cout << "Error Connecting" << endl;
		return -1;
	}
	return sockfd;
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

void prompt(){
	cout << "$";
}

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