const char * configFile = ""
"$TestApp = {\n"
"    Class = \"RealTimeApplication\"\n"
"    +Functions = {\n"
"        Class = \"ReferenceContainer\"\n"
"        +GAMTimer = {\n"
"            Class = \"IOGAM\"\n"
"            InputSignals = {\n"
"                Counter = {\n"
"                    DataSource = \"Timer\"\n"
"                    Type = \"uint32\"\n"
"                }\n"
"                Time = {\n"
"                    Frequency = \"10\"\n"
"                    DataSource = \"Timer\"\n"
"                    Type = \"uint32\"\n"
"                }\n"
"                Idle_Thread1_CycleTime = {\n"
"                    DataSource = \"Timings\"\n"
"                    Alias = \"Idle.Thread1_CycleTime\"\n"
"                    Type = \"uint32\"\n"
"                }\n"
"            }\n"
"            OutputSignals = {\n"
"                Counter = {\n"
"                    DataSource = \"DDB1\"\n"
"                    Type = \"uint32\"\n"
"                }\n"
"                Time = {\n"
"                    DataSource = \"DDB1\"\n"
"                    Type = \"uint32\"\n"
"                }\n"
"                Idle_Thread1_CycleTime = {\n"
"                    DataSource = \"DDB1\"\n"
"                    Type = \"uint32\"\n"
"                }\n"
"            }\n"
"        }\n"
"        +GAMSDNSender = {\n"
"            Class = \"IOGAM\"\n"
"            InputSignals = {\n"
"                Time = {\n"
"                    DataSource = \"DDB1\"\n"
"                    Type = \"uint32\"\n"
"                }\n"
"                Idle_Thread1_CycleTime = {\n"
"                    DataSource = \"DDB1\"\n"
"                    Type = \"uint32\"\n"
"                }\n"
"            }\n"
"            OutputSignals = {\n"
"                Time = {\n"
"                    DataSource = \"SDNPub\"\n"
"                    Type = \"uint32\"\n"
"                }\n"
"                Idle_Thread1_CycleTime = {\n"
"                    DataSource = \"SDNPub\"\n"
"                    Type = \"uint32\"\n"
"                }\n"
"            }\n"
"        }\n"
"    }\n"
"    +Data = {\n"
"        Class = \"ReferenceContainer\"\n"
"        DefaultDataSource = \"DDB1\"\n"
"        +DDB1 = {\n"
"            Class = \"GAMDataSource\"\n"
"        }\n"
"        +LoggerDataSource = {\n"
"            Class = \"LoggerDataSource\"\n"
"        }\n"
"        +Timings = {\n"
"            Class = \"TimingDataSource\"\n"
"        }\n"
"        +Timer = {\n"
"            Class = \"LinuxTimer\"\n"
"            SleepNature = \"Default\"\n"
"            ExecutionMode = \"RealTimeThread\"\n"
"            Signals = {\n"
"                Counter = {\n"
"                    Type = \"uint32\"\n"
"                }\n"
"                Time = {\n"
"                    Type = \"uint32\"\n"
"                }\n"
"            }\n"
"        }\n"
"        +SDNPub = {\n"
"            Class = \"UDP::UDPSender\"\n"
"            Address = \"192.168.2.50\"\n"
"            Port = \"45678\"\n"
"            ExecutionMode = \"RealTimeThread\"\n"
"            Signals = {\n"
"                Locked = \"1\"\n"
"                Time = {\n"
"                    Type = \"uint32\"\n"
"                }\n"
"                Idle_Thread1_CycleTime = {\n"
"                    Type = \"uint32\"\n"
"                }\n"
"            }\n"
"        }\n"
"    }\n"
"    +States = {\n"
"        Class = \"ReferenceContainer\"\n"
"        +Idle = {\n"
"            Class = \"RealTimeState\"\n"
"            +Threads = {\n"
"                Class = \"ReferenceContainer\"\n"
"                +Thread1 = {\n"
"                    Class = \"RealTimeThread\"\n"
"                    CPUs = \"0x2\"\n"
"                    Functions = { \"GAMTimer\" \"GAMSDNSender\" } \n"
"                }\n"
"            }\n"
"        }\n"
"    }\n"
"    +Scheduler = {\n"
"        Class = \"GAMScheduler\"\n"
"        TimingDataSource = \"Timings\"\n"
"    }\n"
"}\n"
;
