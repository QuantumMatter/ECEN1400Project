//
//  Server.hpp
//  Playground
//
//  Created by David Kopala on 7/23/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef Server_h
#define Server_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <functional>
#include "List.h"

class Server
{
    
public:
    //Constructor - Creates server at the specified port
    Server(int port);
    //unused create function
    void create();
    //unused error handler
    static void sigchld_handler(int s);
    //function that listens for new connections
    static void *listening_handler(void *sockfd);
    //function that handles data from each connection
    static void *connection_handler(void *socket_desc);
    
    //groups data about each client
    struct ClientInfo {
        char *addr;
        int port;
        int fd;
    };
    //data about the message that was received
    struct TCPMessage {
        ClientInfo *addr;
        char *message;
    };
    //unused struct - switched to the List.h stuff
    struct socketList {
        int sockfd;
        struct socketList *next;
    };
    
    //writes a message to all clients
    void writeToAll(char *message);
    //a list of all the clients
    static List<ClientInfo> *clients;
    //a list of all the messages received
    static List<TCPMessage> *messages;
    
    //a function to be called when a new client connects
    static std::function<void(void)> newConnectionCallback;
    //a function to be called when a new message is received
    static std::function<void(TCPMessage *msg)> newMessageCallback;
    
private:
    void *get_in_addr(struct sockaddr *sa);
    //the port number
    static int p;
    
};

#endif /* Server_hpp */
