/**
 * @file ProcessorTypeTest.cpp
 * @brief Source file for class ProcessorTypeTest
 * @date 25/06/2015
 * @author Giuseppe Ferro
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
 * the class ProcessorTypeTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ProcessorTypeTest.h"
#include "GeneralDefinitions.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ProcessorTypeTest::ProcessorTypeTest() :
        ptFirst(0xFE),
        ptSecond(0xFD),
        ptAny(0xFF),
        ptNone(0x0) {
}

bool ProcessorTypeTest::TestAssignmentOperator() {

    bool result = true;

    ProcessorType ptTest;
    ptTest = 0xFD;
    result &= (ptTest.GetProcessorMask() == 0xFDu);

    ptTest = ptFirst;
    result &= (ptTest.GetProcessorMask() == ptFirst.GetProcessorMask());

    return result;
}

bool ProcessorTypeTest::TestOROperator() {

    bool result = true;

    ProcessorType ptTest(ptSecond);
    ptTest |= ptAny;
    result &= (ptTest == ptAny);

    ptTest = ptSecond;
    ptTest |= ptFirst;
    result &= (ptTest == ptAny);

    ptTest = ptSecond;
    ptTest |= ptNone;
    result &= (ptTest == ptSecond);

    ptTest = 0x01u;
    ptTest |= 0x02u;
    result &= (ptTest == 0x03u);

    return result;

}

bool ProcessorTypeTest::TestEqualityOperator() {

    bool result = true;

    ProcessorType ptTest(ptSecond);
    result &= (ptTest == 0xFDu);

    ptTest = 0xFDu;
    result &= (ptTest == ptSecond);

    return result;

}

bool ProcessorTypeTest::TestInequalityOperator() {

    bool result = true;

    ProcessorType ptTest(0xFAu);
    result &= (ptTest != 0xFDu);

    result &= (ptTest != ptSecond);

    return result;

}

bool ProcessorTypeTest::TestSetGetDefaultCPUs() {
    ProcessorType ptTest(ptSecond);

    ptTest.SetDefaultCPUs(0xAAu);
    uint64 test = ptTest.GetDefaultCPUs();

    if (test != 0xAAu){
        ptTest.SetDefaultCPUs(0u);
        return false;
    }
    ptTest.SetDefaultCPUs(0u);
    return true;

}

bool ProcessorTypeTest::TestDefaultConstructor() {
    ProcessorType ptDefault;             // Std contructor
    return (ptDefault.GetProcessorMask() == 0xFEu);
}
bool ProcessorTypeTest::TestConstructorFromMask() {
    ProcessorType ptFromMask(0xFCu);      // Mask constructor

    return (ptFromMask.GetProcessorMask() == 0xFCu);

}

bool ProcessorTypeTest::TestConstructorFromProcessorType() {
    ProcessorType toCopy(0xFCu);
    ProcessorType ptFromPT(toCopy);  // Constructor from other PT
    return (ptFromPT.GetProcessorMask() == 0xFCu);

}

bool ProcessorTypeTest::TestSetMask(uint64 mask) {
    ProcessorType test;
    test.SetMask(mask);
    return ((test.GetProcessorMask() == mask));
}

bool ProcessorTypeTest::TestAddCPU(uint32 cpuNumber1,
                                   uint32 cpuNumber2) {
    ProcessorType test;
    test.SetMask(0u);
    test.AddCPU(cpuNumber1);

    uint64 save = 1 << (cpuNumber1 - 1);
    if (test.GetProcessorMask() != save) {
        return false;
    }

    test.AddCPU(cpuNumber2);
    save |= (1 << (cpuNumber2 - 1));
    if (test.GetProcessorMask() != save) {
        return false;
    }

     return true;
}
