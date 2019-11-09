#ifndef __nodeclass_H_INCLUDED__
#define __nodeclass_H_INCLUDED__

#include <iostream>
#include <string>
#include <utility>
#include <pthread.h>

using namespace std;

class Node{
private:
	string nodeip;
	long long int nodeportno;
	long long int nodeid;
	bool connectedtoring;
	// vector<pair<int,string>> fingertable;
	pair<long long int, pair<string,long long int>> successornode;
	// pair.first = hashvalue // pair.second.first = ip // pair.second.second = portno
	
	pair<long long int, pair<string,long long int>> predecessornode;

public:
	Node(string ip,int portno);

	string getip();

	long long int getid();

	long long int getnodeportno();

	bool ringstatus();

	void setid(long long int id);

	void setringstatus();

	void successor(string ip,long long int portno,long long int id);

	void predecessor(string ip,long long int portno,long long int id);

	pair<string,long long int> successordetail();

	long long int successorid();

	long long int predecessorid();

	pair<string,long long int> predecessordetail();

	void nodedetails();

	long long int findsuccessor(long long int requestid);
};

#endif