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
#include "help.cpp"

using namespace std;

class Node{
private:
	string nodeip;
	long long int nodeportno;
	long long int nodeid;
	bool connectedtoring;
	// vector<pair<int,string>> fingertable;
	pair<long long int, pair<string,long long int>> successornode;// pair.first = hashvalue // pair.second.first = ip // pair.second.second = portno
	pair<long long int, pair<string,long long int>> predecessornode;

public:
	Node(string ip,int portno){
		this->nodeip = ip;
		this->nodeportno = portno;
		connectedtoring = false;
	}

	string getip(){
		return nodeip;
	}

	long long int getnodeportno(){
		return nodeportno;
	}

	bool ringstatus(){
		return connectedtoring;
	}

	void setid(long long int id){
		this->nodeid = id;
	}

	void setringstatus()
	{
		this->connectedtoring = true;
	}

	void successor(string ip,long long int portno,long long int id){
		this->successornode.first = id;
		this->successornode.second.first = ip;
		this->successornode.second.second = portno;
	}

	void predecessor(string ip,long long int portno,long long int id){
		this->predecessornode.first = id;
		this->predecessornode.second.first = ip;
		this->predecessornode.second.second = portno;
	}

	pair<string,long long int> successordetail(){
		return make_pair(this->successornode.second.first , this->successornode.second.second);
	}

	void nodedetails()
	{
		cout << "Node ip is " << this->nodeip << endl;
		cout << "Node portno is " << this->nodeportno << endl;
		cout << "Node nodeid is " << this->nodeid << endl;
		cout << "Node successornode id is " << this->successornode.first << " ip and port is " << successornode.second.first << " " << successornode.second.second << endl;
		cout << "Node predecessornode id is " << this->predecessornode.first << " ip and port is " << predecessornode.second.first << " " << predecessornode.second.second << endl;
	}

	long long int findsuccessor(long long int requestid){
		long long int s = this->successornode.first;
		long long int n = this->nodeid;

		if(s == n){
			return n;
		}
		else if(n < s){
			if(n < requestid && requestid <= s){
				return s;
			}
			else{
				return -1;
			}
		}
		else{
			if(!(s < requestid && requestid <= n)){
				return s;
			}
			else{
				return -1;
			}
		}
	}

};

int newconnection(string ip, string portno){
	
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

void *listener(void *fd){ // this function will create a node socket and will serve all request given by other nodes.
		Node *args = (Node *)fd;
		
		string ip = args->getip();
		long long int portno = args->getnodeportno();
		cout << "Listner is on " << ip << " " << portno << endl;
		int sockfd;
		struct sockaddr_in serveraddress , clientaddress;
		sockfd = socket(AF_INET,SOCK_STREAM,0);// socket created
		if(sockfd < 0){
			cout << "error opening socket" << endl;
			return NULL;
		}

		bzero((char *)&serveraddress,sizeof(serveraddress));
		serveraddress.sin_family = AF_INET;
		serveraddress.sin_addr.s_addr = INADDR_ANY;
		serveraddress.sin_port= htons(portno);
	    serveraddress.sin_addr.s_addr = inet_addr(ip.c_str()); 
		
		//bind and start listning
		if(bind(sockfd,(struct sockaddr *)&serveraddress, sizeof(serveraddress)) < 0){
			cout << "error bind socket" << endl;
		}
		listen(sockfd,5);
		
		while(1)
		{
			socklen_t clilen = sizeof(clientaddress);
			int clientsockfd = accept(sockfd,(struct sockaddr *)&clientaddress,&clilen);
			cout << "clientsockfd " << clientsockfd << endl;
			//do all the task given to listener
			
			char buffer[1024];
			memset(buffer,'\0',1024);
			recv(clientsockfd,buffer,sizeof(buffer), 0);
			string input;
			int i=0;
			while(buffer[i] != '\0'){
				input = input + buffer[i];
				i++;
			}
			cout << "command recv " << input << endl;
			vector<string> command = splitcommand(input);
			// recv command is "findsuccessor nodeid"
			if(command[0] == "findsuccessor"){

				long long int rid = atoi(command[1].c_str());
				long long int result = args->findsuccessor(rid);
				if(result == -1){

						pair<string,long long int> ipport = args->successordetail();
						int newsockfd = newconnection(ipport.first,to_string(ipport.second));
						string commandtosend = "findsuccessor " + to_string(rid); // command to be send to listner "findsuccessor nodeid"
						char buffer[255];
						memset(buffer,'\0',sizeof(buffer));
						send(newsockfd,commandtosend.c_str(),commandtosend.size(),0);
						recv(newsockfd,buffer,sizeof(buffer), 0);
						string msg = "";
						int j=0;
						while(buffer[j] != '\0'){
							msg = msg + buffer[j];
							j++;
						}
						cout << "msg from neighbour " << msg << endl;
						send(clientsockfd,msg.c_str(),msg.size(),0);

				}
				else{
					
					string r = to_string(result);
					pair<string,long long int> otherdetails = args->successordetail();
					r = r + " " + otherdetails.first + " " + to_string(otherdetails.second);
					// send is "nodeid ip port"
					send(clientsockfd,r.c_str(),r.size(),0);	
				}
				
			}

		}
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
	long long int idd = gethash(myip+":"+to_string(portno));
	currentnode.setid(idd);

	while(true){
		printprompt();
		getline(cin,inputcommand);

		if(inputcommand == ""){ // if no command is been entered
			continue;
		}

		vector<string> command;
		command = splitcommand(inputcommand);

		if(command[0] == "create_ring"){ // createring
			
			if(currentnode.ringstatus() == false){
				
				string key = myip +":"+ to_string(portno);	// node id = sha("ip:port")
				long long int id = gethash(key);			// currentnode nodeid, given by sha1 (key = ip+port)
				currentnode.setid(id);						// set all node values
				currentnode.setringstatus();
				currentnode.successor(myip,portno,id);
				currentnode.predecessor("",-1,-1);

				// launch thread to start listening for other nodes to join chord ring 
				pthread_t l;
				pthread_create(&l,NULL,listener,(void *)&currentnode);
				pthread_detach(l);
				
				// launch thread for stabalization

			}
				
			else{
				cout << "Node is already part of ring" << endl;
				}
		}
		
		else if(command[0] == "display"){
			currentnode.nodedetails();
			}
		
		else if(command[0] == "join"){

			string iptojoin = command[1];
			string portnotojoin = command[2];
			long long int id = gethash(currentnode.getip() + ":" + to_string(currentnode.getnodeportno()));

			int sockfd = newconnection(command[1],command[2]);
			
			string commandtosend = "findsuccessor " + to_string(id); // command to be send to listner "findsuccessor nodeid"

			char buffer[255];
			memset(buffer,'\0',sizeof(buffer));
			send(sockfd,commandtosend.c_str(),commandtosend.size(),0);
			cout << "commandtosend " << commandtosend<< endl;

			recv(sockfd,buffer,sizeof(buffer), 0);
			cout << "msg recv at outer node " << buffer << endl;
			string succid="";
			int i=0;
			while(buffer[i] != '\0'){
				succid = succid + buffer[i];
				i++;
			}
			vector<string> cc = splitcommand(succid);

			string temp1 = cc[0];
			string temp2 = cc[2];
			currentnode.successor(cc[1],atoi(temp2.c_str()),atoi(temp1.c_str()));

			currentnode.predecessor("",-1,-1);
			currentnode.setid(id);
			currentnode.setringstatus();
			currentnode.nodedetails();
			close(sockfd);
			
			// sock program 
			// send hash id to chord ring whose ip and port is known
			// recv successor id from that node
			// setnode details like successor list predecco...
			
			// release thread -> listening purpose
			pthread_t ll;
			pthread_create(&ll,NULL,listener,(void *)&currentnode);
			pthread_detach(ll);
			// release thread -> stabalization

			}

		else if(command[0] == "temp"){

			string ppp = command[2];
			string idpp = command[3];
			currentnode.successor(command[1],atoi(ppp.c_str()),atoi(idpp.c_str()));
		}
		else{
			cout << "Wrong Command input" << endl;
		}
	}


	return 0;


}