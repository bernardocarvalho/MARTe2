Example 7: Simple Control System
--------------------------------

As we have seen in the previous section, Control Systems usually make use of different protocols and databases usually managed by a State Machine. In this example we are going to create an application that simulates this: A State Machine interacting with the environment to control a system.

Our application will have a State Machine as its core. Basically, a State Machine is a mathematical behaviour model of computation defining a finite number of states. The model can state in only one state at a time, and can change from state to state as a response to some predefined inputs. In each state, it can istruct different functions, thus allowing for the control of a system under different scenarios.

Take a look to the configuration file: 
    
.. literalinclude:: Example7.cfg 
   :linenos:

The configuration file has 4 main modules:

* **TCPMessageProxy** is a module included in MARTe2 that allows TCP communication from outside the application. We will use it to send messages to the StateMachine via port 24680

* **MessagePrompter** is an external application very similar to the ApplicationKiller we saw in :doc:`Example4 <../Example4/Example4>`, but in this case we have modified it adding another method called *SendMessage* that we will call when changin states. You can find the code in :download:`MessagePrompter.h <../../../../../../Projects/MARTe2-components/Source/Components/Interfaces/MessagePrompter/MessagePrompter.h>` and :download:`MessagePrompter.cpp <../../../../../../Projects/MARTe2-components/Source/Components/Interfaces/MessagePrompter/MessagePrompter.cpp>`.

.. note:: 
   
   For compiling it, follow the same process we used in Example4 for the ApplicationKiller.

* **StateMachine** is the module defining the main State Machine that will control the application and

* **TestApp** is the module including the functions to be run at each State Machine state.

In MARTe2, States Machines are used through the *StateMachine* class, in which different State Machine Events can be defined via the *StateMachineEvent* class, representing different states of the State Machine. For more details check the :doc:`State Machine <../../core/statemachine/statemachine>`.

In our case, we will define a State Machine with 3 states: 

**INITIAL**: Since this is the first state defined in the State Machine, when the application starts it will automatically enter here. Usually, systems need a set-up state in which they configure the system as needed to start operation. In our case, it will move the TestApp to AppState1 and the State Machine to *STATE1*. This is done via a *StateMachineEvent* called *GOTOSTATE1*. 

When a message sent to the State Machine triggers an event, the State Machine will move to the state defined in the "NextState" directive, and will execute all the messages included in the event. In our case, there are two states associated to the *GOTOSTATE1* event, that will prepare the state we want to move the TestApp with *PrepareChangeToRUN1Msg* message, defining the next state as a parameter, in this case *AppState1* - and then start it via the *StartNextStateExecutionMsg* message. 

.. note:: 
   
   It is worth noticing here that the states defined in the State Machine such as *INITIAL* or *STATE1* are not necessarily related to the states defined in the TestApp **+States** section. The first define the States of the State Machine whereas the second define the GAMs that will be associated to the threads of the TestApp - that happens to be called States as well.

**STATE1**: In this state we have an event called *GOTOSTATE2* that basically performs the same actions than the previous *GOTOSTATE1* but moving to STATE2 instead and putting the TestApp into AppState2. Also, we need to notice that before changing the state in the TestApp, we need to stop the execution of the previous one, that we do by means of the *StopCurrentStateExecutionMsg* message.

**STATE2**: Here we have the same event as *GOTOSTATE2* but moving again to *STATE1* and *AppState1*.

**ERROR**: In the previous states we defined also another event called *GOTOERROR*, that is used to set the State Machine in an error state. In that case, we will call the MessagePrompter with the Kill function, that will terminate the application.

Lets see the behaviour launching the application with the following command: ::

   $MARTe2_DIR/Docs/User/source/_static/examples/MARTeApp.sh -l RealTimeLoader -m StateMachine:START -f Example7.cfg

Notice that in this case we launch the application with a message - using the -m option - instead of telling the Application in which state we start running it. This is in line with the MARTe2 principle that objects should communicate through messages. Actually, the previous way to launch the application is kind of a shortcut, but it is recommended to use the messages approach.

After several messages, the application will show somthing like this: ::

   [Information - MARTeApp.cpp:131]: Application starting
   [Information - StateMachine.cpp:201]: Changing from state (INITIAL) to state (STATE1)
   [Information - StateMachine.cpp:340]: In state (INITIAL) triggered message (PrepareChangeToRUN1Msg)
   [Information - LinuxTimer.cpp:482]: Frequency found = 2.000000
   ...
   [Information - StateMachine.cpp:340]: In state (INITIAL) triggered message (StartNextStateExecutionMsg)
   [Information - LoggerBroker.cpp:152]: Counter1 [0:0]:1
   [Information - LoggerBroker.cpp:152]: Time1 [0:0]:500000
   [Information - LoggerBroker.cpp:152]: Counter1 [0:0]:2
   [Information - LoggerBroker.cpp:152]: Time1 [0:0]:1000000
   [Information - LoggerBroker.cpp:152]: Counter1 [0:0]:3
   [Information - LoggerBroker.cpp:152]: Time1 [0:0]:1500000

The State Machine started in the *INITIAL* state and moved to *STATE1*, launching the *AppState1*, which starts prompting the *Counter1* and *Time1* signals, which are associated to the GAM *GAMDisplay1*, that is only used in AppState1.

If while running in that state, we send a message to the State Machine calling a function defined in that state, it will execute it. In this case, we can call the *GOTOSTATE2* function, which is an event of *STATE1*. For that, we can use the following command in a different terminal: ::

   echo -e "Destination=StateMachine\nNextState=GOTOSTATE2" | nc 127.0.0.1 24680

The output will change to something similar to that: ::

   [Information - TCPSocketMessageProxyExample.cpp:116]: Connection accepted!
   ...
   [ParametersError - TCPSocketMessageProxyExample.cpp:145]: Received configuration message [size=45]:Destination=StateMachine
   Function=GOTOSTATE2
   ...
   [Information - StateMachine.cpp:201]: Changing from state (STATE1) to state (STATE2)
   [Information - StateMachine.cpp:340]: In state (STATE1) triggered message (PrepareChangeToRUN2Msg)
   [Information - LinuxTimer.cpp:482]: Frequency found = 2.000000
   ...
   [Information - StateMachine.cpp:340]: In state (STATE1) triggered message (StopCurrentStateExecutionMsg)
   [Information - LoggerBroker.cpp:152]: Counter1 [0:0]:15
   [Information - LoggerBroker.cpp:152]: Time1 [0:0]:7500000
   [Information - StateMachine.cpp:340]: In state (STATE1) triggered message (StartNextStateExecutionMsg)
   [Information - StateMachine.cpp:340]: In state (STATE1) triggered message (StateChangeMsg)
   [Information - MessagePrompter.cpp:59]: State Changed!
   [Information - LoggerBroker.cpp:152]: Counter2 [0:0]:16
   [Information - LoggerBroker.cpp:152]: Time2 [0:0]:8000000
   [Information - LoggerBroker.cpp:152]: Counter2 [0:0]:17
   [Information - LoggerBroker.cpp:152]: Time2 [0:0]:8500000
   [Information - LoggerBroker.cpp:152]: Counter2 [0:0]:18

As we can see, the prompt confirms the reception of the message and the change of state. It finishes running the current state pending actions and then moves to the next state. We can confirm this happens because the terminal shows now signals *Counter2* and *Time2*, which are associated to AppState2.

Finally, we can send the following message: ::
   
   echo -e "Destination=StateMachine\nFunction=GOTOERROR" | nc 127.0.0.1 24680

With this, the output will read something like: ::

   [Information - TCPSocketMessageProxyExample.cpp:116]: Connection accepted!
   ...
   [ParametersError - TCPSocketMessageProxyExample.cpp:145]: Received configuration message [size=44]:Destination=StateMachine
   Function=GOTOERROR
   ...
   [Information - StateMachine.cpp:201]: Changing from state (STATE1) to state (ERROR)
   [Information - StateMachine.cpp:340]: In state (ERROR) triggered message (StopAppMsg)
   [Information - MessagePrompter.cpp:67]: Kill signal successfully sent!
   [Information - Bootstrap.cpp:69]: Application recieved SIGUSR1.

   [Information - Bootstrap.cpp:87]: Application successfully stopped.

   [NoError - MARTeApp.cpp:133]: Application terminated

Which, as expected, terminates the application by sending a kill signal as defined in the *Kill* function of the MessagePrompter.