#include "AdafruitGPS.hpp"

AdafruitGPS::AdafruitGPS() {
    latitude = -1;
    longitude = -1;
}

void AdafruitGPS::update() {
    ifstream in("gps_data.txt");
    if (!in.is_open()) {
        latitude = -1;
        longitude = -1;
        return;
    }
    
    string temp;
    getline(in, temp);
    latitude = atof(temp);
    getline(in, temp);
    longitude = atof(temp);
}


