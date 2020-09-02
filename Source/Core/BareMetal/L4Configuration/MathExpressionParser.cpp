/**
 * @file MathExpressionParser.cpp
 * @brief Source file for class MathExpressionParser
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
 * the class MathExpressionParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MathExpressionParser.h"
#include "TypeConversion.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
static uint32 Production[] = {0,3,10,11,19 ,3,11,12,16 ,6,12,22,15,14,15,17 ,3,12,23,15,5,12,1,11,2,18 ,2,13,3 ,2,13,4 ,2,14,5 ,2,14,6,2,15,7 ,2,15,8 ,6,16,20,13,12,21,16 ,1,16 ,4,17,14,15,17,1,17 ,8,18,20,14,1,11,2,21,18 ,1,18,0};

static uint32 Production_row[] = {0,1,5,9,16,20,26,29,32,35,38,41,44,51,53,58,60,69,0};

static uint32 ParseArray[] = {0,0,1,15,15,15,14,14,1,1,15,17,17,17,16,16,2,0,17,13,12,12,2,2,5,0,13,6,7,0,19,18,8,9,10,11,0};

static uint32 Parse_row[] = {0,1,15,23,24,27,27,17,1,9,0};

static uint32 Conflict[] = {0,0,0,4,4,4,3,3};

static uint32 Conflict_row[] = {0,1,1,0};

static const uint32 Constants[] = { 10, 9, 0, 18, 20, 19, 24, 2 };

static char * Terminal_name[] = { "0", "(", ")", "+", "-", "*", "/", "STRING", "NUMBER", "END_OF_SLK_INPUT" };

static const char8 *GetTerminalName(const uint32 symbol) {
    return Terminal_name[symbol];
}

static void PrintErrorOnStream(const char8 * const format,
                               const uint32 lineNumber,
                               BufferedStreamI * const err) {
    if (err != NULL) {
        if (!err->Printf(format, lineNumber)) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on parseError stream");
        }
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, format, lineNumber);
    }
}

static const char8* GetCurrentTokenData(Token * const token) {

    return (token != NULL)?(token->GetData()):(static_cast<const char8*>(NULL));
}

static uint32 GetCurrentTokenId(const Token * const token) {
    return (token != NULL)?(token->GetId()):(ERROR_TOKEN);
}

static uint32 GetCurrentTokenLineNumber(const Token * const token) {
    return (token != NULL)?(token->GetLineNumber()):0u;
}

}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MathExpressionParser::MathExpressionParser(StreamI &stream,
                                           StructuredDataI &databaseIn,
                                           BufferedStreamI * const err) :
    tokenProducer(stream, &(MathGrammar.assignment), MathGrammar.separators, MathGrammar.beginOneLineComment, MathGrammar.beginMultipleLinesComment, MathGrammar.endMultipleLinesComment),
    memory(1u) {
        numberOfColumns = 0u;
        firstNumberOfColumns = 0u;
        numberOfRows = 0u;
        database = &databaseIn;
        errorStream = err;
        tokenType = 0u;
        numberOfDimensions = 0u;
        grammar = MathGrammar;
        currentToken = static_cast<Token*>(NULL);
        isError = false;
        operatorStack[0]=0;
        stackTop=0u;

        Action[0] = static_cast<void (MathExpressionParser::*)(void)>(NULL);
        Action[1] = &MathExpressionParser::End;
        Action[2] = &MathExpressionParser::PushOperator;
        Action[3] = &MathExpressionParser::AddOperator;
        Action[4] = &MathExpressionParser::AddQuark;
        Action[5] = &MathExpressionParser::AddVariable;

}

MathExpressionParser::~MathExpressionParser() {
// Auto-generated destructor stub for MathExpressionParser
// TODO Verify if manual additions are needed
}

uint32 MathExpressionParser::GetNextTokenType() {
uint32 ret = 0u;

currentToken = tokenProducer.GetToken();

uint32 endTokendId = GetConstant(ParserConstant::START_SYMBOL); //StringHelper::Length(terminals)+2u;
const char8* toCompare = static_cast<const char8 *>(NULL);

// if it is a terminal use the data
if (currentToken->GetId() == TERMINAL_TOKEN) {
    toCompare = currentToken->GetData();
}
// otherwise use the description
else {
    toCompare = currentToken->GetDescription();
}
// return the slk token number
for (uint32 i = 0u; i < endTokendId; i++) {
    if (StringHelper::Compare(toCompare, GetSymbolName(i)) == 0) {
        ret = i;
    }
}
return ret;
}

uint32 MathExpressionParser::PeekNextTokenType(const uint32 position) {
uint32 ret = 0u;

Token* tok = tokenProducer.PeekToken(position);
uint32 endTokendId = GetConstant(ParserConstant::START_SYMBOL);
const char8* toCompare = static_cast<const char8 *>(NULL);

if (tok->GetId() == TERMINAL_TOKEN) {
    toCompare = tok->GetData();
}
else {
    toCompare = tok->GetDescription();
}
for (uint32 i = 0u; i < endTokendId; i++) {
    if (StringHelper::Compare(toCompare, GetSymbolName(i)) == 0) {
        ret = i;
    }
}

return ret;
}

void MathExpressionParser::End() {
REPORT_ERROR_STATIC(ErrorManagement::Information, "END");
}

void MathExpressionParser::PushOperator() {
stackTop++;
operatorStack[stackTop] = *(currentToken->GetData());
}

void MathExpressionParser::AddOperator() {
StreamString operat;
operat=(operatorStack[stackTop]);
REPORT_ERROR_STATIC(ErrorManagement::Information, "Add Operator %s", operat.Buffer());
operatorStack[stackTop] = 0;
if (stackTop > 0u) {
    stackTop--;
}
}

void MathExpressionParser::AddQuark() {
Token* operation = tokenProducer.PeekToken(0);
Token* op2 = tokenProducer.PeekToken(1);
REPORT_ERROR_STATIC(ErrorManagement::Information, "Add Quark %s %s %s", currentToken->GetData(), op2->GetData(), operation->GetData());
}

void MathExpressionParser::AddVariable(){
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Add Variable %s", currentToken->GetData());
}


void MathExpressionParser::Execute(const uint32 number) {
(this->*Action[number])();
}

const char8 *MathExpressionParser::GetSymbolName(const uint32 symbol) const {
const char8 *symbolName = static_cast<const char8 *>(NULL);

if((symbol > 0u) && (symbol < Constants[ParserConstant::START_SYMBOL])) {
    symbolName=GetTerminalName(symbol);
}
else {
    symbolName="not a symbol";
}
return symbolName;
}

uint32 &MathExpressionParser::GetProduction(const uint32 index) const {
return Production[index];
}

uint32 MathExpressionParser::GetProductionRow(const uint32 index) const {
return Production_row[index];
}

uint32 MathExpressionParser::GetParse(const uint32 index) const {
return ParseArray[index];
}

uint32 MathExpressionParser::GetParseRow(const uint32 index) const {
return Parse_row[index];
}

uint32 MathExpressionParser::GetConflict(const uint32 index) const {
return Conflict[index];
}

uint32 MathExpressionParser::GetConflictRow(const uint32 index) const {
return Conflict_row[index];
}

uint32 MathExpressionParser::GetConstant(const uint32 index) const {
return Constants[index];
}

bool MathExpressionParser::Parse() {
typeName = defaultTypeName;

bool isEOF = false;

while ((!isError) && (!isEOF)) {

    uint32 stackArray[ParserConstant::PARSE_STACK_SIZE];
    uint32 *stack = &stackArray[0];

    uint32 *top = &stackArray[ParserConstant::PARSE_STACK_SIZE - 1u];
    *top = 0u;
    uint32 start_symbol = GetConstant(ParserConstant::START_SYMBOL);

    StackPush(start_symbol, stack, top);
    uint32 token = GetNextTokenType();
    uint32 new_token = token;

    for (uint32 symbol = StackPop(top); (symbol > 0u) && (!isError);) {
        if (symbol >= GetConstant(ParserConstant::START_ACTION)) {
            Execute(symbol - (GetConstant(ParserConstant::START_ACTION) - 1u));

        }
        else if (symbol >= GetConstant(ParserConstant::START_SYMBOL)) {
            uint32 level = 0u; // before was 1

            uint32 index = GetParseRow(symbol - (GetConstant(ParserConstant::START_SYMBOL) - 1u));
            index += token;
            uint32 entry = GetParse(index);
            while (entry >= GetConstant(ParserConstant::START_CONFLICT)) {
                index = GetConflictRow(entry - (GetConstant(ParserConstant::START_CONFLICT) - 1u));
                index += PeekNextTokenType(level);
                entry = GetConflict(index);
                ++level;
            }
            if (entry > 0u) {
                uint32 *production = &GetProduction(GetProductionRow(entry));
                uint32 production_length = *production - 1u;
                production = &production[1];
                /*lint -e{415} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                if (*production == symbol) {
                    /*lint -e{661} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                    for (; production_length > 0u; production_length--) {
                        /*lint -e{662} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                        uint32 toPush = production[production_length];
                        StackPush(toPush, stack, top);
                    }
                }
                else {
                    (token == 0u) ? (isEOF = true) : (isError = true);
                    if (isError) {
                        PrintErrorOnStream("\nInvalid Token! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
                    }
                    new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
                }
            }
            else {
                (token == 0u) ? (isEOF = true) : (isError = true);
                if (isError) {
                    PrintErrorOnStream("\nInvalid Token! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
                }
                new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
            }
        }
        else {
            if (symbol > 0u) {
                if (symbol == token) {
                    token = GetNextTokenType();
                    new_token = token;
                }
                else {
                    isError = true;
                    PrintErrorOnStream("\nInvalid Expression! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
                    new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
                }
            }
        }
        if (token != new_token) {
            if (new_token > 0u) {
                token = new_token;
            }
            if (token != GetConstant(ParserConstant::END_OF_SLK_INPUT)) {
                continue;
            }
        }
        symbol = StackPop(top);
    }
    if (token != GetConstant(ParserConstant::END_OF_SLK_INPUT)) {
        PrintErrorOnStream("\nEOF found with tokens on internal parser stack! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

return !isError;
}

}
