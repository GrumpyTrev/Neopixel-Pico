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
#include "Blinking.hpp"
#include "Chasing.hpp"
#include "Cycling.hpp"

using namespace Lights;

#define NUM_PIXELS 50

#define WS2812_PIN 10
#define OUTPUT_ENABLE_PIN 11

const Colour black = Colour(0, 0, 0);
const Colour red = Colour(0xff, 0, 0);
const Colour green = Colour(0, 0xff, 0);
const Colour blue = Colour(0, 0, 0xff);
const Colour white = Colour(0xff, 0xff, 0xff);
const Colour warmFluorescent = Colour(0xFFF4E5); /* 0 K, 255, 244, 229 */

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
		seg->SetPixelColour(i, Colour(rand()));
	seg->Show();
}

void pattern_sparkle(LedSegment *seg, uint t)
{
	if (t % 8)
		return;
	for (int i = 0; i < seg->NumLeds(); ++i)
		seg->SetPixelColour(i, Colour(rand() % 16 ? 0 : 0xffffff));
	seg->Show();
}

void pattern_greys(LedSegment *seg, uint t)
{
	int max = 100; // let's not draw too much current!
	t %= max;
	for (int i = 0; i < seg->NumLeds(); ++i)
	{
		seg->SetPixelColour(i, Colour(t * 0x10101));
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
		return Colour(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170)
	{
		WheelPos -= 85;
		return Colour(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return Colour(WheelPos * 3, 255 - WheelPos * 3, 0);
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

Lights::ws2812Strip leds(WS2812_PIN, NUM_PIXELS);
LedController controller(&leds);

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
struct
{
	animation pat;
	const char *name;
	int speedIndex;
} animationTable[] = {
	/*	{ColourWipe::Wipe, "Wipe", 4},
		{ColourWipe::WipeInverse, "Wipe Inverse", 4},
		{ColourWipe::WipeReverse, "Wipe Reverse", 3},
		{ColourWipe::WipeReverseInverse, "Wipe Reverse Inverse", 3},
		{ColourWipe::WipeRandom, "Wipe Random", 3},
		{ColourWipe::SweepRandom, "Sweep Random", 3},
		{Blinking::Blink, "Blink", 4},
		{Blinking::BlinkRainbow, "Blink Rainbow", 4},
		{Blinking::BlinkRandomRainbow, "Blink Random Rainbow", 4},
		{Blinking::Stobe, "Strobe", 4},
		{Blinking::StobeRainbow, "Strobe Rainbow", 3},
		{Chasing::TricolourChase, "Tricolour Chase", 2},
		{Chasing::TheatreChase, "Theatre Chase", 2},
		{Chasing::TheatreChaseRainbow, "Theatre Chase Rainbow", 2},
		*/
	{Cycling::CycleRainbow, "Cycle Rainbow", 0}};

#define NO_COMMAND 0
#define NEXT_ANIMATION 1
#define TOGGLE_CYCLE 2
#define SPEED 3
#define TOGGLE_GAMMA 4

int command = NO_COMMAND;
const struct
{
	int speed;
	const char *name;
} speeds[] = {{100, "X fast 100"}, {250, "Very fast 250"}, {500, "Fast 500"}, {1000, "Normal 1000"}, {2000, "Slow 2000"}};

absolute_time_t runUntil;
int animationIndex = 0;
bool cycling = true;

void GetCommand()
{
	int input;
	do
	{
		input = getchar_timeout_us(0);
		if (input != PICO_ERROR_TIMEOUT)
		{
			putchar(input);
			if ((input == 'N') || (input == 'n'))
			{
				command = NEXT_ANIMATION;
			}
			else if ((input == 'C') || (input == 'c'))
			{
				command = TOGGLE_CYCLE;
			}
			else if ((input == 'S') || (input == 's'))
			{
				command = SPEED;
			}
			else if ((input == 'G') || (input == 'g'))
			{
				command = TOGGLE_GAMMA;
			}
		}
	} while (input != PICO_ERROR_TIMEOUT);
}

void SetAnimation(int index)
{
	controller.SetAnimation(animationTable[index].pat);
	controller.SetSpeed(speeds[animationTable[index].speedIndex].speed);

	printf("%s Speed %s\n", animationTable[index].name,
		   speeds[animationTable[index].speedIndex].name);

	if (cycling == true)
	{
		// Let each pattern run for 20 seconds
		runUntil = delayed_by_ms(get_absolute_time(), 20000);
	}
}

void NextAnimation()
{
	if (++animationIndex >= count_of(animationTable))
	{
		animationIndex = 0;
	}
	SetAnimation(animationIndex);
}

bool TimeReached()
{
	return (cycling == true) && (time_reached(runUntil) == true);
}

int main()
{
	stdio_init_all();

	// Enable the level shifter output
	gpio_init(OUTPUT_ENABLE_PIN);
	gpio_set_dir(OUTPUT_ENABLE_PIN, GPIO_OUT);
	gpio_put(OUTPUT_ENABLE_PIN, 0);

	leds.SetBrightness(50);
	leds.ApplyGamma(true);
	printf("Speed %d Brightness %d\n", controller.Speed(), leds.Brightness());

	//	controller.SetColours({blue, warmFluorescent});
	controller.SetColours({red, warmFluorescent, black});
	SetAnimation(animationIndex);

	controller.SetSpeed(20);
	cycling = false;

	while (1)
	{
		while ((TimeReached() == false) && (command == NO_COMMAND))
		{
			controller.Animate();
			GetCommand();
		}

		if (command != NO_COMMAND)
		{
			if (command == NEXT_ANIMATION)
			{
				cycling = false;
				NextAnimation();
			}
			else if (command == TOGGLE_CYCLE)
			{
				cycling = !cycling;
				printf("Cycling %s \n", cycling ? "True" : "False");
			}
			else if (command == SPEED)
			{
				int speedIndex = animationTable[animationIndex].speedIndex;

				if (++speedIndex >= count_of(speeds))
				{
					speedIndex = 0;
				}
				animationTable[animationIndex].speedIndex = speedIndex;
				controller.SetSpeed(speeds[speedIndex].speed);
				printf("Speed %s\n", speeds[speedIndex].name);
			}
			else if (command == TOGGLE_GAMMA)
			{
				leds.ApplyGamma(!leds.Gamma());
				printf("Gamma %s \n", leds.Gamma() ? "True" : "False");
			}

			command = NO_COMMAND;
		}
		else if (cycling == true)
		{
			NextAnimation();
		}
	}
}
