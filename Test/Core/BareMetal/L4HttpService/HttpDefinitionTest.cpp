/**
 * @file HttpDefinitionTest.cpp
 * @brief Source file for class HttpDefinitionTest
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

 * @details This source file contains the definition of all the methods for
 * the class HttpDefinitionTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpDefinitionTest.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HttpDefinitionTest::HttpDefinitionTest() {
    // Auto-generated constructor stub for HttpDefinitionTest
    // TODO Verify if manual additions are needed
}

HttpDefinitionTest::~HttpDefinitionTest() {
    // Auto-generated destructor stub for HttpDefinitionTest
    // TODO Verify if manual additions are needed
}

bool HttpDefinitionTest::TestHttpEncode() {
    const char8 *input="This is a simple & short test";
    StreamString output;
    HttpDefinition::HttpEncode(output, input);

    printf("\n%s\n%s\n", input, output.Buffer());
    bool ret=(output=="This%20is%20a%20simple%20%26%20short%20test");
    if(ret){
        const char8 *input2=" #%<>&~,$+=:/?";
        StreamString output2;
        HttpDefinition::HttpEncode(output2, input2);
        printf("\n%s\n%s\n", input2, output2.Buffer());
        ret=(output2=="%20%23%25%3C%3E%26%7E%2C%24%2B%3D%3A%2F%3F");
    }


    return ret;
}

bool HttpDefinitionTest::TestHexDecode() {
    char8 c = '0';
    bool ret = (HttpDefinition::HexDecode(c) == 0);
    c = '1';
    ret &= (HttpDefinition::HexDecode(c) == 1);
    c = '2';
    ret &= (HttpDefinition::HexDecode(c) == 2);
    c = '3';
    ret &= (HttpDefinition::HexDecode(c) == 3);
    c = '4';
    ret &= (HttpDefinition::HexDecode(c) == 4);
    c = '5';
    ret &= (HttpDefinition::HexDecode(c) == 5);
    c = '6';
    ret &= (HttpDefinition::HexDecode(c) == 6);
    c = '7';
    ret &= (HttpDefinition::HexDecode(c) == 7);
    c = '8';
    ret &= (HttpDefinition::HexDecode(c) == 8);
    c = '9';
    ret &= (HttpDefinition::HexDecode(c) == 9);
    c = 'a';
    ret &= (HttpDefinition::HexDecode(c) == 10);
    c = 'b';
    ret &= (HttpDefinition::HexDecode(c) == 11);
    c = 'c';
    ret &= (HttpDefinition::HexDecode(c) == 12);
    c = 'd';
    ret &= (HttpDefinition::HexDecode(c) == 13);
    c = 'e';
    ret &= (HttpDefinition::HexDecode(c) == 14);
    c = 'f';
    ret &= (HttpDefinition::HexDecode(c) == 15);

    c = 'A';
    ret &= (HttpDefinition::HexDecode(c) == 10);
    c = 'B';
    ret &= (HttpDefinition::HexDecode(c) == 11);
    c = 'C';
    ret &= (HttpDefinition::HexDecode(c) == 12);
    c = 'D';
    ret &= (HttpDefinition::HexDecode(c) == 13);
    c = 'E';
    ret &= (HttpDefinition::HexDecode(c) == 14);
    c = 'F';
    ret &= (HttpDefinition::HexDecode(c) == 15);

    return ret;
}

bool HttpDefinitionTest::TestHttpDecode() {

    StreamString input="This%20is%20a%20simple%20%26%20short%20test";
    input.Seek(0);
    StreamString output;
    HttpDefinition::HttpDecode(output, input);

    printf("\n%s\n%s\n", input.Buffer(), output.Buffer());
    bool ret=(output=="This is a simple & short test");
    if(ret){
        StreamString input2="%20%23%25%3C%3E%26%7E%2C%24%2B%3D%3A%2F%3F";
        input2.Seek(0);
        StreamString output2;
        HttpDefinition::HttpDecode(output2, input2);
        printf("\n%s\n%s\n", input2.Buffer(), output2.Buffer());
        ret=(output2==" #%<>&~,$+=:/?");
    }


    return ret;

    return true;
}

