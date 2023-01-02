#include "Twinkling.hpp"
#include <stdlib.h>

namespace Lights
{
	// Backing store for per-pixel values.
	// Must work out how to make this configurable
	static uint8_t heat[50];

	uint Twinkling::Twinkle(LedSegment *segment)
	{
		uint numLeds = segment->NumLeds();
		uint8_t seeds;

		// Step 1. Create a random number of seeds
		AnimationHelper::Random16AddEntropy(rand()); // random8() isn't very random, so this mixes things up a bit
		seeds = AnimationHelper::Random8(10, numLeds - 10);

		// Step 2. "Cool" down every location on the strip a little
		for (int i = 0; i < numLeds; i++)
		{
			heat[i] = AnimationHelper::QSub8(heat[i], 20);
		}

		// Step 3. Make the seeds into heat on the string
		for (int j = 0; j < seeds; j++)
		{
			// again, we have to mix things up so the same locations don't always light up
			AnimationHelper::Random16AddEntropy(rand());
			heat[AnimationHelper::Random8(numLeds)] = AnimationHelper::Random8(50, 255);
		}

		// Step 4. Add some "flicker" to LEDs that are already lit
		//         Note: this is most visible in dim LEDs
		for (int k = 0; k < numLeds; k++)
		{
			if ((heat[k] > 0) && (AnimationHelper::Random8() < 50))
			{
				heat[k] = AnimationHelper::QAdd8(heat[k], 10);
			}
		}

		// Check if a sparkle is required
		if (segment->CallCount() % 300 == 0)
		{
			seeds = AnimationHelper::Random8(numLeds - 20, numLeds);

			for (int i = 0; i < seeds; i++)
			{
				{
					int pos = AnimationHelper::Random8(numLeds);
					AnimationHelper::Random16AddEntropy(rand());
					heat[pos] = AnimationHelper::Random8(127, 255);
				}
			}
		}

		// Step 5. Map from heat cells to LED colors
		for (int j = 0; j < numLeds; j++)
		{
			segment->SetPixelColour(j, AnimationHelper::HeatColor(heat[j]));
			//			segment->SetPixelColour(j, Colour::ColourHSV(9000, 204, heat[j]));
		}

		return segment->Speed() / 30; // assign the next time Twinkle() should happen
	}
}