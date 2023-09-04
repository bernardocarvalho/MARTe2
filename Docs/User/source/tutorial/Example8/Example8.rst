Example 7: Keyboard response
----------------------------

The idea now is to implement a State Machine that reacts to the pressing of a key. We will programm a GAM that reads keyboard inputs and if the proper key is presed, sends a message asking the State Machine to change state and do something.

GAMs shall not interface with operatin system (appart from memory allocation during configuration), so the keyboard interface should be done from an external application.

Take a look to the configuration file: 
    
.. literalinclude:: Example7.cfg 
   :linenos:

The way to set input or output variables in a MARTe object can be seen in the statement of the setup() method: ::
   
   key = reinterpret_cast<int32 *>(GetOutputSignalMemory(0u));

The method GetOutputSignalMemory() reserves a memory slot to be retreived later by a DataSource. In this particular case, we are reserving memory allocations by index, i.e., this slot will be reserved for the first variable defined in the configuration file, independently of its name: ::

   OutputSignals = {
                key = {
                    DataSource = DDB1
                    Type = int32
                }
            }

.. note:: 
   
   If we want to force the use of a specific name for each variable we can do so by implementing the appropriate logic in the initialization() method of the GAM.



