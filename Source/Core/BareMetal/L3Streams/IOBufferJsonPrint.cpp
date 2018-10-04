/**
 * @file IOBufferJsonPrint.cpp
 * @brief Source file for class IOBufferJsonPrint
 * @date 06/09/2018
 * @author Giuseppe Ferro
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
 * the class IOBufferJsonPrint (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Json header includes                          */
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

/**
 * @brief Prints the Json open matrix separator
 * @param[in] iobuff the IOBuffer where to print to
 */
bool PrintJsonOpenMatrix(IOBuffer &iobuff) {
    return iobuff.PutC('[');
}

/**
 * @brief Prints the Json close matrix separator
 * @param[in] iobuff the IOBuffer where to print to
 */
bool PrintJsonCloseMatrix(IOBuffer &iobuff) {
    return iobuff.PutC(']');
}

/**
 * @brief Prints the Json scalar separator
 * @param[in] iobuff the IOBuffer where to print to
 */
bool PrintJsonScalarSeparator(IOBuffer &iobuff) {
    return iobuff.PutC(',');
}

/**
 * @brief Prints the Json vector separator
 * @param[in] iobuff the IOBuffer where to print to
 */
bool PrintJsonVectorSeparator(IOBuffer &iobuff) {
    return iobuff.PutC(',');
}

/**
 * @brief Prints the Json variable separator
 * @param[in] iobuff the IOBuffer where to print to
 */
bool PrintJsonVariableSeparator(IOBuffer &iobuff) {
    return iobuff.PutC(',');
}

/**
 * @brief Prints the Json block separator
 * @param[in] iobuff the IOBuffer where to print to
 */
bool PrintJsonBlockSeparator(IOBuffer &iobuff) {
    return iobuff.PutC(',');
}

/**
 * @brief Prints the Json open vector separator
 * @param[in] iobuff the IOBuffer where to print to
 */
bool PrintJsonOpenVector(IOBuffer &iobuff) {
    return iobuff.PutC('[');

}

/**
 * @brief Prints the Json close vector separator
 * @param[in] iobuff the IOBuffer where to print to
 */
bool PrintJsonCloseVector(IOBuffer &iobuff) {
    return iobuff.PutC(']');
}

/**
 * @brief Prints the Json open block separator
 * @param[in] iobuff the IOBuffer where to print to
 */
bool PrintJsonOpenBlock(IOBuffer &iobuff, const char8 * const blockName) {
    AnyType at[]={blockName, voidAnyType};
    return iobuff.PrintFormatted("\"%s\": {", at);
}

/**
 * @brief Prints the Json close block separator
 * @param[in] iobuff the IOBuffer where to print to
 */
bool PrintJsonCloseBlock(IOBuffer &iobuff) {
    return iobuff.PutC('}');
}

/**
 * @brief Prints the Json open assignment separator
 * @param[in] iobuff the IOBuffer where to print to
 */
bool PrintJsonOpenAssignment(IOBuffer &iobuff,
                                const char8 * const varName) {
    uint32 size = StringHelper::Length(varName);
    bool ret = iobuff.PutC('"');
    if (ret) {
        ret = iobuff.Write(varName, size);
    }
    if (ret) {
        ret = iobuff.PutC('"');
    }
    if (ret) {
        ret = iobuff.PutC(':');
    }
    return ret;
}

}
