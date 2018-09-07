/**
 * @file StandardPrinter.h
 * @brief Header file for class StandardPrinter
 * @date 07 set 2018
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

 * @details This header file contains the declaration of the class StandardPrinter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STANDARDPRINTER_H_
#define STANDARDPRINTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "PrinterI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class StandardPrinter: public PrinterI {
public:
    StandardPrinter(BufferedStreamI & streamIn);

    virtual ~StandardPrinter();

    virtual bool PrintOpenMatrix();

    virtual bool PrintCloseMatrix();

    virtual bool PrintScalarSeparator();

    virtual bool PrintVectorSeparator();

    virtual bool PrintVariableSeparator();

    virtual bool PrintBlockSeparator();

    virtual bool PrintOpenVector();

    virtual bool PrintCloseVector();

    virtual bool PrintOpenBlock(const char8 *blockName);

    virtual bool PrintCloseBlock(const char8 *blockName);

    virtual bool PrintOpenAssignment(const char8 *varName);

    virtual bool PrintCloseAssignment(const char8 *varName);

    virtual bool PrintVariable(AnyType var);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STANDARDPRINTER_H_ */

