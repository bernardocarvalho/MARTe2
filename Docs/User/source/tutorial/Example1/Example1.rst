Linux Timer 1
-------------
-------------

Let's now create our first MARTe2 real-time applications, starting from very simple ones and progresively complicating the examples, following all the steps from coding to execution - this is probably a good moment to review the :doc:`GAMs <../../core/app/gams/gam>` section.

Read from a LinuxTimer and print the Counter and the Time to a LoggerDataSource at a fixed frequency of 2 Hz
-------------------------------------------------------------------------------------------------------------

First, we will do a MARTe application that reads from a LinuxTimer and prints the Counter and the Time to a LoggerDataSource at a fixed frequency of 2 Hz. 

In this example, we are going to use only GAMs which are already implemented in the MARTe2 components so we will not need to create any new ones, therefore no .cpp or .h files need to be coded. We will only need to create the appropriate configuration file.

This is the configuration file needed for the execution of the real-time application (for a reminder on how configuration files are related to real-time applications refer to :doc:`RealTimeApplication <../../../core/app/gams/rtappdetails.rst>`): ::

    $TestApp = {
        Class = RealTimeApplication
        +Functions = {
            Class = ReferenceContainer
            +GAMLinuxTimer = {
                Class = IOGAM
                InputSignals = {
                    Counter = {
                        DataSource = Timer
                        Type = uint32
                    }
                    Time = {
                        Frequency = 2
                        DataSource = Timer
                        Type = uint32
                    }
                }
                OutputSignals = {
                    Counter = {
                        DataSource = LoggerDataSource
                        Type = uint32
                    }                
                    Time = {
                        DataSource = LoggerDataSource
                        Type = uint32
                    }            
                }
            }
        }
        +Data = {
            Class = ReferenceContainer
            DefaultDataSource = DDB1
            +LoggerDataSource = {
                Class = LoggerDataSource
            }
            +Timings = {
                Class = TimingDataSource
            }
            +Timer = {
                Class = LinuxTimer
                SleepNature = "Default"
                Signals = {
                    Counter = {
                        Type = uint32
                    }
                    Time = {
                        Type = uint32
                    }
                }
            }        
        }
        +States = {
            Class = ReferenceContainer
            +State1 = {
                Class = RealTimeState
                +Threads = {
                    Class = ReferenceContainer
                    +Thread1 = {
                        Class = RealTimeThread
                        CPUs = 0x1
                        Functions = {GAMLinuxTimer}
                    }
                }
            }        
        }
        +Scheduler = {
            Class = GAMScheduler
            TimingDataSource = Timings
        }
    }

As can be seen in the .cfg file, in the Functions section we refer to our GAM *GAMLinuxTimer*. We could use any name we want which is not in use already, as long as we use the same in the *Functions* part of the *+States* section. For the IOGAM, that maps inputs on outputs, we set two signals, "Counter" and "Time", with their datasources and types. Time also contains the frequency property - maximum one per thread - which sets the synchronisation point of the GAM, that is, how often are we going to get the GAM executing. In our case, we want it to be 2Hz so it is set to 2. The output would read the following: ::

    [Information - RealTimeLoader.cpp:111]: Started application in state State1 
    [Information - MARTeApp.cpp:131]: Application starting
    [Information - LoggerBroker.cpp:152]: Counter [0:0]:1
    [Information - LoggerBroker.cpp:152]: Time [0:0]:0
    [Information - LoggerBroker.cpp:152]: Counter [0:0]:2
    [Information - LoggerBroker.cpp:152]: Time [0:0]:1000000
    [Information - LoggerBroker.cpp:152]: Counter [0:0]:3
    [Information - LoggerBroker.cpp:152]: Time [0:0]:1500000
    [Information - LoggerBroker.cpp:152]: Counter [0:0]:4
    [Information - LoggerBroker.cpp:152]: Time [0:0]:2000000
    ...


