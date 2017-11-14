//
//  StreamForwarding.hpp
//  Stream Forwarding
//
//  Created by David Kopala on 11/7/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef StreamForwarding_hpp
#define StreamForwarding_hpp

#include <stdio.h>
#include "Server.h"

class StreamForwarding {
    
public:
    //Constructor - creates a new server on that port
    StreamForwarding(int port);
    //port getter
    int getPort() {
        return port;
    }
    
    //function that handles new clients
    static void newConnectionHandler();
    //function that handles data from the clients
        //writes data back to all clients
    static void readHandler(Server::TCPMessage *message);
    
private:
    //the server instance
    static Server *server;
    //the port number
    int port;
};

#endif /* StreamForwarding_hpp */
