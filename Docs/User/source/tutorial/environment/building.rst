Building libraries and frameworks
---------------------------------
---------------------------------

Build Open623541:
    

    cd ~/Projects/open62541

    Open the CMakeLists.txt file and comment the lines marked with # below
        if("${CMAKE_VERSION}" VERSION_GREATER 3.9)
            cmake_policy(SET CMP0069 NEW) # needed as long as required cmake < 3.9
            #include(CheckIPOSupported)
            #check_ipo_supported(RESULT CC_HAS_IPO) # Inter Procedural Optimization / Link Time Optimization (should be same as -flto)
            #if(CC_HAS_IPO)
            #    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
            #endif()
        endif()

    Start building the open62541 library: ::

    mkdir ~/Projects/open62541/build
    cd ~/Projects/open62541/build
    cmake3 -DUA_ENABLE_AMALGAMATION=ON ..
    make

Compile EPICS: ::

    cd ~/Projects/epics-base-7.0.2
    echo "OP_SYS_CXXFLAGS += -std=c++11" >> configure/os/CONFIG_SITE.linux-x86_64.Common
    make

Compile SDN: ::

    cd ~/Projects/SDN_1.0.12_nonCCS
    make