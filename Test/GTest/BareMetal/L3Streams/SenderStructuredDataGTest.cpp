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
    float32 floatArr[]={-1.5, 3.25, 55.4};
    StreamString stringMat[][2]={{"Hello", "Ciao"}, {"World", "Mondo"}};
    stringMat[0][0].Seek(0);
    stringMat[0][1].Seek(0);
    stringMat[1][0].Seek(0);
    stringMat[1][1].Seek(0);

    AnyType at[] = { x, y, z,floatArr, stringMat, voidAnyType };
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
    float32 floatArr[]={-1.5, 3.25, 55.4};
    StreamString stringMat[][2]={{"Hello", "Ciao"}, {"World", "Mondo"}};
    stringMat[0][0].Seek(0);
    stringMat[0][1].Seek(0);
    stringMat[1][0].Seek(0);
    stringMat[1][1].Seek(0);

    AnyType at[] = { x, y, z,floatArr, stringMat, voidAnyType };
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
    float32 floatArr[]={-1.5, 3.25, 55.4};
    StreamString stringMat[][2]={{"Hello", "Ciao"}, {"World", "Mondo"}};
    stringMat[0][0].Seek(0);
    stringMat[0][1].Seek(0);
    stringMat[1][0].Seek(0);
    stringMat[1][1].Seek(0);

    AnyType at[] = { x, y, z,floatArr, stringMat, voidAnyType };
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

TEST(SenderStructuredDataGTest, TestAddToCurrentNode) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestAddToCurrentNode());
}

TEST(SenderStructuredDataGTest, TestMoveToRoot) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToRoot());
}

TEST(SenderStructuredDataGTest, TestMoveToAncestor) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToAncestor());
}

TEST(SenderStructuredDataGTest, TestMoveAbsolute) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveAbsolute());
}

TEST(SenderStructuredDataGTest, TestMoveRelative) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveRelative());
}

TEST(SenderStructuredDataGTest, TestMoveToChild) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestMoveToChild());
}

TEST(SenderStructuredDataGTest, TestCreateAbsolute) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestCreateAbsolute());
}

TEST(SenderStructuredDataGTest, TestCreateRelative) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestCreateRelative());
}

TEST(SenderStructuredDataGTest, TestDelete) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestDelete());
}

TEST(SenderStructuredDataGTest, TestGetName) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestGetName());
}

TEST(SenderStructuredDataGTest, TestGetChildName) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestGetChildName());
}

TEST(SenderStructuredDataGTest, TestGetNumberOfChildren) {
    SenderStructuredDataTest<StandardPrinter> mySenderStructuredDataTest;
    ASSERT_TRUE(mySenderStructuredDataTest.TestGetNumberOfChildren());
}
