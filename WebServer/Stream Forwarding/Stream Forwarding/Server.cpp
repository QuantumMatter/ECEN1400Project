//
//  Server.cpp
//  Playground
//
//  Created by David Kopala on 7/23/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "Server.h"

#define BACKLOG 10

//Server::smsg *Server::messages;
List<Server::TCPMessage> *Server::messages;
List<Server::ClientInfo> *Server::clients;
//Server::socketList *Server::clients;
std::function<void(void)> Server::newConnectionCallback;
std::function<void(Server::TCPMessage *msg)> Server::newMessageCallback;
int Server::p;

//Creates a TCP Server on the specified port
//Really easy to connect to with telnet on the command line
Server::Server(int port)
{
    struct sockaddr_in myaddr;
    int sockfd;
    
    p = port;
    
    //Create socket & check for errors
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Could not create socket");
    }
    
    //Bind to socket
    memset((char *)&myaddr, 0, sizeof(myaddr));
    //Use IPv4
    myaddr.sin_family = AF_INET;
    //Use any address for this machine
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //Set the port
    myaddr.sin_port = htons(port);
    //Bind the socket
    if (bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("Could not bind to socket");
        return;
    }
    
    //Set SO_REUSEADDR option to prevent program from being relaunched quickly
    int optval = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("could not set socket options");
    }
    
    //Initialize the lists
    clients = new List<ClientInfo>();
    messages = new List<TCPMessage>();
    
    //Start listening on the socket
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    
    //Createa new thread to handle new connections
    pthread_t listeningThread;
    //Pass in the fd for the socket
    int *new_sock = (int *)malloc(1);
    *new_sock = sockfd;
    if (pthread_create(&listeningThread, NULL, Server::listening_handler, (void *)new_sock) >= 0) {
        
    }
}

void *Server::listening_handler(void *sockfd) {
    int newfd;
    sockaddr their_addr;//struct sockaddr_storage their_addr;
    socklen_t sin_size = sizeof(their_addr);
    //Get the fd
    int mysock = *(int*)sockfd;
    //socketList *lastSocket = clients;
    
    while (true) {
        //Accept incoming connections
        newfd = accept(mysock, (struct sockaddr *)&their_addr, &sin_size);
        
        //If there is a callback, call it
        if (newConnectionCallback != NULL) {
            newConnectionCallback();
        }
        
        //Create an object to record information about the client
        ClientInfo *info = new ClientInfo;
        info->fd = newfd;
        //Add the IP address of the client
        struct sockaddr_in *test = (struct sockaddr_in *) &their_addr;
        info->addr = inet_ntoa(test->sin_addr);
        //Add the port
        info->port = p;
        //Add it to the list of clients
        clients->addCopy(*info);
        //Create another new thread to listen for new messages
        pthread_t snifferThread;
        if (pthread_create(&snifferThread, NULL, Server::connection_handler, (void *)info) < 0) {
            
        }
    }
    
    return 0;
}

void *Server::connection_handler(void *tcp_info)
{
    //Get the socket descriptor
    //int sock = *(int*)socket_desc;
    //Get the data from the pointer
    ClientInfo *info = (ClientInfo *) tcp_info;
    ssize_t read_size;
    char client_message[2000];
    
    //Read data from the client
    while( (read_size = recv(info->fd , client_message , 2000 , 0)) > 0 )
    {
        //Create a new object to record the message
        TCPMessage *message = new TCPMessage;
        message->addr = info;
        //Copy the string the new object
        char mes_cpy[500] = {'\0', };
        strcpy(mes_cpy, client_message);
        message->message = mes_cpy;
        //Add the object to the list
        messages->add(message);
        //If there is a callback, call it
        if (newMessageCallback != NULL) {
            newMessageCallback(message);
        }
        //Erase the buffer
        bzero(client_message, 2000);
    }
    
    //Gets here ONLY if the client disconnects
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    
    //free(socket_desc);
    
    return 0;
}

//Writes a message to all the clients
void Server::writeToAll(char *message)
{
    //socketList *client = clients;
    for (int i = 0; i < clients->count(); i++) {
        write(clients->get(i)->fd, message, strlen(message));
    }
}

void Server::sigchld_handler(int s)
{
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);
    
    errno = saved_errno;
}
