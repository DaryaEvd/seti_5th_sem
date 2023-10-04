## Copy finder  

### How to compile 

from dir `lab1` :  
`mkdir build`  
`cd build`  
`cmake ..`  
`make`  
`./finder [ip group multicast] [port number]`  

### What does the project aim to?  
The application shold find the copy of itself in local networks using UDP multicast messages.  

### How this program works?  
The program takes 2 args: ip of multicast group and a port number we want to connect. Multicast group might be in ipv4 or ipv6. The program checks wether arguments are valid, if not - it is finishing.   
There are 2 threads: one is responsible for sending messages, other - for receiving(listnening) them. 

 


