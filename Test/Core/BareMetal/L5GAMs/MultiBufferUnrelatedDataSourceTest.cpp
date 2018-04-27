/**
 * @file MultiBufferUnrelatedDataSourceTest.cpp
 * @brief Source file for class MultiBufferUnrelatedDataSourceTest
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
 * the class MultiBufferUnrelatedDataSourceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MultiBufferUnrelatedDataSourceTest.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "MemoryMapUnrelatedInputBroker.h"
#include "MemoryMapUnrelatedOutputBroker.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//need to create a dummy for tests
class MultiBufferUnrelatedDataSourceInstanceTest: public MultiBufferUnrelatedDataSource {

public:
    CLASS_REGISTER_DECLARATION()

MultiBufferUnrelatedDataSourceInstanceTest    ();

    ~MultiBufferUnrelatedDataSourceInstanceTest();

    virtual uint32 GetPacketSizeSum();

    virtual uint32 GetNumberOfInternalBuffers();

    virtual HeapI *GetMemoryHeap();

    virtual int32 GetOffset(const uint32 signalIdx,const uint32 samples,
            const uint32 flag);

    virtual bool Synchronise();

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    StructuredDataI *GetConfiguredDatabase();
};

MultiBufferUnrelatedDataSourceInstanceTest::MultiBufferUnrelatedDataSourceInstanceTest() {
}

MultiBufferUnrelatedDataSourceInstanceTest::~MultiBufferUnrelatedDataSourceInstanceTest() {

}

int32 MultiBufferUnrelatedDataSourceInstanceTest::GetOffset(const uint32 signalIdx,
                                                            const uint32 samples,
                                                            const uint32 flag) {

    return 0u;
}

HeapI *MultiBufferUnrelatedDataSourceInstanceTest::GetMemoryHeap() {

    return memoryHeap;
}

uint32 MultiBufferUnrelatedDataSourceInstanceTest::GetNumberOfInternalBuffers() {
    return numberOfInternalBuffers;
}

uint32 MultiBufferUnrelatedDataSourceInstanceTest::GetPacketSizeSum() {
    uint32 nOfSignals = GetNumberOfSignals();
    uint32 bufferSize = 0u;
    for (uint32 i = 0u; i < nOfSignals; i++) {
        bufferSize += packetSize[i];
    }

    return bufferSize;
}

bool MultiBufferUnrelatedDataSourceInstanceTest::Synchronise() {
    return true;
}

bool MultiBufferUnrelatedDataSourceInstanceTest::PrepareNextState(const char8 * const currentStateName,
                                                                  const char8 * const nextStateName) {
    return true;
}

StructuredDataI *MultiBufferUnrelatedDataSourceInstanceTest::GetConfiguredDatabase() {
    return &configuredDatabase;
}

CLASS_REGISTER(MultiBufferUnrelatedDataSourceInstanceTest, "1.0")

/**
 * @brief GAM empty implementation to support the MemoryMapInputBroker tests
 */
class MultiBufferUnrelatedDataSourceTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MultiBufferUnrelatedDataSourceTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();

    void *GetInputSignalsMemory1();

    void *GetOutputSignalsMemory1();
};

MultiBufferUnrelatedDataSourceTestGAM1::MultiBufferUnrelatedDataSourceTestGAM1() :
        GAM() {
}

bool MultiBufferUnrelatedDataSourceTestGAM1::Setup() {
    return true;
}

bool MultiBufferUnrelatedDataSourceTestGAM1::Execute() {
    return true;
}

void *MultiBufferUnrelatedDataSourceTestGAM1::GetInputSignalsMemory1() {
    return GAM::GetInputSignalsMemory();
}

void *MultiBufferUnrelatedDataSourceTestGAM1::GetOutputSignalsMemory1() {
    return GAM::GetOutputSignalsMemory();
}
CLASS_REGISTER(MultiBufferUnrelatedDataSourceTestGAM1, "1.0")

/**
 * MARTe configuration structure to test the MemoryMapInputBroker
 */
static const char8 * const config = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = MultiBufferUnrelatedDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Trigger = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 10"
        "                   Type = uint32"
        "                   Frequency = 1"
        "               }"
        "               ErrorCheck = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = MultiBufferUnrelatedDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0, 0}, {9, 9}}"
        "                   Type = uint32"
        "               }"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   Samples = 2"
        "               }"
        "               InternalTimeStamp = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal5 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "               Signal6 = {"
        "                   DataSource = Drv1"
        "                   Type = float64"
        "                   Samples = 5"
        "                   Trigger = 1"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = MultiBufferUnrelatedDataSourceInstanceTest"
        "            NumberOfBuffers = 3"
        "            HeapName = StandardHeap"
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

MultiBufferUnrelatedDataSourceTest::MultiBufferUnrelatedDataSourceTest() {
    // Auto-generated constructor stub for MultiBufferUnrelatedDataSourceTest
    // TODO Verify if manual additions are needed
}

MultiBufferUnrelatedDataSourceTest::~MultiBufferUnrelatedDataSourceTest() {
    // Auto-generated destructor stub for MultiBufferUnrelatedDataSourceTest
    // TODO Verify if manual additions are needed
}

bool MultiBufferUnrelatedDataSourceTest::TestInitialise() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = (dataSource->GetNumberOfInternalBuffers() == 3u);
    }

    if (ret) {
        const char8 *heapName = "StandardHeap";

        HeapI* memoryHeap = HeapManager::FindHeap(heapName);
        ret = (memoryHeap != NULL_PTR(HeapI *));
        if (ret) {
            ret = (dataSource->GetMemoryHeap() == memoryHeap);
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MultiBufferUnrelatedDataSourceTest::TestInitialise_StandardHeapDefault() {

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        if (ret) {
            ret = cdb.Delete("HeapName");
        }
        if (ret) {
            MultiBufferUnrelatedDataSourceInstanceTest dataSource;
            ret = dataSource.Initialise(cdb);
            if (ret) {
                ret = (dataSource.GetMemoryHeap() == GlobalObjectsDatabase::Instance()->GetStandardHeap());
            }
        }

    }
    return ret;
}

bool MultiBufferUnrelatedDataSourceTest::TestInitialise_False_NoNumberOfBuffers() {
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        if (ret) {
            ret = cdb.Delete("NumberOfBuffers");
        }
        if (ret) {
            MultiBufferUnrelatedDataSourceInstanceTest dataSource;
            ret = !dataSource.Initialise(cdb);
        }
    }
    return ret;

}

bool MultiBufferUnrelatedDataSourceTest::TestInitialise_False_InvalidHeap() {
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        if (ret) {
            ret = cdb.Delete("HeapName");
        }
        if (ret) {
            StreamString unknownHeap = "UnknownHeap";
            ret = cdb.Write("HeapName", unknownHeap);
        }

        MultiBufferUnrelatedDataSourceInstanceTest dataSource;
        ret = !dataSource.Initialise(cdb);
    }
    return ret;

}

bool MultiBufferUnrelatedDataSourceTest::TestAllocateMemory() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = (dataSource->GetPacketSizeSum() == 69);
    }
    return ret;

}

bool MultiBufferUnrelatedDataSourceTest::TestGetNumberOfMemoryBuffers() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = (dataSource->GetNumberOfMemoryBuffers() == 1);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool MultiBufferUnrelatedDataSourceTest::TestGetSignalMemoryBuffer() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        uint32 numberOfSignals = dataSource->GetNumberOfSignals();
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            void *signalAddress = NULL;
            ret=dataSource->GetSignalMemoryBuffer(i,0u,signalAddress);
            if(ret) {
                ret = (signalAddress != NULL);
            }
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool MultiBufferUnrelatedDataSourceTest::TestGetBrokerName() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        StructuredDataI *data = dataSource->GetConfiguredDatabase();
        uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            ret = data->MoveToRoot();
            if (ret) {
                ret = data->MoveRelative("Functions");
            }
            if (ret) {
                ret = data->MoveToChild(i);
            }
            if (ret) {
                uint32 numberOfSignalsPerFunction = 0u;
                if (data->MoveRelative("InputSignals")) {
                    dataSource->GetFunctionNumberOfSignals(InputSignals, i, numberOfSignalsPerFunction);
                    for (uint32 j = 0u; (j < numberOfSignalsPerFunction) && (ret); j++) {
                        data->MoveToChild(j);
                        StreamString brokerName = dataSource->GetBrokerName(*data, InputSignals);
                        bool sync = false;
                        float32 frequency = 0;
                        if (data->Read("Frequency", frequency)) {
                            sync = (frequency > 0.);
                        }

                        uint32 trigger = 0u;
                        if (data->Read("Trigger", trigger)) {
                            sync |= (trigger > 0);
                        }

                        if (sync) {
                            ret = (brokerName == "MemoryMapSyncUnrelatedInputBroker");
                        }
                        else {
                            ret = (brokerName == "MemoryMapUnrelatedInputBroker");
                        }
                        data->MoveToAncestor(1u);
                    }
                    if (ret) {
                        ret = data->MoveToAncestor(1u);
                    }
                }
                if (data->MoveRelative("OutputSignals")) {

                    dataSource->GetFunctionNumberOfSignals(OutputSignals, i, numberOfSignalsPerFunction);
                    for (uint32 j = 0u; (j < numberOfSignalsPerFunction) && (ret); j++) {
                        data->MoveToChild(j);
                        StreamString brokerName = dataSource->GetBrokerName(*data, OutputSignals);
                        bool sync = false;
                        float32 frequency = 0;
                        if (data->Read("Frequency", frequency)) {
                            sync = (frequency > 0.);
                        }
                        uint32 trigger = 0u;
                        if (data->Read("Trigger", trigger)) {
                            sync = (trigger > 0);
                        }

                        if (sync) {
                            ret |= (brokerName == "MemoryMapSyncUnrelatedOutputBroker");
                        }
                        else {
                            ret = (brokerName == "MemoryMapUnrelatedOutputBroker");
                        }
                        data->MoveToAncestor(1u);
                    }
                }
            }

        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool MultiBufferUnrelatedDataSourceTest::TestGetInputBrokers() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            if (ret) {
                uint32 numberOfSignalsPerFunction = 0u;
                StreamString functionName;
                dataSource->GetFunctionName(i, functionName);
                StreamString gamPath = "Application1.Functions.";
                gamPath += functionName.Buffer();
                ReferenceT<MultiBufferUnrelatedDataSourceTestGAM1> gam = ObjectRegistryDatabase::Instance()->Find(gamPath.Buffer());
                ret = gam.IsValid();
                ReferenceContainer inputBrokers;

                if (ret) {
                    ret = dataSource->GetInputBrokers(inputBrokers, functionName.Buffer(), gam->GetInputSignalsMemory1());
                    dataSource->GetFunctionNumberOfSignals(InputSignals, i, numberOfSignalsPerFunction);
                }
                uint32 flag = 0;

                for (uint32 j = 0u; (j < numberOfSignalsPerFunction) && (ret); j++) {

                    float32 frequency = 0;

                    dataSource->GetFunctionSignalReadFrequency(InputSignals, i, j, frequency);
                    if (frequency > 0.) {
                        flag |= 2;
                    }
                    else {
                        flag |= 1;
                    }
                    uint32 trigger = 0u;
                    dataSource->GetFunctionSignalTrigger(InputSignals, i, j, trigger);

                    if (trigger > 0) {
                        flag |= 2;
                    }
                    else {
                        flag |= 1;
                    }
                }
                if (flag == 1) {
                    ret = (inputBrokers.Size() == 1);
                }
                else if (flag == 3) {
                    ret = (inputBrokers.Size() == 2);
                }
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MultiBufferUnrelatedDataSourceTest::TestGetOutputBrokers() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();
        for (uint32 i = 0u; (i < numberOfFunctions) && (ret); i++) {
            if (ret) {
                uint32 numberOfSignalsPerFunction = 0u;
                StreamString functionName;
                dataSource->GetFunctionName(i, functionName);
                StreamString gamPath = "Application1.Functions.";
                gamPath += functionName.Buffer();
                ReferenceT<MultiBufferUnrelatedDataSourceTestGAM1> gam = ObjectRegistryDatabase::Instance()->Find(gamPath.Buffer());
                ret = gam.IsValid();
                ReferenceContainer outputBrokers;

                if (ret) {
                    ret = dataSource->GetOutputBrokers(outputBrokers, functionName.Buffer(), gam->GetInputSignalsMemory1());
                    dataSource->GetFunctionNumberOfSignals(OutputSignals, i, numberOfSignalsPerFunction);
                }
                uint32 flag = 0;

                for (uint32 j = 0u; (j < numberOfSignalsPerFunction) && (ret); j++) {

                    float32 frequency = 0;

                    dataSource->GetFunctionSignalReadFrequency(OutputSignals, i, j, frequency);
                    if (frequency > 0.) {
                        flag |= 2;
                    }
                    else {
                        flag |= 1;
                    }
                    uint32 trigger = 0u;
                    dataSource->GetFunctionSignalTrigger(OutputSignals, i, j, trigger);

                    if (trigger > 0) {
                        flag |= 2;
                    }
                    else {
                        flag |= 1;
                    }
                }
                if (flag == 1) {
                    ret = (outputBrokers.Size() == 1);
                }
                else if (flag == 3) {
                    ret = (outputBrokers.Size() == 2);
                }
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MultiBufferUnrelatedDataSourceTest::TestSetConfiguredDatabase() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        StructuredDataI* data = dataSource->GetConfiguredDatabase();
        data->MoveToRoot();
        ret = dataSource->SetConfiguredDatabase(*data);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MultiBufferUnrelatedDataSourceTest::TestSetConfiguredDatabase_False_InternalTimeStamp_BadType() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        StructuredDataI* data = dataSource->GetConfiguredDatabase();
        data->MoveToRoot();
        ret = data->MoveRelative("Signals.4");
        if (ret) {
            ret = data->Delete("Type");
        }
        if (ret) {
            StreamString type = "uint32";
            ret = data->Write("Type", type);
        }
        if (ret) {
            data->MoveToRoot();
            ret = !dataSource->SetConfiguredDatabase(*data);
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MultiBufferUnrelatedDataSourceTest::TestSetConfiguredDatabase_False_InternalTimeStamp_Produced() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        StructuredDataI* data = dataSource->GetConfiguredDatabase();

        data->MoveToRoot();
        ret = data->MoveRelative("Signals.4.States.State1");
        if (ret) {

            Vector<StreamString> producers(1);
            producers[0] = "1";
            ret = data->Write("GAMProducers", producers);
        }
        if (ret) {
            data->MoveToRoot();
            ret = !dataSource->SetConfiguredDatabase(*data);
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MultiBufferUnrelatedDataSourceTest::TestSetConfiguredDatabase_False_ErrorCheck_BadType() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        StructuredDataI* data = dataSource->GetConfiguredDatabase();
        data->MoveToRoot();
        ret = data->MoveRelative("Signals.2");
        if (ret) {
            ret = data->Delete("Type");
        }
        if (ret) {
            StreamString type = "uint8";
            ret = data->Write("Type", type);
        }
        if (ret) {
            data->MoveToRoot();
            ret = !dataSource->SetConfiguredDatabase(*data);
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MultiBufferUnrelatedDataSourceTest::TestSetConfiguredDatabase_False_ErrorCheck_Produced() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<MultiBufferUnrelatedDataSourceInstanceTest> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        StructuredDataI* data = dataSource->GetConfiguredDatabase();

        data->MoveToRoot();
        ret = data->MoveRelative("Signals.2.States.State1");
        if (ret) {

            Vector<StreamString> producers(1);
            producers[0] = "1";
            ret = data->Write("GAMProducers", producers);
        }
        if (ret) {
            data->MoveToRoot();
            ret = !dataSource->SetConfiguredDatabase(*data);
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MultiBufferUnrelatedDataSourceTest::TestTerminateRead() {
    MultiBufferUnrelatedDataSourceInstanceTest dataSource;
    dataSource.TerminateRead(0, 0, 0, 0);
    return true;
}

bool MultiBufferUnrelatedDataSourceTest::TestTerminateWrite() {
    MultiBufferUnrelatedDataSourceInstanceTest dataSource;
    dataSource.TerminateWrite(0, 0, 0, 0);
    return true;
}

