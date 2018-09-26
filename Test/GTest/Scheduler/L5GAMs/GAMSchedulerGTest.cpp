/**
 * @file GAMSchedulerGTest.cpp
 * @brief Source file for class GAMSchedulerGTest
 * @date 09/ago/2016
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
 * the class GAMSchedulerGTest (public, protected, and private). Be aware that some 
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
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "GAMSchedulerTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(GAMSchedulerGTest,TestConstructor) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(GAMSchedulerGTest,TestStartNextStateExecution) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestStartNextStateExecution());
}

TEST(GAMSchedulerGTest,TestStartNextStateExecution_False_PrepareNextState) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestStartNextStateExecution_False_PrepareNextState());
}

TEST(GAMSchedulerGTest,TestStopCurrentStateExecution) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestStopCurrentStateExecution());
}

TEST(GAMSchedulerGTest,TestIntegrated) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestIntegrated());
}
/*
TEST(GAMSchedulerGTest,TestIntegrated_TriggerErrorMessage) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestIntegrated_TriggerErrorMessage());
}
*/
TEST(GAMSchedulerGTest,TestPurge) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestPurge());
}

TEST(GAMSchedulerGTest,TestInitialise) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(GAMSchedulerGTest,TestInitialise_ErrorMessage) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestInitialise_ErrorMessage());
}

TEST(GAMSchedulerGTest,TestInitialise_False_InvalidMessage) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestInitialise_False_InvalidMessage());
}

TEST(GAMSchedulerGTest,TestInitialise_False_MoreThanOneErrorMessage) {
    GAMSchedulerTest test;
    ASSERT_TRUE(test.TestInitialise_False_MoreThanOneErrorMessage());
}
