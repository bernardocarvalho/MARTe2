/**
 * @file SenderStructuredData.cpp
 * @brief Source file for class SenderStructuredData
 * @date 06 set 2018
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
 * the class SenderStructuredData (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SenderStructuredData.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

SenderStructuredData::SenderStructuredData() :
        treeDescriptor(GlobalObjectsDatabase::Instance()->GetStandardHeap()) {
    stream = NULL;

    //accelerators
    streamSingle=NULL;
    streamDouble=NULL;

    bufferType = 0u;

}

SenderStructuredData::~SenderStructuredData() {

}

void SenderStructuredData::SetStream(DoubleBufferedStream &streamIn) {
    stream = streamIn;
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

bool SenderStructuredData::Read(const char8 * const name,
                                const AnyType &value) {
    return false;
}

AnyType SenderStructuredData::GetType(const char8 * const name) {
    return voidAnyType;
}

bool SenderStructuredData::Write(const char8 * const name,
                                 const AnyType &value) {

    //TODO
    //use custom component to print

}

bool SenderStructuredData::Copy(StructuredDataI &destination) {
    return false;
}

bool SenderStructuredData::AddToCurrentNode(Reference node) {
    ReferenceT<NodeName> toAdd = node;
    bool ret = toAdd.IsValid();
    if (ret) {
        ret = currentNode->Insert(node);
    }
    return ret;
}

bool SenderStructuredData::MoveToRoot() {

    //find the last node by path
    //close the nodes along the path

    ReferenceContainerFilterObjectReferences filter(1, ReferenceContainerFilterMode::PATH, currentNode);
    ReferenceContainer path;
    treeDescriptor->Find(path, filter);

    uint32 pathSize = path.Size();
    bool ret = (pathSize > 0u);
    for (uint32 i = 0u; (i < pathSize) && (ret); i++) {
        ReferenceT<NodeName> ref = path.Get(i);
        ret = ref.IsValid();
        if (ret) {
            ref->isClosed = 1u;
        }
    }

    if (ret) {
        currentPath = "";
        currentNode = treeDescriptor;
    }

    return ret;
}

bool SenderStructuredData::MoveToAncestor(uint32 generations) {

    bool ret = true;
    if (generations > 0u) {

        ReferenceContainerFilterObjectReferences filter(1, ReferenceContainerFilterMode::PATH, currentNode);
        ReferenceContainer path;
        treeDescriptor->Find(path, filter);

        uint32 pathSize = path.Size();
        ret = (pathSize > generations);

        uint32 goodOnes = (pathSize - generations);
        StreamString currentPathTmp = "";
        for (uint32 i = 0u; (i < pathSize) && (ret); i++) {

            if (i < goodOnes) {
                StreamString token;
                char8 terminator;
                ret = curPathTemp.GetToken(token, ".", terminator);
                if (ret) {
                    ret = (terminator == '.');
                    if (ret) {
                        currentPathTmp += token;
                    }
                }
            }
            else {
                ReferenceT<NodeName> ref = path.Get(i);
                ret = (ref.IsValid());
                if (ret) {
                    ref->isClosed = 1u;
                }
            }
        }
        if (ret) {
            currentNode = path.Get(goodOnes - 1u);
            currentPath = currentPathTmp;
        }
    }
    return ret;
}

bool SenderStructuredData::MoveAbsolute(const char8 * const path) {

    ReferenceContainerFilterObjectName filterDest(1, ReferenceContainerFilterMode::PATH, path);
    ReferenceContainer resultDest;
    treeDescriptor->Find(resultDest, filterDest);

    uint32 pathDestSize = resultDest.Size();
    bool ret = (pathDestSize > 0u);
    if (ret) {
        ReferenceT<NodeName> ref = path.Get(pathDestSize - 1u);
        ret = (ref.IsValid());
        if (ret) {
            ret = (ref->isClosed == 0u);
        }
        if (ret) {

            ReferenceContainerFilterObjectReferences filter(1, ReferenceContainerFilterMode::PATH, currentNode);
            ReferenceContainer result;
            treeDescriptor->Find(result, filter);

            uint32 pathSize = result.Size();

            StreamString curPathTemp = currentPath;
            char8 terminator = '.';
            uint32 i = 0u;

            for (i = 0u; (i < pathDestSize) && (terminator == '.') && (ret); i++) {

                StreamString token;
                curPathTemp.GetToken(token, ".", terminator);
                ReferenceT < NodeName > ref = resultDest.Get(i);
                ret = (ref.IsValid());
                if (ret) {
                    StreamString nodeName = ref.GetName();
                    if (token != nodeName) {
                        break;
                    }
                }
            }

            while ((i < pathSize) && (ret)) {
                ReferenceT < NodeName > ref = result.Get(i);
                ret = (ref.IsValid());
                if (ret) {
                    ref->isClosed = 1u;
                }
                i++;
            }

            if (ret) {
                //close deeply also the children
                uint32 curSize = currentNode->Size();
                for (uint32 j = 0u; (j < curSize) && (ret); j++) {
                    Reference ref = currentNode->Get(j);
                    ret = currentNode->Delete(ref);
                }
                if (ret) {
                    currentPath = path;
                    currentNode = ref;
                }
            }
        }
    }

    return ret;
}

bool SenderStructuredData::MoveRelative(const char8 * const path) {
    StreamString totalPath = (currentPath + path);
    return MoveAbsolute(totalPath);

}

bool SenderStructuredData::MoveToChild(const uint32 childIdx) {

    ReferenceT<NodeName> child = currentNode.Get(childIdx);
    bool ret = child.IsValid();
    if (ret) {
        ret = (child.isClosed == 0u);
    }
    if (ret) {
        //todo send the braces
        currentPath += child.GetName();
    }
    return ret;
}

bool SenderStructuredData::CreateAbsolute(const char8 * const path) {
    StreamString pathStr = path;
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
                found = (token == child.GetName());
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
                newNode.SetName(token.Buffer());
                ret = node->Insert(newNode);
                node = newNode;
            }
        }
        token.SetSize(0);
    }

    if (ret) {
        currentNode = node;
        currentPath = path;
    }

    return ret;
}

bool SenderStructuredData::CreateRelative(const char8 * const path) {
    StreamString totalPath = (currentPath + path);
    return CreateAbsolute(totalPath.Buffer());
}

bool SenderStructuredData::Delete(const char8 * const name) {
    return false;
}

const char8 *SenderStructuredData::GetName() {
    return currentNode.GetName();
}

const char8 *SenderStructuredData::GetChildName(const uint32 index) {
    uint32 size = currentNode.Size();

    const char8 * ret = NULL;
    if (index < size) {
        ReferenceT<NodeName> child = currentNode.Get(index);
        if (child.IsValid()) {
            ret = child.GetName();
        }
    }
    return ret;
}

uint32 SenderStructuredData::GetNumberOfChildren() {
    return currentNode.Size();
}

}
