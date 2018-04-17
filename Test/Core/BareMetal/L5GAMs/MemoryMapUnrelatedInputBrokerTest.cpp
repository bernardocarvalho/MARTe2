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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

//need to create a dummy for tests
class MultiBufferUnrelatedDataSourceInstanceTest: public MultiBufferUnrelatedDataSource {

public:
    CLASS_REGISTER_DECLARATION()

MultiBufferUnrelatedDataSourceInstanceTest    ();

    ~MultiBufferUnrelatedDataSourceInstanceTest();

    virtual int32 GetOffset(const uint32 signalIdx,
            const int32 flag);

    virtual uint32 GetCurrentBuffer();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    virtual bool AllocateMemory();

    virtual void TerminateRead(const uint32 signalIdx,
            const uint32 offset);

    virtual void TerminateWrite(const uint32 signalIdx,
            const uint32 offset);

protected:

    uint32 currentOffsets[3];
    uint32 currentBuffer;
};

MultiBufferUnrelatedDataSourceInstanceTest::MultiBufferUnrelatedDataSourceInstanceTest() {
    currentOffsets[0] = 0u;
    currentOffsets[1] = 0u;
    currentOffsets[2] = 0u;
    currentBuffer = 0u;
}

MultiBufferUnrelatedDataSourceInstanceTest::~MultiBufferUnrelatedDataSourceInstanceTest() {

}

int32 MultiBufferUnrelatedDataSourceInstanceTest::GetOffset(const uint32 signalIdx,
                                                            const int32 flag) {

    return currentOffsets[signalIdx % 3];
}

uint32 MultiBufferUnrelatedDataSourceInstanceTest::GetNumberOfMemoryBuffers() {
    return 2u;
}

bool MultiBufferUnrelatedDataSourceInstanceTest::GetSignalMemoryBuffer(const uint32 signalIdx,
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

bool MultiBufferUnrelatedDataSourceInstanceTest::AllocateMemory() {

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
            //add the double buffer mem
            memorySize *= 2;
            mem = reinterpret_cast<uint8 *>(memoryHeap->Malloc(memorySize));
        }
        ret = MemoryOperationsHelper::Set(mem, '\0', memorySize);
    }
    if (ret) {
        uin32 *memint = (uint32 *) mem;
        for (uint32 i = 0u; i < (memorySize / 4); i++) {
            memint[i] = i;
        }
    }

    return ret;

}

void MultiBufferUnrelatedDataSourceInstanceTest::TerminateRead(const uint32 signalIdx,
                                                               const uint32 offset) {

    uint32 index = (signalIdx % 3);
    if (index == 0) {
        currentOffsets[index] += packetSize[index];
    }
    if (index == 1) {
        currentOffsets[index] += 2 * packetSize[index];
    }
    if (index == 3) {
        currentOffsets[index] += 3 * packetSize[index];
    }
    currentOffsets[index] %= numberOfBuffers;

}

uint32 MultiBufferUnrelatedDataSourceInstanceTest::GetCurrentBuffer() {
    return currentBuffer;
}

void MultiBufferUnrelatedDataSourceInstanceTest::TerminateWrite(const uint32 signalIdx,
                                                                const uint32 offset) {
    currentBuffer++;
    currentBuffer %= 2;
}

CLASS_REGISTER(MultiBufferUnrelatedDataSourceInstanceTest, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapInputBroker tests
 */
class MemoryMapUnrelatedInputBrokerTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapUnrelatedInputBrokerTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();
};

MemoryMapUnrelatedInputBrokerTestGAM1::MemoryMapInputBrokerTestGAM1() :
        GAM() {
}

bool MemoryMapUnrelatedInputBrokerTestGAM1::Setup() {
    return true;
}

bool MemoryMapUnrelatedInputBrokerTestGAM1::Execute() {
    return true;
}

CLASS_REGISTER(MemoryMapUnrelatedInputBrokerTestGAM1, "1.0")

class MemoryMapUnrelatedInputBrokerTestInputBroker: public MemoryMapUnrelatedInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

MemoryMapUnrelatedInputBrokerTestInputBroker    () {

    }

    virtual ~MemoryMapUnrelatedInputBrokerTestInputBroker() {

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
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 10"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
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
        "                   Samples = 5"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = MultiBufferUnrelatedDataSourceInstanceTest"
        "            NumberOfBuffers = 2"
        "        }"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
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

bool MemoryMapUnrelatedInputBrokerTest::TestInit() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;
}

bool MemoryMapUnrelatedInputBrokerTest::TestExecute() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    return ret;
}

