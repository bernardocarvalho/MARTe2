/**
 * @file HttpObjectBrowserTest.cpp
 * @brief Source file for class HttpObjectBrowserTest
 * @date 18/03/2019
 * @author Andre Neto
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
 * the class HttpObjectBrowserTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HttpClient.h"
#include "HttpObjectBrowser.h"
#include "HttpObjectBrowserTest.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class HttpObjectBrowserTestRealm: public MARTe::Object, public MARTe::HttpRealmI {
public:
    CLASS_REGISTER_DECLARATION();

    HttpObjectBrowserTestRealm();
    virtual ~HttpObjectBrowserTestRealm();

    virtual bool Validate(const MARTe::char8 * key, MARTe::int32 command, MARTe::uint32 ipNumber);

    virtual bool DigestSecurityNeeded();

    virtual bool GetAuthenticationRequest(MARTe::StreamString &message);

    bool valid;
};

HttpObjectBrowserTestRealm::HttpObjectBrowserTestRealm() {
    valid = false;
}

HttpObjectBrowserTestRealm::~HttpObjectBrowserTestRealm() {

}

bool HttpObjectBrowserTestRealm::Validate(const MARTe::char8 * key, MARTe::int32 command, MARTe::uint32 ipNumber) {
    return valid;
}

bool HttpObjectBrowserTestRealm::DigestSecurityNeeded() {
    return true;
}

bool HttpObjectBrowserTestRealm::GetAuthenticationRequest(MARTe::StreamString &message) {
    return true;
}

CLASS_REGISTER(HttpObjectBrowserTestRealm, "1.0")
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool HttpObjectBrowserTest::TestInitialise() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Root", ".");

    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_ObjectRegistryDatabase_Root() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Root", "/");

    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_Self_Root() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Root", ".");

    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_Other_Root() {
    using namespace MARTe;
    StreamString cfg = ""
            "+ARoot = {\n"
            "    Class = ReferenceContainer\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = ARoot"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_False_No_Root() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = !httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_False_Bad_Root_1() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Root", "?");

    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = !httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_False_Bad_Root_2() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Root", "HttpObjectBrowserTestARoot");

    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = !httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_Realm() {
    using namespace MARTe;
    StreamString cfg = ""
            "+ARoot = {\n"
            "    Class = ReferenceContainer\n"
            "}\n"
            "+ARealm = {\n"
            "    Class = HttpObjectBrowserTestRealm\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = ARoot\n"
            "    Realm = ARealm\n"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_False_Realm() {
    using namespace MARTe;
    StreamString cfg = ""
            "+ARoot = {\n"
            "    Class = ReferenceContainer\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = ARoot\n"
            "    Realm = ARoot\n"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = !ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

#include <stdio.h>
bool HttpObjectBrowserTest::TestGetAsStructuredData_This_Self() {
    using namespace MARTe;
    StreamString cfg = ""
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    MinNumberOfThreads 2\n"
            "    MaxNumberOfThreads = 8\n"
            "    WebRoot = HttpObjectBrowser1\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = ."
            "    +AChild = {\n"
            "        Class = ReferenceContainer\n"
            "    }\n"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }

    HttpClient test;
    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("?TextMode=0");
    StreamString readData;
    test.HttpExchange(readData, HttpDefinition::HSHCGet, NULL, 20000u);
    printf("[%s]\n", readData.Buffer());
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}
