/**
 * @file KeyActionGAM.h
 * @brief Header file for class KeyActionGAM
 * @date 06/08/2016
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

 * @details This header file contains the declaration of the class KeyActionGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef KEYACTIONGAM_H_
#define KEYACTIONGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "RegisteredMethodsMessageFilter.h"
#include "ConfigurationDatabase.h"
#include "Message.h"
#include "MessageI.h"
#include "GAM.h"
#include "NcursesInterface.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class KeyActionGAM: public GAM, public MessageI {
    public:
        CLASS_REGISTER_DECLARATION()

        KeyActionGAM();
        virtual ~KeyActionGAM();
        virtual bool Setup();
        virtual bool Execute();
        ErrorManagement::ErrorType Action(int32 k);

    private:
        MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter> filter;
        int32 actionKey;
};      

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* KEYACTIONGAM_H_ */