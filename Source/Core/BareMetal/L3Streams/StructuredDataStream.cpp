/**
 * @file StructuredDataStream.cpp
 * @brief Source file for class StructuredDataStream
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
 * the class StructuredDataStream (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StructuredDataStream.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
//#define NULL_PTR(x) NULL

StructuredDataStream::StructuredDataStream() {
    data = NULL_PTR(StructuredDataI *);
    nodeName = "";
    leafName = "";
}

StructuredDataStream::~StructuredDataStream() {
    data = NULL_PTR(StructuredDataI *);
}

bool StructuredDataStream::Switch(const char8 *path) {
    bool ret = true;
    bool moved = false;
    if (path != NULL) {
        if(StringHelper::Length(path)>0u) {
            if(!data->MoveRelative(path)) {
                ret=data->CreateRelative(path);
                moved=true;
            }
        }
    }
    if (!moved) {
        ret = data->MoveToRoot();
        nodeName = "";
    }
    leafName = "";
    return ret;
}

bool StructuredDataStream::Load(const char8 *varName) {
    bool ret = data->Read(varName, *this);
    if (ret) {
        leafName = varName;
    }
    return ret;
}

bool StructuredDataStream::Commit(const char8 *varName) {
    (void) DeleteStream(varName);
    bool ret = data->Write(varName, this->Buffer());
    if (ret) {
        leafName = varName;
    }
    return ret;
}

bool StructuredDataStream::DeleteStream(const char8 *varName) {
    return data->Delete(varName);
}

void StructuredDataStream::GetCurrentPath(StreamString &path) {
    path = nodeName;
    path += ".";
    path += leafName;
}

bool StructuredDataStream::SwitchPrintAndCommit(const char8* path,
                                                const char8* varName,
                                                const char8* format,
                                                const AnyType pars[]) {
    StreamString save = this->Buffer();
    StreamString saveLeafName = leafName;
    StreamString saveNodeName = nodeName;

    bool ret = Switch(path);
    if (ret) {
        ret = PrintFormatted(format, pars);

    }
    if (ret) {
        ret = Commit(varName);
    }
    if (ret) {
        *dynamic_cast<StreamString*>(this) = save.Buffer();
        leafName = saveLeafName;
        nodeName = saveNodeName;
        Switch(nodeName.Buffer());
    }
    return ret;
}

}

