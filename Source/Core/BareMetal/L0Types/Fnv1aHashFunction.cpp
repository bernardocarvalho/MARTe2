/**
 * @file Fnv1aHashFunction.cpp
 * @brief Source file for class Fnv1aHashFunction
 * @date 19 ago 2019
 * @author pc
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
 * the class Fnv1aHashFunction (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Fnv1aHashFunction.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

Fnv1aHashFunction::Fnv1aHashFunction() :
        HashFunction() {
    // Auto-generated constructor stub for Fnv1aHashFunction
    // TODO Verify if manual additions are needed
}

Fnv1aHashFunction::~Fnv1aHashFunction() {
    // Auto-generated destructor stub for Fnv1aHashFunction
    // TODO Verify if manual additions are needed
}

uint32 Fnv1aHashFunction::Compute(const char8 * const in,
                                  const uint32 size) {

    uint8 *input = reinterpret_cast<uint8*>(const_cast<char8 *>(in));
    uint32 hash = 0x811c9dc5u;
    if (input != NULL) {
        if (size == 0u) {
            while ((*input)!=0u) {
                hash ^= (*input);
                input=&input[1];
                hash *= 0x01000193u;
            }
        }
        else {
            for (uint32 i = 0u; i < size; i++) {
                hash ^= (*input);
                input=&input[1];
                hash *= 0x01000193u;
            }
        }
    }
    return hash;
}
}
