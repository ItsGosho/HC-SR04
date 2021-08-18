#ifndef HC_SR04_MEASUREMENTCONFIGURATION_H
#define HC_SR04_MEASUREMENTCONFIGURATION_H

#include "Arduino.h"
#include "TemperatureUnits.h"
#include "DistanceUnits.h"
#include "Optional.h"

class MeasurementConfiguration {

private:

    unsigned int* samples;
    float* maxDistanceValue;
    DistanceUnit* maxDistanceUnit;
    float* temperatureValue;
    TemperatureUnit* temperatureUnit;
    unsigned long* responseTimeoutUS;
    DistanceUnit* measurementDistanceUnit;

public:
    class builder;

    MeasurementConfiguration(unsigned int* samples, float* maxDistanceValue, DistanceUnit* maxDistanceUnit, float* temperatureValue, TemperatureUnit* temperatureUnit, unsigned long* responseTimeoutUS, DistanceUnit* measurementDistanceUnit)
            : samples(samples), maxDistanceValue(maxDistanceValue), maxDistanceUnit(maxDistanceUnit), temperatureValue(temperatureValue), temperatureUnit(
            temperatureUnit), responseTimeoutUS(responseTimeoutUS), measurementDistanceUnit(measurementDistanceUnit) {
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

    Optional<unsigned long> getResponseTimeoutUS() const {
        return {this->responseTimeoutUS};
    }

    Optional<DistanceUnit> getMeasurementDistanceUnit() const {
        return {this->measurementDistanceUnit};
    }
};

/*TODO: Explain each of the withX parameters/methods*/
class MeasurementConfiguration::builder {

private:
    unsigned int* mSamples;
    float* mMaxDistanceValue;
    DistanceUnit* mMaxDistanceUnit;
    float* mTemperatureValue;
    TemperatureUnit* mTemperatureUnit;
    unsigned long* mResponseTimeoutUS;
    DistanceUnit* mMeasurementDistanceUnit;

public:
    builder() {
        this->mSamples = nullptr;
        this->mMaxDistanceValue = nullptr;
        this->mMaxDistanceUnit = nullptr;
        this->mTemperatureValue = nullptr;
        this->mTemperatureUnit = nullptr;
        this->mResponseTimeoutUS = nullptr;
        this->mMeasurementDistanceUnit = nullptr;
    }

    builder& withSamples(const unsigned int& samples) {
        this->mSamples = &const_cast<unsigned int&>(samples);
        return *this;
    }

    builder& withMaxDistance(const float& maxDistance, const DistanceUnit& maxDistanceUnit) {
        this->mMaxDistanceValue = &const_cast<float&>(maxDistance);
        this->mMaxDistanceUnit = &const_cast<DistanceUnit&>(maxDistanceUnit);
        return *this;
    }

    builder& withTemperature(const float& temperature, const TemperatureUnit& temperatureUnit) {
        this->mTemperatureValue = &const_cast<float&>(temperature);
        this->mTemperatureUnit = &const_cast<TemperatureUnit&>(temperatureUnit);
        return *this;
    }

    builder& withResponseTimeoutUS(const unsigned long& responseTimeoutUS) {
        this->mResponseTimeoutUS = &const_cast<unsigned long&>(responseTimeoutUS);
        return *this;
    }

    builder& withMeasurementDistanceUnit(const DistanceUnit& measurementDistanceUnit) {
        this->mMeasurementDistanceUnit = &const_cast<DistanceUnit&>(measurementDistanceUnit);
        return *this;
    }

    MeasurementConfiguration build() const {
        return {this->mSamples, this->mMaxDistanceValue, this->mMaxDistanceUnit, this->mTemperatureValue, this->mTemperatureUnit, this->mResponseTimeoutUS, this->mMeasurementDistanceUnit};
    }

};

#endif //HC_SR04_MEASUREMENTCONFIGURATION_H
