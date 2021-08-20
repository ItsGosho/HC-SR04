#include "HCSR04.h"

HCSR04::HCSR04(const uint8_t& oneWirePin) : oneWirePin(oneWirePin) {

    this->isOneWireMode = true;
    this->initializeDefaults();
}

HCSR04::HCSR04(const uint8_t& triggerPin, const uint8_t& echoPin) : triggerPin(triggerPin), echoPin(echoPin) {

    pinMode(this->triggerPin, OUTPUT);
    pinMode(this->echoPin, INPUT);

    this->isOneWireMode = false;
    this->initializeDefaults();
}

/**
 * Will set the default values of the properties.
 */
void HCSR04::initializeDefaults() {
    this->defaultSamples = DEFAULT_SAMPLES;
    this->defaultMaxDistanceValue = DEFAULT_MAX_DISTANCE_CENTIMETERS;
    this->defaultMaxDistanceUnit = DistanceUnit::CENTIMETERS;
    this->defaultTemperatureValue = DEFAULT_TEMPERATURE_CELSIUS;
    this->defaultTemperatureUnit = TemperatureUnit::CELSIUS;
    this->defaultResponseTimeoutMS = DEFAULT_RESPONSE_TIMEOUT_MS;
    this->defaultMeasurementDistanceUnit = DistanceUnit::CENTIMETERS;
    this->defaultResponseTimeoutCoolDownTimeMS = DEFAULT_RESPONSE_COOL_DOWN_MS;
    this->responseCoolDownEndMS = DEFAULT_RESPONSE_COOL_DOWN_MS;
}

/**
 * Will calculate the signal length as actual distance in the given unit
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
 * Will calculate the signal length as actual distance in the given unit
 * The signal length represents the time the signal travelled per cm/us.
 *
 * @param hcsr04Response The response, which signal length will be used for the distance's calculation
 * @param measurementConfiguration The configuration, which will determinate the temperature and measurement distance unit
 * @return The calculated distance
 */
float HCSR04::calculateDistance(const HCSR04Response& hcsr04Response, const MeasurementConfiguration& measurementConfiguration) {

    float temperatureValue = measurementConfiguration.getTemperatureValue().orElseGet(this->defaultTemperatureValue);
    TemperatureUnit temperatureUnit = measurementConfiguration.getTemperatureUnit().orElseGet(this->defaultTemperatureUnit);
    DistanceUnit measurementDistanceUnit = measurementConfiguration.getMeasurementDistanceUnit().orElseGet(this->defaultMeasurementDistanceUnit);;

    float soundSpeedMetersPerSecond = this->calculateSoundSpeedByTemperature(temperatureValue, temperatureUnit);

    return this->calculateDistanceBySignalLengthAndSoundSpeed(hcsr04Response.getHighSignalLengthUS(), soundSpeedMetersPerSecond, measurementDistanceUnit);
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

/**
 * Will send the trigger signal to the HCSR04. It consists of holding a high signal for specific period.
 * This method works in both one wire and two wire (trigger/echo) mode
 */
void HCSR04::sendTriggerSignalToHCSR04() {

    uint8_t pin = this->isOneWireMode ? this->oneWirePin : this->triggerPin;

    pinMode(pin, OUTPUT);

    digitalWrite(pin, HIGH);
    delayMicroseconds(TRIGGER_SIGNAL_LENGTH_US);
    digitalWrite(pin, LOW);
}

/**
 * Will send a request for measurement to the HCSR04 and wait for its response.
 * If the response doesn't arrive in the given timeout time, then it will be time outed
 *
 * @param responseTimeOutMS The maximum time that the response has to arrive
 * @return The results from the measurement.
 */
HCSR04Response HCSR04::sendAndReceivedToHCSR04(const MeasurementConfiguration& measurementConfiguration) {

    unsigned long responseTimeOutMS = measurementConfiguration.getResponseTimeoutMS().orElseGet(this->defaultResponseTimeoutMS);

    uint8_t measurementPin = this->isOneWireMode ? this->oneWirePin : this->echoPin;

    this->sendTriggerSignalToHCSR04();

    SignalLengthMeasurementUS signalLengthMeasurementUs = measureSignalLength(measurementPin, HIGH, responseTimeOutMS);

    delay(COOL_DOWN_DELAY_MS);
    return {signalLengthMeasurementUs.signalLengthUS, signalLengthMeasurementUs.isTimedOut};
}

/**
 * Will send a multiple requests for measurement to the HCSR04 and wait for their responses.
 * The measurement configuration defines how they will be collected.
 *
 * @param hcsr04Responses The array, which will be filled with the responses
 * @param measurementConfiguration Defines how the measurements will be collected
 */
void HCSR04::sendAndReceivedToHCSR04(HCSR04Response hcsr04Responses[], const MeasurementConfiguration& measurementConfiguration) {

    unsigned int samples = measurementConfiguration.getSamples().orElseGet(this->defaultSamples);

    for (int i = 0; i < samples; i++)
        hcsr04Responses[i] = this->sendAndReceivedToHCSR04(measurementConfiguration);
}

/**
 * Will check if a response is valid based on its timeouts and the distance that has been measured.
 *
 * @param measurementConfiguration Will be used to determinate if the response was valid
 * @param hcsr04Response The response that will be check if valid
 * @return If the given response was valid
 */
bool HCSR04::isResponseValid(const MeasurementConfiguration& measurementConfiguration, const HCSR04Response& hcsr04Response) {

    float maxDistanceValue = measurementConfiguration.getMaxDistanceValue().orElseGet(this->defaultMaxDistanceValue);
    DistanceUnit maxDistanceUnit = measurementConfiguration.getMaxDistanceUnit().orElseGet(this->defaultMaxDistanceUnit);
    DistanceUnit measurementDistanceUnit = measurementConfiguration.getMeasurementDistanceUnit().orElseGet(this->defaultMeasurementDistanceUnit);;

    float distance = this->calculateDistance(hcsr04Response, measurementConfiguration);

    bool isMaxDistanceExceeded = convertDistanceUnit(distance, measurementDistanceUnit, maxDistanceUnit) > maxDistanceValue;

    return !hcsr04Response.isSignalTimedOut(TIMEOUT_SIGNAL_LENGTH_US) && !hcsr04Response.isResponseTimedOut() && !isMaxDistanceExceeded;
}

/**
 * Checks if the max distance for the given response has been exceeded.
 *
 * @param hcsr04Response The response, which distance will be checked
 * @param measurementConfiguration The configuration, which parameters will determinate if the response's distance was valid
 * @return If the distance was valid
 */
bool HCSR04::isMaxDistanceExceeded(const HCSR04Response& hcsr04Response, const MeasurementConfiguration& measurementConfiguration) {

    float maxDistanceValue = measurementConfiguration.getMaxDistanceValue().orElseGet(this->defaultMaxDistanceValue);
    DistanceUnit maxDistanceUnit = measurementConfiguration.getMaxDistanceUnit().orElseGet(this->defaultMaxDistanceUnit);
    DistanceUnit measurementDistanceUnit = measurementConfiguration.getMeasurementDistanceUnit().orElseGet(this->defaultMeasurementDistanceUnit);;

    float distance = this->calculateDistance(hcsr04Response, measurementConfiguration);

    return convertDistanceUnit(distance, measurementDistanceUnit, maxDistanceUnit) > maxDistanceValue;;
}

/**
 * Counts the errors of the responses that have a error.
 * There is priority that determinants in which category the error will go.
 * 1. Response Timed Out
 * 2. Signal Timed Out
 * 3. Max Distance Exceeded
 *
 */
HCSR04ResponseErrors HCSR04::countResponseErrors(HCSR04Response hcsr04Responses[], const unsigned int& responsesCount, const MeasurementConfiguration& measurementConfiguration) {

    unsigned int signalTimedOutCount = 0;
    unsigned int responseTimedOutCount = 0;
    unsigned int maxDistanceExceededCount = 0;

    for (int i = 0; i < responsesCount; ++i) {
        HCSR04Response hcsr04Response = hcsr04Responses[i];

        bool isMaxDistanceExceeded = this->isMaxDistanceExceeded(hcsr04Response, measurementConfiguration);

        if (hcsr04Response.isResponseTimedOut())
            responseTimedOutCount++;
        else if (hcsr04Response.isSignalTimedOut(TIMEOUT_SIGNAL_LENGTH_US))
            signalTimedOutCount++;
        else if (isMaxDistanceExceeded)
            maxDistanceExceededCount++;
    }

    return {signalTimedOutCount, responseTimedOutCount, maxDistanceExceededCount};
}

/**
 * Calculates the average sum of the distances.
 */
float HCSR04::calculateAverage(HCSR04Response hcsr04Responses[], const unsigned int& responsesCount, const MeasurementConfiguration& measurementConfiguration) {

    float distancesSum = 0;
    unsigned int validSamples = 0;

    for (int i = 0; i < responsesCount; i++) {
        HCSR04Response hcsr04Response = hcsr04Responses[i];

        float distance = this->calculateDistance(hcsr04Response, measurementConfiguration);

        if (this->isResponseValid(measurementConfiguration, hcsr04Response)) {
            distancesSum += distance;
            validSamples++;
        }
    }

    return distancesSum / (validSamples == 0 ? 1 : static_cast<float>(validSamples));
}

bool HCSR04::isResponseCoolDownRequired(HCSR04Response hcsr04Responses[], const unsigned int& responsesCount, const MeasurementConfiguration& measurementConfiguration) {

    if (!measurementConfiguration.getResponseTimeoutCoolDownTimeMS().has())
        return false;

    unsigned int timedOutResponsesCount = 0;

    for (int i = 0; i < responsesCount; i++) {
        HCSR04Response hcsr04Response = hcsr04Responses[i];
        timedOutResponsesCount += hcsr04Response.isResponseTimedOut() ? 1 : 0;
    }

    return timedOutResponsesCount == responsesCount;
}

void HCSR04::applyResponseCoolDown(const MeasurementConfiguration& measurementConfiguration) {
    unsigned int responseTimeoutCoolDownTimeMS = measurementConfiguration.getResponseTimeoutCoolDownTimeMS().orElseGet(this->defaultResponseTimeoutCoolDownTimeMS);

    this->responseCoolDownEndMS = millis() + responseTimeoutCoolDownTimeMS;
}

bool HCSR04::isResponseCoolDownActive() {

    if(this->responseCoolDownEndMS == 0)
        return false;

    return millis() <= this->responseCoolDownEndMS;
}

/**
 * Will do a measurement with the default values.
 * They are these in the macros in the header file.
 */
Measurement HCSR04::measure() {
    return this->measure(MeasurementConfiguration::builder().build());
}

/**
 * Will do a measurement/s based on the provided configuration.
 */
Measurement HCSR04::measure(const MeasurementConfiguration& measurementConfiguration) {

    if (this->isResponseCoolDownActive())
        return Measurement{0, DistanceUnit::CENTIMETERS, 0, 0, 0, 0, true};

    unsigned int samples = measurementConfiguration.getSamples().orElseGet(this->defaultSamples);
    DistanceUnit measurementDistanceUnit = measurementConfiguration.getMeasurementDistanceUnit().orElseGet(this->defaultMeasurementDistanceUnit);;

    HCSR04Response hcsr04Responses[samples];

    this->sendAndReceivedToHCSR04(hcsr04Responses, measurementConfiguration);

    if (this->isResponseCoolDownRequired(hcsr04Responses, samples, measurementConfiguration))
        this->applyResponseCoolDown(measurementConfiguration);

    HCSR04ResponseErrors hcsr04ResponseErrors = this->countResponseErrors(hcsr04Responses, samples, measurementConfiguration);
    float averageDistance = this->calculateAverage(hcsr04Responses, samples, measurementConfiguration);

    return Measurement{averageDistance, measurementDistanceUnit, samples, hcsr04ResponseErrors.signalTimedOutCount, hcsr04ResponseErrors.responseTimedOutCount, hcsr04ResponseErrors.maxDistanceExceededCount, false};
}

/**
 * How many times to take measurement. Then the returned distance will be the average of a valid measurements.
 */
void HCSR04::setDefaultSamples(const unsigned int& defaultSamples) {
    HCSR04::defaultSamples = defaultSamples;
}

/**
 * The maximum distance that is allowed. If a measurement's distance exceeds it, then the measurement is invalid.
 */
void HCSR04::setDefaultMaxDistance(const float& defaultMaxDistanceValue, const DistanceUnit& defaultMaxDistanceUnit) {
    HCSR04::defaultMaxDistanceValue = defaultMaxDistanceValue;
    HCSR04::defaultMaxDistanceUnit = defaultMaxDistanceUnit;
}

/**
 *  The ambient temperature.
  * Increases the accuracy of the measurement, because the sound speed is dependent on temperature.
 */
void HCSR04::setDefaultTemperature(const float& defaultTemperatureValue, const TemperatureUnit& defaultTemperatureUnit) {
    HCSR04::defaultTemperatureValue = defaultTemperatureValue;
    HCSR04::defaultTemperatureUnit = defaultTemperatureUnit;
}

/**
 * The maximum time to take a measurement.
 * If the time is exceeded then the measurement is invalid.
 * Indication when there is something wrong with the communication to the device eg: not connected
 */
void HCSR04::setDefaultResponseTimeoutMS(const unsigned long& defaultResponseTimeoutMS) {
    HCSR04::defaultResponseTimeoutMS = defaultResponseTimeoutMS;
}

/**
 * In what distance unit the measurement will be returned.
 */
void HCSR04::setDefaultMeasurementDistanceUnit(const DistanceUnit& defaultMeasurementDistanceUnit) {
    HCSR04::defaultMeasurementDistanceUnit = defaultMeasurementDistanceUnit;
}

/**
 * Let's say that you have a measurement in your program with **5** samples.
 * If the **HCSR04** is not connected, then there will be no samples, but you will lose time for example **~315 milliseconds** on **each loop**.
 * To avoid that and you can try measuring after specific time again. That is where the cooldowns come.
 * The cool down will be **activated**, when **all of the samples** that have been measured have **timed out**. If a **measurement fails again**, then it will be **activated again** and so on.
 */
void HCSR04::setDefaultResponseTimeoutCoolDownMS(const unsigned long& defaultResponseTimeoutCoolDownTimeMS) {
    HCSR04::defaultResponseTimeoutCoolDownTimeMS = defaultResponseTimeoutCoolDownTimeMS;
}
