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
    StreamForwarding(int port);
    int getPort() {
        return port;
    }
    
    static void newConnectionHandler();
    static void readHandler(Server::TCPMessage *message);
    
private:
    static Server *server;
    int port;
};

#endif /* StreamForwarding_hpp */
