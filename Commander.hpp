#pragma once
#include "Animator.hpp"

namespace Lights
{
	class Commander
	{
	public:
		void GetCommand();
		void ProcessCommand(Animator *animationTarget);
		bool CommandAvailable() { return command; };

	private:
#define NO_COMMAND 0
#define NEXT_ANIMATION 1
#define TOGGLE_CYCLE 2
#define SPEED 3
#define SPEED_FAST 4
#define TOGGLE_GAMMA 5
#define EXIT_ANIMATION 6
#define SET_OPTION 7

		// The single letter command that has been entered
		int command = NO_COMMAND;

		int numberEntered = -1;

		static uint const speeds[];
	};
}