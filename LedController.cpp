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

	/// Add a new LedSegment to the collection. If this is the first user segement then make it the
	/// segment
	int LedController::AddSegment(LedSegment *segment)
	{
		userSegments.push_back(segment);
		if (userSegments.size() == 1)
		{
			firstSegment = segment;
		}

		return userSegments.size();
	}

	/// @brief Set the animation type for the specified segment
	/// @param index
	/// @param animationFunction
	void LedController::SetAnimation(uint index, LedSegment::animationType animationFunction)
	{
		if (index < userSegments.size())
		{
			userSegments[index]->SetAnimation(animationFunction);
		}
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
}