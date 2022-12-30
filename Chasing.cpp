#include "Chasing.hpp"

namespace Lights
{
	uint Chasing::TricolourChase(LedSegment *segment)
	{
		return TricolourChase(segment, segment->GetColour(0), segment->GetColour(1), segment->GetColour(2));
	}

	uint Chasing::TheatreChase(LedSegment *segment)
	{
		return TricolourChase(segment, segment->GetColour(0), segment->GetColour(1), segment->GetColour(1));
	}

	uint Chasing::TheatreChaseRainbow(LedSegment *segment)
	{
		segment->SetAuxParam(segment->AuxParam() + 1);
		return TricolourChase(segment, AnimationHelper::ColourWheel(segment->AuxParam()), segment->GetColour(1), segment->GetColour(1));
	}

	uint Chasing::TricolourChase(LedSegment *segment, Colour colour1, Colour colour2, Colour colour3)
	{
#define SIZE_MEDIUM (uint8_t)0b00000100
		uint8_t sizeCnt = 1 << SIZE_MEDIUM;
		uint8_t sizeCnt2 = sizeCnt + sizeCnt;
		uint8_t sizeCnt3 = sizeCnt2 + sizeCnt;
		uint16_t index = segment->StepCount() % sizeCnt3;
		for (uint16_t i = 0; i < segment->NumLeds(); i++, index++)
		{
			index = index % sizeCnt3;

			segment->SetPixelColour(segment->NumLeds() - i - 1,
									(index < sizeCnt) ? colour1 : ((index < sizeCnt2) ? colour2 : colour3));
		}

		segment->SetStepCount(segment->StepCount() + 1);

		return (segment->Speed() / 16);
	}
}