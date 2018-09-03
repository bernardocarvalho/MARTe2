/**
 * @file StructuredDataStreamTTest.cpp
 * @brief Source file for class StructuredDataStreamTTest
 * @date 30 ago 2018
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
 * the class StructuredDataStreamTTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StructuredDataStreamTTest.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

StructuredDataStreamTTest::StructuredDataStreamTTest() {
    // Auto-generated constructor stub for StructuredDataStreamTTest
    // TODO Verify if manual additions are needed
}

StructuredDataStreamTTest::~StructuredDataStreamTTest() {
    // Auto-generated destructor stub for StructuredDataStreamTTest
    // TODO Verify if manual additions are needed
}

bool StructuredDataStreamTTest::TestSwitch() {
    StructuredDataStreamT<ConfigurationDatabase> test;

    bool ret = test.Printf("%s", "ciao");

    ret &= test.Switch("A.B.C");

    ret &= test.Commit("var");

    ConfigurationDatabase*cdb = test.GetData();

    ret &= cdb->MoveAbsolute("A.B.C");

    StreamString res;

    ret &= cdb->Read("var", res);

    ret &= (res == "ciao");

    if (ret) {
        ret = test.Switch("A");
        *(StreamString*) (&test) = "";
        ret &= test.Printf("%s", "ImVar2");

        ret &= test.Commit("var");
        ret &= cdb->MoveAbsolute("A");
        res = "";

        ret &= cdb->Read("var", res);
        ret &= (res == "ImVar2");

        //check the node has not be deleted
        res = "";

        ret &= cdb->MoveAbsolute("A.B.C");
        ret &= cdb->Read("var", res);

        ret &= (res == "ciao");

    }

    return ret;
}

bool StructuredDataStreamTTest::TestLoad() {

    StructuredDataStreamT<ConfigurationDatabase> test;

    bool ret = test.Printf("%s", "ciao");

    ret &= test.Switch("A.B.C");

    ret &= test.Commit("var");

    *(StreamString*) (&test) = "invalid";
    ret &= (*(StreamString*) (&test) == "invalid");
    ret &= test.Load("var");
    ret &= (*(StreamString*) (&test) == "ciao");

    ret &= test.Switch("");

    ret &= !test.Load("var");

    ret &= test.Switch("A.B.C");
    *(StreamString*) (&test) = "invalid";

    ret &= test.Load("var");
    ret &= (*(StreamString*) (&test) == "ciao");

    return ret;

}

bool StructuredDataStreamTTest::TestCommit() {
    StructuredDataStreamT<ConfigurationDatabase> test;

    bool ret = test.Printf("%s", "ciao");

    ret &= test.Switch("A.B.C");

    ret &= test.Commit("var");

    if (ret) {
        ret = test.Switch("A");
        test.SetSize(0);
        ret &= test.Printf("%s", "ImVar2");

        ret &= test.Commit("var");

        ret &= test.Switch("A.B.C");

        ret &= test.Load("var");
        ret &= (*(StreamString*) (&test) == "ciao");

        ret &= test.Switch("A");

        ret &= test.Load("var");
        ret &= (*(StreamString*) (&test) == "ImVar2");

    }

    if (ret) {
        ret = test.Switch("A.B.C");
        test.SetSize(0);
        ret &= test.Printf("%s", "ImVar3");

        ret &= test.Commit("var3");

        ret &= test.Switch("A.B");
        test.SetSize(0);
        ret &= test.Printf("%s", "ImVar4");

        ret &= test.Commit("var4");

        ret &= test.Switch("A.B.C");
        ret &= test.Load("var");
        ret &= (*(StreamString*) (&test) == "ciao");

        ret &= test.Load("var3");
        ret &= (*(StreamString*) (&test) == "ImVar3");

        ret &= test.Switch("A.B");
        ret &= test.Load("var4");
        ret &= (*(StreamString*) (&test) == "ImVar4");

        ret &= test.Switch("A");
        ret &= test.Load("var");
        ret &= (*(StreamString*) (&test) == "ImVar2");

    }

    //overwrite
    if (ret) {
        ret = test.Switch("A.B.C");
        test.SetSize(0);
        ret &= test.Printf("%s", "ImVar5");
        ret &= test.Commit("var");

        ret &= test.Switch("A.B");
        ret &= test.Load("var4");
        ret &= (*(StreamString*) (&test) == "ImVar4");

        ret &= test.Switch("A.B.C");
        ret &= test.Load("var");
        ret &= (*(StreamString*) (&test) == "ImVar5");

    }

    return ret;

}

bool StructuredDataStreamTTest::TestDelete() {

    StructuredDataStreamT<ConfigurationDatabase> test;

    bool ret = test.Printf("%s", "ciao");

    ret &= test.Switch("A.B.C");

    ret &= test.Commit("var");

    ret &= test.Load("var");
    ret &= (*(StreamString*) (&test) == "ciao");

    ret &= test.Switch("A");
    ret &= !test.Load("var");
    ret &= test.Switch("A.B.C");

    ret &= test.Load("var");
    ret &= test.Delete("var");

    ret &= !test.Load("var");
    ret &= !test.Delete("var");

    //delete a node
    if (ret) {
        ret = test.Switch("A");
        ret &= test.Delete("B");
        ConfigurationDatabase*cdb = test.GetData();
        ret &= cdb->MoveAbsolute("A");
        ret &= (cdb->GetNumberOfChildren() == 0);
    }

    return ret;

}

bool StructuredDataStreamTTest::TestGetCurrentPath() {

    StructuredDataStreamT<ConfigurationDatabase> test;

    bool ret = test.Printf("%s", "ciao");

    ret &= test.Switch("A.B.C");

    StreamString path;
    test.GetCurrentPath(path);
    ret &= path == "A.B.C";

    ret &= test.Commit("var");
    test.GetCurrentPath(path);
    ret &= path == "A.B.C.var";


    test.SetSize(0);
    ret &= test.Printf("%s", "pippo");
    ret &= test.Commit("var2");
    test.GetCurrentPath(path);
    ret &= path == "A.B.C.var2";

    ret &= test.Load("var");
    test.GetCurrentPath(path);
    ret &= path == "A.B.C.var";

    ret&=test.Switch("A.B");
    ret &= test.Commit("var");
    test.GetCurrentPath(path);
    ret &= path == "A.B.var";

    ret&=test.Delete("var");
    test.GetCurrentPath(path);
    ret &= path == "A.B";

    ret&=test.Delete("C");
    test.GetCurrentPath(path);
    ret &= path == "A.B";

    return ret;
}

bool StructuredDataStreamTTest::TestSwitchPrintAndCommit() {
    StructuredDataStreamT<ConfigurationDatabase> test;
    const char8* arg1="ciao";
    uint32 arg2=20;
    float32 arg3=12.5F;

    AnyType at[]={arg1, arg2, arg3};
    bool ret=test.SwitchPrintAndCommit("A.B.C", "var", "String:%s uint32:%d float32:%f", at);

    if(ret){
        //has returned to root
        ret=!test.Load("var");
        ret&=test.Switch("A.B.C");
        ret*=test.Load("var");
    }
    if(ret){
        ret = *(StreamString*)(&test) == "String:ciao uint32:20 float32:12.500000";
    }

    return ret;
}
