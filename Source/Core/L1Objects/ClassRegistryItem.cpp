/**
 * @file ClassRegistryItem.cpp
 * @brief Source file for class ClassRegistryItem
 * @date Aug 4, 2015
 * @author aneto
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
 * the class ClassRegistryItem (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryDatabase.h"
#include "ClassRegistryItem.h"
#include "FastPollingMutexSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*lint -e{9141} global declaration but only used to support the class implementation.
 * The symbol is not exported (static). This could also be replaced by an anonymous namespace.
 */
static FastPollingMutexSem classRegistryItemMuxSem;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ClassRegistryItem::ClassRegistryItem() :
        LinkedListable(), classProperties(), heap() {
    numberOfInstances = 0u;
    loadableLibrary = NULL_PTR(LoadableLibrary *);
    objectBuildFn = NULL_PTR(ObjectBuildFn *);
}

ClassRegistryItem::ClassRegistryItem(const ClassProperties &clProperties, const ObjectBuildFn * const objBuildFn) :
        LinkedListable(), heap() {
    numberOfInstances = 0u;
    classProperties = clProperties;
    loadableLibrary = NULL_PTR(LoadableLibrary *);
    objectBuildFn = objBuildFn;
    ClassRegistryDatabase::Instance().Add(this);
}

/*lint -e{1551} no exception should be thrown. Only reason is if the pointers are messed-up
 * by some racing condition or similar. Should not happen as the only user of this class
 * is the ClassRegistryDatabase.*/
ClassRegistryItem::~ClassRegistryItem() {
    const LoadableLibrary *loader = loadableLibrary;
    /*lint -e{534} if is missing. This will have to be sent to the logger. TODO*/
    ClassRegistryDatabase::Instance().Delete(this);
    if (loader != NULL) {
        delete loader;
    }
    loadableLibrary = NULL_PTR(LoadableLibrary *);
}

void ClassRegistryItem::GetClassPropertiesCopy(ClassProperties &destination) const {
    destination = classProperties;
}

const ClassProperties *ClassRegistryItem::GetClassProperties() const {
    return &classProperties;
}

void ClassRegistryItem::IncrementNumberOfInstances() {
    if (classRegistryItemMuxSem.FastLock() == NoError) {
        numberOfInstances++;
    }
    classRegistryItemMuxSem.FastUnLock();
}

void ClassRegistryItem::DecrementNumberOfInstances() {
    if (classRegistryItemMuxSem.FastLock() == NoError) {
        numberOfInstances--;
    }
    classRegistryItemMuxSem.FastUnLock();
}

uint32 ClassRegistryItem::GetNumberOfInstances() const {
    return numberOfInstances;
}

void ClassRegistryItem::SetHeap(const Heap& h) {
    heap = h;
}

const LoadableLibrary *ClassRegistryItem::GetLoadableLibrary() const {
    return loadableLibrary;
}

void ClassRegistryItem::SetLoadableLibrary(const LoadableLibrary * const lLibrary) {
    this->loadableLibrary = lLibrary;
}

const ObjectBuildFn *ClassRegistryItem::GetObjectBuildFunction() const {
    return objectBuildFn;
}

void ClassRegistryItem::FreeObject(void *&obj) {
    heap.Free(obj);
}
