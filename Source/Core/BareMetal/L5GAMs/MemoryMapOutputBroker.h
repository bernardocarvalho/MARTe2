/**
 * @file MemoryMapOutputBroker.h
 * @brief Header file for class MemoryMapOutputBroker
 * @date Jul 18, 2016
 * @author aneto
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

 * @details This header file contains the declaration of the class MemoryMapOutputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPOUTPUTBROKER_H_
#define MEMORYMAPOUTPUTBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Output MemoryMapBroker implementation.
 * @details This class copies all the signals declared on a MemoryMapBroker
 * from the GAM memory to the DataSourceI memory.
 */
class DLL_API MemoryMapOutputBroker: public MemoryMapBroker {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
    MemoryMapOutputBroker();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MemoryMapOutputBroker();

    /**
     * @brief Sequentially copies all the signals from the GAM memory to the DataSourceI memory.
     * @details This implementation supports multi-state buffers and will query the DataSource for the GetCurrentStateBuffer.
     * @return true if all copies are successfully performed.
     */
    virtual bool Execute();
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPOUTPUTBROKER_H_ */

