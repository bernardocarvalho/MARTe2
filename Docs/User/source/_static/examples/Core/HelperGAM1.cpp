/**
 * @file HelperGAM1.cpp
 * @brief Source file for class HelperGAM1.cpp
 * @date 28.05.2019
 * @author Cristian Galperti
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class HelperGAM1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "HelperGAM1.h"
#include "math.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
HelperGAM1::HelperGAM1() {
    inputSignal = NULL_PTR(MARTe::uint32 *);
    outputSignal = NULL_PTR(MARTe::uint32 *);
    MARTe::StreamString a;
}

HelperGAM1::~HelperGAM1() {

}

bool HelperGAM1::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Could not Initialise the GAM");
    }
    /*
    if (ok) {
        ok = data.Read("Gain", gain);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The parameter Gain shall be set");
        }
    }
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "Parameter Gain set to %d", gain);
    }
    */

    if(ok){
        ok = data.Read("MdsTimeOffset", mdstimeoffset);
    }
    if(ok){
        ok = data.Read("MdsStartTime", mdsstarttime);
    }
    if(ok){
        ok = data.Read("MdsStopTime", mdsstoptime);
    }


    f1=f2=0.0;
    return ok;
}

bool HelperGAM1::Setup() {
    using namespace MARTe;



    //uint32 numberOfInputSignals = GetNumberOfInputSignals();
    //uint32 numberOfOutputSignals = GetNumberOfOutputSignals();


    /*
    bool ok = (numberOfInputSignals == numberOfOutputSignals);
    if (ok) {
        ok = (numberOfOutputSignals == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The number of input and output signals shall be equal to 1. numberOfInputSignals = %d numberOfOutputSignals = %d",
                         numberOfInputSignals, numberOfOutputSignals);
        }
    }
    if (ok) {
        TypeDescriptor inputSignalType = GetSignalType(InputSignals, 0u);
        TypeDescriptor outputSignalType = GetSignalType(OutputSignals, 0u);
        ok = (inputSignalType == outputSignalType);
        if (ok) {
            ok = (inputSignalType == UnsignedInteger32Bit);
        }
        if (!ok) {
            const char8 * const inputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(inputSignalType);
            const char8 * const outputSignalTypeStr = TypeDescriptor::GetTypeNameFromTypeDescriptor(outputSignalType);
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The type of the input and output signal shall be uint32. inputSignalType = %s outputSignalType = %s", inputSignalTypeStr,
                         outputSignalTypeStr);
        }
    }
    if (ok) {
        uint32 numberOfInputSamples = 0u;
        uint32 numberOfOutputSamples = 0u;
        ok = GetSignalNumberOfSamples(InputSignals, 0u, numberOfInputSamples);
        if (ok) {
            ok = GetSignalNumberOfSamples(OutputSignals, 0u, numberOfOutputSamples);
        }
        if (ok) {
            ok = (numberOfInputSamples == numberOfOutputSamples);
        }
        if (ok) {
            ok = (numberOfInputSamples == 1u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The number of input and output signals samples shall be equal to 1. numberOfInputSamples = %d numberOfOutputSamples = %d",
                         numberOfInputSamples, numberOfOutputSamples);
        }
    }
    if (ok) {
        uint32 numberOfInputDimensions = 0u;
        uint32 numberOfOutputDimensions = 0u;
        ok = GetSignalNumberOfDimensions(InputSignals, 0u, numberOfInputDimensions);
        if (ok) {
            ok = GetSignalNumberOfDimensions(OutputSignals, 0u, numberOfOutputDimensions);
        }
        if (ok) {
            ok = (numberOfInputDimensions == numberOfOutputDimensions);
        }
        if (ok) {
            ok = (numberOfInputDimensions == 0u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The number of input and output signals dimensions shall be equal to 0. numberOfInputDimensions = %d numberOfOutputDimensions = %d",
                         numberOfInputDimensions, numberOfOutputDimensions);
        }
    }
    if (ok) {
        uint32 numberOfInputElements = 0u;
        uint32 numberOfOutputElements = 0u;
        ok = GetSignalNumberOfElements(InputSignals, 0u, numberOfInputElements);
        if (ok) {
            ok = GetSignalNumberOfElements(OutputSignals, 0u, numberOfOutputElements);
        }
        if (ok) {
            ok = (numberOfInputElements == numberOfOutputElements);
        }
        if (ok) {
            ok = (numberOfInputElements == 1u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "The number of input and output signals elements shall be equal to 1. numberOfInputElements = %d numberOfOutputElements = %d",
                         numberOfInputElements, numberOfOutputElements);
        }
    }
    if (ok) {
        inputSignal = reinterpret_cast<uint32 *>(GetInputSignalMemory(0u));
        outputSignal = reinterpret_cast<uint32 *>(GetOutputSignalMemory(0u));
    }
    */


    /*
    inputSignal = reinterpret_cast<uint32 *>(GetInputSignalMemory(0u));
    inputadcs = reinterpret_cast<int16 *>(GetInputSignalMemory(1u));
    */

    /*
    realtime= { DataSource = DDB1 Type = float32  NumberOfElements = 1   CheckSimulinkType = true  NumberOfDimensions=1 }
    adc  = { DataSource = DDB1 Type = int16    NumberOfElements = 192 CheckSimulinkType = true  NumberOfDimensions=1 }
    rfm_in      = { DataSource = DDB1 Type = float32  NumberOfElements = 7   CheckSimulinkType = false NumberOfDimensions=1 }
    wavegen     = { DataSource = DDB1 Type = float32  NumberOfElements = 2   CheckSimulinkType = true  NumberOfDimensions=1 }
    proc_in     = { DataSource = DDB1 Type = float32  NumberOfElements = 3   CheckSimulinkType = false NumberOfDimensions=1 }
    */

    inputtime  = reinterpret_cast<int32 *>(GetInputSignalMemory(0u));

    outputrealtime   = reinterpret_cast<float32 *>(GetOutputSignalMemory(0u));
    outputadc       = reinterpret_cast<int16 *>(GetOutputSignalMemory(1u));
    outputrfm_in    = reinterpret_cast<uint8 *>(GetOutputSignalMemory(2u));
    outputwavegen   = reinterpret_cast<float32 *>(GetOutputSignalMemory(3u));
    outputproc_in   = reinterpret_cast<uint8 *>(GetOutputSignalMemory(4u));
    outputtimeformds = reinterpret_cast<int32 *>(GetOutputSignalMemory(5u));
    outputtriggerformds = reinterpret_cast<uint8 *>(GetOutputSignalMemory(6u));

    *outputtriggerformds=0;

    /*
    int i;
    for(i=0; i<64; i++) outputdacs[i]=0.0;
    for(i=0; i<4; i++) outputdos[i]=0;
    for(i=0; i<8; i++) outputpwms[i]=0;

    cycle=0;
    */

    //return ok;

    return true;

}

bool HelperGAM1::Execute() {
    //REPORT_ERROR(MARTe::ErrorManagement::Debug, "Execute called");


    //*outputSignal = gain * *inputSignal;

    *outputrealtime = (MARTe::float32)(*inputtime) * 1e-6;
    *outputtimeformds = *inputtime + mdstimeoffset;

    if(*outputtimeformds>=mdsstarttime && *outputtimeformds<=mdsstoptime)  *outputtriggerformds=1; else *outputtriggerformds=0;

    /*outputwavegen[0] = f1;
    outputwavegen[1] = f2;

    f1+=1.0;
    f2-=1.0;
    */



    /*
    *outputSignal = *inputSignal - prevval;
    prevval = *inputSignal;

    outputdacs[0] = inputadcs[0];

    if(inputadcs[0]>0)
    {
        outputdos[0]=0xFF;
        outputdos[1]=0xFF;
        outputdos[2]=0xFF;
        outputdos[3]=0xFF;
    }
    else
    {
        outputdos[0]=0x00;
        outputdos[1]=0x00;
        outputdos[2]=0x00;
        outputdos[3]=0x00;
    }


    //cycle++;
    //outputdacs[0] = (MARTe::int16)(3000.0*sin(((double)cycle)/10000.0*6.28));
    */


    return true;
}

CLASS_REGISTER(HelperGAM1, "")
}
