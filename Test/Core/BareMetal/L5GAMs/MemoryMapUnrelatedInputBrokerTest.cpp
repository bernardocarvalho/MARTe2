/**
 * @file MemoryMapUnrelatedInputBrokerTest.cpp
 * @brief Source file for class MemoryMapUnrelatedInputBrokerTest
 * @date Apr 16, 2018
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
 * the class MemoryMapUnrelatedInputBrokerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryMapUnrelatedInputBrokerTest.h"
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
class MemoryMapUnrelatedInputBrokerDSTest: public MultiBufferUnrelatedDataSource {

public:
    CLASS_REGISTER_DECLARATION()

MemoryMapUnrelatedInputBrokerDSTest    ();

    ~MemoryMapUnrelatedInputBrokerDSTest();

    virtual int32 GetInputOffset(const uint32 signalIdx,const uint32 samples);

    virtual int32 GetOutputOffset(const uint32 signalIdx,const uint32 samples);

    virtual uint32 GetCurrentBuffer();

    virtual void PrepareInputOffsets();

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

MemoryMapUnrelatedInputBrokerDSTest::MemoryMapUnrelatedInputBrokerDSTest() {
    currentOffsets[0] = 0u;
    currentOffsets[1] = 0u;
    currentOffsets[2] = 0u;
    currentBuffer = 0u;
    memorySize = 0u;
}

MemoryMapUnrelatedInputBrokerDSTest::~MemoryMapUnrelatedInputBrokerDSTest() {

}
bool MemoryMapUnrelatedInputBrokerDSTest::IsSupportedBroker(const SignalDirection direction,
                                                            const uint32 functionIdx,
                                                            const uint32 functionSignalIdx,
                                                            const char8* const brokerClassName) {
    bool ret = DataSourceI::IsSupportedBroker(direction, functionIdx, functionSignalIdx, brokerClassName);
    if (!ret) {
        if (direction == InputSignals) {
            ret = (StringHelper::Compare(brokerClassName, "MemoryMapUnrelatedInputBrokerTestInputBroker") == 0);
        }
        else {
            ret = (StringHelper::Compare(brokerClassName, "MemoryMapUnrelatedInputBrokerTestOutputBroker") == 0);
        }
    }
    return ret;
}

int32 MemoryMapUnrelatedInputBrokerDSTest::GetInputOffset(const uint32 signalIdx,
                                                     const uint32 samples) {

    return currentOffsets[signalIdx % 3];
}

int32 MemoryMapUnrelatedInputBrokerDSTest::GetOutputOffset(const uint32 signalIdx,
                                                     const uint32 samples) {

    return currentOffsets[signalIdx % 3];
}

uint32 MemoryMapUnrelatedInputBrokerDSTest::GetNumberOfMemoryBuffers() {
    return 2u;
}

uint32 MemoryMapUnrelatedInputBrokerDSTest::GetNumberOfInternalMemoryBuffers() {
    return numberOfInternalBuffers;
}

bool MemoryMapUnrelatedInputBrokerDSTest::Synchronise() {
    return true;
}

bool MemoryMapUnrelatedInputBrokerDSTest::PrepareNextState(const char8 * const currentStateName,
                                                           const char8 * const nextStateName) {
    return true;
}

bool MemoryMapUnrelatedInputBrokerDSTest::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                                const uint32 bufferIdx,
                                                                void *&signalAddress) {
    bool ret = (bufferIdx < 2u);
    uint32 nOfSignals = GetNumberOfSignals();
    uint32 bufferSize = 0u;
    if (ret) {
        for (uint32 i = 0u; i < nOfSignals; i++) {
            bufferSize += packetSize[i] * numberOfInternalBuffers;
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

bool MemoryMapUnrelatedInputBrokerDSTest::AllocateMemory() {

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

void MemoryMapUnrelatedInputBrokerDSTest::TerminateRead(const uint32 signalIdx,
                                                        const uint32 offset,
                                                        const uint32 samples) {

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

uint32 MemoryMapUnrelatedInputBrokerDSTest::GetCurrentBuffer() {
    return currentBuffer;
}

void MemoryMapUnrelatedInputBrokerDSTest::PrepareInputOffsets() {
    currentBuffer++;
    currentBuffer %= 2;
}

void MemoryMapUnrelatedInputBrokerDSTest::TerminateWrite(const uint32 signalIdx,
                                                         const uint32 offset,
                                                         const uint32 samples) {
}

CLASS_REGISTER(MemoryMapUnrelatedInputBrokerDSTest, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapInputBroker tests
 */
class MemoryMapUnrelatedInputBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapUnrelatedInputBrokerTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();

    void *GetInputMemoryBuffer();
};

MemoryMapUnrelatedInputBrokerTestGAM1::MemoryMapUnrelatedInputBrokerTestGAM1() :
        GAM() {
}

bool MemoryMapUnrelatedInputBrokerTestGAM1::Setup() {
    return true;
}

bool MemoryMapUnrelatedInputBrokerTestGAM1::Execute() {
    return true;
}

void *MemoryMapUnrelatedInputBrokerTestGAM1::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

CLASS_REGISTER(MemoryMapUnrelatedInputBrokerTestGAM1, "1.0")

class MemoryMapUnrelatedInputBrokerTestInputBroker: public MemoryMapUnrelatedInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapUnrelatedInputBrokerTestInputBroker    () {

    }

    virtual ~MemoryMapUnrelatedInputBrokerTestInputBroker() {

    }

    virtual uint32 *GetSignalIdxArr(uint32 &size) {
        size=numberOfCopies;
        return signalIdxArr;
    }

};

CLASS_REGISTER(MemoryMapUnrelatedInputBrokerTestInputBroker, "1.0")

class MemoryMapUnrelatedInputBrokerTestOutputBroker: public MemoryMapUnrelatedOutputBroker {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapUnrelatedInputBrokerTestOutputBroker    () {

    }

    virtual ~MemoryMapUnrelatedInputBrokerTestOutputBroker() {

    }
};

CLASS_REGISTER(MemoryMapUnrelatedInputBrokerTestOutputBroker, "1.0")

/**
 * MARTe configuration structure to test the MemoryMapInputBroker
 */
static const char8 * const config = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MemoryMapUnrelatedInputBrokerTestGAM1"
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
        "            Class = MemoryMapUnrelatedInputBrokerTestGAM1"
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
        "            Class = MemoryMapUnrelatedInputBrokerDSTest"
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

MemoryMapUnrelatedInputBrokerTest::MemoryMapUnrelatedInputBrokerTest() {
    // Auto-generated constructor stub for MemoryMapUnrelatedInputBrokerTest
    // TODO Verify if manual additions are needed
}

MemoryMapUnrelatedInputBrokerTest::~MemoryMapUnrelatedInputBrokerTest() {
    // Auto-generated destructor stub for MemoryMapUnrelatedInputBrokerTest
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

bool MemoryMapUnrelatedInputBrokerTest::TestConstructor() {
    MemoryMapUnrelatedInputBrokerTestInputBroker brokerTest;
    uint32 nOfCopies = 0;
    uint32 *signalIdxArr = brokerTest.GetSignalIdxArr(nOfCopies);
    bool ret = nOfCopies == 0;
    if (ret) {
        ret = signalIdxArr == NULL;
    }

    return ret;
}

bool MemoryMapUnrelatedInputBrokerTest::TestExecute() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = MemoryMapUnrelatedInputBrokerTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Type = uint32"
            "                   Ranges = {{0, 0}, {2, 2}}"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = MemoryMapUnrelatedInputBrokerDSTest"
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
    MemoryMapUnrelatedInputBrokerTestInputBroker brokerTest;

    ReferenceT<MemoryMapUnrelatedInputBrokerDSTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<MemoryMapUnrelatedInputBrokerTestGAM1> gam;
    ReferenceContainer brokers;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam->GetInputBrokers(brokers);
    }

    ReferenceT<MemoryMapUnrelatedInputBroker> broker;
    if (ret) {
        ret = brokers.Size() == 1;
        if (ret) {
            broker = brokers.Get(0);
            ret = broker.IsValid();
        }
    }

    uint32 *gamPtr = (uint32*) gam->GetInputMemoryBuffer();
    if (ret) {
        ret = broker->Execute();
        uint32 nBuffers = dataSource->GetNumberOfInternalMemoryBuffers();
        uint32 signal4Shift = 2;    //2 buffers
        uint32 bufferOffset = 26;

        if (ret) {
            ret = (nBuffers == 2);
            if (ret) {
                ret &= (gamPtr[0] == 0 + signal4Shift + bufferOffset);
                ret &= (gamPtr[1] == 2 + signal4Shift + bufferOffset);
                //call terminate read here because of range at shifts the buffer!!
                //signal 2 shift of 3!
                ret &= (gamPtr[2] == 4 + (3 % nBuffers) * 10 + signal4Shift + bufferOffset);
                //signal 3 shift of 4%2=21!
                ret &= (gamPtr[3] == 22 + signal4Shift + bufferOffset);
            }
        }
        bufferOffset = 0;
        if (ret) {
            ret = broker->Execute();
            if (ret) {
                ret &= (gamPtr[0] == 0 + (2 % nBuffers) + signal4Shift + bufferOffset);
                ret &= (gamPtr[1] == 2 + (6 % nBuffers) * 10 + signal4Shift + bufferOffset);
                //call terminate read here!!
                ret &= (gamPtr[2] == 4 + (9 % nBuffers) * 10 + signal4Shift + bufferOffset);
                ret &= (gamPtr[3] == 22 + (1 % nBuffers) + signal4Shift + bufferOffset);
            }
        }

    }

    return ret;
}

