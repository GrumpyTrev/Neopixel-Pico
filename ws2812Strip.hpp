#pragma once
#include "hardware/pio.h"
#include "LedStrip.hpp"

namespace Lights
{
	class ws2812Strip : public LedStrip
	{
	public:
		ws2812Strip(PIO pioBlock, uint stateMachine, uint dataPin, uint numLeds, uint bitsPerPixel);
		ws2812Strip(uint dataPin, uint numLeds, uint bitsPerPixel);
		void Show();

	private:
		void InitialiseStrip();
		PIO pioBlock;
		uint stateMachine;
		uint dataPin;
		uint bitsPerPixel;
		absolute_time_t nextWriteAllowedTime;

		/// @brief The number of microseconds do allow the last set of data to be latched by the
		/// LEDs before sending some more
		const uint LedLatchTimeUs = 600;
	};
}