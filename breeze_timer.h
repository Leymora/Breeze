#pragma once
#include <SDL/SDL.h>

class Breeze_Timer
{
public:
	Breeze_Timer();

	void start();
	void stop();
	void pause();
	void resume();
	Uint32 restart();

	Uint32 getMilliseconds() const;

	bool isStarted() const;
	bool isPaused() const;

private:
	Uint32 startTicks = 0; // The ticks since the timer started
	Uint32 pausedTicks = 0; // Time stored when paused

	// The two statuses of the timer
	bool started = false;
	bool paused = false;
};