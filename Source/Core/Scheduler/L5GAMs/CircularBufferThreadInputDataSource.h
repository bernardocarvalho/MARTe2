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
#include "MemoryDataSourceI.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "SingleThreadService.h"
#include "FastPollingMutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Multi circular buffer data source interface. The internal thread acquires sequentially a sample of each signal using the interface \a DriverRead(*)
 * that have to be implemented for the specific data source that inherits from this one.
 *
 * @details Two optional signals can be defined and they are recognised by name:
 *   \a InternalTimeStamp: stores the HighResolutionTimer::Counter() time stamp of the signals once they have been read. It must be
 *     uint64 type and it must have a number of elements equal to the number of the normal signals that the data source produces.
 *   \a ErrorCheck: provides an error flag for each signal. It must be uint32 type and it must have a number of elements equal to
 *     the number of the normal signals that the data source produces. Only 2 bits are used in this implementation:
 *       - bit 0: DriverRead(*) function returns false.
 *       - bit 1: Write overlap. Attempting to write on a sample that haven't been read yet by the consumers.
 *
 *
 * @details The configuration syntax is (names and signal quantity are only given as an example):
 * <pre>
 * +CircularBuffer_0 = {
 *     Class = (child of) CircularBufferThreadInputDataSource
 *     Signals = {
 *         *InternalTimeStamp = {
 *             Type = uint64
 *             NumberOfDimensions = 1
 *             NumberOfElements = N //the number of signals belonging to this dataSource (except InternalTimeStamp and ErrorCheck)
 *         }
 *         *ErrorCheck = {
 *             Type = uint32
 *             NumberOfDimensions = 1
 *             NumberOfElements = N //the number of signals belonging to this dataSource (except InternalTimeStamp and ErrorCheck)
 *         }
 *         ....
 *         ....
 *         ....
 *     }
 * }
 * <pre>
 */
namespace MARTe {
class CircularBufferThreadInputDataSource: public MemoryDataSourceI, public EmbeddedServiceMethodBinderI {
public:

    /**
     * @brief Constructor
     */
    CircularBufferThreadInputDataSource();

    virtual ~CircularBufferThreadInputDataSource();

    virtual bool Initialise(StructuredDataI &data);

    virtual bool Synchronise();

    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);

    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    virtual void PrepareInputOffsets();

    virtual bool GetInputOffset(const uint32 signalIdx,
                                const uint32 numberOfSamples,
                                uint32 &offset);

    virtual bool GetOutputOffset(const uint32 signalIdx,
                                const uint32 numberOfSamples,
                                uint32 &offset);

    /**
     * @brief The execute function
     */
    virtual ErrorManagement::ErrorType Execute(ExecutionInfo & info);

    virtual bool DriverRead(char8 * const bufferToFill,
                            uint32 &sizeToRead,
                            const uint32 signalIdx)=0;

    virtual void Purge(ReferenceContainer &purgeList);

    virtual bool TerminateInputCopy(const uint32 signalIdx,
                                    const uint32 offset,
                                    const uint32 numberOfSamples);

protected:
    /* The current buffer */
    uint32 *currentBuffer;
    SingleThreadService executor;
    ThreadIdentifier threadID;
    uint8 receiverThreadPriority;
    FastPollingMutexSem mutex;
    uint8 *isRefreshed;
    uint32 *lastReadBuffer;
    uint32 *triggerAfterNPackets;
    uint32 *nBrokerOpPerSignal;
    uint32 *nBrokerOpPerSignalCounter;
    uint32 numberOfChannels;
    uint32 syncSignal;
    uint32 timeStampSignalIndex;
    uint32 errorCheckSignalIndex;


};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CIRCULARBUFFERTHREADINPUTDATASOURCE_H_ */

