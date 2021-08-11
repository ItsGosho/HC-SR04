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

float HCSR04::measure() {

    digitalWrite(this->triggerPin, HIGH);
    delayMicroseconds(TRIGGER_SIGNAL_LENGTH_US);
    digitalWrite(this->triggerPin, LOW);

    unsigned long distanceTimeMeasured = measureSignalLength(this->echoPin, HIGH);

    if(distanceTimeMeasured >= TIMEOUT_SIGNAL_LENGTH_US) {
        //serial_printf(Serial, "Timed out! %l us.\n", distanceTimeMeasured);
        return 0.0;
    } else {
        float distanceCM = (0.034f * distanceTimeMeasured) / 2;
        //serial_printf(Serial, "Distance: %2f cm. %2f m.\n", distanceCM, distanceCM / 100);
        delay(COOL_DOWN_DELAY_MS);
        return distanceCM;
    }
}
