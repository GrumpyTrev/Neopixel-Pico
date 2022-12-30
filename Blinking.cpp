#include "Blinking.hpp"

namespace Lights
{
	// Standard half on/off blinking between two colours
	uint Blinking::Blink(LedSegment *segment)
	{
		return Blink(segment, segment->GetColour(0), segment->GetColour(1), false);
	};

	// Blinking half on/off between a colour from the Wheel and Colour(1)
	// The animation CallCount is used to step along the wheel
	uint Blinking::BlinkRainbow(LedSegment *segment)
	{
		return Blink(segment, AnimationHelper::ColourWheel(segment->CallCount() << 2),
					 segment->GetColour(1), false);
	};

	// Blinking half on/off between two colours from the wheel. Both selected at random
	uint Blinking::BlinkRandomRainbow(LedSegment *segment)
	{
		uint8_t coloutIndex1 = AnimationHelper::RandomColourWheelIndex(segment->CallCount() << 2);
		uint8_t coloutIndex2 = AnimationHelper::RandomColourWheelIndex(coloutIndex1);

		return Blink(segment, AnimationHelper::ColourWheel(coloutIndex1),
					 AnimationHelper::ColourWheel(coloutIndex2), false);
	}

	// Strobe Colour(0) on top of Colour(1)
	uint Blinking::Stobe(LedSegment *segment)
	{
		return Blink(segment, segment->GetColour(0), segment->GetColour(1), true);
	};

	// Stobe a rainbow colour on top of Colour(1)
	uint Blinking::StobeRainbow(LedSegment *segment)
	{
		return Blink(segment, AnimationHelper::ColourWheel(segment->CallCount() << 2),
					 segment->GetColour(1), true);
	};

	// Swap between two colours using the CallCount to select the colour
	// The duration of each colour depends on whether or not stobing is on
	// TODO alolw the stobe duration to be controlled
	uint Blinking::Blink(LedSegment *segment, Colour colour1, Colour colour2, bool strobe)
	{
		uint delay = 0;

		if (segment->CallCount() & 1)
		{
			segment->Fill(colour2);
			delay = strobe ? segment->Speed() - 50 : (segment->Speed() / 2);
		}
		else
		{
			segment->Fill(colour1);
			delay = strobe ? 50 : (segment->Speed() / 2);
		}

		return delay;
	}
}