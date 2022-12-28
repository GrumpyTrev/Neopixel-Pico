#pragma once

#include "pico/types.h"
#include "Colour.hpp"

namespace Lights
{
	/// @brief The LedStrip class represents a string of LEDs together with methods to
	/// set the colour of each individual and to show the entire string
	class LedStrip
	{
	public:
		LedStrip(uint numLeds);
		~LedStrip();

		uint NumLeds();
		uint8_t Brightness();
		void SetBrightness(uint8_t brightness);
		Colour PixelColour(uint index);
		void SetPixelColour(uint index, Colour colour);
		virtual void Show() = 0;
		void Fill(Colour colour, uint first, uint count);
		void Fill(Colour colour);

	protected:
		uint32_t ScalePixelData(uint32_t data, uint16_t scale);

		uint numLeds;
		uint8_t brightness = 255;

		// The Colour data for the entire string of LEDs
		Colour *pixelData;
	};
};
