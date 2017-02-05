#include <errno.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>         // pour close(socketID)
#include <arpa/inet.h>      // pour htons()
#include "mysocket.h"

int main()
{
    std::string buffer;

    mySocket ircBot("173.230.128.213", 6667);

    ircBot.Connect();
    ircBot.Send("USER yollande84 * * : yollande84\r\n");
    ircBot.Send("NICK yollande84\r\n");
    ircBot.Send("JOIN #justcodeit\r\n");

    int exitnow = 0;
    while (exitnow != 1)
    {
        if (ircBot.GetEvent())
        {
            ircBot.Recv();
            buffer = ircBot.GetBuffer();
            if (buffer.find("!quit\r\n") != std::string::npos) { exitnow = 1; }

            // PING / PONG
            if (buffer.substr(0,6) == "PING :") { ircBot.Send("PONG :" + buffer.substr(6) + "\r\n"); }
        }
    }

/*

    for (i=0; i<3; ++i)
    {
        for ( j=0; j<sizeof(recvBuffer); ++j) { recvBuffer[j] = '\0'; }
        if ( recv(cliSock, recvBuffer, sizeof(recvBuffer), 0) == -1)
        {
            std::cout << "Une erreur est survenue lors de la reception des données." << std::endl;
        }
        std::cout << "SRV_MSG: " << recvBuffer << std::endl;
    }

    for (i=1; i<10; ++i)
    {
        for ( j=0; j<sizeof(recvBuffer); ++j) { recvBuffer[j] = '\0'; }
        if ( recv(cliSock, recvBuffer, sizeof(recvBuffer), 0) == -1)
        {
            std::cout << "Une erreur est survenue lors de la reception des données." << std::endl;
        }
        std::cout << "SRV_MSG: " << recvBuffer << std::endl;
    }

    for (i=1; i<100; ++i)
    {
        for ( j=0; j<sizeof(recvBuffer); ++j) { recvBuffer[j] = '\0'; }
        if ( recv(cliSock, recvBuffer, sizeof(recvBuffer), 0) == -1)
        {
            if ((std::string)recvBuffer.substr(0, 6) == sping.c_str()) 
            {
                std::cout << "PINNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNGOOOOOOOOOOOOOOOO" << std::endl;
            }
            std::cout << "Une erreur est survenue lors de la reception des données." << std::endl;
        }
        std::cout << "SRV_MSG: " << recvBuffer << std::endl;
    }




   std::cout << "Fermeture de la connexion en cours..." << std::endl;
    if (close(cliSock) == -1)
    {
        std::cout << "Impossible de fermer le socket" << std::endl;
        return -1;
    }
    std::cout << "Connexion terminée." << std::endl;
    */

    return 0;
}
