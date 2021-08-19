#ifndef HC_SR04_HCSR04RESPONSEERRORS_H
#define HC_SR04_HCSR04RESPONSEERRORS_H


struct HCSR04ResponseErrors {

    unsigned int signalTimedOutCount;
    unsigned int responseTimedOutCount;
    unsigned int maxDistanceExceededCount;
};


#endif //HC_SR04_HCSR04RESPONSEERRORS_H
