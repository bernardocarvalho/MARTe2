/**
 * @file EmbeddedThreadMethodCaller.h
 * @brief Header file for class EmbeddedThreadMethodCaller
 * @date 23/08/2016
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

 * @details This header file contains the declaration of the class EmbeddedThreadMethodCaller
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L2MIXED_EMBEDDEDTHREADMETHODCALLER_H_
#define L2MIXED_EMBEDDEDTHREADMETHODCALLER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EmbeddedThread.h"
#include "GenericVoidMethodCallerI.h"

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
     * TODO
 */
class EmbeddedThreadMethodCaller: public EmbeddedThread{
public:
    /**
     * TODO
     * callerTool is deleted (memory freed) when this object is destroyed
     */
    EmbeddedThreadMethodCaller(GenericVoidMethodCallerI *callerTool);
    /**
     * TODO
     */
    virtual ~EmbeddedThreadMethodCaller();
protected:

    /**
     * TODO
     */
    virtual ErrorManagement::ErrorType Loop();

    /**
     * TODO
     */
    GenericVoidMethodCallerI *caller;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

EmbeddedThreadMethodCaller::EmbeddedThreadMethodCaller(GenericVoidMethodCallerI *callerTool){
    caller = callerTool;
}

/**
 * TODO
 */
EmbeddedThreadMethodCaller::~EmbeddedThreadMethodCaller(){
    if (caller != static_cast<GenericVoidMethodCallerI *>(NULL)){
        delete caller;
    }
}


ErrorManagement::ErrorType EmbeddedThreadMethodCaller::Loop(){
    ErrorManagement::ErrorType ret;
    if (caller == static_cast<GenericVoidMethodCallerI *>(NULL)){
        ret.fatalError = true;
    } else {
        ret = caller->Call();
    }

    return ret;
}


}
#endif /* L2MIXED_EMBEDDEDTHREADMETHODCALLER_H_ */
	
