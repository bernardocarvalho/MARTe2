/**
 * @file TimeoutType.h
 * @brief Header file for class TimeoutType
 * @date 17/06/2015
 * @author Giuseppe Ferr�
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

 * @details This header file contains the declaration of the class TimeoutType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TimeoutTYPE_H_
#define TimeoutTYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "HighResolutionTimer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief timeout type definition.
 *
 * @details These methods define the timeout object which is simply an integer which represent a time in milliseconds.
 * Furthermore here are defined flags for different types of timeout.
 * This class is used to implement semaphores based on spin-locks with timed locks and in applications which needs
 * Timeouts.
 */
class TimeoutType {

public:
    /**
     * @brief Constructor from integer.
     * @param[in] msecs is the time in milliseconds.
     */
    inline TimeoutType(const uint32 &msecs = 0xFFFFFFFFu);

    /**
     * @brief Set timeout from float32.
     * @param[in] secs is the time in seconds.
     */
    inline void SetTimeoutSec(float64 secs);

    /**
     * @brief Set timeout from HRT ticks.
     * @param[in] ticks are the number of cpu ticks.
     */
    inline void SetTimeoutHighResolutionTimerTicks(int64 ticks);

    /**
     * @brief Get the timeout in HighResolutionTimer Ticks
     * @return the number of ticks related to the timeout.
     */
    inline int64 HighResolutionTimerTicks() const;

    /**
     * @brief Subtract mSecs to the timeout.
     * @param[in] mSecs is the value which will be subtracted to the timeout (milliseconds).
     */
    inline TimeoutType &operator-=(const uint32 &mSecs);

    /**
     * @brief Compare two timeout times.
     * @param[in] tt is the timeout object to be compared with this.
     * @return true of msecTimeout attributes of both timeout object are equal.
     */
    /*lint -e(1739) , operation uint32 == ProcessorType will not be supported*/
    inline bool operator==(const TimeoutType &tt) const;

    /**
     * @brief Check if two timeout are different.
     * @param[in] tt is another timeout.
     * @return true if msecTimeout attributes of both timeout object are different.
     */
    /*lint -e(1739) , operation uint32 == TimeoutType will not be supported*/
    inline bool operator!=(const TimeoutType &tt) const;

    /**
     * @brief Copy operator.
     * @param[in] tt is the timout to copy in this.
     */
    inline TimeoutType &operator=(const TimeoutType &tt);

    /**
     * @brief Check if the timeout is finite.
     * @return true if the timeout is less than the max timeout acceptable.
     */
    inline bool IsFinite() const;

    /**
     * @brief Returns the timeout value in milliseconds.
     * @return the timeout value in milliseconds.
     */
    inline uint32 GetTimeoutMSec() const;

private:
    /** timeout in milliseconds  */
    uint32 msecTimeout;

};

/** Do not wait (or wait indefinitely if blocking is set */
const TimeoutType TTNoWait(0x00000000u);

/** Infinite wait timeout */
const TimeoutType TTInfiniteWait(0xFFFFFFFFu);

/** Used in semaphore protected codes to specify to bypass the check! */
const TimeoutType TTUnProtected(0xFFFFFFFDu);

/** Used in semaphore protected codes to specify to bypass the check! */
const TimeoutType TTDefault(0xFFFFFFFEu);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
TimeoutType::TimeoutType(const uint32 &msecs) {
    msecTimeout = msecs;
}

void TimeoutType::SetTimeoutSec(float64 secs) {
    secs *= 1000.0;
    msecTimeout = static_cast<uint32>(secs);
}

void TimeoutType::SetTimeoutHighResolutionTimerTicks(int64 ticks) {
    if (ticks < 0) {
        ticks = 0;
    }
    float64 TimeoutSecFromTicks = (static_cast<float64>(ticks) * HighResolutionTimer::Period());
    float64 TimeoutMSecFromTicks = 1000.0 * TimeoutSecFromTicks;
    msecTimeout = static_cast<uint32>(TimeoutMSecFromTicks);
}

int64 TimeoutType::HighResolutionTimerTicks() const {
    float64 deltaT = static_cast<float64>(msecTimeout);
    deltaT = deltaT * 1e-3;
    float64 frequency = static_cast<float64>(HighResolutionTimer::Frequency());
    deltaT = deltaT * frequency;
    int64 ticks = static_cast<int64>(deltaT);
    return ticks;
}

TimeoutType &TimeoutType::operator-=(const uint32 &mSecs) {
    if (msecTimeout > mSecs) {
        msecTimeout -= mSecs;
    }
    else {
        msecTimeout = 0u;
    }
    return *this;
}

bool TimeoutType::operator==(const TimeoutType &tt) const {
    return msecTimeout == tt.msecTimeout;
}

bool TimeoutType::operator!=(const TimeoutType &tt) const {
    return msecTimeout != tt.msecTimeout;
}

TimeoutType &TimeoutType::operator=(const TimeoutType &tt) {
    if (this != &tt) {
        msecTimeout = tt.msecTimeout;
    }
    return *this;
}

bool TimeoutType::IsFinite() const {
    return (msecTimeout < 0xFFFFFFFEu);
}

inline uint32 TimeoutType::GetTimeoutMSec() const {
    return msecTimeout;
}

#endif /* TimeoutTYPE_H_ */

