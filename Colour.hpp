#pragma once
#include "pico/stdlib.h"

namespace Lights
{
	struct Colour
	{
		union
		{
			// The colour as individual components
			// The order here is important. This order is for RGB leds.
			// The filler byte is making explicit what the compiler would do anyway, i.e. align
			// on a 32 bit boundary.
			struct
			{
				uint8_t blue;
				uint8_t green;
				uint8_t red;
				uint8_t filler;
			} component;

			// The seperate components mapped onto a 32 bit uint.
			// The order the compenents are mapped is |filler|red|green|blue| from MSB to LSB
			uint32_t value;
		};

		// Default constructor
		Colour() : value(0) {}

		// Create a Colour from a 32 bit RGB value
		Colour(uint32_t colourValue) : value(colourValue) {}

		// Create Colour from component RGB values
		Colour(uint8_t red, uint8_t green, uint8_t blue) : component{blue, green, red} {}

		static Colour ColourHSV(uint16_t hue, uint8_t sat, uint8_t val);

		static inline uint8_t Gamma8(uint8_t x) { return GammaTable[x]; }

		static uint32_t Gamma32(uint32_t rgbValue);

		static const uint8_t GammaTable[256];

		static Colour Blend(Colour source, Colour destination, uint8_t amount);
	};
}