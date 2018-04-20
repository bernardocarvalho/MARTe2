/**
 * @file InterleavedInputBroker.cpp
 * @brief Source file for class InterleavedInputBroker
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

 * @details This source file contains the definition of all the methods for
 * the class InterleavedInputBroker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "InterleavedInputBroker.h"
#include "MemoryOperationsHelper.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

InterleavedInputBroker::InterleavedInputBroker() :
        MemoryMapUnrelatedInputBroker() {
    numberOfPacketChunks = NULL_PTR(uint32 *);
    packetChunkSize = NULL_PTR(uint32 *);
    packetSize = NULL_PTR(uint32 *);

}

InterleavedInputBroker::~InterleavedInputBroker() {
    if (packetChunkSize != NULL_PTR(uint32 *)) {
        delete[] packetChunkSize;
        packetChunkSize = NULL_PTR(uint32 *);
    }
    if (numberOfPacketChunks != NULL_PTR(uint32 *)) {
        delete[] numberOfPacketChunks;
        numberOfPacketChunks = NULL_PTR(uint32 *);
    }

    if (packetSize != NULL_PTR(uint32 *)) {
        delete[] packetSize;
        packetSize = NULL_PTR(uint32 *);
    }
}

bool InterleavedInputBroker::Execute() {
    uint32 cnt = 0u;
    /*lint -e{613} null pointer checked before.*/
    uint32 currentBuffer = dataSource->GetCurrentBuffer();
    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
        for (uint32 n = 0u; (n < numberOfCopies); n++) {
            /*lint -e{613} null pointer checked before.*/
            int32 offset = dataSourceCust->GetOffset(signalIdxArr[n], 0);
            if (n > 0u) {
                uint32 index = (n - 1u);
                /*lint -e{613} null pointer checked before.*/
                cnt += numberOfPacketChunks[index];
            }
            if (offset >= 0) {
                uint32 dataSourceIndex = ((currentBuffer * numberOfCopies) + n);
                //do the split of the signals
                uint8 *originDest = reinterpret_cast<uint8 *>(copyTable[n].gamPointer);
                uint8 *originSource = &((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]);

                //this should be the number of samples
                /*lint -e{613} null pointer checked before.*/
                uint32 nPacketsInBox = (copyTable[n].copySize / packetSize[n]);
                for (uint32 k = 0u; k < nPacketsInBox; k++) {
                    //return to the gam pointet
                    uint8 *dest = originDest;
                    //go to the k-th packet in the data source
                    /*lint -e{613} null pointer checked before.*/
                    uint32 index = (k * packetSize[n]);
                    /*lint -e{613} null pointer checked before.*/
                    uint8 *source = &originSource[index];
                    /*lint -e{613} null pointer checked before.*/
                    for (uint32 i = 0u; i < numberOfPacketChunks[n]; i++) {
                        //get the chunk size
                        index = (cnt + i);
                        /*lint -e{613} null pointer checked before.*/
                        uint32 chunkSize = packetChunkSize[index];
                        //put the k-th element
                        index = (k * chunkSize);
                        dest = &dest[index];
                        (void) MemoryOperationsHelper::Copy(dest, source, chunkSize);
                        //skip the N-k other samples to go to the next chunk mem
                        index = ((nPacketsInBox - k) * chunkSize);
                        dest = &dest[index];
                        //get the next chunk in source
                        source = &source[chunkSize];
                    }
                }
                /*lint -e{613} null pointer checked before.*/
                dataSourceCust->TerminateRead(signalIdxArr[n], static_cast<uint32>(offset), 0);
            }
        }
    }
    return true;
}

bool InterleavedInputBroker::Init(const SignalDirection direction,
                                  DataSourceI &dataSourceIn,
                                  const char8 * const functionName,
                                  void * const gamMemoryAddress) {
    dataSource = &dataSourceIn;

    bool ret = InitFunctionPointers(direction, dataSourceIn, functionName, gamMemoryAddress);

    const ClassProperties * properties = GetClassProperties();
    if (ret) {
        ret = (properties != NULL);
    }
    const char8* brokerClassName = NULL_PTR(const char8*);
    if (ret) {
        brokerClassName = properties->GetName();
        ret = (brokerClassName != NULL);
    }

    if (ret) {
        ret = (numberOfCopies > 0u);
    }
    uint32 numberOfBuffers = dataSource->GetNumberOfMemoryBuffers();
    if (ret) {
        uint32 totalNumberOfElements = (numberOfCopies * numberOfBuffers);
        /*lint -e{423} copyTable is freed in MemoryMapBroker destructor.*/
        copyTable = new MemoryMapBrokerCopyTableEntry[totalNumberOfElements];
        ret = (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry*));
        if (ret) {
            signalIdxArr = new uint32[numberOfCopies];
            ret = (signalIdxArr != NULL_PTR(uint32*));
        }
    }
    uint32 functionIdx = 0u;
    if (ret) {
        ret = dataSource->GetFunctionIndex(functionIdx, functionName);
    }
    uint32 functionNumberOfSignals = 0u;
    if (ret) {
        ret = dataSource->GetFunctionNumberOfSignals(direction, functionIdx, functionNumberOfSignals);
    }

    //get the packet chunk sizes
    uint32 totalNumberOfPacketChunks = 0u;
    numberOfPacketChunks = new uint32[numberOfCopies];
    //The same signal can be copied from different ranges. A MemoryMapBrokerCopyTableEntry is added for each signal range.
    uint32 c = 0u;
    uint32 n;
    for (uint32 c0 = 0u; c0 < numberOfBuffers; c0++) {
        for (n = 0u; (n < functionNumberOfSignals) && (ret); n++) {
            if (dataSource->IsSupportedBroker(direction, functionIdx, n, brokerClassName)) {
                uint32 numberOfByteOffsets = 0u;
                ret = dataSource->GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, n, numberOfByteOffsets);

                StreamString functionSignalName;
                TypeDescriptor signalType;
                if (ret) {
                    ret = dataSource->GetFunctionSignalAlias(direction, functionIdx, n, functionSignalName);
                }
                uint32 signalIdx = 0u;
                if (ret) {
                    ret = dataSource->GetSignalIndex(signalIdx, functionSignalName.Buffer());
                    signalType = dataSource->GetSignalType(signalIdx);
                }

                //get the number of buffers
                //Take into account different ranges for the same signal
                uint32 bo;
                for (bo = 0u; (bo < numberOfByteOffsets) && (ret); bo++) {
                    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
                        uint32 numberOfSignalPacketChunks = 0u;
                        ret = dataSource->GetSignalPacketNumberOfChunks(signalIdx, numberOfSignalPacketChunks);
                        if (ret) {
                            numberOfPacketChunks[c % (numberOfCopies)] = numberOfSignalPacketChunks;
                            totalNumberOfPacketChunks += numberOfSignalPacketChunks;

                            copyTable[c].copySize = GetCopyByteSize(c % (numberOfCopies));
                            copyTable[c].gamPointer = GetFunctionPointer(c % (numberOfCopies));
                            copyTable[c].type = signalType;
                            uint32 dataSourceOffset = GetCopyOffset(c % (numberOfCopies));
                            /*lint -e{613} null pointer checked before.*/
                            signalIdxArr[c % (numberOfCopies)] = signalIdx;
                            void *dataSourceSignalAddress;
                            ret = dataSource->GetSignalMemoryBuffer(signalIdx, c0, dataSourceSignalAddress);
                            char8 *dataSourceSignalAddressChar = reinterpret_cast<char8 *>(dataSourceSignalAddress);
                            if (ret) {
                                dataSourceSignalAddressChar = &dataSourceSignalAddressChar[dataSourceOffset];
                                copyTable[c].dataSourcePointer = reinterpret_cast<void *>(dataSourceSignalAddressChar);
                            }
                        }
                    }
                    c++;
                }
            }
        }
    }
    if (ret) {
        packetChunkSize = new uint32[totalNumberOfPacketChunks];
        packetSize = new uint32[numberOfCopies];
    }

    uint32 cnt = 0u;
    for (uint32 i = 0u; (i < numberOfCopies) && (ret); i++) {
        uint32 packetTotSize = 0u;
        /*lint -e{613} null pointer checked before.*/
        for (uint32 d0 = 0u; (d0 < numberOfPacketChunks[i]) && (ret); d0++) {
            /*lint -e{613} null pointer checked before.*/
            ret = dataSource->GetSignalPacketChunkSize(signalIdxArr[i], d0, packetChunkSize[cnt]);
            if (ret) {
                packetTotSize += packetChunkSize[cnt];
                cnt++;
            }
        }
        if (ret) {
            /*lint -e{613} null pointer checked before.*/
            packetSize[i] = packetTotSize;
        }
    }

    if (ret) {
        dataSourceCust = dynamic_cast<MultiBufferUnrelatedDataSource *>(dataSource);
        ret = (dataSourceCust != NULL_PTR(MultiBufferUnrelatedDataSource *));
    }
    return ret;
}

CLASS_REGISTER(InterleavedInputBroker, "1.0")

}
