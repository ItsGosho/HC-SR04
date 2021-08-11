#ifndef HC_SR04_HCSR04_H
#define HC_SR04_HCSR04_H

#include <Arduino.h>

#define TRIGGER_SIGNAL_LENGTH_US 10
#define TIMEOUT_SIGNAL_LENGTH_US 38000
#define COOL_DOWN_DELAY_MS 60

/*
 * TODO:
 * Multiple samples
 * Timeouts
 * Temperature hookup generic way (maybe just passer of temperature to be passed in given degree)
 *
 * */
class HCSR04 {

private:

    uint8_t triggerPin;
    uint8_t echoPin;

    unsigned long HCSR04::measureSignalLength(const uint8_t& pin, const int& mode);

public:

    HCSR04(const uint8_t& triggerPin, const uint8_t& echoPin);
    float measure();
};


#endif //HC_SR04_HCSR04_H