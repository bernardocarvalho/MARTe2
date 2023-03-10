Fixed GAM example
-----------------
-----------------

Let's go through an example of a GAM wich signals fixed by design - as mentioned :doc:`here <../../core/app/gams/gam>`, the GAMs algorithm behaviour may varies with the signal characteristics:

.. literalinclude:: ../../_static/examples/Core/FixedGAMExample1.cpp
   :language: c++
   :caption: Fixed GAM Example 1

and this is its configuration file:

.. literalinclude:: ../../_static/examples/Configurations/GAMs-1.cfg
   :caption: Fixed GAM Configuration File

You can compile the code following the intructions :doc:`here <../../core/examples>`.

As explained in the :doc:`Objects <../../core/objects/objects>` section, the management of objects in MARTe2 is automatically done via the 
ClassRegistryDatabase, and to include an object on it we use CLASS_REGISTER as shown at the very end of the *FixedGAMExample1.cpp* file. 

In this example we are defining a GAM, so after the default constructor and destructor methods, we find the Initialize(), the Setup() and the Execute() ones.

The Initialize() method, as its name suggests, is used to initialize the GAM with a set of data passed as argument. The appropriate checks are done and errors are thrown in case of need.

The Setup() method defines the properties related to input and output signals. First we analyze whether we have the same amount of input and output signals, that in this case should be equal to 1 or an error will be throuwn using the already known from the previous example *REPORT_ERROR* function. Then we repeat the check but in this case to ensure we use a uint32 type of signal, that the number of input and output samples is 1 (that is, the DataSource will acquire just 1 sample per control cycle), that the signal dimension is 0 and that the number of input and output signals elements is 1 (that it, the signal is a scalar).

Finaly, the Execute() method is called at every real-time cycle, having the input signals ready to be processed by the GAM and making sure the output signals are propagated accordingly after it. In this case, the output signal is the input signal weighted by a gain.

