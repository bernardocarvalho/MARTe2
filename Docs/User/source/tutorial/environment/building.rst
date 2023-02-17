Building 
--------

**Open623541**

The Open Platform Communications (OPC) standard, which is a series of standards and specifications for industrial telecommunication. They are based on Object Linking and Embedding (OLE) for process control. OPC specifies the communication of real-time plant data between control devices from different manufacturers.

The OPC Unified Architecture (UA), released in 2008, is a platform independent service-oriented architecture that integrates all the functionality of the individual OPC Classic specifications into one extensible framework. In our case, the OPCUA DataSource relies on the open62541 library.

These are the steps to follow to get it ready to work. In a terminal, write: ::

    cd ~/Projects/open62541;

Open the CMakeLists.txt file and comment the lines marked with # below ::

    if("${CMAKE_VERSION}" VERSION_GREATER 3.9);
        cmake_policy(SET CMP0069 NEW) # needed as long as required cmake < 3.9;
        #include(CheckIPOSupported);
        #check_ipo_supported(RESULT CC_HAS_IPO) # Inter Procedural Optimization / Link Time Optimization (should be same as -flto);
        #if(CC_HAS_IPO);
        #    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON);
        #endif();
    endif();

Start building the open62541 library: ::

    mkdir ~/Projects/open62541/build;
    cd ~/Projects/open62541/build;
    cmake3 -DUA_ENABLE_AMALGAMATION=ON ..;
    make;


**EPICS:**

The Experimental Physics and Industrial Control System (EPICS) is a set of software tools and applications used to develop and implement distributed control systems to operate devices such as particle accelerators, telescopes and other large scientific facilities. The tools are designed to help develop systems which often feature large numbers of networked computers delivering control and feedback. They also provide SCADA capabilities. ::

    cd ~/Projects/epics-base-7.0.2;
    echo "OP_SYS_CXXFLAGS += -std=c++11" >> configure/os/CONFIG_SITE.linux-x86_64.Common;
    make;

**SDN**

Software-defined networking (SDN) technology is an approach to network management that enables dynamic, programmatically efficient network configuration in order to improve network performance and monitoring, making it more like cloud computing than traditional network management. ::

    cd ~/Projects/SDN_1.0.12_nonCCS;
    make;