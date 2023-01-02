#pragma once
#include "pico/types.h"
#include "LedSegment.hpp"

namespace Lights
{
	class Twinkling
	{
	public:
		static uint Twinkle(LedSegment *segment);
	};
}