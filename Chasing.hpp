#pragma once
#include "pico/types.h"
#include "LedSegment.hpp"

namespace Lights
{
	// Collection of scanning and chasing animations
	class Chasing
	{
	public:
		static uint TricolourChase(LedSegment *segment);
		static uint TheatreChase(LedSegment *segment);
		static uint TheatreChaseRainbow(LedSegment *segment);

	private:
		static uint TricolourChase(LedSegment *segment, Colour colour1, Colour colour2, Colour colour3);
	};
}