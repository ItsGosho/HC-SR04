#include "HCSR04.h"

HCSR04::HCSR04(const uint8_t& triggerPin, const uint8_t& echoPin) : triggerPin(triggerPin), echoPin(echoPin) {
    pinMode(this->triggerPin, OUTPUT);
    pinMode(this->echoPin, INPUT);
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
    float temperatureInCelsius = convertTemperatureUnit(temperature, temperatureUnit, TemperatureUnit::CELSIUS);
    return 331.0f + (0.6f * temperatureInCelsius);
}

void HCSR04::sendTriggerSignalToHCSR04() {

    digitalWrite(this->triggerPin, HIGH);
    delayMicroseconds(TRIGGER_SIGNAL_LENGTH_US);
    digitalWrite(this->triggerPin, LOW);
}

HCSR04Response HCSR04::sendAndReceivedToHCSR04() {

    this->sendTriggerSignalToHCSR04();
    unsigned long responseSignalLength = measureSignalLength(this->echoPin, HIGH);

    delay(COOL_DOWN_DELAY_MS);
    return {responseSignalLength, false};
}

/*TODO: Dont forget if comparing the signal length that it is in US!*/
HCSR04Response HCSR04::sendAndReceivedToHCSR04(const unsigned long& responseTimeOutUS) {

    this->sendTriggerSignalToHCSR04();
    SignalLengthMeasurementUS signalLengthMeasurementUs = measureSignalLength(this->echoPin, HIGH, responseTimeOutUS);

    delay(COOL_DOWN_DELAY_MS);
    return {signalLengthMeasurementUs.signalLengthUS, signalLengthMeasurementUs.isTimedOut};
}


void HCSR04::sendAndReceivedToHCSR04(HCSR04Response hcsr04Responses[], const unsigned int& times) {

    for (int i = 0; i < times; i++)
        hcsr04Responses[i] = this->sendAndReceivedToHCSR04();
}


void HCSR04::sendAndReceivedToHCSR04(HCSR04Response hcsr04Responses[], const unsigned int& times, const unsigned long& responseTimeOutUS) {

    for (int i = 0; i < times; i++)
        hcsr04Responses[i] = this->sendAndReceivedToHCSR04(responseTimeOutUS);
}

Measurement HCSR04::measure(const MeasurementConfiguration& measurementConfiguration) {

    /*TODO: Optional.of / else / from*/
    /*TODO: Default values as macros*/
    /*TODO: Option to set them directly and the priority koeto si pisah na tetradkata*/
    /*TODO: Added the response timeout us in the configuration*/
    unsigned long responseTimeOutUS = measurementConfiguration.getResponseTimeoutUS() ? *measurementConfiguration.getResponseTimeoutUS() : 1000000;
    unsigned int samples = measurementConfiguration.getSamples() ? *measurementConfiguration.getSamples() : 1;
    float temperature = measurementConfiguration.getTemperature() ? *measurementConfiguration.getTemperature() : 25.00f;
    TemperatureUnit temperatureUnit = measurementConfiguration.getTemperatureUnit() ? *measurementConfiguration.getTemperatureUnit() : TemperatureUnit::CELSIUS;
    float maxDistance = measurementConfiguration.getMaxDistance() ? *measurementConfiguration.getMaxDistance() : 4.00f;
    DistanceUnit maxDistanceUnit = measurementConfiguration.getMaxDistanceUnit() ? *measurementConfiguration.getMaxDistanceUnit() : DistanceUnit::METERS;

    Measurement measurements[samples];
    HCSR04Response hcsr04Responses[samples];

    this->sendAndReceivedToHCSR04(hcsr04Responses, samples, responseTimeOutUS);

    for (int i = 0; i < samples; i++) {
        HCSR04Response hcsr04Response = hcsr04Responses[i];

        float soundSpeed = this->calculateSoundSpeedByTemperature(temperature, temperatureUnit);
        float distanceInCM = this->calculateDistanceBySignalLengthAndSoundSpeed(hcsr04Response.getHighSignalLengthUS(),
                                                                                soundSpeed);
        bool isMaxDistanceExceeded = convertDistanceUnit(distanceInCM,
                                                         DistanceUnit::CENTIMETERS,
                                                         maxDistanceUnit) > maxDistance;

        measurements[i] = Measurement{distanceInCM, DistanceUnit::CENTIMETERS, hcsr04Response.isSignalTimedOut(), hcsr04Response.isResponseTimedOut(), isMaxDistanceExceeded};
    }

    Measurement aggregatedMeasurement{};
    float distancesSum = 0;

    for (int i = 0; i < samples; i++) {
        Measurement measurement = measurements[i];

        if (measurement.isMaxDistanceExceeded)
            aggregatedMeasurement.isMaxDistanceExceeded = true;

        if (measurement.isResponseTimedOut)
            aggregatedMeasurement.isResponseTimedOut = true;

        if (measurement.isSignalTimedOut)
            aggregatedMeasurement.isSignalTimedOut = true;

        distancesSum += measurement.distance;
    }

    aggregatedMeasurement.distance = distancesSum / samples;

    return aggregatedMeasurement;
}
