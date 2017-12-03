//
//  L293D.hpp
//  RPiMaster
//
//  Created by David Kopala on 12/3/17.
//  Copyright © 2017 David Kopala. All rights reserved.
//

#ifndef L293D_hpp
#define L293D_hpp

#include <stdio.h>
#include <wiringPi.h>

class L293D {

private:
    int type;
    int fPin;
    int rPin;
    
    void initStuff();
    
public:
    L293D(int pinF, int pinB, int type = 0);
    int getType() {
        return type;
    };
    int getFPin() {
        return fPin;
    }
    int getRPin() {
        return rPin;
    }
    
    void forward();
    void reverse();
    void stop();
};

#endif /* L293D_hpp */
