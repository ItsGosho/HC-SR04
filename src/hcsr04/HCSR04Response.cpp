#include "HCSR04Response.h"

HCSR04Response::HCSR04Response() {
    this->mHighSignalLengthUS = 0;
    this->mIsResponseTimedOut = false;
    this->mIsSignalTimedOut = false;
}

HCSR04Response::HCSR04Response(unsigned long mHighSignalLengthUs, bool mIsResponseTimedOut) : mHighSignalLengthUS(
        mHighSignalLengthUs), mIsResponseTimedOut(mIsResponseTimedOut) {
}

unsigned long HCSR04Response::getHighSignalLengthUS() const {
    return this->mHighSignalLengthUS;
}

bool HCSR04Response::isResponseTimedOut() const {
    return this->mIsResponseTimedOut;
}

bool HCSR04Response::isSignalTimedOut(const unsigned long& timedOutSignalLengthUS) const {
    return this->mHighSignalLengthUS >= timedOutSignalLengthUS;
}
