#include <stdio.h>
#include "Animator.hpp"
#include "Commander.hpp"

namespace Lights
{
	void Animator::ToggleCycling()
	{
		// If there are no animations then keep cycling false
		if (animationTable.size() == 0)
		{
			printf("No animations to cycle\n");
		}
		else
		{
			cycling = !cycling;
			printf("Cycling %s \n", cycling ? "True" : "False");

			if (cycling == true)
			{
				// Let each pattern run or 20 seconds
				runUntil = delayed_by_ms(get_absolute_time(), 20000);
			}
		}
	}

	void Animator::NextAnimation()
	{
		if (animationTable.size() > 0)
		{
			if (++animationIndex >= animationTable.size())
			{
				animationIndex = 0;
			}
			SelectAnimation();
		}
	}

	void Animator::SelectAnimation()
	{
		if (animationTable.size() > 0)
		{
			AnimationItem item = animationTable[animationIndex];
			controller->SetAnimation(item.animationMethod);
			controller->SetSpeed(item.speed);
			controller->SetOptions(item.options);

			printf("%s Speed %d Options %d\n", item.name, item.speed, item.options);

			if (cycling == true)
			{
				// Let each pattern run or 20 seconds
				runUntil = delayed_by_ms(get_absolute_time(), 20000);
			}
		}
	}

	void Animator::ToggleGamma()
	{
		controller->ToggleGamma();
		printf("Gamma %s \n", controller->Gamma() ? "True" : "False");
	}

	void Animator::SetSpeed(uint speed)
	{
		if (animationTable.size() > 0)
		{
			animationTable[animationIndex].speed = speed;
		}
		controller->SetSpeed(speed);
		printf("Speed %d\n", speed);
	}

	void Animator::SetOptions(uint8_t options)
	{
		if (animationTable.size() > 0)
		{
			animationTable[animationIndex].options = options;
		}
		controller->SetOptions(options);
		printf("Options %d\n", options);
	};

	void Animator::Animate()
	{
		// Commander used to let the user interact
		Commander commandPoller;

		if (animationTable.size() > 0)
		{
			animationIndex = 0;
			SelectAnimation();

			// Turn off cycling if there is only one animation
			if (animationTable.size() == 0)
			{
				cycling = false;
			}
		}
		else
		{
			cycling = false;
		}

		// Make sure a previous exit instruction is turned off
		exitAnimation = false;
		while (exitAnimation == false)
		{
			while ((TimeReached() == false) && (commandPoller.CommandAvailable() == false))
			{
				controller->Animate();
				commandPoller.GetCommand();
			}

			if (commandPoller.CommandAvailable() == true)
			{
				commandPoller.ProcessCommand(this);
			}
			else if (cycling == true)
			{
				NextAnimation();
			}
		}
	}
}