/**
 * @file StructuredDataStreamT.h
 * @brief Header file for class StructuredDataStreamT
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

 * @details This header file contains the declaration of the class StructuredDataStreamT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRUCTUREDDATASTREAM_H_
#define STRUCTUREDDATASTREAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MultiStream.h"
#include "StructuredDataI.h"
#include "AnyType.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<class structuredData>
class StructuredDataStreamT: public MultiStream {
public:
    StructuredDataStreamT();

    virtual ~StructuredDataStreamT();

    virtual bool Switch(const char8 *path);

    virtual bool Load(const char8 *varName);

    virtual bool Commit(const char8 *varName);

    virtual bool Delete(const char8 *varName);

    virtual void GetCurrentPath(StreamString &path);

    virtual bool SwitchPrintAndCommit(const char8* path,
                                      const char8* varName,
                                      const char8* format,
                                      const AnyType pars[]);

    virtual bool IsValid() const;

    structuredData* GetData() const;

protected:

    structuredData *data;

    StreamString nodeName;
    StreamString leafName;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {
//#define NULL_PTR(x) NULL

template<class structuredData>
StructuredDataStreamT<structuredData>::StructuredDataStreamT() {
    data = new structuredData;
    StructuredDataI* x = dynamic_cast<StructuredDataI*>(data);
    if (x == NULL) {
        delete data;
        data=NULL;
    }
    nodeName = "";
    leafName = "";
}

template<class structuredData>
StructuredDataStreamT<structuredData>::~StructuredDataStreamT() {
    if (data != NULL) {
        delete data;
        data=NULL;
    }
}

template<class structuredData>
bool StructuredDataStreamT<structuredData>::Switch(const char8 *path) {
    bool ret = true;
    if ((path != NULL) && (StringHelper::Length(path)>0u)) {
        if(!data->MoveAbsolute(path)) {
            ret=data->CreateAbsolute(path);
        }
        if(ret) {
            nodeName=path;
        }
    }
    else {
        ret = data->MoveToRoot();
        nodeName = "";
    }
    leafName = "";
    return ret;
}

template<class structuredData>
bool StructuredDataStreamT<structuredData>::Load(const char8 *varName) {
    bool ret = this->SetSize(0u);
    if (ret) {
        ret = data->Read(varName, *this);
        if (ret) {
            leafName = varName;
        }
    }
    return ret;
}

template<class structuredData>
bool StructuredDataStreamT<structuredData>::Commit(const char8 *varName) {
    (void) Delete(varName);
    bool ret = data->Write(varName, this->Buffer());
    if (ret) {
        leafName = varName;
    }
    return ret;
}

template<class structuredData>
bool StructuredDataStreamT<structuredData>::Delete(const char8 *varName) {
    bool ret = data->Delete(varName);
    if (ret) {
        leafName = "";
    }
    return ret;
}

template<class structuredData>
void StructuredDataStreamT<structuredData>::GetCurrentPath(StreamString &path) {
    path = nodeName;
    if (leafName.Size() > 0u) {
        path += ".";
        path += leafName;
    }
}

template<class structuredData>
bool StructuredDataStreamT<structuredData>::SwitchPrintAndCommit(const char8* path,
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

template<class structuredData>
structuredData* StructuredDataStreamT<structuredData>::GetData() const {
    return data;
}

template<class structuredData>
bool StructuredDataStreamT<structuredData>::IsValid() const {

    return (data != NULL);

}

}

#endif /* STRUCTUREDDATASTREAM_H_ */

