/**
 * @file RealTimeDataSourceDef.cpp
 * @brief Source file for class RealTimeDataSourceDef
 * @date 29/feb/2016
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
 * the class RealTimeDataSourceDef (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceDef.h"
#include "ReferenceT.h"
#include "RealTimeDataSourceDefRecord.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

//static const uint32 statesGranularity = 8u;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeDataSourceDef::RealTimeDataSourceDef() {
}

RealTimeDataSourceDef::~RealTimeDataSourceDef() {

    printf("\n!!!\n");

    //if (records != NULL) {
    //  delete [] records;
    // }
}

void RealTimeDataSourceDef::AddConsumer(const char8 *stateIn,
                                        ReferenceT<GAM> gam) {
    uint32 index;
    bool found = false;
    ReferenceT<RealTimeDataSourceDefRecord> record;
    uint32 numberOfStates = Size();
    for (index = 0u; (index < numberOfStates) && (!found); index++) {
        StreamString stateName = stateIn;
        record = Get(index);
        if (record.IsValid()) {
            if (stateName == record->GetStateName()) {
                found = true;
            }
        }
    }
    if (found) {
        record->AddConsumer(gam);
    }
    else {
        record = ReferenceT<RealTimeDataSourceDefRecord>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (record.IsValid()) {
            record->SetStateName(stateIn);
            record->AddConsumer(gam);
            if (!Insert(record)) {
                //TODO
            }
        }
    }
}

void RealTimeDataSourceDef::Addproducer(const char8 *stateIn,
                                        ReferenceT<GAM> gam) {

    uint32 index;
    bool found = false;
    ReferenceT<RealTimeDataSourceDefRecord> record;
    uint32 numberOfStates = Size();
    for (index = 0u; (index < numberOfStates) && (!found); index++) {
        StreamString stateName = stateIn;
        record = Get(index);
        if (record.IsValid()) {
            if (stateName == record->GetStateName()) {
                found = true;
            }
        }
    }
    if (found) {
        record->Addproducer(gam);
    }
    else {
        record = ReferenceT<RealTimeDataSourceDefRecord>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (record.IsValid()) {
            record->SetStateName(stateIn);
            record->Addproducer(gam);
            if (!Insert(record)) {
                //TODO
            }
        }
    }
}

uint32 RealTimeDataSourceDef::GetNumberOfConsumers(const char8 * stateIn) {
    uint32 index;
    uint32 ret = 0u;
    bool found = false;
    ReferenceT<RealTimeDataSourceDefRecord> record;
    uint32 numberOfStates = Size();
    for (index = 0u; (index < numberOfStates) && (!found); index++) {
        record = Get(index);
        if (record.IsValid()) {
            StreamString stateName = stateIn;
            if (stateName == record->GetStateName()) {
                found = true;
            }
        }
    }
    if (found) {
        ret = record->GetNumberOfConsumers();
    }
    return ret;

}

uint32 RealTimeDataSourceDef::GetNumberOfProducers(const char8 * stateIn) {
    uint32 index;
    uint32 ret = 0u;
    bool found = false;
    ReferenceT<RealTimeDataSourceDefRecord> record;
    uint32 numberOfStates = Size();
    for (index = 0u; (index < numberOfStates) && (!found); index++) {
        record = Get(index);
        if (record.IsValid()) {
            StreamString stateName = stateIn;
            if (stateName == record->GetStateName()) {
                found = true;
            }
        }
    }
    if (found) {
        ret = record->GetNumberOfProducers();
    }
    return ret;
}

bool RealTimeDataSourceDef::Verify() {

    bool ret = true;
    uint32 numberOfStates = Size();
    for (uint32 i = 0u; (i < numberOfStates) && (ret); i++) {
        ReferenceT<RealTimeDataSourceDefRecord> record = Get(i);
        if (record.IsValid()) {
            // no more than one producer for each state
            if (record->GetNumberOfProducers() > 1u) {
                ret = false;
            }
            if (record->GetNumberOfConsumers() == 0u) {
                //TODO Warning variable not consumed
            }
        }
    }
    return ret;
}
CLASS_REGISTER(RealTimeDataSourceDef, "1.0")

}

