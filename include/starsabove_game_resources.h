#pragma once

#ifndef __GAMEEVENT_H__
#define __GAMEEVENT_H__

typedef struct
{
	Bool _sent;

	char target_id[128];

	char command[128];
	char descriptor[128];
	int qty;

} Game_Event;

#endif