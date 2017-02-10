#include "socket.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <netdb.h>
#include <string.h>

// Constructor
mySocket::mySocket (const std::string hostname_, int port_)
{
    struct addrinfo     hints;
    struct addrinfo*    addrLst;
    int                 addrRetv = 0;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family     = AF_INET;
    hints.ai_protocol   = 0;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_canonname  = NULL;
    hints.ai_next       = NULL;
    hints.ai_addr       = NULL;

    addrRetv = getaddrinfo(hostname_.c_str(), NULL, &hints, &addrLst);
    if (addrRetv != 0)
    {
        std::cout << "Fatal error!" << std::endl << hostname_.c_str() << " : " << gai_strerror(addrRetv) << std::endl;
        exit(1);
    }
    else
    {
        sockFd                  = 0;
        server                  = *(struct sockaddr_in*)addrLst->ai_addr;
        server.sin_family       = AF_INET;
        server.sin_port         = htons(port_);

        Socket();
        const char* hostIp = inet_ntoa(server.sin_addr);
        std::cout << "Socket Initialisé.\nHostname: " << hostIp << "\nPort : "<< port_ << std::endl;
    }
}


// Création de la socket pour la connexion au serveur
int mySocket::Socket()
{
    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0)
    {
        perror("socket()");
        return -1;
    }
    return 0;
}


// Fonction de connexion au serveur distant
int mySocket::Connect()
{
    std::cout << "Connexion en cours : ..." << std::endl;
    if ( connect(sockFd, (struct sockaddr*)&server, sizeof(server)) < 0 )
    {
        perror("connect()");
        return -1;
    }
    else
    {
        std::cout << "Connection établie avec le serveur distant" << std::endl;
        return 0;
    }
}


// Fonction d'envoi
int mySocket::Send (const std::string msg_)
{
    void *buffer = (void*)msg_.data();
    if ( send(sockFd, buffer, msg_.size(), 0 ) < 0)
    {
        perror("send()");
        return -1;
    }
    std::cout << "SENT -->: " << msg_.c_str(); 
    return 0;
}


// Fonction de Réception
int mySocket::Recv()
{
    unsigned int i=0;
    for ( i=0; i<sizeof(recvBuffer); ++i) { recvBuffer[i] = '\0'; }
    if ( recv(sockFd, recvBuffer, sizeof(recvBuffer), 0) == -1)
    {
        perror("recv()");
        return -1;
    }
    return 0;
    //XXX Vérifier si l'intégralité du message est arrivée.
}


// Destructor
mySocket::~mySocket()
{
    if (sockFd >= 0)
    {
        std::cout << "Fermeture de la socket..." << std::endl;
        close(sockFd);
    }
}
