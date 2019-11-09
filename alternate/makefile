all: ChordNode/util.o node# script

ChordNode/util.o: ChordNode/util.cpp
	g++ -Wall -c -o ChordNode/util.o ChordNode/util.cpp -lssl -lcrypto

node: ChordNode/nodeclass.o ChordNode/util.o ChordNode/nodeserver.o ChordNode/node.o
	g++ -Wall -pthread ChordNode/nodeclass.o ChordNode/nodeserver.o ChordNode/node.o ChordNode/util.o -o ChordNode/node -lssl -lcrypto

ChordNode/node.o: ChordNode/nodeclass.o ChordNode/node.cpp
	g++ -Wall -pthread -c -o ChordNode/node.o ChordNode/node.cpp -lssl -lcrypto

ChordNode/nodeclass.o: ChordNode/nodeclass.cpp
	g++ -Wall -c -o ChordNode/nodeclass.o ChordNode/nodeclass.cpp

ChordNode/nodeserver.o: ChordNode/nodeserver.cpp
	g++ -pthread -Wall -c -o ChordNode/nodeserver.o ChordNode/nodeserver.cpp

script: create_nodes.py
	python3 create_nodes.py 1
