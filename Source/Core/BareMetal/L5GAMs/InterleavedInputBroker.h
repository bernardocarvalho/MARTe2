/**
 * @file InterleavedInputBroker.h
 * @brief Header file for class InterleavedInputBroker
 * @date Apr 20, 2018
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

 * @details This header file contains the declaration of the class InterleavedInputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTERLEAVEDINPUTBROKER_H_
#define INTERLEAVEDINPUTBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapUnrelatedInputBroker.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class InterleavedInputBroker: public MemoryMapUnrelatedInputBroker {
public:

    CLASS_REGISTER_DECLARATION()

    InterleavedInputBroker();
    virtual ~InterleavedInputBroker();
    virtual bool Execute();
    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 * const functionName,
                      void * const gamMemoryAddress);

protected:
    uint32 *numberOfPacketChunks;
    uint32 *packetChunkSize;
    uint32 *packetSize;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_INTERLEAVEDINPUTBROKER_H_ */

