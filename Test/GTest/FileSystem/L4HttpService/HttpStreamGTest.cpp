/**
 * @file HttpStreamGTest.cpp
 * @brief Source file for class HttpStreamGTest
 * @date 03 set 2018
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
 * the class HttpStreamGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HttpStreamTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(HttpStreamGTest, TestConstructor) {
    HttpStreamTest test;
    ASSERT_TRUE(test.TestConstructor());
}


TEST(HttpStreamGTest, TestReadHeader_Get1) {
    HttpStreamTest test;
    ASSERT_TRUE(test.TestReadHeader_Get1());
}

TEST(HttpStreamGTest, TestReadHeader_Get2_Commands) {
    HttpStreamTest test;
    ASSERT_TRUE(test.TestReadHeader_Get2_Commands());
}

TEST(HttpStreamGTest, TestReadHeader_Put1) {
    HttpStreamTest test;
    ASSERT_TRUE(test.TestReadHeader_Put1());
}

TEST(HttpStreamGTest, TestReadHeader_Post1) {
    HttpStreamTest test;
    ASSERT_TRUE(test.TestReadHeader_Post1());
}

TEST(HttpStreamGTest, TestReadHeader_Post2_Multiform) {
    HttpStreamTest test;
    ASSERT_TRUE(test.TestReadHeader_Post2_Multiform());
}

TEST(HttpStreamGTest, TestReadHeader_Head) {
    HttpStreamTest test;
    ASSERT_TRUE(test.TestReadHeader_Head());
}

TEST(HttpStreamGTest, TestReadHeader_Reply) {
    HttpStreamTest test;
    ASSERT_TRUE(test.TestReadHeader_Reply());
}

TEST(HttpStreamGTest, TestCompleteReadOperation) {
    HttpStreamTest test;
    ASSERT_TRUE(test.TestCompleteReadOperation());
}
