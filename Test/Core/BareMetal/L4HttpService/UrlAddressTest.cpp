/**
 * @file UrlAddressTest.cpp
 * @brief Source file for class UrlAddressTest
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
 * the class UrlAddressTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UrlAddressTest.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

UrlAddressTest::UrlAddressTest() {
    // Auto-generated constructor stub for UrlAddressTest
    // TODO Verify if manual additions are needed
}

UrlAddressTest::~UrlAddressTest() {
    // Auto-generated destructor stub for UrlAddressTest
    // TODO Verify if manual additions are needed
}

bool UrlAddressTest::TestConstructor() {
    UrlAddress test;

    bool ret = StringHelper::Compare(test.GetServer(), "") == 0;
    ret &= test.GetPort() == 80;
    ret &= StringHelper::Compare(test.GetUri(), "") == 0;
    ret &= test.GetProtocol() == URLP_HTTP;

    return ret;
}

bool UrlAddressTest::TestInit() {
    UrlAddress test;

    test.Init("ftp://www.google.it", "ciaone", 8080, URLP_FTP);

    bool ret = StringHelper::Compare(test.GetServer(), "ftp://www.google.it") == 0;
    ret &= test.GetPort() == 8080;
    ret &= StringHelper::Compare(test.GetUri(), "ciaone") == 0;
    ret &= test.GetProtocol() == URLP_FTP;
    return ret;
}

bool UrlAddressTest::TestLoadStream() {
    UrlAddress test;

    StreamString address = "http://www.google.it:8040/ciaone";
    address.Seek(0);
    bool ret = test.Load(address);

    ret &= StringHelper::Compare(test.GetServer(), "www.google.it") == 0;
    ret &= test.GetPort() == 8040;
    ret &= StringHelper::Compare(test.GetUri(), "ciaone") == 0;
    ret &= test.GetProtocol() == URLP_HTTP;

    if (ret) {
        address = "ftp://www.ftp.it:25/pippo";
        address.Seek(0);
        ret = test.Load(address);
        ret &= StringHelper::Compare(test.GetServer(), "www.ftp.it") == 0;
        ret &= test.GetPort() == 25;
        ret &= StringHelper::Compare(test.GetUri(), "pippo") == 0;
        ret &= test.GetProtocol() == URLP_FTP;
    }

    if (ret) {
        address = "file://home/Desktop/pluto";
        address.Seek(0);
        ret = test.Load(address);
        ret &= StringHelper::Compare(test.GetServer(), "home") == 0;
        ret &= test.GetPort() == 25;
        ret &= StringHelper::Compare(test.GetUri(), "Desktop/pluto") == 0;
        ret &= test.GetProtocol() == URLP_FILE;
    }

    return ret;
}

bool UrlAddressTest::TestLoadString() {
    UrlAddress test;

    bool ret = test.Load("http://www.google.it:8040/ciaone");

    ret &= StringHelper::Compare(test.GetServer(), "www.google.it") == 0;
    ret &= test.GetPort() == 8040;
    ret &= StringHelper::Compare(test.GetUri(), "ciaone") == 0;
    ret &= test.GetProtocol() == URLP_HTTP;

    if (ret) {
        ret = test.Load("ftp://www.ftp.it:25/pippo");
        ret &= StringHelper::Compare(test.GetServer(), "www.ftp.it") == 0;
        ret &= test.GetPort() == 25;
        ret &= StringHelper::Compare(test.GetUri(), "pippo") == 0;
        ret &= test.GetProtocol() == URLP_FTP;
    }

    if (ret) {
        ret = test.Load("file://home/Desktop/pluto");
        ret &= StringHelper::Compare(test.GetServer(), "home") == 0;
        ret &= test.GetPort() == 25;
        ret &= StringHelper::Compare(test.GetUri(), "Desktop/pluto") == 0;
        ret &= test.GetProtocol() == URLP_FILE;
    }

    return ret;
}

bool UrlAddressTest::TestSave() {
    UrlAddress test;

    bool ret = test.Load("http://www.google.it:8040/ciaone");
    if (ret) {
        StreamString res;
        ret = test.Save(res);
        ret &= res == "http://www.google.it:8040/ciaone";
    }

    if (ret) {
        StreamString res;
        ret = test.Load("ftp://www.ftp.it:25/pippo");
        ret &= test.Save(res);
        ret &= res == "ftp://www.ftp.it/pippo";
    }

    if (ret) {
        StreamString res;
        ret = test.Load("file://home/Desktop/pluto");
        ret &= test.Save(res);
        ret &= res == "file:///home/Desktop/pluto";
    }
    return ret;

}

bool UrlAddressTest::TestGetServer() {
    return TestLoadString();
}

bool UrlAddressTest::TestGetUri() {
    return TestLoadString();
}

bool UrlAddressTest::TestGetPort() {
    return TestLoadString();
}

bool UrlAddressTest::TestGetProtocol() {
    return TestLoadString();
}
