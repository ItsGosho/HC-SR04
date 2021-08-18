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

const char* getDistanceUnitString(const DistanceUnit& distanceUnit) {

    switch (distanceUnit) {

        case DistanceUnit::CENTIMETERS:
            return "centimeters";

        case DistanceUnit::METERS:
            return "meters";

        case DistanceUnit::INCH:
            return "inches";

        case DistanceUnit::FEET:
            return "feets";

        case DistanceUnit::YARD:
            return "yards";

        default:
            return "NOT IMPLEMENTED!";
    }
}

void printMeasurement(Measurement measurement) {
    serial_printf(Serial,
                  "Distance: %2f %s, Valid Samples: %l/%i [Signal Timed Out Count: %i, Response Timed Out Count: %i, Max Distance Exceeded Count: %i]\n",
                  measurement.getDistance(),
                  getDistanceUnitString(measurement.getDistanceUnit()),
                  measurement.getValidMeasurementsCount(),
                  measurement.getTakenSamples(),
                  measurement.getSignalTimedOutCount(),
                  measurement.getResponseTimedOutCount(),
                  measurement.getMaxDistanceExceededCount());
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    pinMode(TEST_BUTTON_PIN, INPUT);

    hcsr04.setDefaultSamples(5);
}

void loop() {

    if (isButtonPressed()) {

        Measurement measurement = hcsr04.measure(MeasurementConfiguration::builder()
                .withSamples(3)
                //.withTemperature(1,TemperatureUnit::CELSIUS)
                //.withMaxDistance(4,DistanceUnit::CENTIMETERS)
                //.withMeasurementDistanceUnit(DistanceUnit::FEET)
                .build());

        printMeasurement(measurement);

        Serial.println("----------------------------------");
    }
}