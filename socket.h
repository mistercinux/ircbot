#ifndef MYSOCKET_H
#define MYSOCKET_H
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

class mySocket
{
    private:
    sockaddr_in server;
    int         sockFd;
    char        recvBuffer[512];

    // Variables nécessaires à select()
    fd_set  readFds, writeFds;

    public:
    int     Socket();
    int     Connect();
    int     Send(const std::string);
    int     Recv();
    int     Pong();
    char*   GetBuffer() { return recvBuffer; }


    mySocket (const std::string, int);
    mySocket();
    ~mySocket();
};

#endif
