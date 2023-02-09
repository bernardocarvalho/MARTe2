Download all the needed software
--------------------------------
--------------------------------

Open a terminal, install epel repository and update the distribution: ::
    yum -y install epel-release
    yum -y update

Install git: ::

    yum -y install git

Install all the standard development tools, the cmake3 compiler and octave: ::

    yum -y groups install "Development Tools"
    yum -y install wget cmake3 octave libxml2 libxml2-devel bc

Solve dependencies for MARTe2 and EPICS:  ::

    yum -y install ncurses-devel readline-devel

Install Python and Perl Parse utilities for open62541: ::

    yum -y install python-dateutil python-six perl-ExtUtils-ParseXS

Install MDSplus ::

    yum -y install http://www.mdsplus.org/dist/el7/stable/RPMS/noarch/mdsplus-repo-7.50-0.el7.noarch.rpm
    yum -y install mdsplus-kernel* mdsplus-java* mdsplus-python* mdsplus-devel*

Create a folder named Projects and clone MARTe2 Core,  MARTe2 components and the MARTe2 demos: ::

    mkdir ~/Projects
    cd ~/Projects
    git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git MARTe2-dev
    git clone -b \master https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-components.git
    git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-demos-padova.git

Download EPICS R70.2: ::

    git clone -b R7.0.2 --recursive https://github.com/epics-base/epics-base.git epics-base-7.0.2

Clone open62541 v1.0 library: ::

    git clone -b 1.0 https://github.com/open62541/open62541.git

Download SDN: ::

    wget https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2-demos-padova/raw/develop/Other/SDN_1.0.12_nonCCS.tar.gz
    tar zxvf SDN_1.0.12_nonCCS.tar.gz
