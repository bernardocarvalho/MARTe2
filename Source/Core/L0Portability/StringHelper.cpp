/**
 * @file StringHelper.cpp
 * @brief Source file for class StringHelper
 * @date Aug 13, 2015
 * @author fsartori
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
 * the class StringHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StringHelper.h"
#include "HeapManager.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace StringHelper {
/**
 * @brief Duplicates a string in the heap memory.
 * @param[in] s The pointer to the string which must be copied.
 * @return The pointer to the new allocated memory which contains a copy of s.
 */
char8 *StringDup(const char8 * const s){

    char8 *duplicate = NULL_PTR(char8 *);
    if (s != NULL){

        void *copy =  HeapManager::Duplicate(static_cast<const void *>(s));
        duplicate = static_cast<char8 *>(copy);

    }

    return duplicate;
}

}

	
