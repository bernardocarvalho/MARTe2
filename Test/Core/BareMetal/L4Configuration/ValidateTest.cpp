/**
 * @file ValidateTest.cpp
 * @brief Source file for class ValidateTest
 * @date 26/01/2016
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
 * the class ValidateTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ValidateTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ValidateTest::TestValidate() {
    const char8*attributes = "+Valid1 = {Class=BasicTypeStandardValidator min=2 max=120 }";

    uint32 x = 10;
    if (!Validate(x, attributes)) {
        return false;
    }
    x = 120;
    if (!Validate(x, attributes)) {
        return false;
    }
    x = 2;
    if (!Validate(x, attributes)) {
        return false;
    }
    x = 121;
    if (Validate(x, attributes)) {
        return false;
    }
    x = 1;
    if (Validate(x, attributes)) {
        return false;
    }
    return true;
}


