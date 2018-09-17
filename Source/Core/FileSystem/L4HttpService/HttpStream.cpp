/**
 * @file HttpStream.cpp
 * @brief Source file for class HttpStream
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

 * @details This source file contains the definition of all the methods for
 * the class HttpStream (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpStream.h"
#include "HttpDefinition.h"
#include "AdvancedErrorManagement.h"
#include "BasicSocket.h"
#include "DoubleBufferedStream.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
using namespace HttpDefinition;

//#define NULL_PTR(x) NULL

HttpStream::HttpStream(DoubleBufferedStream &clientBufferedStreamIn,
                       bool store) :
        StreamString(),
        StreamStructuredData<JsonPrinter>() {
    outputStream = &clientBufferedStreamIn;
    storeBody = store;
    StreamStructuredData<JsonPrinter>::SetStream(*outputStream);

}

HttpStream::~HttpStream() {
    // Auto-generated destructor stub for HttpStream
    // TODO Verify if manual additions are needed
}

bool HttpStream::Read(const char8 * const name,
                      const AnyType &value) {
    //use a special node
    return (storeBody) ? (storedData.Read(name, value)) : (StreamStructuredData<JsonPrinter>::Read(name, value));
}

AnyType HttpStream::GetType(const char8 * const name) {
    return (storeBody) ? (storedData.GetType(name)) : (StreamStructuredData<JsonPrinter>::GetType(name));
}

bool HttpStream::Write(const char8 * const name,
                       const AnyType &value) {
    return (storeBody) ? (storedData.Write(name, value)) : (StreamStructuredData<JsonPrinter>::Write(name, value));
}

bool HttpStream::Copy(StructuredDataI &destination) {
    return (storeBody) ? (storedData.Copy(destination)) : (StreamStructuredData<JsonPrinter>::Copy(destination));
}

bool HttpStream::AddToCurrentNode(Reference node) {
    return (storeBody) ? (storedData.AddToCurrentNode(node)) : (StreamStructuredData<JsonPrinter>::AddToCurrentNode(node));
}

bool HttpStream::MoveToRoot() {
    bool ret = (storeBody) ? (storedData.MoveToRoot()) : (StreamStructuredData<JsonPrinter>::MoveToRoot());
    if ((ret) && (storeBody)) {
        Printf("%J!", storedData);
    }
    return ret;
}

bool HttpStream::MoveToAncestor(uint32 generations) {
    return (storeBody) ? (storedData.MoveToAncestor(generations)) : (StreamStructuredData<JsonPrinter>::MoveToAncestor(generations));
}

bool HttpStream::MoveAbsolute(const char8 * const path) {
    return (storeBody) ? (storedData.MoveAbsolute(path)) : (StreamStructuredData<JsonPrinter>::MoveAbsolute(path));
}

bool HttpStream::MoveRelative(const char8 * const path) {
    return (storeBody) ? (storedData.MoveRelative(path)) : (StreamStructuredData<JsonPrinter>::MoveRelative(path));
}

bool HttpStream::MoveToChild(const uint32 childIdx) {
    return (storeBody) ? (storedData.MoveToChild(childIdx)) : (StreamStructuredData<JsonPrinter>::MoveToChild(childIdx));
}

bool HttpStream::CreateAbsolute(const char8 * const path) {
    return (storeBody) ? (storedData.CreateAbsolute(path)) : (StreamStructuredData<JsonPrinter>::CreateAbsolute(path));

}

bool HttpStream::CreateRelative(const char8 * const path) {
    return (storeBody) ? (storedData.CreateRelative(path)) : (StreamStructuredData<JsonPrinter>::CreateRelative(path));
}

bool HttpStream::Delete(const char8 * const name) {
    return (storeBody) ? (storedData.Delete(name)) : (StreamStructuredData<JsonPrinter>::Delete(name));
}

const char8 *HttpStream::GetName() {
    return (storeBody) ? (storedData.GetName()) : (StreamStructuredData<JsonPrinter>::GetName());
}

const char8 *HttpStream::GetChildName(const uint32 index) {
    return (storeBody) ? (storedData.GetChildName(index)) : (StreamStructuredData<JsonPrinter>::GetChildName(index));
}

uint32 HttpStream::GetNumberOfChildren() {
    return (storeBody) ? (storedData.GetNumberOfChildren()) : (StreamStructuredData<JsonPrinter>::GetNumberOfChildren());
}

}

