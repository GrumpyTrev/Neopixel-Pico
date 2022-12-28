#include "BlinkAnim.hpp"

namespace Lights
{
	// Standard half on/off blinking between two colours
	uint BlinkAnim::Blink(LedSegment *segment)
	{
		return Blink(segment, segment->GetColour(0), segment->GetColour(1), false);
	}

	// Blinking half on/off between a colour from the Wheel and Colour(1)
	// The animation CallCount is used to step along the wheel
	uint BlinkAnim::BlinkRainbow(LedSegment *segment)
	{
		return Blink(segment, Animation::ColourWheel(segment->CallCount() << 2),
					 segment->GetColour(1), false);
	}

	// Blinking half on/off between two colours from the wheel. Both selected at random
	uint BlinkAnim::BlinkRandomRainbow(LedSegment *segment)
	{
		uint8_t index = Animation::RandomColourWheelIndex(segment->CallCount() << 2);

		return Blink(segment, Animation::ColourWheel(index),
					 Animation::ColourWheel(Animation::RandomColourWheelIndex(index)), false);
	}

	// Strobe Colour(0) on top of Colour(1)
	uint BlinkAnim::Stobe(LedSegment *segment)
	{
		return Blink(segment, segment->GetColour(0), segment->GetColour(1), true);
	}

	// Stobe a rainbow colour on top of Colour(1)
	uint BlinkAnim::StobeRainbow(LedSegment *segment)
	{
		return Blink(segment, Animation::ColourWheel(segment->CallCount() << 2),
					 segment->GetColour(1), true);
	}

	// Swap between two colours using the CallCount to select the colour
	// The duration of each colout depends on whether or not stobing is on
	// TODO alolw the stobe duration to be controlled
	uint BlinkAnim::Blink(LedSegment *segment, Colour colour1, Colour colour2, bool strobe)
	{
		uint delay = 0;

		if (segment->CallCount() & 1)
		{
			segment->Fill(colour2);
			delay = strobe ? segment->Speed() - 100 : (segment->Speed() / 2);
		}
		else
		{
			segment->Fill(colour1);
			delay = strobe ? 100 : (segment->Speed() / 2);
		}

		return delay;
	}
}