General Common Structure
------------------------
------------------------

The examples shown in this tutorial share the following common general structure, which describes in general terms, the main parts of any kind of Control System: 

.. image:: ./general_scheme.png
  :width: 800
  :alt: Common scheme for Control Systems


In general, we can differentiate the following parts in a Control System: 

* **Input/Output**: The hardware that we need to control is connected to the Control System by a set of instruments providing input and output signals. These signals may come from different hardware platforms, such as PXI, SDN, etc.

* **Measurement and Control**: The input signals are measured and passed to the Control System - in our case the State Space GAM - where the control algorithms that will stablish the controlling rules sit. These controlling roules are implemented by means of a set of states and their associated messages. This is then provided as output to the instrumentation that will update the system as needed.

* **Storage**: In addition to establishing the rules of the system and receiving from/loading into the instrumentation the appropriate information, the Control System should store all the relevant information in a database (EPICS, MDS+, etc.) for subsequent use.

In our case, MARTe2 provides support to all these activities via a set of GAMs, which may come included in the MARTe2-components library or that can be programmed by the end-user. Some of the ones included in the MARTe2-components library used in the examples in this training are the following:

**Input/Output**:

* `IOGAM <https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1IOGAM.html#details>`_: This is a GAM which is used to manage the input/output operations. It serves as a direct link between Datasources, allowing for a transparent interchange of data between them, even if their protocols are not initially compatible.

**Measurement and Control**:

* `SSMGAM <https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1SSMGAM.html>`_: This GAM is the core of the Control System. It implements a Generic State Space Model that defines the control system equations:

    *x[k+1] = Ax[k]+Bu[k]* 
    *y[k] = Cx[k]+Du[k]*

From the input vector u[k], the system of equations generate the so called output vector y[k] (with dimensions [q x 1]) and the state vector x[k] (with dimensions [n x 1]) which control the change between states of the system.

Also, MARTe2 has with some additional GAMs that provide supporting features such us:

* `WaveformGAM <https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1Waveform.html#details>`_: This GAM provides with the generic common functions and variables to generate any kind of waveform signal. It is the base for derived clases such as the `WaveformSinpGAM <https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1WaveformSin.html>`_ or the `WaveformChirpGAM <https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1WaveformChirp.html>`_, for example.

* `HistogramGAM <https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1HistogramComparator.html#details>`_: This GAM determines to which bin of a histogram belongs a given occurrence.  

With all this in mind we can start creating a MARTe2-based Control System application.


