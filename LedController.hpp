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
		int AddSegment(LedSegment *segment);
		inline void SetAnimation(LedSegment::animationType animationFunction)
		{
			firstSegment->SetAnimation(animationFunction);
		};
		void SetAnimation(uint index, LedSegment::animationType animationFunction);
		void Animate();
		inline void SetSpeed(uint speed) { firstSegment->SetSpeed(speed); };
		inline void SetColours(std::vector<Colour> colours) { firstSegment->SetColours(colours); };
		inline uint Speed() { return firstSegment->Speed(); };
		inline void ToggleGamma() { targetStrip->ApplyGamma(!targetStrip->Gamma()); };
		inline bool Gamma() { return targetStrip->Gamma(); };
		inline void SetOptions(uint8_t value) { firstSegment->SetOptions(value); }
		inline void SetBrightness(uint8_t brightness) { firstSegment->SetBrightness(brightness); };

	private:
		LedStrip *targetStrip;

		std::vector<LedSegment *> userSegments;
		LedSegment *defaultSegment;
		LedSegment *firstSegment;
	};
}
