Example 7: Keyboard response
----------------------------

The idea now is to implement a State Machine that reacts to the pressing of a key. We will programm a GAM that reads keyboard inputs and if the proper key is presed, sends a message asking the State Machine to change state and do something.

GAMs shall not interface with operatin system (appart from memory allocation during configuration), so the keyboard interface should be done from an external application.

Take a look to the configuration file: 
    
.. literalinclude:: Example7.cfg 
   :linenos:

