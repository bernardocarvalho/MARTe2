/**
 * @file IOBufferFloatPrint.cpp
 * @brief Source file for class IOBufferFloatPrint
 * @date 02/10/2015
 * @author Giuseppe Ferrò
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class IOBufferFloatPrint (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "FormatDescriptor.h"
#include "IOBuffer.h"
#include "Shift.h"
#include "BitSetToInteger.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*lint -save -e9023 -e9024
 * 9023, 9024  [MISRA C++ Rule 16-3-1] [MISRA C++ Rule 16-3-2]. Justification: Use of operators # and ## required by this implementation. */

#define CHECK_AND_REDUCE(number,step,exponent)\
if (number >= static_cast<T>(1E ## step)){ \
exponent+=step; \
number *= static_cast<T>(1E- ## step); \
}

/*
 #define CHECK_AND_REDUCE_Q(number,step,exponent)\
if (number >= 1E ## step ## Q){ \
exponent+=step; \
number *= 1E- ## step ## Q; \
}
 */

#define CHECK_AND_INCREASE(number,step,exponent)\
if (number <= static_cast<T>(1E- ## step)){ \
exponent-=(step+1); \
number *= static_cast<T>(10E ## step); \
}

/*
 #define CHECK_AND_INCREASE_Q(number,step,exponent)\
if (number <= 1E- ## step ## Q){ \
exponent-=(step+1); \
number *= 10E ## step ## Q; \
}
 */

namespace MARTe {

static inline bool isNaN(const float32 x) {

    /*lint -e{9137} -e{777} [MISRA C++ Rule 6-2-2]. Justification: It is a trick to detect nan floats in standard IEEE.*/
    return (x != x);
}

static inline bool isNaN(const float64 x) {
    /*lint -e{9137} -e{777} [MISRA C++ Rule 6-2-2]. Justification: It is a trick to detect nan floats in standard IEEE.*/
    return (x != x);
}

static inline bool isInf(const float32 x) {
    return (!isNaN(x)) && (isNaN(x - x));
}

static inline bool isInf(const float64 x) {
    return (!isNaN(x)) && (isNaN(x - x));
}

/** @brief Normalize numbers in the form x.xxxxx 10**y, calculating the related exponent y.
 * @param positiveNumber is the number to normalize.
 * @param is the exponent in return.
 *
 * Exponent is increased or decreased, not set.
 * Supports numbers up to quad precision. */
template<typename T>
static inline void NormalizeFloatNumberPrivate(T &positiveNumber,
                                               int16 &exponent) {

    // used internally
    if (positiveNumber > static_cast<T>(0.0)) {

        // check and normalize progressively following a logaritmic pattern
        if (positiveNumber >= 1.0) {
            /*      if (sizeof(T) > 8) {
             CHECK_AND_REDUCE_Q(positiveNumber, 2048, exponent)
             CHECK_AND_REDUCE_Q(positiveNumber, 1024, exponent)
             CHECK_AND_REDUCE_Q(positiveNumber, 512, exponent)
             }*/
            if (sizeof(T) > 4u) {
                CHECK_AND_REDUCE(positiveNumber, 256, exponent)
                CHECK_AND_REDUCE(positiveNumber, 128, exponent)
                CHECK_AND_REDUCE(positiveNumber, 64, exponent)
            }
            CHECK_AND_REDUCE(positiveNumber, 32, exponent)
            CHECK_AND_REDUCE(positiveNumber, 16, exponent)
            CHECK_AND_REDUCE(positiveNumber, 8, exponent)
            CHECK_AND_REDUCE(positiveNumber, 4, exponent)
            CHECK_AND_REDUCE(positiveNumber, 2, exponent)
            CHECK_AND_REDUCE(positiveNumber, 1, exponent)
        }
        else {
            /*  if (sizeof(T) > 8) {
             CHECK_AND_INCREASE_Q(positiveNumber, 2048, exponent)
             CHECK_AND_INCREASE_Q(positiveNumber, 1024, exponent)
             CHECK_AND_INCREASE_Q(positiveNumber, 512, exponent)
             }*/
            if (sizeof(T) > 4u) {
                CHECK_AND_INCREASE(positiveNumber, 256, exponent)
                CHECK_AND_INCREASE(positiveNumber, 128, exponent)
                CHECK_AND_INCREASE(positiveNumber, 64, exponent)
            }
            CHECK_AND_INCREASE(positiveNumber, 32, exponent)
            CHECK_AND_INCREASE(positiveNumber, 16, exponent)
            CHECK_AND_INCREASE(positiveNumber, 8, exponent)
            CHECK_AND_INCREASE(positiveNumber, 4, exponent)
            CHECK_AND_INCREASE(positiveNumber, 2, exponent)
            CHECK_AND_INCREASE(positiveNumber, 1, exponent)
            CHECK_AND_INCREASE(positiveNumber, 0, exponent)
        }
    }
}

/** @brief Rapid calculation of 10**x passing the exponent x as argument.
 * @param output is 10**exponent.
 * @param exponent is the exponent.
 *
 * Rapid calculation of 10 to n both positive and negative.
 * Supports up to quad precision. */
template<typename T>
static inline void FastPowerOf10Private(T &output,
                                        int16 exponent) {
    T radix = static_cast<T>(10.0);
    if (exponent < 0) {
        radix = static_cast<T>(0.1);
        exponent = -exponent;
    }

    // float64 logaritmic approach
    // decompose exponent in sum of powers of 2
    output = static_cast<T>(1.0);
    // limit to range of quad precision (128 bits)
    uint16 mask = 0x2000u;
    // loop through trailing zeroes
    while ((mask > 0u) && (((static_cast<uint16>(exponent)) & mask) == 0u)) {
        mask = Shift::LogicalRightSafeShift(mask, 1u);
    }
    // loop from first one
    while (mask > 0u) {
        // at each step calculates the square of the power so far
        output *= output;
        // if the bit is set then multiply or divide by 10
        if (((static_cast<uint16>(exponent)) & (mask)) != 0u) {
            output *= radix;
        }
        mask = Shift::LogicalRightSafeShift(mask, 1u);
    }
}

/** @brief Rapid determination of size of the exponent.
 * @param exponent is the exponent parameter.
 * @return the number of digits for the exponent notation E+n. */
static inline int16 NumberOfDigitsOfExponent(int16 exponent) {

    int16 exponentNumberOfDigits = 0;
    // no exponent!
    if (exponent != 0) {

        // workout the size of exponent
        // the size of exponent is 2+expNDigits
        // sign of exponent is always produced
        exponentNumberOfDigits = 3;    // E+n

        // remove sign
        if (exponent < 0) {
            exponent = -exponent;
        }

        // work out size
        if (exponent < 100) { // below 100 is either 3 or 4
            if (exponent >= 10) {
                exponentNumberOfDigits++;
            }
        }
        else { // above or equal 100 is at least 5
            exponentNumberOfDigits += 2;
            // just add one for each size step above
            if (exponent >= 1000) {
                exponentNumberOfDigits++;
            }
            // just add one for each size step above
            if (exponent >= 10000) {
                exponentNumberOfDigits++;
            }
        }
    }
    return exponentNumberOfDigits;
}

/** @brief Decompose an exponent into a multiple of 3 and a remainder part.
 * @param is the exponent parameter.
 * @return the the exponent immediately precedent to argument multiple of 3.
 *
 * Exponent is modified to be the remainder.
 * Original exponent is the sum of the two parts.
 */
static inline int16 ExponentToEngineeringPrivate(int16 &exponent) {
    int16 engineeringExponent = 0;
    // if negative we need to bias by 2 so that exp=-1 => eng_exp = -3 and not 0
    if (exponent < 0) {
        engineeringExponent = (exponent - 2) / 3;
    }
    // if positive it is simply exp/3
    else {
        engineeringExponent = exponent / 3;
    }

    // multiply by 3 so that it is the actual exponent
    engineeringExponent *= 3;
    // calculate remainder
    exponent = exponent - engineeringExponent;

    return engineeringExponent;
}

/** @brief Calculate the size necessary for the rappresentation in fixed point with precision as the number of decimal digits.
 * @param exponent is the exponent of the number.
 * @param hasSign is true if the number is negative.
 * @param precision is the number of decimal digits.
 * @param maximumSize is the desired maximum size.
 * @return the necessary space for the print of the number.
 *
 * Calculates size of fixed numeric representation considering the zeros and the . needed beyond the significative digits.
 * Precision is int16 to allow safe subtraction and is updated to fit within maximumSize and converted into relative format
 * (first significative digits of the number). Negative precision means overflow (? print), zero precision means underflow (0 print). */
static inline int16 NumberOfDigitsFixedNotation(const int16 exponent,
                                                const bool hasSign,
                                                int16 & precision,
                                                const int16 maximumSize) {

    bool returned = false;
    // this will be the output
    int16 fixedNotationSize = 0;

    // in the positive case
    // start by evaluating size of number of minimum resolution
    if (exponent >= 0) {

        // fixed notation for large numbers needs a number of digits = 1+ exponent in this case no . is used
        fixedNotationSize = exponent + 1;

    }
    else { // negative exponent

        // consider space for minimum resolution --> 0;
        fixedNotationSize = 1;

    }

    // these are the decimal digits required beyond the minimum size
    int16 desiredDecimalDigits = precision;

    // consider the sign
    if (hasSign) {
        fixedNotationSize++;
    }

    // how much space left?
    int16 spaceLeft = maximumSize - fixedNotationSize;

    // are we not within limits even when disregarding precision?
    if (spaceLeft < 0) {

        if (exponent >= 0) {

            // 1 for the ?
            fixedNotationSize = 1;

            // -1 means no space!
            precision = -1;

        }
        else {
            // 1 for the 0
            fixedNotationSize = 1;

            // 0  means no zero
            precision = 0;

        }

        // just exit here
        returned = true;
    }

    if (!returned) {
        // do we need more
        if (desiredDecimalDigits > 0) {

            // no space for adding a single digit and the . exit
            if (spaceLeft < 2) {

                if (exponent >= 0) {
                    //precision remains the integer part size
                    precision = exponent + 1;
                }
                else {
                    //precision remains 0 because it means underflow (maxSize < 3 in this case)
                    precision = 0;
                }

                // just exit here
                returned = true;

            }
            else {
                // deduct the space for .
                spaceLeft--;

                // account for . in the space
                fixedNotationSize++;
            }

            if (!returned) {
                // how many digits will I not be able to print?
                int16 excessOfDigits = desiredDecimalDigits - spaceLeft;

                // if any adjust precision and digits
                if (excessOfDigits > 0) {
                    desiredDecimalDigits = spaceLeft;
                    precision -= excessOfDigits;
                }

                // account for the extra digits
                fixedNotationSize += desiredDecimalDigits;
            }
        }

        if (!returned) {
            // convert precision into relative format
            precision += exponent;
            precision++;

            //Negative precision is only possible for negative exponent and not enough space for a single digit
            if (precision < 0) {

                //0 means no zero
                precision = 0;

                //space for 0
                fixedNotationSize = 1;
            }
        }
    }

    return fixedNotationSize;
}

/** @brief Calculates the size necessary for the representation of a number in relative fixed point representation.
 * @param exponent is the exponent of the number.
 * @param hasSign is true if the number is negative.
 * @param precision is the number of the desired first significative digits to print.
 * @param maxSize is the desired maximum size.
 * @return the space necessary to print a number in fixed point relative notation.
 *
 * Calculate size of fixed numeric representation considering the zeros and the . needed beyond the significative digits
 * precision is int16 to allow safe subtraction and is updated to fit within maximumSize. Negative precision means overflow (? print),
 * zero precision means underflow (0 print). */
static inline int16 NumberOfDigitsFixedRelativeNotation(const int16 exponent,
                                                        const bool hasSign,
                                                        int16 & precision,
                                                        const int16 maximumSize) {

    bool returned = false;

    // this will be the output
    int16 fixedNotationSize = 0;

    // how many digits to print below zero
    int16 desiredDecimalDigits = 0;

    // in the positive case
    // start by evaluating size of number without decimals
    if (exponent >= 0) {

        // size of integer part
        // this needs to be printed
        int16 integerPartSize = exponent + 1;

        // these are the decimal digits required
        desiredDecimalDigits = precision - integerPartSize;

        // fixed notation for large numbers needs a number of digits = 1+ exponent in this case no . is used
        fixedNotationSize = integerPartSize;
    }
    else { // negative exponent

        // consider space for minimum resolution;
        fixedNotationSize = 2 - exponent;

        // these are the decimal digits required beyond the minimum size
        desiredDecimalDigits = precision - 1;
    }

    // consider the sign
    if (hasSign) {
        fixedNotationSize++;
    }

    // how much space left?
    int16 spaceLeft = maximumSize - fixedNotationSize;

    // are we not within limits even when disregarding precision?
    if (spaceLeft < 0) {

        if (exponent >= 0) {

            // 1 for the ?
            fixedNotationSize = 1;

            // -1 means overflow - number too big to fit
            precision = -1;

        }
        else {
            // 1 for the 0
            fixedNotationSize = 1;

            // 0 means underflow - reduced to 0
            precision = 0;

        }

        // just exit here
        returned = true;
    }

    if (!returned) {
        // do we need more
        if (desiredDecimalDigits > 0) {

            // no space for adding a single digit exit
            if (spaceLeft == 0) {

                // clip all of the precision expressed by decimals
                precision -= desiredDecimalDigits;

                // just exit here
                returned = true;
            }

            if (!returned) {
                // no space for adding a single digit and the . exit
                if (exponent >= 0) {
                    if (spaceLeft == 1) {

                        // clip all of the precision expressed by decimals
                        precision -= desiredDecimalDigits;

                        // just exit here
                        returned = true;

                    }
                    else {
                        // deduct the space for .
                        spaceLeft--;

                        // account for . in the space
                        fixedNotationSize++;
                    }
                }

                if (!returned) {

                    // how many digits will I not be able to print?
                    int16 excessOfDigits = desiredDecimalDigits - spaceLeft;

                    // if any adjust precision and digits
                    if (excessOfDigits > 0) {
                        desiredDecimalDigits = spaceLeft;
                        precision -= excessOfDigits;
                    }

                    // account for the extra digits
                    fixedNotationSize += desiredDecimalDigits;
                }
            }
        }
    }

    return fixedNotationSize;
}

/** @brief Calculates the size necessary for the representation of a number in exponential representation.
 * @param exponent is the exponent of the number.
 * @param hasSign is true if the number is negative.
 * @param precision is the number of the desired first significative digits to print.
 * @param maxSize is the desired maximum size.
 * @return the space necessary to print a number in exponential notation.
 *
 * Calculate size of exponential representation considering the zeros and the . needed beyond the significative digits
 * precision is int16 to allow safe subtraction and is updated to fit within maximumSize. Negative precision means overflow (? print).*/
static inline int16 NumberOfDigitsExponentialNotation(const int16 exponent,
                                                      const bool hasSign,
                                                      int16 & precision,
                                                      const int16 maximumSize) {
    // does not fit
    int16 retval = 1;

    if (precision > 0) {

        //exponential notation number size
        int16 exponentNotationSize = 0;

        // include exponent size
        exponentNotationSize += NumberOfDigitsOfExponent(exponent);

        // include mantissa size
        int16 mantissaSize = NumberOfDigitsFixedRelativeNotation(0, hasSign, precision, maximumSize - exponentNotationSize);

        retval = exponentNotationSize + mantissaSize;
    }

    return retval;
}

/** @brief Calculates the size necessary for the representation of a number in engineering representation.
 * @param exponent is the exponent of the number.
 * @param hasSign is true if the number is negative.
 * @param precision is the number of the desired first significative digits to print.
 * @param maxSize is the desired maximum size.
 * @return the space necessary to print a number in engineering notation.
 *
 * Calculate size of engineering representation considering the zeros and the . needed beyond the significative digits
 * precision is int16 to allow safe subtraction and is updated to fit within maximumSize. Negative precision means overflow (? print).*/
static inline int16 NumberOfDigitsEngineeringNotation(const int16 exponent,
                                                      const bool hasSign,
                                                      int16 & precision,
                                                      const int16 maximumSize) {

    // does not fit
    int16 retval = 1;
    if (precision > 0) {

        // decompose exponent in two parts
        int16 exponentRemainder = exponent;
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponentRemainder);

        int16 engineeringNotationSize = 0;

        // include exponent size
        engineeringNotationSize += NumberOfDigitsOfExponent(engineeringExponent);

        // include mantissa size
        int16 mantissaSize = NumberOfDigitsFixedRelativeNotation(exponentRemainder, hasSign, precision, maximumSize - engineeringNotationSize);

        retval = engineeringNotationSize + mantissaSize;
    }

    return retval;
}

/** @brief Calculates the size necessary for the representation of a number in smart representation.
 * @param exponent is the exponent of the number.
 * @param hasSign is true if the number is negative.
 * @param precision is the number of the desired first significative digits to print.
 * @param maxSize is the desired maximum size.
 * @return the space necessary to print a number in smart notation.
 *
 * Calculate size of smart representation considering the zeros and the . needed beyond the significative digits
 * precision is int16 to allow safe subtraction and is updated to fit within maximumSize. Negative precision means overflow (? print).*/
static inline int16 NumberOfDigitsSmartNotation(const int16 exponent,
                                                const bool hasSign,
                                                int16 &precision,
                                                const int16 maximumSize) {

    // does not fit
    int16 retval = 1;
    if (precision > 0) {

        // decompose exponent in two parts
        int16 exponentRemainder = exponent;
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponentRemainder);

        int16 smartNotationSize = 0;

        // check if in range for smart replacement of exponent
        if ((engineeringExponent != 0) && (engineeringExponent <= 24) && (engineeringExponent >= -24)) {
            // if so the exponent is simply a letter
            smartNotationSize++;
        }
        else {
            // or the whole E-xxx
            smartNotationSize += NumberOfDigitsOfExponent(engineeringExponent);
        }

        // include mantissa size
        int16 mantissaSize = NumberOfDigitsFixedRelativeNotation(exponentRemainder, hasSign, precision, maximumSize - smartNotationSize);

        retval = smartNotationSize + mantissaSize;
    }

    return retval;
}

/** @brief Calculates the size necessary for the representation of a number in compact representation.
 * @param exponent is the exponent of the number.
 * @param hasSign is true if the number is negative.
 * @param precision is the number of the desired first significative digits to print.
 * @param maxSize is the desired maximum size.
 * @return the space necessary to print a number in compact notation.
 *
 * Calculate size of compact representation considering the zeros and the . needed beyond the significative digits
 * precision is int16 to allow safe subtraction and is updated to fit within maximumSize. Negative precision means overflow (? print).*/
static inline int16 NumberOfDigitsCompactNotation(const int16 exponent,
                                                  const bool hasSign,
                                                  int16 & precision,
                                                  const int16 maximumSize) {

    // does not fit
    int16 retval = 1;

    if (precision > 0) {

        // decompose exponent in two parts
        int16 exponentRemainder = exponent;
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponentRemainder);

        int16 smartNotationSize = 0;
        int16 mantissaSize = 0;

        // check if in range for smart replacement of exponent
        if ((engineeringExponent <= 24) && (engineeringExponent >= -24)) {
            // if so the exponent is simply a letter
            if (engineeringExponent != 0) {
                smartNotationSize++;
            }

            // include mantissa size
            mantissaSize = NumberOfDigitsFixedRelativeNotation(exponentRemainder, hasSign, precision, maximumSize - smartNotationSize);

        }
        else {
            // or the whole E-xxx
            smartNotationSize += NumberOfDigitsOfExponent(engineeringExponent + exponentRemainder);

            // include mantissa size
            mantissaSize = NumberOfDigitsFixedRelativeNotation(0, hasSign, precision, maximumSize - smartNotationSize);

        }

        retval = smartNotationSize + mantissaSize;

    }

    return retval;
}

/** @brief Calculates the necessary size for the print of the number for each format.
 * @param notation specifies what is the desired notation.
 * @param exponent is the exponent of the number.
 * @param hasSign is true if the number is negative.
 * @param precision is the desired first significative numbers to print.
 * @param maximumSize is the desired maximum size.
 *
 * Given the format the exponent, the sign and the available size
 * calculates number size and achievable precision. */
static inline int16 NumberOfDigitsNotation(const FloatNotation &notation,
                                           const int16 exponent,
                                           const bool hasSign,
                                           int16 & precision,
                                           const int16 maximumSize) {

    int16 numberSize = 0;

    // do round ups
    if (notation == FixedPointNotation) {

        numberSize = NumberOfDigitsFixedNotation(exponent, hasSign, precision, maximumSize);
    }
    if (notation == FixedPointRNotation) {

        numberSize = NumberOfDigitsFixedRelativeNotation(exponent, hasSign, precision, maximumSize);
    }

    if (notation == ExponentNotation) {

        numberSize = NumberOfDigitsExponentialNotation(exponent, hasSign, precision, maximumSize);

    }
    if (notation == EngineeringNotation) {

        numberSize = NumberOfDigitsEngineeringNotation(exponent, hasSign, precision, maximumSize);

    }
    if (notation == SmartNotation) {

        numberSize = NumberOfDigitsSmartNotation(exponent, hasSign, precision, maximumSize);

    }
    if (notation == CompactNotation) {

        numberSize = NumberOfDigitsCompactNotation(exponent, hasSign, precision, maximumSize);

    }

    return numberSize;

}

/** @brief Prints the notation E+/-n on a generic ioBuffer which implements a PutC() function.
 * @param ioBuffer is the generic ioBuffer.
 * @param exponent is the exponent of the number.
 */
static inline void ExponentToStreamPrivate(IOBuffer & ioBuffer,
                                           int16 exponent) {
    // output exponent if exists
    if (exponent != 0) {
        if (!ioBuffer.PutC('E')) {
            //TODO
        }
        // print the exponent sign (both)
        // get the absolute value
        if (exponent > 0) {
            if (!ioBuffer.PutC('+')) {
                //TODO
            }
        }
        else {
            exponent = -exponent;
            if (!ioBuffer.PutC('-')) {
                //TODO
            }
        }
        // fast convert to int
        Number2StreamDecimalNotationPrivate(ioBuffer, exponent);
    }
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/** @brief Print the number (without sign and padding) on a generic ioBuffer which implements a PutC() function.
 * @param ioBuffer is the generic ioBuffer.
 * @param positiveNumber is the absolute value of the normalized number.
 * @param exponent is the exponent of the number.
 * @param precision is the number of first significative digits to print.
 * @return false only in case of incorrect digits.
 *
 * Converts a couple of positiveNumber-exponent to a string using fixed format.
 * PositiveNumber is not 0 nor Nan nor Inf and is positive, precision should be strictly positive. */
template<typename T>
bool FloatToFixedPrivate(IOBuffer & ioBuffer,
                         T positiveNumber,
                         int16 exponent,
                         int16 precision) {

    bool ret = true;

    // impossible
    // should never be called like this
    // better handle it anyway
    if ((positiveNumber < static_cast<T>(0.0)) || (positiveNumber >= static_cast<T>(10.0))) {
        if (!ioBuffer.PutC('!')) {
            //TODO
        }
        ret = false;
    }
    else {

        // numbers below 1.0
        // start with a 0.000 until we reach the first non zero digit
        if (exponent < 0) {
            if (!ioBuffer.PutC('0')) {
                //TODO
            }
            if (!ioBuffer.PutC('.')) {
                //TODO
            }

            // loop and add zeros

            for (int16 i = 0; i < -(exponent + 1); i++) {
                if (!ioBuffer.PutC('0')) {
                    //TODO
                }
            }
            // exponent has only the job of marking where to put the '.'
            // here is lowered by 1 to avoid adding a second '.' in the following code
            exponent--;
        }

        // loop to fulfil precision
        // also must reach the end of the integer part thus exponent is checked
        while ((exponent >= 0) || (precision > 0)) {

            // before outputting the fractional part add a '.'
            if (exponent == -1) {
                if (!ioBuffer.PutC('.')) {
                    //TODO
                }
            }

            // no more significative digits (all below rounding )
            // but still some exponent (fixed format)
            if (precision == 0) {
                if (!ioBuffer.PutC('0')) {
                    //TODO
                }
            }
            else {
                // get a digit and shift the number
                int8 digit = static_cast<int8>(positiveNumber);
                positiveNumber -= static_cast<T>(digit);
                positiveNumber *= static_cast<T>(10.0);

                int8 zero = static_cast<int8>('0');
                if (!ioBuffer.PutC(static_cast<char8>(zero + digit))) {
                    //TODO
                }
            }

            // update precision
            if (precision > 0) {
                precision--;
            }

            // update exponent
            exponent--;
        }
    }
    return ret;
}

/** @brief Implements functions to print the number for each format on a generic ioBuffer which implements a PutC() function.
 * @param notation is the desired notation.
 * @param ioBuffer is the generic ioBuffer (any class with a PutC(char8 c) method )
 * @param normalizedNumber is the normalized number.
 * @param exponent is the exponent of the number.
 * @param precision is the number of the first significative digits to print.
 */
template<typename T>
bool FloatToStreamPrivate(const FloatNotation &notation,
                          IOBuffer & ioBuffer,
                          const T normalizedNumber,
                          int16 exponent,
                          const int16 precision) {

    bool ret = false;
    // do round ups
    bool isFPNotation = (notation == FixedPointNotation);
    bool isFPRNotation = (notation == FixedPointRNotation);
    if ((isFPNotation) || (isFPRNotation)) {

        // does all the work of conversion but for the sign and special cases
        ret = FloatToFixedPrivate(ioBuffer, normalizedNumber, exponent, precision);

    }
    if (notation == ExponentNotation) {

        // does all the work of conversion but for the sign and special cases
        ret = FloatToFixedPrivate(ioBuffer, normalizedNumber, 0, precision);

        // writes exponent
        ExponentToStreamPrivate(ioBuffer, exponent);

    }
    if (notation == EngineeringNotation) {

        // partitions the exponent between engineering part and residual
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponent);

        // does all the work of conversion but for the sign and special cases
        ret = FloatToFixedPrivate(ioBuffer, normalizedNumber, exponent, precision);

        // writes exponent
        ExponentToStreamPrivate(ioBuffer, engineeringExponent);

    }
    if (notation == SmartNotation) {

        // partitions the exponent between engineering part and residual
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponent);

        // does all the work of conversion but for the sign and special cases
        ret = FloatToFixedPrivate(ioBuffer, normalizedNumber, exponent, precision);
        // check if exponent in correct range
        if ((engineeringExponent != 0) && (engineeringExponent <= 24) && (engineeringExponent >= -24)) {
            static const char8 * const symbols = "yzafpnum KMGTPEZY";
            if (!ioBuffer.PutC(symbols[(engineeringExponent / 3) + 8])) {
                //TODO
            }
        }
        else {
            // writes exponent
            ExponentToStreamPrivate(ioBuffer, engineeringExponent);
        }

    }
    if (notation == CompactNotation) {

        // partitions the exponent between engineering part and residual
        int16 engineeringExponent = ExponentToEngineeringPrivate(exponent);

        // check if exponent in correct range
        if ((engineeringExponent <= 24) && (engineeringExponent >= -24)) {

            // does all the work of conversion but for the sign and special cases
            ret = FloatToFixedPrivate(ioBuffer, normalizedNumber, exponent, precision);

            //Put the symbol only if the engineering exp is different than zero.
            if (engineeringExponent != 0) {
                static const char8 * const symbols = "yzafpnum KMGTPEZY";
                if (!ioBuffer.PutC(symbols[(engineeringExponent / 3) + 8])) {
                    //TODO
                }
            }
        }
        else {
            // does all the work of conversion but for the sign and special cases
            ret = FloatToFixedPrivate(ioBuffer, normalizedNumber, 0, precision);

            // writes exponent
            ExponentToStreamPrivate(ioBuffer, engineeringExponent + exponent);
        }

    }
    return ret;
}

/** @brief A list of avaiable display modalities to manage the behaviour of the function. */
enum FloatDisplayModes {
    Normal = 0,  // one of the float32 Notations
    ZeroFloat = 11, // 0
    NanFloat = 22, // Nan
    InfPFloat = 33, //+Inf
    InfNFloat = 44, //-Inf
    InsufficientSpaceForFloat = 77, // not enough space
    NoFormat = 99  // undecided yet
};

/** @brief Check if a number is NaN or +/- Inf.
 * @param number is the number to be checked.
 * @param maximumSize is the desired maximum size.
 * @param neededSize returns the size necessary to print NaN or +/- Inf. It is 0 in case of normal number.
 */
template<typename T>
FloatDisplayModes CheckNumber(const T number,
                              const int16 maximumSize,
                              int16 & neededSize) {

    // not decided yet
    neededSize = 0;
    FloatDisplayModes ret = NoFormat;

    // check for NaN
    if (isNaN(number)) {
        if (maximumSize < 3) {
            neededSize = 1;
            ret = InsufficientSpaceForFloat;
        }
        else {
            neededSize = 3;
            ret = NanFloat;
        }
    }

    // check for Inf
    if (isInf(number)) {
        if (maximumSize < 4) {
            neededSize = 1;
            ret = InsufficientSpaceForFloat;
        }
        else {
            neededSize = 4;
            if (number < static_cast<T>(0)) {
                ret = InfNFloat;
            }
            else {
                ret = InfPFloat;
            }
        }
    }

    // check for zero
    /*lint -e{9137} -e{777} [MISRA C++ Rule 6-2-2]. Justification: This condition is intended to be true when the float number is precisely equal to zero
     * and not in a small range.*/
    if (number == static_cast<T>(0.0)) {
        neededSize = 1;
        ret = ZeroFloat;
    }

    return ret;
}

//const bool debug = false;

/** @brief Implements the rounding of the number looking at the precision.
 * @param number is the normalized number to be rounded.
 * @param precision is the desired precision for the round.
 * @return the rounded number.
 *
 * It adds 5 to the first digit after the precision digits. */
template<typename T>
T RoundUpNumber(T number,
                const int16 precision) {
    // what is the magnitude of the correction
    int16 correctionExponent = -precision;

    // to round up add a correction value just below last visible digit
    // calculates 5 * 10**correctionExponent
    T correction;
    FastPowerOf10Private(correction, correctionExponent);
    correction *= static_cast<T>(5.0);
    number += correction;

    return number;
}

/** @brief Prints a float32 (or equivalent) number on a generic ioBuffer which implements a PutC() function.
 * @param ioBuffer is a generic ioBuffer class.
 * @param number is the number to print.
 * @param format specifies the desired format (padding, precision, max size)
 * @return true.
 *
 * In case of incorrect characters a '!' will be printed.
 * If the number cannot fit in the desired maximum size  because the overflow '?' will be printed, '0' in case of underflow.
 * It prints NaN in case of nan (i.e 0/0) or +Inf (i.e 1/0) or -Inf (i.e -1/0). */
template<typename T>
bool FloatToStreamPrivate(IOBuffer & ioBuffer, // must have a GetC(c) function where c is of a type that can be obtained from chars
                          const T number,
                          const FormatDescriptor &format) {

    // the space to our disposal to print the number
    int16 maximumSize = static_cast<int16>(format.size);
    // in case of right alignment
    bool isPadded = format.padded;

    // 0 means unlimited
    // to keep things symmetric we set to a large number
    // 1000 should not constitute a limit for a float32!
    if (maximumSize == 0) {
        maximumSize = 1000;
        isPadded = false;
    }

    int16 formatPrecision = static_cast<int16>(format.precision);

    // on precision 0 the max useful precision is chosen
    // based on the ieee float32 format number of significative digits
    if (format.precision == defaultPrecision) {
        if (sizeof(T) > 8u) {
            formatPrecision = 34;
        }
        if (sizeof(T) == 8u) {
            formatPrecision = 15;
        }
        if (sizeof(T) < 8u) {
            formatPrecision = 7;
        }

        //Default 6 decimal digits for fixed point notation.
        if (format.floatNotation == FixedPointNotation) {
            formatPrecision = 6;
        }
    }

    // number of decimal digits or number of significative digits
    int16 precision = formatPrecision;

    // this is the second main objective of the first part
    // to find out the size that is needed or if there is no space
    int16 numberSize;

    // this will be used everywhere!
    T positiveNumber = number;

    // hold the exponent after normalisation
    int16 exponent = 0;

    // whether the - needs to be output
    bool hasSign = false;

    // adjust sign
    if (number < 0.0) {
        hasSign = true;
        positiveNumber = -number;
    }

    // check for special cases where notation is not relevant
    // if found them then mode and size are assigned
    FloatDisplayModes chosenMode = CheckNumber(number, maximumSize, numberSize);

    // no chosen mode yet try all formats
    if (chosenMode == NoFormat) {

        // normalize number
        NormalizeFloatNumberPrivate(positiveNumber, exponent);

        // work out achievable precision  and number size
        uint8 notation = static_cast<uint8>(format.floatNotation);
        numberSize = NumberOfDigitsNotation(notation, exponent, hasSign, precision, maximumSize);

        //the precision to use after the rounding in case of overflow.
        int16 maxPrecisionAfterRounding = formatPrecision;

        // apply rounding up. Remember that for fix point precision is different.
        bool isFPNotation = format.floatNotation == FixedPointNotation;
        bool isFPRNotation = format.floatNotation == FixedPointRNotation;
        if ((isFPNotation) || (isFPRNotation)) {

            if (precision >= 0) {
                //If the precision is clipped, it will be clipped again because the size could only increase for fix point after round up.
                maxPrecisionAfterRounding = formatPrecision;

                //Round up.
                positiveNumber = RoundUpNumber(positiveNumber, precision);
            }
            //If a fixed point fails, it will fail also after the rounding up, so do nothing.
        }
        else {

            if (precision >= 0) {
                //In case of overflow and precision clip it add a digit.
                maxPrecisionAfterRounding = precision + 1;

                //Round up.
                positiveNumber = RoundUpNumber(positiveNumber, precision);
                //if (debug)printf ("roundN = %f maxP = %i\n",positiveNumber,maxPrecisionAfterRounding);
            }
            else {
                //We enter here only in case of exponential forms.

                // Consider for the approximation precision=1 (i.e 950e3 in format 4.x become 1e6, 949.999 not).
                // (or in exp format 9.5e-10 in 4.x format become 1E-9, 9.4e-10 not).
                maxPrecisionAfterRounding = 2;

                //Round up at the first decimal number.
                positiveNumber = RoundUpNumber(positiveNumber, 1);
                //if (debug)printf ("roundN = %f \n",positiveNumber);

            }
        }

        // if we have an overflow recalculate numbersize and precision
        if (positiveNumber >= static_cast<T>(10.0)) {

            //The precision can't be greater than format.precision
            if (maxPrecisionAfterRounding > formatPrecision) {
                maxPrecisionAfterRounding = formatPrecision;
            }

            positiveNumber /= static_cast<T>(10.0);
            exponent++;

            precision = maxPrecisionAfterRounding;

            //if (debug)printf ("2nd normN = %f exp =%i prec= %i\n",positiveNumber,exponent,precision);

            // work out achievable precision  and number size
            notation = static_cast<uint8>(format.floatNotation);
            numberSize = NumberOfDigitsNotation(notation, exponent, hasSign, precision, maximumSize);
            //if (debug)printf ("2nd Nsize = %i prec= %i\n",numberSize,precision);

        }

        // assume we can print
        chosenMode = Normal;

        // then check
        // If precision is zero print only 0
        if (precision == 0) {
            numberSize = 1;
            chosenMode = ZeroFloat;
        }

        // then check
        // If precision is negative print only ?
        if (precision < 0) {
            numberSize = 1;
            chosenMode = InsufficientSpaceForFloat;
        }

    }

    bool isLeftAligned = format.leftAligned;
    if ((isPadded) && (!isLeftAligned)) {
        for (int32 i = numberSize; i < maximumSize; i++) {
            if (!ioBuffer.PutC(' ')) {
                //TODO
            }
        }
    }

    switch (chosenMode) {
    case Normal: {

        // output sign
        if (hasSign) {
            if (!ioBuffer.PutC('-')) {
                //TODO
            }
        }
        uint8 notation = static_cast<uint8>(format.floatNotation);
        if (!FloatToStreamPrivate(notation, ioBuffer, positiveNumber, exponent, precision)) {
            //TODO
        }
    }
        break;
    case NoFormat:
    case InsufficientSpaceForFloat: {
        if (!ioBuffer.PutC('?')) {
            //TODO
        }
    }
        break;
    case NanFloat: {

        if (!ioBuffer.PutC('N')) {
            //TODO
        }
        if (!ioBuffer.PutC('a')) {
            //TODO
        }
        if (!ioBuffer.PutC('N')) {
            //TODO
        }
    }
        break;
    case InfPFloat: {
        if (!ioBuffer.PutC('+')) {
            //TODO
        }
        if (!ioBuffer.PutC('I')) {
            //TODO
        }
        if (!ioBuffer.PutC('n')) {
            //TODO
        }
        if (!ioBuffer.PutC('f')) {
            //TODO
        }
    }
        break;
    case InfNFloat: {
        if (!ioBuffer.PutC('-')) {
            //TODO
        }
        if (!ioBuffer.PutC('I')) {
            //TODO
        }
        if (!ioBuffer.PutC('n')) {
            //TODO
        }
        if (!ioBuffer.PutC('f')) {
            //TODO
        }
    }
        break;
    case ZeroFloat: {
        if (!ioBuffer.PutC('0')) {
            //TODO
        }
    }
        break;
    }

    // in case of left alignment
    if ((isPadded) && (isLeftAligned)) {
        for (int32 i = numberSize; i < maximumSize; i++) {
            if (!ioBuffer.PutC(' ')) {
                //TODO
            }
        }
    }

    return true;
}

bool FloatToStream(IOBuffer &buffer,
                   const float32 number,
                   const FormatDescriptor &format) {
    return FloatToStreamPrivate(buffer, number, format);
}

bool FloatToStream(IOBuffer &buffer,
                   const float64 number,
                   const FormatDescriptor &format) {
    return FloatToStreamPrivate(buffer, number, format);
}

}
