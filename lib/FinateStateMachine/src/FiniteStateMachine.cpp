/*
* @Author: jerrytron
* @Date:   2015-04-21 20:11:51
* @Last Modified by:   jerrytron
* @Last Modified time: 2015-05-09 12:15:50
*/

#include "FiniteStateMachine.h"

/* Public Methods */

State::State( void (*aUpdateFunction)() ) {
	_stateEnter = 0;
	_stateUpdate = aUpdateFunction;
	_stateExit = 0;
}

State::State( void (*aEnterFunction)(), void (*aUpdateFunction)(), void (*aExitFunction)() ) {
	_stateEnter = aEnterFunction;
	_stateUpdate = aUpdateFunction;
	_stateExit = aExitFunction;
}

void State::enter() {
	if (_stateEnter) {
		_stateEnter();
	}
}

void State::update() {
	if (_stateUpdate) {
		_stateUpdate();
	}
}

void State::exit() {
	if (_stateExit) {
		_stateExit();
	}
}

/* Private Methods */


FiniteStateMachine::FiniteStateMachine(State& aCurrent){
	_triggerEnter = true;
	_currentState = _nextState = &aCurrent;
	_stateChangeTime = 0;
}

FiniteStateMachine& FiniteStateMachine::update() {
	if (_triggerEnter) {
		_currentState->enter();
		_triggerEnter = false;
	} else {
		if (_currentState != _nextState){
			immediateTransitionTo(*_nextState);
		}
		_currentState->update();
	}
	return *this;
}

FiniteStateMachine& FiniteStateMachine::transitionTo(State& aState){
	_nextState = &aState;
	_stateChangeTime = millis();
	return *this;
}

FiniteStateMachine& FiniteStateMachine::immediateTransitionTo(State& aState){
	_currentState->exit();
	_currentState = _nextState = &aState;
	_currentState->enter();
	_stateChangeTime = millis();
	return *this;
}

State& FiniteStateMachine::getCurrentState() {
	return *_currentState;
}

bool FiniteStateMachine::isInState(State &aState) const {
	if (&aState == _currentState) {
		return true;
	} else {
		return false;
	}
}

unsigned long FiniteStateMachine::timeInCurrentState() {
	millis() - _stateChangeTime;
}