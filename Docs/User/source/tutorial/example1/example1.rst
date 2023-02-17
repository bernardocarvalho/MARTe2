Example 1
---------
---------

Let's start from the very beginning by showing the main structure of a MARTe2 application with a hello world type of application. The idea is just to show the message "Say something..." in the logger.

.. literalinclude:: ./makefiles/ExFile1.cpp
   :language: c++
   :caption: Example 1
   :linenos:

You can compile the code following the intructions :doc:`here <../core/examples>` 

The initial part of the code is the usual include section, followed by the static definitions. In this particular example, there is one static method defined, "ErrorProcessFunction", that will manage errors in the application and has two arguments. The first one is &errorInfo, a reference to an ErrorInformation object as defined in ErrorInformation.h. This object contains details about the possible errors happening in the application. The second one is the definition of the type of colours we will use for prompting the error message. 

The ErrorProcessFunction is called by SetErrorProcessFunction() (included in ErrorManagement.h, that is called by AdvancedErrorManagement.h), which sets the routine for error managing 
