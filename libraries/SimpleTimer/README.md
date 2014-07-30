Simple Timer Arduino Library
============================

This is (yet another) simple library to launch timed actions.

It's based on millis(), thus it has 1 ms resolution.

It uses polling, so no guarantee can be made about the exact time when a callback is fired. For example, if you setup the library so that it calls a function every 2ms, but this function requires 5ms to complete, then you'll have an invocation every 5ms.

For applications where non-strict timing is enough, not using interrupts avoids potential problems with global variables shared between the interrupt service routine and the main program, and doesn't consune a hardware timer. 

This library is based on http://playground.arduino.cc/Code/SimpleTimer and adds the following function

int setVariableTimer(timer_callback f)
--------------------------------------

Call the function f repeatedly until signalled to stop. The callback function must be declared as long f(int). The integer argument to the callback function is the number of repetitions that has occurred, and the return value (in milliseconds) determines the length of time before the next callback function call. Note the first call to the callback function occurs immediately with an argument of 0. When the callback function returns a value of 0, the interval is deleted, therefore the value timerId is no longer valid.

Example
=======

The following example shows the use of this to simulate a momentary output pulse of 300ms that occurs 10 seconds after the timer is established.

long delayThenPressButton( int callNumber ) {

	if ( callNumber==0 ) {
		return 10000;
	} else if ( callNumber ==1 ) {
		digitalWrite(PIN,HIGH);
		return 300;
	} else if ( callNumber ==2 ) {
		digitalWrite(PIN,LOW);
		return 0;
	}	
}
timerId = timer.setTimer(delayThenPressButton);

See
===

http://playground.arduino.cc/code/FiniteStateMachine

Version History
===============

2.0 2014-07-30 Introduces the setTimer() method described above



