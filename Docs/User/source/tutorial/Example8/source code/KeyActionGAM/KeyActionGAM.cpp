/**
 * @file KeyActionGAM.cpp
 * @brief Source file for class KeyActionGAM
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

 * @details This source file contains the definition of all the methods for
 * the class KeyActionGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "KeyActionGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

KeyActionGAM::KeyActionGAM() :
        GAM(),
        MessageI() 
{

    filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    InstallMessageFilter(filter);
}

KeyActionGAM::~KeyActionGAM() 
    {}

bool KeyActionGAM::Setup() 
    {return true;}

bool KeyActionGAM::Execute() 
{
    ReferenceT<Message> msg1(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdbMsg;

    cdbMsg.Purge();
    cdbMsg.Write("Destination", "NcursesInt");
    cdbMsg.Write("Function", "moveCursor");
    cdbMsg.Write("ExpectsReply", "true");
    cdbMsg.CreateAbsolute("+Parameters");
    cdbMsg.Write("Class", "ConfigurationDatabase");
    cdbMsg.Write("param1", actionKey); 
    cdbMsg.MoveToAncestor(1u); 
    msg1->Initialise(cdbMsg);
    MessageI::SendMessage(msg1);            // Send a message to NcursesInt with the key to move the cursor
    REPORT_ERROR(ErrorManagement::Debug, "KeyActionGAM moveCursor call message sent correctly with key=%d", actionKey);

    //Change the state of the statemachine to "GOTOIDLE"
    cdbMsg.Purge();
    cdbMsg.Write("Destination", "StateMachine");
    cdbMsg.Write("Function", "GOTOIDLE");  
    ReferenceT<Message> msg2(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    msg2->Initialise(cdbMsg);
    MessageI::SendMessage(msg2, this); 
    REPORT_ERROR(ErrorManagement::Debug, "Sent message for change of state (go back to IDLE)");

    return true;
}

ErrorManagement::ErrorType KeyActionGAM::Action(int32 keyCode) {
    actionKey = keyCode;

    return MARTe::ErrorManagement::NoError;
}

CLASS_REGISTER(KeyActionGAM, "1.0")
CLASS_METHOD_REGISTER(KeyActionGAM, Action)
