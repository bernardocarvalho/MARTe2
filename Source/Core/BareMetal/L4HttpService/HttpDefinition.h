/**
 * @file HttpDefinition.h
 * @brief Header file for class HttpDefinition
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

 * @details This header file contains the declaration of the class HttpDefinition
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPDEFINITION_H_
#define HTTPDEFINITION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BufferedStreamI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
namespace HttpDefinition {
/** read while available (no content length specified)*/
static const int32 HTTPNoContentLengthSpecified = -2;

/** the status of HttpStream */
//HSOperatingMode
/** operates on the string both read and write */
static const int32 HSOMWriteToString = 0x0;

/** final mode: only write, and use the client direct stream */
static const int32 HSOMWriteToClient = 0x1;

/** operates read and write with CDB  */
static const int32 HSOMWriteToCDB = 0x2;

/** no more reading or writing   */
static const int32 HSOMCompleted = 0x4;

/** the command requested via HTTP */
//HSHttpCommand
/** none */
static const int32 HSHCNone = 0;

/** HTTP GET */
static const int32 HSHCGet = 1;

/** HTTP PUT */
static const int32 HSHCPut = 2;

/** HTTP POST */
static const int32 HSHCPost = 3;

/** HTTP HEAD */
static const int32 HSHCHead = 4;

/** HTTP REPLY */
static const int32 HSHCReply = 0x10000000;

/** HTTP REPLY OK*/
static const int32 HSHCReplyOK = (HSHCReply + 200);

/** HTTP REPLY AUTH REQUIURED*/
static const int32 HSHCReplyAUTH = (HSHCReply + 401);


// UrlProtocols
/** NONE  */
static const int32 URLP_NONE = 0;

/** HTTP  */
static const int32 URLP_HTTP = 1;

/** FTP */
static const int32 URLP_FTP = 2;

/** FILE */
static const int32 URLP_FILE = 3;


/** create a HSHttpCommand relative toa reply with a specfic ErrorCode */
static inline int32 GenerateReplyCode(const int32 httpErrorCode) {
    return (httpErrorCode + HSHCReply);
}

/** create a HSHttpCommand relative toa reply with a specfic ErrorCode */
static inline bool IsReplyCode(const int32 command,
                               int32 &httpErrorCode) {

    bool ret = (command >= HSHCReply);
    if (ret) {
        httpErrorCode = command - HSHCReply;
    }
    return ret;
}

/**
 * @return the HTTP string corresponding to the error code
 */
static inline const char8 *GetErrorCodeString(const int32 httpErrorCode) {
    const char8 *ret = "Unknown code";

    switch (httpErrorCode) {
    case 200: {
        ret = "OK";
    }
        break;
    case 201: {
        ret = "CREATED";
    }
        break;
    case 202: {
        ret = "Accepted";
    }
        break;
    case 203: {
        ret = "Partial Information";
    }
        break;
    case 204: {
        ret = "No Response";
    }
        break;
    case 400: {
        ret = "Bad request";
    }
        break;
    case 401: {
        ret = "Unauthorized";
    }
        break;
    case 402: {
        ret = "PaymentRequired";
    }
        break;
    case 403: {
        ret = "Forbidden";
    }
        break;
    case 404: {
        ret = "Not found";
    }
        break;
    case 500: {
        ret = "Internal Error";
    }
        break;
    case 501: {
        ret = "Not implemented";
    }
        break;
    case 301: {
        ret = "Moved";
    }
        break;
    case 302: {
        ret = "Found";
    }
        break;
    case 303: {
        ret = "Method";
    }
        break;
    default: {

    }
    }

    return ret;
}



bool HttpEncode(BufferedStreamI &converted,
                const char8 * const original);

int32 HexDecode(const char8 c);

bool HttpDecode(BufferedStreamI &destination,
                BufferedStreamI &source);

}
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPDEFINITION_H_ */

