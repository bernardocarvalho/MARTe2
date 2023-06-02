Example 2: WaveformSin
----------------------
 
In this case we will read from a LinuxTimer, execute a WaveformSin and print the value of the sine to a LoggerDataSource at a fixed frequency of 10 Hz
As in the previous one, no .h and .cpp files are necessary in this example.

Lets see the configuration file: ::
    
    $TestApp = {
        Class = RealTimeApplication
        +Functions = {
            Class = ReferenceContainer
            +GAMLinuxTimer = {
                Class = IOGAM
                InputSignals = {
                    Time = {
                        Frequency = 10
                        DataSource = Timer
                        Type = uint32
                    }
                }
            OutputSignals = {             
                    Time = {
                        DataSource = "DDB1"
                        Type = uint32
                }            
                }
            }
            +GAMWaveformSin = {
                Class=WaveformGAM::WaveformSin // Since the name of the library WaveformGAM.so do not correspond to the name of the class WaveformSin, we need to specify it here
                Amplitude = 10.0
                Frequency = 0.5
                Phase = 0.0
                Offset = 0.0
                InputSignals = {
                    Time = {
                        DataSource = "DDB1"
                        Type = uint32 
                    }
                }
                OutputSignals = {
                    SineWave = {
                        DataSource = "DDB1"
                        Type = float32
                    }
                }
            }
            +GAMScreenWriter = {
                Class = IOGAM            
                InputSignals = {
                    Time = {
                        Type = uint32
                    }
                    SineWave = {
                        Type = uint32
                    }
                } 
                OutputSignals = {
                    Time_microseconds = {
                        DataSource = LoggerDataSource
                        Type = uint32
                    }  
                    SineWave_values = {
                        DataSource = LoggerDataSource
                        Type = float32
                    }             
                }
            }
        }
        +Data = {
            Class = ReferenceContainer
            DefaultDataSource = DDB1
            +DDB1 = {
                Class = GAMDataSource
            }
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
                        Functions = {GAMLinuxTimer GAMWaveformSin GAMScreenWriter}
                    }
                }
            }        
        }
        +Scheduler = {
            Class = GAMScheduler
            TimingDataSource = Timings
        }
    }


Lets deepen a bit on the detail of the GAMs involved on it.

First we have our previous GAMLinuxTimer that now has a change: the input signal is taken by the Timer DataSource - which is of type LinuxTimer, that gives a reference of the running time of the application - but the output signal is written to DDB1, which is going to be, as can be seen in the +Data section, a GAMDataSource, which basically acts as an interface to allow GAMs to write signals in memory so other GAMs can recall that information without the necessity to go to the GAM itself.

Then we have +GAMWaveformSin. The first line is Class=WaveformGAM::WaveformSin were we define the class to be used. Afterwards, we have several parameters that define the type of sine waveform we want to describe, followed by the definition of the input and output signals as in any GAM. By checking in the documentation for the `WaveformSin <https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1WaveformSin.html#details>`_, the GAM has only one single value input signal indicating the current time - that we already have in the DDB1 as given by our GAMLinuxTimer - and one array output signal, that we will also store in DDB1.

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

Some remarcable things to mention here are:
1- The signal Time is expressed in microseconds, and the GAMWaveformSin acknowledges that the imput will be in microseconds. If the time was given in seconds or any other unit, the result would not have been as expected
2- You may have noticed that in the *GAMWaveformSin* class definition, we provided an extended path to assign the class name, whereas in other GAMs we just put the class name without any additional route. The recommended way to assign class names is this one - we should always add the extended path including the *library-name::class-name* because the WaveformGAM.so library name may not match the name of the class. But when this happens, simply adding the class name will sufice. Nevertheless, even if in some of the examples this shortcut will be used, using the extended path is usually better.
3- As can be seen, the input signals of GAMScreenWriter do not have any DataSource defined, but input is taken directly from DDB1 as we expect. This is because in the +Data section we can find "DefaultDataSource = DDB1", which sets the default database to be used in case of not explicit assignment. This could also have been used in the previous GAMs but it was used only in this one to express that both ways are correct, although affecting readability in different manner.
4- We must be careful when setting the parameters for the simulation. For instance, if we set the frequency of the sine wave to 10, and the frequency of generation of the Timer signal to 10, we will get the following: ::

    ...
    [Information - LoggerBroker.cpp:152]: Time_microseconds [0:0]:300000
    [Information - LoggerBroker.cpp:152]: SineWave_values [0:0]:0
    [Information - LoggerBroker.cpp:152]: Time_microseconds [0:0]:400000
    [Information - LoggerBroker.cpp:152]: SineWave_values [0:0]:0
    [Information - LoggerBroker.cpp:152]: Time_microseconds [0:0]:500000
    [Information - LoggerBroker.cpp:152]: SineWave_values [0:0]:0
    ...

It may seem that something is not working properly, but it is. The reason of having all samples as 0 is that we are always sampling the same value of the sine wave - the first one - because the measurement is synchronized with it.

So with this example we have seen some more detail about the way we should write GAMs, but also about the idea behind this configuration paradigm: we define GAMs serving a specific purpose without having to go into the detail of each one. We can set one GAM to get the time, another one receiving that as an input to calculate a waveform and finally another one managing the gathering of data from memory and its presentation in the terminal. All of that withough having to go into deep programing the GAMs - just stating their signals and where do we want them to be connected.