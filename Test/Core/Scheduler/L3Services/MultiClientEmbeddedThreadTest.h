/**
 * @file MultiClientEmbeddedThreadTest.h
 * @brief Header file for class MultiClientEmbeddedThreadTest
 * @date 19/09/2016
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class MultiClientEmbeddedThreadTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MULTICLIENTEMBEDDEDTHREADTEST_H_
#define MULTICLIENTEMBEDDEDTHREADTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EmbeddedThread.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the EmbeddedThread public methods.
 */
class MultiClientEmbeddedThreadTest {
public:

    /**
     * @brief Default constructor
     */
    MultiClientEmbeddedThreadTest();

    /**
     * @brief Destructor
     */
    virtual ~MultiClientEmbeddedThreadTest();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the ThreadLoop method.
     */
    bool TestThreadLoop();

    /**
     * @brief Callback function of the thread.
     */
    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information);

    /**
     * True when the Execute method is called.
     */
    bool main;
    bool completed;
    bool badTermination;
    bool startup;
    bool nullStageSpecific;
    bool waitRequestStageSpecific;
    bool serviceRequestStageSpecific;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MULTICLIENTEMBEDDEDTHREADTEST_H_ */

