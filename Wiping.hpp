#pragma once
#include "pico/types.h"
#include "LedSegment.hpp"

namespace Lights
{
	class Wiping
	{
	public:
		static uint Wipe(LedSegment *segment);
		static uint WipeInverse(LedSegment *segment);
		static uint Sweep(LedSegment *segment);
		static uint SweepInverse(LedSegment *segment);
		static uint WipeRandom(LedSegment *segment);
		static uint SweepRandom(LedSegment *segment);

	private:
		static uint Wipe(LedSegment *segment, Colour colour1, Colour colour2, bool reverse);
	};
}
