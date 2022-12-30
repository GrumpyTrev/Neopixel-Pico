#include "LedSegment.hpp"
#include <stdexcept>

namespace Lights
{
	LedSegment::LedSegment(LedStrip *target, uint start, uint ledCount)
		: targetStrip(target), startIndex(start), numLeds(ledCount)
	{
		// Make sure that the startIndex and numLeds is valid for the targetStrip
		if ((startIndex + numLeds) > targetStrip->NumLeds())
		{
			throw std::invalid_argument("Invalid start index or count");
		}
	}

	void LedSegment::SetAnimation(animationType animationFunction)
	{
		animationStepCount = 0;
		animationCallCount = 0;
		auxParam = 0;
		animation = animationFunction;
	}

	void LedSegment::Animate()
	{
		if (IsAnimated())
		{
			nextTime = delayed_by_ms(get_absolute_time(), animation(this));
			animationCallCount++;
		}
	}

	void LedSegment::Fill(Colour colour, uint first, uint count)
	{
		uint last = (first + count);
		if (last > numLeds)
		{
			last = numLeds;
		}
		for (uint i = first; i < last; i++)
		{
			SetPixelColour(i, colour);
		}
	}

	Colour LedSegment::GetColour(uint8_t index)
	{
		Colour indexedColour;
		if (index < MaxColours)
		{
			indexedColour = colours[index];
		}

		return indexedColour;
	}

	void LedSegment::SetColours(std::vector<Colour> coloursToSet)
	{
		for (int index = 0; index < MaxColours && index < coloursToSet.size(); index++)
		{
			colours[index] = coloursToSet[index];
		}
	}
}