/**
 * @file HttpClient.h
 * @brief Header file for class HttpClient
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

 * @details This header file contains the declaration of the class HttpClient
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpStream.h"
#include "TCPSocket.h"
#include "UrlAddress.h"
#include "BufferedStreamI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class HttpClient: public Object {
public:
    HttpClient();

    virtual ~HttpClient();

    HttpClient(const char8 * authorisation);

    bool HttpGet(UrlAddress url,
                 BufferedStreamI &stream,
                 TimeoutType msecTimeout,
                 int32 operationId = -1);

private:

    bool Connect(TimeoutType msecTimeout);

    /** */
    void CalculateNonce(StreamString & nonce);

    /** */
    bool GenerateDigestKey(StreamString & key,
                           const char8 * data,
                           const char8 * uri,
                           const char8 * command,
                           int32 nc);

    bool AutenticationProcedure(const TimeoutType &msecTimeout,
                                HttpStream &hs,
                                UrlAddress url,
                                int32 operationId);
    /** the socket being used in the connection */
    TCPSocket socket;

    /** the host at which we are connected to */
    StreamString host;

    /** the host port at which we are connected */
    uint32 port;

    /** authorisation is <BASE64 of User:Password> */
    StreamString authorisation;

    /** digest or basic cooked user:password*/
    StreamString authorisationKey;

    /** a number counting the number of operations performed */
    int32 lastOperationId;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPCLIENT_H_ */
