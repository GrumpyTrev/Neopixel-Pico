#include "ColourWipe.hpp"

namespace Lights
{
	uint ColourWipe::Wipe(LedSegment *segment)
	{
		return Wipe(segment, segment->GetColour(0), segment->GetColour(1), false);
	}

	uint ColourWipe::WipeInverse(LedSegment *segment)
	{
		return Wipe(segment, segment->GetColour(1), segment->GetColour(0), false);
	}

	uint ColourWipe::WipeReverse(LedSegment *segment)
	{
		return Wipe(segment, segment->GetColour(0), segment->GetColour(1), true);
	}

	uint ColourWipe::WipeReverseInverse(LedSegment *segment)
	{
		return Wipe(segment, segment->GetColour(1), segment->GetColour(0), true);
	}

	/*
	 * Turns all LEDs after each other to a random color.
	 * Then starts over with another color.
	 */
	uint ColourWipe::WipeRandom(LedSegment *segment)
	{
		if (segment->StepCount() % segment->NumLeds() == 0)
		{ // aux_param will store our random color wheel index
			segment->SetAuxParam(AnimationHelper::RandomColourWheelIndex(segment->AuxParam()));
		}
		Colour color = AnimationHelper::ColourWheel(segment->AuxParam());
		return Wipe(segment, color, color, false) * 2;
	}

	/*
	 * Random color introduced alternating from start and end of strip.
	 */
	uint ColourWipe::SweepRandom(LedSegment *segment)
	{
		if (segment->StepCount() % segment->NumLeds() == 0)
		{ // aux_param will store our random color wheel index
			segment->SetAuxParam(AnimationHelper::RandomColourWheelIndex(segment->AuxParam()));
		}
		Colour color = AnimationHelper::ColourWheel(segment->AuxParam());
		return Wipe(segment, color, color, true) * 2;
	}

	uint ColourWipe::Wipe(LedSegment *segment, Colour colour1, Colour colour2, bool reverse)
	{
		uint numLeds = segment->NumLeds();
		uint stepCount = segment->StepCount();

		if (stepCount < numLeds)
		{
			segment->SetPixelColour(stepCount, colour1);
		}
		else
		{
			uint ledOffset = stepCount - numLeds;
			if (reverse == true)
			{
				segment->SetPixelColour(numLeds - ledOffset - 1, colour2);
			}
			else
			{
				segment->SetPixelColour(ledOffset, colour2);
			}
		}

		segment->SetStepCount((stepCount + 1) % (numLeds * 2));

		return (segment->Speed() / (numLeds * 2));
	}
}