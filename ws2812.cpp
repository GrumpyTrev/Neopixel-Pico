#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "Colour.hpp"
#include "ws2812Strip.hpp"
#include "LedController.hpp"
#include "Wiping.hpp"
#include "Blinking.hpp"
#include "Chasing.hpp"
#include "Cycling.hpp"
#include "Animator.hpp"
#include "Twinkling.hpp"
#include "Fading.hpp"
#include "Scanning.hpp"

using namespace Lights;

#define NUM_PIXELS 50

#define WS2812_PIN 10
#define OUTPUT_ENABLE_PIN 11

const Colour black = Colour(0, 0, 0);
const Colour red = Colour(0xff, 0, 0);
const Colour green = Colour(0, 0xff, 0);
const Colour blue = Colour(0, 0, 0xff);
const Colour white = Colour(0xff, 0xff, 0xff);
const Colour rubyRed = Colour(224, 17, 95);
const Colour ochre = Colour(0xff4e20);
const Colour magentaPink = Colour(0xc70075);

const Colour warmFluorescent = Colour(0xFFF4E5); /* 0 K, 255, 244, 229 */

std::vector<Animator::AnimationItem> cycle =
	{
		{Wiping::Wipe, "Wipe", 2000, 0},
		{Wiping::WipeInverse, "Wipe Inverse", 2000, 0},
		{Wiping::Sweep, "Wipe Reverse", 1000, 0},
		{Wiping::SweepInverse, "Wipe Reverse Inverse", 1000, 0},
		{Wiping::WipeRandom, "Wipe Random", 1000, 0},
		{Wiping::SweepRandom, "Sweep Random", 1000, 0},
		{Blinking::Blink, "Blink", 2000, 50},
		{Blinking::BlinkRainbow, "Blink Rainbow", 2000, 50},
		{Blinking::BlinkRandomRainbow, "Blink Random Rainbow", 2000, 50},
		{Blinking::Stobe, "Strobe", 2000, 50},
		{Blinking::StobeRainbow, "Strobe Rainbow", 1000, 50},
		{Chasing::TricolourChase, "Tricolour Chase", 500, 16},
		{Chasing::TheatreChase, "Theatre Chase", 500, 16},
		{Chasing::TheatreChaseRainbow, "Theatre Chase Rainbow", 500, 16},
		{Cycling::CycleRainbow, "Cycle Rainbow", 20, 4}};

int main()
{
	stdio_init_all();

	// Enable the level shifter output
	gpio_init(OUTPUT_ENABLE_PIN);
	gpio_set_dir(OUTPUT_ENABLE_PIN, GPIO_OUT);
	gpio_put(OUTPUT_ENABLE_PIN, 0);

	Lights::ws2812Strip leds(WS2812_PIN, NUM_PIXELS);
	leds.SetBrightness(32);
	leds.ApplyGamma(true);

	printf("Gamma %d Brightness %d\n", leds.Gamma(), leds.Brightness());

	// Initialise the cycling controller
	LedController cyclingController(&leds);
	cyclingController.SetColours({red, warmFluorescent, black});
	Animator cyclingAnimator(&cyclingController);
	cyclingAnimator.SetAnimations(cycle);
	cyclingAnimator.SetCycling(true);

	// Initialise the multiple segment controller
	LedController multiSegmentController(&leds);

	std::vector<Colour> colours0 = {magentaPink, black};
	std::vector<Colour> colours1 = {black, ochre};

	LedSegment segments[] = {
		{LedSegment(&leds, 0, 5)},
		{LedSegment(&leds, 5, 5)},
		{LedSegment(&leds, 10, 5)},
		{LedSegment(&leds, 15, 5)},
		{LedSegment(&leds, 20, 5)},
		{LedSegment(&leds, 25, 5)},
		{LedSegment(&leds, 30, 5)},
		{LedSegment(&leds, 35, 5)},
		{LedSegment(&leds, 40, 5)},
		{LedSegment(&leds, 45, 5)}};

	for (uint segIndex = 0; segIndex < count_of(segments); ++segIndex)
	{
		LedSegment *segment = &segments[segIndex];
		multiSegmentController.AddSegment(segment);
		segment->SetSpeed(750);
		segment->SetAnimation(Blinking::Blink);
		segment->SetColours((segIndex % 2) == 0 ? colours0 : colours1);
	}
	Animator segmentAnimator(&multiSegmentController);

	LedController testController(&leds);
	Animator testAnimator(&testController);
	testAnimator.SetAnimations({{Twinkling::Twinkle, "Twinkle", 1000, 204}});

	LedController testController2(&leds);
	Animator testAnimator2(&testController2);
	testAnimator2.SetAnimations({{Fading::Fade, "Fade", 1000, 4}});
	testController2.SetColours({red, magentaPink});

	LedController testController3(&leds);
	Animator testAnimator3(&testController3);
	testAnimator3.SetAnimations({{Fading::FadeRainbow, "Fade rainbow", 2000, 1}});
	testController3.SetColours({black});

	LedController testController4(&leds);
	Animator testAnimator4(&testController4);
	testAnimator4.SetAnimations({{Scanning::ScanFull, "Scan full", 3000, 5},
								 {Scanning::ScanRandom, "Scan random", 3000, 5},
								 {Scanning::ScanDualRandom, "Scan dual random", 3000, 3},
								 {Scanning::ScanDualFull, "Scan dual full", 3000, 3}});
	testController4.SetColours({rubyRed, black});
	testAnimator4.SetCycling(true);

	while (1)
	{
		//		cyclingAnimator.Animate();
		//		segmentAnimator.Animate();
		//		testAnimator.Animate();
		//		testAnimator2.Animate();
		//		testAnimator3.Animate();
		testAnimator4.Animate();
	}
}
