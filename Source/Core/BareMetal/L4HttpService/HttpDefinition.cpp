/**
 * @file HttpDefinition.cpp
 * @brief Source file for class HttpDefinition
 * @date 17 ago 2018
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
 * the class HttpDefinition (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HttpDefinition.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
namespace HttpDefinition {
static const char *Http2Convert = " #%<>";

bool HttpEncode(BufferedStreamI &converted,
                const char8 *original) {

    bool ret = true;
    if (original != NULL) {
        while (*original != '\0') {
            char8 c = *original;
            original=&original[1];
            if (StringHelper::SearchChar(Http2Convert, c)) {
                ret=converted.Printf("%%%2x", c);
            }
            else {
                uint32 size=1u;
                ret=converted.Write(&c, size);
            }
        }
    }
    return ret;
}

int32 HexDecode(char8 c) {
    int32 ret = -1;
    if ((c >= '0') && (c <= '9')) {
        ret = (c - '0');
    }
    if ((c >= 'a') && (c <= 'f')) {
        ret = ((c - 'a') + 10);
    }
    if ((c >= 'A') && (c <= 'F')) {
        ret = ((c - 'A') + 10);
    }
    return ret;
}

bool HttpDecode(BufferedStreamI &destination,
                BufferedStreamI &source) {
    char8 c;
    bool ret = true;
    uint32 sizeR = 1u;
    while ((sizeR > 0u) && (ret)) {
        source.Read(&c, sizeR);
        if (sizeR > 0u) {
            if (c == '%') {
                char8 buffer[2];
                uint32 size = 2;
                source.Read(buffer, size);
                c = HexDecode(buffer[0]) * 16 + HexDecode(buffer[1]);
            }
            uint32 sizeW = 1u;
            ret = destination.Write(&c, sizeW);
            sizeR = 1;
        }
    }
    return ret;

}

}
}
