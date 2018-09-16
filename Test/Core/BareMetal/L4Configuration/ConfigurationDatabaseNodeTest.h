/**
 * @file ConfigurationDatabaseNodeTest.h
 * @brief Header file for class ConfigurationDatabaseNodeTest
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

 * @details This header file contains the declaration of the class ConfigurationDatabaseNodeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONFIGURATIONDATABASENODETEST_H_
#define CONFIGURATIONDATABASENODETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the ConfigurationDatabaseNode functions.
 */
class ConfigurationDatabaseNodeTest {
public:
    /**
     * @brief Tests default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the SetBrother method.
     */
    bool TestSetBrother();

    /**
     * @brief Tests the GetBrother method.
     */
    bool TestGetBrother();

    /**
     * @brief Tests the SetLastAddedChild method.
     */
    bool TestSetLastAddedChild();

    /**
     * @brief Tests the GetLastAddedChild method.
     */
    bool TestGetLastAddedChild();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONFIGURATIONDATABASENODETEST_H_ */
