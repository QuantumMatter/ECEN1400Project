#include <stdlib.h>
#include <fstream>

using namespace std;

class AdafruitGPS {
    
private:
    float longitude;
    float latitude;
    
public:
    AdafruitGPS();
    void update();
    float getLongitude() {
        return longitude;
    };
    float getLatitude() {
        return latitude;
    };
};
