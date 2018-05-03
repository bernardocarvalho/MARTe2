/**
 * @file MARTeApp.cpp
 * @brief Source file for class MARTeApp
 * @date 04/04/2018
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
 * the class MARTeApp (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/**
 * @file A generic MARTe application which uses a Bootstrap to read the input parameters and to setup the execution environment.
 * Note that the Loader to be used is one of the user parameters, so that this main should be sufficiently generic for most use-cases.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Bootstrap.h"
#include "Loader.h"
#include "ObjectRegistryDatabase.h"
#include "ReferenceT.h"
#include "StreamI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static MARTe::Bootstrap bootstrap;

void MainErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo, const char * const errorDescription) {
    MARTe::StreamString errorCodeStr;
    MARTe::ErrorManagement::ErrorCodeToStream(errorInfo.header.errorType, errorCodeStr);
    MARTe::StreamString err;
    err.Printf("[%s - %s:%d]: %s", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorDescription);
    bootstrap.Printf(err.Buffer());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
int main(int argc, char **argv) {
    using namespace MARTe;
    SetErrorProcessFunction(&MainErrorProcessFunction);

    ConfigurationDatabase loaderParameters;
    StreamI *configurationStream = NULL_PTR(StreamI *);

    ErrorManagement::ErrorType ret = bootstrap.ReadParameters(argc, argv, loaderParameters);
    if (ret) {
        ret = bootstrap.GetConfigurationStream(loaderParameters, configurationStream);
        if (ret) {
            ret.fatalError = (configurationStream == NULL_PTR(StreamI *));
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not GetConfigurationStream.");
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not ReadParameters.");
    }

    StreamString loaderClass;
    if (ret) {
        ret.fatalError = !loaderParameters.Read("Loader", loaderClass);
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Loader not specified");
        }
    }

    //Try to instantiate a new loader.
    ReferenceT<Loader> loaderRef;
    if (ret) {
        loaderRef = Reference(loaderClass.Buffer(), GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (loaderRef.IsValid()) {
            ret = loaderRef->Configure(loaderParameters, *configurationStream);
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not Initialise the loader with name %s", loaderClass.Buffer());
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not instantiate loader with name %s", loaderClass.Buffer());
            ret = ErrorManagement::FatalError;
        }
    }
    if (ret) {
        ret = loaderRef->Start();
    }
    if (ret) {
        //Expects to run until an operating specific event exits
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Application starting");
        ret = bootstrap.Run();
        REPORT_ERROR_STATIC(ret, "Application terminated");
    }
    if (ret) {
        ret = loaderRef->Stop();
    }
    MARTe::ObjectRegistryDatabase::Instance()->Purge();

    return 0;
}

