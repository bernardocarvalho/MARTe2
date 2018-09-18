/**
 * @file PrinterI.h
 * @brief Header file for class PrinterI
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

 * @details This header file contains the declaration of the class PrinterI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PRINTERI_H_
#define PRINTERI_H_

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

namespace MARTe{

class PrinterI {
public:
    PrinterI();

    PrinterI(BufferedStreamI & streamIn);

    virtual ~PrinterI();

    virtual void SetStream(BufferedStreamI & streamIn);

    virtual bool PrintOpenMatrix()=0;

    virtual bool PrintCloseMatrix()=0;

    virtual bool PrintScalarSeparator()=0;

    virtual bool PrintVectorSeparator()=0;

    virtual bool PrintVariableSeparator()=0;

    virtual bool PrintBlockSeparator()=0;

    virtual bool PrintOpenVector()=0;

    virtual bool PrintCloseVector()=0;

    virtual bool PrintOpenBlock(const char8 * const blockName)=0;

    virtual bool PrintCloseBlock(const char8 *const blockName)=0;

    virtual bool PrintOpenAssignment(const char8 *const varName)=0;

    virtual bool PrintCloseAssignment(const char8 *const varName)=0;

    virtual bool PrintVariable(const AnyType &var)=0;


    BufferedStreamI *stream;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PRINTERI_H_ */

