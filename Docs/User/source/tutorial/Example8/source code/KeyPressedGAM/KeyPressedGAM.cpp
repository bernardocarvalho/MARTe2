/**
 * @file KeyPressedGAM.cpp
 * @brief Source file for class KeyPressedGAM
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
 * the class KeyPressedGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "KeyPressedGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

KeyPressedGAM::KeyPressedGAM() :
        GAM() 
{
        //key = NULL_PTR(MARTe::int32 *);
        key = 0; 
        previous_key=(int)'d';
}

KeyPressedGAM::~KeyPressedGAM() 
    {}

bool KeyPressedGAM::Setup() 
{
    //key = reinterpret_cast<int32 *>(GetOutputSignalMemory(0u)); this reserves the memory for a signal. If we do it like this, it will reserve by index. That is, in the conf file the first signal will be this address - order is important. If we want to reserve it with a specific name, we should do it in the initialize()

    return true;
}

bool KeyPressedGAM::Execute() {
    using namespace MARTe;
 
    // We are now going to initialize the message we want to send to the ncurses interface
    // The idea is to call the function "readKey" of the NcursesInterface object defined in the .cfg file passing "key" as parameter
    
    // Since our functions expects a parameter, we need to define the structure properly, 
    // even if the value we sent in this message is not relevant (the parameter will only be used for the function 
    // to be able to reply with the key value)
    ConfigurationDatabase cdbMsg;
    cdbMsg.Purge();
    cdbMsg.Write("Destination", "NcursesInt");
    cdbMsg.Write("Function", "readKey");
    cdbMsg.CreateAbsolute("+Parameters");
    cdbMsg.Write("Class", "ConfigurationDatabase");
    cdbMsg.Write("param1", 0);  // we can use any value here. The idea is that key returns the answer to the message
    cdbMsg.MoveToAncestor(1u);  // This is needed as we went down one level in the previous methods

    ReferenceT<Message> msg0(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!msg0->Initialise(cdbMsg)) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to initialise message");   
    }
    ErrorManagement::ErrorType err = MessageI::SendMessageAndWaitReply(msg0, this);
    if (!err.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to send message");   
    } 
     
    //If the message was properly sent, the same message will include the reply in the parameter of the call, in this case 'key'
    ReferenceT<ConfigurationDatabase> params = msg0->Get(0);
    if (params.IsValid()) {
        if (params->Read("param1", key)) {  
            if (key == ERR) {           //ERR = -1 
                REPORT_ERROR(ErrorManagement::Debug, "No key has been pressed. Calling moveCursor with same direction");               
                // Send a message to NcursesInt with the previous key code to keep moving in the same direction
                cdbMsg.Purge();
                cdbMsg.Write("Destination", "NcursesInt");
                cdbMsg.Write("Function", "moveCursor");
                cdbMsg.CreateAbsolute("+Parameters");
                cdbMsg.Write("Class", "ConfigurationDatabase");
                cdbMsg.Write("param1", previous_key); 
                cdbMsg.MoveToAncestor(1u); 
                ReferenceT<Message> msg1(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                msg1->Initialise(cdbMsg);
                err = MessageI::SendMessage(msg1, this);
                usleep(50000);         //Time to wait in microseconds
            } else {
                if (key != 27) {    // key not ESC
                    previous_key = key; 
                    cdbMsg.Purge();
                    cdbMsg.Write("Destination", "StateMachine");
                    cdbMsg.Write("Function", "GOTOKEYPRESSED");  
                    ReferenceT<Message> msg2(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                    msg2->Initialise(cdbMsg);
                    err = MessageI::SendMessage(msg2, this);        //Change the state of the statemachine to "GOTOKEYPRESSED"
                    
                    cdbMsg.Purge();
                    cdbMsg.Write("Destination", "TestApp.Functions.ActionKeyGAM");  //Note that we need to provide the full path to the GAM or we may 
                                                                                    //get a message saying that the GAM is not defined
                    cdbMsg.Write("Function", "Action");
                    cdbMsg.CreateAbsolute("+Parameters");
                    cdbMsg.Write("Class", "ConfigurationDatabase");
                    cdbMsg.Write("param1", key); 
                    cdbMsg.MoveToAncestor(1u); 
                    ReferenceT<Message> msg3(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                    msg3->Initialise(cdbMsg);
                    err = MessageI::SendMessage(msg3, this);        //Send a message to KeyActionGAM with the key code
                } else {
                    // Ensure that Ncurses is properly closed before leaving, otherwise the terminal can have unexpected behaviour
                    cdbMsg.Purge();
                    cdbMsg.Write("Destination", "NcursesInt");
                    cdbMsg.Write("Function", "closeNcurses");
                    ReferenceT<Message> msg4(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                    msg4->Initialise(cdbMsg);
                    err = MessageI::SendMessage(msg4, this);
                    
                    // Send message to kill the application
                    REPORT_ERROR(ErrorManagement::Debug, "ESC pressed. Finishing application.");
                    cdbMsg.Purge();
                    cdbMsg.Write("Destination", "AppKiller");
                    cdbMsg.Write("Function", "Kill"); 
                    ReferenceT<Message> msg5(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                    msg5->Initialise(cdbMsg);
                    err = MessageI::SendMessage(msg5, this);
                    usleep(1000000);         // We need to wait for nCurses to properly close
                }
            }
        }
    } else {
        printf("param1 is not valid!");
    }
    
    return err;
}

CLASS_REGISTER(KeyPressedGAM, "1.0")
