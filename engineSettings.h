#pragma once
#include <SDL/SDL.h>

enum class Coordinate_System
{
    BREEZE_ENGINE,
    OPENGL_STANDARD
};

enum class Game_State
{
    PLAYING,
    PAUSED
};

float SCREEN_WIDTH = 1280;
float SCREEN_HEIGHT = 720;
float TIMESCALE = 1.0f;

double FRAME_RATE = 60.0f;
double FRAME_TARGET = (1000.0f / FRAME_RATE);

bool WIREFRAME_MODE = false;
bool DEBUG_MODE = false;
SDL_bool IS_V_SYNC_ENABLED = SDL_FALSE;
