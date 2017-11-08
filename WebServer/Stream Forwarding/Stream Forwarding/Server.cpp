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

Server::Server(int port)
{
    struct sockaddr_in myaddr;
    int sockfd;
    
    p = port;
    
    //Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Could not create socket");
    }
    
    //Bind to socket
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(port);
    
    if (bind(sockfd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("Could not bind to socket");
        return;
    }
    
    //Set SO_REUSEADDR option to prevent program from being relaunched quickly
    int optval = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("could not set socket options");
    }
    
    clients = new List<ClientInfo>();
    messages = new List<TCPMessage>();
    
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    
    /*sa.sa_handler = Server::sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, nullptr) == -1) {
        perror("sigaction");
        exit(1);
    }*/
    
    pthread_t listeningThread;
    int *new_sock = (int *)malloc(1);
    *new_sock = sockfd;
    if (pthread_create(&listeningThread, NULL, Server::listening_handler, (void *)new_sock) >= 0) {
        
    }
}

void *Server::listening_handler(void *sockfd) {
    int newfd;
    sockaddr their_addr;//struct sockaddr_storage their_addr;
    socklen_t sin_size = sizeof(their_addr);
    int mysock = *(int*)sockfd;
    //socketList *lastSocket = clients;
    
    while (true) {
        newfd = accept(mysock, (struct sockaddr *)&their_addr, &sin_size);
        
        if (newConnectionCallback != NULL) {
            newConnectionCallback();
        }
        
        ClientInfo *info = new ClientInfo;
        info->fd = newfd;
        struct sockaddr_in *test = (struct sockaddr_in *) &their_addr;
        info->addr = inet_ntoa(test->sin_addr);
        info->port = p;
        clients->addCopy(*info);
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
    ClientInfo *info = (ClientInfo *) tcp_info;
    ssize_t read_size;
    char client_message[2000];
    
    while( (read_size = recv(info->fd , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
        TCPMessage *message = new TCPMessage;
        message->addr = info;
        //lastMsg->next = (smsg *)malloc(sizeof(smsg));
        char mes_cpy[500] = {'\0', };
        strcpy(mes_cpy, client_message);
        message->message = mes_cpy;
        messages->add(message);
        if (newMessageCallback != NULL) {
            newMessageCallback(message);
        }
        bzero(client_message, 2000);
    }
    
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
