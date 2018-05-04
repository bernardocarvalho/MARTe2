/**
 * @file MultiBufferUnrelatedDataSource.h
 * @brief Header file for class MultiBufferUnrelatedDataSource
 * @date Apr 12, 2018
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

 * @details This header file contains the declaration of the class MultiBufferUnrelatedDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MULTIBUFFERUNRELATEDDATASOURCE_H_
#define MULTIBUFFERUNRELATEDDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class MultiBufferUnrelatedDataSource: public DataSourceI {
public:
    MultiBufferUnrelatedDataSource();

    virtual ~MultiBufferUnrelatedDataSource();

    virtual bool Initialise(StructuredDataI &data);

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                                       const uint32 bufferIdx,
                                       void *&signalAddress);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);

    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    virtual int32 GetOffset(const uint32 signalIdx,
                            const uint32 numberOfSamples)=0;

    virtual void PrepareOffsets();



    virtual void TerminateRead(const uint32 signalIdx,
                               const uint32 offset,
                               const uint32 samples);

    virtual void TerminateWrite(const uint32 signalIdx,
                                const uint32 offset,
                                const uint32 samples);

    uint32 GetNumberOfInternalBuffers() const;

protected:
    uint32 numberOfInternalBuffers;
    uint8 *mem;
    uint32 *signalOffsets;
    HeapI *memoryHeap;
    uint32 timeStampSignalIndex;
    uint32 errorCheckSignalIndex;
    uint32 *packetSize;
    StreamString syncOutputBrokerName;
    StreamString syncInputBrokerName;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MULTIBUFFERUNRELATEDDATASOURCE_H_ */

