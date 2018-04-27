/**
 * @file MemoryMapSyncUnrelatedOutputBrokerTest.cpp
 * @brief Source file for class MemoryMapSyncUnrelatedOutputBrokerTest
 * @date 27 apr 2018
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
 * the class MemoryMapSyncUnrelatedOutputBrokerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryMapSyncUnrelatedOutputBrokerTest.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "MemoryMapUnrelatedOutputBroker.h"
#include "MemoryMapUnrelatedOutputBroker.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

//need to create a dummy for tests
class MemoryMapSyncUnrelatedOutputBrokerDSTest: public MultiBufferUnrelatedDataSource {

public:
    CLASS_REGISTER_DECLARATION()

MemoryMapSyncUnrelatedOutputBrokerDSTest    ();

    ~MemoryMapSyncUnrelatedOutputBrokerDSTest();

    virtual int32 GetOffset(const uint32 signalIdx,const uint32 samples,
            const uint32 flag);

    virtual uint32 GetCurrentBuffer();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual uint32 GetNumberOfInternalMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    virtual void TerminateRead(const uint32 signalIdx,
            const uint32 offset, const uint32 samples,const uint32 flag);

    virtual void TerminateWrite(const uint32 signalIdx,
            const uint32 offset, const uint32 samples,const uint32 flag);

    virtual bool IsSupportedBroker(const SignalDirection direction,
            const uint32 functionIdx,
            const uint32 functionSignalIdx,
            const char8* const brokerClassName);

    virtual bool Synchronise();

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);
protected:

    uint32 currentOffsets[3];
    uint32 currentBuffer;
    uint32 memorySize;
};

MemoryMapSyncUnrelatedOutputBrokerDSTest::MemoryMapSyncUnrelatedOutputBrokerDSTest() {
    currentOffsets[0] = 0u;
    currentOffsets[1] = 0u;
    currentOffsets[2] = 0u;
    currentBuffer = 0u;
    memorySize = 0u;
}

MemoryMapSyncUnrelatedOutputBrokerDSTest::~MemoryMapSyncUnrelatedOutputBrokerDSTest() {

}
bool MemoryMapSyncUnrelatedOutputBrokerDSTest::IsSupportedBroker(const SignalDirection direction,
                                                             const uint32 functionIdx,
                                                             const uint32 functionSignalIdx,
                                                             const char8* const brokerClassName) {
    bool ret = DataSourceI::IsSupportedBroker(direction, functionIdx, functionSignalIdx, brokerClassName);
    if (!ret) {
        if (direction == InputSignals) {
            ret = (StringHelper::Compare(brokerClassName, "MemoryMapSyncUnrelatedOutputBrokerTestInputBroker") == 0);
        }
        else {
            ret = (StringHelper::Compare(brokerClassName, "MemoryMapSyncUnrelatedOutputBrokerTestOutputBroker") == 0);
        }
    }
    return ret;
}

int32 MemoryMapSyncUnrelatedOutputBrokerDSTest::GetOffset(const uint32 signalIdx,
                                                          const uint32 samples,
                                                      const uint32 flag) {

    return currentOffsets[signalIdx % 3];
}

uint32 MemoryMapSyncUnrelatedOutputBrokerDSTest::GetNumberOfMemoryBuffers() {
    return 2u;
}

uint32 MemoryMapSyncUnrelatedOutputBrokerDSTest::GetNumberOfInternalMemoryBuffers() {
    return numberOfInternalBuffers;
}

bool MemoryMapSyncUnrelatedOutputBrokerDSTest::Synchronise() {
    currentBuffer++;
    currentBuffer %= 2;
    return true;
}

bool MemoryMapSyncUnrelatedOutputBrokerDSTest::PrepareNextState(const char8 * const currentStateName,
                                                            const char8 * const nextStateName) {
    return true;
}

bool MemoryMapSyncUnrelatedOutputBrokerDSTest::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                                 const uint32 bufferIdx,
                                                                 void *&signalAddress) {
    bool ret = (bufferIdx < 2u);
    uint32 nOfSignals = GetNumberOfSignals();
    uint32 bufferSize = 0u;
    if (ret) {
        for (uint32 i = 0u; i < nOfSignals; i++) {
            bufferSize += packetSize[i];
        }

        ret = (signalIdx < nOfSignals);
    }

    if (ret) {
        char8 *signalAddressChar = reinterpret_cast<char8 *>(mem);
        uint32 offset = 0u;
        if (signalOffsets != NULL_PTR(uint32 *)) {
            offset = signalOffsets[signalIdx];
        }
        if (signalAddressChar != NULL_PTR(char8 *)) {
            signalAddressChar = &signalAddressChar[bufferSize * bufferIdx + offset];
            signalAddress = reinterpret_cast<void *&>(signalAddressChar);
        }
    }

    return ret;
}

void MemoryMapSyncUnrelatedOutputBrokerDSTest::TerminateRead(const uint32 signalIdx,
                                                         const uint32 offset,
                                                         const uint32 samples,
                                                         const uint32 flag) {

    currentBuffer++;
    currentBuffer %= 2;
}

uint32 MemoryMapSyncUnrelatedOutputBrokerDSTest::GetCurrentBuffer() {
    return currentBuffer;
}

void MemoryMapSyncUnrelatedOutputBrokerDSTest::TerminateWrite(const uint32 signalIdx,
                                                          const uint32 offset,
                                                          const uint32 samples,
                                                          const uint32 flag) {
    uint32 index = (signalIdx % 3);
    if (index == 0) {
        currentOffsets[index] += packetSize[index];
    }
    if (index == 1) {
        currentOffsets[index] += 2 * packetSize[index];
    }
    if (index == 2) {
        currentOffsets[index] += 3 * packetSize[index];
    }
    currentOffsets[index] %= (packetSize[index] * numberOfInternalBuffers);

}

CLASS_REGISTER(MemoryMapSyncUnrelatedOutputBrokerDSTest, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapInputBroker tests
 */
class MemoryMapSyncUnrelatedOutputBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapSyncUnrelatedOutputBrokerTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();

    void *GetOutputMemoryBuffer();
private:
    uint32 totalSize;
};

MemoryMapSyncUnrelatedOutputBrokerTestGAM1::MemoryMapSyncUnrelatedOutputBrokerTestGAM1() :
        GAM() {
    totalSize = 0u;
}

bool MemoryMapSyncUnrelatedOutputBrokerTestGAM1::Setup() {

    uint32 numberOfSignals = GetNumberOfOutputSignals();

    totalSize = 0u;
    for (uint32 i = 0u; i < numberOfSignals; i++) {
        uint32 byteSize;
        GetSignalByteSize(OutputSignals, i, byteSize);
        uint32 nSamples = 0u;
        GetSignalNumberOfSamples(OutputSignals, i, nSamples);
        totalSize += (byteSize * nSamples);
    }

    uint32 *mem = (uint32 *) GetOutputSignalsMemory();
    for (uint32 i = 0u; i < (totalSize / 4); i++) {
        mem[i] = i;
    }

    return true;
}

bool MemoryMapSyncUnrelatedOutputBrokerTestGAM1::Execute() {
    return true;
}

void *MemoryMapSyncUnrelatedOutputBrokerTestGAM1::GetOutputMemoryBuffer() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(MemoryMapSyncUnrelatedOutputBrokerTestGAM1, "1.0")

class MemoryMapSyncUnrelatedOutputBrokerTestInputBroker: public MemoryMapUnrelatedOutputBroker {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapSyncUnrelatedOutputBrokerTestInputBroker    () {

    }

    virtual ~MemoryMapSyncUnrelatedOutputBrokerTestInputBroker() {

    }

};

CLASS_REGISTER(MemoryMapSyncUnrelatedOutputBrokerTestInputBroker, "1.0")

class MemoryMapSyncUnrelatedOutputBrokerTestOutputBroker: public MemoryMapUnrelatedOutputBroker {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapSyncUnrelatedOutputBrokerTestOutputBroker    () {

    }

    virtual ~MemoryMapSyncUnrelatedOutputBrokerTestOutputBroker() {

    }

    virtual uint32 *GetSignalIdxArr(uint32 &size) {
        size=numberOfCopies;
        return signalIdxArr;
    }

    virtual uint32 *GetSignalFlags(uint32 &size) {
        size=numberOfCopies;
        return flag;
    }
};

CLASS_REGISTER(MemoryMapSyncUnrelatedOutputBrokerTestOutputBroker, "1.0")

/**
 * MARTe configuration structure to test the MemoryMapInputBroker
 */
static const char8 * const config = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapSyncUnrelatedOutputBrokerTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Trigger = 5"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 10"
        "                   Type = uint32"
        "                   Trigger = 8"
        "                   Ranges = {{0, 0}, {2, 2}}"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Trigger = 2"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = MemoryMapSyncUnrelatedOutputBrokerDSTest"
        "            NumberOfBuffers = 2"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = MemoryMapInputBrokerTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MemoryMapSyncUnrelatedOutputBrokerTest::MemoryMapSyncUnrelatedOutputBrokerTest() {
    // Auto-generated constructor stub for MemoryMapSyncUnrelatedOutputBrokerTest
    // TODO Verify if manual additions are needed
}

MemoryMapSyncUnrelatedOutputBrokerTest::~MemoryMapSyncUnrelatedOutputBrokerTest() {
    // Auto-generated destructor stub for MemoryMapSyncUnrelatedOutputBrokerTest
    // TODO Verify if manual additions are needed
}

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapInputBrokerEnviroment(const char8 * const config) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT < RealTimeApplication > application;
    if (ok) {
        application = god->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

bool MemoryMapSyncUnrelatedOutputBrokerTest::TestConstructor() {
    MemoryMapSyncUnrelatedOutputBrokerTestOutputBroker brokerTest;
    uint32 nOfCopies = 0;
    uint32 *signalIdxArr = brokerTest.GetSignalIdxArr(nOfCopies);
    bool ret = nOfCopies == 0;
    if (ret) {
        ret = signalIdxArr == NULL;
    }
    if (ret) {
        uint32 *flags = brokerTest.GetSignalFlags(nOfCopies);
        ret = nOfCopies == 0;
        if (ret) {
            ret = flags == NULL;
        }
    }
    return ret;
}

bool MemoryMapSyncUnrelatedOutputBrokerTest::TestExecute() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    MemoryMapSyncUnrelatedOutputBrokerTestInputBroker brokerTest;

    ReferenceT<MemoryMapSyncUnrelatedOutputBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<MemoryMapSyncUnrelatedOutputBrokerTestGAM1> gam;
    ReferenceContainer brokers;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam->GetOutputBrokers(brokers);
    }

    ReferenceT < MemoryMapUnrelatedOutputBroker > broker;
    if (ret) {
        ret = brokers.Size() == 1;
        if (ret) {
            broker = brokers.Get(0);
            ret = broker.IsValid();
        }
    }

    uint32 *dataPtr = NULL;
    if (ret) {
        ret = dataSource->GetSignalMemoryBuffer(0, 0, (void*&) dataPtr);
    }

    if (ret) {
        ret = broker->Execute();
        uint32 nBuffers = dataSource->GetNumberOfInternalMemoryBuffers();

        if (ret) {
            ret = (nBuffers == 2);
            if (ret) {
                ret &= (dataPtr[0] == 0);
                ret &= (dataPtr[2] == 1);
                //call terminate read here because of range at shifts the buffer!!
                ret &= (dataPtr[4 + (2 % nBuffers) * 10] == 2);
                ret &= (dataPtr[22] == 3);
            }
        }

        if (ret) {
            ret = broker->Execute();
            uint32 bufferShift = 12;
            if (ret) {
                ret &= (dataPtr[0 + (1 % nBuffers) + bufferShift] == 0);
                ret &= (dataPtr[2 + (4 % nBuffers) * 10 + bufferShift] == 1);
                //call terminate read here!!
                ret &= (dataPtr[4 + (6 % nBuffers) * 10 + bufferShift] == 2);
                ret &= (dataPtr[22 + (3 % nBuffers) + bufferShift] == 3);
            }
        }

    }

    return ret;
}

