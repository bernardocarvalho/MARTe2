/**
 * @file MultiBufferUnrelatedDataSourceGTest.cpp
 * @brief Source file for class MultiBufferUnrelatedDataSourceGTest
 * @date Apr 16, 2018
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
 * the class MultiBufferUnrelatedDataSourceGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MultiBufferUnrelatedDataSourceTest.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MultiBufferUnrelatedDataSourceGTest,TestInitialise) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestInitialise_StandardHeapDefault) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_StandardHeapDefault());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestInitialise_False_NoNumberOfBuffers) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoNumberOfBuffers());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestInitialise_False_InvalidHeap) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_InvalidHeap());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestAllocateMemory) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestGetNumberOfMemoryBuffers) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestGetSignalMemoryBuffer) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}


TEST(MultiBufferUnrelatedDataSourceGTest,TestGetBrokerName_SyncInput) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName_SyncInput());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestGetBrokerName_SyncOutput) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName_SyncOutput());
}


TEST(MultiBufferUnrelatedDataSourceGTest,TestGetInputBrokers) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestGetOutputBrokers) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestSetConfiguredDatabase) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestSetConfiguredDatabase_False_InternalTimeStamp_BadType) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InternalTimeStamp_BadType());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestSetConfiguredDatabase_False_InternalTimeStamp_Produced) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InternalTimeStamp_Produced());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestSetConfiguredDatabase_False_ErrorCheck_BadType) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_ErrorCheck_BadType());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestSetConfiguredDatabase_False_ErrorCheck_Produced) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_ErrorCheck_Produced());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestTerminateRead) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestTerminateRead());
}

TEST(MultiBufferUnrelatedDataSourceGTest,TestTerminateWrite) {
    MultiBufferUnrelatedDataSourceTest test;
    ASSERT_TRUE(test.TestTerminateWrite());
}


