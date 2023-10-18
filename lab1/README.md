## Copy finder  

### Task  
Develop an application that finds copies of itself on a local network using UDP multicast messaging. The inclusion should record the moments of display and lose other copies of itself on the local network and, when changes occur, display a list of IP addresses of “live” copies.  

The address of the multicast group must be passed as a parameter to the application. The application must support operation in both IPv4 and IPv6 networks, automatically selecting a protocol depending on the transmitted group address.  

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

 


