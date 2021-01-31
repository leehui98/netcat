#ifndef TCPSTREAM_H
#define TCPSTREAM_H
#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<cassert>
class TcpStream{
private:
    int sockfd;
    int clientfd;
public:
    TcpStream(int port){
        if((sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
        {
            perror("socket");
        }
        sockaddr_in servaddr,clientaddr;
        bzero(&servaddr,sizeof(struct sockaddr_in));
        servaddr.sin_family=AF_INET;
        servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
        servaddr.sin_port=htons(port);
        if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(struct sockaddr_in))<0){
            perror("bind");
        }
        if(listen(sockfd,2)<0){
            perror("listen");
        }
        unsigned int len=sizeof(struct sockaddr);
        std::cout<<"listing..."<<std::endl;
        if((clientfd=accept(sockfd,(struct sockaddr*)&clientaddr,&len))<0){
            perror("accept");
        }
    }
    TcpStream(const char* hostname,int port){
        if((clientfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
        {
            perror("client socket");
        }
        struct hostent *he=gethostbyname(hostname);
        if(!he){
            perror("gethostbyname");
        }
        assert(he->h_addrtype==AF_INET && he->h_length==sizeof(uint32_t));
        struct sockaddr_in servaddr;
        bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family=AF_INET;
        servaddr.sin_port=htons(port);
        servaddr.sin_addr=*reinterpret_cast<struct in_addr*>(he->h_addr);
        
        int ret=connect(clientfd,reinterpret_cast<struct sockaddr*>(&servaddr),sizeof(servaddr));
        if(ret<0){
            perror("connect");
        }
    }
    int receive(char* buff,int length){
        int n=read(clientfd,buff,length);
        return n;
    }
    int send(char* buff,int length){
        int n=write(clientfd,buff,length);
        return n;
    }
    void shutdownWrite(){
        shutdown(clientfd,SHUT_WR);
    }
};


#endif