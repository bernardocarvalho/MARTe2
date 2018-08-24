/**
 * @file Base64Encoder.cpp
 * @brief Source file for class Base64Encoder
 * @date 16 ago 2018
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
 * the class Base64Encoder (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Base64Encoder.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace Base64Encoder {

static class Base64Codec {
public:

    Base64Codec();

    ~Base64Codec();

    char8 decodingTable[256];

    static const char8 *base64Alphabet;

} base64Codec;

const char8 *Base64Codec::base64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

Base64Codec::Base64Codec() {
    for (uint32 i = 0u; i < 256u; i++) {
        decodingTable[i] = 0xFFu;
    }
    for (uint32 i = 0u; i < 64u; i++) {
        decodingTable[static_cast<uint8>(base64Alphabet[i])] = i;
    }
    decodingTable['='] = 0xFEu;

}

Base64Codec::~Base64Codec() {
}

bool Encode(StreamString &input,
            StreamString &output) {

    int32 size = input.Size();
    const uint8 *in = (const uint8 *) input.Buffer();
    output = "";
    uint32 word = 0;
    uint32 code;
    while (size > 0) {
        word = in[0];
        code = word >> 2;
        word &= 0x3;
        output += Base64Codec::base64Alphabet[code];
        if (size > 1) {
            word <<= 8;
            word |= in[1];
            code = word >> 4;
            word &= 0xF;
            output += Base64Codec::base64Alphabet[code];
            if (size > 2) {
                word <<= 8;
                word |= in[2];
                code = word;
                word &= 0x3F;
                code >>= 6;
                output += Base64Codec::base64Alphabet[code];
                output += Base64Codec::base64Alphabet[word];
            }
            else { // 4 bits available
                word <<= 2;
                output += Base64Codec::base64Alphabet[word];
                output += "=";
            }
        }
        else { // 2 bits available
            word <<= 4;
            output += Base64Codec::base64Alphabet[word];
            output += "==";
        }
        size -= 3;
        in += 3;
    }

    return true;
}

bool Decode(StreamString &input,
            StreamString &output) {

    uint32 size = input.Size();
    const uint8 *in = reinterpret_cast<const uint8*>(input.Buffer());
    output = "";
    uint32 word = 0;
    uint32 code;
    uint8 key;
    uint8 c;
    bool done = false;
    bool ret = true;
    while ((size >= 4) && (ret) && (!done)) {
        c = in[0];
        if (c == '=') {
            done = true;
        }
        if (!done) {
            key = base64Codec.decodingTable[c];
            if (key == 0xFF) {
                ret = false;
            }
        }

        if ((ret) && (!done)) {
            word = key;

            c = in[1];
            if (c == static_cast<uint8>('=')) {
                word <<= 2;
                output += word;
                done = true;
            }
            if (!done) {
                key = base64Codec.decodingTable[c];
                if (key == 0xFF) {
                    ret = false;
                }
            }
        }
        if ((ret) && (!done)) {

            word <<= 6;
            word |= key;
            code = (word >> 4);
            word &= 0xF;
            output += code;

            c = in[2];
            if (c == '=') {
                word <<= 4;
                output += word;
                done = true;
            }
            if (!done) {
                key = base64Codec.decodingTable[c];
                if (key == 0xFF) {
                    ret = false;
                }
            }
        }

        if ((ret) && (!done)) {
            word <<= 6;
            word |= key;
            code = (word >> 2);
            word &= 0x3;
            output += code;

            c = in[3];
            if (c == '=') {
                done = true;
            }
            if (!done) {
                key = base64Codec.decodingTable[c];
                if (key == 0xFF) {
                    ret = false;
                }
            }
        }
        if ((ret) && (!done)) {
            word <<= 6;
            word |= key;
            output += word;

            size -= 4;
            in += 4;
        }
    }
    return ret;

}

}
}
