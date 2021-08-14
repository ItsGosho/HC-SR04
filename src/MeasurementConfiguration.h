#ifndef HC_SR04_MEASUREMENTCONFIGURATION_H
#define HC_SR04_MEASUREMENTCONFIGURATION_H

#include "Arduino.h"

enum class TemperatureUnit {
    CELSIUS,
    FAHRENHEIT
};

enum class DistanceUnit {
    CENTIMETERS,
    METERS
};

class MeasurementConfiguration {

private:

    unsigned int* timeoutMS;
    unsigned int* samples;
    unsigned int* distance;
    DistanceUnit* distanceUnit;
    float* temperature;
    TemperatureUnit* temperatureUnit;
    bool excludeTimeouts;

public:
    class builder;

    MeasurementConfiguration(unsigned int* timeoutMs, unsigned int* samples, unsigned int* distance, float* temperature)
            : timeoutMS(timeoutMs), samples(samples), distance(distance), temperature(temperature) {
    }

    unsigned int* getTimeoutMS() const {
        return this->timeoutMS;
    }

    unsigned int* getSamples() const {
        return this->samples;
    }

    unsigned int* getDistance() const {
        return this->distance;
    }

    DistanceUnit* getDistanceUnit() const {
        return this->distanceUnit;
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
    unsigned int* timeoutMS;
    unsigned int* samples;
    unsigned int* distance;
    DistanceUnit* distanceUnit;
    float* temperature;
    TemperatureUnit* temperatureUnit;
    float* humidityPercentage;

public:
    builder() {
        this->timeoutMS = nullptr;
        this->samples = nullptr;
        this->distance = nullptr;
        this->distanceUnit = nullptr;
        this->temperature = nullptr;
        this->temperatureUnit = nullptr;
        this->humidityPercentage = nullptr;
    }

    builder& withTimeoutMS(const unsigned int& timeoutMS) {
        this->timeoutMS = &const_cast<unsigned int&>(timeoutMS);
        return *this;
    }

    builder& withSamples(const unsigned int& samples) {
        this->samples = &const_cast<unsigned int&>(samples);
        return *this;
    }

    builder& withDistance(const unsigned int& distance, const DistanceUnit& distanceUnit) {
        this->distance = &const_cast<unsigned int&>(distance);
        this->distanceUnit = &const_cast<DistanceUnit&>(distanceUnit);
        return *this;
    }

    builder& withTemperature(const float& temperature, const TemperatureUnit& temperatureUnit) {
        this->temperature = &const_cast<float&>(temperature);
        this->temperatureUnit = &const_cast<TemperatureUnit&>(temperatureUnit);
        return *this;
    }

    MeasurementConfiguration build() const {

        return MeasurementConfiguration(this->timeoutMS,
                                        this->samples,
                                        this->distance,
                                        this->temperature);
    }

};

#endif //HC_SR04_MEASUREMENTCONFIGURATION_H
