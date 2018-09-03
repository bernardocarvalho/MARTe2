/**
 * @file MultiStream.h
 * @brief Header file for class MultiStream
 * @date 30 ago 2018
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

 * @details This header file contains the declaration of the class MultiStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MULTISTREAM_H_
#define MULTISTREAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"
#include "StructuredDataI.h"
#include "AnyType.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

class MultiStream: public StreamString {
public:
    MultiStream();

    virtual ~MultiStream();

    virtual bool Switch(const char8 *path)=0;

    virtual bool Load(const char8 *varName)=0;

    virtual bool Commit(const char8 *varName)=0;

    virtual bool Delete(const char8 *varName)=0;

    virtual void GetCurrentPath(StreamString &path)=0;

    virtual bool SwitchPrintAndCommit(const char8* path,
                              const char8* varName,
                              const char8* format,
                              const AnyType pars[])=0;

    virtual bool IsValid() const;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L3STREAMS_MULTISTREAM_H_ */

