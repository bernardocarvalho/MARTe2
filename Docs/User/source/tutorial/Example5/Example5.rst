Example 5: Multithread
----------------------

As expected from a powerful Real-time framework, MARTe2 can also perform multithreaded applications. In this example we will create an application that uses two concurrent threads that will communicate one to each other. The first will be driven by a Linuxtimer GAM running at a frequency of 100 Hz, while the second will print the value of the counter of the first at a sub-frequency of 1 Hz, that is, it will print the counter 1 out of 100 samples.

For doing this, we will use a new type of DataSource, the `RealTimeThreadSynchronisation <https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1RealTimeThreadSynchronisation.html>`_ DataSource. As can be read in the documentation, a GAM will write into this DataSource on a given thread. One or more GAMs (each on its thread) will read and synchronise against this DataSource. If the number of samples read from the GAM is greater than one, the DataSource will block until the specified number of samples is available. 

This feature allows to run and synchronise threads at a lower frequency, so we can make use of that DataSource to synchronize threads without having to worry about access concurrency.

The configuration file would be the following: 
    
.. literalinclude:: Example5.cfg 
   :linenos:
   :emphasize-lines: 16, 27, 34, 59-62

In this example there are only two GAMs. The first one is already our wellknown *GAMLinuxTimer*. The only difference is that the output signal will be assigned to *RealTimeSynch* that - as we can see in the **+Data** section - is defined as a *RealTimeThreadSynchronisation*.

The other GAM is *GAMCounterSampler*, which is basically another IOGAM that in this case reads from *RealTimeSynch* DataSource. Here is it worth to notice the line **Samples = 100** in both the input and output signals. This is how we synchronise the two GAMs through the *RealTimeSynch*, which defines that the GAM will execute 1 every *Samples* times, that is, once every 100 times the *GAMLinuxTimer* executes, which is what we where looking for.

The output will look like the following: ::

   [Information - RealTimeLoader.cpp:111]: Started application in state State1 
   [Information - MARTeApp.cpp:131]: Application starting
   [Information - LoggerBroker.cpp:152]: Time [0:0]:1
   [Information - LoggerBroker.cpp:152]: Time [0:0]:101
   [Information - LoggerBroker.cpp:152]: Time [0:0]:201
   [Information - LoggerBroker.cpp:152]: Time [0:0]:301
   [Information - LoggerBroker.cpp:152]: Time [0:0]:401
   ...

   