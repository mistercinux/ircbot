#ifndef MYSOCKET_H
#define MYSOCKET_H
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <sys/time.h>

class mySocket
{
    private:
    sockaddr_in server;
    int         sockFd;
    char        recvBuffer[512];

    // --- Variables nécessaires à select() ---
    // Timeout vars pour select()
    //const struct timeval timeout;
    //timeout.tv_sec  = 5;
    //timeout.tv_usec = 0;
    // les Sets de FD pour select()
    fd_set  readFds, writeFds;
   // --- Fin des vars pour select() ---


    public:
    int     Socket();
    int     Connect();
    int     Send(const std::string);
    int     Recv();
    int     GetEvent();
    int     Pong();
    char*   GetBuffer() { return recvBuffer; }


    mySocket (const std::string, int);
    mySocket();
    ~mySocket();    //XXX L'objet n'est pas détruit à la fin du programme
};

#endif
