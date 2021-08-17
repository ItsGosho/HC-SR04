#include "Utils.h"

/**
 * Will block until the given LOW/HIGH state is not present on the given pin.
 * If the expected signal is not achieved in the given timeout time, then a timeout will occur
 *
 * @param digitalPin The digital pin that state we are waiting for
 * @param expectedState LOW or HIGH
 * @param timeoutUS The max time that the state has to be the given one
 * @return If there was a timeout.
 */
bool waitStateNot(const unsigned int digitalPin, const char& expectedState, const unsigned long& timeoutUS) {

    unsigned long start = micros();

    while (digitalRead(digitalPin) != expectedState) {

        if (micros() - start >= timeoutUS)
            return true;
    }

    return false;
}

/**
 * Will block until the given LOW/HIGH state is present on the given pin.
 * If the expected signal is not achieved in the given timeout time, then a timeout will occur
 *
 * @param digitalPin The digital pin that state we are waiting for
 * @param expectedState LOW or HIGH
 * @param timeoutUS The max time that the state has to be the given one
 * @return If there was a timeout.
 */
bool waitStateIs(const unsigned int digitalPin, const char& expectedState, const unsigned long& timeoutUS) {

    unsigned long start = micros();

    while (digitalRead(digitalPin) == expectedState) {

        if (micros() - start >= timeoutUS)
            return true;
    }

    return false;
}

/**
 * Will measure the length of a given signal.
 * If the current signal is not the given one, then it will block until it is.
 * If the current signal is the given one, then it will measure it directly.
 *
 * @param pin The digital pin, which will be used to determinate the signal
 * @param mode HIGH or LOW
 * @return The length of the given signal in microseconds
 */
unsigned long measureSignalLength(const uint8_t& pin, const int& mode) {

    while (digitalRead(pin) != mode);

    unsigned long start = micros();

    while (digitalRead(pin) == mode);

    return micros() - start;
}

/**
 * Will measure the length of a given signal, but with a timeout.
 * If the current signal is not the given one, then it will block until it is.
 * If the current signal is the given one, then it will measure it directly.
 * If the waiting of the signal or its measurement take longer than the given timeout, a timeout will occur.
 *
 * @param pin The digital pin, which will be used to determinate the signal
 * @param mode HIGH or LOW
 * @return The data of the measurement. If there was a timeout, then the signal length will be 0 and the timeout - true
 */
SignalLengthMeasurementUS measureSignalLength(const uint8_t& pin, const char& mode, const unsigned long& timeoutUS) {


    bool isWaitingTimedOut = waitStateNot(pin, mode, timeoutUS);

    if (isWaitingTimedOut)
        return SignalLengthMeasurementUS{0, true};

    unsigned long signalLengthStart = micros();
    bool isMeasuringTimedOut = waitStateIs(pin, mode, timeoutUS);

    if (isMeasuringTimedOut)
        return SignalLengthMeasurementUS{0, true};

    unsigned long signalLength = micros() - signalLengthStart;
    return {signalLength, false};
}
