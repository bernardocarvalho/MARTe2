/**
 * @file BasicGAMScheduler.h
 * @brief Header file for class BasicGAMScheduler
 * @date 22/03/2016
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

 * @details This header file contains the declaration of the class BasicGAMScheduler
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICGAMSCHEDULER_H_
#define BASICGAMSCHEDULER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAMSchedulerI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief Basic Bare Metal scheduler.
 */
class BasicGAMScheduler: public GAMSchedulerI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     * @detail Initialises the number of cycles to -1 (infinite loop)
     */
    BasicGAMScheduler();

    /**
     * @brief Initialises the scheduler from StructuredDataI.
     * @details The following field can be specified:
     *
     *   NumberOfCycles = (int64 variable)
     *
     * The default number of cycles is -1, namely infinite cycles.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see SchedulerI::StartExecution(*)
     * @details Executes the GAMs for the specified number of cycles.
     */
    virtual void StartExecution(const uint32 activeBuffer);

    /**
     * @see SchedulerI::StopExecution()
     * @details Empty.
     */
    virtual void StopExecution();


private:

    /**
     * The number of execution cycles.
     */
    int64 numberOfCycles;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICGAMSCHEDULER_H_ */

