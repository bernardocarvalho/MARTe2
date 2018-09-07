/**
 * @file IOBufferStandardPrint.cpp
 * @brief Source file for class IOBufferStandardPrint
 * @date 06 set 2018
 * @author pc
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
 * the class IOBufferStandardPrint (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "IOBuffer.h"
#include "AnyType.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

bool PrintStandardOpenMatrix(IOBuffer &iobuff) {
    return iobuff.PutC('{');
}


bool PrintStandardCloseMatrix(IOBuffer &iobuff) {
    return iobuff.PutC('}');
}

bool PrintStandardScalarSeparator(IOBuffer &iobuff) {
    return true;
}

bool PrintStandardVectorSeparator(IOBuffer &iobuff) {
    return true;
}


bool PrintStandardOpenVector(IOBuffer &iobuff) {
    return iobuff.PutC('{');

}

bool PrintStandardCloseVector(IOBuffer &iobuff) {
    return iobuff.PutC('}');
}


bool PrintStandardOpenBlock(IOBuffer &iobuff, const char8 *blockName) {
    AnyType at[]={blockName, voidAnyType};
    return iobuff.PrintFormatted("%s = {", at);
}

bool PrintStandardCloseBlock(IOBuffer &iobuff) {
    return iobuff.PutC('}');
}

bool PrintStandardOpenAssignment(IOBuffer &iobuff, const char8 *varName){
    uint32 size=StringHelper::Length(varName);
    bool ret=iobuff.Write(varName, size);
    if(ret){
        size=2u;
        ret=iobuff.Write(" =", size);
    }
    return ret;
}

}


