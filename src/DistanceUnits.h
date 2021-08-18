#ifndef HC_SR04_DISTANCEUNITS_H
#define HC_SR04_DISTANCEUNITS_H

enum class DistanceUnit {

    //Metric
    CENTIMETERS, METERS,

    //Imperial
    INCH, FEET, YARD
};

float convertDistanceUnit(const float& distance, const DistanceUnit& fromUnit, const DistanceUnit& toUnit);
float convertCentimetersTo(const float& centimeters, const DistanceUnit& toUnit);
float convertMetersTo(const float& meters, const DistanceUnit& toUnit);
float convertInchesTo(const float& inches, const DistanceUnit& toUnit);
float convertFeetTo(const float& feet, const DistanceUnit& toUnit);
float convertYardsTo(const float& yards, const DistanceUnit& toUnit);


#endif //HC_SR04_DISTANCEUNITS_H
