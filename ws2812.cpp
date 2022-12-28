/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "Colour.hpp"
#include "ws2812Strip.hpp"
#include "LedController.hpp"
#include "ColourWipe.hpp"
#include "BlinkAnim.hpp"
#include "Chasing.hpp"

using namespace Lights;

#define NUM_PIXELS 50

#define WS2812_PIN 10
#define OUTPUT_ENABLE_PIN 11

const Colour black = Colour::RGB(0, 0, 0);
const Colour red = Colour::RGB(0xff, 0, 0);
const Colour green = Colour::RGB(0, 0xff, 0);
const Colour blue = Colour::RGB(0, 0, 0xff);
const Colour white = Colour::RGB(0xff, 0xff, 0xff);
const Colour warmFluorescent = Colour::FromInt(0xFFF4E5); /* 0 K, 255, 244, 229 */

int pixelQueue = 0;
uint8_t colourCycle = 0;

void pattern_snakes(LedSegment *seg, uint t)
{
	for (uint i = 0; i < seg->NumLeds(); ++i)
	{
		uint x = (i + (t >> 1)) % 64;
		if (x < 10)
			seg->SetPixelColour(i, red);
		else if (x >= 15 && x < 25)
			seg->SetPixelColour(i, green);
		else if (x >= 30 && x < 40)
			seg->SetPixelColour(i, blue);
		else
			seg->SetPixelColour(i, black);
	}
	seg->Show();
}

void pattern_random(LedSegment *seg, uint t)
{
	if (t % 8)
		return;
	for (int i = 0; i < seg->NumLeds(); ++i)
		seg->SetPixelColour(i, Colour::FromInt(rand()));
	seg->Show();
}

void pattern_sparkle(LedSegment *seg, uint t)
{
	if (t % 8)
		return;
	for (int i = 0; i < seg->NumLeds(); ++i)
		seg->SetPixelColour(i, Colour::FromInt(rand() % 16 ? 0 : 0xffffff));
	seg->Show();
}

void pattern_greys(LedSegment *seg, uint t)
{
	int max = 100; // let's not draw too much current!
	t %= max;
	for (int i = 0; i < seg->NumLeds(); ++i)
	{
		seg->SetPixelColour(i, Colour::FromInt(t * 0x10101));
		if (++t >= max)
			t = 0;
	}
	seg->Show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
Colour Wheel(uint8_t WheelPos)
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85)
	{
		return Colour::RGB(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170)
	{
		WheelPos -= 85;
		return Colour::RGB(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return Colour::RGB(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(LedSegment *seg, uint t)
{
	// Clear the pixels set last time
	for (int i = 0; i < seg->NumLeds(); i += 3)
	{
		seg->SetPixelColour(i + pixelQueue, black); //  Set pixel's color (in RAM)
	}

	// Set the next set of pixels
	pixelQueue++;
	if (pixelQueue >= 3)
		pixelQueue = 0;

	for (int i = 0; i < seg->NumLeds(); i += 3)
	{
		seg->SetPixelColour(i + pixelQueue, Wheel(colourCycle)); //  Set pixel's color (in RAM)
	}

	seg->Show(); //  Update strip to match

	colourCycle++;
}

Lights::ws2812Strip leds(WS2812_PIN, NUM_PIXELS, 24);
void clear()
{
	leds.Fill(black);
}

void write()
{
	leds.Show();
}

void fillTest(Colour colour)
{
	clear();
	leds.SetPixelColour(0, colour);
	write();

	for (int index = 1; index < NUM_PIXELS; index++)
	{
		leds.SetPixelColour(index - 1, black);
		leds.SetPixelColour(index, colour);
		write();
	}
}

typedef void (*pattern)(LedSegment *seg, uint t);
const struct
{
	pattern pat;
	const char *name;
	uint delay;
} pattern_table[] = {
	//	{pattern_snakes, "Snakes!", 10},
	//	{pattern_random, "Random data", 10},
	//	{pattern_sparkle, "Sparkles", 10},
	//	{pattern_greys, "Greys", 10},
	{theaterChase, "Theatre", 200},
};

typedef uint (*animation)(LedSegment *seg);
const struct
{
	animation pat;
} animationTable[] = {
	{ColourWipe::WipeReverse},
	{ColourWipe::SweepRandom},
	{BlinkAnim::BlinkRandomRainbow},
	{Chasing::TheatreChaseRainbow},
	{Chasing::TricolourChase}};

int main()
{
	stdio_init_all();
	printf("WS2812 Smoke Test, using pin %d", WS2812_PIN);

	// Enable the level shifter output
	gpio_init(OUTPUT_ENABLE_PIN);
	gpio_set_dir(OUTPUT_ENABLE_PIN, GPIO_OUT);
	gpio_put(OUTPUT_ENABLE_PIN, 0);

	int t = 0;
	Colour testColour1 = Colour::RGB(0x7f, 0, 0);
	Colour testColour2 = Colour::RGB(0, 0x7f, 0);
	Colour testColour = testColour1;

	leds.SetBrightness(0x7f);

	LedSegment segmentStart(&leds, 0, NUM_PIXELS / 2);
	LedSegment segmentEnd(&leds, NUM_PIXELS / 2, NUM_PIXELS / 2);

	LedController controller(&leds);
	controller.SetSpeed(500);
	//	controller.SetAnimation(ColourWipe::WipeReverse);
	//  controller.SetAnimation(ColourWipe::SweepRandom);
	//	controller.SetAnimation(BlinkAnim::BlinkRandomRainbow);
	//  controller.SetAnimation(Chasing::TheatreChaseRainbow);
	//  controller.SetAnimation(Chasing::TricolourChase);
	//	controller.SetColours({blue, warmFluorescent});
	controller.SetColours({red, warmFluorescent, black});

	while (1)
	{
		int animation = rand() % count_of(animationTable);
		controller.SetAnimation(animationTable[animation].pat);

		// Let each pattern run for 20 seconds
		absolute_time_t runUntil = delayed_by_ms(get_absolute_time(), 20000);
		while (time_reached(runUntil) == false)
		//	while (true)
		{
			//		pattern_table[pat].pat(&segmentStart, t);
			//		pattern_table[pat].pat(&segmentEnd, t);
			//		sleep_ms(pattern_table[pat].delay);
			//		t += dir;
			controller.Animate();
		}

		/*
				fillTest(testColour);

				if (testColour.value == testColour1.value)
				{
					testColour = testColour2;
				}
				else
				{
					testColour = testColour1;
				}
		*/
	}
}
