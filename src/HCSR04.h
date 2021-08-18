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
#define DEFAULT_MAX_DISTANCE_CENTIMETERS 400.00f

/*
 * TODO: ONE WIRE MODE
 * TODO: Kakvo e tova Gaussian Distribution
 *  - https://www.google.com/search?q=Normal/Gauss&client=firefox-b-d&sa=X&bih=978&biw=1920&hl=bg&sxsrf=ALeKk00_Y6OwSg4B8J4a2AA5paCG14wGmg:1629239720130&tbm=isch&source=iu&ictx=1&fir=WrRoXwMPpcaVIM%252CJeTJtglhtF9QbM%252C_&vet=1&usg=AI4_-kQl3xF_o63DZo_xyNw4FGlOZ8NyBg&ved=2ahUKEwjbk5b2jrnyAhWP16QKHU0xAMQQ_h16BAgbEAE#imgrc=WrRoXwMPpcaVIM
 *  - https://www.khanacademy.org/math/statistics-probability/modeling-distributions-of-data/more-on-normal-distributions/v/introduction-to-the-normal-distribution
 *
 *  TODO: Kakvo e tova noInterrupts(); v arduino
 *
 * */

/*TODO: Separate class .h and .cpp*/
class Measurement {

private:
    float distance;
    DistanceUnit distanceUnit;

    unsigned int takenSamples;

    unsigned int signalTimedOutCount;
    unsigned int responseTimedOutCount;
    unsigned int maxDistanceExceededCount;

public:

    Measurement() {
        this->distance = 0.00f;
        this->distanceUnit = DistanceUnit::CENTIMETERS;
        this->signalTimedOutCount = 0;
        this->responseTimedOutCount = 0;
        this->maxDistanceExceededCount = 0;
    }

    Measurement(float distance, DistanceUnit distanceUnit, unsigned int takenSamples, unsigned int signalTimedOutCount, unsigned int responseTimedOutCount, unsigned int maxDistanceExceededCount)
            : distance(distance), distanceUnit(distanceUnit), takenSamples(takenSamples), signalTimedOutCount(
            signalTimedOutCount), responseTimedOutCount(responseTimedOutCount), maxDistanceExceededCount(
            maxDistanceExceededCount) {
    }

    float getDistance() const {
        return this->distance;
    }

    DistanceUnit getDistanceUnit() const {
        return this->distanceUnit;
    }

    unsigned int getTakenSamples() const {
        return this->takenSamples;
    }

    unsigned int getSignalTimedOutCount() const {
        return this->signalTimedOutCount;
    }

    unsigned int getResponseTimedOutCount() const {
        return this->responseTimedOutCount;
    }

    unsigned int getMaxDistanceExceededCount() const {
        return this->maxDistanceExceededCount;
    }

    unsigned long getInvalidMeasurementsCount() {
        return this->getSignalTimedOutCount() + this->getResponseTimedOutCount() + this->getMaxDistanceExceededCount();
    }

    unsigned long getValidMeasurementsCount() {
        return this->takenSamples - this->getInvalidMeasurementsCount();
    }
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
        return this->mIsResponseTimedOut;
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
