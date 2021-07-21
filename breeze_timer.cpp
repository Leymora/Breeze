#include "breeze_timer.h"

Breeze_Timer::Breeze_Timer()
{
	startTicks = 0;
	pausedTicks = 0;

	started = false;
	paused = false;
}

void Breeze_Timer::start()
{
	this->started = true;
	this->paused = false;

	this->startTicks = SDL_GetTicks();
	this->pausedTicks = 0;
}

void Breeze_Timer::stop()
{
	this->started = false;
	this->paused = false;

	this->startTicks = 0;
	this->pausedTicks = 0;
}

void Breeze_Timer::pause()
{
	if (this->started == true && this->paused == false)
	{
		this->paused = true;
		this->pausedTicks = (SDL_GetTicks() - this->startTicks); // Calculate paused ticks
		this->startTicks = 0; // Reset the started ticks
	}

}

void Breeze_Timer::resume()
{
	if (this->started == true && this->paused == true)
	{
		this->paused = false;
		this->startTicks = (SDL_GetTicks() - this->pausedTicks); // Calculate what the started ticks should be
		this->pausedTicks = 0; // Reset the paused ticks
	}	
}

Uint32 Breeze_Timer::restart()
{
	Uint32 timeToReturn = getTicks();

	this->startTicks = SDL_GetTicks();
	this->pausedTicks = 0;

	return timeToReturn;

}

Uint32 Breeze_Timer::getTicks() const
{
	Uint32 timeToReturn = 0;

	// Check if the timer is even fuggin started
	if (this->started == true)
	{
		if (this->paused == true)
		{
			timeToReturn = this->pausedTicks;
		}
		else
		{
			timeToReturn = (SDL_GetTicks() - this->startTicks);
		}
	}

	return timeToReturn;
}

bool Breeze_Timer::isStarted() const
{
	return this->started;
}

bool Breeze_Timer::isPaused() const
{
	return this->paused && this->started;
}
