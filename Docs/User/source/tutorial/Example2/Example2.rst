Example 2: WaveformSin
----------------------
 
Ok, so we have already created our first MARTe2 application. Next we will read from a LinuxTimer, execute a WaveformSin and print the value of the sine to a LoggerDataSource at a fixed frequency of 10 Hz. As in the previous example, no user components are necessary so we just need to update the configuration file. Lets see how the .cfg file looks like:
    
.. literalinclude:: Example2.cfg 
   :linenos:

First we have our previous *GAMLinuxTimer* that now has a change: the input signal is taken by the Timer DataSource - which is of type LinuxTimer, that gives a reference of the running time of the application - but the output signal is written to DDB1, which is going to be - as can be seen in the **+Data** section - a *GAMDataSource*, which basically acts as an interface to allow GAMs to write signals in memory so other GAMs can recall that information without the necessity to go to the GAM itself.

Then we have the *+GAMWaveformSin*. The first line is **Class=WaveformGAM::WaveformSin** were we define the class to be used. Afterwards, we have several parameters that define the type of sine waveform we want to describe, followed by the definition of the input and output signals as in any GAM. By checking in the documentation for the `WaveformSin <https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1WaveformSin.html#details>`_, the GAM has only one single value input signal indicating the current time - that we already have in the DDB1 as given by our GAMLinuxTimer - and one array output signal, that we will also store in DDB1.

Then we also have a third GAM, the GAMScreenWriter, meant to be getting information from the DDB1 DataSource and putting it into the Screen by means of the LoggerDataSource.

The output would be something like: ::
    
    [Information - MARTeApp.cpp:131]: Application starting
    [Information - LoggerBroker.cpp:152]: Time_microseconds [0:0]:0
    [Information - LoggerBroker.cpp:152]: SineWave_values [0:0]:0
    [Information - LoggerBroker.cpp:152]: Time_microseconds [0:0]:200000
    [Information - LoggerBroker.cpp:152]: SineWave_values [0:0]:5.877852
    [Information - LoggerBroker.cpp:152]: Time_microseconds [0:0]:300000
    [Information - LoggerBroker.cpp:152]: SineWave_values [0:0]:8.090170
    [Information - LoggerBroker.cpp:152]: Time_microseconds [0:0]:400000
    ...

.. note::
    We must be careful when setting the parameters for the simulation. For instance, if we set the frequency of the sine wave to 10 and the frequency of generation of the Timer signal to 10, we will get the following: ::

        ...
        [Information - LoggerBroker.cpp:152]: Time_microseconds [0:0]:300000
        [Information - LoggerBroker.cpp:152]: SineWave_values [0:0]:0
        [Information - LoggerBroker.cpp:152]: Time_microseconds [0:0]:400000
        [Information - LoggerBroker.cpp:152]: SineWave_values [0:0]:0
        [Information - LoggerBroker.cpp:152]: Time_microseconds [0:0]:500000
        [Information - LoggerBroker.cpp:152]: SineWave_values [0:0]:0
        ...

    It may seem that something may not be working properly, but it is. The reason of having all samples as 0 is that we are always sampling the same value of the sine wave - the first one - because the measurement is synchronized with it.

Some remarcable things to mention here are:

1- The signal Time is expressed in microseconds, and the GAMWaveformSin acknowledges that the imput will be in microseconds. If the time was given in seconds or any other unit, the result would not have been as expected.

2- You may have noticed that in the *GAMWaveformSin* class definition, we provided an extended path to assign the class name, whereas in other GAMs we just put the class name without any additional route. When the library name does not match the name of the class, we need to add the extended path including the **library-name::class-name**. That is the case with the *WaveformGAM.so* library, which has a different name than *WaveformSin*, the name of the class. If the name of the class and the library are the same, simply adding the class name will sufice. 

3- As can be seen, the input signals of *GAMScreenWriter* do not have any DataSource defined, but input is taken directly from DDB1 as we expect. This is because in the **+Data** section we can find **"DefaultDataSource = DDB1"**, which sets the default database to be used in case of not explicit assignment. This could also have been used in the previous GAMs but it was used only in this one to express that both ways are correct, although affecting readability in different manner.

So with this example we have seen some more detail about the way we should write GAMs, but also about the idea behind this configuration paradigm: we define the needed GAMs for achieving a specific purpose without having to go into the detail of each one. We can set one GAM to get the time, another one receiving that as an input to calculate a waveform and finally another one managing the gathering of data from memory and its presentation in the terminal, all of that withough having to go into re-programing the GAMs - just stating their signals and where do we want them to be connected.