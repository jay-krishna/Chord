all: ChordNode/util.o node client script

ChordNode/util.o: ChordNode/util.cpp
	g++ -Wall -c -o ChordNode/util.o ChordNode/util.cpp -lssl -lcrypto

node: ChordNode/nodeclass.o ChordNode/util.o ChordNode/nodeserver.o ChordNode/nodesync.o ChordNode/node.o
	g++ -Wall -pthread ChordNode/nodeclass.o ChordNode/nodeserver.o ChordNode/node.o ChordNode/nodesync.o ChordNode/util.o -o ChordNode/node -lssl -lcrypto

ChordNode/node.o: ChordNode/nodeclass.o ChordNode/node.cpp
	g++ -Wall -pthread -c -o ChordNode/node.o ChordNode/node.cpp -lssl -lcrypto

ChordNode/nodeclass.o: ChordNode/nodeclass.cpp
	g++ -Wall -c -o ChordNode/nodeclass.o ChordNode/nodeclass.cpp

ChordNode/nodeserver.o: ChordNode/nodeserver.cpp
	g++ -pthread -Wall -c -o ChordNode/nodeserver.o ChordNode/nodeserver.cpp

ChordNode/nodesync.o: ChordNode/nodesync.cpp
	g++ -pthread -Wall -c -o ChordNode/nodesync.o ChordNode/nodesync.cpp

client: ChordClient/clientutil.o ChordClient/client.o
	g++ -Wall ChordClient/client.o ChordClient/clientutil.o -o ChordClient/client

ChordClient/client.o: ChordClient/client.cpp ChordClient/clientutil.o
	g++ -Wall -c -o ChordClient/client.o ChordClient/client.cpp

ChordClient/clientutil.o: ChordClient/clientutil.cpp
	g++ -Wall -c -o ChordClient/clientutil.o ChordClient/clientutil.cpp	

script: create_nodes.py
	python3 create_nodes.py 4
	python3 create_clients.py 2