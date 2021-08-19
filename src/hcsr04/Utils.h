#ifndef HC_SR04_UTILS_H
#define HC_SR04_UTILS_H

#include <Arduino.h>

/*TODO: Make them work with all type of time units us, ms. No matter if it is for timeout or the response*/
struct SignalLengthMeasurementUS {

    unsigned long signalLengthUS;
    bool isTimedOut;
};

bool waitStateNot(const unsigned int& digitalPin, const char& expectedState, const unsigned long& timeoutMS);

bool waitStateIs(const unsigned int& digitalPin, const char& expectedState, const unsigned long& timeoutMS);

unsigned long measureSignalLength(const uint8_t& pin, const int& mode);

SignalLengthMeasurementUS measureSignalLength(const uint8_t& pin, const char& mode, const unsigned long& timeoutMS);

#endif //HC_SR04_UTILS_H
