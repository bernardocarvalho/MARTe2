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

template<class Printer>
class SenderStructuredDataTest {
public:
    SenderStructuredDataTest();
    virtual ~SenderStructuredDataTest();

    bool TestRead();

    bool TestGetType();

    bool TestWrite(SenderStructuredDataTestWriteStruct *table);

    bool TestCopy();

    bool TestAddToCurrentNode();

    bool TestMoveToRoot();

    bool TestMoveToAncestor();

    bool TestMoveAbsolute();

    bool TestMoveRelative();

    bool TestMoveToChild();

    bool TestCreateAbsolute();

    bool TestCreateRelative();

    bool TestDelete();

    bool TestGetName();

    bool TestGetChildName();

    bool TestGetNumberOfChildren();

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

    if(ret){
        ret=string==table->desResult;
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
bool SenderStructuredDataTest<Printer>::TestAddToCurrentNode() {
    ReferenceT<NodeName> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<NodeName> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<NodeName> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<NodeName> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    c->Insert(d);

    StreamString string;
    SenderStructuredData<JsonPrinter> test(string);

    test.AddToCurrentNode(a);

    test.MoveAbsolute("A.C.D");
    test.MoveAbsolute("A.B");
    test.MoveAbsolute("A.C.D");

    printf("%s\n", string.Buffer());
    return true;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestMoveToRoot() {
    return true;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestMoveToAncestor() {
    return true;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestMoveAbsolute() {
    return true;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestMoveRelative() {
    return true;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestMoveToChild() {
    return true;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestCreateAbsolute() {
    return true;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestCreateRelative() {
    return true;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestDelete() {
    return true;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestGetName() {
    return true;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestGetChildName() {
    return true;
}

template<class Printer>
bool SenderStructuredDataTest<Printer>::TestGetNumberOfChildren() {
    return true;
}


#endif /* TEST_CORE_BAREMETAL_L3STREAMS_SENDERSTRUCTUREDDATATEST_H_ */

