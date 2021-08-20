#ifndef HC_SR04_OPTIONAL_H
#define HC_SR04_OPTIONAL_H

/**
 * A wrapper that gives you the ability to return a object that may exist or not and do different actions.
 *
 * Problem:
 * We want to elegantly return a default value if one doesn't exist.
 *
 * Example 1:
 *
 * int* someMethod();
 *
 * int* someMethodResult = someMethod();
 * int actualResult = 5;
 *
 * if(someMethodResult) {
 *   actualResult = *someMethodResult;
 * }
 *
 * Example 2:
 *
 * int* someMethod();
 *
 * int actualResult = someMethod() ? *someMethod() : 5;
 *
 * In the first example the code is slightly confusing. In the second example it is clear what we want to do, but the method is called twice.
 * Here comes the solution with the Optional. Short solution with one call.
 *
 * Optional<int> someMethod() {
 *   return Optional<int>(&variable);
 * }
 *
 * int actualResult = someMethod().orElse(5);
 *
 */
template<typename B>
class Optional {

private:
    B* base;

public:
    Optional(B* base) {
        this->base = base;
    }

    bool has() {
        return this->base;
    }

    B orElseGet(B value) {
        return this->has() ? *this->base : value;
    }
};


#endif //HC_SR04_OPTIONAL_H
