#include "Cycling.hpp"

namespace Lights
{
	uint Cycling::CycleRainbow(LedSegment *segment)
	{
		// strip.rainbow() can take a single argument (first pixel hue) or
		// optionally a few extras: number of rainbow repetitions (default 1),
		// saturation and value (brightness) (both 0-255, similar to the
		// ColorHSV() function, default 255), and a true/false flag for whether
		// to apply gamma correction to provide 'truer' colors (default true).
		uint16_t firstPixelHue = segment->StepCount();
		Rainbow(segment, firstPixelHue, 1, 255, 255);

		// This will wrap at 65626
		firstPixelHue += 256;

		segment->SetStepCount(firstPixelHue);

		return segment->Speed();
	}

	void Cycling::Rainbow(LedSegment *segment, uint16_t first_hue, int8_t reps,
						  uint8_t saturation, uint8_t brightness)
	{
		for (uint16_t i = 0; i < segment->NumLeds(); i++)
		{
			uint16_t hue = first_hue + (i * reps * 65536) / segment->NumLeds();
			segment->SetPixelColour(i, Colour::ColourHSV(hue, saturation, brightness));
		}
	}
}