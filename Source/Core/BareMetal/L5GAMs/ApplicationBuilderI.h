/**
 * @file ApplicationBuilderI.h
 * @brief Header file for class ApplicationBuilderI
 * @date 21 ago 2019
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

 * @details This header file contains the declaration of the class ApplicationBuilderI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_BAREMETAL_L5GAMS_APPLICATIONBUILDERI_H_
#define SOURCE_CORE_BAREMETAL_L5GAMS_APPLICATIONBUILDERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class RealTimeApplication;

/**
 * @brief The interface of every real time application builder.
 */
class ApplicationBuilderI: public Object {
public:
    /**
     * @brief Default Constructor
     */
    ApplicationBuilderI();

    /**
     * @brief Destructor
     */
    virtual ~ApplicationBuilderI();

    /**
     * @brief Sets the builder parameters
     * @param[in] realTimeApplicationIn the RealTimeApplication to be built
     * @param{in] defaultDataSourceNameIn the default DataSource name
     */
    virtual void SetParameters(RealTimeApplication &realTimeApplicationIn,
                               const char8 * const defaultDataSourceNameIn)=0;

    /**
     * @brief Assigns the brokers to the RealTimeApplication functions
     * @return true if the operation succeeds, false otherwise
     */
    virtual bool AssignBrokersToFunctions()=0;

    /**
     * @brief For each DataSource calls DataSourceI::SetConfiguredDatabase followed by DataSourceI::AllocateMemory.
     * @details Calls DataSourceI::SetConfiguredDatabase on each DataSource under Data, passing the Signals{} and Functions{} branches.
     * @return true if DataSourceI::SetConfiguredDatabase returns true for all DataSources.
     */
    virtual bool PostConfigureDataSources()=0;

    /**
     * @brief For each GAM calls GAM::SetConfiguredDatabase
     * @details Calls calls::SetConfiguredDatabase on each GAM under Functions, passing the Functions{} branches.
     * @return true if calls::SetConfiguredDatabase returns true for all GAMs.
     */
    virtual bool PostConfigureFunctions()=0;

    /**
     * @brief Initialises the SignalDatabae (see InitialiseSignalsDatabase) using directly a
     *  ConfigurationDatabase as the data source.
     * @return (see InitialiseSignalsDatabase)
     */
    virtual bool InitialiseSignalsDatabaseFromConfiguration()=0;

    /**
     * @brief Calls RealTimeThread::ConfigureArchitecture on all the threads.
     * @return true if RealTimeThread::ConfigureArchitecture returns true on all RealTimeThread elements.
     */
    virtual bool ConfigureThreads() const=0;

    /**
     * @brief Compiles all the information required to build a RealTimeApplication after the
     * ObjectRegistryDatabase::Initialise has been successfully called (i.e. it requires access to the live objects).
     * @return true if the following functions, called in order, return true:
     *   InitialiseSignalsDatabase(), FlattenSignalsDatabases(), ResolveDataSources(),
     * VerifyDataSourcesSignals(), ResolveFunctionSignals(), VerifyFunctionSignals(), ResolveStates(),
     * ResolveConsumersAndProducers(), VerifyConsumersAndProducers(), ResolveFunctionSignalsMemorySize(), ResolveFunctionsMemory(),
     * AssignFunctionsMemoryToDataSource)() and AssignBrokersToFunctions()
     * @post
     *   The functions PostConfigureDataSources() and PostConfigureFunctions() can now be called.
     */
    virtual bool ConfigureAfterInitialisation()=0;

    /**
     * @brief Compiles all the information required to build a RealTimeApplication without
     * requiring the ObjectRegistryDatabase::Initialise to be called to load all the Objects (i.e. it works only
     *  with configuration data).
     * @details This allows to generate compiled databases with Function and DataSource databases
     * that can be then exported to be later executed in e.g. another system (i.e. these can be directly fed to a
     * ObjectRegistryDatabase::Initialise without needing to execute a builder). (See Copy)
     * @return true if the following functions, called in order, return true:
     *   InitialiseSignalsDatabaseFromConfiguration(), FlattenSignalsDatabases(), ResolveDataSources(),
     * VerifyDataSourcesSignals(), ResolveFunctionSignals(), VerifyFunctionSignals(), ResolveStatesFromConfiguration(),
     * ResolveConsumersAndProducers(), VerifyConsumersAndProducers(), ResolveFunctionSignalsMemorySize(), ResolveFunctionsMemory()
     * and AssignFunctionsMemoryToDataSource();
     * @post
     *   The ObjectRegistryDatabase can now be initialised (typically this is to be performed in another application, see Set).
     *   The functions PostConfigureDataSources() and PostConfigureFunctions() can then be called.
     */
    virtual bool ConfigureBeforeInitialisation()=0;

    /**
     * @brief Copies the Function and DataSource databases.
     * @param[out] functionsDatabaseOut where to copy the Functions database into.
     * @param[out] dataSourcesDatabaseOut where to copy the DataSource database into.
     * @return true if both ConfigurationDatabase::Copy are successful.
     */
    virtual bool Copy(ConfigurationDatabase &functionsDatabaseOut,
                      ConfigurationDatabase &dataSourcesDatabaseOut)=0;

    /**
     * @brief Sets the Functions and Data databases.
     * @param[in] functionsDatabaseIn where to copy the Functions database from.
     * @param[in] dataSourcesDatabaseIn where to copy the DataSource database from.
     * @return true if both ConfigurationDatabase::Copy are successful.
     */
    virtual bool Set(ConfigurationDatabase &functionsDatabaseIn,
                     ConfigurationDatabase &dataSourcesDatabaseIn)=0;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_APPLICATIONBUILDERI_H_ */

