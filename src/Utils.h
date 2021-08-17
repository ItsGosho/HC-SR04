#ifndef HC_SR04_UTILS_H
#define HC_SR04_UTILS_H

#include <Arduino.h>

struct SignalLengthMeasurementUS {

    unsigned long mSignalLengthUS;
    bool isTimedOut;
};

bool wait

unsigned long measureSignalLength(const uint8_t& pin, const int& mode);

SignalLengthMeasurementUS measureSignalLength(const uint8_t& pin, const int& mode, const unsigned long& timeoutUS);

#endif //HC_SR04_UTILS_H
