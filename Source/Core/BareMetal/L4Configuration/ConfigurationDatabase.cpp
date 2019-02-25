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
#include <iostream>
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
#include <stdio.h>
void ConfigurationDatabase::StupidPrintf(const char8 * fun, bool inp) {
    /*ConfigurationDatabase temp = *this;
     if (temp.MoveAbsolute("Functions.1.Signals")) {
     if (inp) {
     printf("%s\n", fun);
     }
     std::unordered_map<std::string, int>::iterator it;
     printf("My childs of %s now are:\n", temp.currentNode->GetName());
     for (it = temp.currentNode->childs.begin(); (it != temp.currentNode->childs.end()); it++) {
     printf("[%s]\n", it->first.c_str());
     }
     if (!inp) {
     printf("%s\n", fun);
     }
     }*/
}

ConfigurationDatabaseNode::ConfigurationDatabaseNode() {

}

ConfigurationDatabaseNode::~ConfigurationDatabaseNode() {
    childIdxs.clear();
    childRefs.clear();
    parent = Reference();
}

CLASS_REGISTER(ConfigurationDatabaseNode, "1.0")

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
    //rootNode->Purge();
    rootNode->childIdxs.clear();
    rootNode->childRefs.clear();
}

bool ConfigurationDatabase::Write(const char8 * const name, const AnyType &value) {

    StupidPrintf("Write", true);
    bool ok = false;
    // call conversion Object-StructuredDataI or StructuredDataI-StructuredDataI
    bool isRegisteredObject = (value.GetTypeDescriptor().isStructuredData);
    bool isStructuredDataI = (value.GetTypeDescriptor() == StructuredDataInterfaceType);
    if ((isRegisteredObject) || (isStructuredDataI)) {
        //ReferenceT<ReferenceContainer> storeCurrentNode = currentNode;
        ReferenceT<ConfigurationDatabaseNode> storeCurrentNode = currentNode;
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
                        //Check if the value already exists
                        std::unordered_map<std::string, int>::iterator it = currentNode->childIdxs.find(name);
                        if (it == currentNode->childIdxs.end()) {
                            currentNode->childRefs.push_back(objToWrite);
                            currentNode->childIdxs[name] = (currentNode->childRefs.size() - 1);
                        }
                        else {
                            currentNode->childRefs[it->second] = objToWrite;
                        }
                        //ok = currentNode->Insert(objToWrite);
                    }
                }
            }
        }
    }
    StupidPrintf("Write", false);
    return ok;
}

AnyType ConfigurationDatabase::GetType(const char8 * const name) {
    StupidPrintf("GetType", true);
    bool found = false;
    Reference foundReference;
    //uint32 i;
    if (currentNode.IsValid()) {
        /*for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
         foundReference = currentNode->Get(i);
         found = (StringHelper::Compare(foundReference->GetName(), name) == 0);
         }*/
        if (name != NULL_PTR(const char8 * const)) {
            std::unordered_map<std::string, int>::iterator it = currentNode->childIdxs.find(name);
            found = (it != currentNode->childIdxs.end());
            if (found) {
                foundReference = currentNode->childRefs[it->second];
            }
        }
    }
    AnyType retType;
    if (found) {
        ReferenceT<AnyObject> objToRead = foundReference;
        if (objToRead.IsValid()) {
            retType = objToRead->GetType();
        }
    }
    StupidPrintf("GetType", false);
    return retType;
}

bool ConfigurationDatabase::Copy(StructuredDataI &destination) {
    StupidPrintf("Copy", true);
    ReferenceT<ConfigurationDatabaseNode> foundNode;
    bool ok = true;
    std::vector<Reference>::iterator it = currentNode->childRefs.begin();
    for (; it != currentNode->childRefs.end(); it++) {
        //for (uint32 i = 0u; (i < currentNode->childs.size()) && (ok); i++) {
        //foundNode = currentNode->childs.Get(i);
        foundNode = *it;
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
            //ReferenceT<AnyObject> foundLeaf = currentNode->Get(i);
            ReferenceT<AnyObject> foundLeaf = *it;

            if (foundLeaf.IsValid()) {
                ok = destination.Write(foundLeaf->GetName(), foundLeaf->GetType());
            }
        }
    }
    StupidPrintf("Copy", false);

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
    StupidPrintf("Read", true);
    bool ok = (name != NULL_PTR(const char8 * const));
    // call conversion Object-StructuredDataI or StructuredDataI-StructuredDataI
    bool isRegisteredObject = (value.GetTypeDescriptor().isStructuredData);
    bool isStructuredDataI = (value.GetTypeDescriptor() == StructuredDataInterfaceType);
    if ((isRegisteredObject) || (isStructuredDataI)) {
        ReferenceT<ConfigurationDatabaseNode> storeCurrentNode = currentNode;
        if (ok) {
            if (MoveRelative(name)) {
                ok = TypeConvert(value, (*this).operator MARTe::AnyType());
            }
        }
        currentNode = storeCurrentNode;
    }
    else {
        if (ok) {
            ok = currentNode.IsValid();
        }
        if (ok) {
            //Could have used the ReferenceContainerFilterObjectName but this way is faster given that no complex paths are involved
            std::unordered_map<std::string, int>::iterator it = currentNode->childIdxs.find(name);
            ReferenceT<AnyObject> objToRead;
            ok = (it != currentNode->childIdxs.end());
            /*uint32 i;
             for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
             foundReference = currentNode->Get(i);
             found = (StringHelper::Compare(foundReference->GetName(), name) == 0);
             }*/
            if (ok) {
                objToRead = currentNode->childRefs[it->second];
                ok = objToRead.IsValid();
                if (ok) {
                    ok = TypeConvert(value, objToRead->GetType());
                }
            }
        }
    }
    StupidPrintf("Read", false);

    return ok;
}

bool ConfigurationDatabase::MoveAbsolute(const char8 * const path) {
//StupidPrintf("MoveAbsolute", true);

    /*ReferenceContainerFilterObjectName filter(1, 0u, path);
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
     }*/

    StreamString token;
    StreamString paths = path;
    bool ok = paths.Seek(0LLU);
    if (ok) {
        ok = (paths.Size() > 0LLU);
    }
    char8 terminator;
    ReferenceT<ConfigurationDatabaseNode> tempNode = rootNode;
    bool moved = false;
    while ((paths.GetToken(token, ".", terminator)) && (ok)) {
        std::unordered_map<std::string, int>::iterator it = tempNode->childIdxs.find(token.Buffer());
        ok = (it != tempNode->childIdxs.end());
        if (ok) {
            tempNode = tempNode->childRefs[it->second];
            ok = tempNode.IsValid();
            token = "";
            moved = ok;
        }
    }
    if (ok) {
        ok = moved;
    }
    if (ok) {
        currentNode = tempNode;
    }
//StupidPrintf("MoveAbsolute", false);
    return ok;
}

bool ConfigurationDatabase::MoveRelative(const char8 * const path) {
    StupidPrintf("MoveRelative", true);
    /*ReferenceContainerFilterObjectName filter(1, 0u, path);
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
     }*/
    StreamString token;
    StreamString paths = path;
    bool ok = paths.Seek(0LLU);
    if (ok) {
        ok = (paths.Size() > 0LLU);
    }
    char8 terminator;
    ReferenceT<ConfigurationDatabaseNode> tempNode = currentNode;
    bool moved = false;
    while ((paths.GetToken(token, ".", terminator)) && (ok)) {
        std::unordered_map<std::string, int>::iterator it = tempNode->childIdxs.find(token.Buffer());
        ok = (it != tempNode->childIdxs.end());
        if (ok) {
            tempNode = tempNode->childRefs[it->second];
            ok = tempNode.IsValid();
            token = "";
            moved = ok;
        }
    }
    if (ok) {
        ok = moved;
    }
    if (ok) {
        currentNode = tempNode;
    }
    StupidPrintf("MoveRelative", false);
    return ok;
}

bool ConfigurationDatabase::MoveToChild(const uint32 childIdx) {
    StupidPrintf("MoveToChild", true);
    /*bool ok = (childIdx < currentNode->Size());
     if (ok) {
     currentNode = currentNode->Get(childIdx);
     }
     return ok;*/
    bool ok = (childIdx < currentNode->childRefs.size());

    ReferenceT<ConfigurationDatabaseNode> nodeToFind;
    if (ok) {
        nodeToFind = currentNode->childRefs[childIdx];
        ok = nodeToFind.IsValid();
    }
    if (ok) {
        currentNode = nodeToFind;
    }
    StupidPrintf("MoveToChild", false);
    return ok;
}

bool ConfigurationDatabase::MoveToAncestor(const uint32 generations) {
    StupidPrintf("MoveToAncestor", true);
    bool ok = (generations != 0u);
    ReferenceT<ConfigurationDatabaseNode> tempNode = currentNode;
    if (ok) {
        uint32 n;
        bool rootNodeFound = (tempNode == rootNode);
        for (n = 0u; (n < generations) && (ok) && (!rootNodeFound); n++) {
            if (ok) {
                tempNode = tempNode->parent;
            }
            rootNodeFound = (tempNode == rootNode);
        }
        if (rootNodeFound) {
            ok = (n == generations);
        }
        /*ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH, currentNode);
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
         }*/
    }
    if (ok) {
        currentNode = tempNode;
    }
    StupidPrintf("MoveToAncestor", false);
    return ok;
}

bool ConfigurationDatabase::CreateNodes(const char8 * const path) {
    StupidPrintf("CreateNodes", true);
    StreamString pathStr = path;
    bool ok = pathStr.Seek(0Lu);
    if (ok) {
        ok = (pathStr.Size() > 0u);
    }
    StreamString token;
    char8 c;
    bool created = false;
    ReferenceT<ConfigurationDatabaseNode> currentNodeOld = currentNode;
    while ((pathStr.GetToken(token, ".", c)) && (ok)) {
        ok = (token.Size() > 0u);
        if (ok) {
            //Check if a node with this name already exists
            /*uint32 i;
             for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
             foundReference = currentNode->Get(i);
             found = (StringHelper::Compare(foundReference->GetName(), token.Buffer()) == 0);
             }*/
            std::unordered_map<std::string, int>::iterator it = currentNode->childIdxs.find(token.Buffer());
            bool found = (it != currentNode->childIdxs.end());
            if (found) {
                currentNode = currentNode->childRefs[it->second];
            }
            else {
                //ReferenceT<ReferenceContainer> container(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                ReferenceT<ConfigurationDatabaseNode> container(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                container->SetName(token.Buffer());
                container->parent = currentNode;
                //ok = currentNode->Insert(container);
                currentNode->childRefs.push_back(container);
                currentNode->childIdxs[token.Buffer()] = (currentNode->childRefs.size() - 1);

                //printf("Z[%s](%p)[%s] = %s(%p)\n", currentNode->GetName(), currentNode.operator ->(), token.Buffer(), container->GetName(), container.operator ->());
                //if (ok) {
                currentNode = container;
                created = true;
                //}
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
    StupidPrintf("CreateNodes", false);
    return ok;
}

bool ConfigurationDatabase::CreateAbsolute(const char8 * const path) {
    currentNode = rootNode;
    return CreateNodes(path);
}

bool ConfigurationDatabase::CreateRelative(const char8 * const path) {
    bool ok = CreateNodes(path);
//Parent now holds
    /*ReferenceT<ConfigurationDatabaseNode> p = currentNode->parent;
     std::unordered_map<std::string, int>::iterator it;
     //printf("Childs of %s now are:\n", p->GetName());
     for (it = p->childs.begin(); (it != p->childs.end()); it++) {
     printf("[%s]\n", it->first.c_str());
     }*/
    return ok;
}

bool ConfigurationDatabase::Delete(const char8 * const name) {
    bool ok = (name != NULL_PTR(const char8 * const));

    if (ok) {
        std::unordered_map<std::string, int>::iterator it = currentNode->childIdxs.find(name);
        ok = (it != currentNode->childIdxs.end());
        if (ok) {
            currentNode->childRefs.erase(currentNode->childRefs.begin() + it->second);
            currentNode->childIdxs.erase(name);
        }
        if (ok) {
            uint32 i = 0u;
            //Need to remap
            std::vector<Reference>::iterator it = currentNode->childRefs.begin();
            for (; it != currentNode->childRefs.end(); it++) {
                ReferenceT<Object> obj = *it;
                currentNode->childIdxs[obj->GetName()] = i;
                i++;
            }
        }
    }
    /*uint32 i;
     for (i = 0u; (i < currentNode->Size()) && (!ok); i++) {
     foundReference = currentNode->Get(i);
     ok = (StringHelper::Compare(foundReference->GetName(), name) == 0);
     }

     if (ok) {
     ok = currentNode->Delete(foundReference);
     }*/

    return ok;
}

bool ConfigurationDatabase::AddToCurrentNode(Reference node) {
    StupidPrintf("AddToCurrentNode", true);
    ReferenceT<ConfigurationDatabaseNode> nodeToAdd = node;
    bool ok = nodeToAdd.IsValid();
    if (ok) {
        //ok = currentNode->Insert(nodeToAdd);
        std::unordered_map<std::string, int>::iterator it = currentNode->childIdxs.find(node->GetName());
        if (it == currentNode->childIdxs.end()) {
            currentNode->childRefs.push_back(node);
            currentNode->childIdxs[node->GetName()] = (currentNode->childRefs.size() - 1);
        }
        else {
            currentNode->childRefs[it->second] = node;
        }
    }
    StupidPrintf("AddToCurrentNode", false);
    return ok;
}

const char8 *ConfigurationDatabase::GetName() {
    return (currentNode.IsValid()) ? (currentNode->GetName()) : (NULL_PTR(const char8*));
}

const char8 *ConfigurationDatabase::GetChildName(const uint32 index) {
    /*Reference foundReference = currentNode->Get(index);
     return (foundReference.IsValid()) ? (foundReference->GetName()) : (NULL_PTR(const char8*));*/
    const char8 * childName = NULL_PTR(const char8*);
    bool ok = (index < currentNode->childRefs.size());
    if (ok) {
        childName = currentNode->childRefs[index]->GetName();
    }
    return childName;
}

uint32 ConfigurationDatabase::GetNumberOfChildren() {
    uint32 size = 0u;
    if (currentNode.IsValid()) {
        //size = currentNode->Size();
        size = currentNode->childRefs.size();
    }
    return size;
}

bool ConfigurationDatabase::Lock(const TimeoutType &timeout) {
    return (mux.FastLock(timeout) == ErrorManagement::NoError);
}

void ConfigurationDatabase::Unlock() {
    mux.FastUnLock();
}

/*ReferenceT<ConfigurationDatabaseNode> ConfigurationDatabase::GetCurrentNode() const {
 return currentNode;
 }*/

void ConfigurationDatabase::SetCurrentNodeAsRootNode() {
    rootNode = currentNode;
}

void ConfigurationDatabase::Purge(ReferenceContainer &purgeList) {
    if (currentNode.IsValid()) {
        currentNode->Purge(purgeList);
    }
    if (rootNode.IsValid()) {
        rootNode->Purge(purgeList);
    }
}

CLASS_REGISTER(ConfigurationDatabase, "1.0")

}
#if 0

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
        ReferenceT<ReferenceContainer> rootContainer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
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
        if (currentNode.IsValid()) {
            for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
                foundReference = currentNode->Get(i);
                found = (StringHelper::Compare(foundReference->GetName(), name) == 0);
            }
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
            ok = currentNode.IsValid();
            if (ok) {
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

    bool ConfigurationDatabase::MoveToAncestor(const uint32 generations) {
        bool ok = (generations != 0u);
        if (ok) {
            ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH,
                    currentNode);
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
                uint32 i;
                for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
                    foundReference = currentNode->Get(i);
                    found = (StringHelper::Compare(foundReference->GetName(), token.Buffer()) == 0);
                }

                if (found) {
                    currentNode = foundReference;
                }
                else {
                    ReferenceT<ReferenceContainer> container(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                    container->SetName(token.Buffer());
                    ok = currentNode->Insert(container);
                    if (ok) {
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
        uint32 size = 0u;
        if (currentNode.IsValid()) {
            size = currentNode->Size();
        }
        return size;
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
        if (currentNode.IsValid()) {
            currentNode->Purge(purgeList);
        }
        if (rootNode.IsValid()) {
            rootNode->Purge(purgeList);
        }
    }

    CLASS_REGISTER(ConfigurationDatabase, "1.0")

}
#endif
