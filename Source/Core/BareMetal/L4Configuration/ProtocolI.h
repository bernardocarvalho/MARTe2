/**
 * @file ProtocolI.h
 * @brief Header file for class ProtocolI
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

 * @details This header file contains the declaration of the class ProtocolI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PROTOCOLI_H_
#define PROTOCOLI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class ProtocolI: public ConfigurationDatabase {
public:
    ProtocolI();
    virtual ~ProtocolI();

    virtual bool ReadHeader()=0;

    virtual bool WriteHeader(bool isMessageCompleted,
                         int32 command,
                         const char8 * id)=0;

    virtual void SetUnmatchedId(const char8 *unMatchedIdIn)=0;

    virtual void GetUnmatchedId(StreamString& unmatchedIdOut)=0;

    virtual void GetPath(StreamString& pathOut)=0;

    virtual void GetId(StreamString& idOut)=0;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PROTOCOLI_H_ */

