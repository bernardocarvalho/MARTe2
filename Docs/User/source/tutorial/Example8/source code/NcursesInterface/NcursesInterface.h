/**
 * @file NcursesInterface.h
 * @brief Header file for class IOGAM
 * @date 05/07/2023
 * @author José María Gómez
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

 * @details This header file contains the declaration of the class IOGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NCURSESINTERFACE_H_
#define NCURSESINTERFACE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "MessageI.h"
#include "RegisteredMethodsMessageFilter.h"
#include "CLASSMETHODREGISTER.h"
#include "AdvancedErrorManagement.h"
#include "ncurses.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class NcursesInterface: public MARTe::Object, public MARTe::MessageI {
    public:
        CLASS_REGISTER_DECLARATION()    
        
        NcursesInterface ();
        virtual ~NcursesInterface ();
        MARTe::ErrorManagement::ErrorType readKey(int &keyCode); // Needs to be defined like this to allow replying to the messages
        WINDOW* getWin();
        int getPosx();
        int getPosy();
        void setPosx(int x);
        void setPosy(int y);
        bool getFirstExec();
        MARTe::ErrorManagement::ErrorType moveCursor(int keyCode);
        MARTe::ErrorManagement::ErrorType prepareNcurses(int, int, int, int);
        MARTe::ErrorManagement::ErrorType closeNcurses();
        
    private:
        MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter> filter;
        WINDOW *win;
        int posx;
        int posy;
        bool firstExec;

       
        void writeCoords(int y, int x, const char* ch);
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NCURSESINTERFACE_H_ */

