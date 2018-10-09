/**
 * @file Md5Encrypt.cpp
 * @brief Source file for class Md5Encrypt
 * @date 23 ago 2018
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
 * the class Md5Encrypt (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Md5Encrypt.h"
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace Md5Encrypt {

/*
 * 32-bit integer manipulation macros (little endian)
 */
#ifndef GET_UINT32_LE
#define GET_UINT32_LE(n,b,i)                            \
{                                                       \
    (n) = ( static_cast<osulong> ((b)[(i)    ])       )        \
        | ( static_cast<osulong> ((b)[(i) + 1]) <<  8u )        \
        | ( static_cast<osulong> ((b)[(i) + 2]) << 16u )        \
        | ( static_cast<osulong> ((b)[(i) + 3]) << 24u );       \
}
#endif

#ifndef PUT_UINT32_LE
#define PUT_UINT32_LE(n,b,i)                            \
{                                                       \
    (b)[(i)    ] = static_cast<uint8> ( (n)       );       \
    (b)[(i) + 1] = static_cast<uint8> ( (n) >>  8u );       \
    (b)[(i) + 2] = static_cast<uint8> ( (n) >> 16u );       \
    (b)[(i) + 3] = static_cast<uint8> ( (n) >> 24u );       \
}
#endif

static const uint8 md5_padding[64] = { 0x80u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
        0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u };

class Md5Context {
public:
    Md5Context();
    ~Md5Context();

    /*
     * MD5 context setup
     */
    void Starts();

    void Update(uint8 *input,
                uint32 ilen);

    void Finish(uint8 *output);

    void Md5Process(uint8 data[64]);

    /*
     * MD5 HMAC context setup
     */
    void HmacStarts(uint8 *key,
                    uint32 keylen);

    /*
     * MD5 HMAC process buffer
     */
    void HmacUpdate(uint8 *input,
                    uint32 ilen);

    /*
     * MD5 HMAC final digest
     */
    void HmacFinish(uint8 *output);

private:

    osulong total[2]; /*!< number of bytes processed  */
    osulong state[4]; /*!< intermediate digest state  */
    uint8 buffer[64]; /*!< data block being processed */
    uint8 ipad[64]; /*!< HMAC: inner padding        */
    uint8 opad[64]; /*!< HMAC: outer padding        */

};

Md5Context::Md5Context() {
    total[0] = 0u;
    total[1] = 0u;

    state[0] = 0x67452301u;
    state[1] = 0xEFCDAB89u;
    state[2] = 0x98BADCFEu;
    state[3] = 0x10325476u;

    (void)MemoryOperationsHelper::Set(&buffer[0], '\0', 64u);
    (void)MemoryOperationsHelper::Set(&ipad[0], '\0', 64u);
    (void)MemoryOperationsHelper::Set(&opad[0], '\0', 64u);

}

Md5Context::~Md5Context() {

}

/*
 * MD5 context setup
 */
void Md5Context::Starts() {
    total[0] = 0u;
    total[1] = 0u;

    state[0] = 0x67452301u;
    state[1] = 0xEFCDAB89u;
    state[2] = 0x98BADCFEu;
    state[3] = 0x10325476u;
}

void Md5Context::Update(uint8 *input,
                        uint32 ilen) {
    uint32 fill;
    osulong left;

    if (ilen > 0u) {

        left = total[0] & 0x3Fu;
        fill = 64u - static_cast<uint32>(left);

        total[0] += ilen;
        total[0] &= 0xFFFFFFFFu;

        if (total[0] < static_cast<osulong>(ilen)){
            total[1]++;
        }

        if ((left!=0u) && (ilen >= fill)) {
            (void)MemoryOperationsHelper::Copy(reinterpret_cast<void *> (&buffer[left]), reinterpret_cast<void *>(input), fill);
            Md5Process(&buffer[0]);
            input = &input[fill];
            ilen -= fill;
            left = 0u;
        }

        while (ilen >= 64u) {
            Md5Process(input);
            input = &input[64];
            ilen -= 64u;
        }

        if (ilen > 0u) {
            (void)MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&buffer[left]), reinterpret_cast<void *>(input), ilen);
        }
    }
}

void Md5Context::Finish(uint8 * const output) {
    osulong last;
    osulong padn;
    osulong high;
    osulong low;
    uint8 msglen[8];

    high = (total[0] >> 29u) | (total[1] << 3u);
    low = (total[0] << 3u);

    /*lint -e{835} zero sum allowed*/
    PUT_UINT32_LE(low, msglen, 0)
    PUT_UINT32_LE(high, msglen, 4)

    last = total[0] & 0x3Fu;
    padn = (last < 56u) ? (56u - last) : (120u - last);

    Update(const_cast<uint8 *>(md5_padding), static_cast<uint32>(padn));
    Update(&msglen[0], 8u);

    /*lint -e{835} zero sum allowed*/
    PUT_UINT32_LE(state[0], output, 0)
    PUT_UINT32_LE(state[1], output, 4)
    PUT_UINT32_LE(state[2], output, 8)
    PUT_UINT32_LE(state[3], output, 12)
}

void Md5Context::Md5Process(uint8 data[64]) {
    osulong X[16];
    osulong A;
    osulong B;
    osulong C;
    osulong D;

    /*lint -e{835} zero sum allowed*/
    GET_UINT32_LE(X[0], data, 0)
    GET_UINT32_LE(X[1], data, 4)
    GET_UINT32_LE(X[2], data, 8)
    GET_UINT32_LE(X[3], data, 12)
    GET_UINT32_LE(X[4], data, 16)
    GET_UINT32_LE(X[5], data, 20)
    GET_UINT32_LE(X[6], data, 24)
    GET_UINT32_LE(X[7], data, 28)
    GET_UINT32_LE(X[8], data, 32)
    GET_UINT32_LE(X[9], data, 36)
    GET_UINT32_LE(X[10], data, 40)
    GET_UINT32_LE(X[11], data, 44)
    GET_UINT32_LE(X[12], data, 48)
    GET_UINT32_LE(X[13], data, 52)
    GET_UINT32_LE(X[14], data, 56)
    GET_UINT32_LE(X[15], data, 60)

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFFu) >> (32u - n)))

#define P(a,b,c,d,k,s,t)                                \
{                                                       \
    a += F(b,c,d) + X[k] + t; a = S(a,s) + b;           \
}

    A = state[0];
    B = state[1];
    C = state[2];
    D = state[3];

#define F(x,y,z) (z ^ (x & (y ^ z)))

    P(A, B, C, D, 0, 7, 0xD76AA478u)
    P(D, A, B, C, 1, 12, 0xE8C7B756u)
    P(C, D, A, B, 2, 17, 0x242070DBu)
    P(B, C, D, A, 3, 22, 0xC1BDCEEEu)
    P(A, B, C, D, 4, 7, 0xF57C0FAFu)
    P(D, A, B, C, 5, 12, 0x4787C62Au)
    P(C, D, A, B, 6, 17, 0xA8304613u)
    P(B, C, D, A, 7, 22, 0xFD469501u)
    P(A, B, C, D, 8, 7, 0x698098D8u)
    P(D, A, B, C, 9, 12, 0x8B44F7AFu)
    P(C, D, A, B, 10, 17, 0xFFFF5BB1u)
    P(B, C, D, A, 11, 22, 0x895CD7BEu)
    P(A, B, C, D, 12, 7, 0x6B901122u)
    P(D, A, B, C, 13, 12, 0xFD987193u)
    P(C, D, A, B, 14, 17, 0xA679438Eu)
    P(B, C, D, A, 15, 22, 0x49B40821u)

#undef F

#define F(x,y,z) (y ^ (z & (x ^ y)))

    P(A, B, C, D, 1, 5, 0xF61E2562u)
    P(D, A, B, C, 6, 9, 0xC040B340u)
    P(C, D, A, B, 11, 14, 0x265E5A51u)
    P(B, C, D, A, 0, 20, 0xE9B6C7AAu)
    P(A, B, C, D, 5, 5, 0xD62F105Du)
    P(D, A, B, C, 10, 9, 0x02441453u)
    P(C, D, A, B, 15, 14, 0xD8A1E681u)
    P(B, C, D, A, 4, 20, 0xE7D3FBC8u)
    P(A, B, C, D, 9, 5, 0x21E1CDE6u)
    P(D, A, B, C, 14, 9, 0xC33707D6u)
    P(C, D, A, B, 3, 14, 0xF4D50D87u)
    P(B, C, D, A, 8, 20, 0x455A14EDu)
    P(A, B, C, D, 13, 5, 0xA9E3E905u)
    P(D, A, B, C, 2, 9, 0xFCEFA3F8u)
    P(C, D, A, B, 7, 14, 0x676F02D9u)
    P(B, C, D, A, 12, 20, 0x8D2A4C8Au)

#undef F

#define F(x,y,z) (x ^ y ^ z)

    P(A, B, C, D, 5, 4, 0xFFFA3942u)
    P(D, A, B, C, 8, 11, 0x8771F681u)
    P(C, D, A, B, 11, 16, 0x6D9D6122u)
    P(B, C, D, A, 14, 23, 0xFDE5380Cu)
    P(A, B, C, D, 1, 4, 0xA4BEEA44u)
    P(D, A, B, C, 4, 11, 0x4BDECFA9u)
    P(C, D, A, B, 7, 16, 0xF6BB4B60u)
    P(B, C, D, A, 10, 23, 0xBEBFBC70u)
    P(A, B, C, D, 13, 4, 0x289B7EC6u)
    P(D, A, B, C, 0, 11, 0xEAA127FAu)
    P(C, D, A, B, 3, 16, 0xD4EF3085u)
    P(B, C, D, A, 6, 23, 0x04881D05u)
    P(A, B, C, D, 9, 4, 0xD9D4D039u)
    P(D, A, B, C, 12, 11, 0xE6DB99E5u)
    P(C, D, A, B, 15, 16, 0x1FA27CF8u)
    P(B, C, D, A, 2, 23, 0xC4AC5665u)

#undef F

#define F(x,y,z) (y ^ (x | ~z))

    P(A, B, C, D, 0, 6, 0xF4292244u)
    P(D, A, B, C, 7, 10, 0x432AFF97u)
    P(C, D, A, B, 14, 15, 0xAB9423A7u)
    P(B, C, D, A, 5, 21, 0xFC93A039u)
    P(A, B, C, D, 12, 6, 0x655B59C3u)
    P(D, A, B, C, 3, 10, 0x8F0CCC92u)
    P(C, D, A, B, 10, 15, 0xFFEFF47Du)
    P(B, C, D, A, 1, 21, 0x85845DD1u)
    P(A, B, C, D, 8, 6, 0x6FA87E4Fu)
    P(D, A, B, C, 15, 10, 0xFE2CE6E0u)
    P(C, D, A, B, 6, 15, 0xA3014314u)
    P(B, C, D, A, 13, 21, 0x4E0811A1u)
    P(A, B, C, D, 4, 6, 0xF7537E82u)
    P(D, A, B, C, 11, 10, 0xBD3AF235u)
    P(C, D, A, B, 2, 15, 0x2AD7D2BBu)
    P(B, C, D, A, 9, 21, 0xEB86D391u)

#undef F

    state[0] += A;
    state[1] += B;
    state[2] += C;
    state[3] += D;
}

/*
 * MD5 HMAC context setup
 */
void Md5Context::HmacStarts(uint8 *key,
                            uint32 keylen) {

    (void)MemoryOperationsHelper::Set(ipad, 0x36, 64);
    (void)MemoryOperationsHelper::Set(opad, 0x5C, 64);

    for (uint32 i = 0u; i < keylen; i++) {
        if (i >= 64u) {
            break;
        }

        ipad[i] ^= key[i];
        opad[i] ^= key[i];
    }

    Starts();
    Update(ipad, 64u);
}

/*
 * MD5 HMAC process buffer
 */
void Md5Context::HmacUpdate(uint8 *input,
                            uint32 ilen) {
    Update(input, ilen);
}

/*
 * MD5 HMAC final digest
 */
void Md5Context::HmacFinish(uint8 *output) {
    uint8 tmpbuf[16];

    Finish(tmpbuf);
    Starts();
    Update(opad, 64u);
    Update(tmpbuf, 16u);
    Finish(output);

    (void)MemoryOperationsHelper::Set(tmpbuf, 0, sizeof(tmpbuf));
}

/*
 * Output = MD5( input buffer )
 */
void Md5(uint8 *input,
         uint32 ilen,
         uint8 *output) {
    Md5Context ctx;

    ctx.Starts();
    ctx.Update(input, ilen);
    ctx.Finish(output);

}

/*
 * Output = HMAC-MD5( hmac key, input buffer )
 */
void Md5Hmac(uint8 *key,
             uint32 keylen,
             uint8 *input,
             uint32 ilen,
             uint8 *output) {
    Md5Context ctx;

    ctx.HmacStarts(key, keylen);
    ctx.HmacUpdate(input, ilen);
    ctx.HmacFinish(output);

    (void)MemoryOperationsHelper::Set(&ctx, '\0', sizeof(Md5Context));
}

}
}
