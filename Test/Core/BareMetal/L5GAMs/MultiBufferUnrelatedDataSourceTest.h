/**
 * @file MultiBufferUnrelatedDataSourceTest.h
 * @brief Header file for class MultiBufferUnrelatedDataSourceTest
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

 * @details This header file contains the declaration of the class MultiBufferUnrelatedDataSourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MULTIBUFFERUNRELATEDDATASOURCETEST_H_
#define MULTIBUFFERUNRELATEDDATASOURCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MultiBufferUnrelatedDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;


class MultiBufferUnrelatedDataSourceTest {
public:

    MultiBufferUnrelatedDataSourceTest();

    ~MultiBufferUnrelatedDataSourceTest();

    bool TestInitialise();

    bool TestInitialise_StandardHeapDefault();

    bool TestInitialise_False_NoNumberOfBuffers();

    bool TestInitialise_False_InvalidHeap();

    bool TestAllocateMemory();

    bool TestGetNumberOfMemoryBuffers();

    bool TestGetSignalMemoryBuffer();

    bool TestGetBrokerName();

    bool TestGetInputBrokers();

    bool TestGetInputBrokers_SuggestedBroker();

    bool TestGetInputBrokers_False_InvalidSuggestedBroker();

    bool TestGetOutputBrokers();

    bool TestGetOutputBrokers_SuggestedBroker();

    bool TestGetOutputBrokers_False_InvalidSuggestedBroker();

    bool TestSetConfiguredDatabase();

    bool TestSetConfiguredDatabase_False_InternalTimeStamp_BadType();

    bool TestSetConfiguredDatabase_False_InternalTimeStamp_Produced();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MULTIBUFFERUNRELATEDDATASOURCETEST_H_ */

