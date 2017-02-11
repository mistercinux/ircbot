#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>         // pour close(socketID)
#include <arpa/inet.h>      // pour htons()
#include "socket.h"

typedef struct host_settings {
    int port                = 6667;
    int use_ssl             = 0;
    std::string hostname    = "irc.unistra.fr";
    std::string channel     = "#lug";
    std::string nickname    = "CinBot";
}host_settings;


// Prototype of getArgs() witch check the options
int getArgs(int argc, char** argv, host_settings* hset); 


int main(int argc, char** argv)
{
    std::string buffer;
    host_settings set;

    if (getArgs(argc, argv, &set) < 0)
    {
        std::cout << "usage: " << (std::string)argv[0] << " [options]" << std::endl;
        std::cout << "Options are: [-h hostname] [-n nickname] [-p port] [-c '#channel'] [-ssl]" << std::endl;
        return 1;
    }

    mySocket ircBot(set.hostname, set.port);

    std::cout << "FLAG0" << std::endl;

    if (ircBot.Connect() == -1) { return 1; }

    ircBot.Send("USER IRC_BOT * * : Bot\r\n");
    ircBot.Send("NICK " + set.nickname + "\r\n");

    // Boucle principale //
    int exitnow = 0;
    int connected = 0;
    while (exitnow != 1)
    {
        ircBot.Recv();
        buffer = ircBot.GetBuffer();

        std::cout << buffer;
        // EXIT ?
        if (buffer.find("!quit\r\n") != std::string::npos) { exitnow = 1; }
        // PING / PONG
        if (buffer.substr(0,6) == "PING :") 
        { 
            if (connected == 0)
            {
                ircBot.Send("JOIN " + set.channel + "\r\n");
                connected = 1;
            }
            ircBot.Send("PONG :" + buffer.substr(6)); 
        }
    }

    return 0;
}


/*
 * RECUPERATION DES ARGUMENTS 
 */
int getArgs(int argc, char** argv, host_settings* hset) 
{
    if (argc == 1) { return 0; }
    int i =0;
    int end = argc-1;

    for (i=1; i <= (end); ++i)
    {
        //std::cout << "ARGV[" << i << "] =" << argv[i] << std::endl;
        //if ((i+1)<=end) { std::cout << argv[i+1] << std::endl; }
        if (argv[i] == (std::string)"-ssl") { hset->use_ssl = 1; }
        // HOSTNAME OPTION
        else if ((argv[i] == (std::string)"-h") && (i < end))
        {
            if (argv[i+1][0] == '-') { return -1; }
            hset->hostname = (std::string)argv[i+1];
            ++i;
        }
        // NICKNAME OPTION
        else if ((argv[i] == (std::string)"-n") && (i<end))
        {
            if (argv[i+1][0] == '-') { return -1; }
            hset->nickname = (std::string)argv[i+1];
            ++i;
        }
 
        // PORT OPTION
        else if ((argv[i] == (std::string)"-p") && (i<end))
        {
            if (argv[i+1][0] == '-') { return -1; }
            hset->port = atoi(argv[i+1]);
            ++i;
        }
        // CHANNEL OPTION
        else if ((argv[i] == (std::string)"-c") && (i<end))
        {
            std::cout << "FLAG -C :" << argv[i+1] << ":" << std::endl;
            if (argv[i+1][0] != '#') { return -1; }
            hset->channel = (std::string)argv[i+1];
            ++i;
        }
        else { return -1; }
    }
    return 0;
}

