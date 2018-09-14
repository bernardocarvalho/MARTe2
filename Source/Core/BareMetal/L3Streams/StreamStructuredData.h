/**
 * @file StreamStructuredData.h
 * @brief Header file for class StreamStructuredData
 * @date 14 set 2018
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

 * @details This header file contains the declaration of the class StreamStructuredData
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMSTRUCTUREDDATA_H_
#define STREAMSTRUCTUREDDATA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StructuredDataI.h"
#include "LinkedListable.h"
#include "ReferenceContainer.h"
#include "BufferedStreamI.h"
#include "SingleBufferedStream.h"
#include "DoubleBufferedStream.h"
#include "StreamString.h"
#include "ReferenceT.h"
#include "ReferenceContainer.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceContainerFilterObjectName.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class NodeName: public ReferenceContainer {
public:

    NodeName();
    virtual ~NodeName();

    uint8 isClosed;
    uint32 numberOfVariables;
};

template<class Printer>
class StreamStructuredData: public StructuredDataI {
public:
    StreamStructuredData(BufferedStreamI &streamIn);

    virtual ~StreamStructuredData();

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

    StreamStructuredData();

    virtual void SetStream(BufferedStreamI &streamIn);

    BufferedStreamI *stream;

    //accelerators
    SingleBufferedStream *streamSingle;
    DoubleBufferedStream *streamDouble;

    uint32 bufferType;

    StreamString currentPath;
    ReferenceT<NodeName> treeDescriptor;
    ReferenceT<NodeName> currentNode;

    StreamString middleBuffer;

    Printer printer;
    bool blockCloseState;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<class Printer>
StreamStructuredData<Printer>::StreamStructuredData(BufferedStreamI &streamIn) :
        treeDescriptor(GlobalObjectsDatabase::Instance()->GetStandardHeap()),
        printer(streamIn) {
    stream = &streamIn;

    currentNode = treeDescriptor;

    //accelerators
    streamSingle = dynamic_cast<SingleBufferedStream *>(stream);
    streamDouble = dynamic_cast<DoubleBufferedStream *>(stream);
    bufferType = 0u;
    if (streamSingle != NULL) {
        bufferType=1u;
    }
    if (streamDouble != NULL) {
        bufferType=2u;
    }
    blockCloseState = false;
}

template<class Printer>
StreamStructuredData<Printer>::StreamStructuredData() :
        treeDescriptor(GlobalObjectsDatabase::Instance()->GetStandardHeap()),
        printer() {

    stream = NULL;

    currentNode = treeDescriptor;

    //accelerators
    streamSingle = dynamic_cast<SingleBufferedStream *>(stream);
    streamDouble = dynamic_cast<DoubleBufferedStream *>(stream);
    bufferType = 0u;
    streamSingle = NULL;
    streamDouble = NULL;

    blockCloseState = false;
}

template<class Printer>
void StreamStructuredData<Printer>::SetStream(BufferedStreamI &streamIn) {
    stream = &streamIn;
    printer.SetStream(streamIn);

    //accelerators
    streamSingle = dynamic_cast<SingleBufferedStream *>(stream);
    streamDouble = dynamic_cast<DoubleBufferedStream *>(stream);
    bufferType = 0u;
    if (streamSingle != NULL) {
        bufferType=1u;
    }
    if (streamDouble != NULL) {
        bufferType=2u;
    }
}

template<class Printer>
StreamStructuredData<Printer>::~StreamStructuredData() {

}

template<class Printer>
bool StreamStructuredData<Printer>::Read(const char8 * const name,
                                         const AnyType &value) {
    return false;
}

template<class Printer>
AnyType StreamStructuredData<Printer>::GetType(const char8 * const name) {
    return voidAnyType;
}

template<class Printer>
bool StreamStructuredData<Printer>::Write(const char8 * const name,
                                          const AnyType &value) {

    bool ret = true;
    if (currentNode->numberOfVariables > 0u) {
        ret = printer.PrintVariableSeparator();
    }
    if (ret) {
        ret = stream->Printf("%s", "\n\r");
    }
    if (ret) {
        //use custom component to print
        ret = printer.PrintOpenAssignment(name);
    }
    if (ret) {
        ret = printer.PrintVariable(value);
    }
    if (ret) {
        ret = printer.PrintCloseAssignment(name);
    }

    if (ret) {
        if (bufferType == 1u) {
            streamSingle->FlushAndResync();
        }
        else if (bufferType == 2u) {
            streamDouble->Flush();
        }
        currentNode->numberOfVariables++;
    }
    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::Copy(StructuredDataI &destination) {
    return false;
}

template<class Printer>
bool StreamStructuredData<Printer>::AddToCurrentNode(Reference node) {
    ReferenceT<NodeName> toAdd = node;
    bool ret = toAdd.IsValid();
    if (ret) {
        ret = currentNode->Insert(node);
    }
    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::MoveToRoot() {

    //find the last node by path
    //close the nodes along the path

    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH, currentNode);
    ReferenceContainer path;
    treeDescriptor->Find(path, filter);

    uint32 pathSize = path.Size();
    bool ret = (pathSize > 0u);
    for (uint32 i = 0u; (i < pathSize) && (ret); i++) {
        ReferenceT<NodeName> ref = path.Get(pathSize - i - 1u);
        ret = ref.IsValid();
        if (ret) {
            ref->isClosed = 1u;
            ret = stream->Printf("%s", "\n\r");
            if (ret) {
                ret = printer.PrintCloseBlock(ref->GetName());
                blockCloseState = true;
            }
        }
    }

    if (ret) {
        currentPath = "";
        currentNode = treeDescriptor;
    }
    if (ret) {
        if (bufferType == 1u) {
            streamSingle->FlushAndResync();
        }
        else if (bufferType == 2u) {
            streamDouble->Flush();
        }
    }

    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::MoveToAncestor(uint32 generations) {

    bool ret = true;
    if (generations > 0u) {

        ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH, currentNode);
        ReferenceContainer path;
        treeDescriptor->Find(path, filter);

        uint32 pathSize = path.Size();
        ret = (pathSize > generations);

        uint32 goodOnes = (pathSize - generations);
        StreamString currentPathTmp = "";
        currentPath.Seek(0u);
        for (uint32 i = 0u; (i < pathSize) && (ret); i++) {

            if (i < goodOnes) {
                StreamString token;
                char8 terminator;
                ret = currentPath.GetToken(token, ".", terminator);
                if (ret) {
                    ret = (terminator == '.');
                    if (ret) {
                        currentPathTmp += token;
                    }
                }
            }
            else {
                ReferenceT<NodeName> ref = path.Get(pathSize - i + goodOnes - 1u);
                ret = (ref.IsValid());
                if (ret) {
                    ref->isClosed = 1u;
                    ret = stream->Printf("%s", "\n\r");
                    if (ret) {
                        ret = printer.PrintCloseBlock(ref->GetName());
                        blockCloseState = true;
                    }
                }
            }
        }
        if (ret) {
            currentNode = path.Get(goodOnes - 1u);
            currentPath = currentPathTmp;
        }
        if (ret) {
            if (bufferType == 1u) {
                streamSingle->FlushAndResync();
            }
            else if (bufferType == 2u) {
                streamDouble->Flush();
            }
        }
    }
    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::MoveAbsolute(const char8 * const path) {

    ReferenceContainerFilterObjectName filterDest(1, ReferenceContainerFilterMode::PATH, path);
    ReferenceContainer resultDest;
    treeDescriptor->Find(resultDest, filterDest);

    uint32 pathDestSize = resultDest.Size();
    bool ret = (pathDestSize > 0u);
    if (ret) {
        ReferenceT<NodeName> ref = resultDest.Get(pathDestSize - 1u);
        ret = (ref.IsValid());
        if (ret) {
            ret = (ref->isClosed == 0u);
        }
        if (ret) {

            ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH, currentNode);
            ReferenceContainer result;
            treeDescriptor->Find(result, filter);

            uint32 pathSize = result.Size();

            StreamString curPathTemp = currentPath;
            curPathTemp.Seek(0u);
            char8 terminator = '.';
            uint32 i = 0u;

            for (i = 0u; (i < pathDestSize) && (terminator == '.') && (ret); i++) {

                StreamString token;
                curPathTemp.GetToken(token, ".", terminator);
                ReferenceT<NodeName> ref = resultDest.Get(i);
                ret = (ref.IsValid());
                if (ret) {
                    StreamString nodeName = ref->GetName();
                    if (token != nodeName) {
                        break;
                    }
                }
            }

            //close nodes and braces
            uint32 exitIndex = i;
            bool blocksClosed = (exitIndex < pathSize);
            if (pathSize > 0u) {
                for (uint32 j = (pathSize - 1u); (j >= exitIndex) && (ret); j--) {
                    ReferenceT<NodeName> ref = result.Get(j);
                    ret = (ref.IsValid());
                    if (ret) {
                        ref->isClosed = 1u;
                        ret = stream->Printf("%s", "\n\r");
                        if (ret) {
                            ret = printer.PrintCloseBlock(ref->GetName());
                            blockCloseState = true;
                        }
                    }
                }
            }

            //open braces
            for (uint32 j = exitIndex; (j < pathDestSize) && (ret); j++) {
                ReferenceT<NodeName> ref = resultDest.Get(j);
                ret = (ref.IsValid());
                if (ret) {
                    if (j == exitIndex) {
                        if (blockCloseState) {
                            ret = printer.PrintBlockSeparator();
                        }
                    }
                    ret = stream->Printf("%s", "\n\r");
                    if (ret) {
                        ret = printer.PrintOpenBlock(ref->GetName());
                        blockCloseState = false;
                    }
                }
            }

            if (ret) {
                if (blocksClosed) {
                    //close deeply also the children
                    uint32 curSize = currentNode->Size();
                    for (uint32 j = 0u; (j < curSize) && (ret); j++) {
                        Reference ref = currentNode->Get(j);
                        ret = currentNode->Delete(ref);
                    }
                }
                if (ret) {
                    currentPath = path;
                    currentNode = ref;
                }
            }
            if (ret) {
                if (bufferType == 1u) {
                    streamSingle->FlushAndResync();
                }
                else if (bufferType == 2u) {
                    streamDouble->Flush();
                }
            }
        }
    }

    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::MoveRelative(const char8 * const path) {
    StreamString totalPath = currentPath;
    if (currentPath.Size() > 0u) {
        totalPath += ".";
    }
    totalPath += path;
    return MoveAbsolute(totalPath.Buffer());

}

template<class Printer>
bool StreamStructuredData<Printer>::MoveToChild(const uint32 childIdx) {

    ReferenceT<NodeName> child = currentNode->Get(childIdx);
    bool ret = child.IsValid();
    if (ret) {
        ret = (child->isClosed == 0u);
    }
    if (ret) {
        ret = stream->Printf("%s", "\n\r");
        if (ret) {
            ret = printer.PrintOpenBlock(child->GetName());
            blockCloseState = false;
        }
        if (ret) {
            currentPath += child->GetName();
            currentNode = child;
        }
        if (ret) {
            if (bufferType == 1u) {
                streamSingle->FlushAndResync();
            }
            else if (bufferType == 2u) {
                streamDouble->Flush();
            }
        }
    }
    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::CreateAbsolute(const char8 * const path) {
    StreamString pathStr = path;
    pathStr.Seek(0u);
    ReferenceT<NodeName> node = treeDescriptor;

    //tokenize the path
    char8 terminator;
    StreamString token;
    bool ret = true;
    while ((pathStr.GetToken(token, ".", terminator)) && (ret)) {
        uint32 nodeSize = node->Size();
        bool found = false;
        for (uint32 i = 0u; (i < nodeSize) && (ret) && (!found); i++) {
            ReferenceT<NodeName> child = node->Get(i);
            ret = child.IsValid();
            if (ret) {
                found = (token == child->GetName());
                if (found) {
                    node = child;
                }
            }
        }
        if ((ret) && (!found)) {
            //create the node
            ReferenceT<NodeName> newNode(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ret = newNode.IsValid();
            if (ret) {
                newNode->SetName(token.Buffer());
                ret = node->Insert(newNode);
                node = newNode;
            }
        }
        token.SetSize(0);
    }

    if (ret) {
        ret = MoveAbsolute(path);
    }

    return ret;
}

template<class Printer>
bool StreamStructuredData<Printer>::CreateRelative(const char8 * const path) {
    StreamString totalPath = (currentPath);
    if (currentPath.Size() > 0u) {
        totalPath += ".";
    }
    totalPath += path;
    return CreateAbsolute(totalPath.Buffer());
}

template<class Printer>
bool StreamStructuredData<Printer>::Delete(const char8 * const name) {
    return false;
}

template<class Printer>
const char8 *StreamStructuredData<Printer>::GetName() {
    return currentNode->GetName();
}

template<class Printer>
const char8 *StreamStructuredData<Printer>::GetChildName(const uint32 index) {
    uint32 size = currentNode->Size();

    const char8 * ret = NULL;
    if (index < size) {
        ReferenceT<NodeName> child = currentNode->Get(index);
        if (child.IsValid()) {
            ret = child->GetName();
        }
    }
    return ret;
}

template<class Printer>
uint32 StreamStructuredData<Printer>::GetNumberOfChildren() {
    return currentNode->Size();
}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L3STREAMS_STREAMSTRUCTUREDDATA_H_ */

