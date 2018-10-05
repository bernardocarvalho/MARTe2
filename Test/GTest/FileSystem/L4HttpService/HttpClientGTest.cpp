/**
 * @file HttpClientGTest.cpp
 * @brief Source file for class HttpClientGTest
 * @date 05 ott 2018
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
 * the class HttpClientGTest (public, protected, and private). Be aware that some 
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
#include "HttpClientTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(HttpClientGTest, TestConstructor) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(HttpClientGTest, TestInitialise) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(HttpClientGTest, TestInitialise_FalseNoServerAddress) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoServerAddress());
}

TEST(HttpClientGTest, TestInitialise_FalseNoServerPort) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoServerPort());
}

TEST(HttpClientGTest, TestInitialise_FalseNoServerUri) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoServerUri());
}

TEST(HttpClientGTest, TestInitialise_DefaultAuthorization) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultAuthorization());
}

TEST(HttpClientGTest, TestGetHttpProtocol) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestGetHttpProtocol());
}

TEST(HttpClientGTest, TestHttpExchange) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestHttpExchange());
}

TEST(HttpClientGTest, TestSetServerAddress) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestSetServerAddress());
}

TEST(HttpClientGTest, TestSetServerPort) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestSetServerPort());
}

TEST(HttpClientGTest, TestSetServerUri) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestSetServerUri());
}

TEST(HttpClientGTest, TestGetServerAddress) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestGetServerAddress());
}

TEST(HttpClientGTest, TestGetServerPort) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestGetServerPort());
}

TEST(HttpClientGTest, TestGetServerUri) {
    HttpClientTest test;
    ASSERT_TRUE(test.TestGetServerUri());
}
