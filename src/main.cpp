#include <Arduino.h>
#include <SerialPrintF.h>
#include "hcsr04/HCSR04.h"

#define SERIAL_BAUD_RATE 9600
#define HCSR04_ONE_WIRE_PIN 9

HCSR04 hcsr04(HCSR04_ONE_WIRE_PIN);

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
}

void loop() {

    Measurement measurement = hcsr04.measure();

    serial_printf(Serial,
                  "Distance: %2f %s, Valid Samples: %l/%i [Signal Timed Out Count: %i, Response Timed Out Count: %i, Max Distance Exceeded Count: %i, Response Cool Down: %o]\n",
                  measurement.getDistance(), getDistanceUnitAbbreviation(measurement.getDistanceUnit()),
                  measurement.getValidMeasurementsCount(),
                  measurement.getTakenSamples(),
                  measurement.getSignalTimedOutCount(),
                  measurement.getResponseTimedOutCount(),
                  measurement.getMaxDistanceExceededCount(),
                  measurement.getIsResponseCoolDownActive());
}