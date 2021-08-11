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

unsigned long measureSignalLength(String message, int signalType) {

    unsigned long start = micros();
    while (digitalRead(SR04_ECHO_PIN) == signalType);
    unsigned long timeUS = micros() - start;

    Serial.println(message);
    Serial.println(timeUS);
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

        measureSignalLength("8 burst pulses sending time!", LOW);
        measureSignalLength("Distance time", HIGH);
        delay(60);
        Serial.println("Cool down period passed! You can measure again!");
    }
}