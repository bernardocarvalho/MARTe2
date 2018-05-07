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
#include "MemoryMapUnrelatedInputBroker.h"
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
    threadID = 0u;
    receiverThreadPriority = 31u;
    mutex.Create();
    currentBuffer = NULL_PTR(uint32 *);

    isRefreshed = NULL_PTR(uint8 *);
    lastReadBuffer = NULL_PTR(uint32 *);
    lastReadBuffer_1 = NULL_PTR(uint32 *);

    triggerAfterNPackets = NULL_PTR(uint32 *);
    nBrokerOpPerSignal = NULL_PTR(uint32 *);
    nBrokerOpPerSignalCounter = NULL_PTR(uint32 *);
    numberOfChannels = 0u;
    syncSignal = 0u;

}

CircularBufferThreadInputDataSource::~CircularBufferThreadInputDataSource() {
    if (isRefreshed != NULL_PTR(uint8 *)) {
        delete[] isRefreshed;
        isRefreshed = NULL_PTR(uint8 *);
    }
    if (triggerAfterNPackets != NULL_PTR(uint32 *)) {
        delete[] triggerAfterNPackets;
        triggerAfterNPackets = NULL_PTR(uint32 *);
    }
    if (currentBuffer != NULL_PTR(uint32 *)) {
        delete[] currentBuffer;
        currentBuffer = NULL_PTR(uint32 *);
    }
    if (lastReadBuffer != NULL_PTR(uint32 *)) {
        delete[] lastReadBuffer;
        lastReadBuffer = NULL_PTR(uint32 *);
    }

    if (lastReadBuffer_1 != NULL_PTR(uint32 *)) {
        delete[] lastReadBuffer_1;
        lastReadBuffer_1 = NULL_PTR(uint32 *);
    }

    if (nBrokerOpPerSignal != NULL_PTR(uint32 *)) {
        delete[] nBrokerOpPerSignal;
        nBrokerOpPerSignal = NULL_PTR(uint32 *);
    }
    if (nBrokerOpPerSignalCounter != NULL_PTR(uint32 *)) {
        delete[] nBrokerOpPerSignalCounter;
        nBrokerOpPerSignalCounter = NULL_PTR(uint32 *);
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
        // Read cpu mask
        uint32 cpuMask = 0u;
        if (!data.Read("CpuMask", cpuMask)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "CpuMask was not specified. Using default: %d", cpuMask);
            cpuMask = 0xFFFFu;
        }

        if (!data.Read("ReceiverThreadPriority", receiverThreadPriority)) {
            receiverThreadPriority = 31u;
        }
        receiverThreadPriority %= 32u;
        executor.SetCPUMask(cpuMask);
        executor.SetStackSize(THREADS_DEFAULT_STACKSIZE);
        executor.SetPriorityClass(Threads::RealTimePriorityClass);
        executor.SetPriorityLevel(receiverThreadPriority);

    }

    return ret;
}

void CircularBufferThreadInputDataSource::PrepareOffsets() {

    for (uint32 i = 0u; i < numberOfSignals; i++) {
        uint32 nStepsForward = 0u;
        //always go to the end

        //roll on consuming the circular buffer until the last written
        /*lint -e{613} null pointer checked before.*/
        while (nStepsForward < numberOfInternalBuffers) {
            uint32 lastReadBufTemp = (lastReadBuffer[i] + 1);
            if (lastReadBufTemp >= numberOfInternalBuffers) {
                lastReadBufTemp = 0u;
            }
            mutex.FastLock();
            if (isRefreshed[(lastReadBufTemp * numberOfSignals) + i] == 0u) {
                mutex.FastUnLock();
                break;
            }
            mutex.FastUnLock();
            lastReadBuffer[i] = lastReadBufTemp;
            nStepsForward++;
        }

        uint32 stepsBack = nStepsForward % triggerAfterNPackets[i];
        //if asynchronous return back to the last arrived samples, otherwise it will wait for new ones
        //at least double buffer (stepsBack+triggerAfterNPackets)<numberOfInternalBuffers
        lastReadBuffer_1[i] = lastReadBuffer[i] - stepsBack;
        if (lastReadBuffer_1[i] >= numberOfInternalBuffers) {
            lastReadBuffer_1[i] += numberOfInternalBuffers;
        }
        REPORT_ERROR(ErrorManagement::Information, "buffers %d %d", lastReadBuffer_1[i], lastReadBuffer[i]);
    }
}

bool CircularBufferThreadInputDataSource::Synchronise() {

    uint32 nStepsForward = 0u;
    //always go to the end

    //roll on consuming the circular buffer until the last written
    /*lint -e{613} null pointer checked before.*/
    while (nStepsForward < numberOfInternalBuffers) {
        uint32 lastReadBufTemp = (lastReadBuffer[syncSignal] + 1);
        if (lastReadBufTemp >= numberOfInternalBuffers) {
            lastReadBufTemp = 0u;
        }
        mutex.FastLock();
        if (isRefreshed[(lastReadBufTemp * numberOfSignals) + syncSignal] == 0u) {
            mutex.FastUnLock();
            break;
        }
        mutex.FastUnLock();
        lastReadBuffer[syncSignal] = lastReadBufTemp;
        nStepsForward++;
    }

    bool ret = (nStepsForward < numberOfInternalBuffers);
    if (ret) {
        uint32 stepsBack = nStepsForward % triggerAfterNPackets[syncSignal];
        //if asynchronous return back to the last arrived samples, otherwise it will wait for new ones
        //at least double buffer (stepsBack+triggerAfterNPackets)<numberOfInternalBuffers
        lastReadBuffer[syncSignal] -= stepsBack;
        if (lastReadBuffer[syncSignal] >= numberOfInternalBuffers) {
            lastReadBuffer[syncSignal] += numberOfInternalBuffers;
        }

        //return to the last sub-block
        //triggerAfterNPackets not arrived yet

        uint32 numberOfPacketsSinceLastTrigger = triggerAfterNPackets[syncSignal];

        while (numberOfPacketsSinceLastTrigger > 0u) {

            lastReadBuffer[syncSignal]++;
            if (lastReadBuffer[syncSignal] >= numberOfInternalBuffers) {
                lastReadBuffer[syncSignal] = 0u;
            }

            bool isArrived = false;

            while (!(isArrived)) {
                mutex.FastLock(); /*lint -e{613} null pointer checked before.*/
                isArrived = (isRefreshed[(lastReadBuffer[syncSignal] * numberOfSignals) + syncSignal] == 1u);
                mutex.FastUnLock();
            }

            numberOfPacketsSinceLastTrigger--;
        }
    }

    REPORT_ERROR(ErrorManagement::Information, "buffers %d %d", lastReadBuffer_1[syncSignal], lastReadBuffer[syncSignal]);

    return ret;
}

const char8 *CircularBufferThreadInputDataSource::GetBrokerName(StructuredDataI &data,
                                                                const SignalDirection direction) {

    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == InputSignals) {
        syncInputBrokerName = "MemoryMapSyncUnrelatedInputBroker";

        float32 freq = -1.0F;
        if (!data.Read("Frequency", freq)) {
            freq = -1.0F;
        }

        if (freq >= 0.F) {
            brokerName = "MemoryMapSyncUnrelatedInputBroker";
        }
        else {
            brokerName = "MemoryMapUnrelatedInputBroker";
        }
    }

    return brokerName;

}

bool CircularBufferThreadInputDataSource::GetInputBrokers(ReferenceContainer &inputBrokers,
                                                          const char8* const functionName,
                                                          void * const gamMemPtr) {

    uint32 functionIdx = 0u;
    bool ret = GetFunctionIndex(functionIdx, functionName);

    uint32 numSignals = 0u;
    if (ret) {
        ret = GetFunctionNumberOfSignals(InputSignals, functionIdx, numSignals);
    }

    for (uint32 i = 0u; (i < numSignals) && (ret); i++) {
        //search the gam in the configured database
        ret = MoveToFunctionSignalIndex(InputSignals, functionIdx, i);
        StreamString suggestedBrokerNameIn;
        if (ret) {
            //lets try...
            ret = configuredDatabase.Read("Broker", suggestedBrokerNameIn);

        }
        if (ret) {
            StreamString signalName;
            ret = GetFunctionSignalAlias(InputSignals, functionIdx, i, signalName);
            uint32 nOffsets = 0u;
            uint32 rangeSize = 0u;
            if (ret) {
                ret = GetFunctionSignalNumberOfByteOffsets(InputSignals, functionIdx, i, nOffsets);
                if (ret) {
                    uint32 offsetStart;
                    ret = GetFunctionSignalByteOffsetInfo(InputSignals, functionIdx, i, 0u, offsetStart, rangeSize);
                }
            }
            uint32 samples = 0u;
            if (ret) {
                ret = GetFunctionSignalSamples(InputSignals, functionIdx, i, samples);
            }
            uint32 signalIdx = 0u;
            if (ret) {
                ret = GetSignalIndex(signalIdx, signalName.Buffer());
            }
            uint32 byteSize = 0u;
            if (ret) {
                ret = GetSignalByteSize(signalIdx, byteSize);
            }
            if (ret) {
                bool ok = true;
                uint32 nBrokers = inputBrokers.Size();
                for (uint32 j = 0u; (j < nBrokers) && (ret) && (ok); j++) {
                    ReferenceT<MemoryMapUnrelatedInputBroker> brokerRef = inputBrokers.Get(j);
                    if (brokerRef.IsValid()) {
                        if (suggestedBrokerNameIn == brokerRef->GetClassProperties()->GetName()) {
                            bool noRange = ((nOffsets == 1u) && (byteSize == rangeSize));
                            if (noRange) {
                                nBrokerOpPerSignal[signalIdx]++;
                            }
                            else {
                                nBrokerOpPerSignal[signalIdx] += (nOffsets * samples);
                            }
                            ok = false;
                        }
                    }
                }
                if (ok) {

                    REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "creating broker %s for %s and signal %s(%d)", suggestedBrokerNameIn.Buffer(),
                                            functionName, signalName.Buffer(), signalIdx);

                    ReferenceT<MemoryMapUnrelatedInputBroker> broker(suggestedBrokerNameIn.Buffer());

                    ret = broker.IsValid();
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Broker %s not valid", suggestedBrokerNameIn.Buffer());
                    }
                    if (ret) {
                        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
                        if (ret) {
                            //insert at the beginning the sync one
                            if (suggestedBrokerNameIn == syncInputBrokerName) {
                                ret = inputBrokers.Insert(broker, 0);
                            }
                            else {
                                ret = inputBrokers.Insert(broker);
                            }
                            if (ret) {
                                bool noRange = ((nOffsets == 1u) && (byteSize == rangeSize));
                                if (noRange) {
                                    nBrokerOpPerSignal[signalIdx]++;
                                }
                                else {

                                    nBrokerOpPerSignal[signalIdx] += (nOffsets * samples);
                                }
                            }
                        }
                        else {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failed broker %s Init", suggestedBrokerNameIn.Buffer());
                        }

                    }
                }
            }
        }
    }
    return ret;
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
        currentBuffer = new uint32[numberOfSignals];
        lastReadBuffer = new uint32[numberOfSignals];
        lastReadBuffer_1 = new uint32[numberOfSignals];
        triggerAfterNPackets = new uint32[numberOfSignals];
        nBrokerOpPerSignal = new uint32[numberOfSignals];
        nBrokerOpPerSignalCounter = new uint32[numberOfSignals];

        for (uint32 i = 0u; i < numberOfSignals; i++) {
            triggerAfterNPackets[i] = 0u;
            currentBuffer[i] = 0u;
            lastReadBuffer[i] = (numberOfInternalBuffers - 1u);
            lastReadBuffer_1[i] = (numberOfInternalBuffers - 1u);
            nBrokerOpPerSignal[i] = 0u;
            nBrokerOpPerSignalCounter[i] = 0u;
        }
        syncSignal = numberOfSignals;
        uint32 numberOfFunctions = GetNumberOfFunctions();
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            uint32 numberOfSignalsPerFunction = 0u;
            ret = GetFunctionNumberOfSignals(InputSignals, i, numberOfSignalsPerFunction);
            //need to know the related signal index
            for (uint32 j = 0u; (j < numberOfSignalsPerFunction) && (ret); j++) {
                StreamString functionSignalName;
                ret = GetFunctionSignalAlias(InputSignals, i, j, functionSignalName);
                uint32 signalIdx = 0u;
                if (ret) {
                    ret = GetSignalIndex(signalIdx, functionSignalName.Buffer());
                }
                uint32 samples = 0u;
                if (ret) {

                    ret = GetFunctionSignalSamples(InputSignals, i, j, samples);
                }
                float32 frequency = -1.F;
                if (ret) {
                    ret = GetFunctionSignalReadFrequency(InputSignals, i, j, frequency);
                }
                if (ret) {
                    if (frequency >= 0.F) {
                        triggerAfterNPackets[signalIdx] = samples;
                        syncSignal = signalIdx;
                    }
                    else if (signalIdx != syncSignal) {
                        if (samples > triggerAfterNPackets[signalIdx]) {
                            triggerAfterNPackets[signalIdx] = samples;
                        }
                    }
                }
            }
        }

        if (ret) {
            isRefreshed = new uint8[numberOfInternalBuffers * numberOfSignals];
            (void) MemoryOperationsHelper::Set(&isRefreshed[0], '\0', numberOfInternalBuffers * numberOfSignals);
            for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {

                uint32 numberOfStates = 0u;
                ret = GetSignalNumberOfStates(i, numberOfStates);
                //check that all the signals are consumed and not produced
                for (uint32 j = 0u; (j < numberOfStates) && (ret); j++) {
                    StreamString stateName;
                    ret = GetSignalStateName(i, j, stateName);
                    if (ret) {
                        uint32 numberOfProducers = 0u;
                        (void) GetSignalNumberOfProducers(i, stateName.Buffer(), numberOfProducers);
                        if (ret) {
                            ret = (numberOfProducers == 0u);
                            if (!ret) {
                                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The signal with id=%d is written... all the signals must be only read",
                                                        i);
                            }
                        }
                    }
                }
            }
        }

        if (ret) {
            numberOfChannels = numberOfSignals;
            if (timeStampSignalIndex != 0xFFFFFFFFu) {
                numberOfChannels--;
            }
            if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                numberOfChannels--;
            }
            //check the size of the time stamp signal...
            if (timeStampSignalIndex != 0xFFFFFFFFu) {
                uint32 signalByteSize;
                ret = GetSignalByteSize(timeStampSignalIndex, signalByteSize);
                if (ret) {
                    uint32 sizeCheck = static_cast<uint32>(sizeof(uint64)) * (numberOfChannels);
                    ret = (signalByteSize == sizeCheck);
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The size of the InternalTimeStamp signal must be %d != %d", sizeCheck,
                                                signalByteSize);
                    }
                }

            }
        }
        if (ret) {
            //check the size of the time stamp signal...
            if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                uint32 signalByteSize;
                ret = GetSignalByteSize(errorCheckSignalIndex, signalByteSize);
                if (ret) {
                    uint32 sizeCheck = static_cast<uint32>(sizeof(uint32)) * (numberOfChannels);
                    ret = (signalByteSize == sizeCheck);
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The size of the ErrorCheck signal must be %d != %d", sizeCheck, signalByteSize);
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

    if (executor.GetStatus() == EmbeddedThreadI::OffState) {
        ret = executor.Start();
    }
    return ret;
}

/*lint -e{715} .*/
int32 CircularBufferThreadInputDataSource::GetInputOffset(const uint32 signalIdx,
                                                          const uint32 samples) {
    int32 startFromIdx = -1;

    uint32 startFromIdxTmp = ((lastReadBuffer[signalIdx] - samples) + 1u);
    if (startFromIdxTmp >= numberOfInternalBuffers) {
        startFromIdxTmp += (numberOfInternalBuffers); //startFromIdx is negative hence the +
    }
    /*lint -e{737} -e{9114} -e{9117} -e{713} -e{9125} the change of signedness is managed.*/
    startFromIdx = static_cast<int32>(startFromIdxTmp * packetSize[signalIdx]);
    REPORT_ERROR(ErrorManagement::Information, "startFromIdx %d %d %d", startFromIdx, samples, lastReadBuffer[signalIdx]);

    return startFromIdx;
}

int32 CircularBufferThreadInputDataSource::GetOutputOffset(const uint32 signalIdx,
                                                           const uint32 samples) {
    return -1;
}

ErrorManagement::ErrorType CircularBufferThreadInputDataSource::Execute(ExecutionInfo & info) {
    ErrorManagement::ErrorType err;

    if (info.GetStage() == ExecutionInfo::MainStage) {
        //one read for each signal

        uint32 cnt = 0u;
        uint32 errorMemIndex = 0u;
        //reset the error check

        for (uint32 i = 0u; (i < numberOfSignals); i++) {
            bool ok = true;

            if ((i != timeStampSignalIndex) && (i != errorCheckSignalIndex)) {
                uint32 readBytes = packetSize[i];
                uint32 memIndex = signalOffsets[i] + (currentBuffer[i] * packetSize[i]);
                if (!DriverRead(reinterpret_cast<char8*>(&(mem[memIndex])), readBytes, i)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed reading data. Thread is going to return");
                    ok = false;
                }
                if (ok) {
                    if (readBytes == packetSize[i]) {
                        //save the timestamp
                        if (timeStampSignalIndex != 0xFFFFFFFFu) {
                            uint32 index1 = (currentBuffer[timeStampSignalIndex] * (numberOfChannels));
                            uint32 timeMemIndex = (signalOffsets[timeStampSignalIndex] + ((index1 + cnt) * static_cast<uint32>(sizeof(uint64))));
                            *reinterpret_cast<uint64*>(&(mem[timeMemIndex])) = HighResolutionTimer::Counter();
                        }
                        //the DriverRead returns the size read
                        uint32 index = (currentBuffer[i] * (numberOfSignals));

                        if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                            uint32 index1 = (currentBuffer[errorCheckSignalIndex] * (numberOfChannels));
                            errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index1 + cnt) * static_cast<uint32>(sizeof(uint32))));
                            //REPORT_ERROR(ErrorManagement::Information, "Here %d %d %d", isRefreshed[index + i], errorCheckSignalIndex, cnt);

                            //overlap error
                            mutex.FastLock();
                            if (isRefreshed[index + i] == 1u) {
                                //REPORT_ERROR(ErrorManagement::Information, "Case 1");
                                *reinterpret_cast<uint32*>(&(mem[errorMemIndex])) |= 2u;
                            }
                            mutex.FastUnLock();

                        }

                        mutex.FastLock();
                        /*lint -e{613} null pointer checked before.*/
                        isRefreshed[index + i] = 1u;
                        mutex.FastUnLock();

                        currentBuffer[i]++;
                        if (currentBuffer[i] >= numberOfInternalBuffers) {
                            currentBuffer[i] = 0u;
                        }
                    }

                }
                else {
                    //driver read error
                    if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                        uint32 index = (currentBuffer[errorCheckSignalIndex] * (numberOfChannels));
                        errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index + cnt) * static_cast<uint32>(sizeof(uint32))));
                        REPORT_ERROR(ErrorManagement::Information, "Case 2");
                        *reinterpret_cast<uint32*>(&(mem[errorMemIndex])) |= 1u;
                    }
                }
                cnt++;
            }
        }
        if (timeStampSignalIndex != 0xFFFFFFFFu) {
            uint32 index = (currentBuffer[timeStampSignalIndex] * (numberOfSignals));
            mutex.FastLock();
            /*lint -e{613} null pointer checked before.*/
            isRefreshed[index + timeStampSignalIndex] = 1u;
            mutex.FastUnLock();
            currentBuffer[timeStampSignalIndex]++;
            if (currentBuffer[timeStampSignalIndex] >= numberOfInternalBuffers) {
                currentBuffer[timeStampSignalIndex] = 0u;
            }
        }
        if (errorCheckSignalIndex != 0xFFFFFFFFu) {
            uint32 index = (currentBuffer[errorCheckSignalIndex] * (numberOfSignals));
            mutex.FastLock();
            /*lint -e{613} null pointer checked before.*/
            isRefreshed[index + errorCheckSignalIndex] = 1u;
            mutex.FastUnLock();

            currentBuffer[errorCheckSignalIndex]++;
            if (currentBuffer[errorCheckSignalIndex] >= numberOfInternalBuffers) {
                currentBuffer[errorCheckSignalIndex] = 0u;
            }
            index = (currentBuffer[errorCheckSignalIndex] * (numberOfChannels));

            for (uint32 i = 0u; i < numberOfChannels; i++) {
                errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index + i) * static_cast<uint32>(sizeof(uint32))));
                *reinterpret_cast<uint32*>(&(mem[errorMemIndex])) = 0u;
            }
        }

    }
    else if (info.GetStage() == ExecutionInfo::StartupStage) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            currentBuffer[i] = 0u;
        }
        mutex.FastLock();
        MemoryOperationsHelper::Set(&isRefreshed[0], '\0', numberOfInternalBuffers * numberOfSignals);
        mutex.FastUnLock();

    }
    else {

    }
    return err;
}

void CircularBufferThreadInputDataSource::TerminateRead(const uint32 signalIdx,
                                                        const uint32 offset,
                                                        const uint32 samples) {
    nBrokerOpPerSignalCounter[signalIdx]--;

    if ((nBrokerOpPerSignalCounter[signalIdx] == 0u) || (nBrokerOpPerSignalCounter[signalIdx] >= nBrokerOpPerSignal[signalIdx])) {
        REPORT_ERROR(ErrorManagement::Information, "Terminate %d %d", signalIdx, triggerAfterNPackets[signalIdx]);
        //set as read
        nBrokerOpPerSignalCounter[signalIdx] = nBrokerOpPerSignal[signalIdx];
        uint32 index = lastReadBuffer[signalIdx] - triggerAfterNPackets[signalIdx] + 1u;
        //overflow
        if (index >= numberOfInternalBuffers) {
            index += numberOfInternalBuffers;
        }

        while (1) {
            mutex.FastLock();
            isRefreshed[(index * numberOfSignals) + signalIdx] = 0u;
            mutex.FastUnLock();

            if (index == lastReadBuffer[signalIdx]) {
                break;
            }
            index++;
            if (index >= numberOfInternalBuffers) {
                index = 0u;
            }
        }
        nBrokerOpPerSignalCounter[signalIdx] = nBrokerOpPerSignal[signalIdx];
    }

}

}
