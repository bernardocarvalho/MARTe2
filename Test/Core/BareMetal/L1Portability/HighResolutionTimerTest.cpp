/**
 * @file HighResolutionTimerTest.cpp
 * @brief Source file for class HighResolutionTimerTest
 * @date 26/06/2015
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

 * @details This source file contains the definition of all the methods for
 * the class HighResolutionTimerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HighResolutionTimerTest.h"
#include "HighResolutionTimer.h"
#include "Sleep.h"
#include <stdio.h>

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//return true if a and b are equal less than a tolerance
static bool Tolerance(float64 a,
                      float64 b,
                      float64 tolerance) {
    float64 diff = a - b;
    return (diff < tolerance) && (diff > -tolerance);
}

bool HighResolutionTimerTest::TestFrequency() {
    HighResolutionTimerCalibrator calib;
    return HighResolutionTimer::Frequency() > 0;
}

bool HighResolutionTimerTest::TestPeriod() {
    return HighResolutionTimer::Period() > 0;
}

bool HighResolutionTimerTest::TestCounter(float64 sleepTime) {
    uint64 counter_1 = 0;
    uint64 counter = 0;
    float64 time;
    counter_1 = HighResolutionTimer::Counter();

    Sleep::Sec(sleepTime);
    counter = HighResolutionTimer::Counter();
    time = HighResolutionTimer::TicksToTime(int64(counter), int64(counter_1));
    //printf("elapsed=%f", time);
    return Tolerance(time, sleepTime, sleepTime * .2);
}

bool HighResolutionTimerTest::TestCounter32(float64 sleepTime) {
    uint32 counter_1 = 0;
    uint32 counter = 0;
    float64 time;
    counter_1 = HighResolutionTimer::Counter32();

    Sleep::Sec(sleepTime);
    counter = HighResolutionTimer::Counter32();

    time = (counter - counter_1) * HighResolutionTimer::Period();
    //printf("elapsed=%f", time);
    return Tolerance(time, sleepTime, sleepTime * .2);
}

bool HighResolutionTimerTest::TestTicksToTime() {
    bool retValue;
    float64 sleepTime;
    //minimum time
    sleepTime = 0;
    int64 ticks = sleepTime * HighResolutionTimer::Frequency();
    retValue = Tolerance(HighResolutionTimer::TicksToTime(ticks, 0), sleepTime, 0.01);
    //Arbitrary time
    sleepTime = 5;
    ticks = sleepTime * HighResolutionTimer::Frequency();
    retValue &= Tolerance(HighResolutionTimer::TicksToTime(ticks, 0), sleepTime, 0.01);
    //large time
    sleepTime = 123456789;
    ticks = sleepTime * HighResolutionTimer::Frequency();
    retValue &= Tolerance(HighResolutionTimer::TicksToTime(ticks, 0), sleepTime, 0.01);
    return retValue;
}

bool HighResolutionTimerTest::TestGetTimeStamp(uint32 millisecs) {
    TimeStamp myTimeStamp1;
    TimeStamp myTimeStamp2;
    int32 conversions[] = { 1000, 60000, 3600000, 3600000 * 24 };
    //Arbitrary tolerance
    int32 tolerance = int32(3 * millisecs);
    //tolerance has to greater than 0
    if (tolerance == 0) {
        tolerance = 1;
    }
    HighResolutionTimer::GetTimeStamp(myTimeStamp1);
    Sleep::MSec(millisecs);
    HighResolutionTimer::GetTimeStamp(myTimeStamp2);
    //it could fail if you are unlucky and launch the test across one month and the other at least!
    int32 elapsed = (myTimeStamp2.GetDay() - myTimeStamp1.GetDay()) * conversions[3] + (myTimeStamp2.GetHour()- myTimeStamp1.GetHour()) * conversions[2]
            + (myTimeStamp2.GetMinutes()- myTimeStamp1.GetMinutes()) * conversions[1] + (myTimeStamp2.GetSeconds()- myTimeStamp1.GetSeconds()) * conversions[0]
            + ((int32)(myTimeStamp2.GetMicroseconds() - myTimeStamp1.GetMicroseconds())) / conversions[0];
    int32 diff = (int32) (elapsed - millisecs);

    if (diff > tolerance || diff < -tolerance) {
        return false;
    }
    //checks the boundaries
    if (myTimeStamp1.GetMinutes() > 59) {
        return false;
    }
    if (myTimeStamp1.GetHour() > 23) {
        return false;
    }
    if (myTimeStamp1.GetDay() > 30 ) {
        return false;
    }
    if (myTimeStamp1.GetMonth() > 11) {
        return false;
    }
    return true;
}

bool HighResolutionTimerTest::TestPeriodFrequency() {
    int64 HRTfrequency = HighResolutionTimer::Frequency();
    float64 HRTperiod = HighResolutionTimer::Period();
    float64 relativePeriod = 1.0 / HRTfrequency;
    int64 relativeFrequency = (int64) (1.0 / HRTperiod);
    return (Tolerance(HRTperiod, relativePeriod, 1e-9) && Tolerance(float64(HRTfrequency), float64(relativeFrequency), 1e-9));
}
