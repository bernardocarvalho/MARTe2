/**
 * @file MemoryMapUnrelatedOutputBrokerTest.h
 * @brief Header file for class MemoryMapUnrelatedOutputBrokerTest
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

 * @details This header file contains the declaration of the class MemoryMapUnrelatedOutputBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPUNRELATEDOUTPUTBROKERTEST_H_
#define MEMORYMAPUNRELATEDOUTPUTBROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapUnrelatedOutputBroker.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class MemoryMapUnrelatedOutputBrokerTest {
public:
    MemoryMapUnrelatedOutputBrokerTest();
    virtual ~MemoryMapUnrelatedOutputBrokerTest();
    bool TestConstructor();

    bool TestInit();
    bool TestExecute();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPUNRELATEDOUTPUTBROKERTEST_H_ */
