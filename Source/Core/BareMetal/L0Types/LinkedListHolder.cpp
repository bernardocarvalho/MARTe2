/**
 * @file LinkedListHolder.cpp
 * @brief Source file for class LinkedListHolder
 * @date 06/08/2015
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
 * the class LinkedListHolder (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "LinkedListHolder.h"
#include "SearchFilter.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

void LinkedListHolder::CleanUp() {
    LinkedListable *p = llhRoot.Next();
    for (uint32 i = 0u; i < llhSize; i++) {
        if (p == NULL) {
            break;
        }
        LinkedListable *q = p;
        p = p->Next();
        delete q;
    }
    Reset();
}

void LinkedListHolder::Reset() {
    llhRoot.SetNext(NULL_PTR(LinkedListable *));
    llhTail = NULL_PTR(LinkedListable *);
    llhSize = 0u;
}

LinkedListHolder::LinkedListHolder(const bool destroyIn) {
    llhSize = 0u;
    destroy = destroyIn;
    llhRoot.SetNext(NULL_PTR(LinkedListable *));
    llhTail = NULL_PTR(LinkedListable *);
}

/*lint -e{1551} the only reason why this could throw an exception is if
 * due to some racing condition, while destroying the list, the pointer
 * to the next element was to be destructed by some other thread. */
LinkedListHolder::~LinkedListHolder() {
    if (destroy) {
        CleanUp();
    }
    else{
        Reset();
    }
}

LinkedListable *LinkedListHolder::List() {
    return llhRoot.Next();
}

uint32 LinkedListHolder::ListSize() const {
    return llhSize;
}

/*lint -e{429} . Justification: A possible pointer to allocated memory will be freed by LinkedListHolder::CleanUp in the destructor.*/
void LinkedListHolder::ListInsert(LinkedListable * const p) {
    if (p != NULL_PTR(LinkedListable *)) {

        llhSize += p->Size();
        llhRoot.Insert(p);
        llhTail = NULL_PTR(LinkedListable *);
    }
}

/*lint -e{429} . Justification: A possible pointer to allocated memory will be freed by LinkedListHolder::CleanUp in the destructor.*/
void LinkedListHolder::ListInsert(LinkedListable * const p,
                                  SortFilter * const sorter) {
    if (p != NULL_PTR(LinkedListable *)) {

        llhSize += p->Size();
        llhRoot.Insert(p, sorter);
        llhTail = NULL_PTR(LinkedListable *);
    }
}

void LinkedListHolder::ListInsert(LinkedListable * const q,
                                  uint32 index) {
    if (q != NULL_PTR(LinkedListable *)) {

        LinkedListable *p = &llhRoot;
        while ((p->Next() != NULL_PTR(LinkedListable *)) && (index > 0u)) {
            p = p->Next();
            index--;
        }
        llhSize += q->Size();
        p->Insert(q);
        llhTail = NULL_PTR(LinkedListable *);
    }
}

void LinkedListHolder::ListAdd(LinkedListable * const p) {
    if (p != NULL_PTR(LinkedListable *)) {

        llhSize++;
        llhRoot.Add(p);
        llhTail = NULL_PTR(LinkedListable *);
    }
}

void LinkedListHolder::ListAddL(LinkedListable * const p) {
    if (p != NULL_PTR(LinkedListable *)) {

        llhSize += p->Size();
        llhRoot.AddL(p);
        llhTail = NULL_PTR(LinkedListable *);
    }
}

bool LinkedListHolder::ListSearch(const LinkedListable * const p) {

    return (llhRoot.Next() == NULL_PTR(LinkedListable *)) ? false : llhRoot.Next()->Search(p);
}

LinkedListable *LinkedListHolder::ListSearch(SearchFilter * const filter) {

    return (llhRoot.Next() == NULL_PTR(LinkedListable *)) ? NULL_PTR(LinkedListable *) : llhRoot.Next()->Search(filter);
}

bool LinkedListHolder::ListExtract(LinkedListable * const p) {

    bool ret = false;

    if (llhRoot.Extract(p)) {
        llhSize--;
        ret = true;
    }
    llhTail = NULL_PTR(LinkedListable *);

    return ret;
}

LinkedListable *LinkedListHolder::ListExtract(SearchFilter * const filter) {
    LinkedListable *p = llhRoot.Extract(filter);
    if (p != NULL_PTR(LinkedListable *)) {
        llhSize--;
    }
    llhTail = NULL_PTR(LinkedListable *);
    return p;
}

bool LinkedListHolder::ListDelete(LinkedListable * const p) {
    bool ret = false;

    if (llhRoot.Delete(p)) {
        llhSize--;
        ret = true;
    }
    llhTail = NULL_PTR(LinkedListable *);
    return ret;
}

bool LinkedListHolder::ListDelete(SearchFilter * const filter) {
    uint32 deleted = llhRoot.Delete(filter);
    llhSize -= deleted;
    llhTail = NULL_PTR(LinkedListable *);
    return (deleted > 0u);
}

bool LinkedListHolder::ListSafeDelete(SearchFilter * const filter) {
    uint32 deleted = 0u;
    if (filter != NULL_PTR(SearchFilter *)) {

        LinkedListable *p = List();
        while (p != NULL_PTR(LinkedListable *)) {
            if (filter->Test(p)) {
                if (ListExtract(p)) {
                    delete p;
                    // p->next may not be a safe pointer so assume the worst and start over again
                    p = List();
                    deleted++;
                }
                else {
                    p = p->Next();
                }
            }
            else {
                p = p->Next();
            }
        }
    }
    llhTail = NULL_PTR(LinkedListable *);

    return (deleted > 0u);
}

void LinkedListHolder::ListBSort(SortFilter * const sorter) {
    llhRoot.BSort(sorter);
    llhTail = NULL_PTR(LinkedListable *);
}

LinkedListable *LinkedListHolder::ListPeek(const uint32 index) {
    return (llhRoot.Next() == NULL_PTR(LinkedListable *)) ? (NULL_PTR(LinkedListable *)) : (llhRoot.Next()->Peek(index));
}

LinkedListable *LinkedListHolder::ListExtract(uint32 index) {

    LinkedListable *ret = NULL_PTR(LinkedListable *);
    LinkedListable *p = &llhRoot;
    while ((p != NULL_PTR(LinkedListable *)) && (index > 0u)) {
        p = p->Next();
        index--;
    }

    if (p != NULL_PTR(LinkedListable *)) {

        LinkedListable *q = p->Next();
        if (q != NULL_PTR(LinkedListable *)) {
            llhSize--;
            p->SetNext(q->Next());
            q->SetNext(NULL_PTR(LinkedListable *));
        }
        ret = q;

    }
    llhTail = NULL_PTR(LinkedListable *);
    return ret;
}

void LinkedListHolder::ListIterate(Iterator * const it) {
    if (llhRoot.Next() != NULL_PTR(LinkedListable *)) {
        llhRoot.Next()->Iterate(it);
    }
    llhTail = NULL_PTR(LinkedListable *);
}
}
