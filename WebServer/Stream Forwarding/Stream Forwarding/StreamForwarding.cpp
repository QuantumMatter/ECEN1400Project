//
//  StreamForwarding.cpp
//  Stream Forwarding
//
//  Created by David Kopala on 11/7/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "StreamForwarding.hpp"

Server *StreamForwarding::server;

//Constructor
StreamForwarding::StreamForwarding(int p) {
    port = p;
    
    //Create a new server to handle connections
    server = new Server(port);
    //Connect callback handlers
    server->newMessageCallback = readHandler;
    server->newConnectionCallback = newConnectionHandler;
}

//Do nothing for new connections
void StreamForwarding::newConnectionHandler() {
    
}

//Write data from incoming messages to all clients
void StreamForwarding::readHandler(Server::TCPMessage *message) {
    server->writeToAll(message->message);
}
