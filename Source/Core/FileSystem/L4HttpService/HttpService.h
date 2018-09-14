/**
 * @file HttpService.h
 * @brief Header file for class HttpService
 * @date 24 ago 2018
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

 * @details This header file contains the declaration of the class HttpService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPSERVICE_H_
#define HTTPSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MultiClientService.h"
#include "MessageI.h"
#include "TCPSocket.h"
#include "StreamString.h"
#include "ReferenceT.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "FastPollingMutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class HttpService: public MultiClientService, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    HttpService();
    virtual ~HttpService();

    virtual bool Initialise(StructuredDataI &data);

    virtual ErrorManagement::ErrorType Start();

    ErrorManagement::ErrorType ServerCycle(MARTe::ExecutionInfo &information);

    ErrorManagement::ErrorType ClientService(TCPSocket *commClient);

protected:
    /** The server socket */
    TCPSocket server;

    /** The port this server is listening to */
    uint32 port;


    uint32 listenMaxConnections;


    /** http://server:port server that will provide port allocation and relay of http */
    StreamString httpRelayURL;

    /** Where the web pages are contained.
     It will use the URL to search in the container */
    ReferenceT<ReferenceContainer> webRoot;


    EmbeddedServiceMethodBinderI *embeddedMethod;

    StreamString webRootPath;


    uint8 textMode;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPSERVICE_H_ */

