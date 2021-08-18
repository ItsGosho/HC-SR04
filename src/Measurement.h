#ifndef HC_SR04_MEASUREMENT_H
#define HC_SR04_MEASUREMENT_H

#include "DistanceUnits.h"

class Measurement {

private:
    float distance;
    DistanceUnit distanceUnit;

    unsigned int takenSamples;

    unsigned int signalTimedOutCount;
    unsigned int responseTimedOutCount;
    unsigned int maxDistanceExceededCount;

public:

    Measurement();

    Measurement(float distance, DistanceUnit distanceUnit, unsigned int takenSamples, unsigned int signalTimedOutCount, unsigned int responseTimedOutCount, unsigned int maxDistanceExceededCount);

    float getDistance() const;

    DistanceUnit getDistanceUnit() const;

    unsigned int getTakenSamples() const;

    unsigned int getSignalTimedOutCount() const;

    unsigned int getResponseTimedOutCount() const;

    unsigned int getMaxDistanceExceededCount() const;

    unsigned long getInvalidMeasurementsCount();

    unsigned long getValidMeasurementsCount();
};



#endif //HC_SR04_MEASUREMENT_H
