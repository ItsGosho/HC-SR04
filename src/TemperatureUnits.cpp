#include "TemperatureUnits.h"

float convertTemperatureUnit(const float& temperature, const TemperatureUnit& fromUnit, const TemperatureUnit& toUnit) {

    switch (fromUnit) {

        case TemperatureUnit::CELSIUS:

            switch (toUnit) {

                case TemperatureUnit::CELSIUS:
                    return temperature;

                case TemperatureUnit::FAHRENHEIT:
                    return temperature * 1.8f + 32;
            }

            break;

        case TemperatureUnit::FAHRENHEIT:

            switch (toUnit) {

                case TemperatureUnit::CELSIUS:
                    return (temperature - 32) * 0.5556f;

                case TemperatureUnit::FAHRENHEIT:
                    return temperature;
            }

            break;
    }

    return -1;
}
