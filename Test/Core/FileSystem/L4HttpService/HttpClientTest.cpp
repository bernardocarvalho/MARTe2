/**
 * @file HttpClientTest.cpp
 * @brief Source file for class HttpClientTest
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
 * the class HttpClientTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpClientTest.h"
#include "StringHelper.h"
#include "ConfigurationDatabase.h"

#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "StringHelper.h"
#include "MemoryDataSourceI.h"

#include "MemoryMapInputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"

#include "DataExportI.h"
#include "HttpService.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class HttpClientTestObj: public HttpClient {
public:
    HttpClientTestObj();

    virtual ~HttpClientTestObj();

    bool GetReConnect();

    const char8* GetAuthorization();

    const char8* GetAuthorizationKey();

    uint32 GetLastOperationId();

};

HttpClientTestObj::HttpClientTestObj() {

}

HttpClientTestObj::~HttpClientTestObj() {

}

bool HttpClientTestObj::GetReConnect() {
    return reConnect;
}

const char8* HttpClientTestObj::GetAuthorization() {
    return authorisation.Buffer();
}

const char8* HttpClientTestObj::GetAuthorizationKey() {
    return authorisationKey.Buffer();
}

uint32 HttpClientTestObj::GetLastOperationId() {
    return lastOperationId;
}

class HttpClientTestClassTest1: public ReferenceContainer, public DataExportI {
public:
    CLASS_REGISTER_DECLARATION()

HttpClientTestClassTest1    ();

    virtual ~HttpClientTestClassTest1();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol);

    virtual bool GetAsText(StreamI &stream, ProtocolI &protocol);

    virtual int32 GetReplyCode(ProtocolI &data);
};

HttpClientTestClassTest1::HttpClientTestClassTest1() {

}

HttpClientTestClassTest1::~HttpClientTestClassTest1() {

}

bool HttpClientTestClassTest1::GetAsStructuredData(StreamStructuredDataI &data,
                                                   ProtocolI &protocol) {

    protocol.Write("Content-Type", "text/html");
    data.CreateAbsolute("NodeA.NodeB");
    uint32 var1 = 1;
    data.Write("var1", var1);
    data.CreateAbsolute("NodeA.NodeC");
    int32 var2 = -1;
    data.Write("var2", var2);
    data.MoveToRoot();

    return true;
}

bool HttpClientTestClassTest1::GetAsText(StreamI &stream,
                                         ProtocolI &protocol) {
    StreamString hString;
    StreamString *hStream = (&hString);

    hStream->SetSize(0);
    if (!protocol.MoveAbsolute("OutputOptions")) {
        protocol.CreateAbsolute("OutputOptions");
    }
    protocol.Write("Content-Type", "text/html");

    hStream->SetSize(0);

    hStream->Printf("<html><head><TITLE>%s</TITLE>"
                    "</head><BODY BGCOLOR=\"#ffffff\"><H1>%s</H1><UL>",
                    "HttpServiceTestClassTest1", "HttpServiceTestClassTest1");
    hStream->Printf("%s", "</UL></BODY>");
    hStream->Printf("%s", "</html>");
    hStream->Seek(0);
    uint32 stringSize = hStream->Size();
    stream.Write(hStream->Buffer(), stringSize);

    //protocol.WriteHeader(true, HttpDefinition::HSHCReplyOK, hStream, NULL);
    return true;
}

int32 HttpClientTestClassTest1::GetReplyCode(ProtocolI &data) {
    return HttpDefinition::HSHCReplyOK;
}

CLASS_REGISTER(HttpClientTestClassTest1, "1.0")

//#if 0
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
//#endif

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HttpClientTest::HttpClientTest() {
    // Auto-generated constructor stub for HttpClientTest
    // TODO Verify if manual additions are needed
}

HttpClientTest::~HttpClientTest() {
    // Auto-generated destructor stub for HttpClientTest
    // TODO Verify if manual additions are needed
}

bool HttpClientTest::TestConstructor() {
    HttpClientTestObj test;
    bool ret = test.GetReConnect();

    StreamString urlUri;
    test.GetServerUri(urlUri);
    ret &= (urlUri == "");
    StreamString urlHost;
    test.GetServerAddress(urlHost);
    ret &= (urlHost == "");
    ret &= test.GetServerPort() == 0;

    ret &= StringHelper::Compare(test.GetAuthorization(), "") == 0;

    ret &= StringHelper::Compare(test.GetAuthorizationKey(), "") == 0;

    ret &= test.GetLastOperationId() == 0;

    return ret;
}

bool HttpClientTest::TestInitialise() {
    ConfigurationDatabase cdb;

    StreamString address = "127.0.0.1";
    cdb.Write("ServerAddress", address.Buffer());
    uint32 port = 4444;
    cdb.Write("ServerPort", port);
    StreamString uri = "localhost";
    cdb.Write("ServerUri", uri.Buffer());
    StreamString authorization = "1234";
    cdb.Write("Authorization", authorization.Buffer());

    HttpClientTestObj test;
    bool ret = test.Initialise(cdb);

    return ret;
}

bool HttpClientTest::TestInitialise_FalseNoServerAddress() {
    ConfigurationDatabase cdb;

    uint32 port = 4444;
    cdb.Write("ServerPort", port);
    StreamString uri = "localhost";
    cdb.Write("ServerUri", uri.Buffer());
    StreamString authorization = "1234";
    cdb.Write("Authorization", authorization.Buffer());

    HttpClientTestObj test;
    bool ret = !test.Initialise(cdb);

    return ret;
}

bool HttpClientTest::TestInitialise_FalseNoServerPort() {
    ConfigurationDatabase cdb;

    StreamString address = "127.0.0.1";
    cdb.Write("ServerAddress", address.Buffer());
    StreamString uri = "localhost";
    cdb.Write("ServerUri", uri.Buffer());
    StreamString authorization = "1234";
    cdb.Write("Authorization", authorization.Buffer());

    HttpClientTestObj test;
    bool ret = !test.Initialise(cdb);

    return ret;
}

bool HttpClientTest::TestInitialise_FalseNoServerUri() {
    ConfigurationDatabase cdb;

    StreamString address = "127.0.0.1";
    cdb.Write("ServerAddress", address.Buffer());
    uint32 port = 4444;
    cdb.Write("ServerPort", port);
    StreamString authorization = "1234";
    cdb.Write("Authorization", authorization.Buffer());

    HttpClientTestObj test;
    bool ret = !test.Initialise(cdb);

    return ret;
}

bool HttpClientTest::TestInitialise_DefaultAuthorization() {
    ConfigurationDatabase cdb;

    StreamString address = "127.0.0.1";
    cdb.Write("ServerAddress", address.Buffer());
    uint32 port = 4444;
    cdb.Write("ServerPort", port);
    StreamString uri = "localhost";
    cdb.Write("ServerUri", uri.Buffer());

    HttpClientTestObj test;
    bool ret = test.Initialise(cdb);

    return ret;
}

bool HttpClientTest::TestGetHttpProtocol() {
    HttpClientTestObj test;
    HttpProtocol* protocol = test.GetHttpProtocol();
    return (protocol != NULL);
}

bool HttpClientTest::TestHttpExchange() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "       +WebRoot = {"
            "           Class = HttpServiceTestWebRoot"
            "           +ClassLister = {"
            "               Class = HttpServiceTestClassLister"
            "           }"
            "           +Test1 = {"
            "               Class = HttpClientTestClassTest1"
            "           }"
            "       }"
            "       +HttpServerTest = {"
            "           Class = HttpService"
            "           WebRoot = \"Application.WebRoot\""
            "           Port=2222"
            "           ListenMaxConnections = 255"
            "           Timeout = 0"
            "           AcceptTimeout=1000"
            "           MaxNumberOfThreads=100"
            "           MinNumberOfThreads=1"
            "       }"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HttpServiceTestGAM"
            "             InputSignals = {"
            "                Counter = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                    Frequency = 1000"
            "                }"
            "             }"
            "             OutputSignals = {"
            "                CounterOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "                TimeOnDDB = {"
            "                    DataSource = DDB1"
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
            "        +Input = {"
            "            Class = HttpServiceTestDS"
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

    HttpClient test;

    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(2222);
    test.SetServerUri("Test1");

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<HttpService> service = god->Find("Application.HttpServerTest");
    ret = service.IsValid();

    if (ret) {
        ret = service->Start();
    }

    if (ret) {
        StreamString readOut;

        ret=test.HttpExchange(readOut,HttpDefinition::HSHCGet, NULL, 1000u);
        printf("\n%s\n", readOut.Buffer());

    }

    if (ret) {
        ret = service->Stop();
    }
    return ret;
}

bool HttpClientTest::TestSetServerAddress() {
    HttpClientTestObj test;
    StreamString serverAddress;
    test.GetServerAddress(serverAddress);
    bool ret = (serverAddress == "");
    test.SetServerAddress("127.0.0.1");
    test.GetServerAddress(serverAddress);
    ret &= (serverAddress == "127.0.0.1");
    return ret;

}

bool HttpClientTest::TestSetServerPort() {
    HttpClientTestObj test;
    uint32 port = test.GetServerPort();
    bool ret = (port == 0);
    test.SetServerPort(8080);
    port = test.GetServerPort();
    ret &= (port == 8080);
    return ret;
}

bool HttpClientTest::TestSetServerUri() {
    HttpClientTestObj test;
    StreamString serverUri;
    test.GetServerUri(serverUri);
    bool ret = (serverUri == "");
    test.SetServerUri("127.0.0.1");
    test.GetServerUri(serverUri);
    ret &= (serverUri == "127.0.0.1");
    return ret;
}

bool HttpClientTest::TestGetServerAddress() {
    return TestSetServerAddress();
}

bool HttpClientTest::TestGetServerPort() {
    return TestSetServerPort();

}

bool HttpClientTest::TestGetServerUri() {
    return TestSetServerUri();

}

