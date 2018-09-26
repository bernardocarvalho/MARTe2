/**
 * @file HttpProtocolTest.h
 * @brief Header file for class HttpProtocolTest
 * @date 17 set 2018
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

 * @details This header file contains the declaration of the class HttpProtocolTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPPROTOCOLTEST_H_
#define HTTPPROTOCOLTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "HttpProtocol.h"
#include "EventSem.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class HttpProtocolTest {
public:
    HttpProtocolTest();

    virtual ~HttpProtocolTest();

    bool TestConstructor();

    bool TestReadHeader_Get1();

    bool TestReadHeader_Get2_Commands();


    bool TestReadHeader_Put1();

    bool TestReadHeader_Post1();

    bool TestReadHeader_Post2_Multiform();


    bool TestReadHeader_Head();

    bool TestReadHeader_Reply();

    bool TestCompleteReadOperation();

    bool TestCompleteReadOperation_ClipSize();

    bool TestWriteHeader();

    bool TestWriteHeader2();


    bool TestWriteHeader_Get1();

    bool TestWriteHeader_Get2();


    bool TestWriteHeader_Put1();

    bool TestWriteHeader_Put2();



    bool TestWriteHeader_Head1();

    bool TestWriteHeader_Head2();


    bool TestWriteHeader_Post();


    bool TestWriteHeader_Reply();

    bool TestWriteHeader_Reply2();


    bool TestWriteHeader_StrucuredDataStored();

    bool TestWriteHeader_StrucuredDataOnline();



    bool TestSecurityCheck();

    bool TestKeepAliveDefault();

    bool TestKeepAlive(bool isKeepAliveIn);

    bool TestSetKeepAlive(bool isKeepAlive);

    bool TestGetHttpCommand(int32 commandIn);

    bool TestSetUnmatchedId();

    bool TestGetUnmatchedId();

    bool TestGetPath();

    bool TestGetId();

    bool TestTextMode(int8 textMode);

    bool TestReadHeader_False_FailGetLine();

    bool TestReadHeader_False_PostNoContentType();

    bool TestReadHeader_False_PostNoContentLength();



    EventSem eventSem;

    bool retVal;

    int32 command;

    bool isKeepAlive;

    int8 textMode;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPPROTOCOLTEST_H_ */

