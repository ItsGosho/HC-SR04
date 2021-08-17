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

void printMeasurement(Measurement measurement) {
    serial_printf(Serial,
                  "Distance: %2f %s, Signal Timed Out Count: %i, Response Timed Out Count: %i, Max Distance Exceeded Count: %i\n",
                  measurement.getDistance(),
                  measurement.getDistanceUnit() == DistanceUnit::CENTIMETERS ? "cm" : "m",
                  measurement.getSignalTimedOutCount(),
                  measurement.getResponseTimedOutCount(),
                  measurement.getMaxDistanceExceededCount());
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    pinMode(TEST_BUTTON_PIN, INPUT);
}

void loop() {

    if (isButtonPressed()) {

        Measurement measurement1 = hcsr04.measure(
                MeasurementConfiguration::builder()
                .withSamples(5)
                //.withMaxDistance(10,DistanceUnit::CENTIMETERS)
                .build()
                );

        printMeasurement(measurement1);

        Measurement measurement2 = hcsr04.measure(
                MeasurementConfiguration::builder()
                .withSamples(5)
                .withTemperature(1, TemperatureUnit::CELSIUS)
                //.withMaxDistance(10,DistanceUnit::CENTIMETERS)
                .build()
                );

        printMeasurement(measurement2);

        Serial.println("----------------------------------");
    }
}