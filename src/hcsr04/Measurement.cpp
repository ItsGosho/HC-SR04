#include "Measurement.h"

Measurement::Measurement() {
    this->distance = 0.00f;
    this->distanceUnit = DistanceUnit::CENTIMETERS;
    this->signalTimedOutCount = 0;
    this->responseTimedOutCount = 0;
    this->maxDistanceExceededCount = 0;
    this->isResponseCoolDownActive = false;
}

Measurement::Measurement(float distance,
                         DistanceUnit distanceUnit,
                         unsigned int takenSamples,
                         unsigned int signalTimedOutCount,
                         unsigned int responseTimedOutCount,
                         unsigned int maxDistanceExceededCount,
                         bool isResponseCoolDownActive)
                         :
                         distance(distance),
                         distanceUnit(distanceUnit),
                         takenSamples(takenSamples),
                         signalTimedOutCount(signalTimedOutCount),
                         responseTimedOutCount(responseTimedOutCount),
                         maxDistanceExceededCount(maxDistanceExceededCount),
                         isResponseCoolDownActive(isResponseCoolDownActive){
}


float Measurement::getDistance() const {
    return this->distance;
}

DistanceUnit Measurement::getDistanceUnit() const {
    return this->distanceUnit;
}

unsigned int Measurement::getTakenSamples() const {
    return this->takenSamples;
}

unsigned int Measurement::getSignalTimedOutCount() const {
    return this->signalTimedOutCount;
}

unsigned int Measurement::getResponseTimedOutCount() const {
    return this->responseTimedOutCount;
}

unsigned int Measurement::getMaxDistanceExceededCount() const {
    return this->maxDistanceExceededCount;
}

unsigned long Measurement::getInvalidMeasurementsCount() {
    return this->getSignalTimedOutCount() + this->getResponseTimedOutCount() + this->getMaxDistanceExceededCount();
}

unsigned long Measurement::getValidMeasurementsCount() {
    return this->takenSamples - this->getInvalidMeasurementsCount();
}

bool Measurement::getIsResponseCoolDownActive() const {
    return this->isResponseCoolDownActive;
}
