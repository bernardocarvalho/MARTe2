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
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HttpStreamTest::HttpStreamTest() {
    // Auto-generated constructor stub for HttpStreamTest
    // TODO Verify if manual additions are needed
    eventSem.Create();
}

HttpStreamTest::~HttpStreamTest() {
    // Auto-generated destructor stub for HttpStreamTest
    // TODO Verify if manual additions are needed
}

bool HttpStreamTest::TestConstructor() {
    TCPSocket client;

    HttpStream test(client);

    bool ret = test.KeepAlive();
    ret &= StringHelper::Compare(test.GetPath(), "") == 0;
    ret &= StringHelper::Compare(test.GetUrl(), "") == 0;

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
        StreamString url = test.GetUrl();
        ret = (url == "docs/index.html");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path = test.GetPath();
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
        StreamString url = test.GetUrl();
        ret = (url == "docs/index.html");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path = test.GetPath();
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
        StreamString url = test.GetUrl();
        ret = (url == "user/1234567890");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path = test.GetPath();
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
        StreamString url = test.GetUrl();
        ret = (url == "email_verification/");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path = test.GetPath();
        ret = (path == "email_verification.");
        printf("path=%s\n", path.Buffer());
    }

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
        StreamString url = test.GetUrl();
        ret = (url == "submit.cgi");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path = test.GetPath();
        ret = (path == "submit.cgi");
        printf("path=%s\n", path.Buffer());
    }
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
        StreamString url = test.GetUrl();
        ret = (url == "bit/thinner-archives-vol-1.zip.torrent");
        printf("url=%s\n", url.Buffer());

    }

    if (ret) {
        StreamString path = test.GetPath();
        ret = (path == "bit.thinner-archives-vol-1.zip.torrent");
        printf("path=%s\n", path.Buffer());
    }
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
        StreamString url = test.GetUrl();
        ret = (url == "");
        printf("url=%s\n", url.Buffer());
    }

    if (ret) {
        StreamString path = test.GetPath();
        ret = (path == "");
        printf("path=%s\n", path.Buffer());
    }
    return ret;

}

bool HttpStreamTest::TestWriteHeader() {
    return true;
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
    if(ret){
        ret=(remained=="<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>\n");
    }


    return ret;
}

