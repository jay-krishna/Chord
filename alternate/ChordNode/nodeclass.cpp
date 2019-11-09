#include "nodeclass.h"

Node::Node(string ip,int portno){
	this->nodeip = ip;
	this->nodeportno = portno;
	connectedtoring = false;
}

string Node::getip(){
	return nodeip;
}

long long int Node::getnodeportno(){
	return nodeportno;
}

bool Node::ringstatus(){
	return connectedtoring;
}

void Node::setid(long long int id){
	this->nodeid = id;
}

void Node::setringstatus()
{
	this->connectedtoring = true;
}

void Node::successor(string ip,long long int portno,long long int id){
	successornode.first = id;
	successornode.second.first = ip;
	successornode.second.second = portno;
}

void Node::predecessor(string ip,long long int portno,long long int id){
	predecessornode.first = id;
	predecessornode.second.first = ip;
	predecessornode.second.second = portno;
}


void Node::nodedetails()
{
	cout << "Node ip is " << this->nodeip << endl;
	cout << "Node portno is " << this->nodeportno << endl;
	cout << "Node nodeid is " << this->nodeid << endl;
	cout << "Node successornode id is " << this->successornode.first << " ip and port is " << successornode.second.first << " " << successornode.second.second << endl;
	cout << "Node predecessornode id is " << this->predecessornode.first << " ip and port is " << predecessornode.second.first << " " << predecessornode.second.second << endl;
}