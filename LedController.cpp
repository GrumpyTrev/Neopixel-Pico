#include "LedController.hpp"

namespace Lights
{
	/// @brief LedController constructor. Save the target LedStrip and create an LedSegment for it.
	/// @param target
	LedController::LedController(LedStrip *target) : targetStrip(target)
	{
		defaultSegment = new LedSegment(targetStrip, 0, targetStrip->NumLeds());
		firstSegment = defaultSegment;
	}

	/// @brief Iterate through the collecton of LedSegment and aniamate any of those that are
	/// ready to run.
	void LedController::Animate()
	{
		// Assume that no animation runs and therefore no show is required
		bool showRequired = false;

		// If there are no user segments then just check the default segment covering all
		// the target
		if (userSegments.size() == 0)
		{
			if ((defaultSegment->IsAnimated() == true) &&
				(time_reached(defaultSegment->NextTime()) == true))
			{
				defaultSegment->Animate();
				showRequired = true;
			}
		}
		else
		{
			for (int segIndex = 0; segIndex < userSegments.size(); segIndex++)
			{
				LedSegment *segment = userSegments[segIndex];
				if ((segment->IsAnimated() == true) && (time_reached(segment->NextTime()) == true))
				{
					segment->Animate();
					showRequired = true;
				}
			}
		}

		if (showRequired == true)
		{
			targetStrip->Show();
		}
	}

	void LedController::SetAnimation(LedSegment::animationType animationFunction)
	{
		firstSegment->SetAnimation(animationFunction);
	}

	void LedController::SetSpeed(uint speed)
	{
		firstSegment->SetSpeed(speed);
	}

	void LedController::SetColours(std::vector<Colour> colours)
	{
		firstSegment->SetColours(colours);
	}
}