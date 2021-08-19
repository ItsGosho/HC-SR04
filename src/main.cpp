#include <Arduino.h>
#include <SerialPrintF.h>
#include "HCSR04.h"

#define SERIAL_BAUD_RATE 9600
#define HCSR04_ONE_WIRE_PIN 9

HCSR04 hcsr04(HCSR04_ONE_WIRE_PIN);

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
}

void loop() {


    unsigned long start = millis();
    Measurement measurement = hcsr04.measure(
            MeasurementConfiguration::builder()
                        .withSamples(5)
                        .withTemperature(35,TemperatureUnit::CELSIUS)
                        .withMaxDistance(20,DistanceUnit::CENTIMETERS)
                        .withMeasurementDistanceUnit(DistanceUnit::CENTIMETERS)
                        .withResponseTimeoutMS(300)
                        .build());

    Serial.println(millis() - start);

    serial_printf(Serial,
                  "Distance: %2f %s, Valid Samples: %l/%i [Signal Timed Out Count: %i, Response Timed Out Count: %i, Max Distance Exceeded Count: %i]\n",
                  measurement.getDistance(), getDistanceUnitAbbreviation(measurement.getDistanceUnit()),
                  measurement.getValidMeasurementsCount(),
                  measurement.getTakenSamples(),
                  measurement.getSignalTimedOutCount(),
                  measurement.getResponseTimedOutCount(),
                  measurement.getMaxDistanceExceededCount());
}