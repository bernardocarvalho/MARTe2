/**
 * @file HttpService.cpp
 * @brief Source file for class HttpService
 * @date 24 ago 2018
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
 * the class HttpService (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpService.h"
#include "ObjectRegistryDatabase.h"
#include "HttpStream.h"
#include "Select.h"
#include "HttpRealmI.h"
#include "HttpInterface.h"
#include "ReferenceContainerFilterNameAndType.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

HttpService::HttpService() :
        MultiClientService(*(embeddedMethod = new EmbeddedServiceMethodBinderT<HttpService>(*this, &HttpService::ServerCycle))) {
    port = 0u;
}

HttpService::~HttpService() {
    delete embeddedMethod;
    // Auto-generated destructor stub for HttpService
    // TODO Verify if manual additions are needed
}

bool HttpService::Initialise(StructuredDataI &data) {
    bool ret = MultiClientService::Initialise(data);
    if (ret) {
        if (!data.Read("Port", port)) {
            port = 80;
            REPORT_ERROR(ErrorManagement::Information, "Port not specified: using default %d", port);
        }

        if (!data.Read("HttpRelayURL", httpRelayURL)) {
            httpRelayURL = "http://localhost:8080";
            REPORT_ERROR(ErrorManagement::Information, "Port not specified: using default %s", httpRelayURL.Buffer());
        }

        if (!data.Read("ListenMaxConnections", listenMaxConnections)) {
            listenMaxConnections = 255;
            REPORT_ERROR(ErrorManagement::Information, "ListenMaxConnections not specified: using default %d", listenMaxConnections);
        }

        StreamString webRootPath;
        if (!data.Read("WebRoot", webRootPath)) {
            if (ReferenceContainer::Size() > 0u) {
                webRoot = this->Find("WebRoot");

            }
        }
        else {
            webRoot = ObjectRegistryDatabase::Instance()->Find(webRootPath.Buffer());
        }
        ret = webRoot.IsValid();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "WebRoot invalid or not specified");
        }
    }

    return ret;
}

ErrorManagement::ErrorType HttpService::ClientService(TCPSocket *commClient) {
    ErrorManagement::ErrorType err = (commClient == NULL);

    if (err.ErrorsCleared()) {
        commClient->SetBlocking(true);
        HttpStream hstream(*commClient);

        Select sel;
        (void)sel.AddReadHandle(*commClient);
        //give the possibility to stop the thread
        if(sel.WaitUntil(1000u)) {

            if (!hstream.ReadHeader()) {
                err=ErrorManagement::CommunicationError;
                REPORT_ERROR(ErrorManagement::CommunicationError, "Error while reading HTTP header");
            }
            ReferenceT<HttpInterface> hi;
            ReferenceT<HttpRealmI> realm;
            bool pagePrepared = false;

            if (err.ErrorsCleared()) {
                StreamString path=hstream.GetPath();
                if (path.Size() > 0) {
                    // search for destination
                    uint32 occurrences=1u;
                    uint32 mode= ReferenceContainerFilterMode::PATH;
                    ReferenceContainerFilterNameAndType<HttpInterface> filter(occurrences, mode, path.Buffer());
                    ReferenceContainer results;
                    webRoot->Find(results, filter);
                    if(results.Size()>0u) {
                        uint32 last=results.Size()-1u;
                        hi=results.Get(last);
                    }

                    if(hi.IsValid()) {
                        realm=hi->GetRealm();
                    }
                    else {
                        hi = webRoot;
                        if (hi.IsValid()) {
                            if ((hi->GetRealm()).IsValid()) {
                                realm = hi->GetRealm();
                            }
                        }
                    }

                    // save remainder of address
                    uint32 remAddrIndex=filter.GetRemainedAddrIndex();
                    StreamString urlTemp=hstream.GetUrl();
                    StreamString unmatchedUrl=&urlTemp.Buffer()[remAddrIndex];

                    uint32 newUrlLastCharIdx=(unmatchedUrl.Size()-1u);
                    if (unmatchedUrl.Buffer()[newUrlLastCharIdx] == '/') {
                        unmatchedUrl.SetSize(newUrlLastCharIdx);
                    }
                    hstream.SetUnmatchedUrl(unmatchedUrl.BufferReference());
                }

                if (hi.IsValid()) {
                    // check security
                    //            GCRTemplate<HttpRealm> realm = searchFilter.GetRealm();
                    if (realm.IsValid()) {
                        if (!hstream.SecurityCheck(realm, (commClient->GetSource()).GetAddressAsNumber())) {
                            AnyType args[]= {"Content-Type"};
                            hstream.SwitchPrintAndCommit("OutputHttpOtions", "text/html", "%s", args);

                            StreamString realmMsg;
                            realm->GetAuthenticationRequest(realmMsg);
                            args[0]=realmMsg.Buffer();

                            hstream.SwitchPrintAndCommit("OutputHttpOtions", "WWW-Authenticate", "%s", args);

                            hstream.Printf("%s","<HTML><HEAD>\n"
                                    "<TITLE>401 Authorization Required</TITLE>\n"
                                    "</HEAD><BODY>\n"
                                    "<H1>Authorization Required</H1>\n"
                                    "This server could not verify that you\n"
                                    "are authorized to access the document you\n"
                                    "requested.  Either you supplied the wrong\n"
                                    "credentials (e.g., bad password), or your\n"
                                    "browser doesn't understand how to supply\n"
                                    "the credentials required.<P>\n"
                                    "</BODY></HTML>\n");

                            // force reissuing of a new thread
                            hstream.SetKeepAlive(false);
                            if (!hstream.WriteReplyHeader(true, 401)) {
                                err=ErrorManagement::CommunicationError;
                                REPORT_ERROR(ErrorManagement::CommunicationError, "Error while writing page back\n");
                            }
                            pagePrepared = true;
                        }
                    }

                    if ((!pagePrepared) && (err.ErrorsCleared())) {
                        pagePrepared = hi->ProcessHttpMessage(hstream);
                    }
                }
            }
            if (err.ErrorsCleared()) {

                if (!pagePrepared) {
                    AnyType args[]= {"text/html"};
                    hstream.SwitchPrintAndCommit("OutputHttpOtions", "Content-Type", "%s", args);

                    if (!hstream.KeepAlive()) {
                        args[0]="Close";

                        hstream.SwitchPrintAndCommit("OutputHttpOtions", "Connection", "%s", args);

                        hstream.Printf("%s", "<HTML>Page Not Found!</HTML>");
                        if(!hstream.WriteReplyHeader(true, 200)) {
                            err=ErrorManagement::CommunicationError;
                            REPORT_ERROR(ErrorManagement::CommunicationError, "Error while writing page back\n");
                        }
                    }
                }
                if (err.ErrorsCleared()) {
                    if (!hstream.KeepAlive()) {
                        err=ErrorManagement::Completed;
                    }
                }
            }
        }
    }

    return err;

}

ErrorManagement::ErrorType HttpService::ServerCycle(MARTe::ExecutionInfo &information) {
    ErrorManagement::ErrorType err;
    if (information.GetStage() == MARTe::ExecutionInfo::StartupStage) {
        //todo bind and initialisation?? Maybe the listen goes outside??
        err = !(server.Listen(port, listenMaxConnections));
    }
    if (information.GetStage() == MARTe::ExecutionInfo::MainStage) {
        TCPSocket *commClient = NULL;
        if (information.GetStageSpecific() == MARTe::ExecutionInfo::WaitRequestStageSpecific) {
            //wait for the next connection
            if(commClient!=NULL) {
                delete commClient;
            }
            commClient = new TCPSocket();
            if (server.WaitConnection(msecTimeout, commClient) == NULL) {
                err=MARTe::ErrorManagement::Timeout;
            }
            else {
                err= MARTe::ErrorManagement::NoError;
            }
        }
        if (information.GetStageSpecific() == MARTe::ExecutionInfo::ServiceRequestStageSpecific) {
            bool completed=ClientService(commClient);
            if (completed) {
                err= MARTe::ErrorManagement::Completed;
            }
            else {
                err= MARTe::ErrorManagement::FatalError;
            }
        }
    }

    return err;
}

CLASS_REGISTER(HttpService, "1.0")

}
