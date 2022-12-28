#pragma once
#include "pico/types.h"
#include "LedSegment.hpp"

namespace Lights
{
	// Blinking and strobing animations
	class BlinkAnim
	{
	public:
		static uint Blink(LedSegment *segment);
		static uint BlinkRainbow(LedSegment *segment);
		static uint BlinkRandomRainbow(LedSegment *segment);
		static uint Stobe(LedSegment *segment);
		static uint StobeRainbow(LedSegment *segment);

	private:
		static uint Blink(LedSegment *segment, Colour colour1, Colour colour2, bool stobe);
	};
}