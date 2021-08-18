#include "HCSR04.h"

HCSR04::HCSR04(const uint8_t& oneWirePin) : oneWirePin(oneWirePin) {

    this->isOneWireMode = true;

    this->defaultSamples = DEFAULT_SAMPLES;
    this->defaultMaxDistanceValue = DEFAULT_MAX_DISTANCE_CENTIMETERS;
    this->defaultMaxDistanceUnit = DistanceUnit::CENTIMETERS;
    this->defaultTemperatureValue = DEFAULT_TEMPERATURE_CELSIUS;
    this->defaultTemperatureUnit = TemperatureUnit::CELSIUS;
    this->defaultResponseTimeoutUS = DEFAULT_RESPONSE_TIMEOUT_US;
    this->defaultMeasurementDistanceUnit = DistanceUnit::CENTIMETERS;
}

HCSR04::HCSR04(const uint8_t& triggerPin, const uint8_t& echoPin) : triggerPin(triggerPin), echoPin(echoPin) {

    pinMode(this->triggerPin, OUTPUT);
    pinMode(this->echoPin, INPUT);

    this->isOneWireMode = false;

    this->defaultSamples = DEFAULT_SAMPLES;
    this->defaultMaxDistanceValue = DEFAULT_MAX_DISTANCE_CENTIMETERS;
    this->defaultMaxDistanceUnit = DistanceUnit::CENTIMETERS;
    this->defaultTemperatureValue = DEFAULT_TEMPERATURE_CELSIUS;
    this->defaultTemperatureUnit = TemperatureUnit::CELSIUS;
    this->defaultResponseTimeoutUS = DEFAULT_RESPONSE_TIMEOUT_US;
    this->defaultMeasurementDistanceUnit = DistanceUnit::CENTIMETERS;
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
    float distanceInCM = (soundSpeedInCentimetersPerMicrosecond * static_cast<float>(signalLength)) / 2;

    return distanceInCM;
}

/**
 * Will convert the signal length to the actual distance in the given unit
 * The signal length represents the time the signal travelled per cm/us.
 *
 * @param signalLength The length of the HIGH signal from the HC-SR04 in microseconds
 * @param soundSpeed The speed of the sound in meters per second
 * @param distanceUnit In what distance unit the distance to be returned
 * @return The distance that the sensor measured converted in the given one
 */
float HCSR04::calculateDistanceBySignalLengthAndSoundSpeed(const unsigned int& signalLength, const float& soundSpeed, const DistanceUnit& distanceUnit) {

    float soundSpeedInCentimetersPerMicrosecond = this->convertMetersPerSecondToCentimetersPerMicrosecond(soundSpeed);
    float distanceInCM = (soundSpeedInCentimetersPerMicrosecond * static_cast<float>(signalLength)) / 2;

    return convertDistanceUnit(distanceInCM, DistanceUnit::CENTIMETERS, distanceUnit);
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

    uint8_t pin = this->isOneWireMode ? this->oneWirePin : this->triggerPin;

    pinMode(pin, OUTPUT);

    digitalWrite(pin, HIGH);
    delayMicroseconds(TRIGGER_SIGNAL_LENGTH_US);
    digitalWrite(pin, LOW);
}

HCSR04Response HCSR04::sendAndReceivedToHCSR04() {
    /*TODO: From the macro or variable?!*/
    return this->sendAndReceivedToHCSR04(DEFAULT_RESPONSE_TIMEOUT_US);
}

HCSR04Response HCSR04::sendAndReceivedToHCSR04(const unsigned long& responseTimeOutUS) {

    this->sendTriggerSignalToHCSR04();

    uint8_t measurementPin = this->isOneWireMode ? this->oneWirePin : this->echoPin;
    SignalLengthMeasurementUS signalLengthMeasurementUs = measureSignalLength(measurementPin, HIGH, responseTimeOutUS);

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

    unsigned long responseTimeOutUS = measurementConfiguration.getResponseTimeoutUS().orElseGet(this->defaultResponseTimeoutUS);
    unsigned int samples = measurementConfiguration.getSamples().orElseGet(this->defaultSamples);
    float temperatureValue = measurementConfiguration.getTemperatureValue().orElseGet(this->defaultTemperatureValue);
    TemperatureUnit temperatureUnit = measurementConfiguration.getTemperatureUnit().orElseGet(this->defaultTemperatureUnit);
    float maxDistanceValue = measurementConfiguration.getMaxDistanceValue().orElseGet(this->defaultMaxDistanceValue);
    DistanceUnit maxDistanceUnit = measurementConfiguration.getMaxDistanceUnit().orElseGet(this->defaultMaxDistanceUnit);
    DistanceUnit measurementDistanceUnit = measurementConfiguration.getMeasurementDistanceUnit().orElseGet(this->defaultMeasurementDistanceUnit);

    float distancesSum = 0;
    unsigned int validSamples = 0;
    unsigned int signalTimedOutCount = 0;
    unsigned int responseTimedOutCount = 0;
    unsigned int maxDistanceExceededCount = 0;

    HCSR04Response hcsr04Responses[samples];

    this->sendAndReceivedToHCSR04(hcsr04Responses, samples, responseTimeOutUS);

    for (int i = 0; i < samples; i++) {
        HCSR04Response hcsr04Response = hcsr04Responses[i];

        float soundSpeedMetersPerSecond = this->calculateSoundSpeedByTemperature(temperatureValue, temperatureUnit);

        float distance = this->calculateDistanceBySignalLengthAndSoundSpeed(hcsr04Response.getHighSignalLengthUS(),
                                                                            soundSpeedMetersPerSecond,
                                                                            measurementDistanceUnit);

        bool isMaxDistanceExceeded = convertDistanceUnit(distance,
                                                         measurementDistanceUnit,
                                                         maxDistanceUnit) > maxDistanceValue;

        if (!hcsr04Response.isSignalTimedOut() && !hcsr04Response.isResponseTimedOut() && !isMaxDistanceExceeded) {
            distancesSum += distance;
            validSamples++;
        }

        //Note that there is priority
        if (hcsr04Response.isResponseTimedOut())
            responseTimedOutCount++;
        else if (hcsr04Response.isSignalTimedOut())
            signalTimedOutCount++;
        else if (isMaxDistanceExceeded)
            maxDistanceExceededCount++;
    }

    float distanceInCMAverage = distancesSum / (validSamples == 0 ? 1 : static_cast<float>(validSamples));

    return Measurement{distanceInCMAverage, measurementDistanceUnit, samples, signalTimedOutCount, responseTimedOutCount, maxDistanceExceededCount};;
}

void HCSR04::setDefaultSamples(const unsigned int& defaultSamples) {
    HCSR04::defaultSamples = defaultSamples;
}

void HCSR04::setDefaultMaxDistanceValue(const float& defaultMaxDistanceValue) {
    HCSR04::defaultMaxDistanceValue = defaultMaxDistanceValue;
}

void HCSR04::setDefaultMaxDistanceUnit(const DistanceUnit& defaultMaxDistanceUnit) {
    HCSR04::defaultMaxDistanceUnit = defaultMaxDistanceUnit;
}

void HCSR04::setDefaultTemperatureValue(const float& defaultTemperatureValue) {
    HCSR04::defaultTemperatureValue = defaultTemperatureValue;
}

void HCSR04::setDefaultTemperatureUnit(const TemperatureUnit& defaultTemperatureUnit) {
    HCSR04::defaultTemperatureUnit = defaultTemperatureUnit;
}

void HCSR04::setDefaultResponseTimeoutUs(const unsigned long& defaultResponseTimeoutUs) {
    defaultResponseTimeoutUS = defaultResponseTimeoutUs;
}

void HCSR04::setDefaultMeasurementDistanceUnit(const DistanceUnit& defaultMeasurementDistanceUnit) {
    HCSR04::defaultMeasurementDistanceUnit = defaultMeasurementDistanceUnit;
}
