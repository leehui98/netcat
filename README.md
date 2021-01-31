# netcat
A netcat with thread-per-connection

# Usage
g++ netcat.cpp -std=c++11 -lpthread

server:./a.out -l port 

client:./a.out address port

