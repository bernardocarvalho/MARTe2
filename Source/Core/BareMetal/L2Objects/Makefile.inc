#############################################################
#
# Copyright 2015 F4E | European Joint Undertaking for ITER 
#  and the Development of Fusion Energy ('Fusion for Energy')
# 
# Licensed under the EUPL, Version 1.1 or - as soon they 
# will be approved by the European Commission - subsequent  
# versions of the EUPL (the "Licence"); 
# You may not use this work except in compliance with the 
# Licence. 
# You may obtain a copy of the Licence at: 
#  
# http://ec.europa.eu/idabc/eupl
#
# Unless required by applicable law or agreed to in 
# writing, software distributed under the Licence is 
# distributed on an "AS IS" basis, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
# express or implied. 
# See the Licence for the specific language governing 
# permissions and limitations under the Licence. 
#
#############################################################

PACKAGE=Core/BareMetal
ROOT_DIR=../../../..
MARTe2_MAKEDEFAULT_DIR?=$(ROOT_DIR)/MakeDefaults

OBJSX=  AnyType.x \
		ClassMethodCaller.x\
		ClassMethodInterfaceMapper.x\
		ClassProperties.x \
		ClassRegistryDatabase.x \
		ClassRegistryItem.x \
		Introspection.x2\
		IntrospectionEntry.x2 \
		Object.x \
		ObjectRegistryDatabase.x \
		Reference.x \
		ReferenceContainer.x2 \
		ReferenceContainerNode.x2 \
		ReferenceContainerFilter.x \
		ReferenceContainerFilterReferences.x \
		ReferenceContainerFilterObjectName.x

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)

INCLUDES+=-I../L0Types
INCLUDES+=-I../L1Portability

all: $(OBJS) $(SUBPROJ)   \
		$(BUILD_DIR)/L2ObjectsB$(LIBEXT) \
		$(BUILD_DIR)/L2ObjectsB.def
	echo  $(OBJS)

include depends.$(TARGET)

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)

