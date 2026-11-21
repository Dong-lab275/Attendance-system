
#ifndef GPSVALIDATOR_H
#define GPSVALIDATOR_H

#include <cmath>

class GpsValidator {
public:
    static double calculateDistance(
        double studentLatitude,
        double studentLongitude,
        double classLatitude,
        double classLongitude
    ) {
        const double EARTH_RADIUS = 6371000.0;
        const double PI = 3.14159265358979323846;

        double latitudeDifference =
            (classLatitude - studentLatitude) * PI / 180.0;

        double longitudeDifference =
            (classLongitude - studentLongitude) * PI / 180.0;

        double latitude1 = studentLatitude * PI / 180.0;
        double latitude2 = classLatitude * PI / 180.0;

        double a =
            std::pow(std::sin(latitudeDifference / 2), 2) +
            std::cos(latitude1) * std::cos(latitude2) *
            std::pow(std::sin(longitudeDifference / 2), 2);

        double c = 2 * std::atan2(
            std::sqrt(a),
            std::sqrt(1 - a)
        );

        return EARTH_RADIUS * c;
    }

    static bool isWithinRadius(
        double studentLatitude,
        double studentLongitude,
        double classLatitude,
        double classLongitude,
        double allowedRadius
    ) {
        if (allowedRadius < 0) {
            return false;
        }

        double distance = calculateDistance(
            studentLatitude,
            studentLongitude,
            classLatitude,
            classLongitude
        );

        return distance <= allowedRadius;
    }
};

#endif
