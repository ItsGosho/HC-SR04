#ifndef HC_SR04_HCSR04RESPONSE_H
#define HC_SR04_HCSR04RESPONSE_H


class HCSR04Response {

private:
    unsigned long mHighSignalLengthUS;
    bool mIsResponseTimedOut;
    bool mIsSignalTimedOut;

public:

    HCSR04Response();

    HCSR04Response(unsigned long mHighSignalLengthUs, bool mIsResponseTimedOut);

    unsigned long getHighSignalLengthUS() const;

    bool isResponseTimedOut() const;

    bool isSignalTimedOut(const unsigned long& timedOutSignalLengthUS) const;
};


#endif //HC_SR04_HCSR04RESPONSE_H
