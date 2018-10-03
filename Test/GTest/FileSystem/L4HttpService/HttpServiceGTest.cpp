/**
 * @file HttpServiceGTest.cpp
 * @brief Source file for class HttpServiceGTest
 * @date 12 set 2018
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
 * the class HttpServiceGTest (public, protected, and private). Be aware that some 
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
#include "HttpServiceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(HttpServiceGTest, TestConstructor) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(HttpServiceGTest, TestInitialise) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(HttpServiceGTest, TestInitialise_DefaultWebRoot) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultWebRoot());
}

TEST(HttpServiceGTest, TestInitialise_FalseNoDefaultWebRoot) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoDefaultWebRoot());
}


TEST(HttpServiceGTest, TestInitialise_DefaultNListenConnections) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultNListenConnections());
}

TEST(HttpServiceGTest, TestInitialise_DefaultRelyUrl) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultRelyUrl());
}

TEST(HttpServiceGTest, TestInitialise_DefaultPort) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultPort());
}
/*
TEST(HttpServiceGTest, TestClientService_Text_Interactive) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestClientService_Text_Interactive());
}
*/

TEST(HttpServiceGTest, TestStart) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestStart());
}

TEST(HttpServiceGTest, TestStart_InvalidWebRoot) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestStart_InvalidWebRoot());
}

TEST(HttpServiceGTest, TestClientService_Text) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestClientService_Text());
}

TEST(HttpServiceGTest, TestClientService_Structured) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestClientService_Structured());
}

TEST(HttpServiceGTest, TestClientService_CloseConnection) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestClientService_CloseConnection());
}

TEST(HttpServiceGTest, TestClientService_InvalidInterface) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestClientService_InvalidInterface());
}

TEST(HttpServiceGTest, TestClientService_FailReadHeader) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestClientService_FailReadHeader());
}

TEST(HttpServiceGTest, TestServerCycle) {
    HttpServiceTest test;
    ASSERT_TRUE(test.TestServerCycle());
}



