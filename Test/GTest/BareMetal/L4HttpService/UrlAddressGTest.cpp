/**
 * @file UrlAddressGTest.cpp
 * @brief Source file for class UrlAddressGTest
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
 * the class UrlAddressGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/



#include "UrlAddressTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UrlAddressGTest,TestConstructor) {
    UrlAddressTest target;
    ASSERT_TRUE(target.TestConstructor());
}

TEST(UrlAddressGTest,TestInit) {
    UrlAddressTest target;
    ASSERT_TRUE(target.TestInit());
}
TEST(UrlAddressGTest,TestLoadStream) {
    UrlAddressTest target;
    ASSERT_TRUE(target.TestLoadStream());
}
TEST(UrlAddressGTest,TestLoadString) {
    UrlAddressTest target;
    ASSERT_TRUE(target.TestLoadString());
}
TEST(UrlAddressGTest,TestSave) {
    UrlAddressTest target;
    ASSERT_TRUE(target.TestSave());
}
TEST(UrlAddressGTest,TestGetServer) {
    UrlAddressTest target;
    ASSERT_TRUE(target.TestGetServer());
}
TEST(UrlAddressGTest,TestGetUri) {
    UrlAddressTest target;
    ASSERT_TRUE(target.TestGetUri());
}
TEST(UrlAddressGTest,TestGetPort) {
    UrlAddressTest target;
    ASSERT_TRUE(target.TestGetPort());
}
TEST(UrlAddressGTest,TestGetProtocol) {
    UrlAddressTest target;
    ASSERT_TRUE(target.TestGetProtocol());
}
