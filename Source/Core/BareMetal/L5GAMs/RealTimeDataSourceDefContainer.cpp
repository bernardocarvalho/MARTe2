/**
 * @file RealTimeDataSourceDefContainer.cpp
 * @brief Source file for class RealTimeDataSourceDefContainer
 * @date 01/03/2016
 * @author Giuseppe Ferrò
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
 * the class RealTimeDataSourceDefContainer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceDefContainer.h"
#include "ReferenceContainerFilterObjectName.h"
#include "RealTimeDataSourceDef.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeDataSourceDefContainer::RealTimeDataSourceDefContainer() {
    numberOfInitialDDBs = 0u;
    final = true;
}

bool RealTimeDataSourceDefContainer::AddDataDefinition(ReferenceT<GAM> gam) {

    bool ret = (gam.IsValid());
    if (ret) {
        const char8 * userName = gam->GetName();
        StreamString *supportedStates = gam->GetSupportedStates();
        uint32 numberOfStates = gam->GetNumberOfSupportedStates();

        // the number of definition containers (input, output, ecc)
        uint32 numberOfElements = gam->Size();
        for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
            ReferenceT<RealTimeDataDefContainer> defContainer = gam->Get(i);
            ret = defContainer.IsValid();
            if (ret) {
                bool isProducer = defContainer->IsOutput();
                bool isConsumer = defContainer->IsInput();

                uint32 numberOfDefinitions = defContainer->Size();
                for (uint32 j = 0u; (j < numberOfDefinitions) && (ret); j++) {
                    ReferenceT<RealTimeDataDefI> definition = defContainer->Get(j);
                    ret = definition.IsValid();
                    if (ret) {
                        ret = AddSingleDataDefinition(definition, userName, supportedStates, numberOfStates, isProducer, isConsumer);
                    }
                    else {
                        //TODO Does not contain a definition
                    }
                }
            }
            else {
                //TODO Does not contain a definition container
            }
        }
    }
    else {
        //TODO invalid
        printf("\nError, data container not found\n");
    }
    return ret;
}

bool RealTimeDataSourceDefContainer::AddSingleDataDefinition(ReferenceT<RealTimeDataDefI> definition,
                                                             const char8 * userName,
                                                             StreamString *supportedStates,
                                                             uint32 numberOfStates,
                                                             bool isProducer,
                                                             bool isConsumer,
                                                             StreamString defaultPath) {

    bool ret = definition.IsValid();
    if (ret) {
        StreamString path = definition->GetPath();
        bool isLeaf = (definition->Size() == 0u);

        // the path exists?
        // default refresh
        StreamString newDefaultPath = path;
        if (path == "") {
            // set the path
            newDefaultPath=defaultPath;
            newDefaultPath += definition->GetName();
            path = newDefaultPath;
        }
        newDefaultPath += ".";


        if (isLeaf) {
            printf("\nfound a leaf, the path is %s the user is %s\n", path.Buffer(), userName);

            ReferenceT<RealTimeDataSourceDef> element = Find(path.Buffer());
            if (element.IsValid()) {

                printf("\nleaf already inside\n");

                // if the path exists adds only the infos
                if (isConsumer) {
                    for (uint32 i = 0u; i < numberOfStates; i++) {
                        element->AddConsumer(supportedStates[i].Buffer(), userName);
                    }
                }
                if (isProducer) {
                    for (uint32 i = 0u; i < numberOfStates; i++) {
                        element->Addproducer(supportedStates[i].Buffer(), userName);
                    }
                }
            }
            // if the definition does not exist creates it
            else {

                ReferenceT<RealTimeDataSourceDef> element(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                if (element.IsValid()) {
                    printf("\nleaf added\n");

                    if (isConsumer) {
                        for (uint32 i = 0u; i < numberOfStates; i++) {
                            element->AddConsumer(supportedStates[i].Buffer(), userName);
                        }
                    }
                    if (isProducer) {
                        for (uint32 i = 0u; i < numberOfStates; i++) {
                            element->Addproducer(supportedStates[i].Buffer(), userName);
                        }
                    }
                    ret = Insert(path.Buffer(), element);
                }
            }
        }
        else {
            printf("\nfound a structure, the path is %s the user is %s\n", path.Buffer(), userName);

            // is structured, go inside to the sub members
            uint32 numberOfMembers = definition->Size();
            ret = true;
            for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                ReferenceT<RealTimeDataDefI> subDefinition = definition->Get(i);
                if (subDefinition.IsValid()) {
                    ret = AddSingleDataDefinition(subDefinition, userName, supportedStates, numberOfStates, isProducer, isConsumer, newDefaultPath);
                }
            }
        }

        if (ret) {
            // if final can not add another definition
            if (final) {
                ret = (Size() == numberOfInitialDDBs);
            }
        }
    }
    else {
        //TODO Invalid
    }
    return ret;

}

bool RealTimeDataSourceDefContainer::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        StreamString isFinal;
        if (data.Read("IsFinal", isFinal)) {
            if (isFinal == "true") {
                final = true;
            }
            else
                final = false;
        }
    }
    numberOfInitialDDBs = Size();
    return ret;
}

bool RealTimeDataSourceDefContainer::Verify() {
    uint32 size = Size();
    bool ret = true;
    for (uint32 i = 0u; (i < size) && (ret); i++) {
        ReferenceT<RealTimeDataSourceDef> rtDef = Get(i);
        if (rtDef.IsValid()) {
            rtDef->Verify();
        }
    }
    return ret;
}
CLASS_REGISTER(RealTimeDataSourceDefContainer, "1.0")

}
