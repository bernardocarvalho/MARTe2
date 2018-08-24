/**
 * @file UrlAddress.h
 * @brief Header file for class UrlAddress
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

 * @details This header file contains the declaration of the class UrlAddress
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef URLADDRESS_H_
#define URLADDRESS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"
#include "HttpDefinition.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

using namespace HttpDefinition;

class UrlAddress {
public:
    UrlAddress();
    virtual ~UrlAddress();

    void Init(const char8 * server = "",
              const char8 * uri = "",
              uint32 port = 80,
              UrlProtocols protocol = URLP_HTTP);

    bool Load(BufferedStreamI & stream);

    bool Load(const char8 * buffer);

    bool Save(BufferedStreamI & stream);

    const char * Server();

    const char * GetUri();

    uint32 Port();

    HttpDefinition::UrlProtocols Protocol();
private:
    /** valid for HTTP,FTP */
    StreamString server;

    /** valid for HTTP,FTP*/
    uint32 port;

    /** HTTP FTP FILE */
    HttpDefinition::UrlProtocols protocol;

    /** The resource identification */
    StreamString uri;

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* URLADDRESS_H_ */

