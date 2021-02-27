#pragma once

#ifndef __STARSABOVE_H__
#define __STARSABOVE_H__

#include "starsabove_entity.h"
#include "starsabove_ui_base.h"

typedef struct GameState_s
{
	void* currentClickable;

} GameState;

/**
* @brief Do stuff before the game loop begins
* @return NULL on error
*/
void prepare_game();

/**
* @brief The game loop itself, called from game.c
* @return NULL on error
*/
void starsabove_loop();

/**
* @brief check to see if the mouse is hovering over any clickable entity
* @return 1 if hovering over a clickable, 0 if not, NULL on error
*/
Bool starsabove_hoverDetection(float mX, float mY);

#endif
