#include <Arduino.h>
#include <SerialPrintF.h>
#include "HCSR04.h"

#define SERIAL_BAUD_RATE 9600
#define HCSR04_ONE_WIRE_PIN 9
#define HCSR04_TRIGGER_PIN 10
#define HCSR04_ECHO_PIN 11

HCSR04 hcsr04(HCSR04_ONE_WIRE_PIN);

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
}

void loop() {

    Measurement measurement = hcsr04.measure(MeasurementConfiguration::builder()
            .withSamples(3)
            //.withTemperature(1,TemperatureUnit::CELSIUS)
            //.withMaxDistance(4,DistanceUnit::CENTIMETERS)
            //.withMeasurementDistanceUnit(DistanceUnit::FOOT)
                                                     .build());

    serial_printf(Serial,
                  "Distance: %2f %s, Valid Samples: %l/%i [Signal Timed Out Count: %i, Response Timed Out Count: %i, Max Distance Exceeded Count: %i]\n",
                  measurement.getDistance(), getDistanceUnitAbbreviation(measurement.getDistanceUnit()),
                  measurement.getValidMeasurementsCount(),
                  measurement.getTakenSamples(),
                  measurement.getSignalTimedOutCount(),
                  measurement.getResponseTimedOutCount(),
                  measurement.getMaxDistanceExceededCount());

    Serial.println("----------------------------------");
}