/**
 * @file ProcessorTest.cpp
 * @brief Source file for class ProcessorTest
 * @date 25/06/2015
 * @author Giuseppe Ferr�
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
 * the class ProcessorTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ProcessorTest.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ProcessorTest::ProcessorTest() {

}

bool ProcessorTest::TestVendorId() {
    bool testResult = false;
    const char8 * vendorId = NULL;

    vendorId = Processor::VendorId();
    printf("the TestVendorIs is: %s\n", vendorId);
    testResult = (vendorId != NULL);

    return testResult;
}

bool ProcessorTest::TestFamily() {
    bool testResult = false;
    uint32 family = -1;
    uint32 familyAux = family;

    family = Processor::Family();
    printf("family is : %u\n", family);
    testResult = (family != familyAux);

    return testResult;
}

bool ProcessorTest::TestModel() {
    bool testResult = false;
    uint32 model = -1;
    uint32 modelAux = model;

    model = Processor::Model();
    printf("model is : %u\n", model);
    testResult = (model != modelAux);

    return testResult;
}

bool ProcessorTest::TestAvailable() {
    bool testResult = false;
    uint32 available = -1;
    uint32 availableAux = available;

    available = Processor::Available();
    testResult = (available != availableAux);
    printf("available cpus is : %u\n", available);
    return testResult;
}
