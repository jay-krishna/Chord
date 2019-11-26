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

void Node::setringstatusfalse()
{
	this->connectedtoring = false;
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

void Node::fingersuccessor(string ip,long long int portno,long long int id)
	{
		pair<long long int, pair<string,long long int>> temp = this->finger[0];
		for(int i=0;i<4;i++)
		{
			if(finger[i].first == temp.first){
				finger[i].first = id;
				finger[i].second.first = ip;
				finger[i].second.second = portno; 
			}
		}
	}

void Node::datadisplay(){
	for(auto i = data.begin();i!=data.end();i++)
	{
		cout << i->first << " " << i->second << endl;
	}
}

void Node::deletedata(string s){
	// cout<<"deletedata called "<< s <<endl;
	this->data.erase(s);
}

vector<pair<string,long long int>> Node::getdata(){
	vector<pair<string,long long int>> result;
	for(auto i = data.begin();i!=data.end();i++)
	{
		result.push_back(make_pair(i->first,i->second));
	}	
	return result;
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

void Node::updatefingertable(vector<pair<long long int, pair<string,long long int>>> &newfinger){
	this->finger = newfinger;

	// update successor from first entry of finger table
	this->successornode.first = finger[0].first;
	this->successornode.second.first = finger[0].second.first;
	this->successornode.second.second = finger[0].second.second;

}

void Node::fingertableupdate(){

	long long int succ = this->successornode.first;
	string ipp = this->successornode.second.first;
	long long int portt = this->successornode.second.second;

	for(int i=1;i<=4;i++){
		finger.push_back(make_pair(succ,make_pair(ipp,portt)));
	}
}

void Node::fingerdisplay()
{
	for(int i=0;i<4;i++)
	{
		cout << finger[i].first << " " << finger[i].second.first << " " << finger[i].second.second << endl;
	}
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

pair<long long int,pair<string,long long int>> Node::findpred(long long int s){

	long long int n = this->getid();

	for(int i=3;i>=0;i--)
	{
		bool condition = false;
		long long int x = this->finger[i].first;
		pair<string, long long int> ipport = this->finger[i].second;

		if(n == s && x==n){
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
			return make_pair(x,make_pair(ipport.first,ipport.second));
		}

	}

	return make_pair(n,make_pair(this->getip(),this->getnodeportno()));
}