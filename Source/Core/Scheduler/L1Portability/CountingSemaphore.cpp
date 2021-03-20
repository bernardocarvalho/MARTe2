/**
 * @file CountingSemaphore.cpp
 * @brief Source file for class CountingSemaphore
 * @date May 23, 2020
 * @author Giuseppe
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
 * the class CountingSemaphore (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CountingSemaphore.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

CountingSemaphore::CountingSemaphore() {
    // Auto-generated constructor stub for CountingSemaphore
    // TODO Verify if manual additions are needed
    numberOfActors = 0u;
    counter = 0;
}

CountingSemaphore::~CountingSemaphore() {
    // Auto-generated destructor stub for CountingSemaphore
    // TODO Verify if manual additions are needed
}

bool CountingSemaphore::Create(uint32 numberOfActorsIn) {
    bool ret = EventSem::Create();
    if (ret) {
        numberOfActors = numberOfActorsIn;
    }
    return ret;
}

ErrorManagement::ErrorType CountingSemaphore::WaitForAll(const TimeoutType &timeout) {
    ErrorManagement::ErrorType err;
    bool condition = false;
    if (sem.FastLock()) {
        counter++;
        condition = (counter == numberOfActors);
        if (counter > numberOfActors) {
            counter = numberOfActors;
        }
        sem.FastUnLock();
    }
    if (condition) {
        err = (!EventSem::Post());
    }
    else {
        err = EventSem::Wait(timeout);
    }

    return err;
}

bool CountingSemaphore::Reset() {
    if (sem.FastLock()) {
        if (counter >= numberOfActors) {
            counter = 0;
        }
        sem.FastUnLock();
    }
    return EventSem::Reset();
}

}
