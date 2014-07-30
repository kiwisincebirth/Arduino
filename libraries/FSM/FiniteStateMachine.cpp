/*
||
|| @file FiniteStateMachine.cpp
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

#include "FiniteStateMachine.h" 

//FINITE STATE

State::State( fsmCallback update )
	: userEnter(0), userUpdate(update), userExit(0) { }

State::State( fsmCallback enter, fsmCallback update, fsmCallback exit )
	: userEnter(enter), userUpdate(update), userExit(exit) { }

//what to do when entering this state
void State::enter(){
	if (userEnter){
		userEnter();
	}
}

//what to do when this state updates
void State::update(){
	if (userUpdate){
		userUpdate();
	}
}

//what to do when exiting this state
void State::exit(){
	if (userExit){
		userExit();
	}
}

bool State::operator==(const State &other) const {
	return (
		this->userEnter == other.userEnter &&
		this->userUpdate == other.userUpdate &&
		this->userExit == other.userExit );
}

bool State::operator!=(const State &other) const {
	return !(*this == other);
}
    
//END FINITE STATE 

//FINITE STATE MACHINE

FiniteStateMachine::FiniteStateMachine(State& state)
	: needToTriggerEnter(true), currentState(&state), nextState(&state) {}

FiniteStateMachine& FiniteStateMachine::update() {
	//simulate a transition to the first state
	//this only happens the first time update is called
	if (needToTriggerEnter) { 
		stateChangeTime = millis(); // Reset The Time to coincide with call to ENTER
		currentState->enter(); 
		needToTriggerEnter = false;
	} else {
		if (currentState != nextState){
			immediateTransitionTo(*nextState);
		}
		currentState->update();
	}
	return *this;
}

FiniteStateMachine& FiniteStateMachine::transitionTo(State& state){
	nextState = &state;
	// Want the Exit callback function to get full time in current state
	// stateChangeTime = millis(); 
	return *this;
}

FiniteStateMachine& FiniteStateMachine::immediateTransitionTo(State& state){
	nextState = &state; // When we Call Exit, want to ensure next state is set
	currentState->exit();
	lastState = currentState;
	currentState = nextState;
	stateChangeTime = millis(); // Reset The Time to coincide with call to ENTER
	currentState->enter();
	return *this;
}

//return the last state
State& FiniteStateMachine::getLastState() {
	return *lastState;
}

//return the current state
State& FiniteStateMachine::getCurrentState() {
	return *currentState;
}

//return the next state
State& FiniteStateMachine::getNextState() {
	return *nextState;
}

//check if state is equal to the currentState
boolean FiniteStateMachine::isInState( State &state ) const {
	return &state == currentState;
}

boolean FiniteStateMachine::wasInState( State &state ) const {
	return &state == lastState;
}

boolean FiniteStateMachine::willBeInState( State &state ) const {
	return &state == nextState;
}

unsigned long FiniteStateMachine::timeInCurrentState() { 
	return millis() - stateChangeTime; 
}

//END FINITE STATE MACHINE
