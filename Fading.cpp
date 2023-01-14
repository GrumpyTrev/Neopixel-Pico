#include "Fading.hpp"
#include "AnimationHelper.hpp"
// #include <stdio.h>

namespace Lights
{
	uint Fading::Fade(LedSegment *segment)
	{
		return Fading::FadeBetween(segment, segment->GetColour(0), segment->GetColour(1));
	}

	uint Fading::FadeRainbow(LedSegment *segment)
	{
		// Every half full cycle, i.e. when StepCount is 256; Choose another colour index to fade from
		if ((segment->StepCount() == 300) || (segment->CallCount() == 0))
		{
			segment->SetAuxParam(AnimationHelper::RandomColourWheelIndex(segment->CallCount()));
		}

		return Fading::FadeBetween(segment, AnimationHelper::ColourWheel(segment->AuxParam()),
								   segment->GetColour(0));
	}

	uint Fading::FadeBetween(LedSegment *segment, Colour source, Colour dest)
	{
		uint32_t lum = segment->StepCount();
		lum += segment->Options();
		if (lum > 511)
		{
			lum = 0;
		}

		// If the destination is off (0, 0, 0) then skip some of the middle 'lum' values
		// from ( 200 - 300 ) as typically the LEDs will appear off in those values
		if (dest.value == 0)
		{
			if ((lum > 200) && (lum < 300))
			{
				lum = 300;
			}
		}
		segment->SetStepCount(lum);

		if (lum > 255)
		{
			lum = 511 - lum; // lum = 0 -> 255 -> 0
		}

		Colour color = Colour::Blend(source, dest, lum);
		segment->Fill(color);

		//		printf("R %d G %d B %d L %d\n", color.component.red, color.component.green, color.component.blue, lum);

		return (segment->Speed() / 128);
	}
}