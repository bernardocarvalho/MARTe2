/**
 * @file MathExpressionParser.h
 * @brief Header file for class MathExpressionParser
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

 * @details This header file contains the declaration of the class MathExpressionParser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATHEXPRESSIONPARSER_H_
#define MATHEXPRESSIONPARSER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include "Token.h"
#include "StructuredDataI.h"
#include "StreamString.h"
#include "AnyTypeCreator.h"
#include "LexicalAnalyzer.h"
#include "ParserI.h"
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class MathExpressionParser {
public:
    MathExpressionParser(StreamI &stream,
                         StructuredDataI &databaseIn,
                         BufferedStreamI * const err = static_cast<BufferedStreamI*>(NULL));

    virtual ~MathExpressionParser();

    bool Parse();

    GrammarInfo GetGrammarInfo() const;

protected:

    virtual void End();

    virtual void AddOperator();

    virtual void PushOperator();

    virtual void AddQuark();

    virtual void AddVariable();

    virtual uint32 &GetProduction(const uint32 index) const;

    virtual uint32 GetProductionRow(const uint32 index) const;

    virtual uint32 GetParse(const uint32 index) const;

    virtual uint32 GetParseRow(const uint32 index) const;

    virtual uint32 GetConflict(const uint32 index) const;

    virtual uint32 GetConflictRow(const uint32 index) const;

    virtual uint32 GetConstant(const uint32 index) const;

    virtual const char8 *GetSymbolName(const uint32 symbol) const;

    virtual uint32 GetNextTokenType();

    virtual uint32 PeekNextTokenType(const uint32 position);

    inline void StackPush(const uint32 symbol,
                          const uint32 * const stack,
                          uint32* &top) const;

    inline uint32 StackPop(uint32* &top) const;

    virtual void Execute(const uint32 number);

    Token *currentToken;

private:

    StreamString typeName;

    StreamString nodeName;

    StructuredDataI *database;

    bool isError;

    LexicalAnalyzer tokenProducer;

    uint32 numberOfColumns;

    uint32 firstNumberOfColumns;

    uint32 numberOfRows;

    AnyTypeCreator memory;

    BufferedStreamI *errorStream;

    uint32 tokenType;

    uint8 numberOfDimensions;

    GrammarInfo grammar;

    void (MathExpressionParser::*Action[6])(void);

    char8 operatorStack[512];

    uint32 stackTop;
};



}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


namespace MARTe {

void MathExpressionParser::StackPush(const uint32 symbol,
                        const uint32 * const stack,
                        uint32 *&top) const {
    /*lint -e{946} [MISRA C++ Rule 5-0-15], [MISRA C++ Rule 5-0-17]. Justification: stack implementation requires operational applied to pointer. */
    if (top > stack) {
        /*lint -e{165} , [MISRA C++ Rule 5-0-18]. Justification: stack implementation requires subtraction applied to pointer. */
        top--;
        *top = symbol;
    }
}

uint32 MathExpressionParser::StackPop(uint32 * &top) const {
    uint32 ret = 0u;
    if (*top != 0u) {
        ret = *top;
        top++;
    }
    return ret;
}

}
#endif /* MATHEXPRESSIONPARSER_H_ */

