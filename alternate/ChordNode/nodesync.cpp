#include "nodesync.h"
#include "util.h"

pthread_mutex_t lock1;

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
		// cout << "msg recv from suc-1 " << buffer << endl; // msg should be predecessor "id ip port"
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
			// cout<<"c1"<<endl;
			condition = false;
		}
		else if(n == s && x==n){
			// cout<<"c2"<<endl;
			condition = false;
		}
		else if(n < s){
			if(n < x && x < s){
				// cout<<"c3"<<endl;
				condition = true;
			}
			else{
				// cout<<"c4"<<endl;
				condition = false;
			}
		}
		else if(n > s){
			if(!(s <= x && x<=n)){
				// cout<<"c5"<<endl;
				condition = true;
			}
			else{
				// cout<<"c6"<<endl;
				condition = false;
			}
		}
		else{
			// cout<<"c7"<<endl;
			condition = true;
		}

		if(condition){
			// cout<<"Updated successor"<<endl;
			// pthread_mutex_lock(&lock1); 
			args->successor(pred[1],atoi(pred[2].c_str()),x);
			// pthread_mutex_unlock(&lock1); 
		}

		// new successor if above were true
		pair<string,long long int> newneighbour = args->successordetail();
		int newsockfd = newconnection(newneighbour.first , to_string(newneighbour.second));

		string notify = "notify " + args->getip() + " " + to_string(args->getnodeportno()) + " " + to_string(n);
		// cout<<"Notify string "<<notify<<endl;
		send(newsockfd,notify.c_str(),notify.size(),0);
		close(newsockfd);
		sleep(15);
	}

}