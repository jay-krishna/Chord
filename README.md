# Chord

Project Members:
1. Pratik Tiwari (2019201023)
2. Jay Krishna (2019201019)
3. Apurva Jadhav (2019201069)
4. Himani Bhardwaj (2019201081)

Project Details:

Node:
-------------------------------------------------------------------------------------------
Compile - make
Run: Executable file named (node)

1. First time node will create a ring and other will join the ring using IP and port of node already connected to ring

Commands for Node:
--------------------------------------------------------------------------------------------
1. create_ring
2. join ip port (eg join 127.0.0.1 10000)
3. display (output will be node details)
4. fingertable (output will be fingertable of respective node)
5. data_display (output will be data stored on current node)
6. leave (leave node from ring)

Client:
-------------------------------------------------------------------------------------------
Client will initially ask for IP and Port of any node connected to the sever.
After providing IP and Port, client can you below two commands.

1. upload <string> (eg: upload operatingsystem)
  - output on successfull upload will be "File uploaded to server"

2. search <string> (eg: search operatingsystem)
  - output will be node id on which the string is stored.
