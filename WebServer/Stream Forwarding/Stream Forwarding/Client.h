//
//  Client.hpp
//  Playground
//
//  Created by David Kopala on 7/23/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef Client_h
#define Client_h

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <functional>

#include <arpa/inet.h>

typedef struct node {
    char *message;
    struct node *next;
} node_t;

class Client
{
public:
    //Constructor - Connects to the port on the hostname
    Client(char *hostname, int port);
    //sends a message to the server
    void send(char *message);
    
    //return the head of a list of received messages
    node_t *getReceived();
    //sends a message to the server
    void writeMessage(char *message);
    
    //function that handles receiving data from the server
    static void *recvHandler(void *sock_desc);
    
    //a function to be called when a message is received
    static std::function<void(node *msg)> readCallback;

private:
    int sock;
};

#endif /* Client_h */
