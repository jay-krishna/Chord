#include "nodesync.h"
#include "util.h"

pthread_mutex_t lock1;

void fixfinger(void *fd){

	// while(1){
	Node *args = (Node *)fd;
	vector<pair<long long int, pair<string,long long int>>> newfinger;

	long long int n = args->getid();
	long long int mod = pow(2,4);

		for(long long int j=0;j<4;j++)
		{
			long long int requestid = (n +(long long int) pow(2,j)) % mod;
			long long int result = args->findsuccessor(requestid);

		if(result == -1){
			
			pair<string,long long int> ipport = args->successordetail();
			int newsockfd = newconnection(ipport.first,to_string(ipport.second));
			if(newsockfd == -1){
				 cout << "newsofd is -1 connection loss fixfinger" <<  endl;
			}
			string commandtosend = "findsuccessor " + to_string(requestid); // command to be send to listner "findsuccessor nodeid"
			
			char buffer[255];
			memset(buffer,'\0',sizeof(buffer));
			send(newsockfd,commandtosend.c_str(),commandtosend.size(),0);
			recv(newsockfd,buffer,sizeof(buffer), 0);
			string msg = "";
			int kk=0;
			while(buffer[kk] != '\0'){
				msg = msg + buffer[kk];
				kk++;
			}
			// cout << "new connection msg " << msg << endl;
			// send is "nodeid ip port"
			vector<string> ss = splitcommand(msg);
			newfinger.push_back(make_pair(atoi(ss[0].c_str()),make_pair(ss[1],atoi(ss[2].c_str()))));
			shutdown(newsockfd,0);close(newsockfd);

		}
		else{

			pair<string,long long int> ipport = args->successordetail();
			long long int succ = args->successorid();
			newfinger.push_back(make_pair(succ,make_pair(ipport.first,ipport.second)));

		}

		args->updatefingertable(newfinger);
		sleep(1);
	}
	
}

void *stable(void *fd){
	
	Node *args = (Node *)fd;	
	long long int n = args->getid(); // myid

	while(closeall){

		// get successor details
		long long int x;
		long long int s = args->successorid();
		pair<string,long long int> neighbour = args->successordetail();

		// start connection with successor
		int sockfd = newconnection(neighbour.first , to_string(neighbour.second));
		if(sockfd == -1){
				 cout << "newsofd is -1 connection loss in stabalization" <<  endl;
			}

		string commandtosend = "givepredecessor"; // ask for predecessor
		char buffer[1024];
		memset(buffer,'\0',sizeof(buffer));
		
		// ask for predecessor
		send(sockfd,commandtosend.c_str(),commandtosend.size(),0);
		recv(sockfd,buffer,sizeof(buffer), 0);
		string msg="";
		int i=0;
		while(buffer[i] != '\0'){
			msg = msg + buffer[i];
			i++;
		}
		shutdown(sockfd,0);
		close(sockfd);
		vector<string> pred = splitcommand(msg);
		x = atoi(pred[0].c_str()); // successor.predecessor stored in x
		bool condition = false;
		// check conditions
		if(x == -1){
			condition = false;
		}
		else if(n == s && x==n){
			condition = false;
		}
		else if(n < s){
			if(n < x && x < s){
				condition = true;
			}
			else{
				condition = false;
			}
		}
		else if(n > s){
			if(!(s <= x && x<=n)){
				condition = true;
			}
			else{
				condition = false;
			}
		}
		else{
			condition = true;
		}

		if(condition){
			pthread_mutex_lock(&lock1); 
			args->successor(pred[1],atoi(pred[2].c_str()),x);
			pthread_mutex_unlock(&lock1); 
		}

		// new successor if above were true
		pair<string,long long int> newneighbour = args->successordetail();
		int newsockfd = newconnection(newneighbour.first , to_string(newneighbour.second));
		if(newsockfd == -1){
				 cout << "newsofd is -1 connection loss in stabalization-2" <<  endl;
			}

		string notify = "notify " + args->getip() + " " + to_string(args->getnodeportno()) + " " + to_string(n);
		send(newsockfd,notify.c_str(),notify.size(),0);
		shutdown(newsockfd,0);
		close(newsockfd);
		fixfinger(fd);
		sleep(5);
	}
	
	return NULL;
}