/**
 * @file UrlAddress.cpp
 * @brief Source file for class UrlAddress
 * @date 22 ago 2018
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
 * the class UrlAddress (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UrlAddress.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#include "StringHelper.h"
#include "TypeConversion.h"

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

UrlAddress::UrlAddress() {
    this->server = "";
    this->uri = "";
    this->port = 80;
    this->protocol = URLP_HTTP;
}

/** Initialise from discrete information */
void UrlAddress::Init(const char8 * server,
                      const char8 * uri,
                      uint32 port,
                      HttpDefinition::UrlProtocols protocol) {
    this->server = server;
    this->uri = uri;
    this->port = port;
    this->protocol = protocol;
}

UrlAddress::~UrlAddress() {
    // Auto-generated destructor stub for UrlAddress
    // TODO Verify if manual additions are needed
}

/** Initialise from information on a stream*/
bool UrlAddress::Load(BufferedStreamI & stream) {
    StreamString ptoken;
    char8 term;

    bool ret = stream.GetToken(ptoken, "/", term, "");

    if (ret) {
        ret = stream.GetToken(server, "/", term, "");
        if (ret) {
            // evaluate protocol
            protocol = URLP_NONE;
            if (StringHelper::CompareN(ptoken.Buffer(), "http:", 6) == 0) {
                protocol = URLP_HTTP;
                port = 80;
            }
            else if (StringHelper::CompareN(ptoken.Buffer(), "ftp:", 5) == 0) {
                protocol = URLP_FTP;
                port = 25;
            }
            else if (StringHelper::CompareN(ptoken.Buffer(), "file:", 6) == 0) {
                protocol = URLP_FILE;
            }

            server.SetSize(0);
            if (stream.GetToken(server, "/:", term, "")) {

                ptoken.SetSize(0);
                if (term == ':') {
                    stream.GetToken(ptoken, "/", term, "");

                    TypeConvert(port, ptoken.Buffer());
                }
            }

            uint32 sizeW = 1u;
            char8 slash = '/';
            uri.Write(&slash, sizeW);

            //load the whole uri
            //false??
            ret = stream.GetLine(uri);
        }
    }
    return ret;
}

/** Initialise from information on a stream*/
bool UrlAddress::Load(const char8 * buffer) {
    StreamString stream;
    stream = buffer;
    stream.Seek(0);
    return Load(stream);
}

/** the decoded server */
const char * UrlAddress::Server() {
    return server.Buffer();
}

/** The resource locator */
const char * UrlAddress::GetUri() {
    return uri.Buffer();
}

/** The comm port */
uint32 UrlAddress::Port() {
    return port;
}

/** The protocol */
HttpDefinition::UrlProtocols UrlAddress::Protocol() {
    return protocol;
}

bool UrlAddress::Save(BufferedStreamI &stream) {
    bool ret = true;
    switch (protocol) {
    case URLP_HTTP: {
        stream.Printf("http://%s:%i/%s", server.Buffer(), port, uri.Buffer());
    }
        break;
    case URLP_FTP: {
        stream.Printf("ftp://%s/%s", server.Buffer(), uri.Buffer());
    }
        break;
    case URLP_FILE: {
        stream.Printf("file:///%s", uri.Buffer());
    }
        break;
    default:
        ret = false;
    }
    return ret;
}

}
