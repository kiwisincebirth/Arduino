/*
||
|| @file FiniteStateMachine.h
|| @version 1.8
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making finite state machines
|| | 
|| | Version 1.8 introduces these changes :-
|| | * Added Comparison Operator for State object allowing for direct == comparison
|| | * Added a FSM property LastState (getLastState) to track prior state state, 
|| |   typically used during an Enter callback function for conditional logic.
|| | * Added a method to get NextState, typically called from an Exit callback function
|| |   to allow for conditional functionality based on next state
|| | 
|| | Internal Changes include :-
|| | * Definition of callback function is now as typedefs
|| | * Changed the reset of "Time In Current State" counter to coincide with 
|| |   calling the Enter callback Function. Previously it was being reset before
|| |   the exit callback function was being called
|| | * Constructors of State, now use proper C++ initialisation.
|| | 
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
---------------------( COPY )---------------------
This libraries .h file has been updated by Terry King to make it compatible with Ardino 1.0x 

(Example 1.03) and also earlier versions like 0023

#include "WProgram.h"  
...has been replaced by:

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

terry@yourduino.com
02/07/2012
-----------------( END COPY )----------------------
||
*/

#ifndef FINITESTATEMACHINE_H
#define FINITESTATEMACHINE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define NO_ENTER (0)
#define NO_UPDATE (0)
#define NO_EXIT (0)

#define FSM FiniteStateMachine

//forward declarations
class State;

// definitions of callback for Enter and Exit, the parameter is 
// the state we transition away from, or state going to.

//typedef void (*fsmTransCallback)(State *state);

// definitions of callback for Update, milliseconds in the state
//
typedef void (*fsmUpdateCallback)(unsigned long milliseconds);

// definitions of callback for Update, milliseconds in the state

typedef void (*fsmCallback)();

//define the functionality of the states
class State {

	public:
		State( fsmCallback );
		State( fsmCallback, fsmCallback, fsmCallback );
		
		bool operator==(const State &other) const;
		bool operator!=(const State &other) const;
		
		void enter(); // we are entering from a prior state.
		void update(); // update in state, how long been in there. 
		void exit(); // we are leaving to go to a new state 
		
	private:
		fsmCallback userEnter;
		fsmCallback userUpdate;
		fsmCallback userExit;
		
};

//define the finite state machine functionality
class FiniteStateMachine {
	public:
		FiniteStateMachine(State& current);
		
		FiniteStateMachine& update();
		FiniteStateMachine& transitionTo( State& state );
		FiniteStateMachine& immediateTransitionTo( State& state );
		
		State& getLastState();
		State& getCurrentState();
		State& getNextState();
		boolean isInState( State &state ) const;
		boolean wasInState( State &state ) const;
		boolean willBeInState( State &state ) const;
		
		unsigned long timeInCurrentState();
		
	private:
		bool 	needToTriggerEnter;
		State*	lastState;
		State*	currentState;
		State* 	nextState;
		unsigned long stateChangeTime;
};

#endif

/*
|| @changelog
|| | 1.8 2013-10-10 - Mark Pruden : Callback enhancements, see above.
|| | 1.7 2010-03-08- Alexander Brevig : Fixed a bug, constructor ran update, thanks to René Pressé
|| | 1.6 2010-03-08- Alexander Brevig : Added timeInCurrentState() , requested by sendhb
|| | 1.5 2009-11-29- Alexander Brevig : Fixed a bug, introduced by the below fix, thanks to Jon Hylands again...
|| | 1.4 2009-11-29- Alexander Brevig : Fixed a bug, enter gets triggered on the first state. Big thanks to Jon Hylands who pointed this out.
|| | 1.3 2009-11-01 - Alexander Brevig : Added getCurrentState : &State
|| | 1.3 2009-11-01 - Alexander Brevig : Added isInState : boolean, requested by Henry Herman 
|| | 1.2 2009-05-18 - Alexander Brevig : enter and exit bug fix
|| | 1.1 2009-05-18 - Alexander Brevig : Added support for cascaded calls
|| | 1.0 2009-04-13 - Alexander Brevig : Initial Release
|| #
*/