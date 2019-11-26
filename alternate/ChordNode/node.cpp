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

bool closeall = true;

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

				currentnode.fingertableupdate();

				// launch thread to start listening for other nodes to join chord ring 
				pthread_t l;
				pthread_create(&l,NULL,NodeServer,(void *)&currentnode);
				pthread_detach(l);
				
				// launch thread for stabalization
				pthread_t s;
				pthread_create(&s,NULL,stable,(void *)&currentnode);
				pthread_detach(s);

				// pthread_t f;
				// pthread_create(&f,NULL,fixfinger,(void *)&currentnode);
				// pthread_detach(f);


			}
				
			else{
				cout << "Node is already part of ring" << endl;
				}
		}
		
		else if(currentnode.ringstatus() == true && command[0] == "display"){
			currentnode.nodedetails();
			}
		
		else if(currentnode.ringstatus() == false && command[0] == "join"){

			string iptojoin = command[1];
			string portnotojoin = command[2];
			long long int id = gethash(currentnode.getip() + ":" + to_string(currentnode.getnodeportno()));

			int sockfd = newconnection(command[1],command[2]);
			if(sockfd == -1){
				 cout << "newsofd is -1 connection loss join" <<  endl;
			}
			
			string commandtosend = "findsuccessor " + to_string(id); // command to be send to listner "findsuccessor nodeid"

			char buffer[1024];
			memset(buffer,'\0',sizeof(buffer));
			send(sockfd,commandtosend.c_str(),commandtosend.size(),0);
			recv(sockfd,buffer,sizeof(buffer), 0);
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
			shutdown(sockfd,0);
			close(sockfd);
			currentnode.fingertableupdate();

			pthread_t ll;
			pthread_create(&ll,NULL,NodeServer,(void *)&currentnode);
			pthread_detach(ll);

			// release thread -> stabalization
			pthread_t s;
			pthread_create(&s,NULL,stable,(void *)&currentnode);
			pthread_detach(s);

			// pthread_t f;
			// pthread_create(&f,NULL,fixfinger,(void *)&currentnode);
			// pthread_detach(f);

			}

		else if(currentnode.ringstatus() == true && command[0] == "temp"){

			string ppp = command[2];
			string idpp = command[3];
			currentnode.successor(command[1],atoi(ppp.c_str()),atoi(idpp.c_str()));
		}

		else if(currentnode.ringstatus() == true && command[0] == "data_display"){
			currentnode.datadisplay();
		}

		else if(currentnode.ringstatus() == true && command[0] == "leave"){
			currentnode.nodedetails();
			currentnode.fingerdisplay();
			closeall = false;
			senddata(&currentnode);
			changesuccpred(&currentnode);
			changepredsucc(&currentnode);
			// sleep(5);
			long long int ll=0;
			while(ll != 2233720){
				ll++;
			} 
			return 0;

		}

		else if(currentnode.ringstatus() == true && command[0] == "fingertable"){
			currentnode.fingerdisplay();
		}

		else{
			cout << "Wrong Command input" << endl;
		}
	}


	return 0;


}