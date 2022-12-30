#pragma once
#include "pico/types.h"
#include "LedSegment.hpp"

namespace Lights
{
	class Cycling
	{
	public:
		static uint CycleRainbow(LedSegment *segment);

	private:
		static void Rainbow(LedSegment *segment, uint16_t first_hue, int8_t reps,
							uint8_t saturation, uint8_t brightness);
	};
}
