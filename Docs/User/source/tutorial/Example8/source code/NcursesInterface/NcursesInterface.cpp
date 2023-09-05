/**
 * @file NcursesInterface.cpp
 * @brief Source file for class NcursesInterface
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
 * the class NcursesInterface (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "NcursesInterface.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

static const int ROWS = 20;
static const int COLUMNS = 70;

NcursesInterface::NcursesInterface() :
        Object(),
        MessageI() 
{
    win = NULL;
    posx = 0;
    posy = 0;
    firstExec = true;
    filter = MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter>(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    MARTe::MessageI::InstallMessageFilter(filter);
}

NcursesInterface::~NcursesInterface () 
{}

WINDOW* NcursesInterface::getWin()
    {return win;}

int NcursesInterface::getPosx()
    {return posx;}

int NcursesInterface::getPosy()
    {return posy;}

void NcursesInterface::setPosx(int x) 
    {posx = x;}

void NcursesInterface::setPosy(int y) 
    {posy = y;}

void NcursesInterface::writeCoords(int y, int x, const char* ch) 
    {mvwprintw(win, y, x, ch);}

MARTe::ErrorManagement::ErrorType NcursesInterface::prepareNcurses(int rows=ROWS, int cols=COLUMNS, int win_begin_y=1, int win_begin_x=1)
{
    if (firstExec) {
        initscr();
        noecho();   // no echo when pressing a key
        cbreak();   // get input without need of pressing Enter   
        timeout(0);  // no waiting for key press

        win = newwin(rows, cols, win_begin_y, win_begin_x);     // Create a window with borders
        printw("Move the cursor! Pres 'w' to move up, 's' to move down, 'a' to move left or 'd' to move right.");
        wrefresh(win);  // refresh the window
        firstExec = false;
    } else {                             
        wrefresh(win);  //refresh the window         
    }    

    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType NcursesInterface::closeNcurses() 
{
    endwin();
    
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType NcursesInterface::readKey(MARTe::int32 &keyCode) 
{ 
    prepareNcurses();
    keyCode = getch();
    closeNcurses();

    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType NcursesInterface::moveCursor(int keyCode) 
{
    const char* cursor = "C";
    prepareNcurses();
    
    switch(keyCode) {
        case (int)'a':
            if (getPosx()-1 < 0) {
                writeCoords(getPosy(), getPosx(), cursor);
            } else {
                writeCoords(getPosy(), getPosx()," ");
                setPosx(getPosx()-1);
                writeCoords(getPosy(), getPosx(), cursor);
            }
            break;
        case (int)'d':
            if (getPosx()+1 > COLUMNS-1) {
                writeCoords(getPosy(), getPosx(), cursor);
            } else {
                writeCoords(getPosy(), getPosx()," ");
                setPosx(getPosx()+1);
                writeCoords(getPosy(), getPosx(), cursor);
            }
            break;
        case (int)'w':
            if (getPosy()-1 < 0) {
                writeCoords(getPosy(), getPosx(), cursor);
            } else {
                writeCoords(getPosy(), getPosx()," ");
                setPosy(getPosy()-1);
                writeCoords(getPosy(), getPosx(), cursor);
            }
            break;
        case (int)'s':
            if (getPosy()+1 > ROWS-1) {
                writeCoords(getPosy(), getPosx(), cursor);
            } else {
                writeCoords(getPosy(), getPosx()," ");
                setPosy(getPosy()+1);
                writeCoords(getPosy(), getPosx(), cursor);
            }
            break;
        default:    
            writeCoords(getPosy(), getPosx(), cursor);        
            break;
    }
    wrefresh(win);

    return MARTe::ErrorManagement::NoError;
}

// We need to register the methods to allow them to be used by other MARTe objects
CLASS_REGISTER(NcursesInterface, "")
CLASS_METHOD_REGISTER(NcursesInterface, readKey)
CLASS_METHOD_REGISTER(NcursesInterface, moveCursor)
CLASS_METHOD_REGISTER(NcursesInterface, prepareNcurses)
CLASS_METHOD_REGISTER(NcursesInterface, closeNcurses)