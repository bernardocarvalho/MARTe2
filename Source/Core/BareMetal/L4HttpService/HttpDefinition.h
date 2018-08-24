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
enum HSReadMode {
    HTTPNoContentLengthSpecified = -2
};

/** the status of HttpStream */
enum HSOperatingMode {
    /** operates on the string both read and write */
    HSOMWriteToString = 0x0,

    /** final mode: only write, and use the client direct stream */
    HSOMWriteToClient = 0x1,

    /** operates read and write with CDB  */
    HSOMWriteToCDB = 0x2,

    /** no more reading or writing   */
    HSOMCompleted = 0x4
};

/** the command requested via HTTP */
enum HSHttpCommand {
    /** none */
    HSHCNone = 0,

    /** HTTP GET */
    HSHCGet = 1,

    /** HTTP PUT */
    HSHCPut = 2,

    /** HTTP POST */
    HSHCPost = 3,

    /** HTTP HEAD */
    HSHCHead = 4,

    /** HTTP REPLY */
    HSHCReply = 0x10000000,

    /** HTTP REPLY OK*/
    HSHCReplyOK = HSHCReply + 200,

    /** HTTP REPLY AUTH REQUIURED*/
    HSHCReplyAUTH = HSHCReply + 401

};

/** create a HSHttpCommand relative toa reply with a specfic ErrorCode */
static inline HSHttpCommand GenerateReplyCode(uint32 httpErrorCode) {
    return static_cast<HSHttpCommand>(httpErrorCode + static_cast<uint32>(HSHCReply));
}

/** create a HSHttpCommand relative toa reply with a specfic ErrorCode */
static inline bool IsReplyCode(HSHttpCommand command,
                               uint32 &httpErrorCode) {

    bool ret=(command >= HSHCReply);
    if (ret) {
        httpErrorCode = static_cast<uint32>(command) - static_cast<uint32>(HSHCReply);
    }
    return ret;
}

/**
 * @return the HTTP string corresponding to the error code
 */
static inline const char8 *GetErrorCodeString(uint32 httpErrorCode) {
    const char8 *ret = "Unknown code";

    switch (httpErrorCode) {
    case 200u: {
        ret = "OK";
    }
        break;
    case 201u: {
        ret = "CREATED";
    }
        break;
    case 202u: {
        ret = "Accepted";
    }
        break;
    case 203u: {
        ret = "Partial Information";
    }
        break;
    case 204u: {
        ret = "No Response";
    }
        break;
    case 400u: {
        ret = "Bad request";
    }
        break;
    case 401u: {
        ret = "Unauthorized";
    }
        break;
    case 402u: {
        ret = "PaymentRequired";
    }
        break;
    case 403u: {
        ret = "Forbidden";
    }
        break;
    case 404u: {
        ret = "Not found";
    }
        break;
    case 500u: {
        ret = "Internal Error";
    }
        break;
    case 501u: {
        ret = "Not implemented";
    }
        break;
    case 301u: {
        ret = "Moved";
    }
        break;
    case 302u: {
        ret = "Found";
    }
        break;
    case 303u: {
        ret = "Method";
    }
    }

    return ret;
}


enum UrlProtocols {
    /** NONE  */
    URLP_NONE = 0,

    /** HTTP  */
    URLP_HTTP = 1,

    /** FTP */
    URLP_FTP = 2,

    /** FILE */
    URLP_FILE = 3
};




bool HttpEncode(BufferedStreamI &converted, const char8 *original);

int32 HexDecode(char8 c);

bool HttpDecode(BufferedStreamI &destination, BufferedStreamI &source);

}
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPDEFINITION_H_ */

