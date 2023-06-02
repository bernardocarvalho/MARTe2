Example 3: FileDataSource
-------------------------

In the previous example, we used the waveformsin GAM to show in the terminal the values of the wave for each time given. In this example we will store that information in a file. 

The only thing we need to do that is to define a FileWriter type DataSource, that will flush the required signals into a file.

The configuration file would be the following: ::
    
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
                Frequency = 10
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
            +GAMFileWriter = {
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
                        DataSource = FileWriter_0
                        Type = uint32
                    }  
                    SineWave_values = {
                        DataSource = FileWriter_0
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
            +FileWriter_0 = {
                Class = FileDataSource::FileWriter  //The FileDataSource.so library does not coincide with the class name FileWriter
                NumberOfBuffers = 10 //Number of buffers. Each buffer is capable of holding a copy of all the DataSourceI signals.
                CPUMask = 0xFE //Affinity assigned to the threads responsible for asynchronously flush data into the file.
                StackSize = 10000000 //Stack size of the thread above.
                Filename = "test_file.csv" //Optional. If not set the filename shall be set using the OpenFile RPC.
                Overwrite = "yes" //If "yes" the file will be overwritten, otherwise new data will be added to the end of the existent file.
                FileFormat = "csv" //Possible values are: binary and csv.
                CSVSeparator = ";" //If Format=csv. Sets the file separator type.
                StoreOnTrigger = 0 //If 0 all the data in the circular buffer is continuously stored. If 1 data is stored when the Trigger signal is 1 

                Signals = {
                    Time_microseconds = { //As many as required.
                        Type = "uint32"
                        Format = "e" //Optional. Any format specifier supported by FormatDescriptor (without ''). Without effect if FileFormat is "binary".
                    }
                    SineWave_values = { //As many as required.
                        Type = "float32"
                        Format = "e" //Optional. Any format specifier supported by FormatDescriptor (without ''). Without effect if FileFormat is "binary".
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
                        Functions = {GAMLinuxTimer GAMWaveformSin GAMFileWriter}
                    }
                }
            }        
        }
        +Scheduler = {
            Class = GAMScheduler
            TimingDataSource = Timings
        }
    }

In the +Data section, we see we added *+FileWriter_0*, that is from class FileWriter - we also need the extended path here for the same reason as in the previous example.

in the documentation page for `WaFileWriter Class <https://vcis-jenkins.f4e.europa.eu/job/MARTe2-Components-docs-master/doxygen/classMARTe_1_1FileWriter.html#a0486728fd959e4767c8317be7610acbd>`_ we have all the possible parameters of the DataSource. In the example we used some compulsory, such as the *NumberOfBuffers* or the *CPUMask*, together with the output file name mentioned in *Filename*, specified as a .csv file sepparated by ";".

The output in this case would be a file named *test_file.csv* including the pair "time;sine_value" in each row - the first one being the heading that includes the name and the type of data stored in that column, like this: ::

    #Time_microseconds (uint32)[1];SineWave_values (float32)[1]
    0;0
    300000;-7.347880E-15
    400000;-9.797175E-15
    500000;-1.224647E-14
    600000;-1.469576E-14
    700000;-1.714506E-14
    800000;-1.959435E-14
    900000;-2.204364E-14

