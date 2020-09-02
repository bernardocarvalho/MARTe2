/**
 * @file MathExpressionParserTest.cpp
 * @brief Source file for class MathExpressionParserTest
 * @date Apr 7, 2020
 * @author Giuseppe
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
 * the class MathExpressionParserTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MathExpressionParserTest.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MathExpressionParserTest::MathExpressionParserTest() {
    // Auto-generated constructor stub for MathExpressionParserTest
    // TODO Verify if manual additions are needed
}

MathExpressionParserTest::~MathExpressionParserTest() {
    // Auto-generated destructor stub for MathExpressionParserTest
    // TODO Verify if manual additions are needed
}

bool MathExpressionParserTest::Test1(){
    //StreamString config = "((A*B))";
    //StreamString config = "((A*B)+(C*D))";
    StreamString config = "((A*B)+(C*D))-((E-F)*(I+L-M+G*H))";
    ConfigurationDatabase cdb;
    config.Seek(0ull);
    MathExpressionParser parser(config, cdb);
    parser.Parse();
    /**
[Information - MathExpressionParser.cpp:193]: Add Quark A B *
[Information - MathExpressionParser.cpp:193]: Add Quark C D *
[Information - MathExpressionParser.cpp:183]: Add Operator +
[Information - MathExpressionParser.cpp:197]: Add Variable E
[Information - MathExpressionParser.cpp:197]: Add Variable F
[Information - MathExpressionParser.cpp:183]: Add Operator -
[Information - MathExpressionParser.cpp:197]: Add Variable I
[Information - MathExpressionParser.cpp:197]: Add Variable L
[Information - MathExpressionParser.cpp:183]: Add Operator +
[Information - MathExpressionParser.cpp:197]: Add Variable M
[Information - MathExpressionParser.cpp:183]: Add Operator -
[Information - MathExpressionParser.cpp:193]: Add Quark G H *
[Information - MathExpressionParser.cpp:183]: Add Operator +
[Information - MathExpressionParser.cpp:183]: Add Operator *
[Information - MathExpressionParser.cpp:183]: Add Operator -

     */


    return true;
}
