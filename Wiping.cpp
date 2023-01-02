#include "Wiping.hpp"

namespace Lights
{
	/// @brief Wipe the LEDs in two passes, the first with colour 1 and the second with colour 2
	/// The wipes will run forwards only
	uint Wiping::Wipe(LedSegment *segment)
	{
		return Wipe(segment, segment->GetColour(0), segment->GetColour(1), false);
	}

	/// @brief Wipe the LEDs in two passes, the first with colour 2 and the second with colour 1
	/// The wipes will run forwards only
	uint Wiping::WipeInverse(LedSegment *segment)
	{
		return Wipe(segment, segment->GetColour(1), segment->GetColour(0), false);
	}

	/// @brief Wipe the LEDs in two passes, the first with colour 1 and the second with colour 2
	/// The wipes will run forwards and then backwards
	uint Wiping::Sweep(LedSegment *segment)
	{
		return Wipe(segment, segment->GetColour(0), segment->GetColour(1), true);
	}

	/// @brief Wipe the LEDs in two passes, the first with colour 2 and the second with colour 1
	/// The wipes will run forwards and then backwards
	uint Wiping::SweepInverse(LedSegment *segment)
	{
		return Wipe(segment, segment->GetColour(1), segment->GetColour(0), true);
	}

	/// @brief Wipe the LEDs with a random colour, changing the colour at the end of each pass.
	/// The wipe will run forwards only
	/// @param segment
	/// @return
	uint Wiping::WipeRandom(LedSegment *segment)
	{
		if (segment->StepCount() % segment->NumLeds() == 0)
		{
			// AuxParam will store the random color wheel index
			segment->SetAuxParam(AnimationHelper::RandomColourWheelIndex(segment->AuxParam()));
		}

		// Use the stored index to select the colour to wipe with
		Colour color = AnimationHelper::ColourWheel(segment->AuxParam());
		return Wipe(segment, color, color, false) * 2;
	}

	/// @brief Wipe the LEDs with a random colour, changing the colour at the end of each pass.
	/// The wipe will run forwards and then backwards along the LEDs
	/// @param segment
	/// @return
	uint Wiping::SweepRandom(LedSegment *segment)
	{
		// Is it time to specify a new colour
		if (segment->StepCount() % segment->NumLeds() == 0)
		{
			// AuxParam will store the random color wheel index
			segment->SetAuxParam(AnimationHelper::RandomColourWheelIndex(segment->AuxParam()));
		}

		// Use the stored index to select the colour to wipe with
		Colour color = AnimationHelper::ColourWheel(segment->AuxParam());
		return Wipe(segment, color, color, true) * 2;
	}

	/// @brief Perfrom two passes over the LEDs wiping in one colour and then the other
	/// @param segment
	/// @param colour1
	/// @param colour2
	/// @param reverse
	/// @return
	uint Wiping::Wipe(LedSegment *segment, Colour colour1, Colour colour2, bool reverse)
	{
		uint numLeds = segment->NumLeds();
		uint stepCount = segment->StepCount();

		// The stepCount runs from 0 to NumLeds * 2 - 1 enabling two passes over the LEDs
		// If on 1st pass then set the LED to colour 1
		if (stepCount < numLeds)
		{
			segment->SetPixelColour(stepCount, colour1);
		}
		else
		{
			// On the second pass. Either set the LEDs again in the new colour from the start
			// or from the end
			uint ledOffset = stepCount - numLeds;
			if (reverse == true)
			{
				// Set the LEDs from the end
				segment->SetPixelColour(numLeds - ledOffset - 1, colour2);
			}
			else
			{
				// Set the LEDs from the start
				segment->SetPixelColour(ledOffset, colour2);
			}
		}

		segment->SetStepCount((stepCount + 1) % (numLeds * 2));

		// The returned delay will run both passes in the defined speed
		return (segment->Speed() / (numLeds * 2));
	}
}