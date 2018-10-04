/**
 * @file HttpServiceTest.h
 * @brief Header file for class HttpServiceTest
 * @date 12/09/2018
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

 * @details This header file contains the declaration of the class HttpServiceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPSERVICETEST_H_
#define HTTPSERVICETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpService.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the HttpService methods
 */
class HttpServiceTest {
public:

    /**
     * @brief Constructor
     */
    HttpServiceTest();

    /**
     * @brief Destructor
     */
    virtual ~HttpServiceTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method with the default WebRoot parameter.
     */
    bool TestInitialise_DefaultWebRoot();

    /**
     * @brief Tests the Initialise method that fails if WebRoot is not
     * specified and the object does not contain the default web root.
     */
    bool TestInitialise_FalseNoDefaultWebRoot();

    /**
     * @brief Tests the Initialise method with default ListenMaxConnections parameter.
     */
    bool TestInitialise_DefaultNListenConnections();

    /**
     * @brief Tests the Initialise method with default Port parameter.
     */
    bool TestInitialise_DefaultPort();

    /**
     * @brief Tests the Start method
     */
    bool TestStart();

    /**
     * @brief Tests the Start method that fails if
     * the path of the root is invalid.
     */
    bool TestStart_InvalidWebRoot();

    /**
     * @brief An interactive ClientService test
     */
    bool TestClientService_Text_Interactive();

    /**
     * @brief Tests the TestClientService method passing
     * TextMode command equal to one,
     */
    bool TestClientService_Text();

    /**
     * @brief Tests the TestClientService method passing
     * TextMode command equal to zero,
     */
    bool TestClientService_Structured();

    /**
     * @brief Tests the ClientService method with
     * Connection: close
     */
    bool TestClientService_CloseConnection();

    /**
     * @brief Tests the ClientService method that use the default root
     * if the path sent by the client is incorrect
     */
    bool TestClientService_InvalidInterface();

    /**
     * @brief Tests the ClientService method that fails
     * if the header is not read correctly.
     */
    bool TestClientService_FailReadHeader();

    /**
     * @brief Tests the ServerCycle method
     */
    bool TestServerCycle();


};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPSERVICETEST_H_ */

