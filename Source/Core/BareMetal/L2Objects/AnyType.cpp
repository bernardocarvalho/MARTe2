/**
 * @file AnyType.cpp
 * @brief Source file for class AnyType
 * @date Nov 12, 2015
 * @author aneto
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
 * the class AnyType (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "Object.h"
#include "VariableDescriptor.h"
#include "MemoryCheck.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#include "Windows.h"

namespace MARTe {


/*
static bool CheckPointer (void const *p){
	printf ("%016p\n",p);

	bool ret = (p!= NULL);
    MEMORY_BASIC_INFORMATION mbi = {0};
    if (ret) ret = VirtualQuery(p, &mbi, sizeof(mbi));
    if (ret) {
//        DWORD mask = (PAGE_READONLY|PAGE_READWRITE|PAGE_WRITECOPY|PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE_WRITECOPY);
    	DWORD mask = (PAGE_READONLY|PAGE_READWRITE);
        ret = ((mbi.Protect & mask)!=0);
        // check the page is not a guard page
        ret = ret &&  ((mbi.Protect & (PAGE_GUARD|PAGE_NOACCESS))==0);

    }
    return ret;
}
*/


ErrorManagement::ErrorType AnyType::Dereference (uint32 index){

		char8 modifier;
		uint64 max;
		uint64 layerSize =  variableDescriptor.GetSize();
		ErrorManagement::ErrorType ret;
		if (!variableDescriptor.RemoveModifiersLayer(modifier,max)){
			ret.invalidOperation = true;
		}

		if (ret){
			ret.exception = !MemoryCheck::Check(pointer2Variable);
		}

		if (ret){
			switch(modifier){
			case 'P':
			case 'p':{
				char8 const  *ptr = * (reinterpret_cast<char8 const * const *>(pointer2Variable));

				uint64 step = layerSize * index;

				pointer2Variable = (ptr + step);
			}break;
			case 'A':
			case 'a':{
				char8 const *ptr = (reinterpret_cast<char8 const *>(pointer2Variable));

				if (index >= max){
					index = max -1;
				}
				uint64 step = layerSize * index;

				ptr = ptr + step;

				pointer2Variable = static_cast<const void *>(ptr);

			}break;
			case 'D':
			case 'd':
			case 'S':
			case 's':
			case 'Z':
			case 'z':{
				char8 const  *ptr = * (reinterpret_cast<char8 const * const *>(pointer2Variable));
				uint64 step = index;

				while (step > 0 ){
					int i;
					for (i = 0; ((i<layerSize) && ptr[i] == '\0'); i++ );
					if (i != layerSize){
						ptr+= layerSize;
					} else {
						step = 0;
					}
				}

				pointer2Variable = ptr ;

			}break;
			case 'M':
			case 'm':{
				Matrix<const char8> const * matrix = (reinterpret_cast< Matrix<const char8> const *>(pointer2Variable));
				max = matrix->GetNumberOfRows();
				if (index >= max){
					index = max -1;
				}

				char8 const * ptr = static_cast< char8 const *> (matrix->GetDataPointer());

				if (matrix->IsStaticDeclared()){
					uint64 step = layerSize * index;
					ptr += step;

				} else {
					if (!MemoryCheck::Check(pointer2Variable)){
						char8 const * const * pptr = reinterpret_cast< char8 const * const *> (ptr);
						ptr = pptr[index];
					} else {
						ret.exception = true;
					}
				}

				pointer2Variable = ptr;
				// need to add to the variableDescriptor the fact that we have now a Array layer left
				variableDescriptor.InsertModifiersLayer('a',matrix->GetNumberOfColumns());


			}break;
			case 'V':
			case 'v':{
				Vector<const char8> const *vector = (reinterpret_cast<Vector<const char8> const *>(pointer2Variable));
				char8 const * ptr = static_cast<char8 const *>(vector->GetDataPointer());
				max = vector->GetNumberOfElements();

				if (index >= max){
					index = max -1;
				}
				uint64 step = layerSize * index;

				ptr = ptr + step;

				pointer2Variable = ptr;

			}break;
			default:{
				ret.internalSetupError = true;
			}

			}
		}

		return ret;
	}



}


