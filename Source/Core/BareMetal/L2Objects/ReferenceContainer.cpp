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
 *
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

#include "ClassRegistryItemT.h"
#include "ReferenceContainer.h"
#include "ReferenceContainerNode.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceT.h"
#include "ErrorManagement.h"
#include "StringHelper.h"
#include "ReferenceContainerFilterObjectName.h"
#include "MemoryOperationsHelper.h"
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

ReferenceContainer::ReferenceContainer(ReferenceContainer &copy) :
        Object(copy) {
    SetTimeout(copy.GetTimeout());
    uint32 nChildren = copy.Size();
    for (uint32 i = 0u; i < nChildren; i++) {
        Reference toInsert = copy.Get(i);
        if (!Insert(toInsert)) {
            //TODO
        }
    }
}

ReferenceContainer& ReferenceContainer::operator =(ReferenceContainer &copy) {
    SetTimeout(copy.GetTimeout());
    if (this != &copy) {
        uint32 nChildren = copy.Size();
        for (uint32 i = 0u; i < nChildren; i++) {
            Reference toInsert = copy.Get(i);
            if (!Insert(toInsert)) {
                //TODO
            }
        }
    }
    return *this;
}

/*lint -e{929} -e{925} the current implementation of the ReferenceContainer requires pointer to pointer casting*/
Reference ReferenceContainer::Get(const uint32 idx) {
    Reference ref;
    if (Lock()) {
        if (idx < list.ListSize()) {
            ReferenceContainerNode *node = (list.ListPeek(idx));
            if (node != NULL) {
                ref = node->GetReference();
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Warning, "ReferenceContainer: input greater than the list size.");
        }
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

void ReferenceContainer::CleanUp() {

    uint32 numberOfElements;
    if (!Lock()) {
        REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
    }
    numberOfElements = list.ListSize() + purgeList.ListSize();
    UnLock();

    //flat recursion due to avoid stack waste!!
    for (uint32 i = 0u; i < numberOfElements; i++) {
        if (!Lock()) {
            REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
        }
        if (purgeList.ListSize() >= numberOfElements) {
            UnLock();
            break;
        }
        //extract the element from the list
        ReferenceContainerNode *node = list.ListExtract(0u);
        if (node != NULL) {
            purgeList.ListInsert(node);
        }
        UnLock();
    }

    for (uint32 i = 0u; i < numberOfElements; i++) {
        if (!Lock()) {
            REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
        }
        if (purgeList.ListSize() == 0u) {
            UnLock();
            break;
        }

        ReferenceContainerNode * node = purgeList.ListExtract(0u);
        UnLock();

        ReferenceT<ReferenceContainer> element;
        if (node != NULL) {
            element = node->GetReference();
        }

        if (element.IsValid()) {
            element->CleanUp();
        }
        //extract and delete the element from the list
        if (node != NULL) {
            delete node;
        }
    }
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
        ReferenceContainer* currentNode = this;
        uint32 pathSize = StringHelper::Length(path);
        char8 *token = reinterpret_cast<char8*>(HeapManager::Malloc(static_cast<uint32>(sizeof(char8) * pathSize)));
        //   char8 *nextToken = reinterpret_cast<char8*>(HeapManager::Malloc(static_cast<uint32>(sizeof(char8) * pathSize)));
        MemoryOperationsHelper::Set(token, '\0', pathSize);
        //   MemoryOperationsHelper::Set(nextToken, '\0', pathSize);

        const char8* toTokenize = path;
        const char8* next = StringHelper::TokenizeByChars(toTokenize, ".", token);
        toTokenize = next;

        while ((StringHelper::Length(token) > 0u) && (ok)) {
            //Check if a node with this name already exists
            bool found = false;
            Reference foundReference;
            uint32 i;
            for (i = 0u; (i < currentNode->Size()) && (!found); i++) {
                foundReference = currentNode->Get(i);
                found = (StringHelper::Compare(foundReference->GetName(), token) == 0);
            }

            if (found) {
                currentNode = dynamic_cast<ReferenceContainer*>(foundReference.operator->());
                // if it is a leaf exit (and return false)
                if (currentNode == NULL) {
                    ok = false;
                }
            }
            else {
                ReferenceT<ReferenceContainer> container(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                container->SetName(token);
                ok = currentNode->Insert(container);
                if (ok) {
                    currentNode = container.operator->();
                }
            }
            // take the next token

            next = StringHelper::TokenizeByChars(toTokenize, ".", token);
            toTokenize = next;
        }

        // insert the reference
        if (ok) {
            ok = currentNode->Insert(ref);
        }

        if (HeapManager::Free(reinterpret_cast<void*&>(token))) {

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

bool ReferenceContainer::Delete(const char8 * const path) {
    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::REMOVE, path);
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
    bool ok = Lock();
    if (ok) {
        if (list.ListSize() > 0u) {
            if (filter.IsReverse()) {
                index = static_cast<int32>(list.ListSize()) - 1;
            }
            //The filter will be finished when the correct occurrence has been found (otherwise it will walk all the list)
            //lint -e{9007} no side-effects on the right of the && operator
            while ((!filter.IsFinished()) && ((filter.IsReverse() && (index > -1)) || ((!filter.IsReverse()) && (index < static_cast<int32>(list.ListSize()))))) {

                ReferenceContainerNode *currentNode = (list.ListPeek(static_cast<uint32>(index)));

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
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ReferenceContainer: Failed FastLock()");
    }

    UnLock();
}

Reference ReferenceContainer::Find(const char8 * const path,
                                   const bool recursive) {
    Reference ret;
    uint32 mode = ReferenceContainerFilterMode::SHALLOW;
    if (recursive) {
        mode = ReferenceContainerFilterMode::RECURSIVE;
    }
    ReferenceContainerFilterObjectName filter(1, mode, path);
    ReferenceContainer resultSingle;
    Find(resultSingle, filter);
    if (resultSingle.Size() > 0u) {
        ret = resultSingle.Get(resultSingle.Size() - 1u);
    }
    return ret;
}

Reference ReferenceContainer::Find(const char8 * const path,
                                   const Reference current,
                                   bool relative) {
    ReferenceT<ReferenceContainer> domain = current;
    bool isSearchDomain = current.IsValid();
    uint32 backSteps = 0u;
    bool ok = (path != NULL);
    if (isSearchDomain && ok) {
        while (path[backSteps] == ':') {
            backSteps++;
        }
        isSearchDomain = (backSteps > 0u);
        if (isSearchDomain) {
            uint32 stepsCounter = backSteps;
            // search the current remembering the path
            ReferenceContainerFilterReferences filterRef(1, ReferenceContainerFilterMode::PATH, current);
            ReferenceContainer resultPath;
            ReferenceContainer::Find(resultPath, filterRef);
            for (uint32 i = 0u; i < resultPath.Size(); i++) {
                Reference test = resultPath.Get((resultPath.Size() - i) - 1u);
                if (stepsCounter == 0u) {
                    break;
                }
                if (test.IsValid()) {
                    ok = Lock();
                    if (ok) {
                        /*lint -e{613} cheking of NULL pointer done before entering here. */
                        if (test->IsDomain()) {
                            domain = test;
                            stepsCounter--;
                        }
                    }
                    UnLock();
                }
            }

            if (stepsCounter > 0u) {
                REPORT_ERROR(ErrorManagement::Warning, "Find: Too many back steps in the path. The searching will start from the root");
                isSearchDomain = false;
            }
        }

    }
    // now search from the domain forward
    Reference ret;
    if (ok) {
        if (StringHelper::Length(&path[backSteps]) > 0u) {
            ReferenceContainerFilterObjectName filterName(1, ReferenceContainerFilterMode::SHALLOW, &path[backSteps]);
            ReferenceContainer resultSingle;

            if (isSearchDomain) {
                if (domain.IsValid()) {
                    // already safe
                    domain->Find(resultSingle, filterName);
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Find: Invalid domain");
                }
            }
            else {
                // search from the beginning
                if (relative) {
                    domain = current;
                    domain->Find(resultSingle, filterName);
                }
                else {
                    ReferenceContainer::Find(resultSingle, filterName);
                }
            }

            ok = (resultSingle.Size() > 0u);
            if (ok) {
                //Invalidate move to leafs
                ret = resultSingle.Get(resultSingle.Size() - 1u);
            }
        }
        else {
            ret = domain;
        }
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
        ok = (childName != NULL);
        if (ok) {
            // case object
            if ((childName[0] == '+') || (childName[0] == '$')) {
                if (data.MoveRelative(childName)) {
                    Reference newObject;
                    ok = newObject.Initialise(data, false);
                    if (ok) {
                        ok = (newObject.IsValid());
                        if (ok) {
                            if (childName[0] == '$') {
                                newObject->SetDomain(true);
                            }
                            ok = ReferenceContainer::Insert(newObject);
                        }
                        if (ok) {
                            ok = data.MoveToAncestor(1u);
                        }
                    }
                    else {
                        const uint32 maxSize = 64u;
                        char8 errorMsg[maxSize];
                        errorMsg[0] = '\0';
                        bool ret = StringHelper::Concatenate(&errorMsg[0], "Failed to Initialise object with name ");
                        uint32 sizeLeft = 0u;
                        if (ret) {
                            sizeLeft = maxSize - StringHelper::Length(&errorMsg[0]);
                            ret = StringHelper::ConcatenateN(&errorMsg[0], childName, sizeLeft);
                        }
                        if (ret) {
                            REPORT_ERROR(ErrorManagement::FatalError, &errorMsg[0]);
                        }
                    }
                }
                else {
                    ok = false;
                }
            }
        }
    }
    return ok;
}

bool ReferenceContainer::ExportData(StructuredDataI & data) {

    // no need to lock
    const char8 * objName = GetName();
    uint32 objNameLength = StringHelper::Length(objName);
    //To include $ or +
    objNameLength += 1u;
    char8 *objNameToCreate = reinterpret_cast<char8 *>(HeapManager::Malloc(objNameLength));
    objNameToCreate[0] = (IsDomain()) ? ('$') : ('+');
    bool ret = StringHelper::Copy(&objNameToCreate[1], objName);

    if (ret) {
        ret = data.CreateRelative(objNameToCreate);
        if (ret) {
            ret = HeapManager::Free(reinterpret_cast<void*&>(objNameToCreate));
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
                            ret = child->ExportData(data);
                        }
                    }
                }
                if (!data.MoveToAncestor(1u)) {
                    ret = false;
                }
            }
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

