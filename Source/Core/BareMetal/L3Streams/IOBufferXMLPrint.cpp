/**
 * @file IOBufferXMLPrint.cpp
 * @brief Source file for class IOBufferXMLPrint
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
 * the class IOBufferXMLPrint (public, protected, and private). Be aware that some 
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

/*---------------------------------------------------------------------------*/
/*                           definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

bool PrintXMLOpenMatrix(IOBuffer &iobuff) {
    return iobuff.PutC('[');
}

bool PrintXMLCloseMatrix(IOBuffer &iobuff) {
    return iobuff.PutC(']');
}

bool PrintXMLScalarSeparator(IOBuffer &iobuff) {
    return iobuff.PutC(',');
}

bool PrintXMLVectorSeparator(IOBuffer &iobuff) {
    return iobuff.PutC(',');
}

bool PrintXMLOpenVector(IOBuffer &iobuff) {
    return iobuff.PutC('[');

}

bool PrintXMLCloseVector(IOBuffer &iobuff) {
    return iobuff.PutC(']');
}

bool PrintXMLOpenBlock(IOBuffer &iobuff,
                       const char8* blockName) {
    AnyType at[] = { blockName , voidAnyType};
    return iobuff.PrintFormatted("<%s>", at);
}

bool PrintXMLCloseBlock(IOBuffer &iobuff,
                        const char8* blockName) {
    AnyType at[] = { blockName , voidAnyType};
    return iobuff.PrintFormatted("</%s>", at);
}

bool PrintXMLOpenAssignment(IOBuffer &iobuff,
                            const char8* varName) {
    AnyType at[] = { varName , voidAnyType};
    return iobuff.PrintFormatted("<%s>", at);
}

bool PrintXMLCloseAssignment(IOBuffer &iobuff,
                             const char8* varName) {
    AnyType at[] = { varName , voidAnyType};
    return iobuff.PrintFormatted("</%s>", at);
}

}

