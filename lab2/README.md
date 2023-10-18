## TCP FILE TRANSFER MULTITHREADING  

### How to compile 

from dir `lab1` :  
`mkdir build`  
`cd build`  
`cmake ..`  
`make` 

Open at least 2 terminals: one for **server** and one for **clienti**: 
from `server's terminal` run:  
`./server <port number>`  
from `client's terminal` run:  
`./client <path to file> <DNS name or IP address> <port number>`  

### What does the project aim to?  
Multiple clients can send files to server, that stores files in `uploads` dir.   

### How this program works?  
This code uses TCP protocol. 
 


