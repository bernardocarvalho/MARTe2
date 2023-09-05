Example 8: Messages between objects
-----------------------------------

The idea now is to implement a State Machine that reacts to the pressing of a key. We will program an application that reads keyboard inputs and moves the screen cursor. For doing that we will explore how GAMs and other objects can interact between themselves by using messages.

Let's start by taking a look to the configuration file: 
    
.. literalinclude:: Example8.cfg 
   :linenos:

First we see a definition of the already well known *ApplicationKiller* - see :doc:`Example4 <../Example4/Example4>` - and another object colled *NcursesInt*, which is based in the *NcursesInterface* class. This is a class depending on the external library ncurses that allows us to read characters from the keyboard and redraw the screen in text mode to show the cursor moving. You can see the details of its implementation in :download:`NcursesInterface.h <../../../../../../Projects/MARTe2-components/Source/Components/Interfaces/NcursesInterface/NcursesInterface.h>` and :download:`NcursesInterface.cpp <../../../../../../Projects/MARTe2-components/Source/Components/Interfaces/NcursesInterface/NcursesInterface.cpp>`.

Next we see the definition of the state machine that will control the execution of the application. Fundamentally, the state machine will stay in the *IDLE* state until a key for moving the cursor is detected. Then, we will swap to the *KEYPRESSED* state that will move the cursor accordingly and then come back to the *IDLE* state again.

The way to do this can be seen in more detail in the *+TestApp* part. It defines first two objects, one from class *KeyPressedGAM* that reads keys from the keyboard, and another one from class *KeyActionGAM* that will be in charge of moving the cursor as appropriate - well, technically *KeyPressedGAM* also moves the cursor to the same direction if the key pressed is not the correct one and we haven't reached the wall of the virtual window. 

These two GAMs will be executed in different states of the application as described in the *+State* section. As we can see, one state is for reading the key - which is controlled by the *IDLE* state of the state machine discussed before - and the other one to move the cursor - that will be called under the *KEYPRESSED* stated mentioned before as well.

In the definition of the GAMs we use a frequency of 100Hz, which is enough to have a good sampling of the keyboard pressings - you may play changing this value to see the behaviour of the application.

Going a bit deeper in the *KeyPressedGAM* definition - see :download:`KeyPressedGAM.h <../../../../../../Projects/MARTe2-components/Source/Components/GAMs/KeyPressedGAM/KeyPressedGAM.h>` and :download:`KeyPressedGAM.cpp <../../../../../../Projects/MARTe2-components/Source/Components/GAMs/KeyPressedGAM/KeyPressedGAM.cpp>` - we see that the *Execute()* function is in charge of asking for the key pressed if any, and then evaluate what to do with it. The way to do so is by sending a message to the *NcursesInt* object after having initialized it appropriatelly using a *ConfigurationDatabase* object with the information needed to call the appropriate function (*readKey()*) of the corresponding object (*NcursesInt*) as we can see in the following code: ::

   ConfigurationDatabase cdbMsg;
   cdbMsg.Purge();
   cdbMsg.Write("Destination", "NcursesInt");
   cdbMsg.Write("Function", "readKey");
   cdbMsg.CreateAbsolute("+Parameters");
   cdbMsg.Write("Class", "ConfigurationDatabase");
   cdbMsg.Write("param1", 0);  // we can use any value here. The idea is that key returns the answer to the message
   cdbMsg.MoveToAncestor(1u);  // This is needed as we went down one level in the previous functions

   ReferenceT<Message> msg0(GlobalObjectsDatabase::Instance()->GetStandardHeap());
   if (!msg0->Initialise(cdbMsg)) {
      REPORT_ERROR(ErrorManagement::FatalError, "Failed to initialise message");   
   }
   ErrorManagement::ErrorType err = MessageI::SendMessageAndWaitReply(msg0, this);
   if (!err.ErrorsCleared()) {
      REPORT_ERROR(ErrorManagement::FatalError, "Failed to send message");   
   } 

The parameter passed (*param1*) is used as a return value where the code of the key pressed will be stored when the message is replied by the *NcursesInt* object. It is important to define properly the message strucure, as otherwise we will get an error. 

After that, we get the parameter using the function *Get()* of the *ConfigurationDatabase* object and check if the value is valid: ::

   ReferenceT<ConfigurationDatabase> params = msg0->Get(0);
   if (params.IsValid()) {
      ...

Once this is done, we evaluate the different scenarios. If the return value is *ERR* - which is a predefined value in ncurses equivalent to -1 - we keep moving in the same direction without changing state. For that, we send a message to the NcursesInt *moveCursor()* function with the key code of the previous movement that we have stored in *previous_key*. 

.. note::
   Note the presence of the *uslepp(50000)* call. This defines the speed of the cursor in the screen. If not present, the 100Hz of sampling execution of the GAM would be too much to be able to see the cursor in the screen and we will probably have visualizing issues.

If the key code is 27 - ESC key - then we stop the application by sending a Kill message to *AppKiller* but after closing first the *ncurses* interface.

.. note::
   the *usleep(1000000)* call is needed to ensure that ncurses closes appropriatelly. If we don't do this, we may end up with unexpected behaviour in the terminal

If the key received in not the ESC key, we send a message to the state machine asking to change to the *KEYPRESSED* state, by calling the *GOTOKEYPRESSED* function of the *IDLE* state, and we also send the key code to the *Action()* function of the *KeyActionGAM* object - called *ActionKeyGAM* - with another message. The *Action()* function basically keeps the value in the private variable *actionKey* of the class so the next time the *Execute()* function is called it is already available for it to use it.

When the *KeyActionGAM* object is executed, it basically asks through a message to the NcursesInt object to move the cursor as required and then sends another message to the state machine to come back to the *IDLE* state and restart the control cycle.

You can take a look to the code of *KeyActionGAM* in :download:`KeyActionGAM.h <../../../../../../Projects/MARTe2-components/Source/Components/GAMs/KeyActionGAM/KeyActionGAM.h>` and :download:`KeyActionGAM.cpp <../../../../../../Projects/MARTe2-components/Source/Components/GAMs/KeyActionGAM/KeyActionGAM.cpp>`.

Executing the example, we will be able to move the cursor through the screen with the letters 'a', 's', 'd' and 'w' is displayed.

.. note::

   1. Remember that to be able to receive messages from another component, the object receiving needs to inhering from MARTe::MessageI, and that it should implement a filter to manage the messages. The message should trigger the appropriate function of the object using the correct number and type of parameters. In the example, the parameters also act as a way to get the response to the message, by means of passing them by reference so the function can alter their value. For more information check :doc:`Messages <../../core/messages/messages>`.
   2. The *Execute()* function of each GAM is executed at every cycle, byt it does not mean that we cannot invoque another funcition of a GAM outside its periodic execution. The example here is the *Action()* function of the *KeyActionGAM*, that updates the member variable with the value of the last key pressed in order for the execution to use it.
   3. If another object is expected to be using a function, we need to declare it with the *CLASS_METHOD_REGISTER()* macro, and it has to be defined in the form **MARTe::ErrorManagement::ErrorType** function-name(parameters);