Example 3: FileDataSource
-------------------------

In the previous example, we used the waveformsin GAM to show in the terminal the values of the wave for each time given. In this example we will store that information in a file. 

The only thing we need to do that is to define a FileWriter type DataSource, that will flush the required signals into a file.

The configuration file would be the following: 

.. literalinclude:: Example3.cfg 
   :linenos:

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

