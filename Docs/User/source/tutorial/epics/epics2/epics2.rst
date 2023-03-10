EPICS example 2
---------------

After having seen the previous example, next we will see an application which monitors some outputs of the system using EPICSv3. Depending on the state, the application will store data in a file.

This are the Real Time components for this example:

.. image:: ./epics2_RT1.png
  :width: 800
  :alt: RT components 1

.. image:: ./epics2_RT2.png
  :width: 800
  :alt: RT components 2


  

To execute this example, follow the following instructions:

We will need 3 different terminals. In console1 execute the following commands: ::

  cd ~/Projects/MARTe2-demos-padova/Configurations
  softIoc -d EPICSv3-demo.db


This will start the EPICS database EPICSv3-dema.db. For additiona information on EPICS, please reffer to `EPICS documentation <https://docs.epics-controls.org/projects/how-tos/en/latest/index.html>`_

In console2 execute: ::

  cd ~/Projects/MARTe2-demos-padova/Startup
  ./Main.sh -l RealTimeLoader -f ../Configurations/RTApp-EPICSv3-2.cfg -m StateMachine:START

And finally, in console3 execute: ::

  camonitor MARTE2-DEMO-APP:HIST-IDLE-CT
