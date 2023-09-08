/**
 * @file MessagePrompter.h
 * @brief Header file for class MessagePrompter
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

 * @details This header file contains the declaration of the class MessagePrompter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGE_PROMPTER_H_
#define MESSAGE_PROMPTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "ConfigurationDatabase.h"
#include "MessageI.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "RegisteredMethodsMessageFilter.h"
#include "Sleep.h"
#include "signal.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace Example {

    class MessagePrompter: public MARTe::Object, public MARTe::MessageI {
        public:
            CLASS_REGISTER_DECLARATION()

            //@brief Constructor
            MessagePrompter ();
            //@brief Destructor
            virtual ~MessagePrompter ();

            MARTe::ErrorManagement::ErrorType SendMessage();
            MARTe::ErrorManagement::ErrorType Kill();

        protected:

        private:
            MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter> filter;
    };

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MESSAGE_PROMPTER_H_ */
