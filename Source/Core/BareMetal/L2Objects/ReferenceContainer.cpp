/**
 * @file ReferenceContainer.cpp
 * @brief Source file for class ReferenceContainer
 * @date 12/08/2015
 * @author Andre Neto
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
 * the class ReferenceContainer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ReferenceContainerNode.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceT.h"
#include "ErrorManagement.h"
#include "StringHelper.h"
#include "ReferenceContainerFilterObjectName.h"
#include <typeinfo>
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ReferenceContainer::ReferenceContainer() :
        Object() {
    mux.Create();
    muxTimeout = TTInfiniteWait;
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainer requires pointer to pointer casting*/
Reference ReferenceContainer::Get(const uint32 idx) {
    Reference ref;
    if (Lock()) {
        if (idx < list.ListSize()) {
            ReferenceContainerNode *node = dynamic_cast<ReferenceContainerNode *>(list.ListPeek(idx));
            if (node != NULL) {
                ref = node->GetReference();
            }
        }
        REPORT_ERROR(ErrorManagement::Warning, "ReferenceContainer: input greater than the list size.");
    }
    UnLock();
    return ref;
}

TimeoutType ReferenceContainer::GetTimeout() const {
    return muxTimeout;
}

void ReferenceContainer::SetTimeout(const TimeoutType &timeout) {
    muxTimeout = timeout;
}

/*lint -e{1551} no exception should be thrown given that ReferenceContainer is
 * the sole owner of the list (LinkedListHolder)*/
ReferenceContainer::~ReferenceContainer() {
}

/*lint -e{593} .Justification: The node (newItem) will be deleted by the destructor. */
bool ReferenceContainer::Insert(Reference ref,
                                const int32 &position) {
    bool ok = (Lock());
    if (ok) {
        ReferenceContainerNode *newItem = new ReferenceContainerNode();
        if (newItem->SetReference(ref)) {
            if (position == -1) {
                list.ListAdd(newItem);
            }
            else {
                list.ListInsert(newItem, static_cast<uint32>(position));
            }
        }
        else {
            delete newItem;
            ok = false;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
    }
    UnLock();
    return ok;
}

bool ReferenceContainer::Insert(const char8 * const path,
                                Reference ref) {
    bool ok = ref.IsValid();
    if (ok) {
        if (StringHelper::Length(path) == 0u) {
            ok = Insert(ref);
        }
        else {
            bool created = false;
            ReferenceT<ReferenceContainer> currentNode(this);
            char8 *token = reinterpret_cast<char8*>(HeapManager::Malloc(static_cast<uint32>(sizeof(char8) * StringHelper::Length(path))));
            char8 *nextToken = reinterpret_cast<char8*>(HeapManager::Malloc(static_cast<uint32>(sizeof(char8) * StringHelper::Length(path))));

            const char8* toTokenize = path;
            const char8* next = StringHelper::TokenizeByChars(toTokenize, ".", token);
            toTokenize = next;

            while ((token[0] != '\0') && (ok)) {
                ok = (StringHelper::Length(token) > 0u);
                if (ok) {
                    //Check if a node with this name already exists
                    bool found = false;
                    Reference foundReference;
                    uint32 i;
                    for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
                        foundReference = currentNode->Get(i);
                        found = (StringHelper::Compare(foundReference->GetName(), token) == 0);
                    }
                    // take the next token

                    next = StringHelper::TokenizeByChars(toTokenize, ".", nextToken);
                    toTokenize = next;

                    if (found) {
                        currentNode = foundReference;
                        // if it is a leaf exit (and return false)
                        if (!currentNode.IsValid()) {
                            ok = false;
                        }
                    }
                    else {
                        // insert the reference
                        if (nextToken[0] == '\0') {
                            ref->SetName(token);
                            created = currentNode->Insert(ref);
                        }
                        // create a node
                        else {
                            ReferenceT<ReferenceContainer> container(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                            container->SetName(token);
                            ok = currentNode->Insert(container);
                            if (ok) {
                                currentNode = container;
                            }
                        }
                    }
                    if (ok) {
                        ok = StringHelper::Copy(token, nextToken);
                    }
                }
            }

            if (ok) {
                ok = created;
            }

            if (HeapManager::Free(reinterpret_cast<void*&>(token))) {

            }
            if (HeapManager::Free(reinterpret_cast<void*&>(nextToken))) {

            }
        }
    }
    return ok;

}

bool ReferenceContainer::Delete(Reference ref) {
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::REMOVE, ref);
    ReferenceContainer result;
    //Locking is already done inside the Find
    Find(result, filter);
    return (result.Size() > 0u);
}

bool ReferenceContainer::IsContainer(const Reference &ref) const {
    ReferenceT<ReferenceContainer> test = ref;
    return test.IsValid();
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainer requires pointer to pointer casting*/
void ReferenceContainer::Find(ReferenceContainer &result,
                              ReferenceContainerFilter &filter) {
    int32 index = 0;
    bool ok = (Lock());
    if (ok && (list.ListSize() > 0u)) {
        if (filter.IsReverse()) {
            index = static_cast<int32>(list.ListSize()) - 1;
        }
        //The filter will be finished when the correct occurrence has been found (otherwise it will walk all the list)
        //lint -e{9007} no side-effects on the right of the && operator
        while ((!filter.IsFinished()) && ((filter.IsReverse() && (index > -1)) || ((!filter.IsReverse()) && (index < static_cast<int32>(list.ListSize()))))) {

            ReferenceContainerNode *currentNode = dynamic_cast<ReferenceContainerNode *>(list.ListPeek(static_cast<uint32>(index)));

            Reference currentNodeReference = currentNode->GetReference();
            //Check if the current node meets the filter criteria
            bool found = filter.Test(result, currentNodeReference);
            if (found) {
                //IsSearchAll() => all found nodes should be inserted in the output list
                //IsFinished() => that the desired occurrence of this object was found => add it to the output list
                /*lint -e{9007} filter.IsSearchAll() has no side effects*/
                if (filter.IsSearchAll() || filter.IsFinished()) {
                    if (result.Insert(currentNodeReference)) {
                        if (filter.IsRemove()) {
                            //Only delete the exact node index
                            if (list.ListDelete(currentNode)) {
                                //Given that the index will be incremented, but we have removed an element, the index should stay in the same position
                                if (!filter.IsReverse()) {
                                    index--;
                                }
                            }
                            else {
                                REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed StaticList::Delete()");
                            }
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed StaticList::Insert()");
                    }
                }
            }

            // no other stack waste!!
            if (filter.IsFinished()) {
                break;
            }

            /*lint -e{9007} filter.IsRecursive() has no side effects*/
            if ((IsContainer(currentNodeReference)) && filter.IsRecursive()) {
                ok = true;
                if (filter.IsStorePath()) {
                    ok = result.Insert(currentNodeReference);
                }

                if (ok) {
                    ReferenceT<ReferenceContainer> currentNodeContainer = currentNodeReference;
                    uint32 sizeBeforeBranching = result.list.ListSize();
                    UnLock();
                    currentNodeContainer->Find(result, filter);
                    if (Lock()) {
                        //Something was found if the result size has changed
                        if (sizeBeforeBranching == result.list.ListSize()) {
                            //Nothing found. Remove the stored path (which led to nowhere).
                            if (filter.IsStorePath()) {
                                LinkedListable *node = result.list.ListExtract(result.list.ListSize() - 1u);
                                delete node;
                            }
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed StaticList::Insert()");
                }
            }
            if (!filter.IsReverse()) {
                index++;
            }
            else {
                index--;
            }
        }
    }
    UnLock();
}

Reference ReferenceContainer::Find(const char8 * const path) {
    Reference ret;
    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE, path);
    ReferenceContainer resultSingle;
    Find(resultSingle, filter);
    if (resultSingle.Size() > 0u) {
        ret = resultSingle.Get(resultSingle.Size() - 1u);
    }
    return ret;
}

uint32 ReferenceContainer::Size() {
    uint32 size = 0u;
    if (Lock()) {
        size = list.ListSize();
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
    }
    UnLock();
    return size;
}

bool ReferenceContainer::Initialise(StructuredDataI &data) {

    // only one thread has to initialise.

    // Recursive initialization
    bool ok = true;
    uint32 numberOfChildren = data.GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfChildren) && (ok); i++) {
        const char8* childName = data.GetChildName(i);
        // case object
        if ((childName[0] == '+') || (childName[0] == '$')) {
            if (data.MoveRelative(childName)) {
                Reference newObject;
                ok = newObject.Initialise(data, false);
                if (ok) {
                    ok = (newObject.IsValid());
                    if (ok) {
                        newObject->SetName(childName);
                        ok = ReferenceContainer::Insert(newObject);
                    }
                    if (ok) {
                        ok = data.MoveToAncestor(1u);
                    }
                }
            }
            else {
                //TODO error
                ok = false;
            }
        }
    }
    return ok;
}

bool ReferenceContainer::ExportData(StructuredDataI & data) {

    // no need to lock
    const char8 * objName = GetName();
    bool ret = data.CreateRelative(objName);
    if (ret) {
        const ClassProperties *properties = GetClassProperties();
        ret = (properties != NULL);
        if (ret) {
            ret = data.Write("Class", properties->GetName());
            uint32 numberOfChildren = Size();
            for (uint32 i = 0u; (i < numberOfChildren) && (ret); i++) {
                Reference child = Get(i);
                ret = child.IsValid();
                if (ret) {
                    if (ret) {
                        ret = child->ExportData(data);
                    }
                }
            }
        }
        if (!data.MoveToAncestor(1u)) {
            ret = false;
        }
    }
    return ret;
}

bool ReferenceContainer::Lock() {
    return (mux.FastLock(muxTimeout) == ErrorManagement::NoError);
}

void ReferenceContainer::UnLock() {
    mux.FastUnLock();
}

CLASS_REGISTER(ReferenceContainer, "1.0")

}

