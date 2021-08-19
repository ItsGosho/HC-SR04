#include <Arduino.h>
#include <SerialPrintF.h>
#include "hcsr04/HCSR04.h"

#define SERIAL_BAUD_RATE 9600
#define HCSR04_ONE_WIRE_PIN 9

HCSR04 hcsr04(HCSR04_ONE_WIRE_PIN);

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    hcsr04.setDefaultSamples(5);
    hcsr04.setDefaultTemperature(21.55, TemperatureUnit::CELSIUS);
    hcsr04.setDefaultMaxDistance(1, DistanceUnit::METERS);
    hcsr04.setDefaultMeasurementDistanceUnit(DistanceUnit::CENTIMETERS);
    hcsr04.setDefaultResponseTimeoutMS(300);
}

void loop() {


    Measurement measurement = hcsr04.measure();

    serial_printf(Serial,
                  "Distance: %2f %s, Valid Samples: %l/%i [Signal Timed Out Count: %i, Response Timed Out Count: %i, Max Distance Exceeded Count: %i]\n",
                  measurement.getDistance(), getDistanceUnitAbbreviation(measurement.getDistanceUnit()),
                  measurement.getValidMeasurementsCount(),
                  measurement.getTakenSamples(),
                  measurement.getSignalTimedOutCount(),
                  measurement.getResponseTimedOutCount(),
                  measurement.getMaxDistanceExceededCount());
}