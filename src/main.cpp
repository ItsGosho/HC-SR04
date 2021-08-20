#include <Arduino.h>
#include <SerialPrintF.h>
#include "hcsr04/HCSR04.h"

#define SERIAL_BAUD_RATE 9600
#define HCSR04_ONE_WIRE_PIN 9

HCSR04 hcsr04(HCSR04_ONE_WIRE_PIN);

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    //hcsr04.setDefaultResponseTimeoutCoolDownMS(1000);
}

void loop() {


    Measurement measurement = hcsr04.measure(
            MeasurementConfiguration::builder()
                    .withSamples(5)
                    .withDefaultResponseTimeoutCoolDown(5000)
                    .build());

    serial_printf(Serial,
                  "Distance: %2f %s, Valid Samples: %l/%i [Signal Timed Out Count: %i, Response Timed Out Count: %i, Max Distance Exceeded Count: %i, Response CoolDown: %s]\n",
                  measurement.getDistance(),
                  getDistanceUnitAbbreviation(measurement.getDistanceUnit()),
                  measurement.getValidMeasurementsCount(),
                  measurement.getTakenSamples(),
                  measurement.getSignalTimedOutCount(),
                  measurement.getResponseTimedOutCount(),
                  measurement.getMaxDistanceExceededCount(),
                  measurement.getIsResponseCoolDownActive() ? "Yes" : "No");
}