#include <Arduino.h>

#define SERIAL_BAUD_RATE 9600
#define SR04_TRIGGER_PIN 10
#define SR04_ECHO_PIN 11
#define TEST_BUTTON_PIN 12

bool isButtonPressedAlready = false;

bool isButtonPressed() {

    int isPressed = digitalRead(TEST_BUTTON_PIN);

    if (isPressed == HIGH && !isButtonPressedAlready) {
        isButtonPressedAlready = true;
        return true;
    }

    if (isPressed == HIGH && isButtonPressedAlready) {
        return false;
    }

    if (isPressed == LOW && isButtonPressedAlready) {
        isButtonPressedAlready = false;
        return false;
    }

    return false;
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
unsigned long measureSignalLength(const uint8_t& pin, const int& mode) {

    while (digitalRead(pin) != mode);

    unsigned long start = micros();

    while (digitalRead(pin) == mode);

    return micros() - start;
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    pinMode(SR04_TRIGGER_PIN, OUTPUT);
    pinMode(SR04_ECHO_PIN, INPUT);
    pinMode(TEST_BUTTON_PIN, INPUT);
}

void loop() {

    if (isButtonPressed()) {
        digitalWrite(SR04_TRIGGER_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(SR04_TRIGGER_PIN, LOW);

        unsigned long distanceTimeMeasured = measureSignalLength(SR04_ECHO_PIN, HIGH);
        Serial.println(distanceTimeMeasured);
        delay(60);
        Serial.println("Cool down period passed! You can measure again!");
    }
}