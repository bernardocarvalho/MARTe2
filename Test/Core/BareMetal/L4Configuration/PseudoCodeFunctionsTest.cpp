/**
 * @file PseudoCodeFunctionsTest.cpp
 * @brief Source file for class PseudoCodeFunctionsTest
 * @date 06/05/2020
 * @author Didac Magrinya
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
 * the class PseudoCodeFunctionsTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "PseudoCodeFunctionsTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool PseudoCodeFunctionsTest::TestDefaultConstructor() {

    PseudoCode::FunctionRecord functionRecordUT;
    bool ok;

    ok = (functionRecordUT.GetName() == "");
    ok &= (functionRecordUT.GetInputTypes().GetNumberOfElements() == 0);
    ok &= (functionRecordUT.GetOutputTypes().GetNumberOfElements() == 0);

    return ok;
}

bool PseudoCodeFunctionsTest::TestTryConsume(PseudoCode::FunctionRecord functionRecordUT, CCString inputName, StaticStack<TypeDescriptor,32> &inputTypeStack, bool expectedRet, PseudoCode::DataMemoryAddress expectedDataStackSize) {

    bool ok = true;

    //TODO: Consider matchOutput==true case

    PseudoCode::DataMemoryAddress dataStackSize = 0;
    TypeDescriptor outputType;
    Vector<TypeDescriptor> functionInputTypes = functionRecordUT.GetInputTypes();
    Vector<TypeDescriptor> functionOutputTypes = functionRecordUT.GetOutputTypes();
    uint32 initialStackSize = inputTypeStack.GetSize();

    ok &= (expectedRet == functionRecordUT.TryConsume(inputName, inputTypeStack, false, dataStackSize));
    if (expectedRet) {
        ok &= (inputTypeStack.GetSize() == initialStackSize - functionInputTypes.GetNumberOfElements() + functionOutputTypes.GetNumberOfElements());
        for (uint32 i = 0; ((ok) && (i < functionOutputTypes.GetNumberOfElements())); ++i){
            inputTypeStack.Pop(outputType);
            ok &= (outputType == functionOutputTypes[functionOutputTypes.GetNumberOfElements() - i - 1]);
        }
        //TODO: Check that remaining stack has not changed
        ok &= (dataStackSize == expectedDataStackSize);
    } else {
        ok &= (inputTypeStack.GetSize() == initialStackSize);
        //TODO: Check that stack has not changed
        ok &= (dataStackSize == 0);
    }

    return ok;
}

void MockFunction(PseudoCode::Context &context) {
    ((void)0);
}