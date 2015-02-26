# Arduino Ethernet with SS Pin Configuration

This library allows you to specify the SS (Slave Select) pin that the Ethernet (W5100) chip is connected to. This should be used typically when Pin 10 is being used for anther purpose o you need to assign a different pin.

To install this, the two files W5100.h & .cpp need to be copied into the main Arduino (inbuilt) /libraries/Ethernet/src/utility folder, replacing the existing files, no other files need to change

This code is based on Arduino 1.6.0

**Usage:** There are two changes you need to make in your code

1. Include W5100.h

'''
#include <Ethernet.h>
#include <utility/W5100.h> // <- Add this line of code
'''

2. Call select(pinNumber) prior to calling the init() method.

'''
W5100.select(8); // <- Specify the SS Pin to use for Ethernet
Ethernet.begin(mac,ipAddress); 
'''

Thats All.




