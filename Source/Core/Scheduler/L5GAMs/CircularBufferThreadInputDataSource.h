/**
 * @file CircularBufferThreadInputDataSource.h
 * @brief Header file for class CircularBufferThreadInputDataSource
 * @date Apr 13, 2018
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

 * @details This header file contains the declaration of the class CircularBufferThreadInputDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CIRCULARBUFFERTHREADINPUTDATASOURCE_H_
#define CIRCULARBUFFERTHREADINPUTDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MultiBufferUnrelatedDataSource.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "SingleThreadService.h"
#include "FastPollingMutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class CircularBufferThreadInputDataSource: public MultiBufferUnrelatedDataSource, public EmbeddedServiceMethodBinderI {
public:

    CircularBufferThreadInputDataSource();

    virtual ~CircularBufferThreadInputDataSource();

    virtual bool Initialise(StructuredDataI &data);

    virtual bool Synchronise();

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);

    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    virtual int32 GetOffset(const uint32 signalIdx,
                            const int32 flag);

    virtual ErrorManagement::ErrorType Execute(const ExecutionInfo & info);

    virtual bool DriverRead(char8 *bufferToFill,
                            uint32 &sizeToRead,
                            uint32 signalIdx)=0;

    virtual void Purge(ReferenceContainer &purgeList);

protected:

    //Thread parameters
    uint32 currentBuffer;
    int32 bufferSizeWords;
    SingleThreadService executor;
    ThreadIdentifier threadID;
    uint8 receiverThreadPriority;
    FastPollingMutexSem mutex;
    uint8 *isRefreshed;
    uint32 lastWrittenBuffer;
    uint8 flags; //bits: 0-GetLatest | 1-IsSync

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CIRCULARBUFFERTHREADINPUTDATASOURCE_H_ */

