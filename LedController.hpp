#pragma once

#include <vector>
#include "pico/types.h"
#include "LedSegment.hpp"

namespace Lights
{
	/// @brief The LedController applies animation to one or more LedSegments.
	/// When first constructed an LedSegment is automatically created to cover all of the
	/// supplied LedTarget
	class LedController
	{
	public:
		LedController(LedStrip *target);
		void Animate();
		void SetAnimation(LedSegment::animationType animationFunction);
		void SetSpeed(uint speed);
		void SetColours(std::vector<Colour> colours);

	private:
		LedStrip *targetStrip;

		static const uint MaximumNumberOfSegments = 10;
		std::vector<LedSegment *> userSegments;
		LedSegment *defaultSegment;
		LedSegment *firstSegment;
	};
} // namespace Lights
