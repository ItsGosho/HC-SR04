#include <Arduino.h>
#include <SerialPrintF.h>
#include "HCSR04.h"

#define SERIAL_BAUD_RATE 9600
#define HCSR04_TRIGGER_PIN 10
#define HCSR04_ECHO_PIN 11
#define TEST_BUTTON_PIN 12

HCSR04 hcsr04(HCSR04_TRIGGER_PIN, HCSR04_ECHO_PIN);

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

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    pinMode(TEST_BUTTON_PIN, INPUT);
}

void loop() {

    if (isButtonPressed()) {
        Measurement measurement = hcsr04.measure();

        if (measurement.isTimedOut) {
            Serial.println("Measurement timed out!");
        } else {
            serial_printf(Serial, "Distance: %2f cm. %2f m.\n", measurement.distance, measurement.distance / 100);
        }

    }
}