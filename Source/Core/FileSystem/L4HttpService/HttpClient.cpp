/**
 * @file HttpClient.cpp
 * @brief Source file for class HttpClient
 * @date 22/08/2018
 * @author Giuseppe Ferro
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
 * the class HttpClient (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpClient.h"
#include "Base64Encoder.h"
#include "Md5Encrypt.h"
#include "Threads.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static bool SearchKey(const char8 *key,
                      const char8 *name,
                      StreamString &value) {
    value.SetSize(0);

    bool ret = (key != NULL);
    if (ret) {
        ret = (name != NULL);
    }

    if (ret) {
        const char8 *p = StringHelper::SearchString(key, name);
        ret = (p != NULL);
        if (ret) {
            p += StringHelper::Length(name);
            ret = ((p[0] == '=') && (p[1] == '"'));
            if (ret) {
                p += 2;
                while (*p != '"') {
                    value += *p;
                    p = &(p[1]);
                }
            }
        }
    }
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HttpClient::HttpClient() {
    port = 0u;
    host = "";
    lastOperationId = 0;
}

HttpClient::~HttpClient() {
    // Auto-generated destructor stub for HttpClient
    // TODO Verify if manual additions are needed
}

bool HttpClient::AutenticationProcedure(const TimeoutType &msecTimeout,
                                        HttpStream &hs,
                                        UrlAddress url,
                                        int32 operationId) {

    bool ret = true;

    // discard bodyF
    StreamString nullStream;
    ret = hs.CompleteReadOperation(&nullStream, msecTimeout);
    if (ret) {
        ret = (authorisation.Size() >= 3u);
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Authorisation information needed to access %s", url.GetUri());
        }
    }

    if (ret) {
        ret = hs.Switch("InputHttpOtions");
        if (ret) {
            ret = hs.Load("WWW-Authenticate");
        }
    }

    StreamString newAuthorisationKey;
    if (ret) {
        StreamString authRequest;
        hs.Seek(0);
        char8 terminator;
        (void) hs.GetToken(authRequest, "\n\r", terminator);

        // identify basic or digest and call appropriate function
        // to generate newAuthorisationKey
        if (StringHelper::CompareN("Basic ", authRequest.Buffer(), 6u) == 0) {
            newAuthorisationKey = authorisation;
        }
        else if (StringHelper::CompareN("Digest ", authRequest.Buffer(), 7u) == 0) {
            GenerateDigestKey(newAuthorisationKey, &(authRequest.Buffer()[7]), url.GetUri(), "GET", operationId);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Authorisation request of unknown type: %s", authRequest.Buffer());
            ret = false;
        }
    }
    return ret;
}

bool HttpClient::HttpGet(UrlAddress url,
                         BufferedStreamI &stream,
                         TimeoutType msecTimeout,
                         int32 operationId) {

    // absolute time for timeout !
    uint64 startCounter = HighResolutionTimer::Counter();

    // a counter of the transactions
    if (operationId == -1) {
        operationId = lastOperationId++;
    }

    bool ret = (url.GetProtocol() == URLP_HTTP);
    if (!ret) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Only HTTP url is supported\n");
    }
    else {
        const char8 *urlUri = url.GetUri();
        const char8 *urlHost = url.GetServer();
        uint32 urlPort = url.GetPort();

        bool reConnect = (!socket.IsConnected());

        if (host != NULL) {
            if (!(host == urlHost)) {
                host = urlHost;
                reConnect = true;
            }
        }
        if (port > 0u) {
            if (port != urlPort) {
                port = urlPort;
                reConnect = true;
            }
        }

        /* connect to remote host */
        if (reConnect) {
            ret = Connect(msecTimeout);
        }
        HttpStream hs(socket);

        if (ret) {
            /* create and send request */
            hs.SetKeepAlive(true);
            if (authorisationKey.Size() > 0) {
                //switch and write
                ret = hs.Switch("OutputHttpOtions");
                if (ret) {
                    hs.SetSize(0);
                    hs.Printf("%s", authorisationKey.Buffer());
                }
                hs.Commit("Authorization");
            }

            if (ret) {
                ret = hs.WriteHeader(true, HSHCGet, urlUri);
            }

        }
        if (ret) {
            /* read reply */
            ret = hs.ReadHeader();
        }

        if (ret) {
            if (msecTimeout.IsFinite()) {
                uint64 elapsed = (HighResolutionTimer::Counter() - startCounter);
                uint64 delta = msecTimeout.HighResolutionTimerTicks();
                ret = elapsed < delta;
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::Timeout, "Timeout on completion");
                }
                else {
                    uint64 ticksLeft = (delta - elapsed);
                    msecTimeout.SetTimeoutHighResolutionTimerTicks(ticksLeft);
                }
            }
        }

        if (ret) {
            // check reply for 200 (authorisation request)
            if (hs.GetHttpCommand() == HSHCReplyOK) {
                // read body
                ret = hs.CompleteReadOperation(&stream, msecTimeout);
            }
            else {

                // check reply for 401 (authorisation request)
                if (hs.GetHttpCommand() == HSHCReplyAUTH) {

                    ret = AutenticationProcedure(msecTimeout, hs, url, operationId);

                }
                else {

                    // discard body
                    ret = hs.CompleteReadOperation(&stream, msecTimeout);

                    // close if the server says so...
                    if (!hs.KeepAlive()) {
                        socket.Close();
                    }
                }

            }

        }

    }
    return ret;

}

bool HttpClient::Connect(TimeoutType mSecTimeout) {
    (void) socket.Close();
    bool ret = socket.Open();
    if (ret) {
        ret = socket.SetBlocking(true);
    }
    if (ret) {
        ret = socket.Connect(host.Buffer(), port, mSecTimeout);
    }
    return ret;
}

bool HttpClient::GenerateDigestKey(StreamString &key,
                                   const char8 *data,
                                   const char8 *uri,
                                   const char8 *command,
                                   int32 nc) {
    key.SetSize(0);

    StreamString qop = "auth";
    //if (!SearchKeyNoQuote(data,"qop",qop)) return False;

    StreamString userPasswd;
    bool ret = Base64Encoder::Decode(authorisation, userPasswd);
    StreamString user;
    StreamString passwd;
    StreamString realm;
    if (ret) {

        char8 terminator;

        userPasswd.Seek(0);
        (void) userPasswd.GetToken(user, ":", terminator);
        (void) userPasswd.GetToken(passwd, "\n\t ", terminator);

        ret = SearchKey(data, "realm", realm);
    }

    StreamString HA1;
    StreamString HA2;
    StreamString response;
    StreamString ncStr;
    StreamString cnonce;
    StreamString nonce;
    StreamString opaque;

    if (ret) {
        HA1.SetSize(0);
        uint8 buffer[16];
        StreamString toEncode;
        toEncode.Printf("%s:%s:%s", user.Buffer(), realm.Buffer(), passwd.Buffer());
        Md5Encrypt::Md5(reinterpret_cast<uint8 *>(toEncode.BufferReference()), toEncode.Size(), buffer);
        for (uint32 i = 0u; i < 16u; i++) {
            HA1.Printf("%02x", buffer[i]);
        }

        MemoryOperationsHelper::Set(buffer, '\0', 16u);
        HA2.SetSize(0);
        toEncode.SetSize(0);
        toEncode.Printf("%s:%s", command, uri);
        Md5Encrypt::Md5(reinterpret_cast<uint8*>(toEncode.BufferReference()), toEncode.Size(), buffer);
        for (uint32 i = 0; i < 16u; i++) {
            HA2.Printf("%02x", buffer[i]);
        }

        ncStr.Printf("%08x", nc);
        CalculateNonce(cnonce);
        ret = SearchKey(data, "nonce", nonce);
    }
    if (ret) {
        uint8 buffer[16];

        StreamString toEncode;
        toEncode.SetSize(0);
        toEncode.Printf("%s:%s:%s:%s:%s:%s", HA1.Buffer(), nonce.Buffer(), ncStr.Buffer(), cnonce.Buffer(), qop.Buffer(), HA2.Buffer());
        Md5Encrypt::Md5(reinterpret_cast<uint8*>(toEncode.BufferReference()), toEncode.Size(), buffer);
        for (uint32 i = 0u; i < 16u; i++) {
            response.Printf("%02x", buffer[i]);
        }
        ret = SearchKey(data, "opaque", opaque);
    }

    if (ret) {

        key.Printf("Digest "
                   "username=\"%s\","
                   "realm=\"%s\","
                   "nonce=\"%s\","
                   "uri=\"%s\","
                   "qop=%s,"
                   "nc=%s,"
                   "cnonce=\"%s\","
                   "response=\"%s\","
                   "opaque=\"%s\"",
                   user.Buffer(), realm.Buffer(), nonce.Buffer(), uri, qop.Buffer(), ncStr.Buffer(), cnonce.Buffer(), response.Buffer(), opaque.Buffer());
    }
    return ret;

}

void HttpClient::CalculateNonce(StreamString &nonce) {
    nonce.SetSize(0);
    StreamString tid;
    tid.Printf("%08x%08x", static_cast<uint32>(Threads::Id()), this);
    uint8 buffer[16];
    Md5Encrypt::Md5((uint8 *) tid.BufferReference(), tid.Size(), buffer);
    for (uint32 i = 0u; i < 16u; i++) {
        nonce.Printf("%02x", buffer[i]);
    }
}

}

