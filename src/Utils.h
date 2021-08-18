#ifndef HC_SR04_UTILS_H
#define HC_SR04_UTILS_H

#include <Arduino.h>

struct SignalLengthMeasurementUS {

    unsigned long signalLengthUS;
    bool isTimedOut;
};

bool waitStateNot(const unsigned int& digitalPin, const char& expectedState, const unsigned long& timeoutUS);

bool waitStateIs(const unsigned int& digitalPin, const char& expectedState, const unsigned long& timeoutUS);

unsigned long measureSignalLength(const uint8_t& pin, const int& mode);

SignalLengthMeasurementUS measureSignalLength(const uint8_t& pin, const char& mode, const unsigned long& timeoutUS);

#endif //HC_SR04_UTILS_H
