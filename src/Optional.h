#ifndef HC_SR04_OPTIONAL_H
#define HC_SR04_OPTIONAL_H


template<typename B>
class Optional {

private:
    B* base;

public:
    Optional(B* base) {
        this->base = base;
    }

    B orElseGet(B value) {
        return this->base ? *this->base : value;
    }
};


#endif //HC_SR04_OPTIONAL_H
