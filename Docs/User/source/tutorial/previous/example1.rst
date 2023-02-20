Introductory step
-----------------
-----------------

With the aim of introducing  first analyze this simple hello-world-type application. The idea is just to show the message "Say something..." in the logger.

.. literalinclude:: ../../_static/examples/Makefiles/ExFile1.cpp
   :language: c++
   :caption: Example 1
   :linenos:

You can compile the code following the intructions :doc:`here <../../core/examples>`.

The initial part of the code is the usual include section, followed by the static definitions. In this particular example, there is one static method defined, *ErrorProcessFunction*", that will receive errors and process them. Its first argument contains the details of the occurred error type, and the second its description. Further details can be found in ErrorManagement.h.

The function uses the *ErrorCodeToStream* function (ErrorManagement.h), that converts ErrorType to stream and defines a way to prompt the information in the screen.

Next we find the main function. First it sets *ErrorProcessFunction* as the function that will manage errors for the application by passing a reference to it to *SetErrorProcessFunction()*. Then, we see an example of how to send an error message of type warning using the *REPORT_ERROR_STATIC* inline method (ErrorManagement.h), with the description "Say something...". 

After that, an instance object of type ExFile2 is created, which simply sends the information message "Something!" when its method 
exFile2.SaySomething() is called. This is ExFile2.cpp code:

.. literalinclude:: ../../_static/examples/Makefiles/ExFile2.cpp
   :language: c++
   :caption: Example 2
   :linenos:

The ouput of the execution is the following: ::

   [Warning - ExFile1.cpp:65]: Say something...
   [Information - ExFile2.cpp:52]: Something!

Before moving to more complex applications, next we will analyze in depth on of the examples previously seen in the :doc:`GAMs <../../core/app/gams/gam>` setion. In case you have not yet checked it, this is probably a good moment to do so.