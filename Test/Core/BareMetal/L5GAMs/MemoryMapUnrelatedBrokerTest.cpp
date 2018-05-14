/**
 * @file MemoryMapUnrelatedBrokerTest.cpp
 * @brief Source file for class MemoryMapUnrelatedBrokerTest
 * @date 04 mag 2018
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
 * the class MemoryMapUnrelatedBrokerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryMapUnrelatedBrokerTest.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "MemoryMapUnrelatedInputBroker.h"
#include "MemoryMapUnrelatedOutputBroker.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

//need to create a dummy for tests
class MemoryMapUnrelatedBrokerDSTest: public MultiBufferUnrelatedDataSource {

public:
    CLASS_REGISTER_DECLARATION()

MemoryMapUnrelatedBrokerDSTest    ();

    ~MemoryMapUnrelatedBrokerDSTest();

    virtual int32 GetInputOffset(const uint32 signalIdx,const uint32 samples);

    virtual int32 GetOutputOffset(const uint32 signalIdx,const uint32 samples);

    virtual uint32 GetCurrentBuffer();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual uint32 GetNumberOfInternalMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    virtual bool AllocateMemory();

    virtual void TerminateRead(const uint32 signalIdx,
            const uint32 offset, const uint32 samples);

    virtual void TerminateWrite(const uint32 signalIdx,
            const uint32 offset, const uint32 samples);

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

MemoryMapUnrelatedBrokerDSTest::MemoryMapUnrelatedBrokerDSTest() {
    currentOffsets[0] = 0u;
    currentOffsets[1] = 0u;
    currentOffsets[2] = 0u;
    currentBuffer = 0u;
    memorySize = 0u;
}

MemoryMapUnrelatedBrokerDSTest::~MemoryMapUnrelatedBrokerDSTest() {

}
bool MemoryMapUnrelatedBrokerDSTest::IsSupportedBroker(const SignalDirection direction,
                                                       const uint32 functionIdx,
                                                       const uint32 functionSignalIdx,
                                                       const char8* const brokerClassName) {
    bool ret = DataSourceI::IsSupportedBroker(direction, functionIdx, functionSignalIdx, brokerClassName);
    if (!ret) {
        if (direction == InputSignals) {
            ret = (StringHelper::Compare(brokerClassName, "MemoryMapUnrelatedBrokerTestInputBroker") == 0);
        }
        else {
            ret = (StringHelper::Compare(brokerClassName, "MemoryMapUnrelatedBrokerTestOutputBroker") == 0);
        }
    }
    return ret;
}

int32 MemoryMapUnrelatedBrokerDSTest::GetInputOffset(const uint32 signalIdx,
                                                     const uint32 samples) {

    return currentOffsets[signalIdx % 3];
}

int32 MemoryMapUnrelatedBrokerDSTest::GetOutputOffset(const uint32 signalIdx,
                                                      const uint32 samples) {

    return currentOffsets[signalIdx % 3];
}

uint32 MemoryMapUnrelatedBrokerDSTest::GetNumberOfMemoryBuffers() {
    return 2u;
}

uint32 MemoryMapUnrelatedBrokerDSTest::GetNumberOfInternalMemoryBuffers() {
    return numberOfInternalBuffers;
}

bool MemoryMapUnrelatedBrokerDSTest::Synchronise() {
    for (uint32 i = 0u; i < numberOfSignals; i++) {
        currentOffsets[i] += packetSize[i];
        currentOffsets[i] %= (packetSize[i] * numberOfInternalBuffers);
    }
    return true;
}

bool MemoryMapUnrelatedBrokerDSTest::PrepareNextState(const char8 * const currentStateName,
                                                      const char8 * const nextStateName) {
    return true;
}

bool MemoryMapUnrelatedBrokerDSTest::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                           const uint32 bufferIdx,
                                                           void *&signalAddress) {
    bool ret = (bufferIdx < 2u);
    uint32 nOfSignals = GetNumberOfSignals();
    uint32 bufferSize = 0u;
    if (ret) {
        for (uint32 i = 0u; i < nOfSignals; i++) {
            bufferSize += packetSize[i]*numberOfInternalBuffers;
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

bool MemoryMapUnrelatedBrokerDSTest::AllocateMemory() {

    uint32 nOfSignals = GetNumberOfSignals();
    bool ret = (mem == NULL_PTR(uint8 *));
    if (ret) {
        if (nOfSignals > 0u) {
            signalOffsets = new uint32[nOfSignals];
            ret = (signalOffsets != NULL_PTR(uint32*));
            if (ret) {
                packetSize = new uint32[nOfSignals];
                ret = (packetSize != NULL_PTR(uint32*));
            }
        }
    }

    memorySize = 0u;
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
            memorySize += (thisSignalMemorySize * numberOfInternalBuffers);
            /*lint -e{613} null pointer checked before.*/
            packetSize[s] = thisSignalMemorySize;
        }
    }
    if (ret) {
        memorySize *= 2;
        if (memoryHeap != NULL_PTR(HeapI *)) {
            mem = reinterpret_cast<uint8 *>(memoryHeap->Malloc(memorySize));
        }
        ret = MemoryOperationsHelper::Set(mem, '\0', memorySize);
    }

    if (ret) {
        uint32 *memint = (uint32 *) mem;
        for (uint32 i = 0u; i < (memorySize / 4); i++) {
            memint[i] = i;
        }
    }

    return ret;

}

void MemoryMapUnrelatedBrokerDSTest::TerminateRead(const uint32 signalIdx,
                                                   const uint32 offset,
                                                   const uint32 samples) {

}

uint32 MemoryMapUnrelatedBrokerDSTest::GetCurrentBuffer() {
    return currentBuffer;
}

void MemoryMapUnrelatedBrokerDSTest::TerminateWrite(const uint32 signalIdx,
                                                    const uint32 offset,
                                                    const uint32 samples) {
}

CLASS_REGISTER(MemoryMapUnrelatedBrokerDSTest, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapInputBroker tests
 */
class MemoryMapUnrelatedBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapUnrelatedBrokerTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();

    void *GetInputMemoryBuffer();
};

MemoryMapUnrelatedBrokerTestGAM1::MemoryMapUnrelatedBrokerTestGAM1() :
        GAM() {
}

bool MemoryMapUnrelatedBrokerTestGAM1::Setup() {

    uint32 numberOfSignals = GetNumberOfOutputSignals();

    uint32 totalSize = 0u;
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

bool MemoryMapUnrelatedBrokerTestGAM1::Execute() {
    return true;
}

void *MemoryMapUnrelatedBrokerTestGAM1::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

CLASS_REGISTER(MemoryMapUnrelatedBrokerTestGAM1, "1.0")

class MemoryMapUnrelatedBrokerTestInputBroker: public MemoryMapUnrelatedInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapUnrelatedBrokerTestInputBroker    () {

    }

    virtual ~MemoryMapUnrelatedBrokerTestInputBroker() {

    }

    virtual uint32 *GetSignalIdxArr(uint32 &size) {
        size=numberOfCopies;
        return signalIdxArr;
    }

};

CLASS_REGISTER(MemoryMapUnrelatedBrokerTestInputBroker, "1.0")

class MemoryMapUnrelatedBrokerTestOutputBroker: public MemoryMapUnrelatedOutputBroker {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapUnrelatedBrokerTestOutputBroker    () {

    }

    virtual ~MemoryMapUnrelatedBrokerTestOutputBroker() {

    }
};

CLASS_REGISTER(MemoryMapUnrelatedBrokerTestOutputBroker, "1.0")

/**
 * MARTe configuration structure to test the MemoryMapInputBroker
 */
static const char8 * const config = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapUnrelatedBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Trigger = 10"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 10"
        "                   Type = uint32"
        "                   Trigger = 3"
        "                   Ranges = {{0, 0}, {2, 2}}"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Trigger = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = MemoryMapUnrelatedBrokerTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0, 0}, {9, 9}}"
        "                   Type = uint32"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = MemoryMapUnrelatedBrokerDSTest"
        "            NumberOfBuffers = 2"
        "            Signals = {"
        "               Signal4 = {"
        "                   Type = uint32"
        "               }"
        "            }"
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
        "                    Functions = {GAMA GAMB}"
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

MemoryMapUnrelatedBrokerTest::MemoryMapUnrelatedBrokerTest() {
    // Auto-generated constructor stub for MemoryMapUnrelatedBrokerTest
    // TODO Verify if manual additions are needed
}

MemoryMapUnrelatedBrokerTest::~MemoryMapUnrelatedBrokerTest() {
    // Auto-generated destructor stub for MemoryMapUnrelatedBrokerTest
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
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

bool MemoryMapUnrelatedBrokerTest::TestConstructor() {
    MemoryMapUnrelatedBrokerTestInputBroker brokerTest;
    uint32 nOfCopies = 0;
    uint32 *signalIdxArr = brokerTest.GetSignalIdxArr(nOfCopies);
    bool ret = nOfCopies == 0;
    if (ret) {
        ret = signalIdxArr == NULL;
    }

    return ret;
}

bool MemoryMapUnrelatedBrokerTest::TestInit() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    MemoryMapUnrelatedBrokerTestInputBroker brokerTest;

    ReferenceT<MemoryMapUnrelatedBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    MemoryMapUnrelatedBrokerDSTest *dataSourceInstance = dataSource.operator->();

    ReferenceT<MemoryMapUnrelatedBrokerTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        void* gamPtr = gam->GetInputMemoryBuffer();
        ret = brokerTest.Init(InputSignals, *dataSourceInstance, "GAMA", gamPtr);
    }

    if (ret) {
        uint32 nOfCopies = 0;
        uint32 *signalIdxArr = brokerTest.GetSignalIdxArr(nOfCopies);
        ret = (signalIdxArr != NULL);
        if (ret) {
            ret = (nOfCopies == 4);
        }
        if (ret) {
            ret = (signalIdxArr[0] == 1);
            ret &= (signalIdxArr[1] == 2);
            ret &= (signalIdxArr[2] == 2);
            ret &= (signalIdxArr[3] == 3);
        }
    }

    return ret;
}

bool MemoryMapUnrelatedBrokerTest::TestInit1() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    MemoryMapUnrelatedBrokerTestInputBroker brokerTest;

    ReferenceT<MemoryMapUnrelatedBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    MemoryMapUnrelatedBrokerDSTest *dataSourceInstance = dataSource.operator->();

    ReferenceT<MemoryMapUnrelatedBrokerTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gam.IsValid();
    }
    if (ret) {
        void* gamPtr = gam->GetInputMemoryBuffer();
        ret = brokerTest.Init(InputSignals, *dataSourceInstance, "GAMB", gamPtr);
    }

    if (ret) {
        uint32 nOfCopies = 0;
        uint32 *signalIdxArr = brokerTest.GetSignalIdxArr(nOfCopies);
        ret = (signalIdxArr != NULL);
        if (ret) {
            ret = (nOfCopies == 2);
        }
        if (ret) {
            ret = (signalIdxArr[0] == 2);
            ret &= (signalIdxArr[1] == 2);
        }
    }

    return ret;
}

bool MemoryMapUnrelatedBrokerTest::TestExecuteCoreIn() {
    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = MemoryMapUnrelatedBrokerTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   Type = uint32"
            "                   Ranges = {{0, 0}, {2, 2}}"
            "                   Samples = 3"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = MemoryMapUnrelatedBrokerDSTest"
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);
    MemoryMapUnrelatedBrokerTestInputBroker brokerTest;

    ReferenceT<MemoryMapUnrelatedBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<MemoryMapUnrelatedBrokerTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    ReferenceContainer brokers;
    ReferenceT<MemoryMapUnrelatedBroker> broker;
    uint32* data = NULL;
    if (ret) {
        gam->GetInputBrokers(brokers);
        broker = brokers.Get(0);
        data = (uint32*) gam->GetInputMemoryBuffer();
    }

    if (ret) {
        broker->ExecuteCoreIn();
        uint32 nSamples = 9;
        for (uint32 i = 0u; i < nSamples; i++) {
            printf("data[%d]=%d\n", i, data[i]);
        }
        ret = (data[0] == 0);
        ret &= data[1] == 1;
        ret &= data[2] == 0;
        ret &= data[3] == 2;
        ret &= data[4] == 5;
        ret &= data[5] == 2;
        ret &= data[6] == 4;
        ret &= data[7] == 7;
        ret &= data[8] == 4;
    }

    if (ret) {
        dataSource->Synchronise();
        broker->ExecuteCoreIn();
        uint32 nSamples = 9;
        for (uint32 i = 0u; i < nSamples; i++) {
            printf("data[%d]=%d\n", i, data[i]);
        }
        ret = (data[0] == 1);
        ret &= data[1] == 0;
        ret &= data[2] == 1;
        ret &= data[3] == 5;
        ret &= data[4] == 2;
        ret &= data[5] == 5;
        ret &= data[6] == 7;
        ret &= data[7] == 4;
        ret &= data[8] == 7;
    }

    return ret;
}

bool MemoryMapUnrelatedBrokerTest::TestExecuteCoreOut() {
    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = MemoryMapUnrelatedBrokerTestGAM1"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   Type = uint32"
            "                   Ranges = {{0, 0}, {2, 2}}"
            "                   Samples = 3"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = MemoryMapUnrelatedBrokerDSTest"
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);
    MemoryMapUnrelatedBrokerTestInputBroker brokerTest;

    ReferenceT<MemoryMapUnrelatedBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<MemoryMapUnrelatedBrokerTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    ReferenceContainer brokers;
    ReferenceT<MemoryMapUnrelatedBroker> broker;
    uint32* data = NULL;
    if (ret) {
        gam->GetOutputBrokers(brokers);
        broker = brokers.Get(0);
        dataSource->GetSignalMemoryBuffer(0, 0, (void*&) data);
    }

    if (ret) {
        broker->ExecuteCoreOut();
        uint32 nSamples = 8;
        for (uint32 i = 0u; i < nSamples; i++) {
            printf("data[%d]=%d\n", i, data[i]);
        }
        ret = (data[0] == 2);
        ret &= data[1] == 1;
        ret &= data[2] == 5;
        ret &= data[3] == 3;
        ret &= data[4] == 8;
        ret &= data[5] == 4;
        ret &= data[6] == 6;
        ret &= data[7] == 7;

    }
    if (ret) {
        dataSource->Synchronise();
        broker->ExecuteCoreOut();
        uint32 nSamples = 8;
        for (uint32 i = 0u; i < nSamples; i++) {
            printf("data[%d]=%d\n", i, data[i]);
        }
        ret = (data[0] == 1);
        ret &= data[1] == 2;
        ret &= data[2] == 4;
        ret &= data[3] == 3;
        ret &= data[4] == 7;
        ret &= data[5] == 5;
        ret &= data[6] == 6;
        ret &= data[7] == 8;
    }
    return ret;
}

