#include "LedStrip.hpp"

namespace Lights
{

	LedStrip::LedStrip(uint numLeds) : numLeds(numLeds)
	{
		pixelData = new Colour[numLeds];
		Fill(Colour::RGB(0, 0, 0));
	}

	LedStrip::~LedStrip()
	{
		delete pixelData;
	}

	uint LedStrip::NumLeds()
	{
		return numLeds;
	}

	uint8_t LedStrip::Brightness()
	{
		return brightness;
	}

	void LedStrip::SetBrightness(uint8_t brightnessValue)
	{
		brightness = brightnessValue;
	}

	Colour LedStrip::PixelColour(uint index)
	{
		Colour pixelColor = Colour::RGBW(0, 0, 0, 0);
		if (index < numLeds)
		{
			pixelColor = pixelData[index];
		}

		return pixelColor;
	}

	void LedStrip::SetPixelColour(uint index, Colour colour)
	{
		if (index < numLeds)
		{
			pixelData[index] = colour;
		}
	}

	void LedStrip::Fill(Colour colour, uint first, uint count)
	{
		uint last = (first + count);
		if (last > numLeds)
		{
			last = numLeds;
		}
		for (uint i = first; i < last; i++)
		{
			pixelData[i] = colour;
		}
	}

	void LedStrip::Fill(Colour colour)
	{
		Fill(colour, 0, numLeds);
	}

	uint32_t LedStrip::ScalePixelData(uint32_t data, uint16_t scale)
	{
		uint32_t dataA = (data & 0x00FF00FF) * scale;
		uint32_t dataB = ((data >> 8) & 0x00FF00FF) * scale;
		return ((dataA >> 8) & 0x00FF00FF | (dataB & 0xFF00FF00));
	}
}