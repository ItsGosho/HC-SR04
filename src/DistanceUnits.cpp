#include "DistanceUnits.h"

float convertDistanceUnit(const float& distance, const DistanceUnit& fromUnit, const DistanceUnit& toUnit) {

    switch (fromUnit) {

        case DistanceUnit::CENTIMETERS:

            switch (toUnit) {

                case DistanceUnit::CENTIMETERS:
                    return distance;

                case DistanceUnit::METERS:
                    return distance / 100.00f;

                case DistanceUnit::INCH:
                    return distance * 0.393700787f;

                case DistanceUnit::FEET:
                    return distance * 0.032808399f;

                case DistanceUnit::YARD:
                    return distance * 0.010936133f;
            }

            break;

        case DistanceUnit::METERS:

            switch (toUnit) {

                case DistanceUnit::CENTIMETERS:
                    return distance * 100.00f;

                case DistanceUnit::METERS:
                    return distance;

                case DistanceUnit::INCH:
                    return distance * 39.3700787f;

                case DistanceUnit::FEET:
                    return distance * 3.2808399f;

                case DistanceUnit::YARD:
                    return distance * 1.0936133f;
            }

            break;

        case DistanceUnit::INCH:

            switch (toUnit) {

                case DistanceUnit::CENTIMETERS:
                    return distance * 2.54f;

                case DistanceUnit::METERS:
                    return distance * 0.0254f;

                case DistanceUnit::INCH:
                    return distance;

                case DistanceUnit::FEET:
                    return distance * 0.0833333333f;

                case DistanceUnit::YARD:
                    return distance * 0.0277777778f;
            }

            break;

        case DistanceUnit::FEET:

            switch (toUnit) {

                case DistanceUnit::CENTIMETERS:
                    return distance * 30.48f;

                case DistanceUnit::METERS:
                    return distance * 0.3048f;

                case DistanceUnit::INCH:
                    return distance * 12.00f;

                case DistanceUnit::FEET:
                    return distance;

                case DistanceUnit::YARD:
                    return distance * 0.333333333f;
            }

            break;

        case DistanceUnit::YARD:

            switch (toUnit) {

                case DistanceUnit::CENTIMETERS:
                    return distance * 91.44f;

                case DistanceUnit::METERS:
                    return distance * 0.9144f;

                case DistanceUnit::INCH:
                    return distance * 36.00f;

                case DistanceUnit::FEET:
                    return distance * 3.00f;

                case DistanceUnit::YARD:
                    return distance;
            }

            break;
    }

    return -1;
}
