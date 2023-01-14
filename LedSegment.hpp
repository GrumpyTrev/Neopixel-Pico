#pragma once

#include "pico/types.h"
#include "LedStrip.hpp"
#include "AnimationHelper.hpp"
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
		inline Colour PixelColour(uint index) { return targetStrip->PixelColour(index + startIndex); };
		inline void SetPixelColour(uint index, Colour colour)
		{
			targetStrip->SetPixelColour(index + startIndex, colour);
		};
		inline void Show() { targetStrip->Show(); };
		void Animate();
		void Fill(Colour colour, uint first, uint count);
		inline void Fill(Colour colour) { Fill(colour, 0, numLeds); };
		Colour GetColour(uint8_t index);
		void SetColours(std::vector<Colour> colours);
		void SetAnimation(animationType animationFunction);

		inline uint NumLeds() { return numLeds; };
		inline bool IsAnimated() { return (animation != nullptr); };
		inline absolute_time_t NextTime() { return nextTime; };
		inline uint32_t StepCount() { return animationStepCount; };
		inline void SetStepCount(uint32_t stepCount) { animationStepCount = stepCount; };
		inline uint Speed() { return animationSpeed; };
		inline void SetSpeed(uint speed) { animationSpeed = speed; };
		inline uint8_t AuxParam() { return auxParam; };
		inline void SetAuxParam(uint8_t param) { auxParam = param; };
		inline uint CallCount() { return animationCallCount; };
		inline void SetCallCount(uint callCount) { animationCallCount = callCount; };
		inline uint8_t Options() { return options; };
		inline void SetOptions(uint8_t value) { options = value; };
		inline void SetBrightness(uint8_t brightness) { targetStrip->SetBrightness(brightness); };

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
		uint32_t animationStepCount = 0;

		// The interpretation of this depends on the actual animation being used
		// For most it is how long a complete animation cycle should take
		uint animationSpeed = 1000;

		// Auxillary animation parameter typically used as a counter
		uint8_t auxParam = 0;

		// Options parameter, animation specific
		uint8_t options = 0;

		// A counter incremented every time this animation is called
		uint animationCallCount = 0;
	};
}