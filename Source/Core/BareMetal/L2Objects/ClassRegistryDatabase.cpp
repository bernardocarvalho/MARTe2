/**
 * @file ClassRegistryDatabase.cpp
 * @brief Source file for class ClassRegistryDatabase
 * @date 04/08/2015
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
 * the class ClassRegistryDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryItem.h"
#include "HeapManager.h"
#include "Object.h"
#include "StringHelper.h"
#include "MemoryOperationsHelper.h"
#include "LoadableLibrary.h"
#include "ClassRegistryItem.h"
#include "ClassRegistryDatabase.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ClassRegistryDatabase *ClassRegistryDatabase::Instance() {
    static ClassRegistryDatabase *instance = NULL_PTR(ClassRegistryDatabase *);
    if (instance == NULL_PTR(ClassRegistryDatabase *)) {
        instance = new ClassRegistryDatabase();
        GlobalObjectsDatabase::Instance()->Add(instance, NUMBER_OF_GLOBAL_OBJECTS - 2u);
    }
    return instance;
}

ClassRegistryDatabase::ClassRegistryDatabase() {
    classUniqueIdentifier = 0u;
}

ClassRegistryDatabase::~ClassRegistryDatabase() {
    //automatic LinkedListHolder::CleanUp
}

void ClassRegistryDatabase::Add(ClassRegistryItem * const p) {
    if (p != NULL) {

        if (!Lock()) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
        }

        p->SetUniqueIdentifier(classUniqueIdentifier);

        classDatabase.ListInsert(p, classUniqueIdentifier);
        classUniqueIdentifier = classUniqueIdentifier + 1u;

        UnLock();
    }
}

ClassRegistryItem *ClassRegistryDatabase::Find(const char8 *className) {
    const uint32 maxSize = 129u;
    char8 dllName[maxSize];
    dllName[0] = '\0';
    bool found = false;

    //Check for the string pattern dllName::className
    const char8 *classOnlyPartName = StringHelper::SearchString(className, "::");
    uint32 size = 0u;
    if (classOnlyPartName != NULL) {
        size = static_cast<uint32>(StringHelper::SearchIndex(className, "::"));
    }
    else {
        size = static_cast<uint32>(StringHelper::Length(className));
    }
    if (size > (maxSize - 1u)) {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "ClassName size was truncated");
        size = (maxSize - 1u);
    }
    if (StringHelper::CopyN(&(dllName[0]), className, size)) {
        dllName[size] = '\0';
    }
    if (classOnlyPartName != NULL) {
        className = &classOnlyPartName[2];
    }

    ClassRegistryItem *registryItem = NULL_PTR(ClassRegistryItem *);
    if (className != NULL) {
        if (!Lock()) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
        }
        uint32 i;
        uint32 databaseSize = classDatabase.ListSize();
        for (i = 0u; (i < databaseSize) && (!found); i++) {
            ClassRegistryItem *p = classDatabase.ListPeek(i);
            if (p != NULL) {
                const ClassProperties *classProperties = p->GetClassProperties();
                if (classProperties != NULL_PTR(ClassProperties *)) {
                    if (StringHelper::Compare(classProperties->GetName(), className) == 0) {
                        registryItem = p;
                        found = true;
                    }
                }
            }
        }
        //Must unlock as the loader->Open below might trigger the registration of new classes which will call on the
        //Add method and thus Lock the database.
        UnLock();
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ClassRegistryDatabase: NULL pointer in input");
    }
    //registryItem still not found. Try to look inside the dll (if it exists)
    /*lint -e{593} this pointer is freed by the registry item when it is destructed*/
    if (!found) {
        uint32 dllNameSize = StringHelper::Length(&(dllName[0]));
        uint32 fullSize = dllNameSize + 5u;
        /*lint -e{925} pointer to pointer required due to Malloc implementation*/
        char8 *fullName = static_cast<char8 *>(HeapManager::Malloc(fullSize));

        LoadableLibrary *loader = new LoadableLibrary();

        uint32 i = 0u;
        bool dllOpened = false;
        //Check for all known operating system extensions.
        while ((operatingSystemDLLExtensions[i] != 0) && (!dllOpened)) {
            if (MemoryOperationsHelper::Set(fullName, '\0', fullSize)) {
                if (StringHelper::ConcatenateN(fullName, &(dllName[0]), dllNameSize)) {
                    const char8 *extension = operatingSystemDLLExtensions[i];
                    char8 *fullNameWithDllName = &fullName[dllNameSize];
                    if (StringHelper::ConcatenateN(fullNameWithDllName, extension, 4u)) {
                        dllOpened = loader->Open(fullName);
                    }
                }
            }
            i++;
        }
        //If the dll was successfully opened than it is likely that more classes were registered
        //in the database. Search again.
        if (dllOpened) {
            if (!Lock()) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
            }
            uint32 databaseSize = classDatabase.ListSize();
            for (i = 0u; (i < databaseSize) && (!found); i++) {
                ClassRegistryItem *p = classDatabase.ListPeek(i);
                if (p != NULL) {
                    const ClassProperties *classProperties = p->GetClassProperties();
                    if (classProperties != NULL_PTR(ClassProperties *)) {
                        if (StringHelper::Compare(classProperties->GetName(), className) == 0) {
                            registryItem = p;
                            registryItem->SetLoadableLibrary(loader);
                            found = true;
                        }
                    }
                }
            }
            UnLock();
        }
        //Not found...
        if (!found) {
            delete loader;
        }
        if (!HeapManager::Free(reinterpret_cast<void*&>(fullName))) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ClassRegistryDatabase: Free failed");
        }
    }

    return registryItem;
}

ClassRegistryItem *ClassRegistryDatabase::FindTypeIdName(const char8 * const typeidName) {
    ClassRegistryItem *registryItem = NULL_PTR(ClassRegistryItem *);
    if (!Lock()) {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
    }
    bool found = false;
    if (typeidName != NULL) {
        uint32 i;
        uint32 databaseSize = classDatabase.ListSize();
        for (i = 0u; (i < databaseSize) && (!found); i++) {
            ClassRegistryItem *p = classDatabase.ListPeek(i);
            if (p != NULL) {
                const ClassProperties *classProperties = p->GetClassProperties();
                if (classProperties != NULL_PTR(ClassProperties *)) {
                    if (StringHelper::Compare(classProperties->GetTypeIdName(), typeidName) == 0) {
                        registryItem = p;
                        found = true;
                    }
                }
            }
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ClassRegistryDatabase: NULL pointer in input");
    }

    UnLock();
    return registryItem;
}

uint32 ClassRegistryDatabase::GetSize() {
    uint32 size = 0u;
    if (Lock()) {
        size = classDatabase.ListSize();
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
    }
    UnLock();
    return size;
}

const ClassRegistryItem *ClassRegistryDatabase::Peek(const uint32 &idx) {
    ClassRegistryItem *item = NULL_PTR(ClassRegistryItem *);
    if (Lock()) {
        item = classDatabase.ListPeek(idx);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
    }
    UnLock();
    return item;
}

const char8 * const ClassRegistryDatabase::GetClassName() const {
    return "ClassRegistryDatabase";
}

void ClassRegistryDatabase::CleanUp() {
    classDatabase.CleanUp();
}

bool ClassRegistryDatabase::Lock() {
    return (mux.FastLock() == ErrorManagement::NoError);
}

void ClassRegistryDatabase::UnLock() {
    mux.FastUnLock();
}

}
