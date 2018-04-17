/**
 * @file CircularBufferThreadInputDataSource.cpp
 * @brief Source file for class CircularBufferThreadInputDataSource
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

 * @details This source file contains the definition of all the methods for
 * the class CircularBufferThreadInputDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CircularBufferThreadInputDataSource.h"
#include "AdvancedErrorManagement.h"
#include "Threads.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

CircularBufferThreadInputDataSource::CircularBufferThreadInputDataSource() :
        MultiBufferUnrelatedDataSource(),
        EmbeddedServiceMethodBinderI(),
        executor(*this) {
    bufferSizeWords = 0;
    threadID = 0u;
    receiverThreadPriority = 31u;
    mutex.Create();
    currentBuffer = 0u;

    isRefreshed = NULL_PTR(uint8 *);
    lastWrittenBuffer = 0u;
    flags = 0u;
    triggerAfterNPackets = 0u;
}

CircularBufferThreadInputDataSource::~CircularBufferThreadInputDataSource() {
    if (isRefreshed != NULL_PTR(uint8 *)) {
        delete[] isRefreshed;
        isRefreshed = NULL_PTR(uint8 *);
    }

}

void CircularBufferThreadInputDataSource::Purge(ReferenceContainer &purgeList) {

    if (executor.GetStatus() != EmbeddedThreadI::OffState) {
        if (!executor.Stop()) {
            if (!executor.Stop()) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
            }
        }
    }
    ReferenceContainer::Purge(purgeList);
}

bool CircularBufferThreadInputDataSource::Initialise(StructuredDataI &data) {
    bool ret = MultiBufferUnrelatedDataSource::Initialise(data);
    if (ret) {
        //the number of packets that arrives to the DS at each cycle
        ret = data.Read("TriggerAfterNPackets", triggerAfterNPackets);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "TriggerAfterNPackets not specified");
        }
    }

    if (ret) {
        // Read cpu mask
        uint32 cpuMask = 0u;
        if (!data.Read("CpuMask", cpuMask)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "CpuMask was not specified. Using default: %d", cpuMask);
            cpuMask = 0xFFFFu;
        }

        if (!data.Read("ReceiverThreadPriority", receiverThreadPriority)) {
            receiverThreadPriority = 31u;
        }
        executor.SetCPUMask(cpuMask);
        executor.SetStackSize(THREADS_DEFAULT_STACKSIZE);
        executor.SetPriorityClass(Threads::RealTimePriorityClass);
        executor.SetPriorityLevel(receiverThreadPriority);

    }

    if (ret) {
        uint8 flagIn = 0u;
        //when synchronise goes directly to the last acquired sample
        if (data.Read("GetLatest", flagIn)) {
            flags |= (flagIn & (0x1u));
        }
        //does not block on synchronise
        if (data.Read("NonBlocking", flagIn)) {
            flags |= ((flagIn & (0x1u)) << 1u);
        }
        //TODO if NonBlocking and !GetLatest, it always returns the same sample!
        //It can be a desired behavior??

    }

    return ret;
}

bool CircularBufferThreadInputDataSource::Synchronise() {

    uint32 originalLastWritten = lastWrittenBuffer;
    uint32 nStepsForward = 0u;
    //if get latest go to the end
    if ((flags & (0x1u)) == (0x1u)) {
        lastWrittenBuffer++;
        nStepsForward++;
        if (lastWrittenBuffer >= numberOfBuffers) {
            lastWrittenBuffer = 0u;
        }
        while (!mutex.FastTryLock()) {

        }
        //roll on consuming the circular buffer until the last written
        /*lint -e{613} null pointer checked before.*/
        while (isRefreshed[lastWrittenBuffer] == 1u) {

            if (lastWrittenBuffer >= numberOfBuffers) {
                lastWrittenBuffer = 0u;
            }
            /*lint -e{613} null pointer checked before.*/
            isRefreshed[lastWrittenBuffer] = 0u;
            lastWrittenBuffer++;
            nStepsForward++;
        }

        uint32 lastBuffer = lastWrittenBuffer;
        mutex.FastUnLock();
        nStepsForward--;
        lastBuffer--;
        if (lastBuffer >= numberOfBuffers) {
            lastBuffer += numberOfBuffers;
        }

        uint32 stepsBack = nStepsForward % triggerAfterNPackets;
        //if asynchronous return back to the last arrived samples, otherwise it will wait for new ones
        //at least double buffer (stepsBack+triggerAfterNPackets)<numberOfBuffers
        lastBuffer -= (stepsBack + triggerAfterNPackets);
        if (lastBuffer >= numberOfBuffers) {
            lastBuffer += numberOfBuffers;
        }
        lastWrittenBuffer = lastBuffer;
    }

    //if synchronous get the new triggerAfterNPackets otherwise return
    if ((flags & (0x2u)) == 0u) {

        //if synchronous cannot return back
        if (nStepsForward < triggerAfterNPackets) {
            lastWrittenBuffer = originalLastWritten;
        }

        uint32 numberOfPacketsSinceLastTrigger = triggerAfterNPackets;

        while (numberOfPacketsSinceLastTrigger > 0u) {

            lastWrittenBuffer++;
            if (lastWrittenBuffer >= numberOfBuffers) {
                lastWrittenBuffer = 0u;
            }

            bool isArrived = false;

            while (!(isArrived)) {
                while (!mutex.FastTryLock()) {

                }
                /*lint -e{613} null pointer checked before.*/
                isArrived = (isRefreshed[lastWrittenBuffer] == 1u);
                /*lint -e{613} null pointer checked before.*/
                isRefreshed[lastWrittenBuffer] = 0u;

                mutex.FastUnLock();
            }

            numberOfPacketsSinceLastTrigger--;
        }
    }

    return true;

}

/*lint -e{715} .*/
bool CircularBufferThreadInputDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                           const char8* const functionName,
                                                           void * const gamMemPtr) {
    return false;
}

bool CircularBufferThreadInputDataSource::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = MultiBufferUnrelatedDataSource::SetConfiguredDatabase(data);

    if (ret) {
        isRefreshed = new uint8[numberOfBuffers];
        (void) MemoryOperationsHelper::Set(isRefreshed, '\0', numberOfBuffers);
        uint32 nOfSignals = GetNumberOfSignals();
        for (uint32 i = 0u; (i < nOfSignals) && (ret); i++) {

            uint32 numberOfStates = 0u;
            ret = GetSignalNumberOfStates(i, numberOfStates);
            //check tha t all the signals are consumed and not produced
            for (uint32 j = 0u; (j < numberOfStates) && (ret); j++) {
                StreamString stateName;
                ret = GetSignalStateName(i, j, stateName);
                if (ret) {
                    uint32 numberOfProducers = 0u;
                    (void) GetSignalNumberOfProducers(i, stateName.Buffer(), numberOfProducers);
                    if (ret) {
                        ret = (numberOfProducers == 0u);
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The signal with id=0 is written... all the signals must be only read", i);
                        }
                    }
                }
            }
        }

        if (ret) {
            //check the size of the time stamp signal...
            if (timeStampSignalIndex != 0xFFFFFFFFu) {
                uint32 signalByteSize;
                ret = GetSignalByteSize(timeStampSignalIndex, signalByteSize);
                if (ret) {
                    uint32 sizeCheck = static_cast<uint32>(sizeof(uint64)) * triggerAfterNPackets * (nOfSignals - 1u);
                    ret = (signalByteSize == sizeCheck);
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The size of the InternalTimeStamp signal must be %d != %d", sizeCheck,
                                                signalByteSize);
                    }
                }
            }
        }
    }
    return ret;
}

/*lint -e{715} .*/
bool CircularBufferThreadInputDataSource::PrepareNextState(const char8 * const currentStateName,
                                                           const char8 * const nextStateName) {
    bool ret = true;
    //start the thread
    currentBuffer = (numberOfBuffers - 1u);
    lastWrittenBuffer = (numberOfBuffers - 1u);

    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
        ret = executor.Start();
    }
    return ret;
}

/*lint -e{715} .*/
int32 CircularBufferThreadInputDataSource::GetOffset(const uint32 signalIdx,
                                                     const int32 flag) {
    /*lint -e{737} -e{9114} -e{9117} -e{713} -e{9125} the change of signedness is managed.*/
    int32 startFromIdx = static_cast<int32>((lastWrittenBuffer - triggerAfterNPackets) + 1u) * packetSize[signalIdx];
    if (startFromIdx < 0) {
        /*lint -e{737} -e{9114} -e{9117} -e{713} -e{9125} the change of signedness is managed.*/
        startFromIdx += static_cast<int32>(numberOfBuffers * packetSize[signalIdx]); //startFromIdx is negative hence the +
    }

    return startFromIdx;
}

ErrorManagement::ErrorType CircularBufferThreadInputDataSource::Execute(const ExecutionInfo & info) {
    ErrorManagement::ErrorType err;

    if (info.GetStage() == ExecutionInfo::MainStage) {
        if (currentBuffer >= numberOfBuffers) {
            currentBuffer = 0u;
        }
        //one read for each signal
        uint32 nOfSignals = GetNumberOfSignals();

        uint32 cnt = 0u;
        if (static_cast<bool>(err)) {
            for (uint32 i = 0u; (i < nOfSignals); i++) {
                if (i != timeStampSignalIndex) {
                    uint32 readBytes = packetSize[i];
                    uint32 memIndex = signalOffsets[i] + (currentBuffer * packetSize[i]);
                    if (!DriverRead(reinterpret_cast<char8*>(&(mem[memIndex])), readBytes, i)) {
                        REPORT_ERROR(ErrorManagement::FatalError, "Failed reading data. Thread is going to return");
                        err.fatalError = true;
                    }
                    if (static_cast<bool>(err)) {
                        //save the timestamp
                        if (timeStampSignalIndex != 0xFFFFFFFFu) {
                            uint32 timeMemIndex = (signalOffsets[timeStampSignalIndex] + ((currentBuffer + cnt) * static_cast<uint32>(sizeof(uint64))));
                            *reinterpret_cast<uint64*>(&(mem[timeMemIndex])) = HighResolutionTimer::Counter();
                        }
                        // Check the buffer length
                        if (readBytes != packetSize[i]) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                                    "Packet size mismatch. readBytes(%d)!=packetSize(%d) Thread is going to return", readBytes, packetSize[i]);
                            err.fatalError = true;
                        }
                    }
                }
                cnt++;
            }
        }

        if (static_cast<bool>(err)) {
            while (!mutex.FastTryLock()) {

            }

            currentBuffer++;
            /*lint -e{613} null pointer checked before.*/
            isRefreshed[currentBuffer] = 1u;
            mutex.FastUnLock();
        }
    }
    else if (info.GetStage() == ExecutionInfo::StartupStage) {

        currentBuffer = 0u;

    }
    else {

    }
    return err;
}

}
