/**
 * @file MessagePrompter.cpp
 * @brief Source file for class MessagePrompter
 * @date 15/06/2023
 * @author Jose M Gomez
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
 * the class MessagePrompter (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MessagePrompter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Example {
    using namespace MARTe;
    MessagePrompter::MessagePrompter() :
            Object(),
            MessageI() {
        filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        MARTe::MessageI::InstallMessageFilter(filter);
    }

    MessagePrompter::~MessagePrompter   () {
    }
 
    ErrorManagement::ErrorType MessagePrompter::SendMessage() {
        using namespace MARTe;
        
        REPORT_ERROR(ErrorManagement::Information, "State Changed!");
        
        return true;
    }

    ErrorManagement::ErrorType MessagePrompter::Kill() {
        //if (kill(0, SIGUSR1) == 0) { //0 --> terminate all process in the current PGID. Therefore it kills the script because this process belongs to the PGID of the script as well
        if (kill(getpid(), SIGUSR1) == 0) {
        REPORT_ERROR(ErrorManagement::Information, "Kill signal successfully sent!");
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Signal sending error");
        }
        return true;
    }

    CLASS_REGISTER(MessagePrompter, "1.0")
    CLASS_METHOD_REGISTER(MessagePrompter, SendMessage)
    CLASS_METHOD_REGISTER(MessagePrompter, Kill)
}
