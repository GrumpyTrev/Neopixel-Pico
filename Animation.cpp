#include <stdlib.h>
#include <algorithm>
#include "Animation.hpp"

namespace Lights
{
	// Returns a new, random wheel index with a minimum distance of 42 from seed.
	// Stolen from  kitesurfer1404/WS2812FX
	uint8_t Animation::RandomColourWheelIndex(uint8_t seed)
	{
		uint8_t r = 0;
		uint8_t x = 0;
		uint8_t y = 0;
		uint8_t d = 0;

		while (d < 42)
		{
			r = Random8();
			x = abs(seed - r);
			y = 255 - x;
			d = std::min(x, y);
		}

		return r;
	}

	// Put a value 0 to 255 in to get a color value.
	// The colours are a transition r -> g -> b -> back to r
	// Inspired by the Adafruit examples.
	Colour Animation::ColourWheel(uint8_t wheelPos)
	{
		Colour colourValue;
		wheelPos = 255 - wheelPos;
		if (wheelPos < 85)
		{
			colourValue = Colour::RGB(255 - wheelPos * 3, 0, wheelPos * 3);
		}
		else if (wheelPos < 170)
		{
			wheelPos -= 85;
			colourValue = Colour::RGB(0, wheelPos * 3, 255 - wheelPos * 3);
		}
		else
		{
			wheelPos -= 170;
			colourValue = Colour::RGB(wheelPos * 3, 255 - wheelPos * 3, 0);
		}

		return colourValue;
	}

	// Fast 8-bit random number generator shamelessly borrowed from FastLED
	uint8_t Animation::Random8()
	{
		randomSeed16 = (randomSeed16 * 2053) + 13849;
		return (uint8_t)((randomSeed16 + (randomSeed16 >> 8)) & 0xFF);
	}

	uint16_t Animation::randomSeed16 = 0;
}