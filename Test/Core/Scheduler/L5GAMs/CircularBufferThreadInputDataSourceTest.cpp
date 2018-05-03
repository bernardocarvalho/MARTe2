/**
 * @file CircularBufferThreadInputDataSourceTest.cpp
 * @brief Source file for class CircularBufferThreadInputDataSourceTest
 * @date 26 apr 2018
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
 * the class CircularBufferThreadInputDataSourceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CircularBufferThreadInputDataSourceTest.h"
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

class CircularBufferThreadInputDataSourceTestDS: public CircularBufferThreadInputDataSource {
public:
    CLASS_REGISTER_DECLARATION()

CircularBufferThreadInputDataSourceTestDS    ();
    virtual ~CircularBufferThreadInputDataSourceTestDS();

    virtual bool DriverRead(char8 * const bufferToFill,
            uint32 &sizeToRead,
            const uint32 signalIdx);

    virtual uint32 *GetCurrentInternalBuffer();

    virtual uint8 *GetIsRefreshed();

    virtual uint32 *GetLastReadBuffer();

    virtual uint32 *GetLastReadBuffer_1();

    virtual uint32 *GetTriggerAfterNPackets();

    virtual uint32 *NBrokerOpPerSignal();

    virtual uint32 GetNumberOfChannels();

    virtual void TerminateRead(const uint32 signalIdx,
            const uint32 offset,
            const uint32 samples,
            const uint32 flag);

    virtual FastPollingMutexSem *GetMutex();

    void ContinueRead();

private:
    uint32 counter;
    volatile int32 continueRead;

};

CircularBufferThreadInputDataSourceTestDS::CircularBufferThreadInputDataSourceTestDS() {
    counter = 0u;
    continueRead = 0;
}

CircularBufferThreadInputDataSourceTestDS::~CircularBufferThreadInputDataSourceTestDS() {

}

uint32 *CircularBufferThreadInputDataSourceTestDS::GetCurrentInternalBuffer() {
    return currentBuffer;
}

uint8 *CircularBufferThreadInputDataSourceTestDS::GetIsRefreshed() {
    return isRefreshed;
}

uint32 *CircularBufferThreadInputDataSourceTestDS::GetLastReadBuffer() {
    return lastReadBuffer;
}

uint32 *CircularBufferThreadInputDataSourceTestDS::GetLastReadBuffer_1() {
    return lastReadBuffer_1;
}

uint32 *CircularBufferThreadInputDataSourceTestDS::GetTriggerAfterNPackets() {
    return triggerAfterNPackets;
}

uint32 *CircularBufferThreadInputDataSourceTestDS::NBrokerOpPerSignal() {
    return nBrokerOpPerSignal;
}

uint32 CircularBufferThreadInputDataSourceTestDS::GetNumberOfChannels() {
    return numberOfChannels;
}

bool CircularBufferThreadInputDataSourceTestDS::DriverRead(char8 * const bufferToFill,
                                                           uint32 &sizeToRead,
                                                           const uint32 signalIdx) {
    //give time to sync
    if (signalIdx == 0u) {
        while (continueRead == 0) {
            Sleep::MSec(5);
        }
        Sleep::MSec(500);
    }

    printf("WRITE %d %d\n", signalIdx, continueRead);
    uint32 *bufferPtr = (uint32 *) bufferToFill;
    for (uint32 i = 0u; i < sizeToRead / 4; i++) {
        bufferPtr[i] = counter;
        counter++;
    }

    return true;
}

void CircularBufferThreadInputDataSourceTestDS::ContinueRead() {

    Atomic::TestAndSet(&continueRead);
}

void CircularBufferThreadInputDataSourceTestDS::TerminateRead(const uint32 signalIdx,
                                                              const uint32 offset,
                                                              const uint32 samples,
                                                              const uint32 flag) {
    CircularBufferThreadInputDataSource::TerminateRead(signalIdx, offset, samples, flag);

    if (signalIdx == 0) {
        Atomic::Decrement(&continueRead);

        printf("HERE %d %d\n", signalIdx, continueRead);
    }
}

FastPollingMutexSem *CircularBufferThreadInputDataSourceTestDS::GetMutex() {
    return &mutex;
}

CLASS_REGISTER(CircularBufferThreadInputDataSourceTestDS, "1.0")

class CircularBufferThreadInputDataSourceTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

CircularBufferThreadInputDataSourceTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();
    void *GetInputMemoryBuffer();

    void *GetOutputMemoryBuffer();
private:
    uint32 totalSize;
};

CircularBufferThreadInputDataSourceTestGAM1::CircularBufferThreadInputDataSourceTestGAM1() :
        GAM() {
    totalSize = 0u;
}

bool CircularBufferThreadInputDataSourceTestGAM1::Setup() {

    return true;
}

bool CircularBufferThreadInputDataSourceTestGAM1::Execute() {
    return true;
}

void *CircularBufferThreadInputDataSourceTestGAM1::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

void *CircularBufferThreadInputDataSourceTestGAM1::GetOutputMemoryBuffer() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(CircularBufferThreadInputDataSourceTestGAM1, "1.0")

static const char8 * const config = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = CircularBufferThreadInputDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Trigger = 2"
        "                   Samples = 3"
        "                   Frequency = 0"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0,0}, {2,2}}"
        "                   Trigger = 1"
        "                   Samples = 2"
        "               }"
        "               Signal3 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Trigger = 1"
        "                   Samples = 4"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = CircularBufferThreadInputDataSourceTestDS"
        "            NumberOfBuffers = 100"
        "            CpuMask = 1"
        "            ReceiverThreadPriority = 31"
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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

CircularBufferThreadInputDataSourceTest::CircularBufferThreadInputDataSourceTest() {
    // Auto-generated constructor stub for CircularBufferThreadInputDataSourceTest
    // TODO Verify if manual additions are needed
}

CircularBufferThreadInputDataSourceTest::~CircularBufferThreadInputDataSourceTest() {
    // Auto-generated destructor stub for CircularBufferThreadInputDataSourceTest
    // TODO Verify if manual additions are needed
}

bool CircularBufferThreadInputDataSourceTest::TestConstructor() {
    CircularBufferThreadInputDataSourceTestDS dataSource;

    bool ret = (dataSource.GetCurrentInternalBuffer() == NULL);

    if (ret) {
        ret = (dataSource.GetIsRefreshed() == NULL);
    }
    if (ret) {
        ret = (dataSource.GetLastReadBuffer() == NULL);
    }
    if (ret) {
        ret = (dataSource.GetLastReadBuffer_1() == NULL);
    }
    if (ret) {
        ret = (dataSource.GetTriggerAfterNPackets() == NULL);
    }
    if (ret) {
        ret = (dataSource.NBrokerOpPerSignal() == NULL);
    }
    if (ret) {
        ret = (dataSource.GetNumberOfChannels() == 0);
    }
    return true;
}

bool CircularBufferThreadInputDataSourceTest::TestInitialise() {
    CircularBufferThreadInputDataSourceTestDS dataSource;

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
    }
    if (ret) {
        ret = dataSource.Initialise(cdb);
    }

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSynchronise() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestGetBrokerName() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ConfigurationDatabase cdb;
        float32 freq = 0.;
        cdb.Write("Frequency", freq);
        const char8* brokerName = dataSource->GetBrokerName(cdb, InputSignals);
        ret = StringHelper::Compare(brokerName, "MemoryMapSyncUnrelatedInputBroker") == 0;
        if (ret) {
            freq = -1.;
            cdb.Delete("Frequency");
            cdb.Write("Frequency", freq);
            const char8* brokerName = dataSource->GetBrokerName(cdb, InputSignals);
            ret = StringHelper::Compare(brokerName, "MemoryMapUnrelatedInputBroker") == 0;
            if (ret) {
                brokerName = dataSource->GetBrokerName(cdb, OutputSignals);
                ret = brokerName == NULL;
            }
        }
    }

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestGetInputBrokers() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        uint32 *nbrokerOpPerSignal = dataSource->NBrokerOpPerSignal();
        ret = nbrokerOpPerSignal[0] == 1;
        ret &= nbrokerOpPerSignal[1] == 4;
        ret &= nbrokerOpPerSignal[2] == 1;

    }

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestGetOutputBrokers() {

    CircularBufferThreadInputDataSourceTestDS dataSource;

    ReferenceContainer inputBrokers;
    uint32 x;
    bool ret = !dataSource.GetOutputBrokers(inputBrokers, "GAMA", (void*) &x);

    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestSetConfiguredDatabase() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 2"
            "                   Samples = 10"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Trigger = 1"
            "                   Samples = 5"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 1"
            "                   Samples = 12"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   Trigger = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   Trigger = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 100"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
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

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = (dataSource->GetIsRefreshed() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetLastReadBuffer() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetLastReadBuffer_1() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetTriggerAfterNPackets() != NULL);
    }
    if (ret) {
        ret = (dataSource->NBrokerOpPerSignal() != NULL);
    }
    if (ret) {
        ret = (dataSource->GetNumberOfChannels() == 3);
    }
    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestPrepareNextState() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }

    if (ret) {
        uint32 *currentBuffer = dataSource->GetCurrentInternalBuffer();
        FastPollingMutexSem *mutex = dataSource->GetMutex();
        while (1) {
            dataSource->ContinueRead();
            mutex->FastLock();
            if (currentBuffer[0] >= 2) {
                mutex->FastUnLock();
                break;
            }
            mutex->FastUnLock();
            Sleep::MSec(5);
            printf("I am %d\n", currentBuffer[0]);
        }
    }

    return true;
}

bool CircularBufferThreadInputDataSourceTest::TestGetOffset() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }

    if (ret) {
        dataSource->ContinueRead();
        dataSource->Synchronise();
        int32 offset = dataSource->GetOffset(0, 10, 0);

        ret = (offset == 0);
        printf("offset = %d\n", offset);
    }
    if (ret) {
        dataSource->ContinueRead();
        dataSource->Synchronise();
        int32 offset = dataSource->GetOffset(0, 10, 0);

        ret = (offset == 12);
        printf("offset = %d\n", offset);
    }
    return ret;

}

bool CircularBufferThreadInputDataSourceTest::TestExecute() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    if (ret) {
        ReferenceT<MemoryMapUnrelatedInputBroker> broker, broker1;
        ReferenceContainer brokers;
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
        }

        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }
        if (ret) {
            dataSource->ContinueRead();
            ret = broker->Execute();
            ret = broker1->Execute();
        }

    }

    if (ret) {
        uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();
        uint32 numberOfIntegers = 11;
        for (uint32 i = 0u; i < numberOfIntegers; i++) {
            printf("mem[%d]=%d\n", i, mem[i]);
        }
        ret &= mem[0] == 0;
        ret &= mem[1] == 12;
        ret &= mem[2] == 24;
        ret &= mem[3] == 1;
        ret &= mem[4] == 13;
        ret &= mem[5] == 3;
        ret &= mem[6] == 15;
        ret &= mem[7] == 0;
        ret &= mem[8] == 0;
        ret &= mem[9] == 0;
        ret &= mem[10] == 0;

    }
    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestExecute_SameSignalDifferentModes() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Trigger = 2"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,9}}"
            "                   Trigger = 1"
            "                   Samples = 3"
            "                   Alias = Signal1"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 0"
            "                   Samples = 4"
            "                   Alias = Signal1"
            "               }"
            "               Signal4 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 1"
            "                   Samples = 6"
            "                   Alias = Signal1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 20"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapUnrelatedInputBroker> broker, broker1;
    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
        }

        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }

    }
    uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();

    uint32 numberOfReads = 4;
    uint32 offsetRead = 0u;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        if (ret) {
            dataSource->ContinueRead();
            ret = broker->Execute();
            ret = broker1->Execute();
        }

        if (ret) {
            printf("read %d\n", n);
            uint32 numberOfIntegers = 140;
            for (uint32 i = 0u; i < numberOfIntegers; i++) {
                printf("mem[%d]=%d\n", i, mem[i]);
            }
            //the first are separated by 10
            uint32 nSamples = 5;
            uint32 j = 0u;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == i * 10 + offsetRead);
                j++;
            }
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == (i * 10 + 2 + offsetRead));
                j++;
            }
            nSamples = 30;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == (i + 30 + offsetRead));
                j++;
            }
            nSamples = 40;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == i + offsetRead);
                j++;
            }
            nSamples = 60;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == (i + offsetRead));
                j++;
            }
        }
        offsetRead += 60;
    }

    return ret;

}

bool CircularBufferThreadInputDataSourceTest::TestExecute_SameSignalDifferentModes1() {

    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Trigger = 3"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,9}}"
            "                   Trigger = 1"
            "                   Samples = 3"
            "                   Alias = Signal1"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 0"
            "                   Samples = 4"
            "                   Alias = Signal1"
            "               }"
            "               Signal4 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Trigger = 1"
            "                   Samples = 6"
            "                   Alias = Signal1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 20"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapUnrelatedInputBroker> broker, broker1;
    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
        }

        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }

    }
    uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();

    uint32 numberOfReads = 4;
    uint32 offsetRead = 0u;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        if (ret) {
            dataSource->ContinueRead();
            ret = broker->Execute();
            ret = broker1->Execute();
        }

        if (ret) {
            printf("read %d\n", n);
            uint32 numberOfIntegers = 140;
            for (uint32 i = 0u; i < numberOfIntegers; i++) {
                printf("mem[%d]=%d\n", i, mem[i]);
            }
            //the first are separated by 10
            uint32 nSamples = 5;
            uint32 j = 0u;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == i * 10 + 10 + offsetRead);
                j++;
            }
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == (i * 10 + 12 + offsetRead));
                j++;
            }
            nSamples = 30;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == (i + 30 + offsetRead));
                j++;
            }
            nSamples = 40;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == i + offsetRead);
                j++;
            }
            nSamples = 60;
            for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
                ret &= (mem[j] == (i + offsetRead));
                j++;
            }
        }
        offsetRead += 60;
    }
    return ret;

}

bool CircularBufferThreadInputDataSourceTest::TestExecute_ErrorCheck() {
    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = CircularBufferThreadInputDataSourceTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "                   Trigger = 3"
            "                   Samples = 5"
            "                   Frequency = 0"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 1"
            "                   Trigger = 1"
            "                   Samples = 5"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = CircularBufferThreadInputDataSourceTestDS"
            "            NumberOfBuffers = 20"
            "            CpuMask = 1"
            "            ReceiverThreadPriority = 31"
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<CircularBufferThreadInputDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<CircularBufferThreadInputDataSourceTestGAM1> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    ReferenceT<MemoryMapUnrelatedInputBroker> broker, broker1;
    ReferenceContainer brokers;
    if (ret) {
        ret = gam->GetInputBrokers(brokers);
        if (ret) {
            broker = brokers.Get(0);
            broker1 = brokers.Get(1);
            ret = broker.IsValid();
        }

        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }

    }
    uint32 *mem = (uint32*) gam->GetInputMemoryBuffer();

    uint32 numberOfReads = 4;
    for (uint32 n = 0u; (n < numberOfReads) && (ret); n++) {
        if (ret) {
            dataSource->ContinueRead();
            ret = broker->Execute();
            ret = broker1->Execute();
        }

        if (ret) {
            uint32 nSamples = 5;
            for (uint32 i = 0u; i < nSamples; i++) {
                printf("error[%d]=%d\n", i, mem[10 + i]);
            }
        }
    }
    return ret;
}

bool CircularBufferThreadInputDataSourceTest::TestPurge() {
    CircularBufferThreadInputDataSourceTestDS ds;
    ReferenceContainer purgeList;
    ds.Purge(purgeList);
    return true;
}

bool CircularBufferThreadInputDataSourceTest::TestTerminateRead() {
    return true;
}

