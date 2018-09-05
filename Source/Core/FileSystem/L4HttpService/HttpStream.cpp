/**
 * @file HttpStream.cpp
 * @brief Source file for class HttpStream
 * @date 17 ago 2018
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
 * the class HttpStream (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpStream.h"
#include "HttpDefinition.h"
#include "AdvancedErrorManagement.h"
#include "BasicSocket.h"
#include "DoubleBufferedStream.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
using namespace HttpDefinition;

//#define NULL_PTR(x) NULL

HttpStream::HttpStream(DoubleBufferedStream &clientBufferedStreamIn) {
    httpCommand = HSHCNone;
    httpVersion = 1000;
    httpErrorCode = 200;
    keepAlive = true;
    socketStream = &clientBufferedStreamIn;
    lastUpdateTime = 0ull;
    /** unknown information length */
    unreadInput = -1;
}

HttpStream::~HttpStream() {
    // Auto-generated destructor stub for HttpStream
    // TODO Verify if manual additions are needed
}

bool HttpStream::CompleteReadOperation(BufferedStreamI *streamout,
                                       TimeoutType msecTimeout) {

    bool ret = true;
    //This way we can change the falsely undefined content-length when this is called from Write Header
    //complete the read only when the body is completed from the other part
    if ((streamout == NULL) && (unreadInput < 0)) {
        unreadInput = -1;
    }
    else {
        uint64 startCounter = HighResolutionTimer::Counter();

        // convert the stop time
        uint64 maxTicks = startCounter + static_cast<uint64>((msecTimeout.GetTimeoutMSec() / 1000.0) * HighResolutionTimer::Frequency());

        uint32 bufferSize = 1024u;
        char8 *buffer = new char8[bufferSize];
        uint32 readSize = 1u;
        uint32 sizeToRead = bufferSize;

        if (unreadInput > 0) {
            sizeToRead = unreadInput;
            //clip the size
            if (sizeToRead > bufferSize) {
                sizeToRead = bufferSize;
            }
        }

        while ((sizeToRead > 0u) && (readSize > 0u) && (ret)) {

            ret = socketStream->Read(buffer, sizeToRead, msecTimeout);

            if ((ret) || (sizeToRead>0u)) {
                readSize = sizeToRead;
                if (streamout!=NULL_PTR(BufferedStreamI *)) {
                    uint32 sizeToWrite = readSize;
                    //complete write?
                    streamout->Write(buffer, sizeToWrite, msecTimeout);
                }
                sizeToRead = bufferSize;

                if (unreadInput > 0) {
                    unreadInput -= readSize;
                    sizeToRead = unreadInput;
                    //clip the size
                    if (sizeToRead > bufferSize) {
                        sizeToRead = bufferSize;
                    }
                }
                if (msecTimeout.IsFinite()) {
                    uint64 lastCounter = HighResolutionTimer::Counter();
                    uint64 ticksLeft = (maxTicks - lastCounter);
                    msecTimeout.SetTimeoutHighResolutionTimerTicks(ticksLeft);
                }
            }
            else {
                //REPORT_ERROR_STATIC(ErrorManagement::Timeout, "Possible Timeout on completion");
            }
        }

        delete[] buffer;
    }
    return (unreadInput <= 0);

}

bool HttpStream::WriteHeader(bool bodyCompleted,
                             HSHttpCommand command,
                             const char8 * url) {

    //if sending something with bodyCompleted=false
    //remember to write Transfer-Encoding: chunked in options

// complete transaction with remote host
    CompleteReadOperation(NULL_PTR(BufferedStreamI*));

// if it is a reply get errorCode
// otherwise mark the httpCommand as none
    bool isReply = IsReplyCode(command, httpErrorCode);
    if (!isReply) {
        httpCommand = HSHCNone;
    }

    // saves all the pending changes
    bool ret = true;

    // assemble the header
    // deal with a reply
    uint32 majorVersion = (httpVersion / 1000u);
    uint32 minorVersion = ((httpVersion % 1000u) / 100u);

    const char8* urlToUse = url;
    if (urlToUse == NULL) {
        urlToUse="";
    }

    if (isReply) {
        if (!socketStream->Printf("HTTP/%i.%i %i %s\r\n", majorVersion, minorVersion, httpErrorCode, GetErrorCodeString(httpErrorCode))) {
            //REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Write on socket failed\n");
            ret = false;
        }
    }
    else if (command == HSHCGet) {
        if (!socketStream->Printf("GET %s HTTP/%i.%i\r\n", urlToUse, majorVersion, minorVersion)) {
            REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Write on socket failed\n");
            ret = false;
        }
    }
    else if (command == HSHCPut) {
        if (!socketStream->Printf("PUT %s HTTP/%i.%i\r\n", urlToUse, majorVersion, minorVersion)) {
            REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Write on socket failed\n");
            ret = false;
        }
    }
    else if (command == HSHCPost) {
        if (!socketStream->Printf("POST %s HTTP/%i.%i\r\n", urlToUse, majorVersion, minorVersion)) {
            REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Write on socket failed\n");
            ret = false;
        }
    }
    else if (command == HSHCHead) {
        if (!socketStream->Printf("HEAD %s HTTP/%i.%i\r\n", urlToUse, majorVersion, minorVersion)) {
            REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Write on socket failed\n");
            ret = false;
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Command code %i unknown \n", command);
        ret = false;
    }

    if (ret) {

        data->MoveToRoot();
        if (!data->MoveRelative("OutputHttpOtions")) {
            ret = data->CreateRelative("OutputHttpOtions");
        }
        if (ret) {
            if (bodyCompleted) {
                ret = data->Write("Content-Length", Size());
            }
        }
        if (ret) {
            StreamString connection;
            if (data->Read("Connection", connection)) {
                if (StringHelper::CompareNoCaseSensN(connection.Buffer(), "keep-alive", 10u) == 0) {
                    keepAlive = true;
                }
                else if (StringHelper::CompareNoCaseSensN(connection.Buffer(), "close", 6u) == 0) {
                    keepAlive = false;
                }
            }
            // write all options
            uint32 numberOfChildren = data->GetNumberOfChildren();
            for (uint32 i = 0u; (i < numberOfChildren) && (ret); i++) {
                StreamString value;
                StreamString key = data->GetChildName(i);
                if (data->Read(key.Buffer(), value)) {
                    ret = socketStream->Printf("%s: %s\r\n", key.Buffer(), value.Buffer());
                    if (!ret) {
                        REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "write key %s on socket failed\n", key.Buffer());
                    }
                }
            }
            if (ret) {
                socketStream->Printf("%s", "\r\n");

                // return to root
                data->MoveToRoot();

                // send out the body
                uint32 toWrite = Size();
                ret = socketStream->Write(Buffer(), toWrite);
            }
        }
        if (ret) {
            //flush the stream
            socketStream->Flush();
        }
    }

    return ret;

}

bool HttpStream::RetrieveHttpCommand(StreamString &command,
                                     StreamString &line) {
    char8 terminator;

    bool ret = true;
    if (command == "GET") {
        httpCommand = HSHCGet;
    }
    else if (command == "PUT") {
        httpCommand = HSHCPut;
    }
    else if (command == "POST") {
        httpCommand = HSHCPost;
    }
    else if (command == "HEAD") {
        httpCommand = HSHCHead;
    }
    else if (StringHelper::CompareN(command.Buffer(), "HTTP", 4) == 0) {
        // in a reply there is no command
        // it starts with HTTP ...
        float32 fVersion = 0.0;
        const char8* versionStr = &(command.Buffer()[5u]);
        if (!TypeConvert(fVersion, versionStr)) {
            fVersion = 0.0;
        }

        //convert the version
        if (fVersion == 0.0) {
            httpVersion = 1000u;
        }
        else {
            httpVersion = static_cast<uint32>(fVersion * 1000.0);
        }

        //convert to the error code
        StreamString errorCode;
        (void) line.GetToken(errorCode, " \n\t", terminator, " \n\t");
        uint32 errorCodeInt;
        if (!TypeConvert(errorCodeInt, errorCode.Buffer())) {
            REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Failed converting the error code of the reply %s to an integer", errorCode.Buffer());
            ret = false;
        }
        else {
            httpCommand = GenerateReplyCode(errorCodeInt);
        }

    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Cannot deal with a %s request (GET/PUT/POST/HEAD only)", command.Buffer());
        ret = false;
    }
    return ret;
}

char8 HttpStream::BuildUrl(StreamString &line) {

    StreamString tempUrl;
    char8 terminator;
    //get the url
    (void) line.GetToken(tempUrl, " \n?", terminator, " \n?");
    tempUrl.Seek(0);
    StreamString decoded;
    //decode the url
    HttpDecode(decoded, tempUrl);
    decoded.Seek(0);
    bool ok = true;
    StreamString urlPart;
    char8 saveTerm;
    //build url and path
    url.SetSize(0);
    path.SetSize(0);
    while (ok) {
        urlPart = "";
        ok = decoded.GetToken(urlPart, "\\/", saveTerm);
        if (ok) {
            url += urlPart.Buffer();
            path += urlPart.Buffer();
            if (saveTerm != '\0') {
                url += saveTerm;
                path += '.';
            }
        }
    }
    unMatchedUrl = url;
    return terminator;
}

bool HttpStream::StoreCommands(StreamString &line) {
    StreamString commands;
    char8 terminator;
    (void) line.GetToken(commands, " \t", terminator);

    bool ret = data->MoveToRoot();
    if (ret) {
        //delete existing
        (void) data->Delete("InputCommands");
        ret = data->CreateRelative("InputCommands");
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to create node InputCommands");
        }
    }

    if (ret) {
        commands.Seek(0ull);
        StreamString command;
        char8 terminator;
        while ((commands.GetToken(command, "&", terminator)) && (ret)) {
            command.Seek(0ull);
            if (command.Size() > 3u) {
                StreamString variable;
                StreamString value;
                ret = command.GetToken(variable, "=", terminator);
                if (ret) {
                    //get until the end
                    (void) command.GetToken(value, "", terminator);
                    StreamString decodedValue;
                    StreamString decodedVariable;
                    value.Seek(0ull);
                    variable.Seek(0ull);
                    HttpDecode(decodedValue, value);
                    HttpDecode(decodedVariable, variable);
                    ret = data->Write(decodedVariable.Buffer(), decodedValue.Buffer());
                }
            }
            command.SetSize(0);
        }
    }
    return ret;
}

bool HttpStream::StoreOutputOptions() {
    bool ret = data->MoveToRoot();
    if (ret) {
        //delete existing
        (void) data->Delete("OutputHttpOtions");
        ret = data->CreateRelative("OutputHttpOtions");
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to create node OutputHttpOtions");
        }
    }
    return ret;
}

bool HttpStream::StoreInputOptions() {

    bool ret = data->MoveToRoot();
    char8 terminator;

    if (ret) {
//delete existing
        (void) data->Delete("InputHttpOptions");
        ret = data->CreateRelative("InputHttpOptions");
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to create node InputHttpOptions");
        }
    }
    if (ret) {
        bool ok = true;
        while (ok) {
            StreamString line;
            ret = socketStream->GetLine(line);
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Failed reading a line from socket");
            }

            if (ret) {
                ok = (line.Size() > 0u);
// parse HTTP Options and add to CDB
                if (ok) {
                    line.Seek(0);
                    StreamString key;
                    StreamString value;
                    ret = line.GetToken(key, " \t:", terminator);
                    if (ret) {
                        (void) line.GetToken(value, " \t", terminator);
                        // any other part separated by spaces add to the token
                        // use a space as separator
                        if (line.Size() > line.Position()) {
                            uint32 sizeW = 1u;
                            char8 space = ' ';
                            value.Write(&space, sizeW);
                        }
                        (void) line.GetToken(value, "", terminator);
                        ret = data->Write(key.Buffer(), value.Buffer());
                    }
                }
            }
        }
    }

    return ret;
}

bool HttpStream::HandlePostHeader(StreamString &line,
                                  StreamString &content,
                                  StreamString &name,
                                  StreamString &filename) {
    const char8 *temp = NULL;

    bool ret = true;
    //read the name (it is in the form name="_NAME_")
    const char8* nameTemp = StringHelper::SearchString(line.Buffer(), "name=\"");
    if (nameTemp != NULL) {
        name = &nameTemp[StringHelper::Length("name=\"")];
        temp = name.Buffer();
        uint32 count = 0u;
        while (temp[count] != '\"') {
            count++;
        }
        name.SetSize(count);
    }
    else {
        nameTemp = StringHelper::SearchString(line.Buffer(), "name=");

        if (nameTemp != NULL) {
            name = &nameTemp[StringHelper::Length("name=")];
        }
    }
    //Check if the content is a file
    const char8* filenameTemp = StringHelper::SearchString(line.Buffer(), "filename=\"");
    if (filenameTemp != NULL) {
        filename = &filenameTemp[StringHelper::Length("filename=\"")];
        temp = filename.Buffer();
        uint32 count = 0u;
        while (temp[count] != '\"') {
            count++;
        }
        filename.SetSize(count);
    }
    if (filename.Size() > 0u) {
        StreamString key = name;
        key += ":filename";

        //Write the filename
        ret = data->Write(key.Buffer(), filename.Buffer());
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed writing the filename");
        }
        if (ret) {
            //Check the file mime type
            line.SetSize(0);
            if (content.GetLine(line)) {
                const char8* fcTypeTemp = StringHelper::SearchString(line.Buffer(), "Content-Type: ");
                if (fcTypeTemp != NULL) {
                    StreamString fcType = &fcTypeTemp[StringHelper::Length("Content-Type: ")];
                    key = name;
                    key += ":Content-Type";
                    ret = data->Write(key.Buffer(), fcType.Buffer());
                    if (!ret) {
                        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed writing the file content type");
                    }
                }
            }
        }
    }
    return ret;
}

bool HttpStream::HandlePostContent(StreamString &line,
                                   StreamString &boundary,
                                   StreamString &name,
                                   StreamString &filename,
                                   StreamString &value,
                                   bool &headerHandled) {

    bool ret = true;
    //search for the boundary end
    if (StringHelper::SearchString(line.Buffer(), boundary.Buffer()) != NULL) {

        //reset...new blank line is new handler
        headerHandled = false;

        ret = data->Write(name.Buffer(), value.Buffer());
        if (!ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed writing the content");
        }
        if (ret) {
            value.SetSize(0);
            line.SetSize(0);
            filename.SetSize(0);
            name.SetSize(0);
        }
    }
    //if not boundary store in value
    else {
        if (value.Size() > 0u) {
            value += "\n";
        }
        value += line;
    }

    return ret;
}

bool HttpStream::HandlePostMultipartFormData(StreamString &contentType,
                                             StreamString &content) {

    const char8* parsedBoundaryTemp = StringHelper::SearchString(contentType.Buffer(), "boundary=");
    bool ret = (parsedBoundaryTemp != NULL);
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "In multipart/form-data a boundary must be specified");
    }

    if (ret) {
        //Store the boundary identifier
        StreamString parsedBoundary = &parsedBoundaryTemp[StringHelper::Length("boundary=")];
        //Check if the boundary contains quotes and remove if so
        if (parsedBoundary.Size() > 1u) {
            // remove the ""
            if ((parsedBoundary[0] == '\"') && parsedBoundary[parsedBoundary.Size() - 1] == '\"') {
                parsedBoundary = (parsedBoundary.Buffer() + 1u);
                parsedBoundary.SetSize(parsedBoundary.Size() - 1);
            }
        }
        //add the prefix
        StreamString boundary = "--";
        boundary += parsedBoundary;

        //The header is not written in the cdb
        bool headerHandled = false;
        //The contents (after the header are stored in this field
        StreamString value;

        content.Seek(0);
        StreamString line;
        //The cdb name key
        StreamString name = "Unknown";
        //If a filename is received
        StreamString filename;
        while ((content.GetLine(line)) && (ret)) {
            //The header is handled when an empty line is detected
            headerHandled |= (line.Size() == 0);
            if (line.Size() > 0u) {
                //While the header is not handled (separated from main content by empty line)
                //look for name and filename
                if (!headerHandled) {
                    //store name and filename
                    ret = HandlePostHeader(line, content, name, filename);
                }
                //if the header is handled
                else {
                    //parse and store the actual message content
                    ret = HandlePostContent(line, boundary, name, filename, value, headerHandled);
                }
                line.SetSize(0);
            }
        }
    }
    return ret;
}

bool HttpStream::HandlePostApplicationForm(StreamString &contentType,
                                           StreamString &content) {

    bool ret = true;
    StreamString line;

    if (content.GetLine(line)) {
        line.Seek(0);

        StreamString command;
        char8 terminator;
        while (line.GetToken(command, "&", terminator)) {
            command.Seek(0);
            if (command.Size() > 3u) {
                StreamString variable;
                StreamString value;
                ret = command.GetToken(variable, "=", terminator);
                if (ret) {
                    (void) command.GetToken(value, ";", terminator);
                    value.Seek(0);
                    variable.Seek(0);
                    StreamString decodedValue;
                    StreamString decodedVariable;
                    HttpDecode(decodedValue, value);
                    HttpDecode(decodedVariable, variable);
                    decodedValue.Seek(0);
                    ret = data->Write(decodedVariable.Buffer(), decodedValue.Buffer());
                }
            }
            command.SetSize(0);
        }
    }

    return ret;
}

bool HttpStream::HandlePost(StreamString &contentType,
                            StreamString &content) {
    //Write the raw content
    content.Seek(0);
    StreamString key = "rawPost";

    //we are in InputCommands
    bool ret = data->Write(key.Buffer(), content.Buffer());
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed writing the raw post content");
    }

    if (ret) {
        content.Seek(0);

        //Check if it is a "multipart/form-data"
        if (StringHelper::SearchString(contentType.Buffer(), "multipart/form-data") != NULL) {
            ret = HandlePostMultipartFormData(contentType, content);
        }
        else if (StringHelper::SearchString(contentType.Buffer(), "application/x-www-form-urlencoded") != NULL) {
            //read the content. Key values encoded as in a GET url
            ret = HandlePostApplicationForm(contentType, content);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Content-type handler for: %s not found", contentType.Buffer());
            ret = false;
        }
    }
    return ret;
}

/** uses this stream to communicate with client
 before exiting reads header  */
bool HttpStream::ReadHeader() {
    /** unknown information length */
    unreadInput = -1;
    lastUpdateTime = HighResolutionTimer::Counter();

    StreamString line;
    char8 terminator;
    // Reads the HTTP command
    bool ret = socketStream->GetLine(line, false);
    if (!ret) {
        REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Failed reading a line from socket");
    }
    if (ret) {
        line.Seek(0ull);
        StreamString command;
        //check the arrived command
        (void) line.GetToken(command, " \n\t", terminator, " \n\t");
        ret = RetrieveHttpCommand(command, line);
    }

    // if httpCommand is a HSHCReply then the version has already been calculated
    if (HSHCReply > httpCommand) {

        if (ret) {
            // extract the uri and build a path based n that
            char8 termChar = BuildUrl(line);
            if (termChar == '?') {
                // extracts commands
                ret = StoreCommands(line);
            }
        }

        //store the HTTP version
        if (ret) {
            StreamString version;
            ret = line.GetToken(version, " \n\t", terminator, " \n\t");
            if (ret) {
                float32 fVersion = 0.f;
                //skip HTTP
                const char8 *fVersionStr = &(version.Buffer()[5u]);
                if (!TypeConvert(fVersion, fVersionStr)) {
                    fVersion = 0.0f;
                }
                if (fVersion == 0.0f) {
                    httpVersion = 1000u;
                }
                else {
                    httpVersion = static_cast<uint32>(fVersion * 1000.0);
                }
            }
        }
    }

    if (ret) {
        ret = StoreInputOptions();
    }

    StreamString contentType;
    if (ret) {
        // now evaluate options
        // first check what policy to follow: if dataSize is available and connection keep-alive,
        // then do not shut the connection and load only the specified size
        keepAlive = (httpVersion >= 1100);
        StreamString connection;
        if (data->Read("Connection", connection)) {
            if (StringHelper::CompareNoCaseSensN(connection.Buffer(), "keep-alive", 10u) == 0) {
                keepAlive = true;
            }
            else if (StringHelper::CompareNoCaseSensN(connection.Buffer(), "closed", 6u) == 0) {
                keepAlive = false;
            }

        }
        if (!data->Read("Content-Length", unreadInput)) {
            unreadInput = HTTPNoContentLengthSpecified;
        }

        if (!data->Read("Content-Type", contentType)) {
            contentType = "";
        }

    }

    //write the peer in the configuration
    if (ret) {
        //HTTP 1.1 might require to reply to 100-continue so that the client will continue to send more information
        StreamString expectStr;
        if (!data->Read("Expect", expectStr)) {
            expectStr = "";
        }
        if (expectStr == "100-continue") {
            socketStream->Printf("%s", "HTTP/1.1 100 Continue\r\n");
            socketStream->Flush();
        }
        ret = data->MoveToRoot();
        if (ret) {
            StreamString peer = "";
            BasicSocket *clientSocket = dynamic_cast<BasicSocket *>(socketStream);
            if (clientSocket != NULL) {
                // this could be very slow I think ip number will suffice
                // clientSocket->Source().HostName(peer);
                peer = (clientSocket->GetSource()).GetAddress();
                if (peer == "127.0.0.1") {
                    peer = InternetHost::GetLocalAddress();
                }
                ret = data->Write("Peer", peer.Buffer());
            }
        }
    }

    if (ret) {
        //If it post read the body
        if (httpCommand == HSHCPost) {
            ret = (contentType.Size() > 0u);
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "If using POST the Content-Type MUST be specified");
            }

            if (ret) {
                ret = (unreadInput > 0);
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "If using POST the Content-Length MUST be specified");
                }
            }

            if (ret) {
                //???absolute or relative??
                if (!data->MoveRelative("InputCommands")) {
                    (void) data->Delete("InputCommands");
                    ret = data->CreateRelative("InputCommands");
                    if (!ret) {
                        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to create node InputCommands");
                    }
                }

                if (ret) {
                    StreamString postContent;
                    char8 buffer[1024];
                    while ((unreadInput > 0) && (ret)) {
                        uint32 bufferReadSize = 1024u;
                        ret = socketStream->Read(buffer, bufferReadSize);
                        if (!ret) {
                            REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Failed reading from socket in POST section");
                        }

                        if (ret) {
                            if (bufferReadSize > 0u) {
                                ret = (postContent.Write(buffer, bufferReadSize));
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Failed updating content buffer in POST section");
                                }
                            }
                            unreadInput -= bufferReadSize;
                        }
                    }
                    if (ret) {
                        ret = (HandlePost(contentType, postContent));
                        if (!ret) {
                            REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "Error handling the post");
                        }
                    }
                    if (ret) {
                        ret = data->MoveToRoot();
                    }
                }
            }
        }
    }

    httpErrorCode = 200;
    return ret;
}

bool HttpStream::SecurityCheck(ReferenceT<HttpRealmI> realm) {
    bool ret = false;

    // no valid realm !
    if (realm.IsValid()) {
        // get key. on failure exit
        StreamString authorisationKey;
        if (data->MoveRelative("InputHttpOptions")) {
            if (data->Read("Authorization", authorisationKey)) {
                BasicSocket* socket = dynamic_cast<BasicSocket *>(socketStream);
                if (socket != NULL) {
                    ret = realm->Validate(authorisationKey.Buffer(), httpCommand, (socket->GetSource()).GetAddressAsNumber());
                }
            }
            (void) data->MoveToAncestor(1u);
        }
    }
    return ret;
}

bool HttpStream::KeepAlive() const {
    return keepAlive;
}

void HttpStream::SetKeepAlive(bool isKeepAlive) {
    keepAlive = isKeepAlive;
}

HSHttpCommand HttpStream::GetHttpCommand() const {
    return httpCommand;

}

void HttpStream::SetUnmatchedUrl(const char8 *unMatchedUrlIn) {
    unMatchedUrl = unMatchedUrlIn;
}

void HttpStream::GetUnmatchedUrl(StreamString& unmatchedUrlOut) {
    unmatchedUrlOut = unMatchedUrl;
}

void HttpStream::GetPath(StreamString& pathOut) {
    pathOut = path;
}

void HttpStream::GetUrl(StreamString& urlOut) {
    urlOut = url;
}

}

