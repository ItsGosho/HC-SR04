#ifndef HC_SR04_TEMPERATUREUNITS_H
#define HC_SR04_TEMPERATUREUNITS_H

enum class TemperatureUnit {
    CELSIUS, FAHRENHEIT
};

float convertTemperatureUnit(const float& temperature, const TemperatureUnit& fromUnit, const TemperatureUnit& toUnit);

float convertCelsiusTo(const float& celsius, const TemperatureUnit& toUnit);

float convertFahrenheitTo(const float& fahrenheit, const TemperatureUnit& toUnit);

#endif //HC_SR04_TEMPERATUREUNITS_H
