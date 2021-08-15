#include "HCSR04.h"

HCSR04::HCSR04(const uint8_t& triggerPin, const uint8_t& echoPin) : triggerPin(triggerPin), echoPin(echoPin) {
    pinMode(this->triggerPin, OUTPUT);
    pinMode(this->echoPin, INPUT);
}

/**
 * Will measure the length of a given signal.
 * If the current signal is not the given one, then it will block until it is.
 * If the current signal is the given one, then it will measure it directly.
 *
 * @param pin The digital pin, which will be used to determinate the signal
 * @param mode HIGH or LOW
 * @return The length of the given signal
 */
unsigned long HCSR04::measureSignalLength(const uint8_t& pin, const int& mode) {

    while (digitalRead(pin) != mode);

    unsigned long start = micros();

    while (digitalRead(pin) == mode);

    return micros() - start;
}

/**
 * Will convert the signal length to the actual distance in cm.
 * The signal length represents the time the signal travelled per cm/us.
 * Note that here we assume the sound speed is 340.
 * It is dependent on various factors.
 *
 * @param signalLength The length of the HIGH signal from the HC-SR04 in microseconds
 * @return The distance that the sensor measured in cm
 */
float HCSR04::calculateDistanceBySignalLength(const unsigned int& signalLength) {
    return this->calculateDistanceBySignalLengthAndSoundSpeed(signalLength, 340);
}

/**
 * Will convert the signal length to the actual distance in cm.
 * The signal length represents the time the signal travelled per cm/us.
 *
 * @param signalLength The length of the HIGH signal from the HC-SR04 in microseconds
 * @param soundSpeed The speed of the sound in meters per second
 * @return The distance that the sensor measured in cm
 */
float HCSR04::calculateDistanceBySignalLengthAndSoundSpeed(const unsigned int& signalLength, const float& soundSpeed) {
    float soundSpeedInCentimetersPerMicrosecond = this->convertMetersPerSecondToCentimetersPerMicrosecond(soundSpeed);
    return (soundSpeedInCentimetersPerMicrosecond * static_cast<float>(signalLength)) / 2;
}

/**
 * Will convert meters per second to centimeters per microsecond/
 *
 * @param metersPerSecond The meters per second that will be converted
 * @return The converted meters per second into centimeters per microsecond
 */
float HCSR04::convertMetersPerSecondToCentimetersPerMicrosecond(const float& metersPerSecond) {
    return metersPerSecond * 0.0001f;
}

/**
 * The speed of sound in air is dependent on the temperature & other factors such as air pressure and humidity.
 * The temperature plays more important role that the other two.
 *
 * https://www.engineeringtoolbox.com/air-speed-sound-d_603.html
 *
 * @param temperature The ambient temperature
 * @param temperatureUnit The unit of the temperature
 * @return The speed of sound in m/s
 */
float HCSR04::calculateSoundSpeedByTemperature(const float& temperature, const TemperatureUnit& temperatureUnit) {
    float temperatureInCelsius = this->convertTemperatureUnit(temperature, temperatureUnit, TemperatureUnit::CELSIUS);
    return 331.0f + (0.6f * temperatureInCelsius);
}

float HCSR04::convertDistanceUnit(const float& distance, const DistanceUnit& fromUnit, const DistanceUnit& toUnit) {

    switch (fromUnit) {

        case DistanceUnit::CENTIMETERS:

            switch (toUnit) {

                case DistanceUnit::CENTIMETERS:
                    return distance;

                case DistanceUnit::METERS:
                    return distance / 100.00f;

                case DistanceUnit::INCH:
                    return distance * 0.393700787f;

                case DistanceUnit::FEET:
                    return distance * 0.032808399f;

                case DistanceUnit::YARD:
                    return distance * 0.010936133f;
            }

            break;

        case DistanceUnit::METERS:

            switch (toUnit) {

                case DistanceUnit::CENTIMETERS:
                    return distance * 100.00f;

                case DistanceUnit::METERS:
                    return distance;

                case DistanceUnit::INCH:
                    return distance * 39.3700787f;

                case DistanceUnit::FEET:
                    return distance * 3.2808399f;

                case DistanceUnit::YARD:
                    return distance * 1.0936133f;
            }

            break;

        case DistanceUnit::INCH:

            switch (toUnit) {

                case DistanceUnit::CENTIMETERS:
                    return distance * 2.54f;

                case DistanceUnit::METERS:
                    return distance * 0.0254f;

                case DistanceUnit::INCH:
                    return distance;

                case DistanceUnit::FEET:
                    return distance * 0.0833333333f;

                case DistanceUnit::YARD:
                    return distance * 0.0277777778f;
            }

            break;

        case DistanceUnit::FEET:

            switch (toUnit) {

                case DistanceUnit::CENTIMETERS:
                    return distance * 30.48f;

                case DistanceUnit::METERS:
                    return distance * 0.3048f;

                case DistanceUnit::INCH:
                    return distance * 12.00f;

                case DistanceUnit::FEET:
                    return distance;

                case DistanceUnit::YARD:
                    return distance * 0.333333333f;
            }

            break;

        case DistanceUnit::YARD:

            switch (toUnit) {

                case DistanceUnit::CENTIMETERS:
                    return distance * 91.44f;

                case DistanceUnit::METERS:
                    return distance * 0.9144f;

                case DistanceUnit::INCH:
                    return distance * 36.00f;

                case DistanceUnit::FEET:
                    return distance * 3.00f;

                case DistanceUnit::YARD:
                    return distance;
            }

            break;
    }

    return -1;
}

float HCSR04::convertTemperatureUnit(const float& temperature, const TemperatureUnit& fromUnit, const TemperatureUnit& toUnit) {

    switch (fromUnit) {

        case TemperatureUnit::CELSIUS:

            switch (toUnit) {

                case TemperatureUnit::CELSIUS:
                    return temperature;

                case TemperatureUnit::FAHRENHEIT:
                    return temperature * 1.8f + 32;
            }

            break;

        case TemperatureUnit::FAHRENHEIT:

            switch (toUnit) {

                case TemperatureUnit::CELSIUS:
                    return (temperature - 32) * 0.5556f;

                case TemperatureUnit::FAHRENHEIT:
                    return temperature;
            }

            break;
    }

    return -1;
}

void HCSR04::sendTriggerSignal() {

    digitalWrite(this->triggerPin, HIGH);
    delayMicroseconds(TRIGGER_SIGNAL_LENGTH_US);
    digitalWrite(this->triggerPin, LOW);
}

Measurement HCSR04::measure() {

    this->sendTriggerSignal();

    unsigned long responseSignalLength = measureSignalLength(this->echoPin, HIGH);

    delay(COOL_DOWN_DELAY_MS);

    if (responseSignalLength >= TIMEOUT_SIGNAL_LENGTH_US)
        return Measurement{0, DistanceUnit::CENTIMETERS, true, false};

    return Measurement{this->calculateDistanceBySignalLength(responseSignalLength), DistanceUnit::CENTIMETERS, false, false};
}

Measurement HCSR04::measureWithTemperature(const float& temperature, const TemperatureUnit& temperatureUnit) {

    this->sendTriggerSignal();

    unsigned long responseSignalLength = measureSignalLength(this->echoPin, HIGH);

    delay(COOL_DOWN_DELAY_MS);

    if (responseSignalLength >= TIMEOUT_SIGNAL_LENGTH_US)
        return Measurement{0, DistanceUnit::CENTIMETERS, true, false};

    float soundSpeed = this->calculateSoundSpeedByTemperature(temperature, temperatureUnit);
    float distanceInCM = this->calculateDistanceBySignalLengthAndSoundSpeed(responseSignalLength, soundSpeed);

    return Measurement{distanceInCM, DistanceUnit::CENTIMETERS, false, false};
}


Measurement HCSR04::measureWithSamples(const unsigned int& samples) {

    float total = 0;

    for (int i = 0; i < samples; i++) {
        Measurement measurement = this->measure();

        if (measurement.isTimedOut)
            return Measurement{0, DistanceUnit::CENTIMETERS, true, false};

        total += measurement.distance;
    }

    float averageDistanceInCentimeters = total / static_cast<float>(samples);
    return Measurement{averageDistanceInCentimeters, DistanceUnit::CENTIMETERS, false, false};
}

Measurement HCSR04::measureWithSamplesAndTemperature(const unsigned int& samples, const float& temperature, const TemperatureUnit& temperatureUnit) {

    float total = 0;

    for (int i = 0; i < samples; i++) {
        Measurement measurement = this->measureWithTemperature(temperature, temperatureUnit);

        if (measurement.isTimedOut)
            return Measurement{0, DistanceUnit::CENTIMETERS, true, false};

        total += measurement.distance;
    }

    float averageDistanceInCentimeters = total / static_cast<float>(samples);
    return Measurement{averageDistanceInCentimeters, DistanceUnit::CENTIMETERS, false, false};
}

Measurement HCSR04::measure(const MeasurementConfiguration& configuration) {

    Measurement measurement{};

    bool isMeasureWithTemperature = configuration.getTemperature() && configuration.getTemperatureUnit();
    bool isMeasureWithSamples = configuration.getSamples();

    if (isMeasureWithSamples && isMeasureWithTemperature) {
        measurement = this->measureWithSamplesAndTemperature(*configuration.getSamples(),
                                                             *configuration.getTemperature(),
                                                             *configuration.getTemperatureUnit());

    } else if (isMeasureWithTemperature) {
        measurement = this->measureWithTemperature(*configuration.getTemperature(),
                                                   *configuration.getTemperatureUnit());

    } else if (isMeasureWithSamples) {
        measurement = this->measureWithSamples(*configuration.getSamples());
    }

    if (configuration.getMaxDistance() && configuration.getMaxDistanceUnit() && !measurement.isTimedOut) {
        measurement.isMaxDistanceExceeded = this->convertDistanceUnit(measurement.distance,
                                                                      measurement.distanceUnit,
                                                                      *configuration.getMaxDistanceUnit()) > *configuration.getMaxDistance();
    }

    return measurement;
}

