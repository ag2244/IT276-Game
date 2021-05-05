#pragma once

#ifndef __GAMERESOURCES_H__
#define __GAMERESOURCES_H__

#include "simple_logger.h"

#include "gfc_audio.h"
#include "gfc_types.h"

typedef struct Menu_State;

typedef struct
{
	Bool _sent;

	char target_id[128];
	char sub_target_id[128];

	char command[128];
	char descriptor[128];
	int qty;

	Bool _menubase;
	struct Menu_State* menu_state;

	Sound* eventsound;

} Game_Event;

/*
* @brief Create a new game_event struct
* @param target_id The target entity's name
* @param sub_target_id Some target within the target entity if anything
* @param command The command to process
* @param descriptor The description to be used by the command, e.g. command = "Build" and descriptor = "Building"
* @param qty quantity
* @param menu_state The menu_state to be triggered, if any
* @param _menubase Whether (1) or not (0) the associated menu_state is a base menu
* @return NULL on error or a pointer to a new game event
*/
Game_Event* new_gameevent(char* target_id, char* sub_target_id, char* command, char* descriptor, int qty, struct Menu_State* menu_state, Bool _menubase);

/*
* @brief copy The data from one Game_Event to another
* @param dst Destination
* @param scr Source
*/
void gameevent_copy(Game_Event* dst, Game_Event* src);

/*
* @brief Free a game_event
* @param gameevent The Game_Event to free
*/
void gameevent_free(Game_Event* gameevent);

#endif