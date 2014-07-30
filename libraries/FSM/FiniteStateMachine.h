/*
||
|| @file FiniteStateMachine.h
|| @version 2.1 - by Mark Pruden
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making finite state machines
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
|| | 2.1 2014-07-30 - Mark Pruden : See LINK: https://github.com/kiwisincebirth/Arduino/tree/master/libraries/FSM
|| | 2.0 2013-02-08 - LINK: Versions fixed for Arduino 1.0x Terry King http://arduino-info.wikispaces.com/HAL-LibrariesUpdates
|| | 1.8 2010-02-04 - Fixed a bug, missing return type, thanks to Rick Howard
|| | 1.7 2010-03-08 - Alexander Brevig : Fixed a bug, constructor ran update, thanks to René Pressé
|| | 1.6 2010-03-08 - Alexander Brevig : Added timeInCurrentState() , requested by sendhb
|| | 1.5 2009-11-29 - Alexander Brevig : Fixed a bug, introduced by the below fix, thanks to Jon Hylands again...
|| | 1.4 2009-11-29 - Alexander Brevig : Fixed a bug, enter gets triggered on the first state. Big thanks to Jon Hylands who pointed this out.
|| | 1.3 2009-11-01 - Alexander Brevig : Added getCurrentState : &State
|| | 1.3 2009-11-01 - Alexander Brevig : Added isInState : boolean, requested by Henry Herman 
|| | 1.2 2009-05-18 - Alexander Brevig : enter and exit bug fix
|| | 1.1 2009-05-18 - Alexander Brevig : Added support for cascaded calls
|| | 1.0 2009-04-13 - Alexander Brevig : Initial Release
|| #
*/