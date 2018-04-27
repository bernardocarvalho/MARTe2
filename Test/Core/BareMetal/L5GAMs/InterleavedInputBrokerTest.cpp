/**
 * @file InterleavedInputBrokerTest.cpp
 * @brief Source file for class InterleavedInputBrokerTest
 * @date 23 apr 2018
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
 * the class InterleavedInputBrokerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "InterleavedInputBrokerTest.h"
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

class InterleavedInputBrokerTestBroker: public InterleavedInputBroker{
    CLASS_REGISTER_DECLARATION()
    InterleavedInputBrokerTestBroker();
    virtual ~InterleavedInputBrokerTestBroker();

    uint32 GetNumberOfPacketChunks(uint32 i);

    uint32 GetPacketChunkSize(uint32 i);

    uint32 GetPacketSize(uint32 i);

};



uint32 InterleavedInputBrokerTestBroker::GetNumberOfPacketChunks(uint32 i){
    return numberOfPacketChunks[i];
}

uint32 InterleavedInputBrokerTestBroker::GetPacketChunkSize(uint32 i){
    return packetChunkSize[i];
}

uint32 InterleavedInputBrokerTestBroker::GetPacketSize(uint32 i){
    return packetSize[i];
}


class InterleavedInputBrokerTestDS: public MultiBufferUnrelatedDataSource {

public:
    CLASS_REGISTER_DECLARATION()InterleavedInputBrokerTestDS();

    virtual ~InterleavedInputBrokerTestDS();

    virtual int32 GetOffset(const uint32 signalIdx,
            const int32 flag);

    virtual void TerminateRead(const uint32 signalIdx,
            const uint32 offset,
            const int32 flag);

    virtual void TerminateWrite(const uint32 signalIdx,
            const uint32 offset,
            const int32 flag);

    virtual bool AllocateMemory();

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    virtual bool Synchronise();
};

InterleavedInputBrokerTestDS::InterleavedInputBrokerTestDS() {

}

InterleavedInputBrokerTestDS::~InterleavedInputBrokerTestDS() {

}

bool InterleavedInputBrokerTestDS::AllocateMemory() {
    bool ret = MultiBufferUnrelatedDataSource::AllocateMemory();
    if (ret) {
        //fill the packets for 3 samples
        uint8 k = 0u;
        uint64 pcounter = 0ull;
        uint16 *ptr16 = (uint16 *) mem;
        for (uint32 i = 0u; i < 3u; i++) {
            uint64* ptr64 = (uint64*) ptr16;
            *ptr64 = pcounter;
            pcounter++;
            ptr64++;
            ptr16 = (uint16 *) ptr64;
            for (uint32 j = 0u; j < 10; j++) {
                *ptr16 = k;
                ptr16++;
                k++;
            }
            uint32 *ptr32 = (uint32 *) ptr16;
            for (uint32 j = 0u; j < 2; j++) {
                *ptr32 = k;
                ptr32++;
                k++;
            }

            uint8 *ptr8 = (uint8 *) ptr32;
            for (uint32 j = 0u; j < 2; j++) {
                *ptr8 = k;
                ptr8++;
                k++;
            }

            ptr16 = (uint16 *) ptr8;
            *ptr16 = k;
            ptr16++;
            k++;
        }

        //second signal
        uint64* ptr64 = (uint64*) ptr16;
        for (uint32 j = 0u; j < 4; j++) {
            *ptr64 = 100 + j;
            ptr64++;
        }

    }
    return ret;
}

bool InterleavedInputBrokerTestDS::PrepareNextState(const char8 * const currentStateName,
                                                    const char8 * const nextStateName) {
    return true;
}

bool InterleavedInputBrokerTestDS::Synchronise() {
    return true;
}

int32 InterleavedInputBrokerTestDS::GetOffset(const uint32 signalIdx,
                                              const int32 flag) {
    return 0;

}

void InterleavedInputBrokerTestDS::TerminateRead(const uint32 signalIdx,
                                                 const uint32 offset,
                                                 const int32 flag) {
    return;
}

void InterleavedInputBrokerTestDS::TerminateWrite(const uint32 signalIdx,
                                                  const uint32 offset,
                                                  const int32 flag) {
    return;
}

CLASS_REGISTER(InterleavedInputBrokerTestDS, "1.0")

class InterleavedInputBrokerTestGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

InterleavedInputBrokerTestGAM    ();
    virtual bool Setup();
    virtual bool Execute();

    void *GetInputSignalsMemory1();

    void *GetOutputSignalsMemory1();

protected:
    uint32 totalSignalsByteSize;

};

InterleavedInputBrokerTestGAM::InterleavedInputBrokerTestGAM() :
        GAM() {
    totalSignalsByteSize = 0u;

}
bool InterleavedInputBrokerTestGAM::Setup() {
    bool ret = true;
    uint32 n;
    totalSignalsByteSize = 0u;
    uint32 totalSignalOutSize = 0u;

    for (n = 0u; (n < GetNumberOfInputSignals()) && (ret); n++) {
        uint32 inByteSize = 0u;
        uint32 inSamples = 1u;
        uint32 idx = n;
        ret = GetSignalByteSize(InputSignals, idx, inByteSize);
        if (ret) {
            ret = GetSignalNumberOfSamples(InputSignals, idx, inSamples);
        }
        if (ret) {
            inByteSize *= inSamples;
            totalSignalsByteSize += inByteSize;
        }
    }

    for (n = 0u; (n < GetNumberOfOutputSignals()) && (ret); n++) {
        uint32 outByteSize = 0u;
        uint32 outSamples = 1u;
        uint32 idx = n;
        ret = GetSignalByteSize(OutputSignals, idx, outByteSize);
        if (ret) {
            ret = GetSignalNumberOfSamples(OutputSignals, idx, outSamples);
        }

        if (ret) {
            outByteSize *= outSamples;
            totalSignalOutSize += outByteSize;
        }
    }

    if (ret) {
        ret = (totalSignalsByteSize == totalSignalOutSize);
    }
    return ret;
}

bool InterleavedInputBrokerTestGAM::Execute() {
    return MemoryOperationsHelper::Copy(GetOutputSignalsMemory(), GetInputSignalsMemory(), totalSignalsByteSize);
}

void *InterleavedInputBrokerTestGAM::GetInputSignalsMemory1() {
    return GAM::GetInputSignalsMemory();
}

void *InterleavedInputBrokerTestGAM::GetOutputSignalsMemory1() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(InterleavedInputBrokerTestGAM, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

static const char8 * const config = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = InterleavedInputBrokerTestGAM"
        "            InputSignals = {"
        "               Signal$InterleavedInputBroker = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 40"
        "                   Samples = 3"
        "                   Frequency = 1.0"
        "               }"
        "               Signal2$InterleavedInputBroker = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 16"
        "                   Samples = 2"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               PacketCounter = {"
        "                   DataSource = DDB1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               MHVPS_Voltage = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               GY1_MHVPS_Current = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               GY2_MHVPS_Current = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               GY1_BPS_Voltage = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               GY2_BPS_Voltage = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               GY1_BPS_Current = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               GY2_BPS_Current = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               GY1_RF_Detector = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               GY2_RF_Detector = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               AI = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               Alarm_Statuses = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               InternalErrors = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               D0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               State = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               Reserved = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               Signal2_1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "               Signal2_2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = InterleavedInputBrokerTestDS"
        "            NumberOfBuffers = 1"
        "            Signals = {"
        "               Signal$InterleavedInputBroker = {"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 40"
        "                   PacketChunkSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
        "               }"
        "               Signal2$InterleavedInputBroker = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 16"
        "                   Samples = 2"
        "                   PacketChunkSizes = {8, 8}"
        "               }"
        "            }"
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
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

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

InterleavedInputBrokerTest::InterleavedInputBrokerTest() {
    // Auto-generated constructor stub for InterleavedInputBrokerTest
    // TODO Verify if manual additions are needed
}

InterleavedInputBrokerTest::~InterleavedInputBrokerTest() {
    // Auto-generated destructor stub for InterleavedInputBrokerTest
    // TODO Verify if manual additions are needed
}

bool InterleavedInputBrokerTest::TestInit() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<InterleavedInputBrokerTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<InterleavedInputBrokerTestGAM> gam;
    ReferenceT<InterleavedInputBroker> broker;

    if (ret) {

        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    if (ret) {
        ReferenceContainer brokerContainer;
        ret = gam->GetInputBrokers(brokerContainer);
        if (ret) {
            broker = brokerContainer.Get(0);
            ret = broker.IsValid();
        }
        if (ret) {
            ret = broker->Execute();
            if (ret) {
                ret = gam->Execute();
            }
        }
        if (ret) {
            uint8* mem = (uint8*) gam->GetOutputSignalsMemory1();
            //the packet size
            ret = (*(uint64*) mem == 0);
            mem += 8;
            ret &= (*(uint64*) mem == 1);
            mem += 8;
            ret &= (*(uint64*) mem == 2);
            mem += 8;

            for (uint32 k = 0u; k < 10; k++) {
                ret = (*(uint16*) mem == k);
                mem += 2;
                ret &= (*(uint16*) mem == k + 15);
                mem += 2;
                ret &= (*(uint16*) mem == k + 30);
                mem += 2;
            }

            for (uint32 k = 10u; k < 12; k++) {
                ret = (*(uint32*) mem == k);
                mem += 4;
                ret &= (*(uint32*) mem == k + 15);
                mem += 4;
                ret &= (*(uint32*) mem == k + 30);
                mem += 4;
            }

            for (uint32 k = 12u; k < 14; k++) {
                ret = (*(uint8*) mem == k);
                mem += 1;
                ret &= (*(uint8*) mem == k + 15);
                mem += 1;
                ret &= (*(uint8*) mem == k + 30);
                mem += 1;
            }

            ret = (*(uint16*) mem == 14);
            mem += 2;
            ret &= (*(uint16*) mem == 29);
            mem += 2;
            ret &= (*(uint16*) mem == 44);
            mem += 2;

            uint64 *mem2=(uint64*) mem;
            ret&=(mem2[0]==100);
            ret&=(mem2[1]==102);
            ret&=(mem2[2]==101);
            ret&=(mem2[3]==103);
        }
    }
    return ret;
}

bool InterleavedInputBrokerTest::TestExecute() {
    return true;
}

