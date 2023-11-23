## SOCKS-PROXY  

This is a single threaded multiplexed SOCKS5 proxy server.  

### TASK  
1. You must implement a proxy server that complies with SOCKS version 5.  
2. In parameters, only the port on which the proxy will wait for incoming connections from clients is passed to the program.  
3. Of the three commands available in the protocol, only the implementation of the command 1 (establish a TCP/IP stream connection) is required.   
4. Authentication and IPv6 address support are not required.   
5. To implement proxy use non-blocking sockets, working with them in one thread. Additional threads are not allowed. Accordingly, no blocked calls (other than the call selector) are allowed.  
6. Proxies should not assume which application-level protocol will be used inside the redirected TCP connection. In particular, both sides of the data should be supported, and connections should be closed neatly (only after they are no longer needed).   
7. The annex shall not have blank cycles in any situation. In other words, it should not be possible to have a program state that repeatedly runs a loop body that does not make any actual data transfer per iteration.   
8. No unlimited memory is allowed for one client. 
9. Performance via proxy should not be noticeably worse than without proxy. To keep track of correctness and speed, you can look at the Network tab in the browser’s Developer tools.  
10. Proxy must support domain name resolving (value 0x03 in address field). Resolving must also be non-blocking. To do this, the following approach is suggested:
	-  At the start of the program, create a new UDP socket and add it to the selector for reading.  
	- When you want to parse a domain name, send the DNS query to the recursive DNS resolver address. 
	- In the socket reader, process the response to the DNS query and continue working with the received address.  

To get the address of recursive resolver, as well as to form and parsing DNS-messages in Java, it is proposed to use dnsjava library (for other languages find yourself).  

For testing, you can configure any Web browser to use your proxy, and visit any websites rich in content.  

### Description of the protocol  
1. [English wiki](https://en.wikipedia.org/wiki/SOCKS)  
2. [SOCKS 5 RFC](https://www.ietf.org/rfc/rfc1928.replayClientt) 
3. [dlya samih malenkih](http://fit.ippolitov.me/CN_2/2023/socks.jpg)  

## HOW TO COMPILE?
```
mkdir build && cd build  
cmake ..  
make  
./proxy <port>  
```

## What's going on and how it works?  
- Read about [firewall](https://ru.wikipedia.org/wiki/%D0%9C%D0%B5%D0%B6%D1%81%D0%B5%D1%82%D0%B5%D0%B2%D0%BE%D0%B9_%D1%8D%D0%BA%D1%80%D0%B0%D0%BD)  

In the lab I used `epoll` to manipulate with multiplexed single threaded server.   
Recomment to read this literature:  
- [From habr](https://habr.com/ru/companies/infopulse/articles/415259/)  
- [Smth like man](https://ru.manpages.org/epoll/7)  
- [Non-blocking sockets](https://www.scottklement.com/rpg/socktut/nonblocking.html)   
