#pragma once

#ifndef __STARSABOVE_H__
#define __STARSABOVE_H__

#include "starsabove_entity.h"
#include "starsabove_ui_base.h"

typedef struct GameState_s
{
	UI_Element* currentClickable_ui;
	Entity* currentClickable_entity;

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
* @brief take in the keys and mouse states and perform actions based on them
* @return NULL on error
*/
void processKeys(Uint8 keys, Uint32 mouse);

/**
* @brief check to see if the mouse is hovering over any clickable entity
* @return 1 if hovering over a clickable, 0 if not, NULL on error
*/
Bool starsabove_hoverDetection(float mX, float mY);

/**
* @brief perform actions when left mouse button is clicked
* @return NULL on error
*/
void onClick_left();

/**
* @brief set up the game's test world
* @return NULL on error
*/
void test();

#endif
