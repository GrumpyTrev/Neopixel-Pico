#include "pico/stdlib.h"
#include <stdio.h>
#include <ctype.h>
#include "Commander.hpp"

namespace Lights
{
	const uint Commander::speeds[] = {10, 20, 50, 100, 250, 500, 1000, 2000};

	/// @brief Get a single character command from the Uart
	void Commander::GetCommand()
	{
		int input;
		do
		{
			input = getchar_timeout_us(0);
			if (input != PICO_ERROR_TIMEOUT)
			{
				putchar(input);

				if ((isdigit(input) == true) && (command == NO_COMMAND))
				{
					// Add this digit to the number being built
					if (numberEntered == -1)
					{
						numberEntered = input - '0';
					}
					else
					{
						numberEntered = numberEntered * 10 + input - '0';
					}
				}
				if ((input == 'N') || (input == 'n'))
				{
					command = NEXT_ANIMATION;
				}
				else if ((input == 'C') || (input == 'c'))
				{
					command = TOGGLE_CYCLE;
				}
				else if (input == 's')
				{
					command = SPEED;
				}
				else if (input == 'S')
				{
					command = SPEED_FAST;
				}
				else if ((input == 'G') || (input == 'g'))
				{
					command = TOGGLE_GAMMA;
				}
				else if ((input == 'X') || (input == 'x'))
				{
					command = EXIT_ANIMATION;
				}
				else if ((input == 'O') || (input == 'o'))
				{
					command = SET_OPTION;
				}
			}
		} while (input != PICO_ERROR_TIMEOUT);
	}

	/// @brief If a command is available apply it to the supplied Animator
	/// @param animationTarget
	void Commander::ProcessCommand(Animator *animationTarget)
	{
		if (command != NO_COMMAND)
		{
			if (command == NEXT_ANIMATION)
			{
				animationTarget->SetCycling(false);
				animationTarget->NextAnimation();
			}
			else if (command == TOGGLE_CYCLE)
			{
				animationTarget->ToggleCycling();
			}
			else if ((command == SPEED) || (command == SPEED_FAST))
			{
				// Check first for a number
				if (numberEntered != -1)
				{
					animationTarget->SetSpeed(numberEntered);
				}
				else
				{
					// Map the current speed to an index in the speed table
					int speed = animationTarget->Speed();
					int speedIndex = 0;
					while ((speed > speeds[speedIndex]) && (++speedIndex < count_of(speeds)))
					{
					};

					if (++speedIndex >= count_of(speeds))
					{
						// Only allow the fast speeds if processing the SPEED_FAST command
						speedIndex = (command == SPEED_FAST) ? 0 : 3;
					}

					animationTarget->SetSpeed(speeds[speedIndex]);
				}
			}
			else if (command == TOGGLE_GAMMA)
			{
				animationTarget->ToggleGamma();
			}
			else if (command == EXIT_ANIMATION)
			{
				animationTarget->ExitAnimation();
			}
			else if (command == SET_OPTION)
			{
				// Only process if there is a preceeding number
				if (numberEntered != -1)
				{
					animationTarget->SetOptions(numberEntered);
				}
			}

			command = NO_COMMAND;
			numberEntered = -1;
		}
	}

}