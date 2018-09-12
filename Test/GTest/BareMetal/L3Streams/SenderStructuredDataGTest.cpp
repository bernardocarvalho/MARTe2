/**
 * @file SenderStructuredDataGTest.cpp
 * @brief Source file for class SenderStructuredDataGTest
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

 * @details This source file contains the definition of all the methods for
 * the class SenderStructuredDataGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "SenderStructuredDataTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(SenderStructuredDataGTest, TestRead) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestRead());
}

TEST(SenderStructuredDataGTest, TestGetType) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestGetType());
}

TEST(SenderStructuredDataGTest, TestWriteStandard) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    uint32 x = 1;
    int32 y = -1;
    const char8 *z = "hello";
    float32 floatArr[] = { -1.5, 3.25, 55.4 };
    StreamString stringMat[][2] = { { "Hello", "Ciao" }, { "World", "Mondo" } };
    stringMat[0][0].Seek(0);
    stringMat[0][1].Seek(0);
    stringMat[1][0].Seek(0);
    stringMat[1][1].Seek(0);

    AnyType at[] = { x, y, z, floatArr, stringMat, voidAnyType };
    const char8 *names[] = { "x", "y", "z", "floatArr", "stringMat", NULL};
    SenderStructuredDataTestWriteStruct table;
    table.toWrite = at;
    table.varnames = names;
    table.desResult = "\n\rx =+1\n\r"
            "y =-1\n\r"
            "z =\"hello\"\n\r"
            "floatArr ={ -1.500000 3.250000 55.400004 } \n\r"
            "stringMat ={ { \"Hello\" \"Ciao\" } { \"World\" \"Mondo\" } } ";
    ASSERT_TRUE(mySenderStructuredDataTest.TestWrite(&table));
}

TEST(SenderStructuredDataGTest, TestWriteJson) {
    SenderStructuredDataTest<JsonPrinter> mySenderStructuredDataTest;
    uint32 x = 1;
    int32 y = -1;
    const char8 *z = "hello";
    float32 floatArr[] = { -1.5, 3.25, 55.4 };
    StreamString stringMat[][2] = { { "Hello", "Ciao" }, { "World", "Mondo" } };
    stringMat[0][0].Seek(0);
    stringMat[0][1].Seek(0);
    stringMat[1][0].Seek(0);
    stringMat[1][1].Seek(0);

    AnyType at[] = { x, y, z, floatArr, stringMat, voidAnyType };
    const char8 *names[] = { "x", "y", "z", "floatArr", "stringMat", NULL};
    SenderStructuredDataTestWriteStruct table;
    table.toWrite = at;
    table.varnames = names;
    table.desResult = "\n\r\"x\": +1,\n\r"
            "\"y\": -1,\n\r"
            "\"z\": \"hello\",\n\r"
            "\"floatArr\": [ -1.500000, 3.250000, 55.400004 ] ,\n\r"
            "\"stringMat\": [ [ \"Hello\", \"Ciao\" ] ,[ \"World\", \"Mondo\" ] ] ";
    ASSERT_TRUE(mySenderStructuredDataTest.TestWrite(&table));
}

TEST(SenderStructuredDataGTest, TestWriteXML) {
    SenderStructuredDataTest<XMLPrinter> mySenderStructuredDataTest;
    uint32 x = 1;
    int32 y = -1;
    const char8 *z = "hello";
    float32 floatArr[] = { -1.5, 3.25, 55.4 };
    StreamString stringMat[][2] = { { "Hello", "Ciao" }, { "World", "Mondo" } };
    stringMat[0][0].Seek(0);
    stringMat[0][1].Seek(0);
    stringMat[1][0].Seek(0);
    stringMat[1][1].Seek(0);

    AnyType at[] = { x, y, z, floatArr, stringMat, voidAnyType };
    const char8 *names[] = { "x", "y", "z", "floatArr", "stringMat", NULL};
    SenderStructuredDataTestWriteStruct table;
    table.toWrite = at;
    table.varnames = names;
    table.desResult = "\n\r<x>+1</x>\n\r"
            "<y>-1</y>\n\r"
            "<z>\"hello\"</z>\n\r"
            "<floatArr>[ -1.500000, 3.250000, 55.400004 ] </floatArr>\n\r"
            "<stringMat>[ [ \"Hello\", \"Ciao\" ] ,[ \"World\", \"Mondo\" ] ] </stringMat>";
    ASSERT_TRUE(mySenderStructuredDataTest.TestWrite(&table));
}

TEST(SenderStructuredDataGTest, TestCopy) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestCopy());
}

TEST(SenderStructuredDataGTest, TestAddToCurrentNodeStandard) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "}\n\r"
            "}\n\r"
            "B = {";

    ASSERT_TRUE(mySenderStructuredDataTest.TestAddToCurrentNode(&arg));
}

TEST(SenderStructuredDataGTest, TestAddToCurrentNodeJson) {
    SenderStructuredDataTest<JsonPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "},\n\r"
            "\"B\": {";

    ASSERT_TRUE(mySenderStructuredDataTest.TestAddToCurrentNode(&arg));
}

TEST(SenderStructuredDataGTest, TestAddToCurrentNodeJson2) {
    SenderStructuredDataTest<JsonPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A", "A.B", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "},\n\r"
            "\"B\": {";

    ASSERT_TRUE(mySenderStructuredDataTest.TestAddToCurrentNode(&arg));
}

TEST(SenderStructuredDataGTest, TestAddToCurrentNodeXML) {
    SenderStructuredDataTest<XMLPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>\n\r"
            "</D>\n\r"
            "</C>\n\r"
            "<B>";

    ASSERT_TRUE(mySenderStructuredDataTest.TestAddToCurrentNode(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveToRootStandard) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "}\n\r"
            "}\n\r"
            "B = {\n\r"
            "}\n\r"
            "}";

    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToRoot(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveToRootJson) {
    SenderStructuredDataTest<JsonPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "},\n\r"
            "\"B\": {\n\r"
            "}\n\r"
            "}";

    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToRoot(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveToRootXML) {
    SenderStructuredDataTest<XMLPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>\n\r"
            "</D>\n\r"
            "</C>\n\r"
            "<B>\n\r"
            "</B>\n\r"
            "</A>";

    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToRoot(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveToAncestorStandard) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.B", "A.C.D", "A.C.D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.nMoveUps = 2u;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "B = {\n\r"
            "}\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "}\n\r"
            "}";

    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToAncestor(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveToAncestorJson) {
    SenderStructuredDataTest<JsonPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.B", "A.C.D", "A.C.D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.nMoveUps = 2u;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"B\": {\n\r"
            "},\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "}";

    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToAncestor(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveToAncestorXML) {
    SenderStructuredDataTest<XMLPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.B", "A.C.D", "A.C.D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.nMoveUps = 2u;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<B>\n\r"
            "</B>\n\r"
            "<C>\n\r"
            "<D>\n\r"
            "</D>\n\r"
            "</C>";

    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToAncestor(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveAbsoluteStandard) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "}\n\r"
            "}\n\r"
            "B = {";

    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveAbsolute(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveAbsoluteJson) {
    SenderStructuredDataTest<JsonPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "},\n\r"
            "\"B\": {";

    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveAbsolute(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveAbsoluteXML) {
    SenderStructuredDataTest<XMLPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>\n\r"
            "</D>\n\r"
            "</C>\n\r"
            "<B>";

    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveAbsolute(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveRelativeStandard) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C.D", "D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {";
    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveRelative(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveRelativeJson) {
    SenderStructuredDataTest<JsonPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C.D", "D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {";
    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveRelative(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveRelativeXML) {
    SenderStructuredDataTest<XMLPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C.D", "D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>";
    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveRelative(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveToChildStandard) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C", "D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {";

    uint32 index[] = { 0, 1, 0 };
    arg.index = index;
    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToChild(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveToChildJson) {
    SenderStructuredDataTest<JsonPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C", "D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {";

    uint32 index[] = { 0, 1, 0 };
    arg.index = index;
    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToChild(&arg));
}

TEST(SenderStructuredDataGTest, TestMoveToChild) {
    SenderStructuredDataTest<XMLPrinter> mySenderStructuredDataTest;

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

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    arg.toAdd = a;
    const char8 *movements[] = { "A", "C", "D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>";

    uint32 index[] = { 0, 1, 0 };
    arg.index = index;
    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToChild(&arg));
}

TEST(SenderStructuredDataGTest, TestCreateAbsoluteStandard) {

    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;


    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "}\n\r"
            "}\n\r"
            "B = {";

    ASSERT_TRUE(mySenderStructuredDataTest.TestCreateAbsolute(&arg));
}

TEST(SenderStructuredDataGTest, TestCreateAbsoluteJson) {

    SenderStructuredDataTest<JsonPrinter> mySenderStructuredDataTest;


    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "\"A\": {\n\r"
            "\"C\": {\n\r"
            "\"D\": {\n\r"
            "}\n\r"
            "},\n\r"
            "\"B\": {";

    ASSERT_TRUE(mySenderStructuredDataTest.TestCreateAbsolute(&arg));
}


TEST(SenderStructuredDataGTest, TestCreateAbsoluteXML) {

    SenderStructuredDataTest<XMLPrinter> mySenderStructuredDataTest;

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    const char8 *movements[] = { "A.C.D", "A.B", "A.C.D", NULL };
    bool expected[] = { true, true, false };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "<A>\n\r"
            "<C>\n\r"
            "<D>\n\r"
            "</D>\n\r"
            "</C>\n\r"
            "<B>";

    ASSERT_TRUE(mySenderStructuredDataTest.TestCreateAbsolute(&arg));
}


TEST(SenderStructuredDataGTest, TestCreateRelativeStandard) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;

    SenderStructuredDataTestAddToCurrentNodeStruct arg;
    const char8 *movements[] = { "A", "C.D", "E", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    arg.desResult = "\n\r"
            "A = {\n\r"
            "C = {\n\r"
            "D = {\n\r"
            "E = {";

    ASSERT_TRUE(mySenderStructuredDataTest.TestCreateRelative(&arg));
}

TEST(SenderStructuredDataGTest, TestDelete) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestDelete());
}

TEST(SenderStructuredDataGTest, TestGetNameStandard) {

    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;

    SenderStructuredDataTestAddToCurrentNodeStruct arg;

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

    arg.toAdd = a;

    const char8 *movements[] = { "A", "C", "D", NULL };
    bool expected[] = { true, true, true };
    arg.expected = expected;
    arg.movements = movements;
    uint32 index[]={0, 1, 0};
    arg.index = index;

    ASSERT_TRUE(mySenderStructuredDataTest.TestGetName(&arg));
}

TEST(SenderStructuredDataGTest, TestGetChildName) {

    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;

    SenderStructuredDataTestAddToCurrentNodeStruct arg;

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

    arg.toAdd = a;

    const char8 *movements[] = { "B", "C", NULL };
    bool expected[] = { true, true };
    arg.expected = expected;
    arg.movements = movements;
    uint32 index[]={0, 1};
    arg.index = index;

    ASSERT_TRUE(mySenderStructuredDataTest.TestGetChildName(&arg));

}

TEST(SenderStructuredDataGTest, TestGetNumberOfChildren) {

    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;

    SenderStructuredDataTestAddToCurrentNodeStruct arg;

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

    arg.toAdd = a;
    arg.nMoveUps = 2u;

    ASSERT_TRUE(mySenderStructuredDataTest.TestGetNumberOfChildren(&arg));
}
