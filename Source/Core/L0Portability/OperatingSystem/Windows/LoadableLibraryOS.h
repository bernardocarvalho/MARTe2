/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id: $
 *
 **/
/**
 * @file
 * @brief Loadable library implementation in Linux
 */
#ifndef LOADABLE_LIBRARY_OS_H
#define LOADABLE_LIBRARY_OS_H

#include <Winbase.h>

class LoadableLibraryOS {

public:

    /**
     * @see LoadableLibrary::Close
     * @brief Close a library previously opened dinamically.
     * @param ll is the library to close.
     */
    static void Close(LoadableLibrary &ll) {
        if (ll.GetModule() != 0) {
            FreeLibrary((HMODULE) ll.GetModule());
        }
    }

    /**
     * @see LoadableLibrary::Open
     * @brief Open a library dinamically.
     * @param ll is the library in return.
     * @param dllName is the name of the library.
     * @return true if the requested library is found and opened.
     */
    static bool Open(LoadableLibrary &ll,
                     char8 const * const dllName) {

        if (ll.GetModule() != 0) {
            Close(ll);
        }

        ll.SetModule(LoadLibrary(dllName));
        return ll.GetModule() != NULL;

    }

    /**
     * @see LoadableLibrary::Function.
     * @brief Load a function of a library dinamically.
     * @param ll is the library where the requested function is.
     * @param name is the name of the function.
     * @return a pointer to the requested function.
     */
    static void *Function(LoadableLibrary &ll,
                          char8 const * const name) {

        void* ret = NULL;
        if ((ll.GetModule() != NULL) && (name != NULL)) {

            ret = GetProcAddress((HMODULE) ll.GetModule(), name);
        }

        return ret;
    }

};

#endif
