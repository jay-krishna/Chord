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

pthread_mutex_t lock0;
pthread_mutex_t lock1;

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

	long long int getid()
	{
		return this->nodeid;
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

	long long int successorid(){
		return this->successornode.first;
	}
	long long int predecessorid(){
		return this->predecessornode.first;
	}

	pair<string,long long int> predecessordetail(){
		return make_pair(this->predecessornode.second.first , this->predecessornode.second.second);	
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

struct threaddetails{
	Node *currentnode;
	int socketfd;
};

// creates a new connection given ip and portno
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


// event function will handle all task given to listner
void *event(void *fd){
	struct threaddetails *nn = (struct threaddetails *)fd;

	int clientsockfd = nn->socketfd;
	Node *args = nn->currentnode;
	
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

	else if(command[0] == "givepredecessor"){
		string msg="";
		
		pair<string,long long int> predetails = args->predecessordetail();
		long long int predid = args->predecessorid();
		
		msg = msg + to_string(predid) + " " + predetails.first + " " + to_string(predetails.second);
		send(clientsockfd,msg.c_str(),msg.size(),0);
	}

	else if(command[0] == "notify"){

		long long int n = args->getid();
		long long int n1 = atoi(command[3].c_str());

		long long int p = args->predecessorid(); 

		bool condition = false;
		cout<<"In nofitfy values "<<n <<" "<<n1<<" "<<p<<endl;
		if(p == -1){
			cout<<"predecessor is -1"<<endl;
			condition = true;
		}
		else{
			// n' belongsto (p,n)

			if(p == n && n1==p){
				cout<<"p1"<<endl;
				condition = false;
			}
			else if(p < n){
				if(p < n1 && n1 < n){
					cout<<"p2"<<endl;
					condition = true;
				}
				else{
					cout<<"p3"<<endl;
					condition = false;
				}
			}
			else if(p > n){
				if(!(n <= n1 && n1<=p)){
					cout<<"p4"<<endl;
					condition = true;
				}
				else{
					cout<<"p5"<<endl;
					condition = false;
				}
			}
			else{
				 condition=true;
			}
		}

		if(condition){
			cout<<"predecessor Updated"<<endl;
			pthread_mutex_lock(&lock0); 
			args->predecessor(command[1],atoi(command[2].c_str()),n1);
			pthread_mutex_unlock(&lock0); 
		}
	}
}

// this function will create a node socket and will serve all request given by other nodes.
void *listener(void *fd){ 
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

		int clientsockfd[100];
		int sockcount=0;

		while(1)
		{
			socklen_t clilen = sizeof(clientaddress);
			clientsockfd[sockcount] = accept(sockfd,(struct sockaddr *)&clientaddress,&clilen);
			//do all the task given to listener
			struct threaddetails td;
			td.currentnode = args;
			td.socketfd = clientsockfd[sockcount];https://github.com/mitul227/Chord-DHT
			pthread_t th;
			pthread_create(&th,NULL,event,(void *)&td);
			pthread_detach(th);
			sockcount++;
		}
}

void *stable(void *fd){
	
	Node *args = (Node *)fd;	
	long long int n = args->getid(); // myid

	while(true){

		// get successor details
		long long int x;
		long long int s = args->successorid();
		pair<string,long long int> neighbour = args->successordetail();

		// start connection with successor
		int sockfd = newconnection(neighbour.first , to_string(neighbour.second));

		string commandtosend = "givepredecessor"; // ask for predecessor
		char buffer[255];
		memset(buffer,'\0',sizeof(buffer));
		
		// ask for predecessor
		send(sockfd,commandtosend.c_str(),commandtosend.size(),0);
		
		recv(sockfd,buffer,sizeof(buffer), 0);
		cout << "msg recv from suc-1 " << buffer << endl; // msg should be predecessor "id ip port"
		string msg="";
		int i=0;
		while(buffer[i] != '\0'){
			msg = msg + buffer[i];
			i++;
		}
		
		close(sockfd);

		vector<string> pred = splitcommand(msg);
		x = atoi(pred[0].c_str()); // successor.predecessor stored in x
		bool condition = false;
		// check conditions
		if(x == -1){
			cout<<"c1"<<endl;
			condition = false;
		}
		else if(n == s && x==n){
			cout<<"c2"<<endl;
			condition = false;
		}
		else if(n < s){
			if(n < x && x < s){
				cout<<"c3"<<endl;
				condition = true;
			}
			else{
				cout<<"c4"<<endl;
				condition = false;
			}
		}
		else if(n > s){
			if(!(s <= x && x<=n)){
				cout<<"c5"<<endl;
				condition = true;
			}
			else{
				cout<<"c6"<<endl;
				condition = false;
			}
		}
		else{
			cout<<"c7"<<endl;
			condition = true;
		}

		if(condition){
			cout<<"Updated successor"<<endl;
			pthread_mutex_lock(&lock1); 
			args->successor(pred[1],atoi(pred[2].c_str()),x);
			pthread_mutex_unlock(&lock1); 
		}

		// new successor if above were true
		pair<string,long long int> newneighbour = args->successordetail();
		int newsockfd = newconnection(newneighbour.first , to_string(newneighbour.second));

		string notify = "notify " + args->getip() + " " + to_string(args->getnodeportno()) + " " + to_string(n);
		cout<<"Notify string "<<notify<<endl;
		send(newsockfd,notify.c_str(),notify.size(),0);
		close(newsockfd);
		sleep(15);
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
				pthread_t s;
				pthread_create(&s,NULL,stable,(void *)&currentnode);
				pthread_detach(s);

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
			pthread_t s;
			pthread_create(&s,NULL,stable,(void *)&currentnode);
			pthread_detach(s);

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