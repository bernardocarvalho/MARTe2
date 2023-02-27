Setting the environment
-----------------------

Make sure that all the environment variables are correctly exported. ::
    
    export MARTe2_DIR=~/Projects/MARTe2-dev
    export MARTe2_Components_DIR=~/Projects/MARTe2-components
    export OPEN62541_DIR=~/Projects/open62541
    export OPEN62541_LIB=~/Projects/open62541/build/bin
    export OPEN62541_INCLUDE=~/Projects/open62541/build
    export EPICS_BASE=~/Projects/epics-base-7.0.2
    export EPICSPVA=~/Projects/epics-base-7.0.2
    export EPICS_HOST_ARCH=linux-x86_64
    export PATH=$PATH:$EPICS_BASE/bin/$EPICS_HOST_ARCH
    export SDN_CORE_INCLUDE_DIR=~/Projects/SDN_1.0.12_nonCCS/src/main/c++/include/
    export SDN_CORE_LIBRARY_DIR=~/Projects/SDN_1.0.12_nonCCS/target/lib/
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/x86-linux/Core/:$EPICS_BASE/lib/$EPICS_HOST_ARCH:$SDN_CORE_LIBRARY_DIR
    cd ~/Projects/MARTe2-dev
    make -f Makefile.linux
    cd ~/Projects/MARTe2-components
    make -f Makefile.linux
    cd ~/Projects/MARTe2-demos-padova
    make -f Makefile.x86-linux

Disable firewall rules (otherwise the communication with EPICS may not work): ::

    iptable -F

Export all variables permanently (assumes that the relative paths above were used!). ::

    cp marte2-exports.sh /etc/profile.d/


Now that the environment is ready, the next section will introduce the general structure of a Control System.