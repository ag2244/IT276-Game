#pragma once

#ifndef __GAMERESOURCES_H__
#define __GAMERESOURCES_H__

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

	struct Menu_State* menu_state;

} Game_Event;

/*
* @brief Create a new game_event struct
* @param target_id The target entity's name
* @param sub_target_id Some target within the target entity if anything
* @param command The command to process
* @param descriptor The description to be used by the command, e.g. command = "Build" and descriptor = "Building"
* @param qty quantity
* @param menu_state The menu_state to be triggered, if any
* @return NULL on error or a pointer to a new game event
*/
Game_Event* new_gameevent(char* target_id, char* sub_target_id, char* command, char* descriptor, int qty, struct Menu_State* menu_state);

#endif