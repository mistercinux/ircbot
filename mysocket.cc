#include "mysocket.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

// Constructor
mySocket::mySocket (const std::string serverIp_, int serverPort_)
{
    sockFd                  = 0;
    server.sin_addr.s_addr  = inet_addr(serverIp_.c_str());
    server.sin_family       = AF_INET;
    server.sin_port         = htons(serverPort_);

    Socket();

    std::cout << "Bot Initialisé.\nIP: " << serverIp_.c_str() << "\nPort : "<< serverPort_ << "\nSockFd: " << sockFd << std::endl;

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
    std::cout << "Connection en cours : ...";
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
    if (recvBuffer[sizeof(recvBuffer -1] 
    std::cout << "RECV <--: " << recvBuffer << std::endl;
    return 0;
    //XXX Vérifier si l'intégralité du message est arrivée.
}


// Fonction de gestion des évènements
int mySocket::GetEvent()
{
    FD_ZERO (&readFds);
    FD_SET  (sockFd, &readFds);
 
    int event=0;
    event = select((sockFd+1), &readFds, &writeFds, NULL, NULL);
    if (event == -1) { perror("Select()"); return -1;}
    else if FD_ISSET(sockFd, &readFds)
    {
        std::cout << "Données reçues" << std::endl;
    }
    return 1;
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
