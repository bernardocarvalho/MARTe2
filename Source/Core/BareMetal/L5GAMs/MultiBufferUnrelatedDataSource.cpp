/**
 * @file MultiBufferUnrelatedDataSource.cpp
 * @brief Source file for class MultiBufferUnrelatedDataSource
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

 * @details This source file contains the definition of all the methods for
 * the class MultiBufferUnrelatedDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MultiBufferUnrelatedDataSource.h"
#include "AdvancedErrorManagement.h"
#include "StreamString.h"
#include "MemoryMapUnrelatedInputBroker.h"
#include "MemoryMapUnrelatedOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

//Very powerful...
//Possibility to choose the Broker putting a $BrokerName in the signal name
//Using Ranges in GAMs and defining the signal with the maximum of the sizes,
//it is possible make operations directly inside the brokers. The broker itself defines
//the packet size to be acquired from the driver, independently from the signal size.
//The default brokers (MemoryMapUnrelatedInputBroker and SimpleWriters), simply acts like the MemoryMapBroker
//returning the packet size defined by the signal and copying from the moving buffer index defined
//time by time by the DataSource with the GetOffset function
MultiBufferUnrelatedDataSource::MultiBufferUnrelatedDataSource() :
        DataSourceI() {
    numberOfBuffers = 0u;
    mem = NULL_PTR(uint8 *);
    timeStampSignalIndex = 0xFFFFFFFFu;
    signalOffsets = NULL_PTR(uint32 *);
    memoryHeap = NULL_PTR(HeapI *);
    triggerAfterNPackets = 0u;
    packetSize = NULL_PTR(uint32 *);
}

MultiBufferUnrelatedDataSource::~MultiBufferUnrelatedDataSource() {
    if (memoryHeap != NULL_PTR(HeapI *)) {
        if (mem != NULL_PTR(uint8 *)) {
            /*lint -e{1551} HeapManager::Free is expected to be exception free*/
            memoryHeap->Free(reinterpret_cast<void *&>(mem));
            mem = NULL_PTR(uint8 *);
        }
        memoryHeap = NULL_PTR(HeapI *);
    }
    if (signalOffsets != NULL_PTR(uint32 *)) {
        delete[] signalOffsets;
        signalOffsets = NULL_PTR(uint32 *);
    }

    if (packetSize != NULL_PTR(uint32 *)) {
        delete[] packetSize;
        packetSize = NULL_PTR(uint32 *);
    }

}

bool MultiBufferUnrelatedDataSource::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);

    if (ret) {
        //the total number of packets in the circular buffer
        ret = data.Read("NumberOfBuffers", numberOfBuffers);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "NumberOfBuffers was not specified");
        }
    }
    if (ret) {
        //the number of packets that arrives to the DS at each cycle
        ret = data.Read("TriggerAfterNPackets", triggerAfterNPackets);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "TriggerAfterNPackets not specified");
        }
    }
    if (ret) {
        StreamString heapName;
        if (data.Read("HeapName", heapName)) {
            memoryHeap = HeapManager::FindHeap(heapName.Buffer());
            if (memoryHeap == NULL_PTR(HeapI *)) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not instantiate an memoryHeap with the name: %s", heapName.Buffer());
                ret = false;
            }
        }
        else {
            memoryHeap = GlobalObjectsDatabase::Instance()->GetStandardHeap();
        }
    }
    return ret;
}

bool MultiBufferUnrelatedDataSource::AllocateMemory() {

    uint32 nOfSignals = GetNumberOfSignals();
    bool ret = (nOfSignals > 0u);
    if (ret) {
        ret = (mem == NULL_PTR(uint8 *));
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "No signals defined for DataSource with name %s", GetName());
    }
    if (ret) {
        signalOffsets = new uint32[nOfSignals];
        ret = (signalOffsets != NULL_PTR(uint32*));
        if (ret) {
            packetSize = new uint32[nOfSignals];
            ret = (packetSize != NULL_PTR(uint32*));
        }
    }

    uint32 memorySize = 0u;
    for (uint32 s = 0u; (s < nOfSignals) && (ret); s++) {
        uint32 thisSignalMemorySize;
        ret = GetSignalByteSize(s, thisSignalMemorySize);
        if (ret) {
            if (signalOffsets != NULL_PTR(uint32 *)) {
                signalOffsets[s] = memorySize;
            }
        }
        if (ret) {
            ret = (thisSignalMemorySize > 0u);
        }
        if (ret) {
            memorySize += (thisSignalMemorySize * numberOfBuffers);
            /*lint -e{613} null pointer checked before.*/
            packetSize[s] = thisSignalMemorySize;
        }
    }
    if (ret) {
        if (memoryHeap != NULL_PTR(HeapI *)) {
            mem = reinterpret_cast<uint8 *>(memoryHeap->Malloc(memorySize));
        }
        ret = MemoryOperationsHelper::Set(mem, '\0', memorySize);
    }
    return ret;

}

uint32 MultiBufferUnrelatedDataSource::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool MultiBufferUnrelatedDataSource::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                           const uint32 bufferIdx,
                                                           void *&signalAddress) {
    bool ret = (bufferIdx < 1u);
    uint32 nOfSignals = GetNumberOfSignals();
    if (ret) {
        ret = (signalIdx < nOfSignals);
    }

    if (ret) {
        char8 *signalAddressChar = reinterpret_cast<char8 *>(mem);
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

const char8 *MultiBufferUnrelatedDataSource::GetBrokerName(StructuredDataI &data,
                                                           const SignalDirection direction) {
    suggestedBrokerName = "";
    if (direction == InputSignals) {

        //get the node name
        StreamString signalName;
        //get the signal name
        if (data.Read("QualifiedName", signalName)) {
            if (signalName.Seek(0ull)) {
                suggestedBrokerName = "";
            }
            //if defined... get the suggested broker name
            for (uint32 i = 0u; i < signalName.Size(); i++) {
                if (signalName[i] == '$') {
                    uint32 index = (i + 1u);
                    suggestedBrokerName = &(signalName.Buffer()[index]);
                }
            }

            if (suggestedBrokerName == "") {
                REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "No suggested broker for signal %s, use by default MemoryMapUnrelatedInputBroker",
                                        signalName.Buffer());

                float32 frequency = 0.F;
                if (data.Read("Frequency", frequency)) {
                    if (frequency > 0.) {
                        suggestedBrokerName = "MemoryMapSyncUnrelatedInputBroker";
                    }
                }
                uint32 trigger = 0u;
                if (data.Read("Trigger", trigger)) {
                    if (trigger > 0u) {
                        suggestedBrokerName = "MemoryMapSyncUnrelatedInputBroker";
                    }
                }
                if (suggestedBrokerName == "") {
                    suggestedBrokerName = "MemoryMapUnrelatedInputBroker";
                }
            }
        }
    }

    if (direction == OutputSignals) {

        //get the node name
        StreamString signalName;
        //get the signal name
        if (data.Read("QualifiedName", signalName)) {
            if (signalName.Seek(0ull)) {
                suggestedBrokerName = "";
            }
            for (uint32 i = 0u; i < signalName.Size(); i++) {
                //if defined... get the suggested broker name
                if (signalName[i] == '$') {
                    uint32 index = (i + 1u);
                    suggestedBrokerName = &(signalName.Buffer()[index]);
                }
            }

            if (suggestedBrokerName == "") {
                REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "No suggested broker for signal %s, use by default MemoryMapUnrelatedOutputBroker",
                                        signalName.Buffer());

                float32 frequency = 0.F;
                if (data.Read("Frequency", frequency)) {
                    if (frequency > 0.) {
                        suggestedBrokerName = "MemoryMapSyncUnrelatedOutputBroker";
                    }
                }
                uint32 trigger = 0u;
                if (data.Read("Trigger", trigger)) {
                    if (trigger > 0u) {
                        suggestedBrokerName = "MemoryMapSyncUnrelatedOutputBroker";
                    }
                }
                if (suggestedBrokerName == "") {
                    suggestedBrokerName = "MemoryMapUnrelatedOutputBroker";
                }
            }
        }
    }

    return suggestedBrokerName.Buffer();
}

bool MultiBufferUnrelatedDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers,
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
            ret = configuredDatabase.Read("QualifiedName", signalName);
            uint32 signalIdx = 0u;
            if (ret) {
                ret = GetSignalIndex(signalIdx, signalName.Buffer());
            }
            if (ret) {
                bool ok = true;
                uint32 nBrokers = outputBrokers.Size();
                for (uint32 j = 0u; (j < nBrokers) && (ret) && (ok); j++) {
                    ReferenceT<MemoryMapUnrelatedOutputBroker> brokerRef = outputBrokers.Get(j);
                    if (brokerRef.IsValid()) {
                        if (suggestedBrokerNameIn == brokerRef->GetClassProperties()->GetName()) {
                            ok = false;
                        }
                    }
                }

                if (ok) {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "creating broker %s for %s and signal %s(%d)", suggestedBrokerNameIn.Buffer(),
                                            functionName, signalName.Buffer(), signalIdx);

                    ReferenceT<MemoryMapUnrelatedOutputBroker> broker(suggestedBrokerNameIn.Buffer());

                    ret = broker.IsValid();
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Broker %s not valid", suggestedBrokerName.Buffer());
                    }
                    if (ret) {
                        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failed broker %s Init", suggestedBrokerNameIn.Buffer());
                        }
                    }
                }
            }
        }
    }
    return ret;
}

bool MultiBufferUnrelatedDataSource::GetInputBrokers(ReferenceContainer &inputBrokers,
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
            ret = configuredDatabase.Read("QualifiedName", signalName);
            uint32 signalIdx = 0u;
            if (ret) {
                ret = GetSignalIndex(signalIdx, signalName.Buffer());
            }
            if (ret) {
                bool ok = true;
                uint32 nBrokers = inputBrokers.Size();
                for (uint32 j = 0u; (j < nBrokers) && (ret) && (ok); j++) {
                    ReferenceT<MemoryMapUnrelatedInputBroker> brokerRef = inputBrokers.Get(j);
                    if (brokerRef.IsValid()) {
                        if (suggestedBrokerNameIn == brokerRef->GetClassProperties()->GetName()) {
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
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failed broker %s Init", suggestedBrokerNameIn.Buffer());
                        }

                    }
                }
            }
        }
    }
    return ret;
}

bool MultiBufferUnrelatedDataSource::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);

    if (ret) {
        uint32 nOfSignals = GetNumberOfSignals();

        for (uint32 i = 0u; (i < nOfSignals) && (ret); i++) {
            StreamString signalName;
            ret = GetSignalName(i, signalName);
            if (ret) {
                if (signalName == "InternalTimeStamp") {
                    timeStampSignalIndex = i;
                    //check the type of the time stamp signal
                    TypeDescriptor td = GetSignalType(timeStampSignalIndex);
                    ret = (td == UnsignedInteger64Bit);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "The InternalTimeStamp signal type must be uint64");
                    }
                    //check that this signal is only consumed
                    if (ret) {
                        uint32 numberOfStates = 0u;
                        ret = GetSignalNumberOfStates(timeStampSignalIndex, numberOfStates);
                        for (uint32 j = 0u; (j < numberOfStates) && (ret); j++) {
                            StreamString stateName;
                            ret = GetSignalStateName(timeStampSignalIndex, j, stateName);
                            if (ret) {
                                uint32 numberOfProducers = 0u;
                                (void) GetSignalNumberOfProducers(timeStampSignalIndex, stateName.Buffer(), numberOfProducers);
                                ret = (numberOfProducers == 0u);
                            }
                        }
                    }
                }
            }
        }
    }
    return ret;
}

}
