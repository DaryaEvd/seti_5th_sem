## TCP FILE TRANSFER MULTITHREADING  

### How to compile 

from dir `lab2` :  
`mkdir build`  
`cd build`  
`cmake ..`  
`make` 

Open at least 2 terminals: one for **server** and one for **client**:  
from `server's terminal` run:  
`./server <port number>`  
from `client's terminal` run:  
`./client <path to file> <DNS name or IP address> <port number>`  

### What does the project aim to?  
Multiple clients can send files to server, that stores files in `uploads` dir.   

### How this program works?  
This code uses TCP protocol.   
![tcp_pic](./image_tcp.png)  

In my realisation:  
Client sends 
- file name 
- file size  
And server receives it.  

Then a client sends a file's source.  

Then server is receiving it. After full downloading server compares amount of received bytes and a file size.  
If sizes are the same, then server sends a message about it to client and a client prints it.  

