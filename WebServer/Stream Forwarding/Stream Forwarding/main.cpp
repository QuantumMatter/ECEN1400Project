//
//  main.cpp
//  Stream Forwarding
//
//  Created by David Kopala on 11/7/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include <iostream>
#include "Server.h"
#include "StreamForwarding.hpp"
#include "List.h"
#include "Client.h"
#include <fstream>

using namespace std;

bool debug = true;

//Create a list to keep track of the current forwarding
List<StreamForwarding> *forwardings = new List<StreamForwarding>();

Server *server;
ofstream log;

//Callback for when new connections are made to the server
void newConnectionCallback() {
    if (debug) {
        cout<<"new connection"<<endl;
    }
}

//Callback for when new messages are received from clients on 13456
void newMessageCallback(Server::TCPMessage *msg) {
    if (debug) {
        cout<<msg->addr->addr<<":"<<msg->addr->port<<" => "<<msg->message<<endl;
    }
    if(msg->addr->port == 13456) {
        //Create a new stream forwarding server on the specified port
        StreamForwarding *forward = new StreamForwarding(atoi(msg->message));
        //Add the server to the list
        forwardings->add(forward);
        //Write connected to the initiator
        server->writeToAll("connected");
        
        //Add an entry in the log
        log<<msg->addr->addr<<":"<<msg->addr->port<<" => "<<msg->message<<endl;
    }
}

int main(int argc, const char * argv[]) {
    //Create a log of the requests
    log.open("log.txt", ios::app);
    
    //Create a new server to handle requests
    server = new Server(13456);
    //Setup the callback handlers
    server->newConnectionCallback = newConnectionCallback;
    server->newMessageCallback = newMessageCallback;
    
    //Loop forever
    //Everything is threaded, so this does not block the operations
    while (true) {
        
    }
    
    return 0;
}
