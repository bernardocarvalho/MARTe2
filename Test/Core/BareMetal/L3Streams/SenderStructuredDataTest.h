/**
 * @file SenderStructuredDataTest.h
 * @brief Header file for class SenderStructuredDataTest
 * @date 07 set 2018
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

 * @details This header file contains the declaration of the class SenderStructuredDataTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SENDERSTRUCTUREDDATATEST_H_
#define SENDERSTRUCTUREDDATATEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SenderStructuredData.h"
#include "StandardPrinter.h"
#include "JsonPrinter.h"
#include "XMLPrinter.h"
#include "Matrix.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

struct SenderStructuredDataTestWriteStruct {
    AnyType *toWrite;
    const char8 ** varnames;
    StreamString desResult;
};

struct SenderStructuredDataTestAddToCurrentNodeStruct {
    Reference toAdd;
    const char8 ** movements;
    bool *expected;
    StreamString desResult;
    uint32 nMoveUps;
    uint32 *index;
};

template<class Printer>
class SenderStructuredDataTest {
public:
    SenderStructuredDataTest();
    virtual ~SenderStructuredDataTest();

    bool TestRead();

    bool TestGetType();

    bool TestWrite(SenderStructuredDataTestWriteStruct *table);

    bool TestCopy();

    bool TestAddToCurrentNode(SenderStructuredDataTestAddToCurrentNodeStruct *table);

    bool TestMoveToRoot(SenderStructuredDataTestAddToCurrentNodeStruct *table);

    bool TestMoveToAncestor(SenderStructuredDataTestAddToCurrentNodeStruct *table);

    bool TestMoveAbsolute(SenderStructuredDataTestAddToCurrentNodeStruct *table);

    bool TestMoveRelative(SenderStructuredDataTestAddToCurrentNodeStruct *table);

    bool TestMoveToChild(SenderStructuredDataTestAddToCurrentNodeStruct *table);

    bool TestCreateAbsolute(SenderStructuredDataTestAddToCurrentNodeStruct *table);

    bool TestCreateRelative(SenderStructuredDataTestAddToCurrentNodeStruct *table);

    bool TestDelete();

    bool TestGetName(SenderStructuredDataTestAddToCurrentNodeStruct *table);

    bool TestGetChildName(SenderStructuredDataTestAddToCurrentNodeStruct *table);

    bool TestGetNumberOfChildren(SenderStructuredDataTestAddToCurrentNodeStruct *table);

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<class Printer>
SenderStructuredDataTest<Printer>::SenderStructuredDataTest() {
    // Auto-generated constructor stub for SenderStructuredDataTest
    // TODO Verify if manual additions are needed
}

template<class Printer>
SenderStructuredDataTest<Printer>::~SenderStructuredDataTest() {
    // Auto-generated destructor stub for SenderStructuredDataTest
    // TODO Verify if manual additions are needed
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestRead() {
    StreamString string;
    SenderStructuredData<StandardPrinter> test(string);

    uint32 x;
    return !test.Read("boh", x);
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestGetType() {
    StreamString string;
    SenderStructuredData<StandardPrinter> test(string);

    return test.GetType("boh").IsVoid();

}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestWrite(SenderStructuredDataTestWriteStruct *table) {
    StreamString string;
    SenderStructuredData<Printer> test(string);

    bool ret = true;
    uint32 i = 0u;
    while ((!(table->toWrite[i].IsVoid())) && (ret)) {
        ret = test.Write(table->varnames[i], table->toWrite[i]);
        i++;
    }

    if (ret) {
        ret = string == table->desResult;
        printf("|%s||%s| %lld %lld\n", string.Buffer(), table->desResult.Buffer(), string.Size(), table->desResult.Size());

    }

    return ret;

}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestCopy() {
    StreamString string;
    SenderStructuredData<StandardPrinter> test(string);
    SenderStructuredData<StandardPrinter> copy(string);

    //copy not supported
    return !test.Copy(copy);
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestAddToCurrentNode(SenderStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    SenderStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveAbsolute(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestMoveToRoot(SenderStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    SenderStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveAbsolute(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = test.MoveToRoot();
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestMoveToAncestor(SenderStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    SenderStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveAbsolute(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = test.MoveToAncestor(table->nMoveUps);
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestMoveAbsolute(SenderStructuredDataTestAddToCurrentNodeStruct *table) {
    return TestAddToCurrentNode(table);
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestMoveRelative(SenderStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    SenderStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveRelative(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestMoveToChild(SenderStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    SenderStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveToChild(table->index[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestCreateAbsolute(SenderStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    SenderStructuredData<Printer> test(string);

    bool ret = true;

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.CreateAbsolute(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestCreateRelative(SenderStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    SenderStructuredData<Printer> test(string);

    bool ret = true;

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.CreateRelative(table->movements[i])==table->expected[i]);
        i++;
    }

    if (ret) {
        ret = (string == table->desResult);
    }
    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestDelete() {
    StreamString string;
    SenderStructuredData<Printer> test(string);

    return !test.Delete("boh");
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestGetName(SenderStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    SenderStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(test.MoveToChild(table->index[i])==table->expected[i]);
        if(ret) {
            ret=(StringHelper::Compare(test.GetName(),table->movements[i])==0);
        }
        i++;
    }

    return ret;

}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestGetChildName(SenderStructuredDataTestAddToCurrentNodeStruct *table) {
    StreamString string;
    SenderStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    test.MoveToChild(0);

    uint32 i = 0u;
    while ((table->movements[i] != NULL) && (ret)) {
        ret=(StringHelper::Compare(test.GetChildName(table->index[i]),table->movements[i])==0);
        i++;
    }

    printf("|%s||%s|\n", string.Buffer(), table->desResult.Buffer());

    return ret;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestGetNumberOfChildren(SenderStructuredDataTestAddToCurrentNodeStruct *table) {

    StreamString string;
    SenderStructuredData<Printer> test(string);

    bool ret = test.AddToCurrentNode(table->toAdd);

    test.MoveToChild(0);

    if (ret) {
        ret = test.GetNumberOfChildren() == table->nMoveUps;
    }

    return ret;
}

#endif /* TEST_CORE_BAREMETAL_L3STREAMS_SENDERSTRUCTUREDDATATEST_H_ */

