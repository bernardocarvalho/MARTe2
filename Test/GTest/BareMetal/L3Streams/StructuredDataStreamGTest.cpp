/**
 * @file StructuredDataStreamGTest.cpp
 * @brief Source file for class StructuredDataStreamGTest
 * @date 30 ago 2018
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
 * the class StructuredDataStreamGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include "StructuredDataStreamTest.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(StructuredDataStreamGTest, TestSwitch) {
    StructuredDataStreamTest test;
    ASSERT_TRUE(test.TestSwitch());
}

TEST(StructuredDataStreamGTest, TestLoad) {
    StructuredDataStreamTest test;
    ASSERT_TRUE(test.TestLoad());
}

TEST(StructuredDataStreamGTest, TestCommit) {
    StructuredDataStreamTest test;
    ASSERT_TRUE(test.TestCommit());
}

TEST(StructuredDataStreamGTest, TestDelete) {
    StructuredDataStreamTest test;
    ASSERT_TRUE(test.TestDelete());
}

TEST(StructuredDataStreamGTest, TestGetCurrentPath) {
    StructuredDataStreamTest test;
    ASSERT_TRUE(test.TestGetCurrentPath());
}

TEST(StructuredDataStreamGTest, TestSwitchPrintAndCommit) {
    StructuredDataStreamTest test;
    ASSERT_TRUE(test.TestSwitchPrintAndCommit());
}

