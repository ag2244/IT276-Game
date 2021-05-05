#pragma once

#ifndef __STARSABOVE_H__
#define __STARSABOVE_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

#include "gfc_color.h"

#include "simple_json.h"

#include "starsabove_game_resources.h"

#include "starsabove_entity.h"

#include "starsabove_ui_base.h"
#include "starsabove_ui_textbox.h"

#include "starsabove_text.h"

typedef enum
{
	SA_MAIN_MENU = 0,
	SA_INGAME,
	SA_PAUSED,
	SA_LEVEL_EDITOR
}
gameStatus;

typedef struct GameState_s
{
	int turn;
	char playerNation[128];

	UI_Element* currentClickable_ui;
	Entity* currentClickable_entity;

	Menu_State* player_menustate;

	Bool _hasevent;
	Game_Event frame_event;

	gameStatus status;

} GameState;

/**
* @brief Load a game dictionary from a filename
*/
void load_gamerule(char* filename);

/**
* @brief Load a game from a filename
*/
void load_gamefile(char* filename);

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

/*
* @brief Initialize the UI_Element that is the new turn button
*/
UI_Element* newturnbutton_init();

/*
* @brief Do stuff before the game loop begins
*/
void prepare_game();

/**
* @brief Load up a game and set up for play
* @return NULL on error
*/
void load_game(char filename[128]);

/**
* @brief The game loop itself, called from game.c
* @return NULL on error
*/
void starsabove_loop();

/**
* @brief End the turn and begin a new one
*/
void newTurn();

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
* @brief Go through all game objects that may be listening for an event
*/
void event_relay();

/**
* @brief perform actions when left mouse button is clicked
* @return NULL on error
*/
void onClick_left();

/**
* @brief called on program exit
*/
void starsabove_exit();

void test();

void test_ui();

void testcmd();

#endif
