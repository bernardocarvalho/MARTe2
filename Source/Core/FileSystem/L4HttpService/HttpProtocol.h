/**
 * @file HttpProtocol.h
 * @brief Header file for class HttpProtocol
 * @date 14 set 2018
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

 * @details This header file contains the declaration of the class HttpProtocol
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPPROTOCOL_H_
#define HTTPPROTOCOL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "ProtocolI.h"
#include "DoubleBufferedStream.h"
#include "HttpDefinition.h"
#include "HttpRealmI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
using namespace HttpDefinition;
class HttpProtocol: public ProtocolI {
public:
    HttpProtocol(DoubleBufferedStream &clientBufferedStreamIn, BufferedStreamI &payloadIn);

    virtual ~HttpProtocol();

    bool ReadHeader();

    bool WriteHeader(bool isMessageCompleted=true,
                         int32 command=0,
                         const char8 * id=NULL);

    //void SetPayload(BufferedStreamI &payloadIn)

    bool CompleteReadOperation(BufferedStreamI *s,
                               TimeoutType msecTimeout = TTInfiniteWait);

    bool SecurityCheck(ReferenceT<HttpRealmI> realm);

    bool KeepAlive() const;

    void SetKeepAlive(bool isKeepAlive);

    HSHttpCommand GetHttpCommand() const;

    void SetUnmatchedUrl(const char8 *unMatchedUrlIn);

    void GetUnmatchedUrl(StreamString& unmatchedUrlOut);

    void GetPath(StreamString& pathOut);

    void GetUrl(StreamString& urlOut);

    uint8 TextMode();

protected:
    DoubleBufferedStream *outputStream;


    BufferedStreamI* payload;

    /**
     * How much data is still waiting in the
     * input stream from the client
     */
    int32 unreadInput;

    /** The Http return code */
    uint32 httpErrorCode;

    HSHttpCommand httpCommand;

    /**
     * 1000 means v1.0 2100 means v2.1
     */
    uint32 httpVersion;

    /**
     * True if communication should continue after transaction
     */
    bool keepAlive;

    /**
     * The last time the body has been updated
     */
    uint64 lastUpdateTime;

    /**
     * The requested page URL
     */
    StreamString url;

    /**
     * The URL with . instead of \/
     */
    StreamString path;

    /**
     * The remainder of url not matched in the search
     */
    StreamString unMatchedUrl;


    ConfigurationDatabase options;

    int8 textMode;

private:
    /**
     * Handles a post request
     */

    /***********************/
    /* used in ReadHeader  */
    /***********************/
    bool RetrieveHttpCommand(StreamString &command,
                             StreamString &line);

    char8 BuildUrl(StreamString &line);

    bool StoreCommands(StreamString &line);

    bool StoreOutputOptions();

    bool StoreInputOptions();

    /*****************************************/
    /* used in ReadHeader to handle the post */
    /*****************************************/

    bool HandlePost(StreamString &contentType,
                    StreamString &content);

    bool HandlePostMultipartFormData(StreamString &contentType,
                                     StreamString &content);

    bool HandlePostApplicationForm(StreamString &contentType,
                                   StreamString &content);

    bool HandlePostHeader(StreamString &line,
                          StreamString &content,
                          StreamString &name,
                          StreamString &filename);

    bool HandlePostContent(StreamString &line,
                           StreamString &boundary,
                           StreamString &name,
                           StreamString &filename,
                           StreamString &value,
                           bool &headerHandled);

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_FILESYSTEM_L4HTTPSERVICE_HTTPPROTOCOL_H_ */

