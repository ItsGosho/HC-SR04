#ifndef HC_SR04_MEASUREMENTCONFIGURATION_H
#define HC_SR04_MEASUREMENTCONFIGURATION_H

#include "Arduino.h"
#include "TemperatureUnits.h"
#include "DistanceUnits.h"
#include "Optional.h"

class MeasurementConfiguration {

private:

    unsigned int* samples;
    float* maxDistance;
    DistanceUnit* maxDistanceUnit;
    float* temperature;
    TemperatureUnit* temperatureUnit;
    unsigned long* responseTimeoutUS;
    DistanceUnit* measurementDistanceUnit;

public:
    class builder;

    MeasurementConfiguration(unsigned int* samples, float* maxDistance, DistanceUnit* maxDistanceUnit, float* temperature, TemperatureUnit* temperatureUnit, unsigned long* responseTimeoutUS, DistanceUnit* measurementDistanceUnit)
            : samples(samples), maxDistance(maxDistance), maxDistanceUnit(maxDistanceUnit), temperature(temperature), temperatureUnit(
            temperatureUnit), responseTimeoutUS(responseTimeoutUS), measurementDistanceUnit(measurementDistanceUnit) {
    }

    Optional<unsigned int> getSamples() const {
        return {this->samples};
    }

    Optional<float> getMaxDistance() const {
        return {this->maxDistance};
    }

    Optional<DistanceUnit> getMaxDistanceUnit() const {
        return {this->maxDistanceUnit};
    }

    Optional<float> getTemperature() const {
        return {this->temperature};
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

class MeasurementConfiguration::builder {

private:
    unsigned int* mSamples;
    float* mMaxDistance;
    DistanceUnit* mMaxDistanceUnit;
    float* mTemperature;
    TemperatureUnit* mTemperatureUnit;
    unsigned long* mResponseTimeoutUS;
    DistanceUnit* mMeasurementDistanceUnit;

public:
    builder() {
        this->mSamples = nullptr;
        this->mMaxDistance = nullptr;
        this->mMaxDistanceUnit = nullptr;
        this->mTemperature = nullptr;
        this->mTemperatureUnit = nullptr;
        this->mResponseTimeoutUS = nullptr;
        this->mMeasurementDistanceUnit = nullptr;
    }

    builder& withSamples(const unsigned int& samples) {
        this->mSamples = &const_cast<unsigned int&>(samples);
        return *this;
    }

    builder& withMaxDistance(const float& maxDistance, const DistanceUnit& maxDistanceUnit) {
        this->mMaxDistance = &const_cast<float&>(maxDistance);
        this->mMaxDistanceUnit = &const_cast<DistanceUnit&>(maxDistanceUnit);
        return *this;
    }

    builder& withTemperature(const float& temperature, const TemperatureUnit& temperatureUnit) {
        this->mTemperature = &const_cast<float&>(temperature);
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
        return {this->mSamples, this->mMaxDistance, this->mMaxDistanceUnit, this->mTemperature, this->mTemperatureUnit, this->mResponseTimeoutUS, this->mMeasurementDistanceUnit};
    }

};

#endif //HC_SR04_MEASUREMENTCONFIGURATION_H
