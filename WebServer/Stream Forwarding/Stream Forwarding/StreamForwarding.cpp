//
//  StreamForwarding.cpp
//  Stream Forwarding
//
//  Created by David Kopala on 11/7/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "StreamForwarding.hpp"

Server *StreamForwarding::server;

StreamForwarding::StreamForwarding(int p) {
    port = p;
    
    server = new Server(port);
    server->newMessageCallback = readHandler;
    server->newConnectionCallback = newConnectionHandler;
}

void StreamForwarding::newConnectionHandler() {
    
}

void StreamForwarding::readHandler(Server::TCPMessage *message) {
    server->writeToAll(message->message);
}
