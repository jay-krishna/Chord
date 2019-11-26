#include "util.h"

long long int gethash(string key){
    unsigned char obuf[41];
    char finalHash[41];
    string keyHash = "";
    unsigned int i;
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

int newconnection(string ip, string portno){
    
    long long int port = atoi(portno.c_str());
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    while(sockfd < 0){
        cout << "Error Opening Socket";
        sockfd = socket(AF_INET,SOCK_STREAM,0);
    }
    
    server = gethostbyname(ip.c_str());
    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
        cout << "Error Connecting " << ip << " " << portno << endl;
        return -1;
    }
    return sockfd;
}