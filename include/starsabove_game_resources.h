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

Game_Event* new_gameevent(char* target_id, char* sub_target_id, char* command, char* descriptor, int qty, struct Menu_State* menu_state);

#endif