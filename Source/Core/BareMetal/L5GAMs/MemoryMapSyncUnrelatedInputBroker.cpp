/**
 * @file MemoryMapSyncUnrelatedInputBroker.cpp
 * @brief Source file for class MemoryMapSyncUnrelatedInputBroker
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
 * the class MemoryMapSyncUnrelatedInputBroker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryMapSyncUnrelatedInputBroker.h"
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

MemoryMapSyncUnrelatedInputBroker::MemoryMapSyncUnrelatedInputBroker() :
        MemoryMapUnrelatedInputBroker() {
    // Auto-generated constructor stub for MemoryMapSyncUnrelatedInputBroker
    // TODO Verify if manual additions are needed

}

MemoryMapSyncUnrelatedInputBroker::~MemoryMapSyncUnrelatedInputBroker() {
    // Auto-generated constructor stub for MemoryMapSyncUnrelatedInputBroker
    // TODO Verify if manual additions are needed

}

bool MemoryMapSyncUnrelatedInputBroker::Execute() {
    uint32 n;
    /*lint -e{613} null pointer checked before.*/
    uint32 i = dataSource->GetCurrentBuffer();
    dataSourceCust->Synchronise();
    for (n = 0u; (n < numberOfCopies); n++) {
        //REPORT_ERROR(ErrorManagement::Warning, "Executing...");
        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
            int32 offset = dataSourceCust->GetOffset(signalIdxArr[n], 0);
            if (offset >= 0) {
                uint32 dataSourceIndex = ((i * numberOfCopies) + n);
                MemoryOperationsHelper::Copy(copyTable[n].gamPointer, &(((uint8 *) (copyTable[dataSourceIndex].dataSourcePointer))[offset]), copyTable[n].copySize);
                dataSourceCust->TerminateRead(signalIdxArr[n], static_cast<uint32>(offset));
            }
        }
    }
    return true;
}
CLASS_REGISTER(MemoryMapSyncUnrelatedInputBroker, "1.0")
}
