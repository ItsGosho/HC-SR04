#ifndef HC_SR04_MEASUREMENTCONFIGURATION_H
#define HC_SR04_MEASUREMENTCONFIGURATION_H

#include "Arduino.h"
#include "TemperatureUnits.h"
#include "hcsr04/DistanceUnits.h"
#include "Optional.h"

class MeasurementConfiguration {

private:

    unsigned int* samples;
    float* maxDistanceValue;
    DistanceUnit* maxDistanceUnit;
    float* temperatureValue;
    TemperatureUnit* temperatureUnit;
    unsigned long* responseTimeoutMS;
    DistanceUnit* measurementDistanceUnit;

    unsigned long* responseTimeoutCoolDownTimeMS;

public:
    class builder;

    MeasurementConfiguration(unsigned int* samples,
                             float* maxDistanceValue,
                             DistanceUnit* maxDistanceUnit,
                             float* temperatureValue,
                             TemperatureUnit* temperatureUnit,
                             unsigned long* responseTimeoutMS,
                             DistanceUnit* measurementDistanceUnit,
                             unsigned long* responseTimeoutCoolDownTimeMS
                             )
                             :
                             samples(samples),
                             maxDistanceValue(maxDistanceValue),
                             maxDistanceUnit(maxDistanceUnit),
                             temperatureValue(temperatureValue),
                             temperatureUnit(temperatureUnit),
                             responseTimeoutMS(responseTimeoutMS),
                             measurementDistanceUnit(measurementDistanceUnit),
                             responseTimeoutCoolDownTimeMS(responseTimeoutCoolDownTimeMS)
                             {
    }

    Optional<unsigned int> getSamples() const {
        return {this->samples};
    }

    Optional<float> getMaxDistanceValue() const {
        return {this->maxDistanceValue};
    }

    Optional<DistanceUnit> getMaxDistanceUnit() const {
        return {this->maxDistanceUnit};
    }

    Optional<float> getTemperatureValue() const {
        return {this->temperatureValue};
    }

    Optional<TemperatureUnit> getTemperatureUnit() const {
        return {this->temperatureUnit};
    }

    Optional<unsigned long> getResponseTimeoutMS() const {
        return {this->responseTimeoutMS};
    }

    Optional<DistanceUnit> getMeasurementDistanceUnit() const {
        return {this->measurementDistanceUnit};
    }

    Optional<unsigned long> getResponseTimeoutCoolDownTimeMS() const {
        return {this->responseTimeoutCoolDownTimeMS};
    }
};

class MeasurementConfiguration::builder {

private:
    unsigned int* mSamples;
    float* mMaxDistanceValue;
    DistanceUnit* mMaxDistanceUnit;
    float* mTemperatureValue;
    TemperatureUnit* mTemperatureUnit;
    unsigned long* mResponseTimeoutMS;
    DistanceUnit* mMeasurementDistanceUnit;

    unsigned long* mResponseTimeoutCoolDownTimeMS;

public:
    builder() {
        this->mSamples = nullptr;
        this->mMaxDistanceValue = nullptr;
        this->mMaxDistanceUnit = nullptr;
        this->mTemperatureValue = nullptr;
        this->mTemperatureUnit = nullptr;
        this->mResponseTimeoutMS = nullptr;
        this->mMeasurementDistanceUnit = nullptr;
        this->mResponseTimeoutCoolDownTimeMS = nullptr;
    }

    /**
      * How many times to take measurement. Then the returned distance will be the average of a valid measurements.
      */
    builder& withSamples(const unsigned int& samples) {
        this->mSamples = &const_cast<unsigned int&>(samples);
        return *this;
    }

    /**
      * The maximum distance that is allowed. If a measurement's distance exceeds it, then the measurement is invalid.
      */
    builder& withMaxDistance(const float& maxDistance, const DistanceUnit& maxDistanceUnit) {
        this->mMaxDistanceValue = &const_cast<float&>(maxDistance);
        this->mMaxDistanceUnit = &const_cast<DistanceUnit&>(maxDistanceUnit);
        return *this;
    }

    /**
      *  The ambient temperature.
      * Increases the accuracy of the measurement, because the sound speed is dependent on temperature.
      */
    builder& withTemperature(const float& temperature, const TemperatureUnit& temperatureUnit) {
        this->mTemperatureValue = &const_cast<float&>(temperature);
        this->mTemperatureUnit = &const_cast<TemperatureUnit&>(temperatureUnit);
        return *this;
    }

    /**
      * The maximum time to take a measurement.
      * If the time is exceeded then the measurement is invalid.
      * Indication when there is something wrong with the communication to the device eg: not connected
      */
    builder& withResponseTimeoutMS(const unsigned long& responseTimeoutMS) {
        this->mResponseTimeoutMS = &const_cast<unsigned long&>(responseTimeoutMS);
        return *this;
    }

    /**
      * In what distance unit the measurement will be returned.
      */
    builder& withMeasurementDistanceUnit(const DistanceUnit& measurementDistanceUnit) {
        this->mMeasurementDistanceUnit = &const_cast<DistanceUnit&>(measurementDistanceUnit);
        return *this;
    }

    /**
      * Let's say that you have a measurement in your program with **5** samples.
      * If the **HCSR04** is not connected, then there will be no samples, but you will lose time for example **~315 milliseconds** on **each loop**.
      * To avoid that and you can try measuring after specific time again. That is where the cooldowns come.
      * The cool down will be **activated**, when **all of the samples** that have been measured have **timed out**. If a **measurement fails again**, then it will be **activated again** and so on.
      */
    builder& withResponseTimeoutCoolDown(const unsigned long& responseTimeoutCoolDownTimeMS) {
        this->mResponseTimeoutCoolDownTimeMS = &const_cast<unsigned long&>(responseTimeoutCoolDownTimeMS);
        return *this;
    }

    MeasurementConfiguration build() const {
        return {this->mSamples,
                this->mMaxDistanceValue,
                this->mMaxDistanceUnit,
                this->mTemperatureValue,
                this->mTemperatureUnit,
                this->mResponseTimeoutMS,
                this->mMeasurementDistanceUnit,
                this->mResponseTimeoutCoolDownTimeMS};
    }

};

#endif //HC_SR04_MEASUREMENTCONFIGURATION_H
