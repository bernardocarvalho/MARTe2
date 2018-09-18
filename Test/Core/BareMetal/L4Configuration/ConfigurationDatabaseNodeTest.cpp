/**
 * @file ConfigurationDatabaseNodeTest.cpp
 * @brief Source file for class ConfigurationDatabaseNodeTest
 * @date 16/09/2018
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
 * the class ConfigurationDatabaseNodeTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabaseNode.h"
#include "ConfigurationDatabaseNodeTest.h"
#include "GlobalObjectsDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ConfigurationDatabaseNodeTest::TestConstructor() {
    using namespace MARTe;
    ConfigurationDatabaseNode test;
    return (test.Size() == 0u);
}

bool ConfigurationDatabaseNodeTest::TestSetBrother() {
    using namespace MARTe;
    ReferenceT<ConfigurationDatabaseNode> brother(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabaseNode test;
    test.SetBrother(brother);
    return (test.GetBrother() == brother);
}

bool ConfigurationDatabaseNodeTest::TestGetBrother() {
    return TestSetBrother();
}

bool ConfigurationDatabaseNodeTest::TestSetLastAddedChild() {
    using namespace MARTe;
    ReferenceT<ConfigurationDatabaseNode> lastChild(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabaseNode test;
    test.SetLastAddedChild(lastChild);
    return (test.GetLastAddedChild() == lastChild);
}

bool ConfigurationDatabaseNodeTest::TestGetLastAddedChild() {
    return TestSetLastAddedChild();
}

