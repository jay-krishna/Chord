#include "nodeclass.h"

Node::Node(string ip,int portno){
	this->nodeip = ip;
	this->nodeportno = portno;
	connectedtoring = false;
}

string Node::getip(){
	return nodeip;
}

long long int Node::getid()
{
	return this->nodeid;
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

	// pthread_mutex_lock(&lock1);

	successornode.first = id;
	successornode.second.first = ip;
	successornode.second.second = portno;

	// pthread_mutex_unlock(&lock1);

}

void Node::predecessor(string ip,long long int portno,long long int id){

	// pthread_mutex_lock(&lock0);

	predecessornode.first = id;
	predecessornode.second.first = ip;
	predecessornode.second.second = portno;

	// pthread_mutex_unlock(&lock0);
}

void Node::datadisplay(){
	for(auto i = data.begin();i!=data.end();i++)
	{
		cout << i->first << " " << i->second << endl;
	}
}

bool Node::search(string s){
	if(this->data.find(s) == this->data.end()){
		return false;
	}
	else{
		return true;
	}
}

pair<string,long long int> Node::successordetail(){
	return make_pair(this->successornode.second.first , this->successornode.second.second);
}

long long int Node::successorid(){
	return this->successornode.first;
}

long long int Node::predecessorid(){
	return this->predecessornode.first;
}

pair<string,long long int> Node::predecessordetail(){
	return make_pair(this->predecessornode.second.first , this->predecessornode.second.second);	
}

void Node::nodedetails()
{
	cout << "Node ip is " << this->nodeip << endl;
	cout << "Node portno is " << this->nodeportno << endl;
	cout << "Node nodeid is " << this->nodeid << endl;
	cout << "Node successornode id is " << this->successornode.first << " ip and port is " << successornode.second.first << " " << successornode.second.second << endl;
	cout << "Node predecessornode id is " << this->predecessornode.first << " ip and port is " << predecessornode.second.first << " " << predecessornode.second.second << endl;
}

void Node::storedata(long long int id, string s){
	this->data[s] = id;
}

long long int Node::findsuccessor(long long int requestid){
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