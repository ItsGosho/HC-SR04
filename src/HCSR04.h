#ifndef HC_SR04_HCSR04_H
#define HC_SR04_HCSR04_H

#include <Arduino.h>
#include <MeasurementConfiguration.h>
#include <Utils.h>

#define TRIGGER_SIGNAL_LENGTH_US 10
#define TIMEOUT_SIGNAL_LENGTH_US 38000
#define COOL_DOWN_DELAY_MS 60

#define DEFAULT_RESPONSE_TIMEOUT_US 1000000
#define DEFAULT_SAMPLES 1
#define DEFAULT_TEMPERATURE_CELSIUS 25.00f
#define DEFAULT_MAX_DISTANCE_CENTIMETERS 10.00f

/*
 * TODO:
 * Multiple samples : DONE
 * Timeouts
 * Temperature hookup generic way (maybe just passer of temperature to be passed in given degree)
 * Max distance, which can be present and returned in the object if exceed, but the timed out must be more specific in the variable name
 * ONE WIRE MODE
 * Option in the configuration to exlude/include in multiple samples timeouts
 *
 * */

struct Measurement {

    float distance;
    DistanceUnit distanceUnit;
    bool isSignalTimedOut;
    bool isResponseTimedOut;
    bool isMaxDistanceExceeded;

};

class HCSR04Response {

private:
    unsigned long mHighSignalLengthUS;
    bool mIsResponseTimedOut;
    bool mIsSignalTimedOut;

public:

    HCSR04Response() {
        this->mHighSignalLengthUS = 0;
        this->mIsResponseTimedOut = false;
        this->mIsSignalTimedOut = false;
    }

    HCSR04Response(unsigned long mHighSignalLengthUs, bool mIsResponseTimedOut) : mHighSignalLengthUS(
            mHighSignalLengthUs), mIsResponseTimedOut(mIsResponseTimedOut) {
    }

    unsigned long getHighSignalLengthUS() const {
        return this->mHighSignalLengthUS;
    }

    bool isResponseTimedOut() const {
        return this->mHighSignalLengthUS;
    }

    bool isSignalTimedOut() const {
        return this->mHighSignalLengthUS >= TIMEOUT_SIGNAL_LENGTH_US;
    }
};

class HCSR04 {

private:

    uint8_t triggerPin;
    uint8_t echoPin;

    float calculateDistanceBySignalLength(const unsigned int& signalLength);

    float calculateDistanceBySignalLengthAndSoundSpeed(const unsigned int& signalLength, const float& soundSpeed);

    float convertMetersPerSecondToCentimetersPerMicrosecond(const float& metersPerSecond);

    float calculateSoundSpeedByTemperature(const float& temperature, const TemperatureUnit& temperatureUnit);

    void sendTriggerSignalToHCSR04();

public:

    HCSR04(const uint8_t& triggerPin, const uint8_t& echoPin);

    Measurement measure(const MeasurementConfiguration& configuration);

    HCSR04Response sendAndReceivedToHCSR04();

    void sendAndReceivedToHCSR04(HCSR04Response hcsr04Responses[], const unsigned int& times);

    void sendAndReceivedToHCSR04(HCSR04Response hcsr04Responses[], const unsigned int& times, const unsigned long& responseTimeOutUS);

    HCSR04Response sendAndReceivedToHCSR04(const unsigned long& responseTimeOutUS);
};


#endif //HC_SR04_HCSR04_H
