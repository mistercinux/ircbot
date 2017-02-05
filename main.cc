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

    if (ircBot.Connect() == -1)
    {
        delete[] &ircBot;
        return -1;
    }
    ircBot.Send("USER yollande84 * * : yollande84\r\n");
    ircBot.Send("NICK yollande84\r\n");
    ircBot.Send("JOIN #justcodeit\r\n");

    // Boucle principale //
    int exitnow = 0;
    while (exitnow != 1)
    {
        if (ircBot.GetEvent())
        {
            ircBot.Recv();
            buffer = ircBot.GetBuffer();

            // EXIT ?
            if (buffer.find("!quit\r\n") != std::string::npos) { exitnow = 1; }

            // PING / PONG
            if (buffer.substr(0,6) == "PING :") { ircBot.Send("PONG :" + buffer.substr(6) + "\r\n"); }
        }
    }

    return 0;
}
