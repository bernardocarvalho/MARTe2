/**
 * @file CircularBufferThreadInputDataSourceTest.h
 * @brief Header file for class CircularBufferThreadInputDataSourceTest
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

 * @details This header file contains the declaration of the class CircularBufferThreadInputDataSourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CIRCULARBUFFERTHREADINPUTDATASOURCETEST_H_
#define CIRCULARBUFFERTHREADINPUTDATASOURCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CircularBufferThreadInputDataSource.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class CircularBufferThreadInputDataSourceTest {
public:

    /**
     * @brief Constructor
     */
    CircularBufferThreadInputDataSourceTest();

    /**
     * @brief Destructor
     */
    ~CircularBufferThreadInputDataSourceTest();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Initialise
     */
    bool TestInitialise();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::GetBrokerName
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::GetInputBrokers
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::GetOutputBrokers
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::SetConfiguredDatabase
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests that CircularBufferThreadInputDataSource::SetConfiguredDatabase returns
     * false if a signal is produced
     */
    bool TestSetConfiguredDatabase_False_WrittenSignal();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::PrepareNextState
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Synchronise
     */
    bool TestSynchronise();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Synchronise when the signal is not
     * read and returns false
     */
    bool TestSynchronise_FullRolling();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Synchronise when more than one chunks of
     * samples is arrived for the sync signal
     */
    bool TestSynchronise_GetLatest();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::GetInputOffset
     */
    bool TestGetInputOffset();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::GetOutputOffset
     */
    bool TestGetOutputOffset();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Execute
     */
    bool TestExecute();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Execute with different number of samples
     * for the same signal
     */
    bool TestExecute_SameSignalDifferentModes();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Execute with the ErrorCheck signal
     */
    bool TestExecute_ErrorCheck();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Execute with the ErrorCheck signal and
     * producing a write overlap error.
     */
    bool TestExecute_ErrorCheck_Overwrite();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Execute with the ErrorCheck signal and
     * producing a DriverRead error.
     */
    bool TestExecute_ErrorCheck_DriverRead();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Execute with the ErrorCheck signal
     * producing different error types.
     */
    bool TestExecute_ErrorCheck_Both();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::SetConfiguredDatabase that fails if
     * the ErrorCheck signal has a type different than uint32
     */
    bool TestSetConfiguredDatabase_ErrorCheck_False_BadType();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::SetConfiguredDatabase that fails if
     * the ErrorCheck signal has a bad number of elements.
     */
    bool TestSetConfiguredDatabase_ErrorCheck_False_BadSize();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Execute with the InternalTimeSTamp signal
     */
    bool TestExecute_TimeStamp();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Execute with the InternalTimeSTamp signal
     * when the signal has not be read.
     */
    bool TestExecute_TimeStamp_NoRead();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Execute with the InternalTimeSTamp signal
     * when the DriverRead fails.
     */
    bool TestExecute_TimeStamp_FalseDriverRead();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::SetConfiguredDatabase that fails if the
     * InternalTimeStamp signal has a type different than uint64
     */
    bool TestSetConfiguredDatabase_TimeStamp_False_BadType();


    /**
     * @brief Tests the CircularBufferThreadInputDataSource::SetConfiguredDatabase that fails if the
     * InternalTimeStamp signal has a bad number of elements
     */
    bool TestSetConfiguredDatabase_TimeStamp_False_BadSize();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::Purge
     */
    bool TestPurge();

    /**
     * @brief Tests the CircularBufferThreadInputDataSource::TerminateInputCopy
     */
    bool TestTerminateInputCopy();


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CIRCULARBUFFERTHREADINPUTDATASOURCETEST_H_ */

