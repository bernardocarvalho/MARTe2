/**
 * @file MultiThreadService.cpp
 * @brief Source file for class MultiThreadService
 * @date 02/09/2016
 * @author Filippo Sartori
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
 * the class MultiThreadService (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MultiThreadService.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MultiThreadService::MultiThreadService(EmbeddedServiceMethodBinderI &binder) :
        EmbeddedServiceI(),
        method(binder) {
    numberOfPoolThreads = 1u;
}

/*lint -e{1551} the only reason why this could throw an exception is if
 * the callback method throws an exception while stopping (but this is not
 * caught anyway). */
MultiThreadService::~MultiThreadService() {
    ErrorManagement::ErrorType err = MultiThreadService::Stop();
    if (!err.ErrorsCleared()) {
        REPORT_ERROR(err, "Could not Stop all the EmbeddedThreadI instances.");
    }
}

bool MultiThreadService::Initialise(StructuredDataI &data) {
    threadPool.SetName("ThreadPool");
    ErrorManagement::ErrorType err = EmbeddedServiceI::Initialise(data);
    if (err.ErrorsCleared()) {
        err.parametersError = !data.Read("NumberOfPoolThreads", numberOfPoolThreads);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPoolThreads has to be specified.");
        }
    }

    return err;
}

ErrorManagement::ErrorType MultiThreadService::Start() {
    ErrorManagement::ErrorType err;
    err.illegalOperation = (threadPool.Size() > 0u);
    bool errorsCleared = err.ErrorsCleared();
    while ((threadPool.Size() < numberOfPoolThreads) && (errorsCleared)) {
        ReferenceT<EmbeddedThread> thread(new (NULL) EmbeddedThread(method));
        err.fatalError = !thread.IsValid();
        if (err.ErrorsCleared()) {
            thread->SetPriorityClass(GetPriorityClass());
            thread->SetPriorityLevel(GetPriorityLevel());
            thread->SetCPUMask(GetCPUMask());
            thread->SetTimeout(GetTimeout());
            err = thread->Start();
        }
        if (err.ErrorsCleared()) {
            err.fatalError = !threadPool.Insert(thread);
        }
        errorsCleared = err.ErrorsCleared();
    }
    return err;
}

ErrorManagement::ErrorType MultiThreadService::Stop() {
    ErrorManagement::ErrorType err;
    uint32 i;
    for (i = 0u; i < threadPool.Size(); i++) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
        if (thread.IsValid()) {
            err = thread->Stop();
            if (!err.ErrorsCleared()) {
                uint32 threadNumber = i;
                REPORT_ERROR_PARAMETERS(err, "Could not Stop EmbeddedThreadI(%d)", threadNumber)
            }
        }
    }
    // perform kill if necessary
    for (i = 0u; i < threadPool.Size(); i++) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
        if (thread.IsValid()) {
            if (thread->GetStatus() != EmbeddedThreadI::OffState) {
                uint32 threadNumber = i;
                err = thread->Stop();
                if (!err.ErrorsCleared()) {
                    REPORT_ERROR_PARAMETERS(err, "Could not Kill EmbeddedThreadI(%d)", threadNumber)
                }
                else {
                    REPORT_ERROR_PARAMETERS(err, "Killed EmbeddedThreadI(%d)", threadNumber)
                }
            }
        }
    }
    // remove dead threads
    threadPool.CleanUp();
    if (err.ErrorsCleared()) {
        // some service died hard
        err.timeout = (threadPool.Size() > 0u);
    }

    return err;

}

EmbeddedThreadI::States MultiThreadService::GetStatus(const uint32 threadIdx) {
    EmbeddedThreadI::States status = EmbeddedThreadI::OffState;
    if (threadIdx < threadPool.Size()) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            status = thread->GetStatus();
        }
    }

    return status;
}

uint32 MultiThreadService::GetNumberOfPoolThreads() const {
    return numberOfPoolThreads;
}

void MultiThreadService::SetNumberOfPoolThreads(const uint32 numberOfPoolThreadsIn) {
    if (threadPool.Size() == 0u) {
        numberOfPoolThreads = numberOfPoolThreadsIn;
    }
}

void MultiThreadService::SetTimeout(const TimeoutType & msecTimeoutIn) {
    EmbeddedServiceI::SetTimeout(msecTimeoutIn);
    uint32 i;
    for (i = 0u; i < threadPool.Size(); i++) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
        if (thread.IsValid()) {
            thread->SetTimeout(msecTimeoutIn);
        }
    }
}

void MultiThreadService::SetPriorityClass(const Threads::PriorityClassType priorityClassIn) {
    bool allStop = true;
    uint32 i;
    for (i = 0u; (i < threadPool.Size()) && (allStop); i++) {
        allStop = (GetStatus(i) == EmbeddedThreadI::OffState);
    }
    if (allStop) {
        EmbeddedServiceI::SetPriorityClass(priorityClassIn);
        for (i = 0u; (i < threadPool.Size()) && (allStop); i++) {
            ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
            if (thread.IsValid()) {
                thread->SetPriorityClass(priorityClassIn);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Priority class cannot be changed if the service is running");
    }
}

void MultiThreadService::SetPriorityLevel(const uint8 priorityLevelIn) {
    bool allStop = true;
    uint32 i;
    for (i = 0u; (i < threadPool.Size()) && (allStop); i++) {
        allStop = (GetStatus(i) == EmbeddedThreadI::OffState);
    }
    if (allStop) {
        EmbeddedServiceI::SetPriorityLevel(priorityLevelIn);
        for (i = 0u; (i < threadPool.Size()) && (allStop); i++) {
            ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
            if (thread.IsValid()) {
                thread->SetPriorityLevel(priorityLevelIn);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Priority level cannot be changed if the service is running");
    }
}

void MultiThreadService::SetCPUMask(const ProcessorType& cpuMaskIn) {
    bool allStop = true;
    uint32 i;
    for (i = 0u; (i < threadPool.Size()) && (allStop); i++) {
        allStop = (GetStatus(i) == EmbeddedThreadI::OffState);
    }
    if (allStop) {
        EmbeddedServiceI::SetCPUMask(cpuMaskIn);
        for (i = 0u; (i < threadPool.Size()) && (allStop); i++) {
            ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
            if (thread.IsValid()) {
                thread->SetCPUMask(cpuMaskIn);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "CPUMask cannot be changed if the service is running");
    }
}

}
