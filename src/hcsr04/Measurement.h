#ifndef HC_SR04_MEASUREMENT_H
#define HC_SR04_MEASUREMENT_H

#include "hcsr04/DistanceUnits.h"

class Measurement {

private:
    float distance;
    DistanceUnit distanceUnit;

    unsigned int takenSamples;

    unsigned int signalTimedOutCount;
    unsigned int responseTimedOutCount;
    unsigned int maxDistanceExceededCount;

    bool isResponseCoolDownActive;

public:

    Measurement();

    Measurement(float distance, DistanceUnit distanceUnit, unsigned int takenSamples, unsigned int signalTimedOutCount, unsigned int responseTimedOutCount, unsigned int maxDistanceExceededCount, bool isResponseCoolDownActive);

    float getDistance() const;

    DistanceUnit getDistanceUnit() const;

    unsigned int getTakenSamples() const;

    unsigned int getSignalTimedOutCount() const;

    unsigned int getResponseTimedOutCount() const;

    unsigned int getMaxDistanceExceededCount() const;

    unsigned long getInvalidMeasurementsCount();

    unsigned long getValidMeasurementsCount();

    bool getIsResponseCoolDownActive() const;
};



#endif //HC_SR04_MEASUREMENT_H
