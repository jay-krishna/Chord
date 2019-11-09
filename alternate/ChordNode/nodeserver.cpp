#include "nodeserver.h"

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

	return NULL;

}

void* NodeServer(void* pointer){
	Node *args = (Node *)pointer;
	// cout<<"server"<<endl;
	// args->nodedetails();

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
		td.socketfd = clientsockfd[sockcount];
		pthread_t th;
		pthread_create(&th,NULL,event,(void *)&td);
		pthread_detach(th);
		sockcount++;
	}
}