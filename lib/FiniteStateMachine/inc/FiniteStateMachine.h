#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#ifdef PARTICLE
#include "Particle.h"
#endif

class State
{
	public:
		/* Public Methods */
		State( void (*aUpdateFunction)() );
		State( void (*aEnterFunction)(), void (*aUpdateFunction)(), void (*aExitFunction)() );

		void enter();
		void update();
		void exit();

		/* Public Variables */

	private:
		/* Private Methods */
		void (*_stateEnter)();
		void (*_stateUpdate)();
		void (*_stateExit)();

		/* Private Variables */

};

class FiniteStateMachine {
	public:
		/* Public Methods */
		FiniteStateMachine(State& aCurrent);

		FiniteStateMachine& update();
		FiniteStateMachine& transitionTo(State& aState);
		FiniteStateMachine& immediateTransitionTo(State& aState);

		State& getCurrentState();
		bool isInState(State& aState) const;

		uint32_t timeInCurrentState();

		/* Public Variables */

	private:
		/* Private Variables */
		bool _triggerEnter;
		State* _currentState;
		State* _nextState;
		uint32_t _stateChangeTime;
};

#endif