/**
 * @file InternetService.cpp
 * @brief Source file for class InternetService
 * @date 27/10/2015
 * @author Giuseppe Ferrò
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
 * the class InternetService (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "InternetService.h"
#include <netdb.h>
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

bool InternetService::SearchByName(const char8 * const name,
                                   const char8 * const protocol) {
    servent *serv = getservbyname(name, protocol);
    bool ret = (serv != NULL);
    if (ret) {

        service = *serv;
    }
    return ret;

}

bool InternetService::SearchByPort(const int32 port,
                                   const char8 * const protocol) {
    servent *serv = getservbyport(port, protocol);
    bool ret = (serv != NULL);
    if (ret) {
        service = *serv;
    }
    return ret;

}

int32 InternetService::Port() const {
    return service.s_port;
}

const char8 *InternetService::Name() const {
    return service.s_name;
}

const char8 *InternetService::Protocol() const {
    return service.s_proto;
}

int32 InternetService::GetPortByName(const char8 * const name) {
    InternetService serviceT;
    if(serviceT.SearchByName(name)){
       //TODO
    }
    return serviceT.Port();
}

}
