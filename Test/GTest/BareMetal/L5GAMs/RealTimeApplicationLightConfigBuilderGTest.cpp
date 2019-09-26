 /**
 * @file RealTimeApplicationLightConfigBuilderGTest.cpp
 * @brief Source file for class RealTimeApplicationLightConfigBuilderGTest
 * @date 22 ago 2019
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
 * the class RealTimeApplicationLightConfigBuilderGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "RealTimeApplicationLightConfigBuilderTest.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabase1) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase1(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabase1_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase1(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabase2) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase2(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabase2_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase2(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabase_IgnoreDataSource) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase_IgnoreDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabaseFalse_GAMsNoIO) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_GAMsNoIO(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabaseFalse_GAMsNoIO_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_GAMsNoIO(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabaseFalse_InvalidApplication) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_InvalidApplication());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabaseFalse_NoTimeDataSource) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_NoTimeDataSource(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestInitialiseSignalsDatabaseFalse_NoTimeDataSource_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_NoTimeDataSource(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases1) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases2) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases3) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases3());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases4) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases4());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases5) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases5());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases6) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases6());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_LockedDataSource) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_LockedDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_CopyAllKnownSignalsProperties) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_CopyAllKnownSignalsProperties());
}


TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_MemberAliases) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_MemberAliases());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_Defaults) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_Defaults());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_Defaults2) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_Defaults2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_False) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_InvalidDimensions) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_InvalidNDimensionsInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_InvalidNElementsInStruct) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_InvalidNElementsInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_OnlyFrequencyInStruct) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_OnlyFrequencyInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_OnlySyncSignalInStruct) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_OnlySyncSignalInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_OnlyTriggerInStruct) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_OnlyTriggerInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_OnlyTriggerSignalInStruct) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_OnlyTriggerSignalInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_InvalidSyncSignalInStruct) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_InvalidSyncSignalInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_InvalidTriggerSignalInStruct) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_InvalidTriggerSignalInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestFlattenSignalsDatabases_UnregisteredType) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_UnregisteredType());
}


TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStates) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStates_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStates2) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates2(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStates2_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates2(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStates_FunctionGroups) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_FunctionGroups(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStates_FunctionGroups_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_FunctionGroups(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStates_MoreThreads) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_MoreThreads(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStates_MoreThreads_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_MoreThreads(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStatesFalse_SameGAMInMoreThreads) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_SameGAMInMoreThreads(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStatesFalse_SameGAMInMoreThreads_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_SameGAMInMoreThreads(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStatesFalse_MoreSyncs1) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs1(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStatesFalse_MoreSyncs1_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs1(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStatesFalse_MoreSyncs2) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs2(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStatesFalse_MoreSyncs2_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs2(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStatesFalse_MoreSyncs3) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs3(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStatesFalse_MoreSyncs3_FromConfig) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs3(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveStatesFalse_InvalidApplication) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_InvalidApplication());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestConfigureBeforeInitialisation_ResolveStates_False) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_ResolveStates_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSources1) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSources2) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSources3) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources3());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSources_StructMap) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructMap());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSources_StructMap2) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructMap2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSources_StructMap3) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructMap3());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSources_StructMapNamespaces) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructMapNamespaces());
}


TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSources_StructNamespaceMap) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructNamespaceMap());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSources_StructNamespaceMapInDs) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructNamespaceMapInDs());
}


TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSource_False) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSource_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourceFalse_TypeMismatch) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_TypeMismatch());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourceFalse_TypeMismatch1) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_TypeMismatch1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourceFalse_NElementsMismatch) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NElementsMismatch());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourceFalse_NElementsMismatch1) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NElementsMismatch1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourceFalse_NDimensionsMismatch) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NDimensionsMismatch());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourceFalse_NDimensionsMismatch1) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NDimensionsMismatch1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourceFalse_DefaultMismatch) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_DefaultMismatch());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourceFalse_DefaultMismatch1) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_DefaultMismatch1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourceFalse_DefaultMismatch3) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_DefaultMismatch3());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourcesSignalsFalse_IncompatibleMap) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_IncompatibleMap());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourcesSignalsFalse_IncompatibleMap1) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_IncompatibleMap1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourcesSignalsFalse_PartialDataSourceWithType) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_PartialDataSourceWithType());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourcesSignalsFalse_LeafMappedOnNode) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_LeafMappedOnNode());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourcesSignalsFalse_AddSignalToLockedDataSource) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_AddSignalToLockedDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestResolveDataSourcesSignalsFalse_CompleteSignalInLockedDataSource) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_CompleteSignalInLockedDataSource());
}
TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyDataSourcesSignals1) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyDataSourcesSignals_PartialDataSource) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_PartialDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyDataSourcesSignals_DefaultValuesMatch) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_DefaultValuesMatch());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyDataSourcesSignals_AssignNumberOfElements) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_AssignNumberOfElements());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyDataSourcesSignals_AssignNumberOfDimensions) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_AssignNumberOfDimensions());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyDataSourcesSignalsFalse_NoType) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_NoType());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyDataSourcesSignalsFalse_NoType2) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_NoType2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyDataSourcesSignalsFalse_InvalidNElements) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_InvalidNElements());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyDataSourcesSignalsFalse_IncompatibleDefaults) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_IncompatibleDefaults());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyDataSourcesSignalsFalse_IncompatibleDefaultsMultiVarAsArray) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_IncompatibleDefaultsMultiVarAsArray());
}


TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestConfigureBeforeInitialisation_VerifyConsumersProducers_False) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_VerifyConsumersProducers_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyConsumersAndProducers) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyConsumersAndProducers_Ranges) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers_Ranges());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyConsumersAndProducers_TimingSignals) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers_TimingSignals());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyConsumersAndProducersFalse_MoreProducers) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_MoreProducers());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyConsumersAndProducersFalse_MoreProducers2) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_MoreProducers2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyConsumersAndProducersFalse_RangeOverlap) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_RangeOverlap());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyConsumersAndProducersFalse_RangeOverlap2) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_RangeOverlap2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestVerifyConsumersAndProducersFalse_ProducerInTimeSignals) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_ProducerInTimeSignals());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestCopy) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestCopy());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestSet) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestSet());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestPostConfiguredDataSource) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestPostConfigureDataSourceFalse_InvalidDataSource) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureDataSourceFalse_InvalidDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestPostConfigureFunctions) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureFunctions());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestPostConfigureFunctionsFalse_InvalidDataSource) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureFunctionsFalse_InvalidDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestConfigureAfterInitialisation) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestConfigureAfterInitialisation());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestConfigureBeforeInitialisation) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestConfigureBeforeInitialisation_VerifyDataSourcesSignals_False) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_VerifyDataSourcesSignals_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestConfigureThreads) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestConfigureThreads());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestConfigureBeforeInitialisation_False_NoFunctions) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_False_NoFunctions());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestConfigureBeforeInitialisation_False_NoData) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_False_NoData());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestArraysOfStructures1) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestArraysOfStructures1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationLightConfigBuilderGTest,TestArraysOfStructures2) {
    RealTimeApplicationLightConfigBuilderTest test;
    ASSERT_TRUE(test.TestArraysOfStructures2());
}
