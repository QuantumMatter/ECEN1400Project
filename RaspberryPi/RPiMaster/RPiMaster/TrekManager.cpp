//
//  TrekManager.cpp
//  LibCurl
//
//  Created by David Kopala on 12/4/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#include "TrekManager.hpp"

string TrekManager::readBuffer;

TrekManager::TrekManager() {
    latitude = -1;
    longitude = -1;
    trekID = -1;
}

TrekManager::~TrekManager() {
    curl_easy_cleanup(curl);
}

size_t TrekManager::CurlWriteCallback(char *content, size_t size, size_t nmeb, void *userp) {
    readBuffer.append(content);
    return 0;
}

void TrekManager::startTrek() {
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    CURLcode res;
    
    if (curl) {
        //set the url
        curl_easy_setopt(curl, CURLOPT_URL, "https://davidkopala.com/ecen/dbconn.php?table=treks");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &TrekManager::CurlWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        res = curl_easy_perform(curl);
        
        if (readBuffer.substr(3, 2) == "ID") {
            string id = "";
            for (int i = 8; i < readBuffer.length(); i++) {
                if (readBuffer[i] != '"') {
                    id += readBuffer[i];
                } else {
                    trekID = atoi(id.c_str());
                    break;
                }
            }
        }
        readBuffer = "";
    }
}

void TrekManager::postData(string key, string data, string units) {
    if(curl) {
        string postData = "TrekID=" + to_string(trekID);
        postData += "&type=" + key;
        postData += "&latitude=" + to_string(latitude);
        postData += "&longitude=" + to_string(longitude);
        postData += "&value=" + data;
        postData += "&Units=" + units;
        
        curl_easy_setopt(curl, CURLOPT_URL, "https://davidkopala.com/ecen/dbconn.php?table=data");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &TrekManager::CurlWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        curl_easy_perform(curl);
        
        cout<<readBuffer<<endl;
        readBuffer = "";
    }
}
