/**
 * @file ConfigurationDatabaseNode.h
 * @brief Header file for class ConfigurationDatabaseNode
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

 * @details This header file contains the declaration of the class ConfigurationDatabaseNode
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONFIGURATIONDATABASENODE_H_
#define CONFIGURATIONDATABASENODE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceT.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Helper class that allows to know what is the brother of a given node in a ConfigurationDatabase.
 */
class ConfigurationDatabaseNode: public ReferenceContainer {
public:
    /**
     * @brief Constructor.
     */
    ConfigurationDatabaseNode();

    /**
     * @brief Destructor.
     */
    virtual ~ConfigurationDatabaseNode();

    /**
     * @brief Sets the brother of this node.
     * @param[in] brotherIn the brother of this node.
     */
    void SetBrother(ReferenceT<ConfigurationDatabaseNode> brotherIn);

    /**
     * @brief Gets the brother of this node.
     * @return the brother of this node.
     */
    ReferenceT<ConfigurationDatabaseNode> const & GetBrother() const;

    /**
     * @brief Sets the last child that was added to this node. Accelerator to allow to quickly find and set the brother of the next node to be added.
     * @param[in] lastAddedChildIn the last child that was added to this node.
     */
    void SetLastAddedChild(ReferenceT<ConfigurationDatabaseNode> lastAddedChildIn);

    /**
     * @brief Gets the last child that was added to this node.
     * @return the last child that was added to this node.
     */
    ReferenceT<ConfigurationDatabaseNode> const & GetLastAddedChild() const;

private:
    /**
     * The brother.
     */
    ReferenceT<ConfigurationDatabaseNode> brother;

    /**
     * The child that was last added to this node.
     */
    ReferenceT<ConfigurationDatabaseNode> lastAddedChild;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L4CONFIGURATION_CONFIGURATIONDATABASENODE_H_ */

