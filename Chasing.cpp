#include "Chasing.hpp"

namespace Lights
{
	/// @brief Chase using the 3 colours from configuration
	/// @param segment
	/// @return
	uint Chasing::TricolourChase(LedSegment *segment)
	{
		return TricolourChase(segment, segment->GetColour(0), segment->GetColour(1), segment->GetColour(2));
	}

	/// @brief Chase using 2 colours from configuration
	/// @param segment
	/// @return
	uint Chasing::TheatreChase(LedSegment *segment)
	{
		return TricolourChase(segment, segment->GetColour(0), segment->GetColour(1), segment->GetColour(1));
	}

	/// @brief Chase using 2 colours from configuration and a colour from the colour wheel
	/// @param segment
	/// @return
	uint Chasing::TheatreChaseRainbow(LedSegment *segment)
	{
		// Increment the colour selector index. This will wrap at 256
		segment->SetAuxParam(segment->AuxParam() + 1);
		return TricolourChase(segment, AnimationHelper::ColourWheel(segment->AuxParam()), segment->GetColour(1), segment->GetColour(1));
	}

	/// @brief Chase segments of leds in 3 colours.
	/// The size of the segments is specified by the Options value.
	/// The chase effect is provided by saving the index specifying the colour in the StepCount.
	/// The effect of the segment size seems to be related to the number of leds and is not
	/// obvious. With 50 leds a value of 16 or 17 works well.
	/// TODO is some more deterministic way of doing this
	/// @param segment
	/// @param colour1
	/// @param colour2
	/// @param colour3
	/// @return
	uint Chasing::TricolourChase(LedSegment *segment, Colour colour1, Colour colour2, Colour colour3)
	{
		uint8_t sizeCnt = segment->Options();
		uint8_t sizeCnt2 = sizeCnt + sizeCnt;
		uint8_t sizeCnt3 = sizeCnt2 + sizeCnt;
		uint16_t index = segment->StepCount();

		for (uint16_t i = 0; i < segment->NumLeds(); i++, index++)
		{
			// Keep index mod(sizeCnt3)
			if (index >= sizeCnt3)
			{
				index = 0;
			}

			// Set the pixel colours from the end of the strip
			segment->SetPixelColour(segment->NumLeds() - i - 1,
									(index < sizeCnt) ? colour1 : ((index < sizeCnt2) ? colour2 : colour3));
		}

		segment->SetStepCount(index);

		return (segment->Speed() / 16);
	}
}