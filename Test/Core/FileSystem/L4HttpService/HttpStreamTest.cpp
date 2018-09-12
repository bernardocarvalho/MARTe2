/**
 * @file HttpStreamTest.cpp
 * @brief Source file for class HttpStreamTest
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
 * the class HttpStreamTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpStreamTest.h"
#include "StringHelper.h"
#include "TCPSocket.h"
#include "Threads.h"
#include "GlobalObjectsDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class HttpStreamTestRealm: public HttpRealmI {
public:
    CLASS_REGISTER_DECLARATION();

    HttpStreamTestRealm();
    virtual ~HttpStreamTestRealm();

    virtual bool Validate(const char8 * key,
            HttpDefinition::HSHttpCommand command,
            uint32 ipNumber);

    virtual bool DigestSecurityNeeded();

    virtual bool GetAuthenticationRequest(StreamString &message);

    void SetPassw(const char8* passwIn);

private:
    StreamString passw;
};

HttpStreamTestRealm::HttpStreamTestRealm() {

}

HttpStreamTestRealm::~HttpStreamTestRealm() {

}

void HttpStreamTestRealm::SetPassw(const char8* passwIn) {
    passw = passwIn;
}

bool HttpStreamTestRealm::Validate(const char8 * key,
                                   HttpDefinition::HSHttpCommand command,
                                   uint32 ipNumber) {
    return (passw == key);
}

bool HttpStreamTestRealm::DigestSecurityNeeded() {
    return true;
}

bool HttpStreamTestRealm::GetAuthenticationRequest(StreamString &message) {
    message = passw;
    return true;
}

CLASS_REGISTER(HttpStreamTestRealm, "1.0")
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HttpStreamTest::HttpStreamTest() {
    // Auto-generated constructor stub for HttpStreamTest
    // TODO Verify if manual additions are needed
    eventSem.Create();
    retVal = true;
}

HttpStreamTest::~HttpStreamTest() {
    // Auto-generated destructor stub for HttpStreamTest
    // TODO Verify if manual additions are needed
}

bool HttpStreamTest::TestConstructor() {
    TCPSocket client;

    HttpStream test(client);

    bool ret = test.KeepAlive();
    StreamString path;
    test.GetPath(path);
    ret &= StringHelper::Compare(path.Buffer(), "") == 0;
    StreamString url;
    test.GetPath(url);
    ret &= StringHelper::Compare(url.Buffer(), "") == 0;

    ret &= test.GetHttpCommand() == HSHCNone;

    return ret;
}

static void clientJobGet1(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 4444);
    if (ret) {
        socket.Printf("%s", "GET /docs/index.html HTTP/1.1\n");
        socket.Printf("%s", "Host: www.nowhere123.com\n");
        socket.Printf("%s", "Accept: image/gif, image/jpeg, */*\n");
        socket.Printf("%s", "Accept-Language: en-us\n");
        socket.Printf("%s", "Accept-Encoding: gzip, deflate\n");
        socket.Printf("%s", "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\n");
        socket.Printf("%s", "\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpStreamTest::TestReadHeader_Get1() {
    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobGet1, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    StreamString peer;
    ret &= cdb->Read("Peer", peer);

    if (ret) {
        ret = cdb->MoveAbsolute("InputHttpOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "www.nowhere123.com");
        par.SetSize(0);
        ret &= cdb->Read("Accept", par);
        ret &= (par == "image/gif, image/jpeg, */*");
        par.SetSize(0);

        ret &= cdb->Read("Accept-Language", par);
        ret &= (par == "en-us");
        par.SetSize(0);

        ret &= cdb->Read("User-Agent", par);
        ret &= (par == "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)");
        par.SetSize(0);
    }

    if (ret) {
        StreamString url;
        test.GetUrl(url);
        ret = (url == "docs/index.html");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "docs.index.html");
        printf("path=%s\n", path.Buffer());
    }

    newSocket.Close();
    socket.Close();

    return ret;
}

static void clientJobGet2(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 4444);
    if (ret) {
        socket.Printf("%s", "GET /docs/index.html?index=2&page=3 HTTP/1.1\n");
        socket.Printf("%s", "Host: www.nowhere123.com\n");
        socket.Printf("%s", "Accept: image/gif, image/jpeg, */*\n");
        socket.Printf("%s", "Accept-Language: en-us\n");
        socket.Printf("%s", "Accept-Encoding: gzip, deflate\n");
        socket.Printf("%s", "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\n");
        socket.Printf("%s", "\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpStreamTest::TestReadHeader_Get2_Commands() {
    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobGet2, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    StreamString peer;
    ret &= cdb->Read("Peer", peer);

    if (ret) {
        ret = cdb->MoveAbsolute("InputCommands");
        StreamString par;
        ret &= cdb->Read("index", par);
        ret &= (par == "2");
        par.SetSize(0);
        ret &= cdb->Read("page", par);
        ret &= (par == "3");
        par.SetSize(0);
    }

    if (ret) {
        ret = cdb->MoveAbsolute("InputHttpOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "www.nowhere123.com");
        par.SetSize(0);
        ret &= cdb->Read("Accept", par);
        ret &= (par == "image/gif, image/jpeg, */*");
        par.SetSize(0);

        ret &= cdb->Read("Accept-Language", par);
        ret &= (par == "en-us");
        par.SetSize(0);

        ret &= cdb->Read("User-Agent", par);
        ret &= (par == "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)");
        par.SetSize(0);
    }

    if (ret) {
        StreamString url;
        test.GetUrl(url);
        ret = (url == "docs/index.html");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "docs.index.html");
        printf("path=%s\n", path.Buffer());
    }

    newSocket.Close();
    socket.Close();

    return ret;
}

static void clientJobPut1(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 4444);
    if (ret) {
        socket.Printf("%s", "PUT /user/1234567890 HTTP/1.1\n");
        socket.Printf("%s", "Host: http://sookocheff.com\n");
        socket.Printf("%s", "\n{\n");
        socket.Printf("%s", "\"name\": \"Kevin Sookocheff\"\n");
        socket.Printf("%s", "\"website\": \"http://sookocheff.com\"\n");
        socket.Printf("%s", "}\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpStreamTest::TestReadHeader_Put1() {
    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobPut1, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    StreamString peer;
    ret &= cdb->Read("Peer", peer);

    if (ret) {
        ret = cdb->MoveAbsolute("InputHttpOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "http://sookocheff.com");
        par.SetSize(0);
    }

    if (ret) {
        StreamString url;
        test.GetUrl(url);
        ret = (url == "user/1234567890");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "user.1234567890");
        printf("path=%s\n", path.Buffer());
    }

    newSocket.Close();
    socket.Close();

    return ret;
}

static void clientJobPost1(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 4444);
    if (ret) {
        socket.Printf("%s", "POST /email_verification/ HTTP/1.1\n");
        socket.Printf("%s", "Host: api.evasrv.com\n");
        socket.Printf("%s", "Cache-Control: no-cache\n");
        socket.Printf("%s", "Content-Type: application/x-www-form-urlencoded\n");
        const char8* content =
                "email=username%40domain.com&user_API_token=YOUR-EV-APP-API-TOKEN&free=true&disposable=true&did_you_mean=true&role=true&bad=true&ev_score=true";
        socket.Printf("%s: %d\n\n", "Content-Length", StringHelper::Length(content));
        socket.Printf("%s\n", content);
        socket.Flush();
    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpStreamTest::TestReadHeader_Post1() {
    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobPost1, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        ret = cdb->MoveAbsolute("InputHttpOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "api.evasrv.com");
        par.SetSize(0);
        ret &= cdb->Read("Cache-Control", par);
        ret &= (par == "no-cache");
        par.SetSize(0);
        ret &= cdb->Read("Content-Type", par);
        ret &= (par == "application/x-www-form-urlencoded");
        par.SetSize(0);
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "141");
    }

    if (ret) {
        ret = cdb->MoveAbsolute("InputCommands");
        StreamString par;
        ret &= cdb->Read("rawPost", par);
        ret &=
                (par
                        == "email=username%40domain.com&user_API_token=YOUR-EV-APP-API-TOKEN&free=true&disposable=true&did_you_mean=true&role=true&bad=true&ev_score=true\n");
        par.SetSize(0);
        ret &= cdb->Read("email", par);
        ret &= (par == "username@domain.com");
        par.SetSize(0);
        ret &= cdb->Read("user_API_token", par);
        ret &= (par == "YOUR-EV-APP-API-TOKEN");
        par.SetSize(0);
        ret &= cdb->Read("free", par);
        ret &= (par == "true");
        par.SetSize(0);
        ret &= cdb->Read("disposable", par);
        ret &= (par == "true");
        par.SetSize(0);
        ret &= cdb->Read("did_you_mean", par);
        ret &= (par == "true");
        par.SetSize(0);
        ret &= cdb->Read("role", par);
        ret &= (par == "true");
        par.SetSize(0);
        ret &= cdb->Read("bad", par);
        ret &= (par == "true");
        par.SetSize(0);
        ret &= cdb->Read("ev_score", par);
        ret &= (par == "true");
        par.SetSize(0);
    }

    if (ret) {
        StreamString url;
        test.GetUrl(url);
        ret = (url == "email_verification/");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "email_verification.");
        printf("path=%s\n", path.Buffer());
    }
    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobPost2(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 4444);
    if (ret) {
        socket.Printf("%s", "POST /submit.cgi HTTP/1.1\n");
        socket.Printf("%s", "Host: example.com\n");
        socket.Printf("%s", "User-Agent: curl/7.46.0\n");
        socket.Printf("%s", "Accept: */*\n");
        socket.Printf("%s", "Content-Length: 298\n");
        socket.Printf("%s", "Expect: 100-continue\n");
        socket.Printf("%s", "Content-Type: multipart/form-data; boundary=------------------------d74496d66958873e\n\n");
        socket.Printf("%s", "--------------------------d74496d66958873e\n");
        socket.Printf("%s", "Content-Disposition: form-data; name=\"person\"\n\n");
        socket.Printf("%s", "anonymous\n");
        socket.Printf("%s", "--------------------------d74496d66958873e\n");
        socket.Printf("%s", "Content-Disposition: form-data; name=\"secret\"; filename=\"file.txt\"\n");
        socket.Printf("%s", "Content-Type: text/plain\n\n");
        socket.Printf("%s", "contents of the file\n\n");
        socket.Printf("%s", "--------------------------d74496d66958873e--\n\n");
        socket.Flush();
    }

    StreamString resp;
    char8 term;
    socket.GetToken(resp, "\n", term);
    printf("response=%s\n", resp.Buffer());
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpStreamTest::TestReadHeader_Post2_Multiform() {
    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobPost2, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        ret = cdb->MoveAbsolute("InputHttpOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "example.com");
        par.SetSize(0);
        ret &= cdb->Read("User-Agent", par);
        ret &= (par == "curl/7.46.0");
        par.SetSize(0);
        ret &= cdb->Read("Accept", par);
        ret &= (par == "*/*");
        par.SetSize(0);
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "298");
        par.SetSize(0);
        ret &= cdb->Read("Expect", par);
        ret &= (par == "100-continue");
        par.SetSize(0);
        ret &= cdb->Read("Content-Type", par);
        ret &= (par == "multipart/form-data; boundary=------------------------d74496d66958873e");
    }

    if (ret) {
        ret = cdb->MoveAbsolute("InputCommands");
        StreamString par;
        ret &= cdb->Read("rawPost", par);
        par.SetSize(0);
        ret &= cdb->Read("person", par);
        ret &= (par == "anonymous");
        par.SetSize(0);
        ret &= cdb->Read("secret:filename", par);
        ret &= (par == "file.txt");
        par.SetSize(0);
        ret &= cdb->Read("secret:Content-Type", par);
        ret &= (par == "text/plain");
        par.SetSize(0);
        ret &= cdb->Read("secret", par);
        ret &= (par == "contents of the file");
        par.SetSize(0);
    }
    if (ret) {
        StreamString url;
        test.GetUrl(url);
        ret = (url == "submit.cgi");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "submit.cgi");
        printf("path=%s\n", path.Buffer());
    }

    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobHead(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 4444);
    if (ret) {
        socket.Printf("%s", "HEAD /bit/thinner-archives-vol-1.zip.torrent HTTP/1.1\n");
        socket.Printf("%s", "Host: www.legaltorrents.com\n\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpStreamTest::TestReadHeader_Head() {
    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobHead, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        ret = cdb->MoveAbsolute("InputHttpOptions");
        StreamString par;
        ret &= cdb->Read("Host", par);
        ret &= (par == "www.legaltorrents.com");
    }
    if (ret) {
        StreamString url;
        test.GetUrl(url);
        ret = (url == "bit/thinner-archives-vol-1.zip.torrent");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "bit.thinner-archives-vol-1.zip.torrent");
        printf("path=%s\n", path.Buffer());
    }

    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobReply(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 4444);
    if (ret) {
        socket.Printf("%s", "HTTP/1.1 200 OK\n");
        socket.Printf("%s", "Date: Mon, 27 Jul 2009 12:28:53 GMT\n");
        socket.Printf("%s", "Server: Apache/2.2.14 (Win32)\n");
        socket.Printf("%s", "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n");
        socket.Printf("%s", "Content-Length: 53\n");
        socket.Printf("%s", "Content-Type: text/html\n");
        socket.Printf("%s", "Connection: Closed\n\n");
        socket.Printf("%s", "<html>\n");
        socket.Printf("%s", "<body>\n");
        socket.Printf("%s", "<h1>Hello, World!</h1>\n");
        socket.Printf("%s", "</body>\n");
        socket.Printf("%s", "</html>\n");
        socket.Flush();
    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpStreamTest::TestReadHeader_Reply() {
    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobReply, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        ret = cdb->MoveAbsolute("InputHttpOptions");
        StreamString par;
        ret &= cdb->Read("Date", par);
        ret &= (par == "Mon, 27 Jul 2009 12:28:53 GMT");
        par.SetSize(0);
        ret &= cdb->Read("Server", par);
        ret &= (par == "Apache/2.2.14 (Win32)");
        par.SetSize(0);
        ret &= cdb->Read("Last-Modified", par);
        ret &= (par == "Wed, 22 Jul 2009 19:15:56 GMT");
        par.SetSize(0);
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "53");
        par.SetSize(0);
        ret &= cdb->Read("Content-Type", par);
        ret &= (par == "text/html");
        par.SetSize(0);
        ret &= cdb->Read("Connection", par);
        ret &= (par == "Closed");
        par.SetSize(0);
    }
    if (ret) {
        StreamString url;
        test.GetUrl(url);
        ret = (url == "");
        printf("url=%s\n", url.Buffer());
    }

    if (ret) {
        StreamString path;
        test.GetPath(path);
        ret = (path == "");
        printf("path=%s\n", path.Buffer());
    }

    newSocket.Close();
    socket.Close();
    return ret;

}

bool HttpStreamTest::TestCompleteReadOperation() {
    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobReply, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained);
    }

    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    printf("remained %s\n", remained.Buffer());
    if (ret) {
        ret = (remained == "<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>\n");
    }
    newSocket.Close();
    socket.Close();
    return ret;
}

static void clientJobWrite(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    tt.retVal = socket.Connect("127.0.0.1", 4444);
    if (tt.retVal) {
        HttpStream test(socket);
        AnyType at1[] = { "application/x-www-form-urlencoded" };
        test.SwitchPrintAndCommit("OutputHttpOtions", "Content-Type", "%s", at1);
        test.SetSize(0);
        test.Printf("%s", "ciaobellooo\n");
        tt.retVal = test.WriteHeader(true, tt.command, "localhost");
    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

static void clientJobWrite2(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4444, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    tt.retVal = socket.Connect("127.0.0.1", 4444);
    if (tt.retVal) {
        HttpStream test(socket);
        AnyType at[] = { "chunked" };
        test.SwitchPrintAndCommit("OutputHttpOtions", "Transfer-Encoding", "%s", at);
        AnyType at1[] = { "application/x-www-form-urlencoded" };
        test.SwitchPrintAndCommit("OutputHttpOtions", "Content-Type", "%s", at1);
        test.SetSize(0);
        test.Printf("%s", "ciao");
        tt.retVal = test.WriteHeader(false, tt.command, "localhost");
        tt.eventSem.Wait();
        tt.eventSem.Reset();
        test.Switch("OutputHttpOtions");
        test.DeleteStream("Transfer-Encoding");
        test.Switch("");

        test.SetSize(0);
        test.Printf("%s", "bellooo\n");
        tt.retVal = test.WriteHeader(true, tt.command, "localhost");
    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

static void clientJobWriteStructuredStored(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4444, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    tt.retVal = socket.Connect("127.0.0.1", 4444);
    if (tt.retVal) {
        HttpStream test(socket);
        AnyType at[] = { "chunked" };
        test.SwitchPrintAndCommit("OutputHttpOtions", "Transfer-Encoding", "%s", at);
        AnyType at1[] = { "application/x-www-form-urlencoded" };
        test.SwitchPrintAndCommit("OutputHttpOtions", "Content-Type", "%s", at1);
        test.SetSize(0);
        test.Printf("%s", "ciao");
        tt.retVal = test.WriteHeader(false, tt.command, "localhost");
        tt.eventSem.Wait();
        tt.eventSem.Reset();
        test.Switch("OutputHttpOtions");
        test.DeleteStream("Transfer-Encoding");
        test.Switch("");

        test.SetSize(0);
        test.Printf("%s", "bellooo\n");

        test.CreateAbsolute("A.B.C");
        test.CreateAbsolute("A.D");
        test.CreateAbsolute("A.E.F");
        test.MoveAbsolute("A.B.C");
        uint32 var1 = 1u;
        test.Write("var1", var1);

        test.MoveAbsolute("A.D");
        uint32 var2 = 2u;
        test.Write("var2", var2);

        test.MoveAbsolute("A.E.F");
        uint32 var3 = 3u;
        test.Write("var3", var3);

        //test.WriteStructuredDataOnSocket();

        tt.retVal = test.WriteHeader(true, tt.command, "localhost");

    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpStreamTest::TestWriteHeader_StrucuredDataStored() {
    retVal = true;
    eventSem.Reset();
    command = HSHCReplyOK;
    InternetHost source(4444, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    Threads::BeginThread((ThreadFunctionType) clientJobWriteStructuredStored, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();

    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained, 10);
    }

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());
    /*
     if (ret) {
     ret = cdb->MoveAbsolute("InputHttpOptions");
     StreamString par;
     ret &= cdb->Read("Transfer-Encoding", par);
     ret &= (par == "chunked");
     par.SetSize(0);
     }
     */
    eventSem.Post();

    StreamString remained2;
    if (ret) {
        ret = test.ReadHeader();
        if (ret) {
            ret = test.CompleteReadOperation(&remained2);
        }
    }

    eventSem.Wait();

    cdb->MoveToRoot();
    StreamString output2;
    output2.Printf("%@", *cdb);
    printf("%s\n", output2.Buffer());

    printf("%s\n", remained.Buffer());
    printf("|%s||%s|\n", remained2.Buffer(), "bellooo\n"
           "\"A\": {\r\n"
           "    \"B\": {\r\n"
           "        \"C\": {\r\n"
           "            \"var1\": +1\r\n"
           "        }\r\n"
           "    },\r\n"
           "    \"D\": {\r\n"
           "        \"var2\": +2\r\n"
           "    },\r\n"
           "    \"E\": {\r\n"
           "        \"F\": {\r\n"
           "            \"var3\": +3\r\n"
           "        }\r\n"
           "    }\r\n"
           "}\r\n");

    if (ret) {
        ret = cdb->MoveAbsolute("InputHttpOptions");
        StreamString par;
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "200");
        par.SetSize(0);
    }
    if (ret) {

        ret = (remained == "ciao");
        if (ret) {
            ret = (remained2 == "bellooo\n"
                    "\"A\": {\r\n"
                    "    \"B\": {\r\n"
                    "        \"C\": {\r\n"
                    "            \"var1\": +1\r\n"
                    "        }\r\n"
                    "    },\r\n"
                    "    \"D\": {\r\n"
                    "        \"var2\": +2\r\n"
                    "    },\r\n"
                    "    \"E\": {\r\n"
                    "        \"F\": {\r\n"
                    "            \"var3\": +3\r\n"
                    "        }\r\n"
                    "    }\r\n"
                    "}\r\n");
        }
    }
    newSocket.Close();
    socket.Close();
    return (ret && retVal);
}

static void clientJobWriteStructuredOnline(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4444, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    tt.retVal = socket.Connect("127.0.0.1", 4444);
    if (tt.retVal) {
        HttpStream test(socket, false);
        AnyType at[] = { "chunked" };
        test.SwitchPrintAndCommit("OutputHttpOtions", "Transfer-Encoding", "%s", at);
        AnyType at1[] = { "application/x-www-form-urlencoded" };
        test.SwitchPrintAndCommit("OutputHttpOtions", "Content-Type", "%s", at1);
        test.SetSize(0);
        test.Printf("%s", "ciao");
        tt.retVal = test.WriteHeader(false, tt.command, "localhost");
        tt.eventSem.Wait();
        tt.eventSem.Reset();
        test.Switch("OutputHttpOtions");
        test.DeleteStream("Transfer-Encoding");
        test.Switch("");

        test.SetSize(0);
        test.Printf("%s", "bellooo");

        tt.retVal = test.WriteHeader(false, tt.command, "localhost");
        test.CreateAbsolute("A.B.C");
        uint32 var1 = 1u;
        test.Write("var1", var1);
        test.CreateAbsolute("A.D");
        uint32 var2 = 2u;
        test.Write("var2", var2);
        test.CreateAbsolute("A.E.F");
        uint32 var3 = 3u;
        test.Write("var3", var3);
        test.MoveToRoot();

    }

    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpStreamTest::TestWriteHeader_StrucuredDataOnline() {
    retVal = true;
    eventSem.Reset();
    command = HSHCReplyOK;
    InternetHost source(4444, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    Threads::BeginThread((ThreadFunctionType) clientJobWriteStructuredOnline, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();

    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained, 10);
    }

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());
    /*
     if (ret) {
     ret = cdb->MoveAbsolute("InputHttpOptions");
     StreamString par;
     ret &= cdb->Read("Transfer-Encoding", par);
     ret &= (par == "chunked");
     par.SetSize(0);
     }
     */
    eventSem.Post();

    StreamString remained2;
    if (ret) {
        ret = test.ReadHeader();
        if (ret) {
            ret = test.CompleteReadOperation(&remained2);
        }
    }

    eventSem.Wait();

    cdb->MoveToRoot();
    StreamString output2;
    output2.Printf("%@", *cdb);
    printf("%s\n", output2.Buffer());

    printf("%s\n", remained.Buffer());
    printf("|%s||%s|\n", remained2.Buffer(), "bellooo\n\r"
           "\"A\": {\n\r"
           "\"B\": {\n\r"
           "\"C\": {\n\r"
           "\"var1\": +1\n\r"
           "}\n\r"
           "},\n\r"
           "\"D\": {\n\r"
           "\"var2\": +2\n\r"
           "},\r\n"
           "\"E\": {\n\r"
           "\"F\": {\n\r"
           "\"var3\": +3\n\r"
           "}\n\r"
           "}\n\r"
           "}");

    if (ret) {

        ret = (remained == "ciao");
        if (ret) {
            ret = (remained2 == "bellooo\n\r"
                    "\"A\": {\n\r"
                    "\"B\": {\n\r"
                    "\"C\": {\n\r"
                    "\"var1\": +1\n\r"
                    "}\n\r"
                    "},\n\r"
                    "\"D\": {\n\r"
                    "\"var2\": +2\n\r"
                    "},\n\r"
                    "\"E\": {\n\r"
                    "\"F\": {\n\r"
                    "\"var3\": +3\n\r"
                    "}\n\r"
                    "}\n\r"
                    "}");
        }
    }

    newSocket.Close();
    socket.Close();
    return (ret && retVal);
}

bool HttpStreamTest::TestWriteHeader() {

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobWrite, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained);
    }

    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    printf("%s\n", remained.Buffer());
    if (command != HSHCPost) {
        if (ret) {
            ret = (remained == "ciaobellooo\n");
        }
    }
    if (ret) {
        ret = cdb->MoveAbsolute("InputHttpOptions");
        StreamString par;
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "12");
        par.SetSize(0);

        ret &= cdb->Read("Content-Type", par);
        ret &= (par == "application/x-www-form-urlencoded");
        par.SetSize(0);
    }
    newSocket.Close();
    socket.Close();
    return (ret && retVal);

}

bool HttpStreamTest::TestWriteHeader2() {
    InternetHost source(4444, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    Threads::BeginThread((ThreadFunctionType) clientJobWrite2, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();

    StreamString remained;
    if (ret) {
        ret = test.CompleteReadOperation(&remained, 10);
    }

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());
    if (ret) {
        ret = cdb->MoveAbsolute("InputHttpOptions");
        StreamString par;
        ret &= cdb->Read("Transfer-Encoding", par);
        ret &= (par == "chunked");
        par.SetSize(0);
    }

    eventSem.Post();

    StreamString remained2;
    if (ret) {
        ret = test.ReadHeader();
        if (ret) {
            ret = test.CompleteReadOperation(&remained2);
        }
    }

    eventSem.Wait();

    cdb->MoveToRoot();
    StreamString output2;
    output2.Printf("%@", *cdb);
    printf("%s\n", output2.Buffer());

    printf("%s\n", remained.Buffer());
    printf("%s\n", remained2.Buffer());
    if (ret) {
        ret = cdb->MoveAbsolute("InputHttpOptions");
        StreamString par;
        ret &= cdb->Read("Content-Length", par);
        ret &= (par == "8");
        par.SetSize(0);
    }
    if (command != HSHCPost) {

        if (ret) {
            ret = (remained == "ciao");
        }
        if (ret) {
            ret = (remained2 == "bellooo\n");
        }
    }

    newSocket.Close();
    socket.Close();
    return (ret && retVal);

}

bool HttpStreamTest::TestWriteHeader_Get1() {
    retVal = true;
    eventSem.Reset();
    command = HSHCGet;
    return TestWriteHeader();
}

bool HttpStreamTest::TestWriteHeader_Get2() {
    retVal = true;
    eventSem.Reset();
    command = HSHCGet;
    return TestWriteHeader2();
}

bool HttpStreamTest::TestWriteHeader_Put1() {
    retVal = true;
    eventSem.Reset();
    command = HSHCPut;
    return TestWriteHeader();

}

bool HttpStreamTest::TestWriteHeader_Put2() {
    retVal = true;
    eventSem.Reset();
    command = HSHCPut;
    return TestWriteHeader2();

}

bool HttpStreamTest::TestWriteHeader_Head1() {
    retVal = true;
    eventSem.Reset();
    command = HSHCHead;
    return TestWriteHeader();

}

bool HttpStreamTest::TestWriteHeader_Head2() {
    retVal = true;
    eventSem.Reset();
    command = HSHCHead;
    return TestWriteHeader2();

}

bool HttpStreamTest::TestWriteHeader_Post() {
    retVal = true;
    eventSem.Reset();
    command = HSHCPost;
    return TestWriteHeader();

}

bool HttpStreamTest::TestWriteHeader_Reply() {
    retVal = true;
    eventSem.Reset();
    command = HSHCReplyOK;
    return TestWriteHeader();
}

bool HttpStreamTest::TestWriteHeader_Reply2() {
    retVal = true;
    eventSem.Reset();
    command = HSHCReplyOK;
    return TestWriteHeader2();
}

static void clientJobAuth(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 4444);
    if (ret) {
        socket.Printf("%s", "GET /docs/index.html HTTP/1.1\n");
        socket.Printf("%s", "Host: www.nowhere123.com\n");
        socket.Printf("%s", "Authorization: admin1234\n");
        socket.Printf("%s", "\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpStreamTest::TestSecurityCheck() {
    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    ReferenceT<HttpStreamTestRealm> realm(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    realm->SetPassw("admin1234");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobAuth, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    if (ret) {
        ret = test.SecurityCheck(realm);
    }
    if (ret) {
        realm->SetPassw("invalid");
        ret = !test.SecurityCheck(realm);
    }

    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    newSocket.Close();
    socket.Close();

    return ret;
}

bool HttpStreamTest::TestKeepAliveDefault() {
    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobGet1, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        ret = test.KeepAlive();
    }

    newSocket.Close();
    socket.Close();

    return ret;
}

static void clientJobKeepAlive(HttpStreamTest &tt) {
    //tells to the main process that the thread begins

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    tt.eventSem.Wait();
    tt.eventSem.Reset();
    bool ret = socket.Connect("127.0.0.1", 4444);
    if (ret) {
        socket.Printf("%s", "GET /docs/index.html HTTP/1.1\n");
        socket.Printf("%s", "Host: www.nowhere123.com\n");
        if (tt.isKeepAlive) {
            socket.Printf("%s", "Connection: keep-alive\n");
        }
        else {
            socket.Printf("%s", "Connection: closed\n");
        }
        socket.Printf("%s", "\n");
        socket.Flush();
    }
    tt.eventSem.Post();
    socket.Close();
    Threads::EndThread();
}

bool HttpStreamTest::TestKeepAlive(bool isKeepAliveIn) {
    isKeepAlive = isKeepAliveIn;

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobKeepAlive, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        ret = (test.KeepAlive() == isKeepAlive);
    }

    newSocket.Close();
    socket.Close();

    return ret;
}

bool HttpStreamTest::TestSetKeepAlive(bool isKeepAlive) {
    TCPSocket newSocket;

    HttpStream test(newSocket);
    test.SetKeepAlive(isKeepAlive);
    return test.KeepAlive() == isKeepAlive;
}

bool HttpStreamTest::TestGetHttpCommand(HSHttpCommand commandIn) {

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    if (commandIn == HSHCGet) {
        Threads::BeginThread((ThreadFunctionType) clientJobGet1, this);
    }
    else if (commandIn == HSHCPut) {
        Threads::BeginThread((ThreadFunctionType) clientJobPut1, this);
    }
    else if (commandIn == HSHCHead) {
        Threads::BeginThread((ThreadFunctionType) clientJobHead, this);
    }
    else if (commandIn == HSHCPost) {
        Threads::BeginThread((ThreadFunctionType) clientJobPost1, this);
    }
    else if (commandIn == HSHCReply) {
        Threads::BeginThread((ThreadFunctionType) clientJobReply, this);
    }
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        if (commandIn == HSHCReply) {
            ret = test.GetHttpCommand() == (commandIn + 200);
        }
        else {
            ret = test.GetHttpCommand() == commandIn;
        }
    }
    newSocket.Close();
    socket.Close();

    return ret;

}

bool HttpStreamTest::TestSetUnmatchedUrl() {
    TCPSocket newSocket;
    HttpStream test(newSocket);
    test.SetUnmatchedUrl("pippo");
    StreamString ret;
    test.GetUnmatchedUrl(ret);
    return ret == "pippo";
}

bool HttpStreamTest::TestGetUnmatchedUrl() {
    //normally it is the url

    InternetHost source(4443, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;
    socket.SetSource(source);
    socket.SetDestination(destination);

    socket.Open();
    socket.Listen(4444, 255);
    //todo launch a thread with the client request
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) clientJobGet1, this);
    TCPSocket newSocket;

    eventSem.Post();

    socket.WaitConnection(TTInfiniteWait, &newSocket);

    HttpStream test(newSocket);
    bool ret = test.ReadHeader();
    eventSem.Wait();

    ConfigurationDatabase *cdb = test.GetData();

    cdb->MoveToRoot();
    StreamString output;
    output.Printf("%@", *cdb);
    printf("%s\n", output.Buffer());

    if (ret) {
        StreamString unmatchedUrl;
        test.GetUnmatchedUrl(unmatchedUrl);
        StreamString url;
        test.GetUnmatchedUrl(url);
        ret = (unmatchedUrl == url);
    }

    newSocket.Close();
    socket.Close();

    return (ret && TestSetUnmatchedUrl());
}

bool HttpStreamTest::TestGetPath() {
    return TestReadHeader_Get1();
}

bool HttpStreamTest::TestGetUrl() {
    return TestReadHeader_Get1();
}

