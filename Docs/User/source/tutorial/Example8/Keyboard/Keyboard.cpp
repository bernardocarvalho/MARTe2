/**
 * @file Keyboard.cpp
 * @brief Source file for class Keyboard
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

 * @details This source file contains the definition of all the methods for
 * the class Keyboard (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Keyboard.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

Keyboard::Keyboard() :
        Object(),
        MessageI() {
  
    filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (ret != MARTe::ErrorManagement::NoError) {
        REPORT_ERROR(ErrorManagement::Information, "Error installing filter in constructor!");
    }
    
}

Keyboard::~Keyboard () {
}

void Keyboard::PrepareNcurses()
{
    initscr();
    raw();
    //newterm(NULL, stderr, stdin);
    w = newwin(100, 200, 1, 1);

    // making box border with default border styles
    box(w, 0, 0);

    // move and print in window
    mvwprintw(w, 0, 1, "Window loaded succesfully");

    // refreshing the window
    wrefresh(w);
}

int32 Keyboard::ReadKey() {    
    int32 ch = getch();

    while (true) {
        if (ch != ERR) {
            return ch;
        } else {
            ch = getch();
        }
    }
}

