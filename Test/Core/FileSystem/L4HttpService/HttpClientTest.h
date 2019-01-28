/**
 * @file HttpClientTest.h
 * @brief Header file for class HttpClientTest
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

 * @details This header file contains the declaration of the class HttpClientTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPCLIENTTEST_H_
#define HTTPCLIENTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpClient.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the HttpClient methods
 */
class HttpClientTest {
public:

    /**
     * @brief Constructor
     */
    HttpClientTest();

    /**
     * @brief Destructor
     */
    virtual ~HttpClientTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method that fails if
     * ServerAddress parameter is not specified
     */
    bool TestInitialise_FalseNoServerAddress();

    /**
     * @brief Tests the Initialise method that fails if
     * ServerPort parameter is not specified
     */
    bool TestInitialise_FalseNoServerPort();

    /**
     * @brief Tests the Initialise method that fails if
     * ServerUri parameter is not specified
     */
    bool TestInitialise_FalseNoServerUri();

    /**
     * @brief Tests the Initialise method with default Authorization
     * parameter
     */
    bool TestInitialise_DefaultAuthorization();

    /**
     * @brief Tests the GetHttpProtocol method
     */
    bool TestGetHttpProtocol();

    /**
     * @brief Tests the HttpExchange method
     */
    bool TestHttpExchange();

    /**
     * @brief Tests the HttpExchange method with digest auth method
     */
    bool TestHttpExchange_Authorization_Digest();

    /**
     * @brief Tests the HttpExchange method with basic auth method
     */
    bool TestHttpExchange_Authorization_Basic();

    /**
     * @brief Tests the HttpExchange method that fails if the auth method
     * is not digest neither basic
     */
    bool TestHttpExchange_Authorization_FalseInvalidAuthType();

    /**
     * @brief Tests the HttpExchange method that fails if timeout expires
     */
    bool TestHttpExchange_Authorization_FalseTimeout();

    /**
     * @brief Tests the HttpExchange method that fails if the command is a reply
     */
    bool TestHttpExchange_Authorization_FalseReplyCommand();

    /**
     * @brief Tests the GetHttpProtocol method that sends again the HTTP request if
     * the server does not close the connection
     */
    bool TestHttpExchange_Authorization_Digest_KeepAlive();

    /**
     * @brief Tests the SetServerAddress method
     */
    bool TestSetServerAddress();

    /**
     * @brief Tests the SetServerPort method
     */
    bool TestSetServerPort();

    /**
     * @brief Tests the SetServerUri method
     */
    bool TestSetServerUri();

    /**
     * @brief Tests the GetServerAddress method
     */
    bool TestGetServerAddress();

    /**
     * @brief Tests the GetServerPort method
     */
    bool TestGetServerPort();

    /**
     * @brief Tests the GetServerUri method
     */
    bool TestGetServerUri();

    /**
     * @brief Tests the GetAuthorisation method
     */
    bool TestGetAuthorisation();

    /**
     * @brief Tests the SetAuthorisation method
     */
    bool TestSetAuthorisation();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPCLIENTTEST_H_ */

