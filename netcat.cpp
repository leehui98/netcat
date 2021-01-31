#include<iostream>
#include<string.h>
#include<unistd.h>
#include<thread>
#include"TcpStream.h"

void run(TcpStream stream){//网络线程
    std::thread thr([&stream]{
        char buff[8192];
        int nr=0;
        while((nr=stream.receive(buff,sizeof(buff)))>0){
            int nw=write(STDOUT_FILENO,buff,nr);
            if(nw<nr)
            {
                break;
            }
        }
        ::exit(0);

    });
    char buff[8192];
    int nr;
    while((nr=read(STDIN_FILENO,buff,sizeof(buff)))>0){
        int nw=stream.send(buff,nr);
        if(nw<nr){
            break;
        }
    }
    stream.shutdownWrite();
    thr.join();
}


int main(int argc,const char * argv[])
{
    if(argc<3){
        printf("Usage:\n %s hostname port\n %s -l port \n",argv[0],argv[0]);
        return 0;
    }
    int port=atoi(argv[2]);
    if(strcmp(argv[1],"-l")==0){
        TcpStream stream(port);
        run(stream);
    }
    else {
        TcpStream stream(argv[1],port);
        run(stream);
    }
    return 0;
}