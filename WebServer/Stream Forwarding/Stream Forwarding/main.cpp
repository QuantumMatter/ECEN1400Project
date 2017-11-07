//
//  main.cpp
//  Stream Forwarding
//
//  Created by David Kopala on 11/7/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include <iostream>
#include "Server.h"

using namespace std;

bool debug = true;

void newConnectionCallback() {
    if (debug) {
        cout<<"new connection"<<endl;
    }
}

void newMessageCallback(Server::TCPMessage *msg) {
    if (debug) {
        cout<<msg->addr->addr<<":"<<msg->addr->port<<" => "<<msg->message<<endl;
    }
}

void portForward() {
    
}

int main(int argc, const char * argv[]) {
    Server *server = new Server("12345");
    server->newConnectionCallback = newConnectionCallback;
    server->newMessageCallback = newMessageCallback;
    return 0;
}
