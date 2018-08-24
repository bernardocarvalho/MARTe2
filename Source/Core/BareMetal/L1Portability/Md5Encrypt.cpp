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
    (n) = ( (osulong) (b)[(i)    ]       )        \
        | ( (osulong) (b)[(i) + 1] <<  8 )        \
        | ( (osulong) (b)[(i) + 2] << 16 )        \
        | ( (osulong) (b)[(i) + 3] << 24 );       \
}
#endif

#ifndef PUT_UINT32_LE
#define PUT_UINT32_LE(n,b,i)                            \
{                                                       \
    (b)[(i)    ] = (uint8) ( (n)       );       \
    (b)[(i) + 1] = (uint8) ( (n) >>  8 );       \
    (b)[(i) + 2] = (uint8) ( (n) >> 16 );       \
    (b)[(i) + 3] = (uint8) ( (n) >> 24 );       \
}
#endif

/*
 * MD5 context setup
 */
void Starts(Md5Context *ctx) {
    ctx->total[0] = 0;
    ctx->total[1] = 0;

    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xEFCDAB89;
    ctx->state[2] = 0x98BADCFE;
    ctx->state[3] = 0x10325476;
}

static void md5_process(Md5Context *ctx,
                        uint8 data[64]) {
    osulong X[16], A, B, C, D;

    GET_UINT32_LE(X[0], data, 0);
    GET_UINT32_LE(X[1], data, 4);
    GET_UINT32_LE(X[2], data, 8);
    GET_UINT32_LE(X[3], data, 12);
    GET_UINT32_LE(X[4], data, 16);
    GET_UINT32_LE(X[5], data, 20);
    GET_UINT32_LE(X[6], data, 24);
    GET_UINT32_LE(X[7], data, 28);
    GET_UINT32_LE(X[8], data, 32);
    GET_UINT32_LE(X[9], data, 36);
    GET_UINT32_LE(X[10], data, 40);
    GET_UINT32_LE(X[11], data, 44);
    GET_UINT32_LE(X[12], data, 48);
    GET_UINT32_LE(X[13], data, 52);
    GET_UINT32_LE(X[14], data, 56);
    GET_UINT32_LE(X[15], data, 60);

#define S(x,n) ((x << n) | ((x & 0xFFFFFFFF) >> (32 - n)))

#define P(a,b,c,d,k,s,t)                                \
{                                                       \
    a += F(b,c,d) + X[k] + t; a = S(a,s) + b;           \
}

    A = ctx->state[0];
    B = ctx->state[1];
    C = ctx->state[2];
    D = ctx->state[3];

#define F(x,y,z) (z ^ (x & (y ^ z)))

    P(A, B, C, D, 0, 7, 0xD76AA478);
    P(D, A, B, C, 1, 12, 0xE8C7B756);
    P(C, D, A, B, 2, 17, 0x242070DB);
    P(B, C, D, A, 3, 22, 0xC1BDCEEE);
    P(A, B, C, D, 4, 7, 0xF57C0FAF);
    P(D, A, B, C, 5, 12, 0x4787C62A);
    P(C, D, A, B, 6, 17, 0xA8304613);
    P(B, C, D, A, 7, 22, 0xFD469501);
    P(A, B, C, D, 8, 7, 0x698098D8);
    P(D, A, B, C, 9, 12, 0x8B44F7AF);
    P(C, D, A, B, 10, 17, 0xFFFF5BB1);
    P(B, C, D, A, 11, 22, 0x895CD7BE);
    P(A, B, C, D, 12, 7, 0x6B901122);
    P(D, A, B, C, 13, 12, 0xFD987193);
    P(C, D, A, B, 14, 17, 0xA679438E);
    P(B, C, D, A, 15, 22, 0x49B40821);

#undef F

#define F(x,y,z) (y ^ (z & (x ^ y)))

    P(A, B, C, D, 1, 5, 0xF61E2562);
    P(D, A, B, C, 6, 9, 0xC040B340);
    P(C, D, A, B, 11, 14, 0x265E5A51);
    P(B, C, D, A, 0, 20, 0xE9B6C7AA);
    P(A, B, C, D, 5, 5, 0xD62F105D);
    P(D, A, B, C, 10, 9, 0x02441453);
    P(C, D, A, B, 15, 14, 0xD8A1E681);
    P(B, C, D, A, 4, 20, 0xE7D3FBC8);
    P(A, B, C, D, 9, 5, 0x21E1CDE6);
    P(D, A, B, C, 14, 9, 0xC33707D6);
    P(C, D, A, B, 3, 14, 0xF4D50D87);
    P(B, C, D, A, 8, 20, 0x455A14ED);
    P(A, B, C, D, 13, 5, 0xA9E3E905);
    P(D, A, B, C, 2, 9, 0xFCEFA3F8);
    P(C, D, A, B, 7, 14, 0x676F02D9);
    P(B, C, D, A, 12, 20, 0x8D2A4C8A);

#undef F

#define F(x,y,z) (x ^ y ^ z)

    P(A, B, C, D, 5, 4, 0xFFFA3942);
    P(D, A, B, C, 8, 11, 0x8771F681);
    P(C, D, A, B, 11, 16, 0x6D9D6122);
    P(B, C, D, A, 14, 23, 0xFDE5380C);
    P(A, B, C, D, 1, 4, 0xA4BEEA44);
    P(D, A, B, C, 4, 11, 0x4BDECFA9);
    P(C, D, A, B, 7, 16, 0xF6BB4B60);
    P(B, C, D, A, 10, 23, 0xBEBFBC70);
    P(A, B, C, D, 13, 4, 0x289B7EC6);
    P(D, A, B, C, 0, 11, 0xEAA127FA);
    P(C, D, A, B, 3, 16, 0xD4EF3085);
    P(B, C, D, A, 6, 23, 0x04881D05);
    P(A, B, C, D, 9, 4, 0xD9D4D039);
    P(D, A, B, C, 12, 11, 0xE6DB99E5);
    P(C, D, A, B, 15, 16, 0x1FA27CF8);
    P(B, C, D, A, 2, 23, 0xC4AC5665);

#undef F

#define F(x,y,z) (y ^ (x | ~z))

    P(A, B, C, D, 0, 6, 0xF4292244);
    P(D, A, B, C, 7, 10, 0x432AFF97);
    P(C, D, A, B, 14, 15, 0xAB9423A7);
    P(B, C, D, A, 5, 21, 0xFC93A039);
    P(A, B, C, D, 12, 6, 0x655B59C3);
    P(D, A, B, C, 3, 10, 0x8F0CCC92);
    P(C, D, A, B, 10, 15, 0xFFEFF47D);
    P(B, C, D, A, 1, 21, 0x85845DD1);
    P(A, B, C, D, 8, 6, 0x6FA87E4F);
    P(D, A, B, C, 15, 10, 0xFE2CE6E0);
    P(C, D, A, B, 6, 15, 0xA3014314);
    P(B, C, D, A, 13, 21, 0x4E0811A1);
    P(A, B, C, D, 4, 6, 0xF7537E82);
    P(D, A, B, C, 11, 10, 0xBD3AF235);
    P(C, D, A, B, 2, 15, 0x2AD7D2BB);
    P(B, C, D, A, 9, 21, 0xEB86D391);

#undef F

    ctx->state[0] += A;
    ctx->state[1] += B;
    ctx->state[2] += C;
    ctx->state[3] += D;
}

/*
 * MD5 process buffer
 */
void Update(Md5Context *ctx,
            uint8 *input,
            int32 ilen) {
    int32 fill;
    osulong left;

    if (ilen > 0) {

        left = ctx->total[0] & 0x3F;
        fill = 64 - left;

        ctx->total[0] += ilen;
        ctx->total[0] &= 0xFFFFFFFF;

        if (ctx->total[0] < (osulong) ilen)
            ctx->total[1]++;

        if (left && ilen >= fill) {
            MemoryOperationsHelper::Copy((void *) (ctx->buffer + left), (void *) input, fill);
            md5_process(ctx, ctx->buffer);
            input += fill;
            ilen -= fill;
            left = 0;
        }

        while (ilen >= 64) {
            md5_process(ctx, input);
            input += 64;
            ilen -= 64;
        }

        if (ilen > 0) {
            MemoryOperationsHelper::Copy((void *) (ctx->buffer + left), (void *) input, ilen);
        }
    }
}

static const uint8 md5_padding[64] = { 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/*
 * MD5 final digest
 */
void Finish(Md5Context *ctx,
            uint8 *output) {
    osulong last, padn;
    osulong high, low;
    uint8 msglen[8];

    high = (ctx->total[0] >> 29) | (ctx->total[1] << 3);
    low = (ctx->total[0] << 3);

    PUT_UINT32_LE(low, msglen, 0);
    PUT_UINT32_LE(high, msglen, 4);

    last = ctx->total[0] & 0x3F;
    padn = (last < 56) ? (56 - last) : (120 - last);

    Update(ctx, (uint8 *) md5_padding, padn);
    Update(ctx, msglen, 8);

    PUT_UINT32_LE(ctx->state[0], output, 0);
    PUT_UINT32_LE(ctx->state[1], output, 4);
    PUT_UINT32_LE(ctx->state[2], output, 8);
    PUT_UINT32_LE(ctx->state[3], output, 12);
}

/*
 * Output = MD5( input buffer )
 */
void Md5(uint8 *input,
         int32 ilen,
         uint8 *output) {
    Md5Context ctx;

    Starts(&ctx);
    Update(&ctx, input, ilen);
    Finish(&ctx, output);

    MemoryOperationsHelper::Set(&ctx, '\0', sizeof(Md5Context));
}

/*
 * MD5 HMAC context setup
 */
void HmacStarts(Md5Context *ctx,
                uint8 *key,
                int32 keylen) {
    int32 i;

    MemoryOperationsHelper::Set(ctx->ipad, 0x36, 64);
    MemoryOperationsHelper::Set(ctx->opad, 0x5C, 64);

    for (i = 0; i < keylen; i++) {
        if (i >= 64) {
            break;
        }

        ctx->ipad[i] ^= key[i];
        ctx->opad[i] ^= key[i];
    }

    Starts(ctx);
    Update(ctx, ctx->ipad, 64);
}

/*
 * MD5 HMAC process buffer
 */
void HmacUpdate(Md5Context *ctx,
                uint8 *input,
                int32 ilen) {
    Update(ctx, input, ilen);
}

/*
 * MD5 HMAC final digest
 */
void HmacFinish(Md5Context *ctx,
                uint8 *output) {
    uint8 tmpbuf[16];

    Finish(ctx, tmpbuf);
    Starts(ctx);
    Update(ctx, ctx->opad, 64);
    Update(ctx, tmpbuf, 16);
    Finish(ctx, output);

    MemoryOperationsHelper::Set(tmpbuf, 0, sizeof(tmpbuf));
}

/*
 * Output = HMAC-MD5( hmac key, input buffer )
 */
void Md5Hmac(uint8 *key,
             int32 keylen,
             uint8 *input,
             int32 ilen,
             uint8 *output) {
    Md5Context ctx;

    HmacStarts(&ctx, key, keylen);
    HmacUpdate(&ctx, input, ilen);
    HmacFinish(&ctx, output);

    MemoryOperationsHelper::Set(&ctx, 0, sizeof(Md5Context));
}

}
}
