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
 * @param temperature The ambient temperature in celsius
 * @return The speed of sound in m/s
 */
float HCSR04::calculateSoundSpeed(const float& temperature) {
    return 331.0f + (0.6f * temperature);
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

    return Measurement{this->calculateDistanceBySignalLength(responseSignalLength), DistanceUnit::CENTIMETERS, false, false}
}

Measurement HCSR04::measure(const unsigned int& samples) {

    float total = 0;

    for (int i = 0; i < samples; i++) {
        Measurement measurement = this->measure();

        if (measurement.isTimedOut)
            return Measurement{0, DistanceUnit::CENTIMETERS, true, false};

        total += measurement.distance;
    }

    float averageDistanceInCentimeters = total / static_cast<float>(samples);
    return Measurement{averageDistanceInCentimeters, DistanceUnit::CENTIMETERS, false, false}
}

Measurement HCSR04::measure(const MeasurementConfiguration& configuration) {

    Measurement measurement{};

    if (configuration.getSamples()) {
        measurement = this->measure(*configuration.getSamples());
    }

    if (configuration.getMaxDistance() && configuration.getMaxDistanceUnit()) {
        //measurement.isMaxDistanceExceeded = measurement.distance
    }

    if (configuration.getTimeoutMS()) {

    }

    if (configuration.getTemperature() && configuration.getTemperatureUnit()) {

    }

    return measurement;
}
