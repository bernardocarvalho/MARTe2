/**
 * @file ConfigurationDatabase.cpp
 * @brief Source file for class ConfigurationDatabase
 * @date 27/10/2015
 * @author Andre' Neto
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
 * the class ConfigurationDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#define DLL_API

#include "AnyObject.h"
#include "ErrorType.h"
#include "ConfigurationDatabase.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferences.h"
#include "StreamString.h"
#include "TypeConversion.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ConfigurationDatabase::ConfigurationDatabase() :
        Object() {
    mux.Create();
    ReferenceT<ConfigurationDatabaseNode> rootContainer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    rootNode = rootContainer;
    currentNode = rootNode;
}

ConfigurationDatabase::~ConfigurationDatabase() {
}

ConfigurationDatabase::ConfigurationDatabase(const ConfigurationDatabase &toCopy) :
        Object(toCopy) {
    mux = toCopy.mux;
    rootNode = toCopy.rootNode;
    currentNode = toCopy.currentNode;
}

ConfigurationDatabase &ConfigurationDatabase::operator =(const ConfigurationDatabase &toCopy) {
    if (this != &toCopy) {
        mux = toCopy.mux;
        rootNode = toCopy.rootNode;
        currentNode = toCopy.currentNode;
    }
    return *this;
}

void ConfigurationDatabase::Purge() {
    currentNode = rootNode;
    rootNode->Purge();
}

bool ConfigurationDatabase::Write(const char8 * const name, const AnyType &value) {

    bool ok = false;
    // call conversion Object-StructuredDataI or StructuredDataI-StructuredDataI
    bool isRegisteredObject = (value.GetTypeDescriptor().isStructuredData);
    bool isStructuredDataI = (value.GetTypeDescriptor() == StructuredDataInterfaceType);
    if ((isRegisteredObject) || (isStructuredDataI)) {
        ReferenceT<ReferenceContainer> storeCurrentNode = currentNode;
        if (CreateRelative(name)) {
            ok = TypeConvert((*this).operator MARTe::AnyType(), value);
        }
        currentNode = storeCurrentNode;
    }
    else {
        ok = (StringHelper::Length(name) > 0u);
        if (ok) {
            AnyType existentType = GetType(name);
            if (existentType.GetTypeDescriptor() != voidAnyType.GetTypeDescriptor()) {
                ok = Delete(name);
            }
        }
        if (ok) {

            ReferenceT<AnyObject> objToWrite(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ok = objToWrite.IsValid();
            if (ok) {
                ok = objToWrite->Serialise(value);
                if (ok) {
                    objToWrite->SetName(name);
                    ok = currentNode.IsValid();
                    if (ok) {
                        ok = currentNode->Insert(objToWrite);
                    }
                }
            }
        }
    }
    return ok;
}

AnyType ConfigurationDatabase::GetType(const char8 * const name) {
    bool found = false;
    Reference foundReference;
    uint32 i;
    for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
        foundReference = currentNode->Get(i);
        found = (StringHelper::Compare(foundReference->GetName(), name) == 0);
    }

    AnyType retType;
    if (found) {
        ReferenceT<AnyObject> objToRead = foundReference;
        if (objToRead.IsValid()) {
            retType = objToRead->GetType();
        }
    }

    return retType;
}

bool ConfigurationDatabase::Copy(StructuredDataI &destination) {
    ReferenceT<ReferenceContainer> foundNode;
    bool ok = true;
    for (uint32 i = 0u; (i < currentNode->Size()) && (ok); i++) {
        foundNode = currentNode->Get(i);
        if (foundNode.IsValid()) {
            if (!destination.CreateRelative(foundNode->GetName())) {
                ok = false;
            }
            if ((!MoveRelative(foundNode->GetName())) && ok) {
                ok = false;
            }
            if (ok) {
                // go recursively !
                ok = Copy(destination);
            }
            if ((!MoveToAncestor(1u)) && ok) {
                ok = false;
            }
            if ((!destination.MoveToAncestor(1u)) && ok) {
                ok = false;
            }
        }
        else {
            ReferenceT<AnyObject> foundLeaf = currentNode->Get(i);

            if (foundLeaf.IsValid()) {
                ok = destination.Write(foundLeaf->GetName(), foundLeaf->GetType());
            }
        }
    }
    return ok;
}

bool ConfigurationDatabase::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Copy(*this);
    }
    return ok;
}

bool ConfigurationDatabase::MoveToRoot() {
    bool ok = rootNode.IsValid();
    if (ok) {
        currentNode = rootNode;
    }
    return ok;
}

bool ConfigurationDatabase::Read(const char8 * const name, const AnyType &value) {

    bool ok = false;
    // call conversion Object-StructuredDataI or StructuredDataI-StructuredDataI
    bool isRegisteredObject = (value.GetTypeDescriptor().isStructuredData);
    bool isStructuredDataI = (value.GetTypeDescriptor() == StructuredDataInterfaceType);
    if ((isRegisteredObject) || (isStructuredDataI)) {
        ReferenceT<ReferenceContainer> storeCurrentNode = currentNode;
        if (MoveRelative(name)) {
            ok = TypeConvert(value, (*this).operator MARTe::AnyType());
        }
        currentNode = storeCurrentNode;
    }
    else {

        //Could have used the ReferenceContainerFilterObjectName but this way is faster given that no complex paths are involved
        bool found = false;
        Reference foundReference;
        uint32 i;
        for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
            foundReference = currentNode->Get(i);
            found = (StringHelper::Compare(foundReference->GetName(), name) == 0);
        }

        ok = found;
        if (ok) {

            ReferenceT<AnyObject> objToRead = foundReference;
            ok = objToRead.IsValid();
            if (ok) {
                ok = TypeConvert(value, objToRead->GetType());
            }
        }
    }

    return ok;
}

bool ConfigurationDatabase::MoveAbsolute(const char8 * const path) {

    ReferenceContainerFilterObjectName filter(1, 0u, path);
    ReferenceContainer resultSingle;
    rootNode->Find(resultSingle, filter);

    bool ok = (resultSingle.Size() > 0u);
    if (ok) {
        //Invalidate move to leafs
        ReferenceT<ReferenceContainer> container = resultSingle.Get(resultSingle.Size() - 1u);
        ok = container.IsValid();
        if (ok) {
            currentNode = container;
        }
    }

    return ok;
}

bool ConfigurationDatabase::MoveRelative(const char8 * const path) {

    ReferenceContainerFilterObjectName filter(1, 0u, path);
    ReferenceContainer resultSingle;
    currentNode->Find(resultSingle, filter);

    bool ok = (resultSingle.Size() > 0u);
    if (ok) {
        //Invalidate move to leafs
        ReferenceT<ReferenceContainer> container = resultSingle.Get(resultSingle.Size() - 1u);
        ok = container.IsValid();
        if (ok) {
            currentNode = container;
        }
    }

    return ok;
}

bool ConfigurationDatabase::MoveToChild(const uint32 childIdx) {
    bool ok = (childIdx < currentNode->Size());
    if (ok) {
        currentNode = currentNode->Get(childIdx);
    }
    return ok;
}

bool ConfigurationDatabase::MoveToBrother() {
    ReferenceT<ReferenceContainer> ref = currentNode->GetBrother();
    if (ref.IsValid()) {
        currentNode = ref;
    }
    return ref.IsValid();
}

bool ConfigurationDatabase::MoveToAncestor(const uint32 generations) {
    bool ok = (generations != 0u);
    if (ok) {
        ReferenceContainerFilterReferences filter(
                1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH, currentNode);
        ReferenceContainer resultPath;
        rootNode->Find(resultPath, filter);
        ok = (resultPath.Size() > 0u);
        if (ok) {
            int32 newPositionIdx = (static_cast<int32>(resultPath.Size()) - 1) - static_cast<int32>(generations);
            ok = (newPositionIdx >= -1);
            if (ok) {
                if (newPositionIdx == -1) {
                    currentNode = rootNode;
                }
                else {
                    currentNode = resultPath.Get(static_cast<uint32>(newPositionIdx));
                }
            }
        }
    }
    return ok;
}

bool ConfigurationDatabase::CreateNodes(const char8 * const path) {
    StreamString pathStr = path;
    bool ok = pathStr.Seek(0Lu);
    if (ok) {
        ok = (pathStr.Size() > 0u);
    }
    StreamString token;
    char8 c;
    bool created = false;
    ReferenceT<ReferenceContainer> currentNodeOld = currentNode;

    while ((pathStr.GetToken(token, ".", c)) && (ok)) {
        ok = (token.Size() > 0u);
        if (ok) {
            //Check if a node with this name already exists
            bool found = false;
            Reference foundReference;
            Reference foundReferencePrev;
            uint32 i;
            for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
                foundReferencePrev = foundReference;
                foundReference = currentNode->Get(i);
                found = (StringHelper::Compare(foundReference->GetName(), token.Buffer()) == 0);
            }

            if (found) {
                currentNode = foundReference;
            }
            else {
                ReferenceT<ConfigurationDatabaseNode> container(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                container->SetName(token.Buffer());
                ok = currentNode->Insert(container);
                if (ok) {
                    ReferenceT<ConfigurationDatabaseNode> lastAddedChild = currentNode->GetLastAddedChild();
                    if (lastAddedChild.IsValid()) {
                        lastAddedChild->SetBrother(container);
                    }
                    currentNode->SetLastAddedChild(container);
                    currentNode = container;
                    created = true;
                }
            }
        }

        if (ok) {
            ok = token.Seek(0Lu);
            if (ok) {
                ok = token.SetSize(0Lu);
            }

        }
    }
    if (ok) {
        ok = created;
    }
    if (!ok) {
        currentNode = currentNodeOld;
    }
    return ok;
}

bool ConfigurationDatabase::CreateAbsolute(const char8 * const path) {
    currentNode = rootNode;
    return CreateNodes(path);
}

bool ConfigurationDatabase::CreateRelative(const char8 * const path) {
    return CreateNodes(path);
}

bool ConfigurationDatabase::Delete(const char8 * const name) {
    bool ok = false;
    Reference foundReference;
    uint32 i;
    for (i = 0u; (i < currentNode->Size()) && (!ok); i++) {
        foundReference = currentNode->Get(i);
        ok = (StringHelper::Compare(foundReference->GetName(), name) == 0);
    }

    if (ok) {
        ok = currentNode->Delete(foundReference);
    }

    return ok;
}

bool ConfigurationDatabase::AddToCurrentNode(Reference node) {
    ReferenceT<ReferenceContainer> nodeToAdd = node;
    bool ok = nodeToAdd.IsValid();
    if (ok) {
        ok = currentNode->Insert(nodeToAdd);
    }
    return ok;
}

const char8 *ConfigurationDatabase::GetName() {
    return (currentNode.IsValid()) ? (currentNode->GetName()) : (NULL_PTR(const char8*));
}

const char8 *ConfigurationDatabase::GetChildName(const uint32 index) {
    Reference foundReference = currentNode->Get(index);
    return (foundReference.IsValid()) ? (foundReference->GetName()) : (NULL_PTR(const char8*));
}

uint32 ConfigurationDatabase::GetNumberOfChildren() {
    return currentNode->Size();
}

bool ConfigurationDatabase::Lock(const TimeoutType &timeout) {
    return (mux.FastLock(timeout) == ErrorManagement::NoError);
}

void ConfigurationDatabase::Unlock() {
    mux.FastUnLock();
}

ReferenceT<ReferenceContainer> ConfigurationDatabase::GetCurrentNode() const {
    return currentNode;
}

void ConfigurationDatabase::SetCurrentNodeAsRootNode() {
    rootNode = currentNode;
}

void ConfigurationDatabase::Purge(ReferenceContainer &purgeList) {
    ReferenceT<ReferenceContainer> emptyReference;
    if (currentNode.IsValid()) {
        currentNode->SetLastAddedChild(emptyReference);
        currentNode->SetBrother(emptyReference);
        currentNode->Purge(purgeList);
    }
    if (rootNode.IsValid()) {
        rootNode->Purge(purgeList);
    }
}

CLASS_REGISTER(ConfigurationDatabase, "1.0")

}
