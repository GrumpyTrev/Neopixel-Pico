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

	Colour LedSegment::PixelColour(uint index)
	{
		return targetStrip->PixelColour(index + startIndex);
	}

	void LedSegment::SetPixelColour(uint index, Colour colour)
	{
		targetStrip->SetPixelColour(index + startIndex, colour);
	}

	void LedSegment::Show()
	{
		targetStrip->Show();
	}

	uint LedSegment::NumLeds()
	{
		return numLeds;
	}

	void LedSegment::SetAnimation(animationType animationFunction)
	{
		animation = animationFunction;
	}

	bool LedSegment::IsAnimated()
	{
		return (animation != nullptr);
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

	void LedSegment::Fill(Colour colour)
	{
		Fill(colour, 0, numLeds);
	}

	absolute_time_t LedSegment::NextTime()
	{
		return nextTime;
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

	uint LedSegment::StepCount()
	{
		return animationStepCount;
	}

	void LedSegment::SetStepCount(uint stepCount)
	{
		animationStepCount = stepCount;
	}

	uint LedSegment::Speed()
	{
		return animationSpeed;
	}

	void LedSegment::SetSpeed(uint speed)
	{
		animationSpeed = speed;
	}

	uint8_t LedSegment::AuxParam()
	{
		return auxParam;
	}

	void LedSegment::SetAuxParam(uint8_t param)
	{
		auxParam = param;
	}

	uint LedSegment::CallCount()
	{
		return animationCallCount;
	}

	void LedSegment::SetCallCount(uint callCount)
	{
		animationCallCount = callCount;
	}
}