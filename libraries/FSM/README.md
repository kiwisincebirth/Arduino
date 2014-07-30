Finite State Machine Arduino Library
====================================

A FSM serves as a manager that organizes a set of states, or behaviors. It manages the transition between states, and the state itself.

The transitions might be caused by the user, a wall or something other that is external, and they might be caused by some internal logic. Transitions either happen immediately, or they are deferred to the next update. The latter is the most common. It's used because that way you'll know that all code related to the current state, will have executed on the same state.

See
===
http://playground.arduino.cc/code/FiniteStateMachine

Version History
===============

2.1 2014-07-30 Introduces these changes :-

 * Added Comparison Operator for State object allowing for direct == comparison
 * Added a FSM property LastState (getLastState) to track prior state state, 
   typically used during an Enter callback function for conditional logic.
 * Added a method to get NextState, typically called from an Exit callback function
   to allow for conditional functionality based on next state
 
 Internal Changes include :-
 * Definition of callback function is now as typedefs
 * Changed the reset of "Time In Current State" counter to coincide with 
   calling the Enter callback Function. Previously it was being reset before
   the exit callback function was being called
 * Constructors of State, now use proper C++ initialisation.

2.0 2013-02-08: LINK: Versions fixed for Arduino 1.0x Terry King http://arduino-info.wikispaces.com/HAL-LibrariesUpdates

1.8 2010-02-04: Fixed a bug, missing return type, thanks to Rick Howard

1.7 2010-03-08: Fixed a bug, constructor ran update, thanks to René Pressé

1.6 2010-03-08: Added timeInCurrentState() , requested by sendhb

1.5 2009-11-29: Fixed a bug that was introduced by the last fix, thanks to Jon Hylands again

1.4 2009-11-29: The first state now calls it's enter(), thank you Jon Hylands for bringing this bug to my attention.

1.3 2009-11-01: Added getCurrentState and isInState, requested by Henry Herman 

1.2 2009-05-18: Enter and exit bug fix

1.1 2009-05-18: Added support for cascaded calls

1.0 2009-04-11: Beta
