/**
 * @file EmbeddedThreadTest.h
 * @brief Header file for class EmbeddedThreadTest
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

 * @details This header file contains the declaration of the class EmbeddedThreadTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EMBEDDEDTHREADTEST_H_
#define EMBEDDEDTHREADTEST_H_

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
class EmbeddedThreadTest {
public:

    /**
     * @brief Default constructor
     */
    EmbeddedThreadTest();

    /**
     * @brief Destructor
     */
    virtual ~EmbeddedThreadTest();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor_Template();

    /**
     * @brief Tests the Initialise function.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise function without specifying the Timeout parameter.
     */
    bool TestInitialise_False();

    /**
     * @brief Tests the Start function.
     */
    bool TestStart();

    /**
     * @brief Tests the Start function calling it twice, which should trigger an illegal operation.
     */
    bool TestStart_False();

    /**
     * @brief Tests the Start function after a stop has been called.
     */
    bool TestStart_Restart();

    /**
     * @brief Tests the Stop function.
     */
    bool TestStop();

    /**
     * @brief Tests the Stop function forcing a kill.
     */
    bool TestStop_Kill();

    /**
     * @brief Tests the ThreadLoop function.
     */
    bool TestThreadLoop();

    /**
     * @brief Tests the GetStatus function.
     */
    bool TestGetStatus();

    /**
     * @brief Tests the SetTimeout function
     */
    bool TestSetTimeout();

    /**
     * @brief Tests the GetTimeout function
     */
    bool TestGetTimeout();

    /**
     * @brief Tests the GetThreadId function
     */
    bool TestGetThreadId();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EMBEDDEDTHREADTEST_H_ */

