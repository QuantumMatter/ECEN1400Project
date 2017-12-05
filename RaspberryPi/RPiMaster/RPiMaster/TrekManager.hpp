//
//  TrekManager.hpp
//  LibCurl
//
//  Created by David Kopala on 12/4/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef TrekManager_hpp
#define TrekManager_hpp

#include <stdio.h>
#include <string>
#include <curl/curl.h>
#include <iostream>

using namespace std;

class TrekManager {
    
private:
    int trekID;
    
    CURL *curl;
    static string readBuffer;
    static size_t CurlWriteCallback(char *content, size_t size, size_t nmeb, void *userp);;
    
public:
    TrekManager();
    ~TrekManager();
    void startTrek();
    void postData(string key, string data, string units);
    
    int getTrekID() {
        return trekID;
    };
    
    float latitude;
    float longitude;
    
};

#endif /* TrekManager_hpp */
