#pragma once
#include "pico/types.h"
#include "Colour.hpp"

namespace Lights
{
	// Collection of miscellaneous functions used by the animation classes
	class Animation
	{
	public:
		static uint8_t RandomColourWheelIndex(uint8_t seed);
		static Colour ColourWheel(uint8_t pos);
		static uint8_t Random8();

	private:
		// 16 bit random number seed
		static uint16_t randomSeed16;
	};
}