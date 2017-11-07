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

using namespace std;

bool debug = true;

List<StreamForwarding> *forwardings = new List<StreamForwarding>();

void newConnectionCallback() {
    if (debug) {
        cout<<"new connection"<<endl;
    }
}

void newMessageCallback(Server::TCPMessage *msg) {
    if (debug) {
        cout<<msg->addr->addr<<":"<<msg->addr->port<<" => "<<msg->message<<endl;
    }
    if(msg->addr->port == 12345) {
        StreamForwarding *forward = new StreamForwarding(atoi(msg->message));
        forwardings->add(forward);
    }
}

void portForward() {
    
}

int main(int argc, const char * argv[]) {
    if (false) {
        Server *server = new Server(12345);
        server->newConnectionCallback = newConnectionCallback;
        server->newMessageCallback = newMessageCallback;
        
        Server *test = new Server(11111);
        server->newConnectionCallback = newConnectionCallback;
        server->newMessageCallback = newMessageCallback;
        
        while (true) {
            
        }
    } else {
        Server *server = new Server(12345);
        server->newConnectionCallback = newConnectionCallback;
        server->newMessageCallback = newMessageCallback;
        
        while (true) {
            
        }
    }
    return 0;
}
