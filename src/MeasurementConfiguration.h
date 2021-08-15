#ifndef HC_SR04_MEASUREMENTCONFIGURATION_H
#define HC_SR04_MEASUREMENTCONFIGURATION_H

#include "Arduino.h"

enum class TemperatureUnit {
    CELSIUS, FAHRENHEIT
};

enum class DistanceUnit {

    //Metric
    CENTIMETERS, METERS,

    //Imperial
    INCH, FEET, YARD
};

class MeasurementConfiguration {

private:

    unsigned int* samples;
    float* maxDistance;
    DistanceUnit* maxDistanceUnit;
    float* temperature;
    TemperatureUnit* temperatureUnit;
    bool excludeTimeouts;

public:
    class builder;

    MeasurementConfiguration(unsigned int* samples, float* maxDistance, DistanceUnit* maxDistanceUnit, float* temperature)
            : samples(samples), maxDistance(maxDistance), maxDistanceUnit(maxDistanceUnit), temperature(temperature) {
    }

    unsigned int* getSamples() const {
        return this->samples;
    }

    float* getMaxDistance() const {
        return this->maxDistance;
    }

    DistanceUnit* getMaxDistanceUnit() const {
        return this->maxDistanceUnit;
    }

    float* getTemperature() const {
        return this->temperature;
    }

    TemperatureUnit* getTemperatureUnit() const {
        return this->temperatureUnit;
    }

};

class MeasurementConfiguration::builder {

private:
    unsigned int* samples;
    float* maxDistance;
    DistanceUnit* maxDistanceUnit;
    float* temperature;
    TemperatureUnit* temperatureUnit;
    float* humidityPercentage;

public:
    builder() {
        this->samples = nullptr;
        this->maxDistance = nullptr;
        this->maxDistanceUnit = nullptr;
        this->temperature = nullptr;
        this->temperatureUnit = nullptr;
        this->humidityPercentage = nullptr;
    }

    builder& withSamples(const unsigned int& samples) {
        this->samples = &const_cast<unsigned int&>(samples);
        return *this;
    }

    builder& withMaxDistance(const float& maxDistance, const DistanceUnit& maxDistanceUnit) {
        this->maxDistance = &const_cast<float&>(maxDistance);
        this->maxDistanceUnit = &const_cast<DistanceUnit&>(maxDistanceUnit);
        return *this;
    }

    builder& withTemperature(const float& temperature, const TemperatureUnit& temperatureUnit) {
        this->temperature = &const_cast<float&>(temperature);
        this->temperatureUnit = &const_cast<TemperatureUnit&>(temperatureUnit);
        return *this;
    }

    MeasurementConfiguration build() const {

        return MeasurementConfiguration(this->samples, this->maxDistance, this->maxDistanceUnit, this->temperature);
    }

};

#endif //HC_SR04_MEASUREMENTCONFIGURATION_H
