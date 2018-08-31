/**
 * @file HttpStream.h
 * @brief Header file for class HttpStream
 * @date 17 ago 2018
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

 * @details This header file contains the declaration of the class HttpStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPSTREAM_H_
#define HTTPSTREAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpRealmI.h"
#include "ConfigurationDatabase.h"
#include "StructuredDataStreamT.h"
#include "EventSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
using namespace HttpDefinition;

class HttpStream: public StructuredDataStreamT<ConfigurationDatabase>{
public:
    HttpStream();

    HttpStream(BufferedStreamI &clientBufferedStreamIn);

    virtual ~HttpStream();

    bool WriteHeader(bool bodyCompleted,
                     HSHttpCommand command,
                     const char8 * url);

    bool WriteReplyHeader(bool bodyCompleted,
                          uint32 httpErrorCode = 200);

    bool CompleteReadOperation(BufferedStreamI *s,
                               TimeoutType msecTimeout = TTInfiniteWait);

    bool BodyCompleted();

    bool WaitForBodyCompleted(TimeoutType msecTimeout);

    bool ReadHeader();

    uint32 NumberOfInputCommands();

    bool InputCommandName(StreamString &name,
                          uint32 idx);

    bool InputCommandValue(StreamString &value,
                           uint32 idx);

    bool InputCommandValue(StreamString &value,
                           const char8 *name);

    bool SecurityCheck(ReferenceT<HttpRealmI> realm,
                       uint32 ipNumber);

    bool KeepAlive() const;

    void SetKeepAlive(bool isKeepAlive);

    HSHttpCommand GetHttpCommand() const;

    void SetUnmatchedUrl(char8 *unMatchedUrlIn);

    const char8 *GetPath();

    const char8 *GetUrl();

protected:

    BufferedStreamI *clientStream;

    /**
     * How much data is still waiting in the
     * input stream from the client
     */
    int32 unreadInput;

    /** The Http return code */
    uint32 httpErrorCode;

    HSHttpCommand httpCommand;

    /**
     * possible values are
     * writeToString writeToClient
     * writeToCDB
     */
    HSOperatingMode operationMode;

    /**
     * 1000 means v1.0 2100 means v2.1
     */
    uint32 httpVersion;

    /**
     * True if communication should continue after transaction
     */
    bool keepAlive;

    /**
     * to wait for body completed
     */
    EventSem bodyCompletedEvent;

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

private:
    /**
     * Handles a post request
     */

    /***********************/
    /* used in ReadHeader  */
    /***********************/
    bool RetrieveHttpCommand(StreamString &command,
                             StreamString &line);

    void BuildUrl(StreamString &line);

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

#endif /* SOURCE_CORE_BAREMETAL_L4CONFIGURATION_HTTPSTREAM_H_ */

