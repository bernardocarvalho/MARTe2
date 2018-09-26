/**
 * @file HttpStreamTTT.h
 * @brief Header file for class HttpStreamTTT
 * @date 19 set 2018
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

 * @details This header file contains the declaration of the class HttpStreamTTT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HTTPSTREAMT_H_
#define HTTPSTREAMT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpRealmI.h"
#include "ConfigurationDatabase.h"
#include "DoubleBufferedStream.h"
/*lint -e{766} this module is used*/
#include "JsonPrinter.h"
/*lint -e{766} this module is used*/
#include "StandardPrinter.h"
/*lint -e{766} this module is used*/
#include "XMLPrinter.h"
#include "StreamStructuredData.h"
/*lint -e{766} this module is used*/
#include "ProtocolI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/*lint -esym(9107, MARTe::HttpStreamT*) [MISRA C++ Rule 3-1-1] required for template implementation*/
template<class Printer>
class HttpStreamT: public StreamString, public StreamStructuredData<Printer> {
public:
    HttpStreamT();

    HttpStreamT(DoubleBufferedStream &clientBufferedStreamIn,
                const bool store = true);

    void SetStoreMode(bool store);

    virtual ~HttpStreamT();

    /**
     * @brief Reads a previously stored AnyType. The node with this name has to be a child of the current node.
     * @param[in] name the name of the leaf used to store the AnyType \a value.
     * @param[out] value the read AnyType will be stored in this parameter. If the AnyType
     * cannot be successfully read its value will be set to VoidType and the function will return false.
     * @return true if the AnyType is successfully read.
     * @pre
     *   GetType(name).GetTypeDescriptor() != VoidType
     */
    virtual bool Read(const char8 * const name,
                      const AnyType &value);

    /**
     * @brief Gets the type of a previously stored AnyType.
     * @param[in] name the name of the leaf used to store the AnyType \a value.
     * @return the type of the stored AnyType or VoidType if this \a name does not exist.
     */
    virtual AnyType GetType(const char8 * const name);

    /**
     * @brief Writes an AnyType against the provided \a name and adds it to the current node.
     * @details If the name already exists the value will be overridden.
     * @param[in] name the name of the leaf against which the AnyType will be stored.
     * @param[in] value the AnyType to store.
     * @return true if the AnyType is successfully stored.
     * @pre
     *   name != NULL &&
     *   StringHelper::Length(name) > 0
     */
    virtual bool Write(const char8 * const name,
                       const AnyType &value);

    /**
     * @brief Copies the content of the current node to the provided destination.
     * @details A deep copy of the contents is recursively performed.
     * @param[out] destination where the database will be copied to.
     * @return true if the copy is successful.
     */
    /*lint -e{1511} the BufferedStreamI::Copy has a different interface*/
    virtual bool Copy(StructuredDataI &destination);

    /**
     * @brief Adds a node to the current node.
     * @param[in] node a reference to the node to add.
     * @return true if the node is successfully added.
     * @post
     *   If successful: the current node will be node
     *   If unsuccessful: the current node will not be changed
     */
    virtual bool AddToCurrentNode(Reference node);

    /**
     * @brief Moves the current node to the root node.
     * @return true if the move is successful and the current node is now the root node.
     */
    virtual bool MoveToRoot();

    /**
     * @brief Moves to the generations-th node containing this node.
     * @param[in] generations number of parent nodes to climb.
     * @return true if the move is successful and the current node is now the parent node which is n-generations above.
     * @pre
     *   generations > 0
     */
    /*lint -e{952} the generations parameter cannot be declared const because it is inherited)*/
    virtual bool MoveToAncestor(uint32 generations);

    /**
     * @brief Moves the current node to a new node address specified by an absolute path.
     * @param[in] path a path with the node address.
     * @return true if the move was successful and the current node is the node described by \a path. If unsuccessful the current node
     * is not changed.
     */
    virtual bool MoveAbsolute(const char8 * const path);

    /**
     * @brief Moves the current node to an address specified by a path relative to the current node address.
     * @param[in] path a path with the node address.
     * @return true if the move was successful and the current node is the node described by \a path. If unsuccessful the current node
     * is not changed.
     */
    virtual bool MoveRelative(const char8 * const path);

    /**
     * @brief Moves the current node the child with idx \a childIdx.
     * @param[in] childIdx the index of the child where to move to.
     * @return true if the move was successful and the current node is the node at index \a .childIdx If unsuccessful the current node
     * is not changed.
     */
    virtual bool MoveToChild(const uint32 childIdx);

    /**
     * @brief Create a new series of nodes based on the provided absolute path.
     * @param[in] path the path of nodes to be created.
     * @return true if the nodes were successfully created and if the path does not already exist.
     * @pre
     *   MoveAbsolute(path) == false
     * @post
     *   If successful: the current node will be the last node specified in the path.
     *   If unsuccessful: the current node will not be changed.
     */
    virtual bool CreateAbsolute(const char8 * const path);

    /**
     * @brief Create a new series of nodes based on the provided relative path.
     * @param[in] path the path of nodes to be created, relative to the current node.
     * @return true if the nodes were successfully created and if the path does not already exist.
     * @pre
     *   MoveRelative(path) == false
     * @post
     *   If successful: the current node will be the last node specified in the path.
     *   If unsuccessful: the current node will not be changed.
     */
    virtual bool CreateRelative(const char8 * const path);

    /**
     * @brief Deletes the node with \a name under the current node (and as a consequence all the nodes underneath).
     * @param[in] name the name of the node to be deleted.
     * @return true if the current node is successfully removed.
     */
    virtual bool Delete(const char8 * const name);

    /**
     * @brief Retrieves the name of the current node.
     * @return the name of the current node.
     */
    virtual const char8 *GetName();

    /**
     * @brief Retrieves the name of the child in the specified index.
     * @param[in] index is the index of the current node child.
     * @return the name of the child in the specified index.
     */
    virtual const char8 *GetChildName(const uint32 index);

    /**
     * @brief Retrieves the number of children of the current node.
     * @return the number of children of the current node.
     */
    virtual uint32 GetNumberOfChildren();

protected:

    DoubleBufferedStream *outputStream;

    ConfigurationDatabase storedData;

    bool storeBody;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

//#define NULL_PTR(x) NULL
template<class Printer>
HttpStreamT<Printer>::HttpStreamT(DoubleBufferedStream &clientBufferedStreamIn,
                                  const bool store) :
        StreamString(),
        StreamStructuredData<JsonPrinter>() {
    outputStream = &clientBufferedStreamIn;
    storeBody = store;
    StreamStructuredData<JsonPrinter>::SetStream(*outputStream);

}

template<class Printer>
HttpStreamT<Printer>::~HttpStreamT() {
    // Auto-generated destructor stub for HttpStreamT
    // TODO Verify if manual additions are needed
    outputStream = NULL_PTR(DoubleBufferedStream *);
}

template<class Printer>
void HttpStreamT<Printer>::SetStoreMode(bool store) {
    storeBody = store;
}

template<class Printer>
bool HttpStreamT<Printer>::Read(const char8 * const name,
                                const AnyType &value) {
    //use a special node
    return (storeBody) ? (storedData.Read(name, value)) : (StreamStructuredData<JsonPrinter>::Read(name, value));
}

template<class Printer>
AnyType HttpStreamT<Printer>::GetType(const char8 * const name) {
    return (storeBody) ? (storedData.GetType(name)) : (StreamStructuredData<JsonPrinter>::GetType(name));
}

template<class Printer>
bool HttpStreamT<Printer>::Write(const char8 * const name,
                                 const AnyType &value) {
    return (storeBody) ? (storedData.Write(name, value)) : (StreamStructuredData<JsonPrinter>::Write(name, value));
}

template<class Printer>
bool HttpStreamT<Printer>::Copy(StructuredDataI &destination) {
    return (storeBody) ? (storedData.Copy(destination)) : (StreamStructuredData<JsonPrinter>::Copy(destination));
}

template<class Printer>
bool HttpStreamT<Printer>::AddToCurrentNode(Reference node) {
    return (storeBody) ? (storedData.AddToCurrentNode(node)) : (StreamStructuredData<JsonPrinter>::AddToCurrentNode(node));
}

template<class Printer>
bool HttpStreamT<Printer>::MoveToRoot() {
    bool ret = (storeBody) ? (storedData.MoveToRoot()) : (StreamStructuredData<JsonPrinter>::MoveToRoot());
    if ((ret) && (storeBody)) {
        ret=Printf("%J!", *reinterpret_cast<StructuredDataI*>(&storedData));
    }
    return ret;
}

/*lint -e{952} the generations parameter cannot be declared const because it is inherited)*/
template<class Printer>
bool HttpStreamT<Printer>::MoveToAncestor(uint32 generations) {
    return (storeBody) ? (storedData.MoveToAncestor(generations)) : (StreamStructuredData<JsonPrinter>::MoveToAncestor(generations));
}

template<class Printer>
bool HttpStreamT<Printer>::MoveAbsolute(const char8 * const path) {
    return (storeBody) ? (storedData.MoveAbsolute(path)) : (StreamStructuredData<JsonPrinter>::MoveAbsolute(path));
}

template<class Printer>
bool HttpStreamT<Printer>::MoveRelative(const char8 * const path) {
    return (storeBody) ? (storedData.MoveRelative(path)) : (StreamStructuredData<JsonPrinter>::MoveRelative(path));
}

template<class Printer>
bool HttpStreamT<Printer>::MoveToChild(const uint32 childIdx) {
    return (storeBody) ? (storedData.MoveToChild(childIdx)) : (StreamStructuredData<JsonPrinter>::MoveToChild(childIdx));
}

template<class Printer>
bool HttpStreamT<Printer>::CreateAbsolute(const char8 * const path) {
    return (storeBody) ? (storedData.CreateAbsolute(path)) : (StreamStructuredData<JsonPrinter>::CreateAbsolute(path));

}

template<class Printer>
bool HttpStreamT<Printer>::CreateRelative(const char8 * const path) {
    return (storeBody) ? (storedData.CreateRelative(path)) : (StreamStructuredData<JsonPrinter>::CreateRelative(path));
}

template<class Printer>
bool HttpStreamT<Printer>::Delete(const char8 * const name) {
    return (storeBody) ? (storedData.Delete(name)) : (StreamStructuredData<JsonPrinter>::Delete(name));
}

template<class Printer>
const char8 *HttpStreamT<Printer>::GetName() {
    return (storeBody) ? (storedData.GetName()) : (StreamStructuredData<JsonPrinter>::GetName());
}

template<class Printer>
const char8 *HttpStreamT<Printer>::GetChildName(const uint32 index) {
    return (storeBody) ? (storedData.GetChildName(index)) : (StreamStructuredData<JsonPrinter>::GetChildName(index));
}

template<class Printer>
uint32 HttpStreamT<Printer>::GetNumberOfChildren() {
    return (storeBody) ? (storedData.GetNumberOfChildren()) : (StreamStructuredData<JsonPrinter>::GetNumberOfChildren());
}


/*lint -esym(751, MARTe::HttpJsonStream) -esym(756, MARTe::HttpJsonStream) this symbol could be not referenced*/
/*lint -esym(756, MARTe::HttpJsonStream) -esym(756, MARTe::HttpJsonStream) this symbol could be not referenced*/
typedef HttpStreamT<JsonPrinter> HttpJsonStream;

/*lint -esym(751, MARTe::HttpXMLStream) this symbol could be not referenced*/
/*lint -esym(756, MARTe::HttpXMLStream) this symbol could be not referenced*/
typedef HttpStreamT<XMLPrinter> HttpXMLStream;

/*lint -esym(751, MARTe::HttpStandardStream) -esym(756, MARTe::HttpJsonStream) this symbol could be not referenced*/
/*lint -esym(756, MARTe::HttpStandardStream) -esym(756, MARTe::HttpJsonStream) this symbol could be not referenced*/
typedef HttpStreamT<StandardPrinter> HttpStandardStream;


}

#endif /* HTTPSTREAMT_H_ */

