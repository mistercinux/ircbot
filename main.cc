#include <errno.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>         // pour close(socketID)
#include <arpa/inet.h>      // pour htons()
#include "socket.h"

int main(int argc, char** argv)
{
    std::string buffer;
    std::string hostname = "irc.freenode.net";
    
    if (argc > 2) { std::cout << "Usage: " << argv[0] << " host\n" << std::endl; return 1; }
    else if (argc == 2) { hostname = argv[1]; }
    else 
    {
        std::cout << ":: The default server is irc.freenode.net ::" << std::endl;
        std::cout << "If you're looking to connect to another server, try : " << argv[0] << " host" << std::endl;
    }


    mySocket ircBot(hostname.c_str(), 6667);

    std::cout << "FLAG0" << std::endl;

    if (ircBot.Connect() == -1) { return 1; }

    ircBot.Send("USER NoUser * * : Bot\r\n");
    ircBot.Send("NICK SamBot`\r\n");
    ircBot.Send("JOIN #justcodeit\r\n");

    // Boucle principale //
    int exitnow = 0;
    while (exitnow != 1)
    {
        ircBot.Recv();
        buffer = ircBot.GetBuffer();

        std::cout << buffer;
        // EXIT ?
        if (buffer.find("!quit\r\n") != std::string::npos) { exitnow = 1; }
        // PING / PONG
        if (buffer.substr(0,6) == "PING :") { ircBot.Send("PONG :" + buffer.substr(6)); }
    }

    return 0;
}
