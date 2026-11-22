
#include <iostream>
#include "GpsValidator.h"
using namespace std;

int main() {
    double classLatitude = 10.8000;
    double classLongitude = 106.7000;

    bool inside = GpsValidator::isWithinRadius(
        10.8001, 106.7001,
        classLatitude, classLongitude, 100
    );

    bool outside = GpsValidator::isWithinRadius(
        10.8100, 106.7100,
        classLatitude, classLongitude, 100
    );

    cout << "Inside radius: " << inside << endl;
    cout << "Outside radius: " << outside << endl;

    return 0;
}
