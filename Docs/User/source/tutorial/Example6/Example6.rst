Example 6: Statistics
---------------------

The MARTe2-components package also include some GAMs which can help on performing statistics for our projects. A good example of that is the *HistogramGAM*. In this example, we are going to drive our application with our LinuxTimer GAM and compute a histogram about the thread cycle time, directing the output to a LoggerDataSource.

The configuration file would be the following: 
    
.. literalinclude:: Example6.cfg 
   :linenos:
   :emphasize-lines: 36-54


The output will draw a histogram in line with the defined parameters in the .cfg file. What the application does is to take the counter from the *GAMLinuxTimer* and write in the DDB1 DataSource, that will serve as input for the *HistogramGAM1*. The main parameters we need to consider to define the histogram are *Minlim = 10* and *Maxlim = 100* from the input signal and the *NumberOfElements = 11* of the output signal. The *GAMPrintToLog* GAM is only used to print the Count number in the Logger.

*NumberOfElements* gives the number of bins that we will use for drawing the histogram, while *Minlim* and *Maxlim* define the range between the first and the last values taken into account to define the bins. In this example, the minimum value is 10, which means that counts happening less (strict) than 10 times will be shown in the first bin. The maximum number is 100, meaning that counts happening 100 times or more will appear in the last bin. In the middle, we will have bins splited equaly, with a width given by the formula: ::
   
   (Maxlim - Minlim) / (NumberOfElements - 2)

So, in our case the width would be (100 - 10) / 9 = 10: ::


   [<10) [10-20) [20-30) [30-40) [40-50) [50-60) [60-70) [70-80) [80-90) [90-100) [>100]

If we run the example for 3 samples we get ::

   [Information - RealTimeLoader.cpp:111]: Started application in state State1 
   [Information - MARTeApp.cpp:131]: Application starting
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:1
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 1 0 0 0 0 0 0 0 0 0 0 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:2
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 2 0 0 0 0 0 0 0 0 0 0 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:3
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 3 0 0 0 0 0 0 0 0 0 0 } 

At sample number 10, the new bin will start counting: ::

   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:9
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 0 0 0 0 0 0 0 0 0 0 }
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:10
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 1 0 0 0 0 0 0 0 0 0 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:11
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 2 0 0 0 0 0 0 0 0 0 } 

Over 100 samples we will see something like: ::
   
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:99
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 0 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:100
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 1 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:101
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 2 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:102
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 3 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:103
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 4 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:104
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 5 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:105
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 6 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:106
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 7 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:107
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 8 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:108
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 9 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:109
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 10 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:110
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 11 } 
   [Information - LoggerBroker.cpp:152]: Counter_out [0:0]:111
   [Information - LoggerBroker.cpp:152]: Histogram1 [0:10]:{ 9 10 10 10 10 10 10 10 10 10 12 } 
   ...
   