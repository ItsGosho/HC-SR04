#ifndef HC_SR04_HCSR04_H
#define HC_SR04_HCSR04_H

#include <Arduino.h>
#include "MeasurementConfiguration.h"
#include "Utils.h"
#include "Measurement.h"
#include "HCSR04Response.h"

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

class HCSR04 {

private:

    uint8_t oneWirePin;
    uint8_t triggerPin;
    uint8_t echoPin;

    bool isOneWireMode;

    unsigned int defaultSamples;
    float defaultMaxDistanceValue;
    DistanceUnit defaultMaxDistanceUnit;
    float defaultTemperatureValue;
    TemperatureUnit defaultTemperatureUnit;
    unsigned long defaultResponseTimeoutUS;
    DistanceUnit defaultMeasurementDistanceUnit;

    float calculateDistanceBySignalLength(const unsigned int& signalLength);

    float calculateDistanceBySignalLengthAndSoundSpeed(const unsigned int& signalLength, const float& soundSpeed);

    float calculateDistanceBySignalLengthAndSoundSpeed(const unsigned int& signalLength, const float& soundSpeed, const DistanceUnit& distanceUnit);

    float convertMetersPerSecondToCentimetersPerMicrosecond(const float& metersPerSecond);

    float calculateSoundSpeedByTemperature(const float& temperature, const TemperatureUnit& temperatureUnit);

    void sendTriggerSignalToHCSR04();

public:

    HCSR04(const uint8_t& oneWirePin);

    HCSR04(const uint8_t& triggerPin, const uint8_t& echoPin);

    Measurement measure(const MeasurementConfiguration& configuration);

    HCSR04Response sendAndReceivedToHCSR04();

    void sendAndReceivedToHCSR04(HCSR04Response hcsr04Responses[], const unsigned int& times);

    void sendAndReceivedToHCSR04(HCSR04Response hcsr04Responses[], const unsigned int& times, const unsigned long& responseTimeOutUS);

    HCSR04Response sendAndReceivedToHCSR04(const unsigned long& responseTimeOutUS);

    void setDefaultSamples(const unsigned int& defaultSamples);

    void setDefaultMaxDistanceValue(const float& defaultMaxDistanceValue);

    void setDefaultMaxDistanceUnit(const DistanceUnit& defaultMaxDistanceUnit);

    void setDefaultTemperatureValue(const float& defaultTemperatureValue);

    void setDefaultTemperatureUnit(const TemperatureUnit& defaultTemperatureUnit);

    void setDefaultResponseTimeoutUs(const unsigned long& defaultResponseTimeoutUs);

    void setDefaultMeasurementDistanceUnit(const DistanceUnit& defaultMeasurementDistanceUnit);
};


#endif //HC_SR04_HCSR04_H
