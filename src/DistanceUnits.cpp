#include "DistanceUnits.h"

/**
 * Will converted the given distance in the given fromUnit to the given toUnit
 *
 * @param distance The distance that will be converted
 * @param fromUnit The unit that the distance will be converted to
 * @param toUnit The unit that the distance is now
 * @return The converted distance in the given fromUnit to the given toUnit. -1 If the given fromUnit is not yet implemented.
 */
float convertDistanceUnit(const float& distance, const DistanceUnit& fromUnit, const DistanceUnit& toUnit) {

    switch (fromUnit) {

        case DistanceUnit::CENTIMETERS:
            return convertCentimetersTo(distance, toUnit);

        case DistanceUnit::METERS:
            return convertMetersTo(distance, toUnit);

        case DistanceUnit::INCH:
            return convertInchesTo(distance, toUnit);

        case DistanceUnit::FEET:
            return convertFeetTo(distance, toUnit);

        case DistanceUnit::YARD:
            return convertYardsTo(distance, toUnit);

        default:
            return -1;
    }
}

/**
 * Will convert the given centimeters to the given distance unit
 *
 * @param feet The centimeters to be converted
 * @param toUnit The unit that the centimeters will be converted
 * @return The converted centimeters to the given unit. -1 If the given toUnit is not yet implemented.
 */
float convertCentimetersTo(const float& centimeters, const DistanceUnit& toUnit) {

    switch (toUnit) {

        case DistanceUnit::CENTIMETERS:
            return centimeters;

        case DistanceUnit::METERS:
            return centimeters / 100.00f;

        case DistanceUnit::INCH:
            return centimeters * 0.393700787f;

        case DistanceUnit::FEET:
            return centimeters * 0.032808399f;

        case DistanceUnit::YARD:
            return centimeters * 0.010936133f;

        default:
            return -1;
    }

}

/**
 * Will convert the given meters to the given distance unit
 *
 * @param feet The meters to be converted
 * @param toUnit The unit that the meters will be converted
 * @return The converted meters to the given unit. -1 If the given toUnit is not yet implemented.
 */
float convertMetersTo(const float& meters, const DistanceUnit& toUnit) {

    switch (toUnit) {

        case DistanceUnit::CENTIMETERS:
            return meters * 100.00f;

        case DistanceUnit::METERS:
            return meters;

        case DistanceUnit::INCH:
            return meters * 39.3700787f;

        case DistanceUnit::FEET:
            return meters * 3.2808399f;

        case DistanceUnit::YARD:
            return meters * 1.0936133f;

        default:
            return -1;
    }
}

/**
 * Will convert the given inches to the given distance unit
 *
 * @param feet The inches to be converted
 * @param toUnit The unit that the inches will be converted
 * @return The converted inches to the given unit. -1 If the given toUnit is not yet implemented.
 */
float convertInchesTo(const float& inches, const DistanceUnit& toUnit) {

    switch (toUnit) {

        case DistanceUnit::CENTIMETERS:
            return inches * 2.54f;

        case DistanceUnit::METERS:
            return inches * 0.0254f;

        case DistanceUnit::INCH:
            return inches;

        case DistanceUnit::FEET:
            return inches * 0.0833333333f;

        case DistanceUnit::YARD:
            return inches * 0.0277777778f;

        default:
            return -1;
    }
}

/**
 * Will convert the given feet to the given distance unit
 *
 * @param feet The feet to be converted
 * @param toUnit The unit that the feet will be converted
 * @return The converted feet to the given unit. -1 If the given toUnit is not yet implemented.
 */
float convertFeetTo(const float& feet, const DistanceUnit& toUnit) {

    switch (toUnit) {

        case DistanceUnit::CENTIMETERS:
            return feet * 30.48f;

        case DistanceUnit::METERS:
            return feet * 0.3048f;

        case DistanceUnit::INCH:
            return feet * 12.00f;

        case DistanceUnit::FEET:
            return feet;

        case DistanceUnit::YARD:
            return feet * 0.333333333f;

        default:
            return -1;
    }
}

/**
 * Will convert the given yards to the given distance unit
 *
 * @param yards The yards to be converted
 * @param toUnit The unit that the yards will be converted
 * @return The converted yards to the given unit. -1 If the given toUnit is not yet implemented.
 */
float convertYardsTo(const float& yards, const DistanceUnit& toUnit) {

    switch (toUnit) {

        case DistanceUnit::CENTIMETERS:
            return yards * 91.44f;

        case DistanceUnit::METERS:
            return yards * 0.9144f;

        case DistanceUnit::INCH:
            return yards * 36.00f;

        case DistanceUnit::FEET:
            return yards * 3.00f;

        case DistanceUnit::YARD:
            return yards;

        default:
            return -1;
    }
}
