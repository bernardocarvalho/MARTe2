/**
 * @file XMLPrinter.cpp
 * @brief Source file for class XMLPrinter
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

 * @details This source file contains the definition of all the methods for
 * the class XMLPrinter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "XMLPrinter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

XMLPrinter::XMLPrinter(BufferedStreamI & streamIn): PrinterI(streamIn) {
    // Auto-generated constructor stub for XMLPrinter
    // TODO Verify if manual additions are needed
}


XMLPrinter::XMLPrinter(): PrinterI(){

}

XMLPrinter::~XMLPrinter() {
    // Auto-generated destructor stub for XMLPrinter
    // TODO Verify if manual additions are needed
}


bool XMLPrinter::PrintOpenMatrix(){
    return stream->Printf("%s", "[");
}

bool XMLPrinter::PrintCloseMatrix(){
    return stream->Printf("%s", "]");
}

bool XMLPrinter::PrintScalarSeparator(){
    return stream->Printf("%s", ",");
}

bool XMLPrinter::PrintVectorSeparator(){
    return stream->Printf("%s", ",");
}

bool XMLPrinter::PrintVariableSeparator(){
    return true;
}

bool XMLPrinter::PrintBlockSeparator(){
    return true;
}

bool XMLPrinter::PrintOpenVector(){
    return stream->Printf("%s", "[");
}

bool XMLPrinter::PrintCloseVector(){
    return stream->Printf("%s", "]");
}

bool XMLPrinter::PrintOpenBlock(const char8 *blockName){
    return stream->Printf("<%s>", blockName);
}

bool XMLPrinter::PrintCloseBlock(const char8 *blockName){
    return stream->Printf("</%s>",blockName);
}

bool XMLPrinter::PrintOpenAssignment(const char8 *varName){
    return stream->Printf("<%s>", varName);
}

bool XMLPrinter::PrintCloseAssignment(const char8 *varName){
    return stream->Printf("</%s>", varName);
}

bool XMLPrinter::PrintVariable(AnyType var){
    return stream->Printf("%#X!", var);
}
}
