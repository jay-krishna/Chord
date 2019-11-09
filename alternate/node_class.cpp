string getip(){
		return nodeip;
	}

	long long int getnodeportno(){
		return nodeportno;
	}

	bool ringstatus(){
		return connectedtoring;
	}

	void setid(long long int id){
		this->nodeid = id;
	}

	void setringstatus()
	{
		this->connectedtoring = true;
	}

	void successor(string ip,long long int portno,long long int id){
		successornode.first = id;
		successornode.second.first = ip;
		successornode.second.second = portno;
	}

	void predecessor(string ip,long long int portno,long long int id){
		predecessornode.first = id;
		predecessornode.second.first = ip;
		predecessornode.second.second = portno;
	}


	void nodedetails()
	{
		cout << "Node ip is " << this->nodeip << endl;
		cout << "Node portno is " << this->nodeportno << endl;
		cout << "Node nodeid is " << this->nodeid << endl;
		cout << "Node successornode id is " << this->successornode.first << " ip and port is " << successornode.second.first << " " << successornode.second.second << endl;
		cout << "Node predecessornode id is " << this->predecessornode.first << " ip and port is " << predecessornode.second.first << " " << predecessornode.second.second << endl;
	}