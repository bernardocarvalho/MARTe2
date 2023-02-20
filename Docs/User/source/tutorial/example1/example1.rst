Example 1
---------
---------

Let's start from the very beginning by showing the main structure of a MARTe2 application with a hello world type of application. The idea is just to show the message "Say something..." in the logger.

.. literalinclude:: ./makefiles/ExFile1.cpp
   :language: c++
   :caption: Example 1
   :linenos:

You can compile the code following the intructions :doc:`here <../core/examples>` 

The initial part of the code is the usual include section, followed by the static definitions. In this particular example, there is one static method defined, *ErrorProcessFunctio*", that will manage errors in the application. Its first argument contains the details of the occurred type of error, and the second is its description. Further details can be found in ErrorManagement.h.

The function uses the *ErrorCodeToStream* function (ErrorManagement.h), that converts ErrorType to stream and defines a way to prompt the information in the screen.

Next we find the main function. First it sets *ErrorProcessFunction* as the function that will manage errors for the application by passing a reference to it to *SetErrorProcessFunction()*. Then, using the *REPORT_ERROR_STATIC* inline method (ErrorManagement.h) to force a warning occurrence, with the description "Say something...". 

Then an instance object type ExFile2 is created, which simply sends the information message "Something!" when its method exFile2.SaySomething() is called. This is ExFile2.cpp code:

.. literalinclude:: ./makefiles/ExFile2.cpp
   :language: c++
   :caption: Example 2
   :linenos:

The ouput of the execution is the following: ::
   [Warning - ExFile1.cpp:65]: Say something...
   [Information - ExFile2.cpp:52]: Something!
