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
#include "TCPSocket.h"
#include "BufferedStreamI.h"
#include "HttpProtocol.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class HttpClient: public Object {
public:
    HttpClient();

    virtual ~HttpClient();

    virtual bool Initialise(StructuredDataI &data);

    //can use it to set http options and commands
    HttpProtocol *GetHttpProtocol();

    bool HttpExchange(BufferedStreamI &streamDataRead,
                      int32 command,
                      BufferedStreamI * const payload = NULL_PTR(BufferedStreamI *),
                      TimeoutType msecTimeout=TTInfiniteWait,
                      int32 operationId = -1);


    void SetServerAddress(const char8 * const serverAddressIn);

    void SetServerPort(const uint32 serverPortIn);

    void SetServerUri(const char8 * const serverUriIn);


    void GetServerAddress(StreamString &serverAddrOut) const;

    uint32 GetServerPort() const;

    void GetServerUri(StreamString &serverUriOut) const;

protected:

    bool Connect(TimeoutType msecTimeout);

    /** */
    void CalculateNonce(StreamString & nonce);

    /** */
    bool GenerateDigestKey(StreamString & key,
                           const char8 * const data,
                           const char8 * const command,
                           int32 nc);

    bool AutenticationProcedure(const int32 command, const TimeoutType &msecTimeout,
            const int32 operationId);


    bool reConnect;

    /** the socket being used in the connection */
    TCPSocket socket;

    HttpProtocol protocol;

    StreamString urlUri;
    StreamString urlHost;
    uint32 urlPort;

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
