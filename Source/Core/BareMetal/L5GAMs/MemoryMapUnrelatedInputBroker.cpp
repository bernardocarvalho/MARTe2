/**
 * @file MemoryMapUnrelatedInputBroker.cpp
 * @brief Source file for class MemoryMapUnrelatedInputBroker
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
 * the class MemoryMapUnrelatedInputBroker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryMapUnrelatedInputBroker.h"
#include "MemoryOperationsHelper.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MemoryMapUnrelatedInputBroker::MemoryMapUnrelatedInputBroker() :
        MemoryMapBroker() {

    dataSourceCust = NULL_PTR(MultiBufferUnrelatedDataSource *);
    signalIdxArr = NULL_PTR(uint32 *);
    flag = NULL_PTR(uint32 *);
    samples = NULL_PTR(uint32 *);
    maxOffset = NULL_PTR(uint32 *);
}

MemoryMapUnrelatedInputBroker::~MemoryMapUnrelatedInputBroker() {
    if (signalIdxArr != NULL_PTR(uint32 *)) {
        delete[] signalIdxArr;
        signalIdxArr = NULL_PTR(uint32 *);
    }
    if (flag != NULL_PTR(uint32 *)) {
        delete[] flag;
        flag = NULL_PTR(uint32 *);
    }
    if (samples != NULL_PTR(uint32 *)) {
        delete[] samples;
        samples = NULL_PTR(uint32 *);
    }
    if (maxOffset != NULL_PTR(uint32 *)) {
        delete[] maxOffset;
        maxOffset = NULL_PTR(uint32 *);
    }
    dataSourceCust = NULL_PTR(MultiBufferUnrelatedDataSource *);
}

bool MemoryMapUnrelatedInputBroker::Execute() {
    uint32 n;
    /*lint -e{613} null pointer checked before.*/
    uint32 i = dataSource->GetCurrentBuffer();
    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
        for (n = 0u; (n < numberOfCopies); n++) {
            /*lint -e{613} null pointer checked before.*/
            int32 offset = dataSourceCust->GetOffset(signalIdxArr[n], samples[n], flag[n]);
            if (offset >= 0) {
                uint32 allowedSize = copyTable[n].copySize;
                //trap the circular buffer exception
                if ((GetCopyOffset(n) + offset) >= maxOffset[n]) {
                    offset = 0;
                }
                if (allowedSize > (maxOffset[n] - (GetCopyOffset(n) + offset))) {
                    allowedSize = (maxOffset[n] - offset);
                    uint32 dataSourceIndex = ((i * numberOfCopies) + n);
                    (void) MemoryOperationsHelper::Copy(copyTable[n].gamPointer,
                                                        &((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]), allowedSize);
                    offset = 0;
                    allowedSize = (copyTable[n].copySize - allowedSize);
                }

                uint32 dataSourceIndex = ((i * numberOfCopies) + n);
                (void) MemoryOperationsHelper::Copy(copyTable[n].gamPointer,
                                                    &((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]), allowedSize);
                /*lint -e{613} null pointer checked before.*/
                dataSourceCust->TerminateRead(signalIdxArr[n], static_cast<uint32>(offset), samples[n], flag[n]);
            }
        }
    }
    return true;
}

bool MemoryMapUnrelatedInputBroker::Init(const SignalDirection direction,
                                         DataSourceI &dataSourceIn,
                                         const char8 * const functionName,
                                         void * const gamMemoryAddress) {
    dataSource = &dataSourceIn;
    dataSourceCust = dynamic_cast<MultiBufferUnrelatedDataSource *>(dataSource);
    bool ret = (dataSourceCust != NULL_PTR(MultiBufferUnrelatedDataSource *));
    if (ret) {
        ret = InitFunctionPointers(direction, dataSourceIn, functionName, gamMemoryAddress);
    }
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
        if (ret) {
            flag = new uint32[numberOfCopies];
            ret = (flag != NULL_PTR(uint32*));
        }
        if (ret) {
            samples = new uint32[numberOfCopies];
            ret = (samples != NULL_PTR(uint32*));
        }

        if (ret) {
            maxOffset = new uint32[numberOfCopies];
            ret = (maxOffset != NULL_PTR(uint32*));
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
                uint32 trigger = 0u;
                if (ret) {
                    ret = dataSource->GetFunctionSignalTrigger(direction, functionIdx, n, trigger);
                }
                uint32 nSamples;
                if (ret) {
                    ret = dataSource->GetFunctionSignalSamples(direction, functionIdx, n, nSamples);
                }
                uint32 maxSignalOffset;
                if (ret) {
                    maxSignalOffset = dataSourceCust->GetNumberOfInternalBuffers();
                }

                if (ret) {
                    uint32 byteSize;
                    ret = dataSourceCust->GetSignalByteSize(signalIdx, byteSize);
                    maxSignalOffset *= byteSize;
                }
                //get the number of buffers
                //Take into account different ranges for the same signal
                uint32 bo;
                for (bo = 0u; (bo < numberOfByteOffsets) && (ret); bo++) {
                    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
                        copyTable[c].copySize = GetCopyByteSize(c % (numberOfCopies));
                        copyTable[c].gamPointer = GetFunctionPointer(c % (numberOfCopies));
                        copyTable[c].type = signalType;
                        uint32 dataSourceOffset = GetCopyOffset(c % (numberOfCopies));
                        /*lint -e{613} null pointer checked before.*/
                        signalIdxArr[c % (numberOfCopies)] = signalIdx;
                        flag[c % (numberOfCopies)] = trigger;
                        samples[c % (numberOfCopies)] = nSamples;
                        maxOffset[c % (numberOfCopies)] = maxSignalOffset;
                        void *dataSourceSignalAddress;
                        ret = dataSource->GetSignalMemoryBuffer(signalIdx, c0, dataSourceSignalAddress);
                        char8 *dataSourceSignalAddressChar = reinterpret_cast<char8 *>(dataSourceSignalAddress);
                        if (ret) {
                            dataSourceSignalAddressChar = &dataSourceSignalAddressChar[dataSourceOffset];
                            copyTable[c].dataSourcePointer = reinterpret_cast<void *>(dataSourceSignalAddressChar);
                        }
                    }
                    c++;
                }
            }
        }
    }

    return ret;
}

CLASS_REGISTER(MemoryMapUnrelatedInputBroker, "1.0")

}
