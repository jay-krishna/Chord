all: node script

node: ChordNode/nodeclass.o ChordNode/node.o
	g++ ChordNode/nodeclass.o ChordNode/node.o -o ChordNode/node -lssl -lcrypto

ChordNode/node.o: ChordNode/nodeclass.o ChordNode/node.cpp
	g++ -c -o ChordNode/node.o ChordNode/node.cpp -lssl -lcrypto

ChordNode/nodeclass.o: ChordNode/nodeclass.cpp
	g++ -c -o ChordNode/nodeclass.o ChordNode/nodeclass.cpp

script: create_nodes.py
	python3 create_nodes.py 1