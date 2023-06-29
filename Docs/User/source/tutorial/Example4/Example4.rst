Example 4: Read and Write files
-------------------------------

Now we are going to create an example in which we read from a LinuxTimer to get the time syncrhonization, read some values from a file (using the FileDataSource), use the *MathExpressionGAM* to modify these values (e.g. by multiplying them by 2) and then store the values to another FileDataSource.

The configuration file would be the following: 
    
.. literalinclude:: Example4_infinite.cfg 
   :linenos:

In this case, in addition to the already well known *GAMLinuxTimer**, we have the *GAMFileReader* for reading data from the DataSource *FileReader_0* and write these data into DDB1, the *MathsGAM* that operates - multiplies by 2 - with the values previously read by the *GAMFileReader*, and the *GAMFileWriter* that reads from DDB1 the modified data and writes it in *FileWriter_0*.

As can be seen in the **+Data** section, the *FileReader_0* points to a file called *test_reader.csv* where a sets of two input data are stored, while *FileWriter_0* is directed to a file called *test_writer.csv* where we will store the modified values.

The format of the *test_reader.csv* file is the following: ::

    #Time_read (uint32)[1];Values_read (float32)[1]
    0;0
    200000;0.1
    300000;0.2
    400000;0.3
    500000;0.4
    600000;0.5

The header is a very important part of the file as it has to be alligned with the defined signals in the *GAMFileReader* for the later one to be able to manage them properly. If the names do not match, we will receive an error stating that the DataSource is locked and the signals could not be added. The same happens with the types. 

.. note:: 
    
    There should not be any blank lines at the end of the file.

The *GAMFileWriter* takes the input signals from DDB1 and writes them in the output file *test_writer.csv*. We need to be careful to align the output signals from the *GAMFileWriter* with the signals defined in the *FileWriter_0* DataSource, as if they are not included in the DataSource there will be an initialization error.

In this example we also used the *MathExpressionGAM*, which evaluates mathematical expressions at runtime. The Expression leaf is where we define the operation to be done (you can check `this <../../../../../../MARTe2-components/Source/Components/GAMs/MathExpressionGAM>` link for more details). In this example we use ::

    Expression = "Output = (float32)2 * Values_read;"
    
Notice that the expression should end with a comma, a semicolon or a \n. Also, it is important to explicitly set types as otherwise we may have initialization errors. In this case, the literal *2* is assumed to be a float64, so we need to do an explicit conversion to float32, which is the type of *Values_read*.

The output in this case would be like this: ::

    #Time_write (uint32)[1];Output (float32)[1]
    0;0
    200000;2.000000E-1
    300000;4.000000E-1
    400000;6.000000E-1
    500000;8.000000E-1
    600000;1.000000
    0;0
    200000;2.000000E-1
    300000;4.000000E-1
    400000;6.000000E-1
    500000;8.000000E-1
    600000;1.000000
    ...


As we know, MARTe2 applications are supposed to be running non-stop. In this case, whe will have an infinite process but probably it makes more sense to stop when reaching the end of the input file.

Up to this point, we have just changed the configuration file to create different applications using the components already included in MARTe2. But at this point we need some code that helps us stopping the execution when reaching a specific situation, so we are going to implement a new application named *ApplicationKiller*. Simply, it is a small program that when receiving the function *Kill*, stops the execution of the application sending a kill command (for more details, you can take a look to the files :download:`ApplicationKiller.h <../../../../../../Projects/MARTe2-components/Source/Components/Interfaces/ApplicationKiller/ApplicationKiller.h>` and :download:`ApplicationKiller.cpp <../../../../../../Projects/MARTe2-components/Source/Components/Interfaces/ApplicationKiller/ApplicationKiller.cpp>`).

.. note:: 
    
    When creating a new application or GAM, we first need to compile it to create the corresponding library to link to our MARTe2 application. So first we should add the folder containing the files of the GAM (namely the .h, .cpp and the makefile.gcc and makefile.inc) to our MARTe2-components path, so it is included as part of the components package. Also, we need to add to the Makefile.inc in the MARTe2-components the following line: ::

        SPB=... \
            ApplicationKiller.x

        Once this is done, we can run the command ::
            
            make -f Makefile.x86-linux

        to recompile MARTe2-components. 

        Finally, it will be needed to link the path to the library to the *$LD_LIBRARY_PATH* variable, adding to the already existing lines ::

            :$MARTe2_Components_DIR/Build/x86-linux/Components/Interfaces/ApplicationKiller

We can now update our config file as follows: 

.. literalinclude:: Example4.cfg 
   :linenos:
   :emphasize-lines: 1-3, 118, 122-130

Note the highlighted lines. At the beginning, we reffer to another application that will be our process killer, called *ApplicationKiller*. Then, in the *FileReader_0* section, we set the EOF to "Error", meaning that an error will be triggered when reaching the end of file. At that point, the **+Messages** section defines the *+FileRuntimeError* message, that will be sent to the ApplicationKiller app with the Function "Kill". When the ApplicationKiller receives the message, sends a kill command to the application *TestApp* and stops it. 

Now the output is: :: 

    #Time_write (uint32)[1];Output (float32)[1]
    0;0
    200000;2.000000E-1
    300000;4.000000E-1
    400000;6.000000E-1
    500000;8.000000E-1
    600000;1.000000