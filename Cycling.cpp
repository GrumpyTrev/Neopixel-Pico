#include "Cycling.hpp"

namespace Lights
{
	/// @brief Cycle through the entire HSV colour space lighting the LEDs per call.
	/// @param segment
	/// @return
	uint Cycling::CycleRainbow(LedSegment *segment)
	{
		// Use a saved starting point fo the colour cycle.
		uint16_t firstPixelHue = segment->StepCount();
		Rainbow(segment, firstPixelHue, 1, 255, 255);

		// Change the starting point of the colour cycle for next time
		firstPixelHue += (segment->Options() == 0) ? 256 : segment->Options();

		segment->SetStepCount(firstPixelHue);

		return segment->Speed();
	}

	/// @brief Light the LEDs with colours from the HSV colour space. Each LED will be lit with a
	/// different colour from the space and the entire HSV space will sampled 'reps' times
	/// @param segment
	/// @param first_hue The starting point for getting colours from the HSV space. By changing this in
	/// each call a moving rainbow effect can be produced
	/// @param reps The number of time the entire HSV colour space is shown per string of LEDs
	/// @param saturation
	/// @param brightness
	void Cycling::Rainbow(LedSegment *segment, uint16_t first_hue, int8_t reps,
						  uint8_t saturation, uint8_t brightness)
	{
		uint numLeds = segment->NumLeds();

		for (uint index = 0; index < numLeds; index++)
		{
			uint16_t hue = first_hue + (index * reps * 65536) / numLeds;
			segment->SetPixelColour(index, Colour::ColourHSV(hue, saturation, brightness));
		}
	}
}