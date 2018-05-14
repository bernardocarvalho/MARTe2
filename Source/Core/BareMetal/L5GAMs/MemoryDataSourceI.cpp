/**
 * @file MemoryDataSourceI.cpp
 * @brief Source file for class MemoryDataSourceI
 * @date 07/05/2018
 * @author Andre Neto
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
 * the class MemoryDataSourceI (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryDataSourceI.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MemoryDataSourceI::MemoryDataSourceI() {
    numberOfBuffers = 0u;
    stateMemorySize = 0u;
    totalMemorySize = 0u;
    memory = NULL_PTR(uint8 *);
    signalOffsets = NULL_PTR(uint32 *);
    memoryHeap = NULL_PTR(HeapI *);
    signalSize = NULL_PTR(uint32 *);
}

MemoryDataSourceI::~MemoryDataSourceI() {
    if (memoryHeap != NULL_PTR(HeapI *)) {
        if (memory != NULL_PTR(uint8 *)) {
            /*lint -e{1551} HeapManager::Free is expected to be exception free*/
            memoryHeap->Free(reinterpret_cast<void *&>(memory));
            memory = NULL_PTR(uint8 *);
        }
        memoryHeap = NULL_PTR(HeapI *);
    }
    if (signalOffsets != NULL_PTR(uint32 *)) {
        delete[] signalOffsets;
        signalOffsets = NULL_PTR(uint32 *);
    }

    if (signalSize != NULL_PTR(uint32 *)) {
        delete[] signalSize;
        signalSize = NULL_PTR(uint32 *);
    }
}

bool MemoryDataSourceI::AllocateMemory() {

    uint32 nOfSignals = GetNumberOfSignals();
    bool ret = (memory == NULL_PTR(uint8 *));
    if (ret) {
        if (nOfSignals > 0u) {
            signalOffsets = new uint32[nOfSignals];
            ret = (signalOffsets != NULL_PTR(uint32*));
            if (ret) {
                signalSize = new uint32[nOfSignals];
                ret = (signalSize != NULL_PTR(uint32*));
            }
        }
    }

    stateMemorySize = 0u;
    for (uint32 s = 0u; (s < nOfSignals) && (ret); s++) {
        uint32 thisSignalMemorySize;
        ret = GetSignalByteSize(s, thisSignalMemorySize);

        if (ret) {
            if (signalOffsets != NULL_PTR(uint32 *)) {
                signalOffsets[s] = stateMemorySize;
            }
        }
        if (ret) {
            ret = (thisSignalMemorySize > 0u);
        }
        if (ret) {
            stateMemorySize += (thisSignalMemorySize * numberOfBuffers);
            /*lint -e{613} null pointer checked before.*/
            signalSize[s] = thisSignalMemorySize;
        }
    }
    uint32 numberOfStateBuffers = GetNumberOfStatefulMemoryBuffers();
    if (ret) {
        ret = (numberOfStateBuffers > 0u);
    }
    if (ret) {
        totalMemorySize = stateMemorySize * numberOfStateBuffers;
        if (memoryHeap != NULL_PTR(HeapI *)) {
            memory = reinterpret_cast<uint8 *>(memoryHeap->Malloc(totalMemorySize));
        }
        ret = MemoryOperationsHelper::Set(memory, '\0', totalMemorySize);
    }
    return ret;

}

uint32 MemoryDataSourceI::GetNumberOfMemoryBuffers() {
    return numberOfBuffers;
}

bool MemoryDataSourceI::Initialise(StructuredDataI & data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        ret = data.Read("NumberOfBuffers", numberOfBuffers);
        if (!ret) {
            ret = true;
            numberOfBuffers = 1u;
            REPORT_ERROR(ErrorManagement::Warning, "NumberOfBuffers was not specified. Using default: %d", numberOfBuffers);
        }
    }

    if (ret) {
        StreamString heapName;
        if (data.Read("HeapName", heapName)) {
            memoryHeap = HeapManager::FindHeap(heapName.Buffer());
            if (memoryHeap == NULL_PTR(HeapI *)) {
                ret = false;
                REPORT_ERROR(ErrorManagement::Warning, "Could not instantiate an memoryHeap with the name: %s", heapName.Buffer());
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Warning, "Using the standard heap");
            memoryHeap = GlobalObjectsDatabase::Instance()->GetStandardHeap();
        }
    }
    return ret;
}



const char8 *MemoryDataSourceI::GetBrokerName(StructuredDataI &data,
                                                           const SignalDirection direction) {

    const char8* brokerName = NULL_PTR(const char8 *);

    if (direction == InputSignals) {
        syncInputBrokerName = "MemoryMapSynchronisedMultiBufferInputBroker";

        float32 frequency = 0.F;
        if (data.Read("Frequency", frequency)) {
            if (frequency > 0.) {
                brokerName = "MemoryMapSynchronisedMultiBufferInputBroker";
            }
        }
        uint32 trigger = 0u;
        if (data.Read("Trigger", trigger)) {
            if (trigger > 0u) {
                brokerName = "MemoryMapSynchronisedMultiBufferInputBroker";
            }
        }
        if (brokerName == NULL_PTR(const char8 *)) {
            brokerName = "MemoryMapMultiBufferInputBroker";
        }
    }

    if (direction == OutputSignals) {
        syncOutputBrokerName = "MemoryMapSynchronisedMultiBufferOutputBroker";

        float32 frequency = 0.F;
        if (data.Read("Frequency", frequency)) {
            if (frequency > 0.) {
                brokerName = "MemoryMapSynchronisedMultiBufferOutputBroker";
            }
        }
        uint32 trigger = 0u;
        if (data.Read("Trigger", trigger)) {
            if (trigger > 0u) {
                brokerName = "MemoryMapSynchronisedMultiBufferOutputBroker";
            }
        }
        if (brokerName == NULL_PTR(const char8 *)) {
            brokerName = "MemoryMapMultiBufferOutputBroker";
        }
    }

    return brokerName;
}

bool MemoryDataSourceI::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                                      const char8* const functionName,
                                                      void * const gamMemPtr) {
    uint32 functionIdx = 0u;
    bool ret = GetFunctionIndex(functionIdx, functionName);

    uint32 numSignals = 0u;
    if (ret) {
        ret = GetFunctionNumberOfSignals(OutputSignals, functionIdx, numSignals);
    }

    for (uint32 i = 0u; (i < numSignals) && (ret); i++) {
        //search the gam in the configured database
        ret = MoveToFunctionSignalIndex(OutputSignals, functionIdx, i);
        StreamString suggestedBrokerNameIn;
        if (ret) {
            //lets try...
            ret = configuredDatabase.Read("Broker", suggestedBrokerNameIn);

        }
        if (ret) {
            StreamString signalName;
            ret = GetFunctionSignalAlias(OutputSignals, functionIdx, i, signalName);
            uint32 signalIdx = 0u;
            if (ret) {
                ret = GetSignalIndex(signalIdx, signalName.Buffer());
            }
            if (ret) {
                bool ok = true;
                uint32 nBrokers = outputBrokers.Size();
                for (uint32 j = 0u; (j < nBrokers) && (ret) && (ok); j++) {
                    ReferenceT<MemoryMapMultiBufferBroker> brokerRef = outputBrokers.Get(j);
                    if (brokerRef.IsValid()) {
                        if (suggestedBrokerNameIn == brokerRef->GetClassProperties()->GetName()) {
                            ok = false;
                        }
                    }
                }

                if (ok) {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "creating broker %s for %s and signal %s(%d)", suggestedBrokerNameIn.Buffer(),
                                            functionName, signalName.Buffer(), signalIdx);

                    ReferenceT<MemoryMapMultiBufferBroker> broker(suggestedBrokerNameIn.Buffer());

                    ret = broker.IsValid();
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Broker %s not valid", suggestedBrokerNameIn.Buffer());
                    }
                    if (ret) {
                        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
                        if (ret) {
                            //insert at the beginning the sync one
                            if (suggestedBrokerNameIn == syncOutputBrokerName) {
                                ret = outputBrokers.Insert(broker, 0);
                            }
                            else {
                                ret = outputBrokers.Insert(broker);
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

bool MemoryDataSourceI::GetInputBrokers(ReferenceContainer &inputBrokers,
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
            uint32 signalIdx = 0u;
            if (ret) {
                ret = GetSignalIndex(signalIdx, signalName.Buffer());
            }
            if (ret) {
                bool ok = true;
                uint32 nBrokers = inputBrokers.Size();
                for (uint32 j = 0u; (j < nBrokers) && (ret) && (ok); j++) {
                    ReferenceT<MemoryMapMultiBufferBroker> brokerRef = inputBrokers.Get(j);
                    if (brokerRef.IsValid()) {
                        if (suggestedBrokerNameIn == brokerRef->GetClassProperties()->GetName()) {
                            ok = false;
                        }
                    }
                }
                if (ok) {

                    REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "creating broker %s for %s and signal %s(%d)", suggestedBrokerNameIn.Buffer(),
                                            functionName, signalName.Buffer(), signalIdx);

                    ReferenceT<MemoryMapMultiBufferBroker> broker(suggestedBrokerNameIn.Buffer());

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

bool MemoryDataSourceI::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    bool ret = (bufferIdx < GetNumberOfStatefulMemoryBuffers());
    uint32 nOfSignals = GetNumberOfSignals();
    if (ret) {
        ret = (signalIdx < nOfSignals);
    }

    if (ret) {
        char8 *signalAddressChar = reinterpret_cast<char8 *>(memory);
        //Move to the correct state buffer
        uint32 stateBufferOffset = stateMemorySize * bufferIdx;
        signalAddressChar = &signalAddressChar[stateBufferOffset];

        uint32 offset = 0u;
        if (signalOffsets != NULL_PTR(uint32 *)) {
            offset = signalOffsets[signalIdx];
        }
        if (signalAddressChar != NULL_PTR(char8 *)) {
            signalAddressChar = &signalAddressChar[offset];
            signalAddress = reinterpret_cast<void *&>(signalAddressChar);
        }
    }

    return ret;
}

}
