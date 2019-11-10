#ifndef __nodeclass_H_INCLUDED__
#define __nodeclass_H_INCLUDED__

#include <iostream>
#include <string>
#include <utility>
#include <pthread.h>
#include <unordered_map>
#include <vector>

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

	unordered_map<string,long long int> data;

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

	void datadisplay();

	void deletedata(string s);

	vector<pair<string,long long int>> getdata();

	bool search(string s);

	pair<string,long long int> successordetail();

	long long int successorid();

	long long int predecessorid();

	pair<string,long long int> predecessordetail();

	void nodedetails();

	void storedata(long long int id, string s);

	long long int findsuccessor(long long int requestid);
};

#endif