//
//  main.cpp
//  RPiMaster
//
//  Created by David Kopala on 11/7/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include <iostream>
#include <functional>
#include "Client.h"

#define SERV_ADDR "127.0.0.1"//"52.38.18.162"
#define SERV_PORT 15003

using namespace std;

void clientReadCallback(char *message) {
    cout<<message<<endl;
}

void streamReqCallback(char *message) {
    if (strcmp(message, "connected") == 0) {
        Client *client = new Client(SERV_ADDR, SERV_PORT);
        client->readCallback = clientReadCallback;
    } else {
        cout<<"could not connect"<<endl;
    }
}

int main(int argc, const char * argv[]) {
    //Connect to web server => davidkopala.com
    //Ask to open port 15003 if not already open
    //Write message AFTER connecting callback
    //  Message could be received before callback is connected
    Client *newStreamReq = new Client(SERV_ADDR, 13456);
    newStreamReq->readCallback = streamReqCallback;
    newStreamReq->writeMessage("15003");
    //Connect to the stream in the callback
    
    while (true) {
        
    }
    
    
    return 0;
}
