#pragma once
#include "pico/stdlib.h"

namespace Lights
{
	struct Colour
	{
		union
		{
			struct
			{
				uint8_t blue;
				uint8_t green;
				uint8_t red;
				uint8_t white;
			} component;

			uint32_t value;
		};

		static Colour RGB(uint8_t red, uint8_t green, uint8_t blue)
		{
			return RGBW(red, green, blue, 0x00);
		}

		static Colour RGBW(uint8_t red, uint8_t green, uint8_t blue, uint8_t white)
		{
			Colour newColour;

			newColour.component.red = red;
			newColour.component.green = green;
			newColour.component.blue = blue;
			newColour.component.white = white;

			return newColour;
		}

		static Colour FromInt(uint32_t colourValue)
		{
			Colour newColour;
			newColour.value = colourValue;

			return newColour;
		}

		/*
			static Colour HSV(uint8_t hue, uint8_t saturation, uint8_t value)
			{
				if (saturation == 0)
				{
					return (struct Colour){.red = value, .green = value, .blue = value, .white = 0};
				}
				uint8_t quadrant = hue / 43;
				uint8_t remainder = (hue - (quadrant * 43)) * 6;
				uint8_t p = (value * (255 - saturation)) >> 8;
				uint8_t q = (value * (255 - ((saturation * remainder) >> 8))) >> 8;
				uint8_t t = (value * (255 - ((saturation * (255 - remainder)) >> 8))) >> 8;
				switch (quadrant)
				{
				case 0:
					return (struct Colour){.red = value, .green = t, .blue = p, .white = std::min(t, p)};
				case 1:
					return (struct Colour){.red = q, .green = value, .blue = p, .white = std::min(q, p)};
				case 2:
					return (struct Colour){.red = p, .green = value, .blue = t, .white = std::min(t, p)};
				case 3:
					return (struct Colour){.red = p, .green = q, .blue = value, .white = std::min(q, p)};
				case 4:
					return (struct Colour){.red = t, .green = p, .blue = value, .white = std::min(t, p)};
				default:
				case 5:
					return (struct Colour){.red = value, .green = p, .blue = q, .white = std::min(q, p)};
				}
			}
	*/

		/*
				static Colour MixColors(Colour colorA, Colour colorB, double ratio)
				{
					return {
						.red = (uint8_t)((double)colorA.red * ratio + (double)colorB.red * (1.0 - ratio)),
						.green = (uint8_t)((double)colorA.green * ratio + (double)colorB.green * (1.0 - ratio)),
						.blue = (uint8_t)((double)colorA.blue * ratio + (double)colorB.blue * (1.0 - ratio)),
						.white = (uint8_t)((double)colorA.white * ratio + (double)colorB.white * (1.0 - ratio)),
					};
				}
				*/
	};
}