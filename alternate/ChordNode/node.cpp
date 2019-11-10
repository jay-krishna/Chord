#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include<math.h>
#include <openssl/sha.h>
#include<bits/stdc++.h>
#include <pthread.h>

#include "nodeclass.h"
#include "util.h"
#include "nodeserver.h"
#include "nodesync.h"

using namespace std;

int main()
{
	string inputcommand;
	string myip;
	int portno;
	cout << "Enter Node IP" << endl;
	cin >> myip;
	cout << "Enter Node Portno" << endl;
	cin >> portno;

	Node currentnode = Node(myip,portno);
	string temp;// junk enter char
	getline(cin,temp);//ignore
	long long int idd = gethash(myip+":"+to_string(portno));
	currentnode.setid(idd);

	
	while(true){
		printprompt();
		getline(cin,inputcommand);

		if(inputcommand == ""){
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
				pthread_create(&l,NULL,NodeServer,(void *)&currentnode);
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
			// cout << "commandtosend " << commandtosend<< endl;

			recv(sockfd,buffer,sizeof(buffer), 0);
			// cout << "msg recv at outer node " << buffer << endl;
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
			pthread_create(&ll,NULL,NodeServer,(void *)&currentnode);
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
		else if(command[0] == "data_display"){
			currentnode.datadisplay();
		}

		else if(command[0]=="exit")
			break;

		else{
			cout << "Wrong Command input" << endl;
		}
	}


	return 0;


}