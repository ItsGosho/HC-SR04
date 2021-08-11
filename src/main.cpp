#include <Arduino.h>

#define SERIAL_BAUD_RATE 9600
#define SR04_TRIGGER_PIN 10
#define SR04_ECHO_PIN 11
#define TEST_BUTTON_PIN 12

bool isButtonPressedAlready = false;

bool isButtonPressed() {

    int isPressed = digitalRead(TEST_BUTTON_PIN);

    if(isPressed == HIGH && !isButtonPressedAlready) {
        isButtonPressedAlready = true;
        return true;
    }

    if(isPressed == HIGH && isButtonPressedAlready) {
        return false;
    }

    if(isPressed == LOW && isButtonPressedAlready) {
        isButtonPressedAlready = false;
        return false;
    }

    return false;
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    pinMode(SR04_TRIGGER_PIN, OUTPUT);
    pinMode(SR04_ECHO_PIN, INPUT);
    pinMode(TEST_BUTTON_PIN, INPUT);
}

void loop() {

    if (isButtonPressed()) {
        Serial.println("Pressed!");
    }
}