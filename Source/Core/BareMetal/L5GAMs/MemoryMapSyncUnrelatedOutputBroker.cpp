/**
 * @file MemoryMapSyncUnrelatedOutputBroker.cpp
 * @brief Source file for class MemoryMapSyncUnrelatedOutputBroker
 * @date Apr 12, 2018
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
 * the class MemoryMapSyncUnrelatedOutputBroker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryMapSyncUnrelatedOutputBroker.h"
#include "MemoryOperationsHelper.h"
#include "Endianity.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

MemoryMapSyncUnrelatedOutputBroker::MemoryMapSyncUnrelatedOutputBroker() :
        MemoryMapUnrelatedOutputBroker() {
// Auto-generated constructor stub for MemoryMapSyncUnrelatedOutputBroker
// TODO Verify if manual additions are needed

}

MemoryMapSyncUnrelatedOutputBroker::~MemoryMapSyncUnrelatedOutputBroker() {
// Auto-generated constructor stub for MemoryMapSyncUnrelatedOutputBroker
// TODO Verify if manual additions are needed

}

bool MemoryMapSyncUnrelatedOutputBroker::Execute() {

    bool ret = MemoryMapUnrelatedBroker::ExecuteCoreOut();

    if (ret) {
        ret = dataSourceCust->Synchronise();
    }

    return ret;
}
CLASS_REGISTER(MemoryMapSyncUnrelatedOutputBroker, "1.0")
}
