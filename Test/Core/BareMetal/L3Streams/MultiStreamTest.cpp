/**
 * @file MultiStreamTest.cpp
 * @brief Source file for class MultiStreamTest
 * @date 03 set 2018
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
 * the class MultiStreamTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MultiStreamTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class MultiStreamTestDS: public MultiStream{
public:
    MultiStreamTestDS();

    virtual ~MultiStreamTestDS();

    virtual bool Switch(const char8 *path);

    virtual bool Load(const char8 *varName);

    virtual bool Commit(const char8 *varName);

    virtual bool Delete(const char8 *varName);

    virtual void GetCurrentPath(StreamString &path);

    virtual bool SwitchPrintAndCommit(const char8* path,
                              const char8* varName,
                              const char8* format,
                              const AnyType pars[]);

};


MultiStreamTestDS::MultiStreamTestDS(){

}

MultiStreamTestDS::~MultiStreamTestDS(){

}

bool MultiStreamTestDS::Switch(const char8 *path){
    return true;
}

bool MultiStreamTestDS::Load(const char8 *varName){
    return true;
}

bool MultiStreamTestDS::Commit(const char8 *varName){
    return true;
}

bool MultiStreamTestDS::Delete(const char8 *varName){
    return true;
}

void MultiStreamTestDS::GetCurrentPath(StreamString &path){

}

bool MultiStreamTestDS::SwitchPrintAndCommit(const char8* path,
                          const char8* varName,
                          const char8* format,
                          const AnyType pars[]){
    return true;
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MultiStreamTest::MultiStreamTest() {
    // Auto-generated constructor stub for MultiStreamTest
    // TODO Verify if manual additions are needed
}

MultiStreamTest::~MultiStreamTest() {
    // Auto-generated destructor stub for MultiStreamTest
    // TODO Verify if manual additions are needed
}


bool MultiStreamTest::TestConstructor(){
    MultiStreamTestDS test;
    return true;
}

bool MultiStreamTest::TestIsValid(){
    MultiStreamTestDS test;
    return test.IsValid();
}

