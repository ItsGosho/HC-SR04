#ifndef HC_SR04_DISTANCEUNITS_H
#define HC_SR04_DISTANCEUNITS_H

enum class DistanceUnit {

    //Metric
    CENTIMETERS, METERS,

    //Imperial
    INCH, FEET, YARD
};

float convertDistanceUnit(const float& distance, const DistanceUnit& fromUnit, const DistanceUnit& toUnit);


#endif //HC_SR04_DISTANCEUNITS_H
