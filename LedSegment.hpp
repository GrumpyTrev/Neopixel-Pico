#pragma once

#include "pico/types.h"
#include "LedStrip.hpp"
#include "Animation.hpp"
#include <vector>

namespace Lights
{
	/// @brief Represents a part of an LedStrip and allows pixel operations to be performed
	/// on that part independent of the rest of the strip.
	class LedSegment
	{
	public:
		// Definition of the signature for all Animation functions
		typedef uint (*animationType)(LedSegment *seg);

		// Maximum number of colours available for animation
		static const int MaxColours = 3;

		LedSegment(LedStrip *target, uint start, uint ledCount);
		Colour PixelColour(uint index);
		void SetPixelColour(uint index, Colour colour);
		void Show();
		uint NumLeds();
		void SetAnimation(animationType animationFunction);
		bool IsAnimated();
		void Animate();
		void Fill(Colour colour, uint first, uint count);
		void Fill(Colour colour);
		absolute_time_t NextTime();
		Colour GetColour(uint8_t index);
		void SetColours(std::vector<Colour> colours);
		uint StepCount();
		void SetStepCount(uint stepCount);
		uint Speed();
		void SetSpeed(uint speed);
		uint8_t AuxParam();
		void SetAuxParam(uint8_t param);
		uint CallCount();
		void SetCallCount(uint callCount);

	private:
		LedStrip *targetStrip;
		uint startIndex;
		uint numLeds;

		/// @brief The colours that can be specified to be used by the animation
		Colour colours[MaxColours];

		// The animation applied to this segment
		animationType animation = nullptr;

		// The next time that the animation associated with this segment can run
		absolute_time_t nextTime = nil_time;

		// A counter used by animations to keep track of where they are in an animation cycle
		uint animationStepCount = 0;

		// The interpretation of this depends on the actual animation being used
		// For most it is how long a complete animation cycle should take
		uint animationSpeed = 1000;

		// Auxillary animation parameter
		uint8_t auxParam = 0;

		// A counter incremented every time this animation is called
		uint animationCallCount = 0;
	};
}