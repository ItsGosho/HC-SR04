#include "TemperatureUnits.h"

/**
 * Will converted the given temperature in the given fromUnit to the given toUnit
 *
 * @param temperature The temperature that will be converted
 * @param fromUnit The unit that the temperature will be converted to
 * @param toUnit The unit that the temperature is now
 * @return The converted temperature in the given fromUnit to the given toUnit. -1 If the given fromUnit is not yet implemented.
 */
float convertTemperatureUnit(const float& temperature, const TemperatureUnit& fromUnit, const TemperatureUnit& toUnit) {

    switch (fromUnit) {

        case TemperatureUnit::CELSIUS:
            return convertCelsiusTo(temperature, toUnit);;

        case TemperatureUnit::FAHRENHEIT:
            return convertFahrenheitTo(temperature, toUnit);

        default:
            return -1;
    }
}

/**
 * Will convert the given celsius to the given temperature unit
 *
 * @param celsius The celsius to be converted
 * @param toUnit The temperature unit that the celsius will be converted
 * @return The converted celsius to the given temperature unit. -1 If the given toUnit is not yet implemented.
 */
float convertCelsiusTo(const float& celsius, const TemperatureUnit& toUnit) {

    switch (toUnit) {

        case TemperatureUnit::CELSIUS:
            return celsius;

        case TemperatureUnit::FAHRENHEIT:
            return celsius * 1.8f + 32;

        default:
            return -1;
    }
}

/**
 * Will convert the given fahrenheit to the given temperature unit
 *
 * @param fahrenheit The fahrenheit to be converted
 * @param toUnit The temperature unit that the fahrenheit will be converted
 * @return The converted fahrenheit to the given temperature unit. -1 If the given toUnit is not yet implemented.
 */
float convertFahrenheitTo(const float& fahrenheit, const TemperatureUnit& toUnit) {
    switch (toUnit) {

        case TemperatureUnit::CELSIUS:
            return (fahrenheit - 32) * 0.5556f;

        case TemperatureUnit::FAHRENHEIT:
            return fahrenheit;

        default:
            return -1;
    }
}
