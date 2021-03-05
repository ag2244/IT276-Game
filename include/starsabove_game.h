#pragma once

#ifndef __STARSABOVE_H__
#define __STARSABOVE_H__

#include "simple_json.h"

#include "starsabove_entity.h"
#include "starsabove_ui_base.h"
#include "starsabove_system.h"

typedef struct GameState_s
{
	UI_Element* currentClickable_ui;
	Entity* currentClickable_entity;

} GameState;

/**
* @brief Load all the systems in a game
* @return NULL on error, 1 if successful
*/
int load_systems(SJson* game);

/**
* @brief Load a game from a filename
* @return NULL on error
*/
void load_game(char* filename);

/**
* @brief Start a new game
* @return NULL on error
*/
void new_game();

/**
* @brief Turn the game into a json file and save it
* @return NULL on error
*/
void save_game(char* savefile_name);

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
