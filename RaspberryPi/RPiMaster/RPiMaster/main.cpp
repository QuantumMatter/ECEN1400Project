//
//  main.cpp
//  RPiMaster
//
//  Created by David Kopala on 11/7/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//


#include <iostream>
#include <wiringPi.h>

#include "Client.h"
#include "L293D.hpp"
#include "ArduinoSlave.hpp"
#include "TrekManager.hpp"
#include "MPL3115A2.hpp"
#include "AdafruitGPS.hpp"

#define SERV_ADDR   "52.38.18.162"
#define SERV_PORT   15003
#define LED         0
//Pin 0 => GPIO Pin BCM 17
#define FPIN_LEFT   25
//Pin 25 => GPIO Pin BCM 26
#define RPIN_LEFT   24
//Pin 25 => GPIO Pin BCM 19
#define FPIN_RIGHT  23
//Pin 23 => GPIO Pin BMC 13
#define RPIN_RIGHT  22
//Pin 22 => GPIO Pin BCM 6

using namespace std;

L293D *leftMotor;
L293D *rightMotor;

//Process data from server
void clientReadCallback(char *message) {
    //cout<<message<<endl;
    if(strlen(message) > 0) {
        if (message[0] == 'o') {
            //Turn LED On
            digitalWrite(LED, 1);
            cout<<"turning LED 0 on"<<endl;
        } else if (message[0] == 'f') {
            //Turn LED Off
            digitalWrite(LED, 0);
            cout<<"turning LED 0 off"<<endl;
        } else if (message[0] == 'w') {
            leftMotor->forward();
            rightMotor->forward();
            cout<<"forward"<<endl;
        } else if (message[0] == 'a') {
            leftMotor->reverse();
            rightMotor->forward();
            cout<<"left"<<endl;
        } else if (message[0] == 's') {
            leftMotor->reverse();
            rightMotor->reverse();
            cout<<"backwards"<<endl;
        } else if (message [0] == 'd') {
            leftMotor->forward();
            rightMotor->reverse();
            cout<<"right"<<endl;
        } else if (message[0] == ' ') {
            leftMotor->stop();
            rightMotor->stop();
            cout<<"stop"<<endl;
        }
    }
}

void streamReqCallback(char *message) {
    if (strcmp(message, "connected") == 0) {
        // Client *client = new Client(SERV_ADDR, SERV_PORT);
        // client->readCallback = clientReadCallback;
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
    
    //Configure WiringPi GPIO utility
    wiringPiSetup();
    pinMode(LED, OUTPUT);
    
    //Setup motors
    rightMotor = new L293D(FPIN_RIGHT, RPIN_RIGHT);
    leftMotor = new L293D(FPIN_LEFT, RPIN_LEFT);
    
    //Setup the Arduino Slave
    ArduinoSlave *arduino = new ArduinoSlave(0x05);
    
    //Setup the altimeter
    MPL3115A2 *altimeter = new MPL3115A2();
    
    //Setup TrekManger
    TrekManager *trek = new TrekManager();
    trek->startTrek();
    
    //Setup GPS
    AdafruitGPS *gps = new AdafruitGPS();
    
    //Wait half second to allow server to open port
    delay(500);
    Client *client = new Client(SERV_ADDR, SERV_PORT);
    client->readCallback = clientReadCallback;
    
    cout<<"starting loop"<<endl;
    int colorIndex = 0;
    while (true) {
        //Make all of the sensors update their locally stored values
        arduino->read();
        altimeter->update();
        gps->update();
        
        //Update the gps
        trek->latitude = gps->getLatitude();
        trek->longitude = gps->getLongitude();
        
        //Post Light Value to Server
        int light = arduino->getData();
        trek->postData("Light", to_string(light/100.0), "V");
        
        //Post altimeter data to server
        trek->postData("Altitude", to_string(altimeter->getAltitude()), "m");
        trek->postData("Pressure", to_string(altimeter->getPressure()), "kPa");
        trek->postData("Temperature", to_string(altimeter->getCelcius()), "C");
        
        //Post humidity data to server
        cout<<"arduino read"<<endl;
        arduino->read();
        trek->postData("Humidity", to_string(arduino->getData()), "%");
        
        delay(2000);
    }
    
    
    return 0;
}

