/**
 * @file HttpServiceTest.h
 * @brief Header file for class HttpServiceTest
 * @date 12 set 2018
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

class HttpServiceTest {
public:
    HttpServiceTest();
    virtual ~HttpServiceTest();

    bool TestConstructor();

    bool TestInitialise();

    bool TestInitialise_DefaultWebRoot();

    bool TestInitialise_FalseNoDefaultWebRoot();

    bool TestInitialise_DefaultNListenConnections();

    bool TestInitialise_DefaultRelyUrl();

    bool TestInitialise_DefaultPort();

    bool TestServerCycle_Text_Interactive();

    bool TestServerCycle_Text();

    bool TestServerCycle_Structured();

    bool TestServerCycle_CloseConnection();

    bool TestClientService();
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPSERVICETEST_H_ */

