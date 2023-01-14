#pragma once
#include "pico/types.h"
#include "LedSegment.hpp"
namespace Lights
{
	class Fading
	{
	public:
		static uint Fade(LedSegment *segment);
		static uint FadeRainbow(LedSegment *segment);

	private:
		static uint FadeBetween(LedSegment *segment, Colour source, Colour dest);
	};
}