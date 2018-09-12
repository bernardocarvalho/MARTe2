/**
 * @file HttpServiceTest.cpp
 * @brief Source file for class HttpServiceTest
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
 * the class HttpServiceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpServiceTest.h"
#include "ConfigurationDatabase.h"

#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class HttpServiceTestService: public HttpService {
public:

    HttpServiceTestService();

    virtual ~HttpServiceTestService();

    uint32 GetPort();

    const char8 *GetHttpRelayURL();

    ReferenceT<ReferenceContainer> GetWebRoot();

    const char8 *GetWebRootPath();

    uint32 GetListenMaxConnections();
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
uint32 HttpServiceTestService::GetPort() {
    return port;
}

const char8 *HttpServiceTestService::GetHttpRelayURL() {
    return httpRelayURL.Buffer();
}

ReferenceT<ReferenceContainer> HttpServiceTestService::GetWebRoot() {
    return webRoot;
}

const char8 *HttpServiceTestService::GetWebRootPath() {
    return webRootPath.Buffer();
}

uint32 HttpServiceTestService::GetListenMaxConnections() {
    return listenMaxConnections;
}

#if 0
/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapInputBrokerEnviroment(const char8 * const config) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Application");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}
#endif

const char8 *config = ""
        "$Application = {"
        "   Class = RealTimeApplication"
        "   +Functions = {"
        "       Class = ReferenceContainer"
        "       WebRoot = {"
        "           Class = ReferenceContainer"
        "       }"
        "       +HttpServerTest = {"
        "           Class = HttpService"
        "           WebRoot"
        "       }"
        "       +GAM1 = {"
        "           Class = IOGAM"
        "             InputSignals = {"
        "                Counter = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                Time = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                    Frequency = 1000"
        "                }"
        "             }"
        "             OutputSignals = {"
        "                CounterOnDDB = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "                TimeOnDDB = {"
        "                    DataSource = Timer"
        "                    Type = uint32"
        "                }"
        "             }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +LoggerDataSource = {"
        "            Class = LoggerDataSource"
        "        }"
        "        +Timer = {"
        "            Class = LinuxTimer"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +Idle = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    CPUs = 2"
        "                    Functions = { GAM1 }"
        "                }"
        "            }"
        "         }"
        "     }"
        "     +Scheduler = {"
        "         Class = GAMScheduler"
        "         TimingDataSource = Timings"
        "     }"
        "}";

HttpServiceTestService::HttpServiceTestService() :
        HttpService() {

}

HttpServiceTestService::~HttpServiceTestService() {

}

HttpServiceTest::HttpServiceTest() {
    // Auto-generated constructor stub for HttpServiceTest
    // TODO Verify if manual additions are needed
}

HttpServiceTest::~HttpServiceTest() {
    // Auto-generated destructor stub for HttpServiceTest
    // TODO Verify if manual additions are needed
}

bool HttpServiceTest::TestConstructor() {
    HttpServiceTestService httpserver;
    bool ret = (httpserver.GetPort() == 0u);

    ret &= (StringHelper::Compare(httpserver.GetHttpRelayURL(), "") == 0);
    ret &= (StringHelper::Compare(httpserver.GetWebRootPath(), "") == 0);
    ret &= httpserver.GetListenMaxConnections() == 0;
    ret &= !httpserver.GetWebRoot().IsValid();
    return ret;

}

bool HttpServiceTest::TestInitialise() {
    StreamString configStream = "+WebRoot = {\n"
            "Class = ReferenceContainer\n"
            "}\n"
            "+HttpServerTest = {\n"
            "Class = HttpService\n"
            "WebRoot=\"WebRoot\"\n"
            "Port=4444\n"
            "ListenMaxConnections = 3\n"
            "HttpRelayURL = \"www.google.it\"\n"
            "Timeout = 0\n"
            "MaxNumberOfThreads=100\n"
            "MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    HttpServiceTestService test;

    if (ret) {
        ret = cdb.MoveAbsolute("+HttpServerTest");
        if (ret) {
            ret = test.Initialise(cdb);
        }
        if (ret) {
            ret = test.GetPort() == 4444;
        }
        ret &= (StringHelper::Compare(test.GetHttpRelayURL(), "www.google.it") == 0);
        ret &= (StringHelper::Compare(test.GetWebRootPath(), "WebRoot") == 0);
        ret &= test.GetListenMaxConnections() == 3;

        ret &= !test.GetWebRoot().IsValid();
    }

    return ret;
}

bool HttpServiceTest::TestInitialise_DefaultWebRoot() {
    StreamString configStream = "+HttpServerTest = {\n"
            "Class = HttpService\n"
            "Port=4444\n"
            "ListenMaxConnections = 3\n"
            "HttpRelayURL = \"www.google.it\"\n"
            "Timeout = 0\n"
            "MaxNumberOfThreads=100\n"
            "MinNumberOfThreads=1\n"
            "+WebRoot = {\n"
            "Class = ReferenceContainer\n"
            "}\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    HttpServiceTestService test;

    if (ret) {
        ret = cdb.MoveAbsolute("+HttpServerTest");
        if (ret) {
            ret = test.Initialise(cdb);
        }
        if (ret) {
            ret = test.GetPort() == 4444;
        }
        ret &= (StringHelper::Compare(test.GetHttpRelayURL(), "www.google.it") == 0);
        ret &= (StringHelper::Compare(test.GetWebRootPath(), "") == 0);
        ret &= test.GetListenMaxConnections() == 3;

        ret &= test.GetWebRoot() == test.Get(0);
    }

    return ret;
}

bool HttpServiceTest::TestInitialise_FalseNoDefaultWebRoot() {
    StreamString configStream = "+HttpServerTest = {\n"
            "Class = HttpService\n"
            "Port=4444\n"
            "ListenMaxConnections = 3\n"
            "HttpRelayURL = \"www.google.it\"\n"
            "Timeout = 0\n"
            "MaxNumberOfThreads=100\n"
            "MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    HttpServiceTestService test;

    if (ret) {
        ret = cdb.MoveAbsolute("+HttpServerTest");
        if (ret) {
            ret = !test.Initialise(cdb);
        }
    }
    return ret;
}

bool HttpServiceTest::TestInitialise_DefaultNListenConnections() {
    StreamString configStream = "+WebRoot = {\n"
            "Class = ReferenceContainer\n"
            "}\n"
            "+HttpServerTest = {\n"
            "Class = HttpService\n"
            "WebRoot=\"WebRoot\"\n"
            "Port=4444\n"
            "HttpRelayURL = \"www.google.it\"\n"
            "Timeout = 0\n"
            "MaxNumberOfThreads=100\n"
            "MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    HttpServiceTestService test;

    if (ret) {
        ret = cdb.MoveAbsolute("+HttpServerTest");
        if (ret) {
            ret = test.Initialise(cdb);
        }
        if (ret) {
            ret = test.GetPort() == 4444;
        }
        ret &= (StringHelper::Compare(test.GetHttpRelayURL(), "www.google.it") == 0);
        ret &= (StringHelper::Compare(test.GetWebRootPath(), "WebRoot") == 0);
        ret &= test.GetListenMaxConnections() == 255;

        ret &= !test.GetWebRoot().IsValid();
    }

    return ret;
}

bool HttpServiceTest::TestInitialise_DefaultRelyUrl() {
    StreamString configStream = "+WebRoot = {\n"
            "Class = ReferenceContainer\n"
            "}\n"
            "+HttpServerTest = {\n"
            "Class = HttpService\n"
            "WebRoot=\"WebRoot\"\n"
            "Port=4444\n"
            "ListenMaxConnections = 3\n"
            "Timeout = 0\n"
            "MaxNumberOfThreads=100\n"
            "MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    HttpServiceTestService test;

    if (ret) {
        ret = cdb.MoveAbsolute("+HttpServerTest");
        if (ret) {
            ret = test.Initialise(cdb);
        }
        if (ret) {
            ret = test.GetPort() == 4444;
        }
        ret &= (StringHelper::Compare(test.GetHttpRelayURL(), "http://localhost:8080") == 0);
        ret &= (StringHelper::Compare(test.GetWebRootPath(), "WebRoot") == 0);
        ret &= test.GetListenMaxConnections() == 3;

        ret &= !test.GetWebRoot().IsValid();
    }

    return ret;
}

bool HttpServiceTest::TestInitialise_DefaultPort() {
    StreamString configStream = "+WebRoot = {\n"
            "Class = ReferenceContainer\n"
            "}\n"
            "+HttpServerTest = {\n"
            "Class = HttpService\n"
            "WebRoot=\"WebRoot\"\n"
            "ListenMaxConnections = 3\n"
            "HttpRelayURL = \"www.google.it\"\n"
            "Timeout = 0\n"
            "MaxNumberOfThreads=100\n"
            "MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    HttpServiceTestService test;

    if (ret) {
        ret = cdb.MoveAbsolute("+HttpServerTest");
        if (ret) {
            ret = test.Initialise(cdb);
        }
        if (ret) {
            ret = test.GetPort() == 80;
        }
        ret &= (StringHelper::Compare(test.GetHttpRelayURL(), "www.google.it") == 0);
        ret &= (StringHelper::Compare(test.GetWebRootPath(), "WebRoot") == 0);
        ret &= test.GetListenMaxConnections() == 3;

        ret &= !test.GetWebRoot().IsValid();
    }

    return ret;
}

bool HttpServiceTest::TestServerCycle() {
    return true;
}

bool HttpServiceTest::TestClientService() {
    return true;
}
