#pragma once
#include "LedController.hpp"

namespace Lights
{
	class Animator
	{
	public:
		Animator(LedController *controller) : controller(controller){};

		typedef uint (*animation)(LedSegment *seg);
		struct AnimationItem
		{
			animation animationMethod;
			const char *name;
			uint speed;
			uint options;
		};

		void NextAnimation();
		inline void SetAnimations(std::vector<AnimationItem> animations)
		{
			animationTable.insert(animationTable.end(), animations.begin(), animations.end());
		};

		inline void AddAnimation(AnimationItem animation) { animationTable.push_back(animation); };
		void ToggleCycling();
		inline void SetCycling(bool valueToSet) { cycling = valueToSet; };
		void SelectAnimation();
		void ToggleGamma();
		inline uint Speed() { return animationTable[animationIndex].speed; };
		void SetSpeed(uint speed);
		void Animate();
		inline void SetCycle(bool value) { cycling = value; };
		inline void ExitAnimation() { exitAnimation = true; };
		void SetOptions(uint8_t options);

	private:
		/// @brief When cycling is enabled each animation is only allowed a certain time to run
		/// @return
		inline bool TimeReached() { return (cycling == true) && (time_reached(runUntil) == true); };

		/// The LedController used to carry out the animation actions
		LedController *controller = nullptr;

		/// @brief Collection of animation functions executed by this Animator
		std::vector<AnimationItem> animationTable;

		/// @brief Determines whether or not the animations are being cycled through
		bool cycling = false;

		/// Index into the animation table of the current animation being played
		uint animationIndex = 0;

		// When cycling the time at which the next animation will be selected
		absolute_time_t runUntil;

		// Flag used to get out of the Animate loop
		bool exitAnimation = false;
	};
}